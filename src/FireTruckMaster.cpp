#include <master/firetruck.h>

// Setup function that only runs once when the Arduino is powered on - ctm
void setup()
{
    Wire.begin();       // join i2c bus
    Serial.begin(9600); // start serial for output
    // block while waiting for character over serial
    while (!Serial)
    {
    }

    // Uncomment for Debug message - ctm
    // Serial.println("Starting Fire Truck Master Test");

#if !defined(__MIPSEL__)
    while (!Serial)
        ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
    if (Usb.Init() == -1)
    {
        Serial.print(F("\r\nOSC did not start"));
        while (1)
            ; // Halt
    }

    Serial.print(F("\r\nGameController Bluetooth Library Started"));

    // Initialize the old state of the firetruck - ctm
    firetruck.oldState = fireTruckStates::still;

    // Initialize the old state of the left stick - ctm
    truckMotorEscControlStick.oldState = leftStickStates::leftStickNeutral;

    // Initialize the old state of the right stick - ctm
    truckSteeringServoControlStick.oldState = servoControlStickStates::rightStickNeutral;

    // set the angles for the motor and servo
    truckMovementAngles.motor = 90;
    truckMovementAngles.servo = 90;
    

  SpeedCon.attach(speedControllerPin); // attaches the speed controller on pin 10
  SpeedCon.write(truckMovementAngles.motor);
  delay(600);
  SteeringServo.attach(servoPin); // attaches the speed controller on pin 10
  SpeedCon.write(truckMovementAngles.servo);
  delay(600);
}

/********************************************************************************************************************/

// Loop function that runs over and over again - ctm
void loop()
{
    // keep track of the current time
    truckControlTimes.current = millis();

    Usb.Task();
    if (GameController.connected())
    {

        // If the X button has been hit, play the second sound - ctm
        if (GameController.getButtonClick(CROSS))
        {
            sendData(TruckControlData.SoundTwo, dummyData);
        }

        // If the triangle button has been hit, play the first sound - ctm
        if (GameController.getButtonClick(TRIANGLE))
        {
            sendData(TruckControlData.SoundOne, dummyData);
        }

        // If the circle button has been hit, stop the current sound from playing - ctm
        if (GameController.getButtonClick(CIRCLE))
        {
            sendData(TruckControlData.SoundStop, dummyData);
        }

        // If the square button has been hit, toggle the water pump - ctm
        if (GameController.getButtonClick(SQUARE))
        {
            sendData(TruckControlData.ToggleWaterPump, dummyData);
        }
        if (GameController.getButtonClick(R3))
        {
            sendData(TruckControlData.ServoStraight, dummyData);
        }

        // call these functions to get and set the current state of the control sticks
        setSteeringServoState();
        setMotorState();
    }
}

/********************************************************************************************************************/

// send data over I2C interface to slave
void sendData(char data, char secondMovementChar)
{
    Serial.print("Sending: ");

    Wire.beginTransmission(I2CAddress); // Transmit to device
    if (sendMovementData)
    {
        Serial.println(data);
        Serial.println(secondMovementChar);
        Wire.write(TruckControlData.MovementData);
        Wire.write(data);
        Wire.write(secondMovementChar);
        sendMovementData = false;
    }
    else
    {
        Serial.println(data);
        Wire.write(data); // Send serial data
    }
    Wire.endTransmission(); // Stop transmitting
}

/********************************************************************************************************************/

// This function updates both the old/new state of both analog sticks - ctm
void getState()
{

}

/********************************************************************************************************************/

