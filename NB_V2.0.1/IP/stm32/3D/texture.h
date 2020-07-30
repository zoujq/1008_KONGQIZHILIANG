/*******************************************************************************************
 * This file is part of avr 3d  program
 *
 * AVR S65 3D Project 
 * By MacroXu <kk20y@yahoo.com.cn> Copyright (C) 2008,2009 
 * 
 * This code is free, You can redistribute it and/or modify it on the following conditions,
 * Send me the modified source code and comment the code you changed.I am credited for the 
 * original work in all derivative code.
 *******************************************************************************************
 */
#ifndef __texture_h
#define __texture_h

const unsigned char code texture_logo[910] = {
0X9E,0XF7,0X7D,0XEF,0X3E,0XEF,0X18,0XD7,0XD6,0XCE,0XD7,0XCE,0X95,0XBE,0XB7,0XCE,
0X54,0XBE,0X55,0XC6,0X56,0XC6,0X33,0XB6,0X34,0XB6,0XB8,0XCE,0X95,0XC6,0XD9,0XD6,
0XD7,0XCE,0XD5,0XC6,0XF7,0XDE,0X18,0XD7,0X59,0XDF,0X59,0XDF,0X59,0XE7,0XDD,0XF7,
0XBB,0XEF,0X99,0XEF,0XFB,0XEF,0XFD,0XF7,0X78,0XDF,0XF6,0XD6,0XD7,0XD6,0X96,0XC6,
0XF8,0XBD,0X75,0XAD,0X34,0XA5,0X9E,0XF7,0X7D,0XEF,0X5F,0XF7,0XC7,0X96,0XC1,0X86,
0XC0,0X7E,0X48,0X9F,0XF9,0XEF,0XA0,0X76,0XCF,0XC7,0XF3,0XD7,0X80,0X6E,0X8D,0XB7,
0XFC,0XFF,0XD0,0XC7,0XFA,0XF7,0XF5,0XDF,0X29,0XA7,0X80,0X76,0XC2,0X86,0XA1,0X7E,
0X80,0X7E,0X91,0XC7,0XF9,0XEF,0XF6,0XDF,0XC5,0X8E,0XE5,0X8E,0XF9,0XEF,0X8F,0XBF,
0X06,0X97,0XC0,0X7E,0XC6,0X8E,0XFB,0XCD,0X75,0XAD,0X34,0XA5,0X9E,0XF7,0X7D,0XEF,
0X5F,0XF7,0XC9,0X9E,0XC4,0X8E,0XA4,0X86,0XFA,0XEF,0XFF,0XFF,0XE7,0X96,0X2C,0XAF,
0XFD,0XFF,0XA3,0X86,0XFB,0XF7,0XB3,0XCF,0XF9,0XEF,0X71,0XC7,0X0A,0XA7,0XFF,0XFF,
0XC5,0X8E,0XC5,0X8E,0XC6,0X8E,0XC6,0X8E,0X4C,0XAF,0XE7,0X96,0XFE,0XFF,0XD3,0XCF,
0X4D,0XB7,0XFF,0XFF,0X6F,0XBF,0XFC,0XF7,0X2B,0XAF,0X88,0X96,0XFB,0XCD,0X75,0XAD,
0X34,0XA5,0X9E,0XF7,0X7D,0XEF,0X5F,0XF7,0XC9,0X9E,0XA3,0X86,0X91,0XC7,0XF8,0XE7,
0XFD,0XFF,0X6D,0XB7,0XC5,0X8E,0XFC,0XF7,0X4E,0XB7,0XFF,0XFF,0XC5,0X8E,0XFA,0XEF,
0X2C,0XAF,0XE6,0X96,0XFF,0XFF,0X4B,0XAF,0XC4,0X8E,0XC6,0X8E,0XC4,0X8E,0X4B,0XAF,
0XFF,0XFF,0XFF,0XFF,0XC6,0X8E,0X6D,0XB7,0XFC,0XFF,0X80,0X76,0X4D,0XB7,0XFE,0XFF,
0X87,0X8E,0XFB,0XC5,0X75,0XAD,0X34,0XA5,0X9E,0XF7,0X7D,0XEF,0X5F,0XF7,0XC9,0X9E,
0XC4,0X8E,0XFE,0XFF,0XFF,0XFF,0XFF,0XFF,0XD5,0XD7,0X81,0X7E,0XF7,0XE7,0XFE,0XFF,
0XF8,0XE7,0XA2,0X86,0XFB,0XF7,0XFF,0XFF,0XFF,0XFF,0X6E,0XBF,0X81,0X7E,0XC6,0X8E,
0XC6,0X8E,0XC5,0X8E,0XC5,0X8E,0X4C,0XAF,0XFF,0XFF,0X4E,0XB7,0X6D,0XB7,0XFD,0XFF,
0X60,0X76,0XE7,0X96,0XFF,0XFF,0X87,0X96,0XDA,0XC5,0X75,0XAD,0X34,0XA5,0X9E,0XF7,
0X7D,0XEF,0X5F,0XF7,0XC7,0X96,0X90,0XC7,0XFF,0XFF,0X0A,0XA7,0XD5,0XD7,0XFE,0XFF,
0X81,0X7E,0X90,0XC7,0XFF,0XFF,0X8F,0XBF,0X82,0X86,0XFC,0XFF,0X91,0XC7,0XB2,0XCF,
0XFD,0XFF,0X90,0XBF,0XA3,0X86,0XC4,0X8E,0X08,0X9F,0XFC,0XFF,0X08,0X9F,0XFB,0XF7,
0X90,0XBF,0X4D,0XB7,0XFF,0XFF,0XE8,0X9E,0XD7,0XDF,0XFB,0XF7,0X86,0X8E,0XFB,0XC5,
0X75,0XAD,0X34,0XA5,0X9E,0XF7,0X7D,0XEF,0X5F,0XF7,0XC7,0X96,0XD4,0XD7,0X6E,0XBF,
0X81,0X7E,0XC5,0X8E,0XD6,0XDF,0XC5,0X8E,0X09,0X9F,0XF6,0XE7,0XE7,0X96,0XC3,0X86,
0X91,0XC7,0X09,0X9F,0X80,0X76,0X91,0XC7,0X4C,0XAF,0XA3,0X86,0XC4,0X8E,0XC5,0X8E,
0XD7,0XDF,0XFC,0XFF,0XD4,0XD7,0X83,0X86,0X08,0X97,0XF7,0XE7,0XFB,0XF7,0XF7,0XE7,
0XE6,0X96,0X87,0X96,0XFB,0XCD,0X55,0XAD,0X14,0XA5,0XDF,0XFF,0XFF,0XFF,0X5D,0XEF,
0X39,0XDF,0XDC,0XF7,0X18,0XD7,0X39,0XDF,0X9B,0XE7,0X7A,0XE7,0X18,0XD7,0X56,0XC6,
0X76,0XC6,0X18,0XD7,0X9B,0XE7,0X9A,0XE7,0X5A,0XE7,0XD8,0XD6,0X79,0XDF,0XFC,0XFF,
0XFD,0XFF,0X9B,0XEF,0X9A,0XE7,0XFC,0XFF,0XDC,0XF7,0XFC,0XF7,0XFF,0XFF,0XFF,0XFF,
0XFF,0XFF,0XDD,0XF7,0X18,0XDF,0XBA,0XEF,0XD9,0XD6,0XF8,0XBD,0XF7,0XBD,0X55,0XAD,
0X59,0XCE,0X6D,0X6B,0X5D,0XEF,0X9E,0XF7,0X09,0X42,0X7A,0XD6,0XAF,0X7B,0X04,0X21,
0X83,0X10,0X6A,0X4A,0X5A,0XCE,0XD7,0XBD,0X87,0X31,0X05,0X21,0XA3,0X18,0X49,0X4A,
0XBF,0XFF,0X8F,0X73,0XE4,0X20,0X09,0X42,0XFC,0XE6,0X3D,0XEF,0XCB,0X5A,0XFF,0XFF,
0XDC,0XE6,0X87,0X39,0X45,0X29,0X49,0X4A,0X3D,0XEF,0X1D,0XEF,0X4A,0X4A,0XF4,0X9C,
0X9A,0XD6,0X69,0X4A,0X51,0X8C,0X75,0XAD,0X00,0X00,0X5D,0XEF,0XDF,0XFF,0X00,0X00,
0X38,0XC6,0XA2,0X10,0XEF,0X7B,0X9A,0XD6,0X04,0X21,0XA2,0X10,0XB2,0X94,0X00,0X00,
0X34,0XA5,0X34,0XA5,0X38,0XC6,0XB6,0XB5,0X00,0X00,0X14,0X9D,0XEB,0X5A,0X08,0X42,
0X9A,0XD6,0X00,0X00,0X9A,0XD6,0X41,0X08,0XF3,0X9C,0XFF,0XFF,0X10,0X84,0X4D,0X6B,
0XDB,0XDE,0X00,0X00,0X61,0X08,0X75,0XAD,0X20,0X00,0XAE,0X73,0X75,0XAD,0X82,0X10,
0XFF,0XFF,0XFF,0XFF,0X00,0X00,0X38,0XC6,0XE3,0X18,0X75,0XAD,0XFF,0XFF,0XF7,0XBD,
0X00,0X00,0X51,0X8C,0X00,0X00,0X0C,0X63,0X2C,0X63,0X75,0XAD,0XDB,0XDE,0X8A,0X52,
0X08,0X42,0X69,0X4A,0X75,0XAD,0XDB,0XDE,0X00,0X00,0X92,0X94,0XE3,0X18,0XFF,0XFF,
0XDB,0XDE,0X51,0X8C,0X75,0XAD,0X9A,0XD6,0X00,0X00,0X34,0XA5,0X61,0X08,0X00,0X00,
0XEF,0X7B,0X38,0XC6,0X00,0X00,0XEB,0X5A,0X08,0X42,0X82,0X10,0X9A,0XD6,0X82,0X10,
0X65,0X29,0X0C,0X63,0X61,0X08,0X49,0X4A,0XB2,0X94,0X00,0X00,0X08,0X42,0XE7,0X39,
0XCF,0X7B,0XD3,0X9C,0X45,0X29,0X4D,0X6B,0X00,0X00,0X28,0X42,0X9A,0XD6,0X00,0X00,
0X1C,0XE7,0X86,0X31,0X49,0X4A,0X69,0X4A,0X00,0X00,0XC7,0X39,0X38,0XCE,0X00,0X00,
0X1C,0XE7,0XEB,0X5A,0X00,0X00,0XCF,0X7B,0XBE,0XF7,0X38,0XC6,0X34,0XA5,0XF3,0X9C,
0X9A,0XD6,0X3C,0XE7,0X75,0XAD,0XB2,0X94,0X71,0X8C,0XB2,0X94,0X59,0XCE,0X38,0XC6,
0X92,0X94,0XD3,0X9C,0XB2,0X94,0X34,0XA5,0XDB,0XDE,0X55,0XAD,0X14,0XA5,0X96,0XB5,
0X5D,0XEF,0X9E,0XF7,0XB6,0XBD,0XFF,0XFF,0X7D,0XF7,0XB6,0XBD,0X38,0XC6,0X79,0XCE,
0X5D,0XEF,0X3C,0XEF,0X34,0XAD,0XDB,0XD6,0X18,0XC6,0X10,0X84,0XF3,0X9C,};

#endif