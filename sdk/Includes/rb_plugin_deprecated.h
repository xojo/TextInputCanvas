// rb_plugin_deprecated.h
//
// This file is part of the Xojo Plugin SDK, it includes all
// the deprecated functionality that should no longer be used
//
// (c) 2015 Xojo, Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef RB_PLUGIN_DEPRECATED_H
#define RB_PLUGIN_DEPRECATED_H

#if defined(_MSC_VER)
	#define REAL_DEPRECATED __declspec(deprecated)
	#define REAL_DEPRECATED_(reason) __declspec(deprecated(reason))
#elif defined(__GNUC__)
	#define REAL_DEPRECATED __attribute__((deprecated))
	#define REAL_DEPRECATED_(reason) __attribute__((deprecated(reason)))
#else
	#define REAL_DEPRECATED
	#define REAL_DEPRECATED_(reason)
#endif

#if TARGET_64BIT && __cplusplus >= 201103L
       #define REAL_64BIT_UNAVAILABLE = delete
#elif TARGET_64BIT && defined(__clang__)
       #define REAL_64BIT_UNAVAILABLE __attribute__((unavailable("Function is unavailable in 64-bit")))
#elif TARGET_64BIT && defined(_MSC_VER) && _MSC_VER >= 1800 && defined(__cplusplus)
       #define REAL_64BIT_UNAVAILABLE = delete
#else
       #define REAL_64BIT_UNAVAILABLE
#endif

#if !defined(__APPLE__) && !defined(nil)
	// This should be avoided and NULL or nullptr used in its place.
	#define nil 0
#endif

// The plugins SDK has historically provided macros for C/C++'s alternate
// keywords 'not', 'and', and 'or' on Windows because Microsoft's compiler is
// non-conforming by default.
#if defined(_MSC_VER)
	#if defined(__cplusplus)
		#include <ciso646>
	#else
		#include <iso646.h>
	#endif
#endif

#if !defined(__APPLE__)
	typedef unsigned char Boolean;
	typedef char *Ptr;
#endif

#if !defined(__APPLE__) && !defined(MACTYPESFORANSI_H)
	// The plugins SDK used to provide this, even though no functions in the SDK
	// use it.
	struct Point {
		int16_t v, h;
	};
#endif

// define some constants for use with REALGetControlPosition etc.
#define kREALLeft 0
#define kREALTop 1
#define kREALWidth 2
#define kREALHeight 3

// Obsolete control flags which do nothing
#define REALcontrolIsHIViewCompatible (1 << 6) 
#define REALdontTryHIViewize          (1 << 7) 
#define REALcontrolRequiresComposite  (1 << 9)

#define REALpropInvalidate 				(1 << 0)

#define REALenabledControl 				(1 << 4)

#define FieldOffset(type, field) offsetof( type, field )
#ifndef _countof
	#define _countof( x )	((x)?(sizeof( x ) / sizeof( x[ 0 ] )):0)
#endif

typedef struct 
{
	const char *szInterface;
	const char *szDescription;
	const char *szPart;
} REALbindingDescription;

typedef REALobject REALpopupMenu;
typedef REALobject REALmemoryBlock;
typedef REALobject REALsocket;
typedef REALobject REALintArray;
typedef REALobject REALstringArray;
typedef REALobject REALobjectArray;
typedef REALobject REALstructureArray;
typedef REALobject REALgraphics;
typedef REALobject REALpicture;
typedef REALobject REALfolderItem;
typedef REALobject REALappleEvent;
typedef REALobject REALwindow;

#define kREALfontStyleVersion 2

typedef struct
{
	uint32_t version;				// this should always be kREALfontStyleVersion
	REALstring fontName;		// name of the font or metafont (e.g. "System"); may be nil
	uint32_t fontSize;				// font size in points
	uint32_t color;					// color in standard plugin form, i.e. 0x00RRGGBB
	Boolean bold;				// style attributes
	Boolean italic;
	Boolean underline;
	unsigned char fontUnit;		// 0 - Default, 1 - Pixel, 2 - Point, 3 - Inches, 4 - Millimeters
} REALfontStyle;

// socket constants
#define socketEventConnect 1
#define socketEventError 2
#define socketEventDataReceived 4
#define socketEventSendComplete 8

