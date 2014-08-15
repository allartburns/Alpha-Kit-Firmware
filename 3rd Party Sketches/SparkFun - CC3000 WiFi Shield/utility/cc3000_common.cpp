/**
 * @file	cc3000_common.cpp
 * @brief 	Defines common functions/macros for the CC3000 library
 * @author	Texas Instruments
 * @author  Modified by Shawn Hymel (SparkFun Electronics)
 *
 * Changes to the original code are listed below:
 * 
 * - Changed file name from *.c to *.cpp to force the Arduino compiler to
 *   treat it as a C++ file
 *
 * - Added the following to allow for debugging:
 *      #include <Arduino.h>
 *      #include "../common.h"
 *
 * - Changed return value of STREAM_TO_UINT16_f to a more explicit conversion
 *   from a char[] to unsigned short
 *
 * - Changed return value of STREAM_TO_UINT32_f to a more explicit conversion
 *   from a char[] to unsigned long
 */

/*****************************************************************************
*
*  cc3000_common.c.c  - CC3000 Host Driver Implementation.
*  Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*****************************************************************************/
//*****************************************************************************
//
//! \addtogroup common_api
//! @{
//
//*****************************************************************************
/******************************************************************************
 *
 * Include files
 *
 *****************************************************************************/
#include <Arduino.h>
#include "../common.h"
 
#include "cc3000_common.h"
#include "socket.h"
#include "wlan.h"
#include "evnt_handler.h"

//*****************************************************************************
//
//!  __error__
//!
//!  @param  pcFilename - file name, where error occurred
//!  @param  ulLine     - line number, where error occurred
//!
//!  @return none
//!
//!  @brief stub function for ASSERT macro
//
//*****************************************************************************
void
__error__(char *pcFilename, unsigned long ulLine)
{
    //TODO full up function
}



//*****************************************************************************
//
//!  UINT32_TO_STREAM_f
//!
//!  @param  p       pointer to the new stream
//!  @param  u32     pointer to the 32 bit
//!
//!  @return               pointer to the new stream
//!
//!  @brief                This function is used for copying 32 bit to stream
//!						   while converting to little endian format.
//
//*****************************************************************************

unsigned char* UINT32_TO_STREAM_f (unsigned char *p, unsigned long u32)
{
	*(p)++ = (unsigned char)(u32);
	*(p)++ = (unsigned char)((u32) >> 8);
	*(p)++ = (unsigned char)((u32) >> 16);
	*(p)++ = (unsigned char)((u32) >> 24);
	return p;
}

//*****************************************************************************
//
//!  UINT16_TO_STREAM_f
//!
//!  @param  p       pointer to the new stream
//!  @param  u32     pointer to the 16 bit
//!
//!  @return               pointer to the new stream
//!
//!  @brief               This function is used for copying 16 bit to stream
//!                       while converting to little endian format.
//
//*****************************************************************************

unsigned char* UINT16_TO_STREAM_f (unsigned char *p, unsigned short u16)
{
	*(p)++ = (unsigned char)(u16);
	*(p)++ = (unsigned char)((u16) >> 8);
	return p;
}

//*****************************************************************************
//
//!  STREAM_TO_UINT16_f
//!
//!  @param  p          pointer to the stream
//!  @param  offset     offset in the stream
//!
//!  @return               pointer to the new 16 bit
//!
//!  @brief               This function is used for copying received stream to
//!                       16 bit in little endian format.
//
//*****************************************************************************

unsigned short STREAM_TO_UINT16_f(char* p, unsigned short offset)
{
    return ((unsigned short)p[offset] & 0x00FF) |
            (((unsigned short)p[offset + 1] << 8) & 0xFF00);

    /*return (unsigned short)((unsigned short)((unsigned short)
				(*(p + offset + 1)) << 8) + (unsigned short)(*(p + offset)));*/
}

//*****************************************************************************
//
//!  STREAM_TO_UINT32_f
//!
//!  @param  p          pointer to the stream
//!  @param  offset     offset in the stream
//!
//!  @return               pointer to the new 32 bit
//!
//!  @brief               This function is used for copying received stream to
//!                       32 bit in little endian format.
//
//*****************************************************************************

unsigned long STREAM_TO_UINT32_f(char* p, unsigned short offset)
{

/*#if (DEBUG == 1)
    unsigned long dest = 0;

    Serial.println("Macro: STREAM_TO_UINT32_f");
    Serial.print("Stream to copy: ");
    Serial.print((uint8_t)p[offset + 3], HEX);
    Serial.print((uint8_t)p[offset + 2], HEX);
    Serial.print((uint8_t)p[offset + 1], HEX);
    Serial.println((uint8_t)p[offset], HEX);
    dest = ((unsigned long)p[offset] & 0x000000FF) | 
            (((unsigned long)p[offset + 1] << 8) & 0x0000FF00) |
            (((unsigned long)p[offset + 2] << 16) & 0x00FF0000) |
            (((unsigned long)p[offset + 3] << 24) & 0xFF000000);
    Serial.print("Copied stream:  ");
    Serial.println(dest, HEX);
#endif*/

    return ((unsigned long)p[offset] & 0x000000FF) | 
            (((unsigned long)p[offset + 1] << 8) & 0x0000FF00) |
            (((unsigned long)p[offset + 2] << 16) & 0x00FF0000) |
            (((unsigned long)p[offset + 3] << 24) & 0xFF000000);

    /*return (unsigned long)((unsigned long)((unsigned long)
                         (*(p + offset + 3)) << 24) + (unsigned long)((unsigned long)
                         (*(p + offset + 2)) << 16) + (unsigned long)((unsigned long)
                         (*(p + offset + 1)) << 8) + (unsigned long)(*(p + offset)));*/
}



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
