//
//  RBXTextInputView.m
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#import "RBXTextInputView.h"
#import "TextInputCanvas.h"
#import "TextRange.h"

static const REALCocoaViewCallbacks *sPluginCallbacks;

static void LoadViewCallbacks()
{
	if (sPluginCallbacks) return;
	sPluginCallbacks = REALGetCocoaViewCallbacks();
}

// CreateNSString
// 
//  Creates an NSString from a REALbasic string.
//
// Gets: str - the REALbasic string
// Returns: an NSString
static NSString * CreateNSString( REALstring str )
{
	if (!str) return @"";
	
	return [(id)REALCopyStringCFString( str ) autorelease];
}

// CreateRBString
// 
//  Creates a REALbasic string from an NSString.
// 
// Gets: str - the NSString
// Returns: the REALbasic string
static REALstring CreateRBString( NSString *str )
{
	const char *utf8 = [str UTF8String];
	return REALBuildString( utf8, strlen( utf8 ), kREALTextEncodingUTF8 );
}

// CreateRange
// 
//  Creates a TextRange object from a given NSRange, iff the NSRange is valid.
// 
// Gets: range - the NSRange
// Returns: the appropriate TextRange object
static REALobject CreateRange( NSRange range )
{
	if (range.location == NSNotFound) return NULL;
	return CreateTextRange( range.location, range.length );
}

// MARK: -

@implementation XOJTextInputView

- (id)initWithControlInstance:(REALcontrolInstance)controlInstance {
	if ((self = [super init])) {
		// NO LOCKING OF THE OBJECT HERE TO AVOID CYCLES
		control = controlInstance;
		LoadViewCallbacks();
	}
	
	return self;
}

- (void)cleanup {
	control = NULL;
}

