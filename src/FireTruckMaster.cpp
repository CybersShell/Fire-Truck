#include <master/firetruck.h>

// These defined macros keep track of if the left stick moves - ctm 
#define upConditional GameController.getAnalogHat(LeftHatY) < 120
#define downConditional GameController.getAnalogHat(LeftHatY) > 150
#define leftNeutralConditional GameController.getAnalogHat(LeftHatY) > 120 && GameController.getAnalogHat(LeftHatY) < 150

// These defined macros keep track of if the right stick moves - ctm 
#define leftConditional GameController.getAnalogHat(RightHatX) > 150
#define rightConditional GameController.getAnalogHat(RightHatX) < 120
#define rightNeutralConditional GameController.getAnalogHat(RightHatX) > 120 && GameController.getAnalogHat(RightHatX) < 150

// Comment out for time being until state machine is working - ctm 
/*
    bool leftStickUp = false; 
    bool leftStickDown = false;
    bool rightStickLeft = false;
    bool rightStickRight = false; 
    bool leftNeutral = false;
    bool rightNeutral = false; 
*/ 

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
}


void loop()
{
    // sendData(TruckControlData.SoundOne);
    // sendData(TruckControlData.SoundTwo);
    // sendData(TruckControlData.SoundStop);
    // #if defined(CONTROLLER_TEST)

    Usb.Task();
    if (GameController.connected())
    {
        setState(); 
    
// Old implementation, can delete when state machine has been figured out - ctm 
/*
        // If the left stick is up - ctm 
        if(GameController.getAnalogHat(LeftHatY) > 150) 
        {
            leftStickUp = true; 
        } else {
            leftStickUp = false; 
        }

        // If the left stick is down 
        if(GameController.getAnalogHat(LeftHatY) < 120)
        {
            leftStickDown = true; 
        } else {
            leftStickDown = false;
        }
        
        // If the left stick is neutral - ctm 
        if(GameController.getAnalogHat(LeftHatY) > 120 && GameController.getAnalogHat(LeftHatY) < 150) 
        {
            leftNeutral = true; 
        } else {
            leftNeutral = false; 
        }

        // If the right stick is to the left - ctm 
        if(GameController.getAnalogHat(RightHatX) > 150) 
        {
            rightStickLeft = true; 
        } else {
            rightStickLeft = false; 
        }

        // If the right stick is to the right 
        if(GameController.getAnalogHat(RightHatX) < 120) 
        {
            rightStickRight = true; 
        } else {
            rightStickRight = false; 
        }

        // If the right stick is neutral - ctm 
        if(GameController.getAnalogHat(RightHatX) > 120 && GameController.getAnalogHat(RightHatX) < 150) 
        {
            rightNeutral = true; 
        } else {
            rightNeutral = false; 
        }

        // // If the left stick is neutral, stop the motor - ctm 
        // if(leftNeutral == true && !leftStickDown && !leftStickUp)
        // {
        //     sendData(TruckControlData.MotorStop);
        // }
        // Here are all the cases in which the left stick is up - ctm 
        if(leftStickUp == true) 
        {

            // If the right stick is turned left - ctm 
            if(rightStickLeft == true) 
            {
                sendData(TruckControlData.ForwardLeft);

            // If the right stick is turned right - ctm 
            } else if (rightStickRight == true) {
                sendData(TruckControlData.ForwardRight); 

            // If the right stick is neutral - ctm 
            } else {
                sendData(TruckControlData.ServoMiddle); 
                sendData(TruckControlData.MotorForward); 
            }
        }

        // Here are all the cases in which the left stick is down - ctm 
        else if(leftStickDown == true)
        {

            // If the right stick is turned left - ctm 
            if(rightStickLeft == true)
            {
                sendData(TruckControlData.BackwardLeft);
            // If the right stick is turned right - ctm 
            } else if(rightStickRight == true) {
                sendData(TruckControlData.BackwardRight);
            // If the right stick is neutral - ctm 
            } else {
                sendData(TruckControlData.ServoMiddle);
                sendData(TruckControlData.MotorBackward);
            }
        }


        // If the right stick is left while the left stick is neutral - ctm 
        else if(rightStickLeft == true && leftNeutral == true) 
        {
            sendData(TruckControlData.ServoLeft);
        }

        // If the right stick is right while the left stick is neutral - ctm 
        else if(rightStickRight == true && leftNeutral == true)
        {
            sendData(TruckControlData.ServoRight);
        }
*/


        // If the X button has been hit, play the second sound - ctm 
        if(GameController.getButtonClick(CROSS))
        {
            sendData(TruckControlData.SoundTwo); 
        }

        // If the triangle button has been hit, play the first sound - ctm 
        if(GameController.getButtonClick(TRIANGLE))
        {
            sendData(TruckControlData.SoundOne);
        }

        // If the circle button has been hit, stop the current sound from playing - ctm 
        if(GameController.getButtonClick(CIRCLE))
        {
            sendData(TruckControlData.SoundStop); 
        }

        // If the square button has been hit, toggle the water pump - ctm 
        if(GameController.getButtonClick(SQUARE))
        {
            sendData(TruckControlData.ToggleWaterPump); 
        }



            // Comment out for now, use it for debugging purposes later 
            /*********************************************************
            Serial.println(F("\r\nLeftHatX: "));
            Serial.print(GameController.getAnalogHat(LeftHatX));
            Serial.print(F("\tLeftHatY: "));
            Serial.println(GameController.getAnalogHat(LeftHatY));
            *********************************************************/
    }
}


