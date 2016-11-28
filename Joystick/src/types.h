//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

/** @file  types.h
 *  @date  November, 2016
 *  @brief This file contains the definitions of special data types
 *  		used in the application.
 *
 *
 *  @author Cooked by Vicente A. (TT)
 *  @bug No know bugs.
 */

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#ifndef _TYPES_H_
 #define _TYPES_H_

 //-----------------------------------------------------------------------------
 //---[ Standard header files: ]------------------------------------------------

 #if defined(ARDUINO) && ARDUINO >= 100
   #include "Arduino.h"
 #else
   #include "WProgram.h"
 #endif

 //---[ Version defines: ]-------------------------------------------------------

 /**
  * Program Major version
  */
 const uint8_t  MAJOR     (1);
 /**
  * Program Minor version
  */
 const uint8_t  MINOR     (0);
 /**
  * Program bugs fixes
  */
 const uint8_t  PATCH     (0);

//-----------------------------------------------------------------------------
//---[ Macros: ]---------------------------------------------------------------

#ifdef SERIAL_OUTPUT
  #define  SERIAL_BEGIN         Serial.begin(115200)
  #define  SERIAL_PRINTLN(a)    Serial.println(a)
  #define  SERIAL_PRINT(a)      Serial.print(a)
#else
  #define  SERIAL_BEGIN
  #define  SERIAL_PRINTLN(a)
  #define  SERIAL_PRINT(a)
#endif

//---[ Generic data Types: ]---------------------------------------------------

//---[ Application enumerated parameters: ]------------------------------------

//---[ Application-specific data Types: ]-------------------------------------

//---[ Global defines: ]-------------------------------------------------------

//---[ Useful Macros: ]--------------------------------------------------------

/**
 * Macro to clear a shared variable in an atomic operation (critical area)
 */
#ifndef CLEAR
  #define CLEAR(x) { cli(); x = 0; sei(); }
#endif

/**
 * Macro to clear individual bits in a SFR register
 */
#ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

/**
 * Macro to set individual bits in a SFR register
 */
#ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

#endif  // _TYPES_H_

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
