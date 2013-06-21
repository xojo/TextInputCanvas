// WinHeader++.h
//
// This file declares macros and functions needed for Win32 compilation
// of plugins using the REALbasic Plugin SDK.  This file should be
// set as the prefix file for your Windows (x86) target.  In addition,
// you'll need to add the "Win32-x86 Support" folder to the system
// access paths.
//
// WinHeader.cpp does not include the C++ precompiled headers, 
// but WinHeader++.cpp does.  The only noticeable difference is 
// if you are using COM.  If you include WinHeader.cpp you have
// to access a COM class' member functions via the vtable pointer,
// but with WinHeader++.cpp it is seamless.
//
// © 1997-2004 REAL Software Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef WINHEADER_H
#define WINHEADER_H

#ifndef WINDOWS
	#define WINDOWS 1
#endif

#include <windows.h>
#include <assert.h>

// If we're visual studio, then we need
// to define some handy macros
#ifdef _MSC_VER
	#define	not		!
	#define	and		&&
	#define	or		||
#endif

#ifndef IGNOREQT
namespace QT {
	#include <QTML.h>
	#include <Movies.h>
}
#else
#define QT
typedef void* Movie;
typedef void* MovieController;
#endif

typedef unsigned char Boolean;
typedef char *Ptr;
struct Rect
{
	short top, left, bottom, right;
};

struct Point
{
	short v, h;
};

#ifdef nil
  #undef nil
#endif
#define nil 0

#ifndef true
	#define true 1
	#define false 0
#endif

#endif

