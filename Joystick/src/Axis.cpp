
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** @file  Axis.cpp
 *  @date  November, 2016
 *  @brief
 *
 *
 *  @author Original from (C) 2011 By P. Bauermeister
 *  @author Cooked by Vicente A. (TT)
 *  @bug No know bugs.
 */

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#include <Joystick.h>

#include "axis.h"
#include "EEPROMAnything.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CAxis::CAxis(uint8_t axisPin,
  uint8_t trimUpPin,
  uint8_t trimDownPin,
  uint8_t EEPROM_ADDR)
  :_ui8AxisPin(axisPin)
  ,_oTrimUpButton(trimUpPin,DEFAULT_LONGPRESS_LEN)
  ,_oTrimDownButton(trimDownPin,DEFAULT_LONGPRESS_LEN)
  ,_ui8EEPROMAddr(EEPROM_ADDR)
  ,_i16TrimValue(0)
  ,_i16AxisValue(0){
  pinMode(_ui8AxisPin, INPUT);
  EEPROM_readAnything(_ui8EEPROMAddr,_i16TrimValue);
}

//-----------------------------------------------------------------------------

bool CAxis::update(void){
  bool bRslt{false};
  int16_t i16Value {analogRead(_ui8AxisPin)};

  if(abs(i16Value - _i16AxisValue) > _iDEADZONE){
    _i16AxisValue = i16Value;
    bRslt = true;
  }

  eEvent eventTrimUp {_oTrimUpButton.handle()};
  eEvent eventTrimDown {_oTrimDownButton.handle()};

  if(eventTrimUp==EV_SHORTPRESS){
    _i16TrimValue++;
    EEPROM_writeAnything(_ui8EEPROMAddr,_i16TrimValue);
  }else if(eventTrimDown==EV_SHORTPRESS){
    _i16TrimValue--;
    EEPROM_writeAnything(_ui8EEPROMAddr,_i16TrimValue);
  }else if(eventTrimUp==EV_LONGPRESS || eventTrimDown==EV_LONGPRESS){
    _i16TrimValue = 0;
    EEPROM_writeAnything(_ui8EEPROMAddr,_i16TrimValue);
  }

  _i16AxisValue += _i16TrimValue;

  return bRslt?true:!((eventTrimUp == EV_NONE) && (eventTrimDown == EV_NONE));
}

//-----------------------------------------------------------------------------

int CAxis::getValue(void) const{
  if(_i16AxisValue<JOYSTICK_DEFAULT_AXIS_MINIMUM)
    return JOYSTICK_DEFAULT_AXIS_MINIMUM;
  if(_i16AxisValue>JOYSTICK_DEFAULT_AXIS_MAXIMUM)
    return JOYSTICK_DEFAULT_AXIS_MAXIMUM;
  return _i16AxisValue;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