- (void)drawRect:(NSRect)dirtyRect {
	sPluginCallbacks->paint( control, dirtyRect );
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

// MARK: - text input

// -insertText:replacementRange: (NSTextInputClient)
// 
//  "The receiver inserts aString replacing the content specified by
//   replacementRange. aString can be either an NSString or NSAttributedString
//   instance."
//
// Gets: aString - the NSString/NSAttributedString to insert
//       replacementRange - where to insert it (can be NSNotFound)
// Returns: nothing
- (void)insertText:(id)aString replacementRange:(NSRange)replacementRange {
	REALstring rbString;
	if ([aString isKindOfClass:[NSAttributedString class]]) {
		// Intentionally disregarding any formatting
		rbString = CreateRBString( [aString string] );
	} else {
		rbString = CreateRBString(aString);
	}
	
	REALobject rbRange = CreateRange( replacementRange );
	FireInsertText( control, rbString, rbRange );
	
	REALUnlockObject( rbRange );
	REALUnlockString( rbString );
}

// -doCommandBySelector: (NSTextInputClient)
//
//  "The receiver invokes the action specified by aSelector."
//
// Gets: aSelector - the selector to perform
// Returns: nothing
 - (void)doCommandBySelector:(SEL)aSelector {
	if (@selector(noop:) == aSelector) return;
	
	FireDoCommand( control, sel_getName( aSelector ) );
 }

// -setMarkedTextselectedRange:replacementRange: (NSTextInputClient)
//
//  "The receiver inserts aString replacing the content specified by replacementRange.
//   aString can be either an NSString or NSAttributedString instance. selectedRange
//   specifies the selection inside the string being inserted; hence, the location
//   is relative to the beginning of aString. When aString is an NSString, the
//   receiver is expected to render the marked text with distinguishing appearance
//   (i.e. NSTextView renders with -markedTextAttributes)."
//
// Gets: aString - the NSString/NSAttributedString for the marked text
//       selectedRange - selection relative to the marked text
//       replacementRange - content to replace
// Returns: nothing
- (void)setMarkedText:(id)aString selectedRange:(NSRange)selectedRange replacementRange:(NSRange)replacementRange {
	REALstring rbString;
	if ([aString isKindOfClass:[NSAttributedString class]]) {
		// Intentionally disregarding any formatting. We probably shouldn't because
		// AppKit will shove some style data in this.
		rbString = CreateRBString( [aString string] );
	} else {
		rbString = CreateRBString( aString );
	}
	
	REALobject rbReplacementRange = CreateRange( replacementRange );
	REALobject rbSelectionRange = CreateRange( selectedRange );
	FireSetIncompleteText( control, rbString, rbReplacementRange, rbSelectionRange );
	REALUnlockObject( rbSelectionRange );
	REALUnlockObject( rbReplacementRange );
	REALUnlockString( rbString );
}

// -unmarkText (NSTextInputClient)
//
//  "The receiver unmarks the marked text. If no marked text, the invocation of
//   this method has no effect."
// 
// Gets: nothing
// Returns: nothing
- (void)unmarkText {
	FireDiscardIncompleteText( control );
}

// -selectedRange (NSTextInputClient)
//
//  "Returns the selection range. The valid location is from 0 to the document
//   length."
// 
// Gets: nothing
// Returns: the selected range
- (NSRange)selectedRange {
	REALobject range = FireSelectedRange( control );
	if (!range) return NSMakeRange( 0, 0 );
	
	int location, length;
	GetTextRangeInfo( range, &location, &length );
	REALUnlockObject( range );
	
	return NSMakeRange( location, length );
}

// -markedRange (NSTextInputClient)
// 
//  "Returns the marked range. Returns {NSNotFound, 0} if no marked range."
// 
// Gets: nothing
// Returns: the marked range
- (NSRange)markedRange {
	REALobject range = FireIncompleteTextRange( control );
	if (!range) return NSMakeRange( NSNotFound, 0 );
	
	int location, length;
	GetTextRangeInfo( range, &location, &length );
	REALUnlockObject( range );
	
	return NSMakeRange( location, length );
}

// - hasMarkedText (NSTextInputClient)
// 
//  "Returns whether or not the receiver has marked text."
// 
// Gets: nothing
// Returns: whether or not there is marked text
- (BOOL)hasMarkedText {
	NSRange markedTextRange = [self markedRange];
	return markedTextRange.location != NSNotFound;
}

// -attributedSubstringForProposedRange:actualRange: (NSTextInputClient)
// 
//  "Returns attributed string specified by aRange. It may return nil. If non-nil
//   return value and actualRange is non-NULL, it contains the actual range for
//   the return value. The range can be adjusted from various reasons (i.e. adjust
//   to grapheme cluster boundary, performance optimization, etc)."
//
// Gets: aRange - the requested range
//       actualRange - the actual range (output)
// Returns: the text for the actual range
- (NSAttributedString *)attributedSubstringForProposedRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
	int textLength = FireTextLength( control );
	
	// Sanitize the input range, because it might be out of bounds
	if (aRange.length > textLength) aRange.length = textLength;
	if (actualRange) *actualRange = aRange;
	
	REALobject range = CreateTextRange( aRange.location, aRange.length );
	REALstring text = FireTextForRange( control, range );
	NSString *nsText = CreateNSString( text );
	REALUnlockString( text );
	REALUnlockObject( range );
	
	// We only populate a subset of what's actually been drawn, but it ought to
	// be good enough for now.
	REALstring fontName = FireFontNameAtLocation( control, aRange.location );
	int fontSize = FireFontSizeAtLocation( control, aRange.location );
	NSFont *font = nil;
	NSString *nsFontName = CreateNSString( fontName );
	if ([nsFontName isEqualToString:@""] || [nsFontName isEqualToString:@"System"]) {
		if (fontSize == 0) {
			font = [NSFont systemFontOfSize:[NSFont systemFontSize]];
		} else {
			font = [NSFont systemFontOfSize:fontSize];
		}
	} else if ([nsFontName isEqualToString:@"SmallSystem"]) {
		if (fontSize == 0) {
			font = [NSFont systemFontOfSize:[NSFont smallSystemFontSize]];
		} else {
			font = [NSFont systemFontOfSize:fontSize];
		}
	} else {
		if (fontSize == 0) {
			font = [NSFont fontWithName:nsFontName size:[NSFont systemFontSize]];
		} else {
			font = [NSFont fontWithName:nsFontName size:fontSize];
		}
	}
	REALUnlockString( fontName );
	
	NSDictionary *attributes = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName];
	return [[[NSAttributedString alloc] initWithString:nsText attributes:attributes] autorelease];
}

// -validAttributesForMarkedText (NSTextInputClient)
//
//  "Returns an array of attribute names recognized by the receiver."
//
// Gets: nothing
// Returns: the attributes we pay attention to
- (NSArray *)validAttributesForMarkedText {
	// The FadingTextView example returns NSMarkedClauseSegmentAttributeName and
    // NSGlyphInfoAttributeName. However, we wouldn't be able to understand them if
    // the text system gave them to us, so we won't bother saying we can do it.
	return [NSArray array];
}