// This function controls the firetruck - ctm
void fireTruckControl()
{

    // If the firetruck state is set to still - ctm
    if (firetruck.newState == fireTruckStates::still)
    {
        sendData(TruckControlData.MotorStop, TruckControlData.MotorStop);

        // If the firetruck state is set to forward to left - ctm
    }
    else if (firetruck.newState == fireTruckStates::forwardToLeft)
    {
        sendData(TruckControlData.MotorForward, TruckControlData.ServoLeft);

        // If the firetruck state is set to forward to right - ctm
    }
    else if (firetruck.newState == fireTruckStates::forwardToRight)
    {
        sendData(TruckControlData.MotorForward, TruckControlData.ServoRight);

        // If the firetruck state is set to forward - ctm
    }
    else if (firetruck.newState == fireTruckStates::forward)
    {
        sendData(TruckControlData.MotorForward, TruckControlData.MotorBackward);

        // If the firetruck state is set to backward to left - ctm
    }
    else if (firetruck.newState == fireTruckStates::backwardToLeft)
    {
        sendData(TruckControlData.MotorBackward, TruckControlData.ServoLeft);

        // If the firetruck state is set to backward to right - ctm
    }
    else if (firetruck.newState == fireTruckStates::backwardToRight)
    {
        sendData(TruckControlData.MotorBackward, TruckControlData.ServoRight);

        // If the firetruck state is set to backward - ctm
    }
    else if (firetruck.newState == fireTruckStates::backward)
    {
        sendData(TruckControlData.MotorBackward, TruckControlData.MotorBackward);

        // If the firetruck state is set to left - ctm
    }
    else if (firetruck.newState == fireTruckStates::left)
    {
        sendData(TruckControlData.ServoLeft, TruckControlData.ServoLeft);

        // If the firetruck state is set to right - ctm
    }
    else if (firetruck.newState == fireTruckStates::right)
    {
        sendData(TruckControlData.MotorStop, TruckControlData.ServoRight);
    }
    else if (firetruck.newState == fireTruckStates::straight)
    {
        sendData(TruckControlData.MotorStop, TruckControlData.ServoStraight);
    }
}

/********************************************************************************************************************/

