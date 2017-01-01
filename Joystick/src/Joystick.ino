//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** @file  vw.ino
 *  @date  November, 2015
 *  @brief Main application
 *
 *
 *  @author Based Matthew Heironimus Joystick and Cooked by Vicente A. (TT)
 *  @bug No know bugs.
 */

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#define SERIAL_OUTPUT

//------------------------------------------------------------

#include <DynamicHID.h>
#include <Joystick.h>

#include "types.h"
#include "pinout.h"
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
};

//-----------------------------------------------------------------------------

inline void toggleLED(){
  digitalWrite(LedPin, !digitalRead(LedPin));
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

  Joystick.setXAxis(JOYSTICK_DEFAULT_AXIS_MAXIMUM>>1);
  Joystick.setYAxis(JOYSTICK_DEFAULT_AXIS_MAXIMUM>>1);
  Joystick.setThrottle(JOYSTICK_DEFAULT_AXIS_MAXIMUM>>1);
  Joystick.setRudder(JOYSTICK_DEFAULT_AXIS_MAXIMUM>>1);

  Joystick.begin(false);

  pinMode(LedPin, OUTPUT);

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
    SERIAL_PRINT("Pitch:");
    SERIAL_PRINT(aAxis[PITCH].getValue());
    SERIAL_PRINT("\t");
    Joystick.setYAxis(aAxis[ROLL].getValue());
    SERIAL_PRINT("Roll:");
    SERIAL_PRINT(aAxis[ROLL].getValue());
    SERIAL_PRINT("\t");
    Joystick.setThrottle(aAxis[THROTTLE].getValue());
    SERIAL_PRINT("Throttle:");
    SERIAL_PRINT(aAxis[THROTTLE].getValue());
    SERIAL_PRINT("\t");
    Joystick.setRudder(aAxis[YAW].getValue());
    SERIAL_PRINT("Yaw:");
    SERIAL_PRINT(aAxis[YAW].getValue());
    SERIAL_PRINTLN("");
    Joystick.sendState();
  }

  toggleLED();
  delay(100);
}
