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

struct Point
{
	int v, h;
};

struct RGBColor
{
	unsigned short red, green, blue;
};

typedef char **Handle;
typedef void* Movie;
typedef long OSType;
typedef short OSErr;
typedef char *Ptr;
typedef Ptr GWorldPtr;
typedef unsigned long ResType;
typedef long RgnHandle;
typedef long Size;
typedef unsigned char Style;
typedef unsigned long UInt32;

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

#ifndef noErr
	const short noErr = 0;
#endif
const short ioErr = -36;
const short badFileFormat = -208;


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

