
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

#include "axis.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CAxis::CAxis(uint8_t dataPin, uint8_t trimUpPin, uint8_t trimDownPin)
  :_ui8AxisPin(axisPin)
  ,_ui8TrimUpPin(trimUpPin)
  ,_ui8TrimDownPin(trimDownPin)
{}

//-----------------------------------------------------------------------------

CAxis& CAxis::operator=(const CAxis& other)
{
    if (this == &other) return *this;
    this->_ui8AxisPin = other._ui8AxisPin;
    this->_ui8TrimUpPin = other._ui8TrimUpPin;
    this->_ui8TrimDownPin = other._ui8TrimDownPin;
    return *this;
}

//-----------------------------------------------------------------------------

void CAxis::update(void)
{
  
}

//-----------------------------------------------------------------------------

int CAxis::getValue()
{
  return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
