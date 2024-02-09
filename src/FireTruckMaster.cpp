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
}

void loop()
{
    sendData(TruckControlData.SoundOne);
    readFromSlave();
    sendData(TruckControlData.SoundTwo);
    sendData(TruckControlData.SoundStop);
#if defined(CONTROLLER_TEST)

    Usb.Task();
// check if the controller is connected
#if defined(PS3_CONTROLLER)
    if (GameController.PS3Connected)
#elif defined(PS4_CONTROLLER)
    if (GameController.connected())
#endif
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
            sendData(TruckControlData.ServoRight); //  
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
// Only write value if it's different
#if defined(PS4_CONTROLLER)
            GameController.setRumbleOn(GameController.getAnalogButton(L2), GameController.getAnalogButton(R2));
#elif defined(PS3_CONTROLLER)
            GameController.setRumbleOn(RumbleEnum::RumbleHigh);
            delay(5000);
            GameController.setRumbleOn(RumbleEnum::RumbleLow);
#endif
        }
        oldL2Value = GameController.getAnalogButton(L2);
        oldR2Value = GameController.getAnalogButton(R2);

        if (GameController.getButtonClick(PS))
        {
            Serial.print(F("\r\nPS"));
            GameController.disconnect();
        }
        else
        {
            if (GameController.getButtonClick(TRIANGLE))
            {
                sendData(TruckControlData.SoundOne); // Implement sound one for triangle button - ctm 
                Serial.print(F("\r\nTriangle"));
                GameController.setRumbleOn(RumbleLow);
            }
            if (GameController.getButtonClick(CIRCLE))
            {
                sendData(TruckControlData.SoundStop); // Implement sound stop for circle button - ctm
                Serial.print(F("\r\nCircle"));
                GameController.setRumbleOn(RumbleHigh);
            }
            if (GameController.getButtonClick(CROSS))
            {
                sendData(TruckControlData.SoundTwo); // Implement sound two for cross button - ctm
                Serial.print(F("\r\nCross"));
#if defined(PS3_CONTROLLER)
                GameController.setLedOn(LED1); // Turn off blinking
#elif defined(PS4_CONTROLLER)
                GameController.setLedFlash(10, 10); // Set it to blink rapidly
#endif
            }
            if (GameController.getButtonClick(SQUARE))
            {
                sendData(TruckControlData.ToggleWaterPump); // Implement water pump for square button - ctm
                Serial.print(F("\r\nSquare"));
#if defined(PS3_CONTROLLER)
                GameController.setLedRaw(OFF); // Turn off blinking
#elif defined(PS4_CONTROLLER)
                GameController.setLedFlash(0, 0); // Turn off blinking
#endif
            }

            if (GameController.getButtonClick(UP))
            {
                Serial.print(F("\r\nUp"));
#if defined(PS3_CONTROLLER)
                GameController.setLedOn(LED2);
#elif defined(PS4_CONTROLLER)
                GameController.setLed(Red);
#endif
            }
            if (GameController.getButtonClick(RIGHT))
            {
                Serial.print(F("\r\nRight"));
#if defined(PS4_CONTROLLER)
                GameController.setLed(Blue);
#endif
            }
            if (GameController.getButtonClick(DOWN))
            {
                Serial.print(F("\r\nDown"));
#if defined(PS4_CONTROLLER)
                GameController.setLed(Yellow);
#endif
            }
            if (GameController.getButtonClick(LEFT))
            {
                Serial.print(F("\r\nLeft"));

#if defined(PS4_CONTROLLER)
                GameController.setLed(Green);
#endif
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