// This function combines the states of the left and right stick - ctm
void combineStates()
{

    // If the new left stick state doesn't match to the old one - ctm
    if (truckMotorEscControlStick.newState != truckMotorEscControlStick.oldState)
    {
        // Debug message- ctm
        // Serial.println("Left stick new state differs from the old one");

        switch (truckMotorEscControlStick.newState)
        {

        // If the left stick state is set to up - ctm
        case leftStickStates::leftStickUp:

            // If the right stick state is set to left - ctm
            if (truckSteeringServoControlStick.newState == servoControlStickStates::rightStickLeft)
            {
                firetruck.newState = fireTruckStates::forwardToLeft;
                break;

                // If the right stick state is set to right - ctm
            }
            else if (truckSteeringServoControlStick.newState == servoControlStickStates::rightStickRight)
            {
                firetruck.newState = fireTruckStates::forwardToRight;
                break;
            }
            else
            {
                firetruck.newState = fireTruckStates::forward;
                break;
            }

        // If the left stick state is set to down - ctm
        case leftStickStates::leftStickDown:

            // If the right stick state is set to left - ctm
            if (truckSteeringServoControlStick.newState == servoControlStickStates::rightStickLeft)
            {
                firetruck.newState = fireTruckStates::backwardToLeft;
                break;

                // If the right stick state is set to right - ctm
            }
            else if (truckSteeringServoControlStick.newState == servoControlStickStates::rightStickRight)
            {
                firetruck.newState = fireTruckStates::backwardToRight;
                break;

                // If the right stick state is set to neutral - ctm
            }
            else
            {
                firetruck.newState = fireTruckStates::backward;
                break;
            }

        // If the left stick state is set to neutral - ctm
        case leftStickStates::leftStickNeutral:

            // If the right stick state is set to left - ctm
            if (truckSteeringServoControlStick.newState == servoControlStickStates::rightStickLeft)
            {
                firetruck.newState = fireTruckStates::left;
                break;

                // If the right stick state is set to right - ctm
            }
            else if (truckSteeringServoControlStick.newState == servoControlStickStates::rightStickRight)
            {
                firetruck.newState = fireTruckStates::right;
                break;

                // If the right stick state is set to neutral - ctm
            }
            else
            {
                firetruck.newState = fireTruckStates::still;
                break;
            }
        }
    }

    // If the new right stick state doesn't match to the old one - ctm
    if (truckSteeringServoControlStick.newState != truckSteeringServoControlStick.oldState)
    {
        // Debug message - ctm
        // Serial.println("Right stick state differs from the old one");
        switch (truckSteeringServoControlStick.newState)
        {

        // If the right stick state is set to left - ctm
        case servoControlStickStates::rightStickLeft:

            // If the left stick state is set to up - ctm
            if (truckMotorEscControlStick.newState == leftStickStates::leftStickUp)
            {
                firetruck.newState = fireTruckStates::forwardToLeft;
                break;

                // If the left stick state is set to down - ctm
            }
            else if (truckMotorEscControlStick.newState == leftStickStates::leftStickDown)
            {
                firetruck.newState = fireTruckStates::backwardToLeft;
                break;

                // If the left stick state is set to neutral - ctm
            }
            else
            {
                firetruck.newState = fireTruckStates::left;
                break;
            }
            break;

        // If the right stick state is set to right - ctm
        case servoControlStickStates::rightStickRight:

            // If the left stick state is set to up - ctm
            if (truckMotorEscControlStick.newState == leftStickStates::leftStickUp)
            {
                firetruck.newState = fireTruckStates::forwardToRight;
                break;

                // If the left stick state is set to down - ctm
            }
            else if (truckMotorEscControlStick.newState == leftStickStates::leftStickDown)
            {
                firetruck.newState = fireTruckStates::backwardToRight;
                break;

                // If the left stick state is set to neutral - ctm
            }
            else
            {
                firetruck.newState = fireTruckStates::right;
                break;
            }

            break;

        // If the right stick state is set to neutral - ctm
        case servoControlStickStates::rightStickNeutral:

            // If the left stick state is set to up - ctm
            if (truckMotorEscControlStick.newState == leftStickStates::leftStickUp)
            {
                firetruck.newState = fireTruckStates::forward;
                break;

                // If the left stick state is set to down - ctm
            }
            else if (truckMotorEscControlStick.newState == leftStickStates::leftStickDown)
            {
                firetruck.newState = fireTruckStates::backward;
                break;

                // If the left stick state is set to neutral - ctm
            }
            else
            {
                firetruck.newState = fireTruckStates::straight;
                break;
            }
        }
    }
}

/********************************************************************************************************************/

// This function updates the new state of the left stick and returns the new state depending on if the old state is the same or not - ctm
void setMotorState()
{
    // Set these bools equal to the macros defined at the beginning - ctm
    // check motor stick position and set flags aproproately
    // Might have to add debouncing later

    bool isUp = upConditional;

    bool isDown = downConditional;

    bool isNeutral = leftNeutralConditional;

    // If the left stick is up - ctm
    if (isUp && !isDown)
    {
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Left stick up");
        truckMotorEscControlStick.newState = leftStickStates::leftStickUp;

        // If the left stick is down - ctm
    }
    else if (!isUp && isDown)
    {
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Left stick down");
        truckMotorEscControlStick.newState = leftStickStates::leftStickDown;

        // If the left stick is neutral - ctm
    }
    else
    {
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Left stick neutral");
        truckMotorEscControlStick.newState = leftStickStates::leftStickNeutral;
    }


}

