// Program used to test the USB Joystick object on the
// Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-03-28 - Original Version
// 2015-11-18 - Updated to use the new Joystick library
//              written for Arduino IDE Version 1.6.6 and
//              above.
// 2016-05-13   Updated to use new dynamic Joystick library
//              that can be customized.
//------------------------------------------------------------

//#define SERIAL_OUTPUT

//------------------------------------------------------------
/*
#include <Joystick.h>

#include "types.h"
#include "pinout.h"
#include "axis.h"

//-----------------------------------------------------------------------------

// Create Joystick
Joystick_ Joystick;

// Create Joystick axis handlers
enum eAxis{
  PITCH = 0
  ,ROLL
  ,YAW
  ,THROTTLE
};

//-----------------------------------------------------------------------------

inline void toggleLED(){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void setup(){

  SERIAL_BEGIN;

  // Set Range Values
  Joystick.setXAxisRange(JOYSTICK_DEFAULT_AXIS_MINIMUM,
    JOYSTICK_DEFAULT_AXIS_MAXIMUM);
  Joystick.setYAxisRange(JOYSTICK_DEFAULT_AXIS_MINIMUM,
    JOYSTICK_DEFAULT_AXIS_MAXIMUM);
  Joystick.setThrottleRange(JOYSTICK_DEFAULT_AXIS_MINIMUM,
    JOYSTICK_DEFAULT_AXIS_MAXIMUM);
  Joystick.setRudderRange(JOYSTICK_DEFAULT_AXIS_MINIMUM,
    JOYSTICK_DEFAULT_AXIS_MAXIMUM);

  Joystick.setXAxis(JOYSTICK_DEFAULT_AXIS_MAXIMUM<<1);
  Joystick.setYAxis(JOYSTICK_DEFAULT_AXIS_MAXIMUM<<1);
  Joystick.setThrottle(JOYSTICK_DEFAULT_AXIS_MAXIMUM<<1);
  Joystick.setRudder(JOYSTICK_DEFAULT_AXIS_MAXIMUM<<1);

  Joystick.begin(false);

  pinMode(LED_BUILTIN, OUTPUT);

  for(uint8_t i = 0; i<20 ; i++){
    toggleLED();
    delay(100);
  }

  SERIAL_PRINTLN("System Running...");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void loop() {

  static CAxis aAxis[4] {
    CAxis(PitchPin,PitchTrimUpPin,PitchTrimDownPin,EEPROM_CAL_PITCH)
    ,CAxis(RollPin,RollTrimUpPin,RollTrimDownPin,EEPROM_CAL_ROLL)
    ,CAxis(YawPin,YawTrimUpPin,YawTrimDownPin,EEPROM_CAL_YAW)
    ,CAxis(ThrottlePin,ThrottleTrimUpPin,ThrottleTrimDownPin,EEPROM_CAL_THROTTLE)};

  //Update data
  static uint8_t iLoop {0};
  static bool bUpdated{false};

  bUpdated = false;
  for(iLoop = 0 ; iLoop < 4 ; iLoop++){
    bUpdated = aAxis[iLoop].update()?true:bUpdated;
  }

  if(bUpdated){
    //Set new values
    Joystick.setXAxis(aAxis[PITCH].getValue());
    Joystick.setYAxis(aAxis[ROLL].getValue());
    Joystick.setThrottle(aAxis[THROTTLE].getValue());
    Joystick.setRudder(aAxis[YAW].getValue());
    Joystick.sendState();
  }

  toggleLED();
  delay(100);
}
*/
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



// Program used to test the USB Joystick object on the
// Arduino Leonardo or Arduino Micro.
//
// Matthew Heironimus
// 2015-03-28 - Original Version
// 2015-11-18 - Updated to use the new Joystick library
//              written for Arduino IDE Version 1.6.6 and
//              above.
// 2016-05-13   Updated to use new dynamic Joystick library
//              that can be customized.
//------------------------------------------------------------

#include "Joystick.h"

// Create Joystick
Joystick_ Joystick;

// Set to true to test "Auto Send" mode or false to test "Manual Send" mode.
//const bool testAutoSendMode = true;
const bool testAutoSendMode = false;

const unsigned long gcCycleDelta = 1000;
const unsigned long gcAnalogDelta = 25;
const unsigned long gcButtonDelta = 500;
unsigned long gNextTime = 0;
unsigned int gCurrentStep = 0;

void testSingleButtonPush(unsigned int button)
{
  if (button > 0)
  {
    Joystick.releaseButton(button - 1);
  }
  if (button < 32)
  {
    Joystick.pressButton(button);
  }
}

void testMultiButtonPush(unsigned int currentStep)
{
  for (int button = 0; button < 32; button++)
  {
    if ((currentStep == 0) || (currentStep == 2))
    {
      if ((button % 2) == 0)
      {
        Joystick.pressButton(button);
      } else if (currentStep != 2)
      {
        Joystick.releaseButton(button);
      }
    } // if ((currentStep == 0) || (currentStep == 2))
    if ((currentStep == 1) || (currentStep == 2))
    {
      if ((button % 2) != 0)
      {
        Joystick.pressButton(button);
      } else if (currentStep != 2)
      {
        Joystick.releaseButton(button);
      }
    } // if ((currentStep == 1) || (currentStep == 2))
    if (currentStep == 3)
    {
      Joystick.releaseButton(button);
    } // if (currentStep == 3)
  } // for (int button = 0; button < 32; button++)
}

