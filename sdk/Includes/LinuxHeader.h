// LinuxHeader.h
//
//	This file is part of the REALbasic plugin API.  You will usually want
//	to use this as the prefix for your Linux plugin targets.
//	Use the gcc -include option to add this prefix when compiling.
//	Use -DWIDGET_GTK=1 when compiling plugins that use GTK.
//
// (c) 2003-2005 REAL Software Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef LINXUHEADER_H
#define LINUXHEADER_H

#define UNIX_ANSI 1
#define X_WINDOW 1

#ifndef NULL
	#define NULL 0
#endif

#ifndef assert
	#define assert(cond) if (!(cond));
	#define debugAssert(cond) assert(cond)
	#define assertMsg(cond, msg) assert(cond)
#endif

#include "macTypesForAnsi.h"

namespace QT {
	typedef void *MovieController;
	typedef void *Movie;
};


#endif	// LINUXHEADER_H

