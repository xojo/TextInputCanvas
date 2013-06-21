#define COCOA 1
#define ALLOW_CARBON_IN_COCOA 1

#define TARGET_CARBON ALLOW_CARBON_IN_COCOA

#ifdef __OBJC__
    #import <Cocoa/Cocoa.h>
#else
	struct NSWindow;
	struct NSView;
#endif

#if ALLOW_CARBON_IN_COCOA
	#import <Carbon/Carbon.h>
#endif

#import <CoreServices/CoreServices.h>
#import <QuickTime/QuickTime.h>