#if defined(__cplusplus)
// Clang had a bug where it erronously thought that deleted functions counted as
// definitions that were missing prototypes.
#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wmissing-prototypes"
#endif

REAL_DEPRECATED_("Use REALGetPropValueInt8 or REALGetPropValueBoolean") Boolean REALGetPropValue(REALobject object, const char *propName, char *outValue) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetPropValueUInt8 or REALGetPropValueBoolean") Boolean REALGetPropValue(REALobject object, const char *propName, unsigned char *outValue) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetPropValueInt16") Boolean REALGetPropValue(REALobject object, const char *propName, short *outValue);
REAL_DEPRECATED_("Use REALGetPropValueUInt16") Boolean REALGetPropValue(REALobject object, const char *propName, unsigned short *outValue);
REAL_DEPRECATED_("Use REALGetPropValueInt32 or REALGetPropValueInteger") Boolean REALGetPropValue(REALobject object, const char *propName, int32_t *outValue) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetPropValueInt32 or REALGetPropValueInteger") Boolean REALGetPropValue(REALobject object, const char *propName, long *outValue) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetPropValueUInt32") Boolean REALGetPropValue(REALobject object, const char *propName, unsigned long *outValue) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetPropValueDouble") Boolean REALGetPropValue(REALobject object, const char *propName, double *outValue);
REAL_DEPRECATED_("Use REALGetPropValueSingle") Boolean REALGetPropValue(REALobject object, const char *propName, float *outValue);
REAL_DEPRECATED_("Use REALGetPropValueString") Boolean REALGetPropValue(REALobject object, const char *propName, REALstring *outValue);
REAL_DEPRECATED_("Use REALGetPropValueObject") Boolean REALGetPropValue(REALobject object, const char *propName, REALobject *outValue);
#if TARGET_CARBON || TARGET_COCOA
	REAL_DEPRECATED_("Use REALGetPropValueCFString") Boolean REALGetPropValue(REALobject object, const char *propName, CFStringRef *outValue);
#endif
#if !(UNIX_ANSI && TARGET_64BIT)
	REAL_DEPRECATED_("Use REALGetPropValueInt64 or REALGetPropValueInteger") Boolean REALGetPropValue(REALobject object, const char *propName, RBInt64 *outValue) REAL_64BIT_UNAVAILABLE;
	REAL_DEPRECATED_("Use REALGetPropValueUInt64") Boolean REALGetPropValue(REALobject object, const char *propName, RBUInt64 *outValue);
#endif

REAL_DEPRECATED_("Use REALSetPropValueString") Boolean REALSetPropValue(REALobject object, const char *propName, REALstring value);
REAL_DEPRECATED_("Use REALSetPropValueDouble") Boolean REALSetPropValue(REALobject object, const char *propName, double value);
REAL_DEPRECATED_("Use REALSetPropValueObject") Boolean REALSetPropValue(REALobject object, const char *propName, REALobject value);
REAL_DEPRECATED_("Use REALSetPropValueUInt8 or REALSetPropValueBoolean") Boolean REALSetPropValue(REALobject object, const char *propName, unsigned char value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetPropValueInt8 or REALSetPropValueBoolean") Boolean REALSetPropValue(REALobject object, const char *propName, char value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetPropValueUInt32") Boolean REALSetPropValue(REALobject object, const char *propName, unsigned long value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetPropValueInt32 or REALSetPropValueInteger") Boolean REALSetPropValue(REALobject object, const char *propName, long value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetPropValueUInt16") Boolean REALSetPropValue(REALobject object, const char *propName, unsigned short value);
REAL_DEPRECATED_("Use REALSetPropValueInt16") Boolean REALSetPropValue(REALobject object, const char *propName, short value);
REAL_DEPRECATED_("Use REALSetPropValueSingle") Boolean REALSetPropValue(REALobject object, const char *propName, float value);
REAL_DEPRECATED_("Use REALSetPropValueCString") Boolean REALSetPropValue(REALobject object, const char *propName, const char *value);
REAL_DEPRECATED_("Use REALSetPropValueWString") Boolean REALSetPropValue(REALobject object, const char *propName, const wchar_t *value);
REAL_DEPRECATED_("Use REALSetPropValuePString") Boolean REALSetPropValue(REALobject object, const char *propName, const unsigned char *value);
#if TARGET_CARBON || TARGET_COCOA
	REAL_DEPRECATED_("Use REALSetPropValueCFString") Boolean REALSetPropValue(REALobject object, const char *propName, CFStringRef value);
