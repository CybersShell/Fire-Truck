#include <master/firetruck.h>

// The following bools are used to keep track of the states of each analog stick - ctm 
bool leftNeutral = false;
bool rightNeutral = false; 
bool leftStickDown = false;
bool leftStickUp = false;
bool rightStickLeft = false; 
bool rightStickRight = false; 

void setup()
{
    Wire.begin();       // join i2c bus
    // Serial.begin(9600); // start serial for output
    // // block while waiting for character over serial
    // while (!Serial)
    // {

    // }

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
        if(leftStickDown == true)
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

        // If the left stick is neutral, stop the motor - ctm 
        if(leftNeutral == true)
        {
            sendData(TruckControlData.MotorStop); 
        }

        // If the right stick is left while the left stick is neutral - ctm 
        if(rightStickLeft == true && leftNeutral == true) 
        {
            sendData(TruckControlData.ServoLeft);
        }

        // If the right stick is right while the left stick is neutral - ctm 
        if(rightStickRight == true && leftNeutral == true)
        {
            sendData(TruckControlData.ServoRight);
        }

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
    // Wire.write(data2);                   // Send serial data
    Wire.endTransmission();             // Stop transmitting

}