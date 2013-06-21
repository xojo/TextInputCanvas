// CarbonHeaders.h
//
//	This file is part of the REALbasic plugin API.  You will usually want
//	to use this as the prefix for your Carbon plugin targets.
//
// © 1997-2004 REAL Software Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#define TARGET_API_MAC_CARBON 1
#define CARBON 1

#ifdef __GNUC__
	#include <Carbon/Carbon.h>
	#include <QuickTime/QuickTime.h> // needed because it wasn't included in new Carbon headers
#else
	#include <fenv.h>	// (sometimes needed to avoid a bug in Metrowerks header-finding)
	#include <Carbon.h>
	#include <Movies.h> // needed because it wasn't included in new Carbon headers
#endif