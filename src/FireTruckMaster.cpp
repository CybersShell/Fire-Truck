#include <master/firetruck.h>

// These defined macros keep track of if the left stick moves - ctm 
#define upConditional GameController.getAnalogHat(LeftHatY) < 70
#define downConditional GameController.getAnalogHat(LeftHatY) > 220
#define leftNeutralConditional GameController.getAnalogHat(LeftHatY) > 70 && GameController.getAnalogHat(LeftHatY) < 220

// These defined macros keep track of if the right stick moves - ctm 
#define leftConditional GameController.getAnalogHat(RightHatX) < 70
#define rightConditional GameController.getAnalogHat(RightHatX) > 220
#define rightNeutralConditional GameController.getAnalogHat(RightHatX) > 70 && GameController.getAnalogHat(RightHatX) < 220

void setup()
{
    Wire.begin();       // join i2c bus
    Serial.begin(9600); // start serial for output
    // block while waiting for character over serial
    while (!Serial)
    {

    }

    Serial.println("Starting Fire Truck Master Test");

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
        leftStick.oldState = leftStickStates::leftStickNeutral;

        // Initialize the old state of the right stick - ctm 
        rightStick.oldState = rightStickStates::rightStickNeutral;
}

void loop()
{
    delayMicroseconds(600);
    // #if defined(CONTROLLER_TEST)

    Usb.Task();
    if (GameController.connected())
    {

        // If the X button has been hit, play the second sound - ctm 
        if(GameController.getButtonClick(CROSS))
        {
            sendData(TruckControlData.SoundTwo, dummyData); 
        }

        // If the triangle button has been hit, play the first sound - ctm 
        if(GameController.getButtonClick(TRIANGLE))
        {
            sendData(TruckControlData.SoundOne, dummyData);
        }

        // If the circle button has been hit, stop the current sound from playing - ctm 
        if(GameController.getButtonClick(CIRCLE))
        {
            sendData(TruckControlData.SoundStop, dummyData); 
        }

        // If the square button has been hit, toggle the water pump - ctm 
        if(GameController.getButtonClick(SQUARE))
        {
            sendData(TruckControlData.ToggleWaterPump, dummyData); 
        }
        if(GameController.getButtonClick(R3))
        {
            sendData(TruckControlData.ServoStraight, dummyData); 
        }
        // delay(500);

        // Set the states of the left stick, right stick, and the firetruck - ctm 
        getState(); 

        // If the new firetruck state is different from the old one - ctm 
        if(firetruck.newState != firetruck.oldState) {
            sendMovementData = true;
            fireTruckControl();
        }
        sendMovementData = false;
    }
}

// send data over I2C interface to slave
void sendData(char data, char secondMovementChar)
{
    Serial.print("Sending: ");


    Wire.beginTransmission(I2CAddress); // Transmit to device  
    if (sendMovementData) {
        Serial.println(data);
        Serial.println(secondMovementChar);
        Wire.write(TruckControlData.MovementData);
        Wire.write(data);
        Wire.write(secondMovementChar);
        sendMovementData = false;
    } else {
        Serial.println(data);
        Wire.write(data);                   // Send serial data
    }
    //Wire.write(data2);                  // Send serial data 
    Wire.endTransmission();             // Stop transmitting 

}

// This function updates both the old/new state of both analog sticks - ctm 
void getState()
{
    // Set the new states to be equal to the result of their respective functions - ctm 
    leftStick.newState = getStateOfLeftStick();
    rightStick.newState = getStateOfRightStick();
    firetruck.oldState = firetruck.newState;
    combineStates();

    // Set the old states to be equal to the new states - ctm
    leftStick.oldState = leftStick.newState;
    rightStick.oldState = rightStick.newState; 
}