#endif
#if !(UNIX_ANSI && TARGET_64BIT)
	REAL_DEPRECATED_("Use REALSetPropValueUInt64 or REALSetPropValueInteger") Boolean REALSetPropValue(REALobject object, const char *propName, RBUInt64 value) REAL_64BIT_UNAVAILABLE;
	REAL_DEPRECATED_("Use REALSetPropValueInt64") Boolean REALSetPropValue(REALobject object, const char *propName, RBInt64 value) REAL_64BIT_UNAVAILABLE;
#endif

REAL_DEPRECATED_("Use REALInsertArrayValueInt32 or REALInsertArrayValueInteger") void REALInsertArrayValue(REALarray arr, RBInteger index, long value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALInsertArrayValueUInt32") void REALInsertArrayValue(REALarray arr, RBInteger index, unsigned long value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALInsertArrayValueInt16") void REALInsertArrayValue(REALarray arr, RBInteger index, short value);
REAL_DEPRECATED_("Use REALInsertArrayValueUInt16") void REALInsertArrayValue(REALarray arr, RBInteger index, unsigned short value);
REAL_DEPRECATED_("Use REALInsertArrayValueInt8 or REALInsertArrayValueBoolean") void REALInsertArrayValue(REALarray arr, RBInteger index, char value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALInsertArrayValueUInt8 or REALInsertArrayValueBoolean") void REALInsertArrayValue(REALarray arr, RBInteger index, unsigned char value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALInsertArrayValueDouble") void REALInsertArrayValue(REALarray arr, RBInteger index, double value);
REAL_DEPRECATED_("Use REALInsertArrayValueSingle") void REALInsertArrayValue(REALarray arr, RBInteger index, float value);
REAL_DEPRECATED_("Use REALInsertArrayValueString") void REALInsertArrayValue(REALarray arr, RBInteger index, REALstring value);
REAL_DEPRECATED_("Use REALInsertArrayValueObject") void REALInsertArrayValue(REALarray arr, RBInteger index, REALobject value);
#if !TARGET_64BIT
	REAL_DEPRECATED_("Use REALInsertArrayValueInt64 or REALInsertArrayValueInteger") void REALInsertArrayValue(REALarray arr, RBInteger index, RBInt64 value) REAL_64BIT_UNAVAILABLE;
	REAL_DEPRECATED_("Use REALInsertArrayValueUInt64") void REALInsertArrayValue(REALarray arr, RBInteger index, RBUInt64 value) REAL_64BIT_UNAVAILABLE;
#endif

REAL_DEPRECATED_("Use REALGetArrayValueInt8 or REALGetArrayValueBoolean") void REALGetArrayValue(REALarray arr, RBInteger index, char *value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetArrayValueUInt8 or REALGetArrayValueBoolean") void REALGetArrayValue(REALarray arr, RBInteger index, unsigned char *value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetArrayValueInt16") void REALGetArrayValue(REALarray arr, RBInteger index, short *value);
REAL_DEPRECATED_("Use REALGetArrayValueUInt16") void REALGetArrayValue(REALarray arr, RBInteger index, unsigned short *value);
REAL_DEPRECATED_("Use REALGetArrayValueInt32 or REALGetArrayValueInteger") void REALGetArrayValue(REALarray arr, RBInteger index, long *value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetArrayValueUInt32") void REALGetArrayValue(REALarray arr, RBInteger index, unsigned long *value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALGetArrayValueString") void REALGetArrayValue(REALarray arr, RBInteger index, REALstring *value);
REAL_DEPRECATED_("Use REALGetArrayValueText") void REALGetArrayValue(REALarray arr, RBInteger index, REALtext *value);
REAL_DEPRECATED_("Use REALGetArrayValueObject") void REALGetArrayValue(REALarray arr, RBInteger index, REALobject *value);
REAL_DEPRECATED_("Use REALGetArrayValueDouble") void REALGetArrayValue(REALarray arr, RBInteger index, double *value);
REAL_DEPRECATED_("Use REALGetArrayValueSingle") void REALGetArrayValue(REALarray arr, RBInteger index, float *value);
#if !(UNIX_ANSI && TARGET_64BIT)
	REAL_DEPRECATED_("Use REALGetArrayValueInt64 or REALGetArrayValueInteger") void REALGetArrayValue(REALarray arr, RBInteger index, RBInt64 *value) REAL_64BIT_UNAVAILABLE;
	REAL_DEPRECATED_("Use REALGetArrayValueUInt64") void REALGetArrayValue(REALarray arr, RBInteger index, RBUInt64 *value) REAL_64BIT_UNAVAILABLE;