// -firstRectForCharacterRange:actualRange: (NSTextInputClient)
// 
//  "Returns the first logical rectangular area for aRange. The return value is
//   in the screen coordinate. The size value can be negative if the text flows
//   to the left. If non-NULL, actuallRange contains the character range corresponding
//   to the returned area."
//
// Gets: aRange - the requested range
//       actualRange - the actual range (output)
// Returns: the text rectangle in screen coordinates
- (NSRect)firstRectForCharacterRange:(NSRange)aRange actualRange:(NSRangePointer)actualRange {
	REALobject rbRange = CreateTextRange( aRange.location, aRange.length );
	REALobject rbRect = FireRectForRange( control, &rbRange );
	
	if (rbRange && actualRange) {
		int location, length;
		GetTextRangeInfo( rbRange, &location, &length );
		*actualRange = NSMakeRange( location, length );
	}
	
	NSRect result = NSZeroRect;
	if (rbRect) {
		long /* FIXME: RBInteger */ top, left, width, height;
		REALGetPropValue( rbRect, "Top", &top );
		REALGetPropValue( rbRect, "Left", &left );
		REALGetPropValue( rbRect, "Width", &width );
		REALGetPropValue( rbRect, "Height", &height );
		
		// Now we have our RB control coordinates. Let's transform them into
		// NSView coordinates and then into screen coordinates.
		result = NSMakeRect( left, top, width, height );
		if (![self isFlipped]) {
			result.origin.y = [self bounds].size.height - result.origin.y - result.size.height;
		}
		
		// On 10.7+ use the HiDPI friendly methods
		if ([[self window] respondsToSelector:@selector(convertRectToScreen:)]) {
			result = [self convertRect:result toView:nil];
			result = [[self window] convertRectToScreen:result];
		} else {
			result = [self convertRectToBase:result];
			result.origin = [[self window] convertBaseToScreen:result.origin];
		}
	}
	
	REALUnlockObject( rbRange );
	REALUnlockObject( rbRect );
	return result;
}

// -characterIndexForPoint: (NSTextInputClient)
//
//  "Returns the index for character that is nearest to aPoint. aPoint is in the
//   screen coordinate system."
//
// Gets: aPoint - the point
// Returns: the character index
- (NSUInteger)characterIndexForPoint:(NSPoint)aPoint {
	// Convert from screen coordinates to window coordinates
	NSRect windowFrame = [[self window] frame];
	aPoint.x -= windowFrame.origin.x;
	aPoint.y -= windowFrame.origin.y;
	
	// Now we're in window coordinates. Convert from those to view coordinates
	aPoint = [self convertPoint:aPoint fromView:nil];
	
	// And now flip it if needed so that we preserve the illusion of 0,0 being in the
	// top left of the control.
	if (![self isFlipped]) {
		aPoint.y = [self bounds].size.height - aPoint.y;
	}
	
	return FireCharacterAtPoint( control, aPoint.x, aPoint.y );
}

// -baselineDeltaForCharacterAtIndex: (NSTextInputClient)
//
//  "Returns the baseline position relative to the origin of rectangle returned
//   by -firstRectForCharacterRange:actualRange:. This information allows the caller
//   to access finer-grained character position inside the NSTextInputClient
//   document."
//
// Gets: anIndex - the character index
// Returns: the baseline relative to the rectangle
- (CGFloat)baselineDeltaForCharacterAtIndex:(NSUInteger)anIndex {
	return FireBaselineAtIndex( control, anIndex );
}

// -inputContext
//
//  The input context associated with the view.
// 
// Gets: nothing
// Returns: the input context
- (NSTextInputContext *)inputContext {
    // If it's not editable we need to return nil here so that we don't get any
    // support for entering text.
    if (!FireIsEditable( control )) return nil;
	return [super inputContext];
}

// MARK: - services

- (BOOL)hasSelectedText {
	REALobject selectedRange = FireSelectedRange( control );
	if (selectedRange) {
		int location, length;
		GetTextRangeInfo( selectedRange, &location, &length );
		REALUnlockObject( selectedRange );
		return length != 0;
	}
	
	return NO;
}

