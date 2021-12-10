// WinHeader++.h
//
// This file used to be a required prefix header but is now only included for
// backwards compatibility. There is no longer a need to use prefix headers.
//
// (c) 1997-2015 REAL Software Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef WINHEADER_H
#define WINHEADER_H

#define WINDOWS 1
#include <Windows.h>
#include <assert.h>

#if defined(__cplusplus)
	#include <ciso646>
#else
	#include <iso646.h>
#endif

#if !defined(nil)
	#define nil 0
#endif

typedef unsigned char Boolean;

#endif // WINHEADER_H