/********************************************************************************************************************

This function updates the new state of the right stick and returns the new state depending on if the old state is the same or not - ctm

  Outline:

    - check and set the current states of the servoControl stick
      - check if the servo control stick is right or left
      - set the truckControlTimes.motorsEngaged to the current time
      - we might want to have additional bools for the state of the servo

********************************************************************************************************************/
void setSteeringServoState()
{

    // Set these bools equal to the macros defined at the beginning - ctm
    // delay(500);
    // delayMicroseconds(2000);
    bool isLeft = leftConditional;
    // delayMicroseconds(2000);
    // delay(500);
    bool isRight = rightConditional;
    // delayMicroseconds(2000);
    // delay(500);
    bool isNeutral = rightNeutralConditional;

    truckMovementAngles.servo = SteeringServo.read();
    // If the right stick is to the left - ctm
    if (isLeft && !isRight)
    {
        if (truckControlTimes.current - truckControlTimes.servoEngaged >= servoPeriod)
        {
            truckControlTimes.servoEngaged = truckControlTimes.current;
            if (truckMovementAngles.servo < 60)
            {
                Serial.println("Left");
                Serial.println(truckMovementAngles.servo);
                SpeedCon.write(truckMovementAngles.servo);
                truckMovementAngles.servo -= steeringAngleChange;
            }
        }
        // Debug message NOTE ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Right stick left");
        truckSteeringServoControlStick.newState = servoControlStickStates::rightStickLeft;

        // If the right stick is to the right - ctm
    }
    else if (!isLeft && isRight)
    {

        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Right stick right");
        truckSteeringServoControlStick.newState = servoControlStickStates::rightStickRight;

        // If the right stick is neutral - ctm
    }
    else
    {

        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Right stick neutral");
        truckSteeringServoControlStick.newState = servoControlStickStates::rightStickNeutral;
    }

}

void truckMovement()
{

   
    truckMovementAngles.motor = SpeedCon.read();
    truckMovementAngles.servo = SteeringServo.read();
    if (currentTime - truckControlTimes.motorsEngaged >= motorPeriod)
    {

        if (MotorBackwardAngleCheck)
        {
            motorsMoving = true;
            truckControlTimes.motorsEngaged = millis();
            motorForward = true;
            Serial.println("Backward");
            Serial.println(truckMovementAngles.motor);
            SpeedCon.write(truckMovementAngles.motor);
            truckMovementAngles.motor -= motorAngleChange;
        }

        if (MotorForwardAngleCheck)
        {
            Serial.println("Forward");
            Serial.println(truckMovementAngles.motor);
            SpeedCon.write(truckMovementAngles.motor);
            truckMovementAngles.motor += motorAngleChange;
        }
        // If the data is a motor stop command, do the following - ctm
        else if (isMotorStickPositionStop && motorsMoving)
        {
            if (truckMovementAngles.motor < 90)
            {
                for (int i = truckMovementAngles.motor; i > 90; i++)
                {
                    Serial.print("angle = ");
                    Serial.println(i);
                    SpeedCon.write(i);
                    truckMovementAngles.motor = i;
                    truckControlTimes.motorsEngaged = 0;
                    delay(15);
                }
            }
            else if (truckMovementAngles.motor > 90)
            {
                for (int i = truckMovementAngles.motor; i < 90; i--)
                {
                    Serial.print("angle = ");
                    Serial.println(i);
                    SpeedCon.write(i);
                    truckMovementAngles.motor = i;
                    truckControlTimes.motorsEngaged = 0;
                    delay(15);
                }
            }
            else
            {
                truckMovementAngles.motor = 90;
                SpeedCon.write(truckMovementAngles.motor);
            }
        }
    }

    if (TruckControlData.ServoLeft)
    {
        if (currentTime - truckControlTimes.servoEngaged >= servoPeriod)
        {
            if (truckMovementAngles.servo <= 180)
            {
                truckControlTimes.servoEngaged = currentTime;
                SteeringServo.write(truckMovementAngles.servo);
                truckMovementAngles.servo += steeringAngleChange;
            }
        }
    }
    else if (isServoStickLeft)
    {
        if (currentTime - truckControlTimes.servoEngaged >= servoPeriod)
        {
            if (truckMovementAngles.servo >= 0)
            {
                truckControlTimes.motorsEngaged = currentTime;
                SteeringServo.write(truckMovementAngles.servo);
                truckMovementAngles.servo += steeringAngleChange;
            }
        }
    }
}