// Function that checks the state of the firetruck and sends data depending on what state its in - ctm 
/* Comment out this function until we get working implementation - ctm 
void setState() {
    switch (FiretruckState) {
        
        // If the firetruck is still or not moving - ctm 
        case firetruckStill: 

            // If the left stick has been moved up - ctm 
            if(GameController.getAnalogHat(LeftHatY) < 120)
            {
                // Changes state then breaks - ctm 
                FiretruckState = firetruckMoveForward;
                break; 

            // If the left stick has been moved down - ctm 
            } else if(GameController.getAnalogHat(LeftHatY) > 150) {

                // Changes states then breaks - ctm 
                FiretruckState = firetruckMoveBackward; 
                break; 

            // If the right stick is turned to the left - ctm 
            } else if(GameController.getAnalogHat(RightHatX) > 150) {

                // Changes states then breaks - ctm 
                FiretruckState = firetruckTurnLeft; 
                break; 

            // If the right stick is turned to the right - ctm 
            } else if(GameController.getAnalogHat(RightHatX) < 120) {

                // Changes state then breaks - ctm 
                FiretruckState = firetruckTurnRight; 
                break; 

            } else {
                break; 
            }

        // If the firetruck is moving forward - ctm 
        case firetruckMoveForward:

            // If the right stick is turned towards the right - ctm 
            if(GameController.getAnalogHat(RightHatX) < 120)
            {   

                // If the old state does not equal to the current state - ctm
                if(OldState != FiretruckState) 
                {
                    OldState = FiretruckState; 
                    sendData(TruckControlData.ForwardRight);
                    break;
                } else {
                    break; 
                }
                

            // If the right stick is turned towards the left - ctm 
            } else if(GameController.getAnalogHat(RightHatX) > 150) 
            { 

                // If the old state does not equal to the current state  - ctm 
               if(OldState != FiretruckState) 
                {
                    OldState = FiretruckState; 
                    sendData(TruckControlData.ForwardLeft);
                    break;
                } else {
                    break; 
                }

            // If the left stick is in the neutral position - ctm 
            } else if(GameController.getAnalogHat(LeftHatY) > 120 && GameController.getAnalogHat(LeftHatY) < 150)
            {
                // Send the corresponding data then reset the state to the still state - ctm 
                sendData(TruckControlData.MotorStop); 
                FiretruckState = firetruckStill; 
                break; 

            } else {
                break; 
            }

        // If the firetruck is moving backwards 
        case firetruckMoveBackward: 

            // If the right stick is to the right - ctm 
            if(GameController.getAnalogHat(RightHatX) < 120)
            {

                // If the old state does not equal to the current state - ctm 
                if(OldState != FiretruckState) 
                {
                    OldState = FiretruckState; 
                    sendData(TruckControlData.BackwardRight);
                    break;
                } else {
                    break; 
                }

            // If the right stick is to the left - ctm 
            } else if(GameController.getAnalogHat(RightHatX) > 150)
            {

                // If the old state does not equal to the current state - ctm
                if(OldState != FiretruckState) 
                {
                    OldState = FiretruckState; 
                    sendData(TruckControlData.BackwardLeft);
                    break;
                } else {
                    break; 
                }

            // If the left stick is in the neutral position - ctm 
            } else if(GameController.getAnalogHat(LeftHatY) > 120 && GameController.getAnalogHat(LeftHatY) < 150)
            {
                // Send the corresponding data then reset state to the still state - ctm 
                sendData(TruckControlData.MotorStop); 
                FiretruckState = firetruckStill; 
                break; 

            } else {
                break; 
            }

        // If the firetruck is turning left - ctm 
        case firetruckTurnLeft:

            // If the left stick is up - ctm 
            if(GameController.getAnalogHat(LeftHatY) < 120)
            {   
                // Change states to the forward state then breaks - ctm 
                FiretruckState = firetruckMoveForward;
                break; 

            // If the left stick is down - ctm 
            } else if(GameController.getAnalogHat(LeftHatY) > 150)
            {
                
                // Changes states to the backward state then breaks - ctm 
                FiretruckState = firetruckMoveBackward;
                break; 

            // If there is nothing going on with the left stick - ctm 
            } else {
                
                // If the old state does not equal to the current state - ctm
                if(OldState != FiretruckState) 
                {
                    OldState = FiretruckState; 
                    sendData(TruckControlData.ServoLeft);
                    break;
                } else {
                    break; 
                }
            }
        
        // If the firetruck is turning right - ctm 
        case firetruckTurnRight: 

            // If the left stick is up - ctm 
            if(GameController.getAnalogHat(LeftHatY) < 120)
            {

                // Changes states to the forward state then breaks - ctm 
                FiretruckState = firetruckMoveForward;
                break; 

            // If the left stick is down - ctm 
            } else if(GameController.getAnalogHat(LeftHatY) > 150)
            {

                // Changes states to the backwards state then breaks - ctm 
                FiretruckState = firetruckMoveBackward;
                break; 

            // If there is nothing going on with the left stick - ctm 
            } else {

                // If the old state does not equal to the current state - ctm
                if(OldState != FiretruckState) 
                {
                    OldState = FiretruckState; 
                    sendData(TruckControlData.ServoLeft);
                    break;
                } else {
                    break; 
                }
            }
    }

}

*/ 
        