void testXYAxis(unsigned int currentStep)
{
  int xAxis;
  int yAxis;

  if (currentStep < 256)
  {
    xAxis = currentStep - 127;
    yAxis = -127;
    Joystick.setXAxis(xAxis);
    Joystick.setYAxis(yAxis);
  }
  else if (currentStep < 512)
  {
    yAxis = currentStep - 256 - 127;
    Joystick.setYAxis(yAxis);
  }
  else if (currentStep < 768)
  {
    xAxis = 128 - (currentStep - 512);
    Joystick.setXAxis(xAxis);
  }
  else if (currentStep < 1024)
  {
    yAxis = 128 - (currentStep - 768);
    Joystick.setYAxis(yAxis);
  }
  else if (currentStep < 1024 + 128)
  {
    xAxis = currentStep - 1024 - 127;
    Joystick.setXAxis(xAxis);
    Joystick.setYAxis(xAxis);
  }
}

void testZAxis(unsigned int currentStep)
{
  if (currentStep < 128)
  {
    Joystick.setZAxis(-currentStep);
  }
  else if (currentStep < 256 + 128)
  {
    Joystick.setZAxis(currentStep - 128 - 127);
  }
  else if (currentStep < 256 + 128 + 127)
  {
    Joystick.setZAxis(127 - (currentStep - 383));
  }
}

void testHatSwitch(unsigned int currentStep)
{
  if (currentStep < 8)
  {
    Joystick.setHatSwitch(0, currentStep * 45);
  }
  else if (currentStep == 8)
  {
    Joystick.setHatSwitch(0, -1);
  }
  else if (currentStep < 17)
  {
    Joystick.setHatSwitch(1, (currentStep - 9) * 45);
  }
  else if (currentStep == 17)
  {
    Joystick.setHatSwitch(1, -1);
  }
  else if (currentStep == 18)
  {
    Joystick.setHatSwitch(0, 0);
    Joystick.setHatSwitch(1, 0);
  }
  else if (currentStep < 27)
  {
    Joystick.setHatSwitch(0, (currentStep - 18) * 45);
    Joystick.setHatSwitch(1, (8 - (currentStep - 18)) * 45);
  }
  else if (currentStep == 27)
  {
    Joystick.setHatSwitch(0, -1);
    Joystick.setHatSwitch(1, -1);
  }
}

void testThrottleRudder(unsigned int value)
{
  Joystick.setThrottle(value);
  Joystick.setRudder(255 - value);
}

void testXYZAxisRotation(unsigned int degree)
{
  Joystick.setRxAxis(degree);
  Joystick.setRyAxis(360 - degree);
  Joystick.setRzAxis(degree * 2);
}

void setup() {

  // Set Range Values
  Joystick.setXAxisRange(-127, 127);
  Joystick.setYAxisRange(-127, 127);
  Joystick.setZAxisRange(-127, 127);
  Joystick.setRxAxisRange(0, 360);
  Joystick.setRyAxisRange(0, 360);
  Joystick.setRzAxisRange(0, 720);
  Joystick.setThrottleRange(0, 255);
  Joystick.setRudderRange(0, 255);

  if (testAutoSendMode)
  {
    Joystick.begin();
  }
  else
  {
    Joystick.begin(false);
  }

  pinMode(A0, INPUT_PULLUP);
  pinMode(13, OUTPUT);
}

void loop() {

  // System Disabled
  if (digitalRead(A0) != 0)
  {
    // Turn indicator light off.
    digitalWrite(13, 0);
    return;
  }

  // Turn indicator light on.
  digitalWrite(13, 1);

  if (millis() >= gNextTime)
  {

    if (gCurrentStep < 33)
    {
      gNextTime = millis() + gcButtonDelta;
      testSingleButtonPush(gCurrentStep);
    }
    else if (gCurrentStep < 37)
    {
      gNextTime = millis() + gcButtonDelta;
      testMultiButtonPush(gCurrentStep - 33);
    }
    else if (gCurrentStep < (37 + 256))
    {
      gNextTime = millis() + gcAnalogDelta;
      testThrottleRudder(gCurrentStep - 37);
    }
    else if (gCurrentStep < (37 + 256 + 1024 + 128))
    {
      gNextTime = millis() + gcAnalogDelta;
      testXYAxis(gCurrentStep - (37 + 256));
    }
    else if (gCurrentStep < (37 + 256 + 1024 + 128 + 510))
    {
      gNextTime = millis() + gcAnalogDelta;
      testZAxis(gCurrentStep - (37 + 256 + 1024 + 128));
    }
    else if (gCurrentStep < (37 + 256 + 1024 + 128 + 510 + 28))
    {
      gNextTime = millis() + gcButtonDelta;
      testHatSwitch(gCurrentStep - (37 + 256 + 1024 + 128 + 510));
    }
    else if (gCurrentStep < (37 + 256 + 1024 + 128 + 510 + 28 + 360))
    {
      gNextTime = millis() + gcAnalogDelta;
      testXYZAxisRotation(gCurrentStep - (37 + 256 + 1024 + 128 + 510 + 28));
    }

    if (testAutoSendMode == false)
    {
      Joystick.sendState();
    }

    gCurrentStep++;
    if (gCurrentStep == (37 + 256 + 1024 + 128 + 510 + 28 + 360))
    {
      gNextTime = millis() + gcCycleDelta;
      gCurrentStep = 0;
    }
  }
}