void fireTruckControl() {

    // If the firetruck state is set to still - ctm 
    if(firetruck.newState == fireTruckStates::still) 
    {
        sendData(TruckControlData.MotorStop, TruckControlData.MotorStop);

    // If the firetruck state is set to forward to left - ctm 
    } else if(firetruck.newState == fireTruckStates::forwardToLeft) {
        sendData(TruckControlData.MotorForward, TruckControlData.ServoLeft);

    // If the firetruck state is set to forward to right - ctm 
    } else if(firetruck.newState == fireTruckStates::forwardToRight) {
        sendData(TruckControlData.MotorForward, TruckControlData.ServoRight);

    // If the firetruck state is set to forward - ctm 
    } else if(firetruck.newState == fireTruckStates::forward) {
        sendData(TruckControlData.MotorForward, TruckControlData.MotorBackward);

    // If the firetruck state is set to backward to left - ctm 
    } else if(firetruck.newState == fireTruckStates::backwardToLeft) {
        sendData(TruckControlData.MotorBackward, TruckControlData.ServoLeft);

    // If the firetruck state is set to backward to right - ctm 
    } else if(firetruck.newState == fireTruckStates::backwardToRight) {
        sendData(TruckControlData.MotorBackward, TruckControlData.ServoRight);

    // If the firetruck state is set to backward - ctm 
    } else if(firetruck.newState == fireTruckStates::backward) {
        sendData(TruckControlData.MotorBackward, TruckControlData.MotorBackward);

    // If the firetruck state is set to left - ctm 
    } else if(firetruck.newState == fireTruckStates::left) {
        sendData(TruckControlData.ServoLeft, TruckControlData.ServoLeft);

    // If the firetruck state is set to right - ctm 
    } else if(firetruck.newState == fireTruckStates::right) {
        sendData(TruckControlData.MotorStop, TruckControlData.ServoRight);
    } else if (firetruck.newState == fireTruckStates::straight) {
        sendData(TruckControlData.MotorStop, TruckControlData.ServoStraight);
    }
}

void combineStates()
{

    // If the new left stick state doesn't match to the old one - ctm 
    if(leftStick.newState != leftStick.oldState) 
    {

        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm 
        // Serial.println("Left stick new state differs from the old one");
        switch (leftStick.newState)
        {
            
            // If the left stick state is set to up - ctm 
            case leftStickStates::leftStickUp:

                    // If the right stick state is set to left - ctm 
                    if(rightStick.newState == rightStickStates::rightStickLeft) 
                    {
                         
                        firetruck.newState = fireTruckStates::forwardToLeft; 
                        break;

                    // If the right stick state is set to right - ctm 
                    } else if(rightStick.newState == rightStickStates::rightStickRight) {
                        
                        firetruck.newState = fireTruckStates::forwardToRight;
                        break; 
                    
                    } else {
 
                        firetruck.newState = fireTruckStates::forward; 
                        break; 
                    }



            // If the left stick state is set to down - ctm 
            case leftStickStates::leftStickDown:

                    // If the right stick state is set to left - ctm 
                    if(rightStick.newState == rightStickStates::rightStickLeft)
                    {

                        firetruck.newState = fireTruckStates::backwardToLeft; 
                        break; 

                    // If the right stick state is set to right - ctm 
                    } else if(rightStick.newState == rightStickStates::rightStickRight) {

                        firetruck.newState = fireTruckStates::backwardToRight; 
                        break; 

                    // If the right stick state is set to neutral - ctm 
                    } else {

                        firetruck.newState = fireTruckStates::backward; 
                        break; 
                    }

            // If the left stick state is set to neutral - ctm 
            case leftStickStates::leftStickNeutral:
                
                    // If the right stick state is set to left - ctm 
                    if(rightStick.newState == rightStickStates::rightStickLeft) 
                    {

                        firetruck.newState = fireTruckStates::left;
                        break;
                    
                    // If the right stick state is set to right - ctm 
                    } else if(rightStick.newState == rightStickStates::rightStickRight) {

                        firetruck.newState = fireTruckStates::right; 
                        break; 

                    // If the right stick state is set to neutral - ctm 
                    } else {

                        firetruck.newState = fireTruckStates::still;
                        break; 
                    }
        }
    }

    // If the new right stick state doesn't match to the old one - ctm 
    if(rightStick.newState != rightStick.oldState)
    {
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Right stick state differs from the old one"); 
        switch (rightStick.newState) 
        {
            
            // If the right stick state is set to left - ctm 
            case rightStickStates::rightStickLeft:

                    // If the left stick state is set to up - ctm 
                    if(leftStick.newState == leftStickStates::leftStickUp) 
                    {

                        firetruck.newState = fireTruckStates::forwardToLeft;
                        break; 

                    // If the left stick state is set to down - ctm 
                    } else if(leftStick.newState == leftStickStates::leftStickDown) {

                        firetruck.newState = fireTruckStates::backwardToLeft; 
                        break; 

                    // If the left stick state is set to neutral - ctm 
                    } else {

                        firetruck.newState = fireTruckStates::left; 
                        break; 
                    }
                break; 
            
            // If the right stick state is set to right - ctm 
            case rightStickStates::rightStickRight:
                    
                // If the left stick state is set to up - ctm 
                if(leftStick.newState == leftStickStates::leftStickUp)
                {

                    firetruck.newState = fireTruckStates::forwardToRight;
                    break;
                    
                // If the left stick state is set to down - ctm 
                } else if(leftStick.newState == leftStickStates::leftStickDown) {

                    firetruck.newState = fireTruckStates::backwardToRight;
                    break; 

                // If the left stick state is set to neutral - ctm 
                } else {

                    firetruck.newState = fireTruckStates::right; 
                    break; 
                }

                break;
            
            // If the right stick state is set to neutral - ctm 
            case rightStickStates::rightStickNeutral:

                    // If the left stick state is set to up - ctm 
                    if(leftStick.newState == leftStickStates::leftStickUp)
                    {

                        firetruck.newState = fireTruckStates::forward;
                        break;

                    // If the left stick state is set to down - ctm 
                    } else if(leftStick.newState == leftStickStates::leftStickDown) {

                        firetruck.newState = fireTruckStates::backward; 
                        break; 

                    // If the left stick state is set to neutral - ctm 
                    } else {

                        firetruck.newState = fireTruckStates::straight;
                        break; 
                    }
        }
    }
}

