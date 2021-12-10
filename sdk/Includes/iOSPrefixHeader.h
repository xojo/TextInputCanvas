// iOSPrefixHeader.h
//
// A convenient prefix header for iOS
//
// (c) 2019 REAL Software Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#include <TargetConditionals.h>
#include <MacTypes.h>
#include <CoreFoundation/CoreFoundation.h>

#ifdef TARGET_CARBON
    #undef TARGET_CARBON
#endif