- (id)validRequestorForSendType:(NSString *)sendType returnType:(NSString *)returnType {
	BOOL validSendType = !sendType || ([sendType isEqualToString:NSStringPboardType] && [self hasSelectedText]);
	BOOL validReturnType = !returnType || ([returnType isEqualToString:NSStringPboardType] && FireIsEditable( control ));
	
	if (validSendType && validReturnType) return self;
	return [super validRequestorForSendType:sendType returnType:returnType];
}

- (BOOL)writeSelectionToPasteboard:(NSPasteboard *)pboard types:(NSArray *)types {
	if ([types containsObject:NSStringPboardType]) {
		REALobject selectedRange = FireSelectedRange( control );
		REALstring text = FireTextForRange( control, selectedRange );
		
		[pboard setString:CreateNSString( text ) forType:NSStringPboardType];

		REALUnlockObject( selectedRange );
		REALUnlockString( text );
		return YES;
	}
	
	return NO;
}

- (BOOL)readSelectionFromPasteboard:(NSPasteboard *)pboard {
	if (NSString *str = [pboard stringForType:NSStringPboardType]) {
		REALstring rbStr = CreateRBString( str );
		FireInsertText( control, rbStr, NULL );
		REALUnlockString( rbStr );
		return YES;
	}
	
	return NO;
}

// MARK: - event forwarding

- (void)keyDown:(NSEvent *)theEvent {
	[NSCursor setHiddenUntilMouseMoves:YES];
	if ([[self inputContext] handleEvent:theEvent]) return;
	(void)sPluginCallbacks->keyDown( control, theEvent );
}

- (void)keyUp:(NSEvent *)theEvent {
	(void)sPluginCallbacks->keyUp( control, theEvent );
}

- (void)mouseDown:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    (void)sPluginCallbacks->mouseDown( control, theEvent );
}

- (void)rightMouseDown:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    if (!sPluginCallbacks->mouseDown( control, theEvent )) {
		// We want to send the rightMouseDown: to our super so that it can show
		// a contextual menu.
		[super rightMouseDown:theEvent];
	}
}

- (void)otherMouseDown:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    (void)sPluginCallbacks->mouseDown( control, theEvent );
}

- (void)mouseDragged:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    (void)sPluginCallbacks->mouseDragged( control, theEvent );
}

- (void)rightMouseDragged:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    (void)sPluginCallbacks->mouseDragged( control, theEvent );
}

- (void)otherMouseDragged:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    (void)sPluginCallbacks->mouseDragged( control, theEvent );
}

- (void)mouseUp:(NSEvent *)theEvent {
	if ([[self inputContext] handleEvent:theEvent]) return;
    (void)sPluginCallbacks->mouseUp( control, theEvent );
}

- (void)rightMouseUp:(NSEvent *)theEvent {
    (void)sPluginCallbacks->mouseUp( control, theEvent );
}

- (void)otherMouseUp:(NSEvent *)theEvent {
    (void)sPluginCallbacks->mouseUp( control, theEvent );
}

- (void)scrollWheel:(NSEvent *)theEvent {
	(void)sPluginCallbacks->scrollWheel( control, theEvent );
}

- (void)resetCursorRects {
    (void)sPluginCallbacks->resetCursorRects( control );
}

- (NSMenu *)menuForEvent:(NSEvent *)event {
	return sPluginCallbacks->menuForEvent( control, event, [super menuForEvent:event] );
}

- (NSDragOperation)draggingEntered:(id <NSDraggingInfo>)sender {
	NSDragOperation result;
	if (sPluginCallbacks->draggingEntered( control, sender, &result )) {
		return result;
	} else {
		return [super draggingEntered:sender];
	}
}

- (NSDragOperation)draggingUpdated:(id <NSDraggingInfo>)sender {
	NSDragOperation result;
	if (sPluginCallbacks->draggingUpdated( control, sender, &result )) {
		return result;
	} else {
		return [super draggingUpdated:sender];
	}
}

- (void)draggingExited:(id <NSDraggingInfo>)sender {
	if (!sPluginCallbacks->draggingExited( control, sender )) {
		[super draggingExited:sender];
	}
}

- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender {
	BOOL result = NO;
	if (sPluginCallbacks->performDragOperation( control, sender, &result )) {
		return result;
	} else {
		return [super performDragOperation:sender];
	}
}

- (BOOL)wantsPeriodicDraggingUpdates {
	return NO;
}

@end