// This function updates the new state of the left stick and returns the new state depending on if the old state is the same or not - ctm 
leftStickStates getStateOfLeftStick()
{
    // Set these bools equal to the macros defined at the beginning - ctm 
    // check motor stick position and set flags aproproately
    // Might have to add debouncing later 
    delayMicroseconds(200);
    bool isUp = upConditional;
    delayMicroseconds(200);
    isUp = upConditional;
    delayMicroseconds(300);
    bool isDown = downConditional;
    delayMicroseconds(300);
    isDown = downConditional;
    delayMicroseconds(300);
    bool isNeutral = leftNeutralConditional; 
    delayMicroseconds(300);
    isNeutral = leftNeutralConditional; 

    // If the left stick is up - ctm 
    if(isUp && !isDown)
    {
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm 
        // Serial.println("Left stick up"); 
        leftStick.newState = leftStickStates::leftStickUp;

    // If the left stick is down - ctm 
    } else if (!isUp && isDown) {
        
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm 
        //Serial.println("Left stick down"); 

        leftStick.newState = leftStickStates::leftStickDown; 

    // If the left stick is neutral - ctm 
    } else {

        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm 
        // Serial.println("Left stick neutral");
        leftStick.newState = leftStickStates::leftStickNeutral; 
    }

    // If the old state differs from the new one - ctm 
    if(leftStick.oldState != leftStick.newState) {
        if(isNeutral)
        {

            return leftStickStates::leftStickNeutral;
        } else if(isUp && !isDown) {
            return leftStickStates::leftStickUp;
        } else {
            return leftStickStates::leftStickDown; 
        }
    }

    return leftStick.oldState; 
}

// This function updates the new state of the right stick and returns the new state depending on if the old state is the same or not - ctm
rightStickStates getStateOfRightStick()
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

    // If the right stick is to the left - ctm 
    if(isLeft && !isRight) {
        
        // Debug message NOTE ARDUINO DOES RECOGNIZE THIS - ctm 
        // Serial.println("Right stick left"); 
        rightStick.newState = rightStickStates::rightStickLeft;

    // If the right stick is to the right - ctm 
    } else if (!isLeft && isRight) {

        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm
        // Serial.println("Right stick right"); 
        rightStick.newState = rightStickStates::rightStickRight; 

    // If the right stick is neutral - ctm 
    } else {
        
        // Debug message NOTE: ARDUINO DOES RECOGNIZE THIS - ctm 
        // Serial.println("Right stick neutral"); 
        rightStick.newState = rightStickStates::rightStickNeutral; 
    }

    // If the old state differs from the new one - ctm 
    if(rightStick.oldState != rightStick.newState) {
        if(isNeutral) {
            return rightStickStates::rightStickNeutral;
        } else if(isLeft && !isRight) {
            return rightStickStates::rightStickLeft;
        } else {
            return rightStickStates::rightStickRight; 
        }
    }

    return rightStick.oldState; 

}