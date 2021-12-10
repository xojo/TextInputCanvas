// macTypesForAnsi.h
//
// This file used to be a required on Linux and is now provided only for
// backwards compatibility.
//
// (c) 1997-2015 REAL Software Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef MACTYPESFORANSI_H
#define MACTYPESFORANSI_H

#if UNIX_ANSI
	#include <ctype.h>
#endif

#ifndef __MACTYPES__

typedef unsigned char Boolean;

struct Rect
{
	short top, left, bottom, right;
};
typedef struct Rect Rect;

struct Point
{
	int v, h;
};
typedef struct Point Point;

struct RGBColor
{
	unsigned short red, green, blue;
};
typedef struct RGBColor RGBColor;

typedef char **Handle;
typedef unsigned int OSType;
typedef short OSErr;
typedef char *Ptr;
typedef Ptr GWorldPtr;
typedef unsigned long ResType;
typedef long RgnHandle;
typedef long Size;
typedef unsigned char Style;
typedef unsigned int UInt32;

#ifndef nil
	#define nil 0
#endif

#ifndef NULL
	#define	NULL	0L
#endif

#ifndef TRUE
	#define	TRUE	1
#endif

#ifndef	FALSE
	#define	FALSE	0
#endif

enum {
	#ifndef noErr
		noErr = 0,
	#endif
	ioErr = -36,
	badFileFormat = -208,
};

enum {
	bold  = 1,
	italic = 2,
	underline = 4,
	condense = 8,
	outline = 16
};

#define kCStackBased 0
#define STACK_ROUTINE_PARAMETER(a, b) 0
#define MemError() 0

enum {
																/* modifiers */
	activeFlagBit				= 0,							/* activate? (activateEvt and mouseDown)*/
	btnStateBit					= 7,							/* state of button?*/
	cmdKeyBit					= 8,							/* command key down?*/
	shiftKeyBit					= 9,							/* shift key down?*/
	alphaLockBit				= 10,							/* alpha lock down?*/
	optionKeyBit				= 11,							/* option key down?*/
	controlKeyBit				= 12,							/* control key down?*/
	rightShiftKeyBit			= 13,							/* right shift key down?*/
	rightOptionKeyBit			= 14,							/* right Option key down?*/
	rightControlKeyBit			= 15							/* right Control key down?*/
};


enum {
	activeFlag					= 1 << activeFlagBit,
	btnState					= 1 << btnStateBit,
	cmdKey						= 1 << cmdKeyBit,
	shiftKey					= 1 << shiftKeyBit,
	alphaLock					= 1 << alphaLockBit,
	optionKey					= 1 << optionKeyBit,
	controlKey					= 1 << controlKeyBit,
	rightShiftKey				= 1 << rightShiftKeyBit,
	rightOptionKey				= 1 << rightOptionKeyBit,
	rightControlKey				= 1 << rightControlKeyBit
}; 

#endif // __MACTYPES__
#endif // MACTYPESFORANSI_H