#endif

REAL_DEPRECATED_("Use REALSetArrayValueInt8 or REALSetArrayValueBoolean") void REALSetArrayValue(REALarray arr, RBInteger index, char value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetArrayValueUInt8 or REALSetArrayValueBoolean") void REALSetArrayValue(REALarray arr, RBInteger index, unsigned char value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetArrayValueInt16") void REALSetArrayValue(REALarray arr, RBInteger index, short value);
REAL_DEPRECATED_("Use REALSetArrayValueUInt16") void REALSetArrayValue(REALarray arr, RBInteger index, unsigned short value);
REAL_DEPRECATED_("Use REALSetArrayValueInt32 or REALSetArrayVaueInteger") void REALSetArrayValue(REALarray arr, RBInteger index, long value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetArrayValueUInt32") void REALSetArrayValue(REALarray arr, RBInteger index, unsigned long value) REAL_64BIT_UNAVAILABLE;
REAL_DEPRECATED_("Use REALSetArrayValueDouble") void REALSetArrayValue(REALarray arr, RBInteger index, double value);
REAL_DEPRECATED_("Use REALSetArrayValueSingle") void REALSetArrayValue(REALarray arr, RBInteger index, float value);
REAL_DEPRECATED_("Use REALSetArrayValueString") void REALSetArrayValue(REALarray arr, RBInteger index, REALstring value);
REAL_DEPRECATED_("Use REALSetArrayValueText") void REALSetArrayValue(REALarray arr, RBInteger index, REALtext value);
REAL_DEPRECATED_("Use REALSetArrayValueObject") void REALSetArrayValue(REALarray arr, RBInteger index, REALobject value);
#if !(UNIX_ANSI && TARGET_64BIT)
	REAL_DEPRECATED_("Use REALSetArrayValueInt64 or REALSetArrayVaueInteger") void REALSetArrayValue(REALarray arr, RBInteger index, RBInt64 value) REAL_64BIT_UNAVAILABLE;
	REAL_DEPRECATED_("Use REALSetArrayValueUInt64") void REALSetArrayValue(REALarray arr, RBInteger index, RBUInt64 value) REAL_64BIT_UNAVAILABLE;
#endif

#if defined(__clang__)
	#pragma clang diagnostic pop
#endif
#endif	// defined __cplusplus

#if defined( __cplusplus )
	extern "C" {
#endif

REAL_DEPRECATED void *REALLoadGlobalMethod( const char *module, const char *prototype );

// You can use these methods to register a global structure
// or enumeration in much the same way you can register 
// global methods.  Note: be careful when adding anything
// to the global namespace as it could cause conflicts.  Use
// these methods with extreme caution.
REAL_DEPRECATED void REALRegisterStructure( REALstructure *defn );
REAL_DEPRECATED void REALRegisterEnum( REALenum *defn );

REAL_DEPRECATED REALobject REALnewInstance(const char *className);

REAL_DEPRECATED void GraphicsDrawLine(REALgraphics graphicsObject, double x1, double y1, double x2, double y2);
REAL_DEPRECATED void REALRegisterMethod(REALmethodDefinition *defn);
REAL_DEPRECATED void REALRegisterClassExtension(REALclassDefinition *defn);

REAL_DEPRECATED const char *REALCString(REALstring str);
REAL_DEPRECATED const unsigned char *REALPString(REALstring str);

REAL_DEPRECATED_("Use 'Untitled' as the caption") REALstring REALDefaultControlCaption(void);

#if TARGET_WIN32
REAL_DEPRECATED void REALDrawPicturePrimitive(HDC hDC, REALpicture pic, const Rect *rBounds, int bTransparent);
#endif

#if TARGET_CARBON || COCOA
REAL_DEPRECATED void REALDrawPicturePrimitive(REALpicture pic, const Rect *rBounds, int bTransparent);
#endif

#if TARGET_WIN32
REAL_DEPRECATED int REALGetWin32Charset(void);
#endif

#if TARGET_WIN32 || UNIX_ANSI
REAL_DEPRECATED REALfolderItem REALFolderItemFromPath(const char *path);
#endif

#if TARGET_WIN32
REAL_DEPRECATED HWND REALGetControlHWND(REALcontrolInstance control);
#endif

REAL_DEPRECATED void REALInvalidateControl(REALcontrolInstance instance);

REAL_DEPRECATED void REALInvalidateControlRect(REALcontrolInstance instance, int left, int top, int right, int bottom);

#if TARGET_COCOA
REAL_DEPRECATED NSWindow *REALGetWindowHandle(REALwindow window);
#endif

REAL_DEPRECATED void REALMarkSocketUsage(void);

REAL_DEPRECATED void REALSocketConnect(REALsocket socket, REALstring address, int port);

REAL_DEPRECATED void REALSocketClose(REALsocket socket);

REAL_DEPRECATED REALstring REALSocketReadAll(REALsocket socket);

REAL_DEPRECATED REALstring REALSocketRead(REALsocket socket, int count);

REAL_DEPRECATED void REALSocketWrite(REALsocket socket, REALstring data);

REAL_DEPRECATED int REALSocketLastErrorCode(REALsocket socket, int unused);

REAL_DEPRECATED REALstring REALSocketLookahead(REALsocket socket, int unused);

REAL_DEPRECATED REALstring REALSocketLocalAddressGetter(REALsocket socket);

REAL_DEPRECATED void REALSocketPoll(REALsocket socket);

REAL_DEPRECATED int REALSocketGetEvents(REALsocket socket, int unused);

REAL_DEPRECATED void REALMessageBox(REALstring text);

REAL_DEPRECATED REALgraphics REALGetPictureGraphics(REALpicture picture);

REAL_DEPRECATED REALpicture REALNewPicture(RBInteger width, RBInteger height, RBInteger depth);

REAL_DEPRECATED REALmemoryBlock REALNewMemoryBlock(int bytes);

REAL_DEPRECATED void*REALMemoryBlockGetPtr(REALmemoryBlock memBlock);

REAL_DEPRECATED RBInteger REALMemoryBlockGetSize(REALmemoryBlock memBlock);

REAL_DEPRECATED REALmemoryBlock REALPtrToMemoryBlock(void*data);

REAL_DEPRECATED int REALGetArrayInt(REALintArray array, RBInteger index);

REAL_DEPRECATED REALstring REALGetArrayString(REALstringArray array, RBInteger index);

REAL_DEPRECATED REALobject REALGetArrayObject(REALobjectArray array, RBInteger index);

REAL_DEPRECATED Boolean REALGetTabPanelVisible(REALcontrolInstance instance);

#if TARGET_WIN32
REAL_DEPRECATED HWND REALGetWindowHandle(REALwindow window);
#endif

REAL_DEPRECATED Boolean REALGetControlFocus(REALcontrolInstance instance);

REAL_DEPRECATED void REALSetControlFocus(REALcontrolInstance instance, Boolean focus);

REAL_DEPRECATED REALcontrolInstance REALGetControlParent(REALcontrolInstance instance);

REAL_DEPRECATED REALstring REALGetControlName(REALcontrolInstance control);

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REAL_DEPRECATED uint32_t REALGetFontEncoding(const char *fontName);
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REAL_DEPRECATED REALpicture REALGetPictureMask(REALpicture pict, Boolean createIfNil);
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REAL_DEPRECATED void REALGraphicsDrawString(REALgraphics graphics, REALstring str, RBInteger x, RBInteger y, RBInteger width);
#endif

#if TARGET_CARBON || TARGET_COCOA
#ifdef CF_RETURNS_RETAINED
REAL_DEPRECATED CFStringRef REALGetStringCFString(REALstring str, Boolean stripAmpersands) CF_RETURNS_RETAINED;
#else
REAL_DEPRECATED CFStringRef REALGetStringCFString(REALstring str, Boolean stripAmpersands);
#endif
#endif

#if TARGET_CARBON || TARGET_COCOA
REAL_DEPRECATED void REALGetStringSystemStr(REALstring str, Boolean stripAmpersands, Str255 outStr255);
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REAL_DEPRECATED void REALGetGraphicsFontStyle(REALgraphics graphics, REALfontStyle*  outStyle);
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REAL_DEPRECATED void REALSetGraphicsStyle(REALgraphics graphics, REALfontStyle*  styleInfo);
#endif

REAL_DEPRECATED double REALGraphicsStringWidth(REALgraphics graphics, REALstring str);

REAL_DEPRECATED double REALGraphicsStringHeight(REALgraphics graphics, REALstring str, double wrapWidth);

REAL_DEPRECATED double REALGraphicsTextHeight(REALgraphics graphics);

REAL_DEPRECATED double REALGraphicsTextAscent(REALgraphics graphics);

REAL_DEPRECATED void REALSocketListen(REALsocket socket);

#if X_WINDOW
REAL_DEPRECATED unsigned long REALGraphicsDC(REALgraphics context);
#endif

#if COCOA
REAL_DEPRECATED CGContextRef REALGraphicsDC(REALgraphics context);
#endif
		
#if X_WINDOW
REAL_DEPRECATED void REALDrawPicturePrimitive(REALgraphics context, REALpicture pic, const Rect *rBounds, int bTransparent);
#endif

#if TARGET_WIN32
REAL_DEPRECATED REALgraphics REALGetControlGraphicsWithDC(REALcontrolInstance instance, HDC dc);
#endif

#if X_WINDOW
REAL_DEPRECATED void *REALGraphicsGdkDrawable(REALgraphics context);
#endif

#if X_WINDOW
REAL_DEPRECATED unsigned long REALGetWindowHandle(REALwindow window);
#endif

REAL_DEPRECATED REALstring REALpathFromFolderItem(REALfolderItem item);

#if TARGET_WIN32
REAL_DEPRECATED HDC REALGraphicsDC(REALgraphics context);
#endif

REAL_DEPRECATED_("Use REALBuildStringWithEncoding") REALstring REALBuildString(const char *contents, int length) RB_WARN_UNUSED_RESULT;

REAL_DEPRECATED_("Use REALBuildStringWithEncoding") void REALSetStringEncoding(REALstring str, uint32_t encoding);

REAL_DEPRECATED_("Use 'System' for the font") REALstring REALDefaultControlFont(void) RB_WARN_UNUSED_RESULT;

REAL_DEPRECATED_("Use 0 for the font size") uint32_t REALDefaultControlFontSize(void);

REAL_DEPRECATED REALgraphics REALGetControlGraphics(REALcontrolInstance instance);

REAL_DEPRECATED Boolean REALGetControlEnabled(REALcontrolInstance instance);
REAL_DEPRECATED void REALSetControlEnabled(REALcontrolInstance instance, RBInteger unused, Boolean enable);

#if TARGET_COCOA
	REAL_DEPRECATED NSView *REALGetControlHandle(REALcontrolInstance control);
#elif TARGET_WIN32
	REAL_DEPRECATED HWND REALGetControlHandle(REALcontrolInstance control);
#elif X_WINDOW
	REAL_DEPRECATED unsigned long REALGetControlHandle(REALcontrolInstance control);
#endif

REAL_DEPRECATED void REALSetControlPosition(REALcontrolInstance instance, RBInteger which, RBInteger value);
REAL_DEPRECATED RBInteger REALGetControlPosition(REALcontrolInstance instance, RBInteger which);

#if TARGET_COCOA || TARGET_CARBON
REAL_DEPRECATED_("Use REALLoadObjectMethod with CopyHandle") CGImageRef REALCopyPictureCGImage(REALpicture pic) RB_WARN_UNUSED_RESULT;
#endif

#if defined(__cplusplus)
}  // extern "C"
#endif

#if defined(__cplusplus)

#endif // defined(__cplusplus)

#endif	// RB_PLUGIN_DEPRECATED_H