void readFromSlave() {
  // if data size is available from nodes
  if(Wire.available()) {
    for (int i = 0; i < 1; i++) {
      messageToMaster[i] = Wire.read();  // get data
    }
  Serial.print("Slave: ");
  Serial.println(messageToMaster[0]);
  if (messageToMaster[0] == 'D')
  {
    
  }

  }
}

// send data over I2C interface to slave
void sendData(char data)
{
// #if !defined(CONTROLLER_TEST)
//     while (!Serial.available())
//     {
//         Serial.println("Waiting for input");
//         delay(500);
//     }
//     data = Serial.read();

// #endif // CONTROLLER_TEST

    Serial.print("Sending: ");
    Serial.println(data);


    Wire.beginTransmission(I2CAddress); // Transmit to device
    Wire.write(data);                   // Send serial data
    // Wire.write(data2);               // Send serial data
    Wire.endTransmission();             // Stop transmitting

}

// This function updates both the old/new state of both analog sticks - ctm 
void getState()
{
    // Set the new states to be equal to the result of their respective functions - ctm 
    leftStick.newState = getStateOfLeftStick();
    rightStick.newState = getStateOfRightStick();
    combineStates();

    // Set the old states to be equal to the new states - ctm
    leftStick.oldState = leftStick.newState;
    rightStick.oldState = rightStick.newState; 
}


