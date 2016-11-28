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

#define SERIAL_OUTPUT

//------------------------------------------------------------

#include "Joystick.h"
#include "EEPROMAnything.h"
#include "ButtonHandler.h"
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
}

CAxis* aAxis[4] = {
  new CAxis(PitchPin,PitchTrimUpPin,PitchTrimDownPin)
  ,new CAxis(RollPin,RollTrimUpPin,RollTrimDownPin)
  ,new CAxis(YawPin,YawTrimUpPin,YawTrimDownPin)
  ,new CAxis(ThrottlePin,ThrottleTrimUpPin,ThrottleTrimDownPin)}

//-----------------------------------------------------------------------------

void toggleLED(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void setup() {

  SERIAL_BEGIN;

  // Set Range Values
  Joystick.setXAxisRange(0, 1024);
  Joystick.setYAxisRange(0, 1024);
  Joystick.setThrottleRange(0, 1024);
  Joystick.setRudderRange(0, 1024);

  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  Joystick.setThrottle(0);
  Joystick.setRudder(0);

  Joystick.begin(false);

  pinMode(A0, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  for(uint8_t i = 0; i<10 ; i++){
    toggleLED();
    delay(100);
  }
  SERIAL_PRINTLN("System Running...");
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void loop() {
  //Update data
  static uint8_t iLoop {0};
  for(iLoop = 0 ; iLoop < 4 ; iLoop++)
    aAxis[iLoop].update();

  //Set new values
  Joystick.setXAxis(aAxis[PITCH].getValue());
  Joystick.setYAxis(aAxis[ROLL].getValue());
  Joystick.setThrottle(aAxis[THROTTLE].getValue());
  Joystick.setRudder(aAxis[YAW].getValue());

  Joystick.sendState();
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
