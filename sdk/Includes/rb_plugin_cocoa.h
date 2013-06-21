// rb_plugin.h
//
// This file is part of the REALbasic plugin API.  Include this file
// at the top of your plugin source files.
//
// (c) 2013 Xojo, Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef RB_PLUGIN_COCOA_H
#define RB_PLUGIN_COCOA_H

#if !__OBJC__
	#error This file must be compiled as Objective-C or Objective-C++.
#endif

#import <AppKit/AppKit.h>
#import "REALplugin.h"

struct REALCocoaViewCallbacks {
	// The version of the callback structure, which is currently 0. If additional
	// fields are added, this will be increased.
	long version;
	
	// These should be invoked from the appropriate NSResponder override. If the
	// function returns YES, you should not continue into your own code because
	// the framework (or user) wants to take over the event's handling.
	BOOL (* mouseDown)(REALcontrolInstance control, NSEvent *event);
	BOOL (* mouseDragged)(REALcontrolInstance control, NSEvent *event);
	BOOL (* mouseUp)(REALcontrolInstance control, NSEvent *event);
	BOOL (* scrollWheel)(REALcontrolInstance control, NSEvent *event);
	BOOL (* keyDown)(REALcontrolInstance control, NSEvent *event);
	BOOL (* keyUp)(REALcontrolInstance control, NSEvent *event);
	
	// Invoke this from within your view's resetCursorRects in order to have
	// the MouseCursor property be respected.
	BOOL (* resetCursorRects)(REALcontrolInstance control);
	
	// This should be invoked from menuForEvent. You should either pass in the
	// result of [super menuForEvent:] into baseMenu or your own menu. The result
	// of this function is the menu the user constructed or the baseMenu if they
	// didn't care.
	NSMenu * (* menuForEvent)(REALcontrolInstance control, NSEvent *event, NSMenu *baseMenu);
	
	// Implement these to get drag and drop for free.
	BOOL (* draggingEntered)(REALcontrolInstance control, id<NSDraggingInfo> sender, NSDragOperation *outOperation);
	BOOL (* draggingUpdated)(REALcontrolInstance control, id<NSDraggingInfo> sender, NSDragOperation *outOperation);
	BOOL (* draggingExited)(REALcontrolInstance control, id<NSDraggingInfo> sender);
	BOOL (* performDragOperation)(REALcontrolInstance control, id<NSDraggingInfo> sender, BOOL *outResult);
	
	// Invoke this to dispatch from your drawRect: method to have the Paint event
	// routed to your control.
	void (* paint)(REALcontrolInstance control, NSRect rect);
};
typedef struct REALCocoaViewCallbacks REALCocoaViewCallbacks;

/**
 * Obtains the program's view callback structure.
 *
 * @return The view callback structure. This will always return the same value,
 *         so it safely can be cached.
 */
const REALCocoaViewCallbacks * REALGetCocoaViewCallbacks();

/**
 * A giant macro which takes care of forwarding the neccessary NSView methods to
 * the framework and then the superclass if needed.
 *
 * @param control The REALcontrolInstance variable to use.
 */
#define REAL_VIEW_MIXINS(control) \
- (void)mouseDown:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseDown( control, theEvent )) return; \
	[super mouseDown:theEvent]; \
} \
\
- (void)rightMouseDown:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseDown( control, theEvent )) return; \
	[super rightMouseDown:theEvent]; \
} \
\
- (void)otherMouseDown:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseDown( control, theEvent )) return; \
	[super otherMouseDown:theEvent]; \
} \
\
- (void)mouseDragged:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseDragged( control, theEvent )) return; \
	[super mouseDragged:theEvent]; \
} \
\
- (void)rightMouseDragged:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseDragged( control, theEvent )) return; \
	[super rightMouseDragged:theEvent]; \
} \
\
- (void)otherMouseDragged:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseDragged( control, theEvent )) return; \
	[super otherMouseDragged:theEvent]; \
} \
\
- (void)mouseUp:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseUp( control, theEvent )) return; \
	[super mouseUp:theEvent]; \
} \
\
- (void)rightMouseUp:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseUp( control, theEvent )) return; \
	[super rightMouseUp:theEvent]; \
} \
\
- (void)otherMouseUp:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->mouseUp( control, theEvent )) return; \
	[super otherMouseUp:theEvent]; \
} \
\
- (void)scrollWheel:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->scrollWheel( control, theEvent )) return; \
	[super scrollWheel:theEvent]; \
} \
\
- (void)keyDown:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->keyDown( control, theEvent )) return; \
	[super keyDown:theEvent]; \
} \
\
- (void)keyUp:(NSEvent *)theEvent { \
	if (REALGetCocoaViewCallbacks()->keyUp( control, theEvent )) return; \
	[super keyUp:theEvent]; \
} \
\
- (void)resetCursorRects { \
	if (REALGetCocoaViewCallbacks()->resetCursorRects( control )) return; \
	[super resetCursorRects]; \
} \
\
- (NSMenu *)menuForEvent:(NSEvent *)event { \
	NSMenu *base = [super menuForEvent:event]; \
	return REALGetCocoaViewCallbacks()->menuForEvent( control, event, base ); \
}

/**
 * A giant macro which takes care of forwarding the neccessary drag and drop
 * methods to the superclass if needed.
 *
 * @param control The REALcontrolInstance variable to use.
 * @param myClass The name of the class that this macro will be put in. This is
 *                required because the drop target protocol's methods are all 
 *                optional and messages cannot simply be forwarded.
 */
#define REAL_DND_MIXINS(control, myClass) \
- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender { \
	NSDragOperation operation; \
	if (REALGetCocoaViewCallbacks()->draggingEntered( control, sender, &operation )) { \
		return operation; \
	} else if ([[myClass superclass] instancesRespondToSelector:_cmd]) { \
		return [super draggingEntered:sender]; \
	} else { \
		return NSDragOperationNone; \
	} \
} \
\
- (NSDragOperation)draggingUpdated:(id <NSDraggingInfo>)sender { \
	NSDragOperation operation; \
	if (REALGetCocoaViewCallbacks()->draggingUpdated( control, sender, &operation )) { \
		return operation; \
	} else if ([[myClass superclass] instancesRespondToSelector:_cmd]) { \
		return [super draggingUpdated:sender]; \
	} else { \
		return NSDragOperationNone; \
	} \
} \
\
- (void)draggingExited:(id <NSDraggingInfo>)sender { \
	if (REALGetCocoaViewCallbacks()->draggingExited( control, sender )) { \
		return; \
	} else if ([[myClass superclass] instancesRespondToSelector:_cmd]) { \
		[super draggingExited:sender]; \
	} \
} \
\
- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender { \
	BOOL result; \
	if (REALGetCocoaViewCallbacks()->performDragOperation( control, sender, &result )) { \
		return result; \
	} else if ([[myClass superclass] instancesRespondToSelector:_cmd]) { \
		return [super performDragOperation:sender]; \
	} else { \
		return NO; \
	} \
}


#endif
