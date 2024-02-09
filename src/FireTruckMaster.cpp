#include <master/firetruck.h>

void setup()
{
    Wire.begin();       // join i2c bus
    Serial.begin(9600); // start serial for output
    // block while waiting for character over serial
    while (!Serial)
    {

    }

    Serial.println("Starting Fire Truck Master Test");

    // #if !defined(__MIPSEL__)
    //     while (!Serial)
    //         ; // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
    // #endif
    //     if (Usb.Init() == -1)
    //     {
    //         Serial.print(F("\r\nOSC did not start"));
    //         while (1)
    //             ; // Halt
    //     }
    //     Serial.print(F("\r\nGameController Bluetooth Library Started"));
}


void loop()
{
    // sendData(TruckControlData.SoundOne);
    // sendData(TruckControlData.SoundTwo);
    // sendData(TruckControlData.SoundStop);
#if defined(CONTROLLER_TEST)

    Usb.Task();
    if (GameController.connected())
    {
        if (GameController.getAnalogHat(LeftHatX) > 137 || GameController.getAnalogHat(LeftHatX) < 117 || GameController.getAnalogHat(LeftHatY) > 137 || GameController.getAnalogHat(LeftHatY) < 117)
        {

            Serial.println(F("\r\nLeftHatX: "));
            Serial.print(GameController.getAnalogHat(LeftHatX));
            Serial.print(F("\tLeftHatY: "));
            Serial.println(GameController.getAnalogHat(LeftHatY));
        }
        if (GameController.getAnalogHat(RightHatX) > 137 || GameController.getAnalogHat(RightHatX) < 117 || GameController.getAnalogHat(RightHatY) > 137 || GameController.getAnalogHat(RightHatY) < 117)
        {
            sendData(TruckControlData.ServoRight);
            Serial.print(F("\r\nRightHatX: "));
            Serial.print(GameController.getAnalogHat(RightHatX));
            Serial.print(F("\tRightHatY: "));
            Serial.print(GameController.getAnalogHat(RightHatY));
        }

        if (GameController.getAnalogButton(L2) || GameController.getAnalogButton(R2))
        { // These are the only analog buttons on the PS4 controller
            Serial.print(F("\r\nL2: "));
            Serial.print(GameController.getAnalogButton(L2));
            Serial.print(F("\tR2: "));
            Serial.print(GameController.getAnalogButton(R2));
        }
        if (GameController.getAnalogButton(L2) != oldL2Value || GameController.getAnalogButton(R2) != oldR2Value)
        {
          GameController.setRumbleOn(GameController.getAnalogButton(L2), GameController.getAnalogButton(R2));
        }
        oldL2Value = GameController.getAnalogButton(L2);
        oldR2Value = GameController.getAnalogButton(R2);

        if (GameController.getButtonClick(PS))
        {
            Serial.print(F("\r\nPS"));
        }
        else
        {
            if (GameController.getButtonClick(TRIANGLE))
            {
                Serial.print(F("\r\nTriangle"));
                GameController.setRumbleOn(RumbleLow);
            }
            if (GameController.getButtonClick(CIRCLE))
            {
                Serial.print(F("\r\nCircle"));
                GameController.setRumbleOn(RumbleHigh);
            }
            if (GameController.getButtonClick(CROSS))
            {
                Serial.print(F("\r\nCross"));
                GameController.setLedFlash(10, 10); // Set it to blink rapidly
            }
            if (GameController.getButtonClick(SQUARE))
            {
                Serial.print(F("\r\nSquare"));
                GameController.setLedFlash(0, 0); // Turn off blinking
            }

            if (GameController.getButtonClick(UP))
            {
                Serial.print(F("\r\nUp"));
                GameController.setLed(Red);
            }
            if (GameController.getButtonClick(RIGHT))
            {
                Serial.print(F("\r\nRight"));

                GameController.setLed(Blue);
            }
            if (GameController.getButtonClick(DOWN))
            {
                Serial.print(F("\r\nDown"));
                GameController.setLed(Yellow);
            }
            if (GameController.getButtonClick(LEFT))
            {
                Serial.print(F("\r\nLeft"));
                GameController.setLed(Green);
            }

            if (GameController.getButtonClick(L1))
                Serial.print(F("\r\nL1"));
            if (GameController.getButtonClick(L3))
                Serial.print(F("\r\nL3"));
            if (GameController.getButtonClick(R1))
                Serial.print(F("\r\nR1"));
            if (GameController.getButtonClick(R3))
                Serial.print(F("\r\nR3"));

            if (GameController.getButtonClick(SHARE))
                Serial.print(F("\r\nShare"));
            if (GameController.getButtonClick(OPTIONS))
            {
                Serial.print(F("\r\nOptions"));
                printAngle = !printAngle;
            }
            if (GameController.getButtonClick(TOUCHPAD))
            {
                Serial.print(F("\r\nTouchpad"));
                printTouch = !printTouch;
            }

            if (printAngle)
            { // Print angle calculated using the accelerometer only
                Serial.print(F("\r\nPitch: "));
                Serial.print(GameController.getAngle(Pitch));
                Serial.print(F("\tRoll: "));
                Serial.print(GameController.getAngle(Roll));
            }
        }
    } else {
        GameController.pair();
    }
#endif
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

    while (!Serial.available())
    {
        Serial.println("Waiting for input");
        delay(500);
    }
    data = Serial.read();
    char data2 = Serial.read();
    Serial.print("Sending: ");
    Serial.println(data);


    Wire.beginTransmission(I2CAddress); // Transmit to device
    Wire.write(data);                   // Send serial data
    // Wire.write(data2);                   // Send serial data
    Wire.endTransmission();             // Stop transmitting
    delay(2500);
    readFromSlave();
    delay(2500);
}