void combineStates()
{

    // If the new left stick state doesn't match to the old one - ctm 
    if(leftStick.newState != leftStick.oldState) 
    {
        switch (leftStick.newState)
        {
            
            // If the left stick state is set to up - ctm 
            case leftStickStates::leftStickUp:

                // If the right stick new state is different from the old state - ctm 
                if(rightStick.newState != rightStick.oldState) {

                    // If the right stick state is set to left - ctm 
                    if(rightStick.newState == rightStickStates::rightStickLeft) 
                    {
                        firetruck.newState = fireTruckStates::forwardToLeft; 
                        break;

                    // If the right stick state is set to right - ctm 
                    } else if(rightStick.newState == rightStickStates::rightStickRight) {
                        firetruck.newState = fireTruckStates::forwardToRight;
                        break; 
                    
                    // If the right stick state is set to neutral - ctm 
                    } else {
                        firetruck.newState = fireTruckStates::forward; 
                        break; 
                    }
                } 

            // If the left stick state is set to down - ctm 
            case leftStickStates::leftStickDown:

                // If the right stick new state is different from the old state - ctm 
                if(rightStick.newState != rightStick.oldState) {

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
                }

            // If the left stick state is set to neutral - ctm 
            case leftStickStates::leftStickNeutral:

                // If the right stick new state is different from the old state - ctm 
                if(rightStick.newState != rightStick.oldState) {
                    if(rightStick.newState == rightStickStates::rightStickLeft) 
                    {
                        firetruck.newState = fireTruckStates::left;
                        break;
                    } else if(rightStick.newState == rightStickStates::rightStickRight) {
                        firetruck.newState = fireTruckStates::right; 
                        break; 
                    } else {
                        firetruck.newState = fireTruckStates::still;
                        break; 
                    }
                } 
        }
    }

    // If the new right stick state doesn't match to the old one - ctm 
    if(rightStick.newState != rightStick.oldState)
    {
        switch (rightStick.newState) 
        {
            case rightStickStates::rightStickLeft:
                if(leftStick.newState != leftStick.oldState)
                {
                    if(leftStick.newState == leftStickStates::leftStickUp) 
                    {
                        firetruck.newState = fireTruckStates::forwardToLeft;
                        break; 
                    } else if(leftStick.newState == leftStickStates::leftStickDown) {
                        firetruck.newState = fireTruckStates::backwardToLeft; 
                        break; 
                    } else {
                        firetruck.newState = fireTruckStates::left; 
                        break; 
                    }
                }
            
            case rightStickStates::rightStickRight:
                if(leftStick.newState != leftStick.oldState) 
                {
                    if(leftStick.newState == leftStickStates::leftStickUp)
                    {
                        firetruck.newState == fireTruckStates::forwardToRight;
                        break;
                    } else if(leftStick.newState == leftStickStates::leftStickDown) {
                        firetruck.newState == fireTruckStates::backwardToRight;
                        break; 
                    } else {
                        firetruck.newState = fireTruckStates::left; 
                        break; 
                    }
                }

            case rightStickStates::rightStickNeutral:
                if(leftStick.newState != leftStick.oldState)
                {
                    if(leftStick.newState == leftStickStates::leftStickUp)
                    {
                        firetruck.newState == fireTruckStates::forward;
                        break;
                    } else if(leftStick.newState == leftStickStates::leftStickDown) {
                        firetruck.newState == fireTruckStates::backward; 
                        break; 
                    } else {
                        firetruck.newState == fireTruckStates::still; 
                        break; 
                    }
                }
            
        }
    }

}

// This function updates the new state of the left stick and returns the new state depending on if the old state is the same or not - ctm 
leftStickStates getStateOfLeftStick()
{
    // Set these bools equal to the macros defined at the beginning - ctm 
    bool isUp = upConditional;
    bool isDown = downConditional;
    bool isNeutral = leftNeutralConditional; 

    // If the left stick is up - ctm 
    if(isUp && !isDown)
    {
        leftStick.newState = leftStickStates::leftStickUp;

    // If the left stick is down - ctm 
    } else if (!isUp && isDown) {
        leftStick.newState = leftStickStates::leftStickDown; 

    // If the left stick is neutral - ctm 
    } else {
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
    bool isLeft = leftConditional;
    bool isRight = rightConditional;
    bool isNeutral = rightNeutralConditional;

    // If the right stick is to the left - ctm 
    if(isLeft && !isRight) {
        rightStick.newState = rightStickStates::rightStickLeft;

    // If the right stick is to the right - ctm 
    } else if (!isLeft && isRight) {
        rightStick.newState = rightStickStates::rightStickRight; 

    // If the right stick is neutral - ctm 
    } else {
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