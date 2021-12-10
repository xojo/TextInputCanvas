//
//  TextInputCanvas.cpp
//
//  (c) 2021 Xojo, Inc. -- All Rights Reserved
//

#include <string.h>
#include "TextInputCanvas.h"
#if COCOA
	#if !__OBJC__
		#error File must be compiled as Objective-C++
	#endif
	
	#import "RBXTextInputView.h"
#elif !TARGET_MACOS
	#if WINDOWS
		enum {
			shiftKey					= 1 << 9,
			optionKey					= 1 << 11,
			controlKey					= 1 << 12,
		};
		#include "TextInputWindows.h"
	#elif WIDGET_GTK
		#include "TextInputGTK.h"
		#include "UnicodeHelper.h"
	#endif

#endif // WINDOWS

static REALclassRef sRectClass;
static REALclassRef sXojoRectClass;
static REALclassRef sDesktopUIControl;

static void InvalidateTextRects( REALcontrolInstance control );
static void LinuxSelectInputMethod( REALcontrolInstance control );

static RBBoolean KeyDownEvent( REALcontrolInstance control, int charCode, int keyCode, int modifiers );
#if WIDGET_GTK
static bool IsTextKey( int keycode, int modifiers );
	static RBBoolean UnfilteredKeyDownEvent( REALcontrolInstance control, int keyCode, int modifiers );
#endif
static void Initializer( REALcontrolInstance control );
static void OpenEvent( REALcontrolInstance control );
static void CloseEvent( REALcontrolInstance control );
static void *HandleGetter( REALcontrolInstance control );
static RBBoolean MouseDown( REALcontrolInstance inst, int x, int y, int modifiers );
static void MouseDrag( REALcontrolInstance inst, int x, int y );
static void MouseUp( REALcontrolInstance inst, int x, int y );
static void GotFocus( REALcontrolInstance control );
static void LostFocus( REALcontrolInstance control );
static void EnableMenuItems( REALcontrolInstance control );
static void ScaleFactorChanged( REALcontrolInstance control, double newScaleFactor );

static bool IsDesktopUIControl(REALcontrolInstance instance)
{
	if (sDesktopUIControl && REALObjectIsA(instance, sDesktopUIControl)) {
		return true;
	}

	return false;
}

static void *GetControlData(REALcontrolInstance instance, REALcontrol *api2Definition, REALcontrol *api1Definition)
{
	if (IsDesktopUIControl(instance)) {
		return REALGetControlData(instance, api2Definition);
	}

	return REALGetControlData(instance, api1Definition);
}

static void *GetEventHook(REALcontrolInstance instance, REALevent *api2Event, REALevent *api1Event)
{
	if (IsDesktopUIControl(instance)) {
		return REALGetEventInstance(instance, api2Event);
	}

	return REALGetEventInstance(instance, api1Event);
}

static void FirePaintStub( REALcontrolInstance control, REALgraphics gfx )
{
	FirePaint( control, gfx, nullptr, 0 );
}

// MARK: - TextInputCanvas declarations

struct InputCanvasData {
#if COCOA
	XOJTextInputView *view;
#elif WINDOWS
	TextInputWindows *view;
#elif WIDGET_GTK
	TextInputGTK *view;
#else
	void *padding;
#endif	
};

static REALmethodDefinition sInputCanvasMethods[] = {
	{ (REALproc)InvalidateTextRects, nullptr, "InvalidateTextRects()" },
	{ (REALproc)LinuxSelectInputMethod, nullptr, "LinuxSelectInputMethod()" }
};

static REALevent sInputCanvasEvents[] = {
	{ "BaselineAtIndex( index as integer ) as integer" },
	{ "CharacterAtPoint( x as integer, y as integer ) as integer" },
	{ "DiscardIncompleteText()" },
	{ "DoCommand( command as string ) as boolean" },
	{ "FontNameAtLocation( location as integer ) as string" },
	{ "FontSizeAtLocation( location as integer ) as integer" },
	{ "IncompleteTextRange() as TextRange" },
	{ "SetIncompleteText( text as string, replacementRange as TextRange, relativeSelection as TextRange )" },
	{ "InsertText( text as string, range as TextRange )" },
	{ "IsEditable() as boolean" },
	{ "KeyFallsThrough( key as string ) as boolean" },
	{ "Paint( g as Graphics, areas() as object )" },
	{ "RectForRange( byref range as TextRange ) as REALbasic.Rect" },
	{ "SelectedRange() as TextRange" },
	{ "TextForRange( range as TextRange ) as string " },
	{ "TextLength() as integer" },
	{ "MouseDown(x as Integer, y as Integer) as Boolean" },
	{ "MouseDrag(x as Integer, y as Integer)" },
	{ "MouseUp(x as Integer, y as Integer)" },
	{ "GotFocus()" },
	{ "LostFocus()" },
	{ "EnableMenuItems()" }
};

static REALevent sDesktopInputCanvasEvents[] = {
	{ "BaselineAtIndex( index as integer ) as integer" },
	{ "CharacterAtPoint( x as double, y as double ) as integer" },
	{ "DiscardIncompleteText()" },
	{ "DoCommand( command as string ) as boolean" },
	{ "FontNameAtLocation( location as integer ) as string" },
	{ "FontSizeAtLocation( location as integer ) as integer" },
	{ "IncompleteTextRange() as TextRange" },
	{ "SetIncompleteText( text as string, replacementRange as TextRange, relativeSelection as TextRange )" },
	{ "InsertText( text as string, range as TextRange )" },
	{ "IsEditable() as boolean" },
	{ "KeyFallsThrough( key as string ) as boolean" },
	{ "Paint( g as Graphics, areas() as Xojo.Rect )" },
	{ "RectForRange( byref range as TextRange ) as Xojo.Rect" },
	{ "SelectedRange() as TextRange" },
	{ "TextForRange( range as TextRange ) as string " },
	{ "TextLength() as integer" },
	{ "MouseDown(x as Integer, y as Integer) as Boolean" },
	{ "MouseDrag(x as Integer, y as Integer)" },
	{ "MouseUp(x as Integer, y as Integer)" },
	{ "FocusReceived()" },
	{ "FocusLost()" },
	{ "MenuBarSelected()" },
	{ "ScaleFactorChanged()" }
};

static REALconstant sInputCanvasConstants[] = {
	#define CMD_CONST( x, y ) { #x " = \"" #y "\"" }
	
	// NSResponder: Selection movement and scrolling
	CMD_CONST(CmdMoveForward, moveForward:),
	CMD_CONST(CmdMoveRight, moveRight:),
	CMD_CONST(CmdMoveBackward, moveBackward:),
	CMD_CONST(CmdMoveLeft, moveLeft:),
	CMD_CONST(CmdMoveUp, moveUp:),
	CMD_CONST(CmdMoveDown, moveDown:),
	CMD_CONST(CmdMoveWordForward, moveWordForward:),
	CMD_CONST(CmdMoveWordBackward, moveWordBackward:),
	CMD_CONST(CmdMoveToBeginningOfLine, moveToBeginningOfLine:),
	CMD_CONST(CmdMoveToEndOfLine, moveToEndOfLine:),
	CMD_CONST(CmdMoveToBeginningOfParagraph, moveToBeginningOfParagraph:),
	CMD_CONST(CmdMoveToEndOfParagraph, moveToEndOfParagraph:),
	CMD_CONST(CmdMoveToEndOfDocument, moveToEndOfDocument:),
	CMD_CONST(CmdMoveToBeginningOfDocument, moveToBeginningOfDocument:),
	CMD_CONST(CmdPageDown, pageDown:),
	CMD_CONST(CmdPageUp, pageUp:),
	CMD_CONST(CmdCenterSelectionInVisibleArea, centerSelectionInVisibleArea:),
	
	// NSResponder: Selection movement and scrolling
	CMD_CONST(CmdMoveBackwardAndModifySelection, moveBackwardAndModifySelection:),
	CMD_CONST(CmdMoveForwardAndModifySelection, moveForwardAndModifySelection:),
	CMD_CONST(CmdMoveWordForwardAndModifySelection, moveWordForwardAndModifySelection:),
	CMD_CONST(CmdMoveWordBackwardAndModifySelection, moveWordBackwardAndModifySelection:),
	CMD_CONST(CmdMoveUpAndModifySelection, moveUpAndModifySelection:),
	CMD_CONST(CmdMoveDownAndModifySelection, moveDownAndModifySelection:),
	
	// NSResponder: Selection movement and scrolling
	CMD_CONST(CmdMoveToBeginningOfLineAndModifySelection, moveToBeginningOfLineAndModifySelection:),
	CMD_CONST(CmdMoveToEndOfLineAndModifySelection, moveToEndOfLineAndModifySelection:),
	CMD_CONST(CmdMoveToBeginningOfParagraphAndModifySelection, moveToBeginningOfParagraphAndModifySelection:),
	CMD_CONST(CmdMoveToEndOfParagraphAndModifySelection, moveToEndOfParagraphAndModifySelection:),
	CMD_CONST(CmdMoveToEndOfDocumentAndModifySelection, moveToEndOfDocumentAndModifySelection:),
	CMD_CONST(CmdMoveToBeginningOfDocumentAndModifySelection, moveToBeginningOfDocumentAndModifySelection:),
	CMD_CONST(CmdPageDownAndModifySelection, pageDownAndModifySelection:),
	CMD_CONST(CmdPageUpAndModifySelection, pageUpAndModifySelection:),
	CMD_CONST(CmdMoveParagraphForwardAndModifySelection, moveParagraphForwardAndModifySelection:),
	CMD_CONST(CmdMoveParagraphBackwardAndModifySelection, moveParagraphBackwardAndModifySelection:),

	// NSResponder: Selection movement and scrolling (added in 10.3)
	CMD_CONST(CmdMoveWordRight, moveWordRight:),
	CMD_CONST(CmdMoveWordLeft, moveWordLeft:),
	CMD_CONST(CmdMoveRightAndModifySelection, moveRightAndModifySelection:),
	CMD_CONST(CmdMoveLeftAndModifySelection, moveLeftAndModifySelection:),
	CMD_CONST(CmdMoveWordRightAndModifySelection, moveWordRightAndModifySelection:),
	CMD_CONST(CmdMoveWordLeftAndModifySelection, moveWordLeftAndModifySelection:),

	// NSResponder: Selection movement and scrolling (added in 10.6)
	CMD_CONST(CmdMoveToLeftEndOfLine, moveToLeftEndOfLine:),
	CMD_CONST(CmdMoveToRightEndOfLine, moveToRightEndOfLine:),
	CMD_CONST(CmdMoveToLeftEndOfLineAndModifySelection, moveToLeftEndOfLineAndModifySelection:),
	CMD_CONST(CmdMoveToRightEndOfLineAndModifySelection, moveToRightEndOfLineAndModifySelection:),
	
	// NSResponder: Selection movement and scrolling
	CMD_CONST(CmdScrollPageUp, scrollPageUp:),
	CMD_CONST(CmdScrollPageDown, scrollPageDown:),
	CMD_CONST(CmdScrollLineUp, scrollLineUp:),
	CMD_CONST(CmdScrollLineDown, scrollLineDown:),

	// NSResponder: Selection movement and scrolling
	CMD_CONST(CmdScrollToBeginningOfDocument, scrollToBeginningOfDocument:),
	CMD_CONST(CmdScrollToEndOfDocument, scrollToEndOfDocument:),

	// NSResponder: Graphical Element transposition
	CMD_CONST(CmdTranspose, transpose:),
	CMD_CONST(CmdTransposeWords, transposeWords:),

	// NSResponder: Selections
	CMD_CONST(CmdSelectAll, selectAll:),
	CMD_CONST(CmdSelectParagraph, selectParagraph:),
	CMD_CONST(CmdSelectLine, selectLine:),
	CMD_CONST(CmdSelectWord, selectWord:),
	
	// NSResponder: Insertions and Indentations
	CMD_CONST(CmdIndent, indent:),
	CMD_CONST(CmdInsertTab, insertTab:),
	CMD_CONST(CmdInsertBacktab, insertBacktab:),
	CMD_CONST(CmdInsertNewline, insertNewline:),
	CMD_CONST(CmdInsertBreakCommand, insertBreakCommand:),
	CMD_CONST(CmdInsertParagraphSeparator, insertParagraphSeparator:),
	CMD_CONST(CmdInsertNewlineIgnoringFieldEditor, insertNewlineIgnoringFieldEditor:),
	CMD_CONST(CmdInsertTabIgnoringFieldEditor, insertTabIgnoringFieldEditor:),
	CMD_CONST(CmdInsertLineBreak, insertLineBreak:),
	CMD_CONST(CmdInsertContainerBreak, insertContainerBreak:),
	CMD_CONST(CmdInsertSingleQuoteIgnoringSubstitution, insertSingleQuoteIgnoringSubstitution:),
	CMD_CONST(CmdInsertDoubleQuoteIgnoringSubstitution, insertDoubleQuoteIgnoringSubstitution:),
	
	// NSResponder: Case changes
	CMD_CONST(CmdChangeCaseOfLetter, changeCaseOfLetter),
	CMD_CONST(CmdUppercaseWord, uppercaseWord:),
	CMD_CONST(CmdLowercaseWord, lowercaseWord:),
	CMD_CONST(CmdCapitalizeWord, capitalizeWord:),
	
	// NSResponder: Deletions
	CMD_CONST(CmdDeleteForward, deleteForward:),
	CMD_CONST(CmdDeleteBackward, deleteBackward:),
	CMD_CONST(CmdDeleteBackwardByDecomposingPreviousCharacter, deleteBackwardByDecomposingPreviousCharacter:),
	CMD_CONST(CmdDeleteWordForward, deleteWordForward:),
	CMD_CONST(CmdDeleteWordBackward, deleteWordBackward:),
	CMD_CONST(CmdDeleteToBeginningOfLine, deleteToBeginningOfLine:),
	CMD_CONST(CmdDeleteToEndOfLine, deleteToEndOfLine:),
	CMD_CONST(CmdDeleteToBeginningOfParagraph, deleteToBeginningOfParagraph:),
	CMD_CONST(CmdDeleteToEndOfParagraph, deleteToEndOfParagraph:),
	CMD_CONST(CmdYank, yank:),
	
	// NSResponder: Completion
	CMD_CONST(CmdComplete, complete:),
	
	// NSResponder: Mark/Point manipulation
	CMD_CONST(CmdSetMark, setMark:),
	CMD_CONST(CmdDeleteToMark, deleteToMark:),
	CMD_CONST(CmdSelectToMark, selectToMark:),
	CMD_CONST(CmdSwapWithMark, swapWithMark:),
	
	// NSResponder: Cancellation
	CMD_CONST(CmdCancelOperation, cancelOperation:),

	// NSResponder: Writing Direction
	CMD_CONST(CmdMakeBaseWritingDirectionNatural, makeBaseWritingDirectionNatural:),
	CMD_CONST(CmdMakeBaseWritingDirectionLeftToRight, makeBaseWritingDirectionLeftToRight:),
	CMD_CONST(CmdMakeBaseWritingDirectionRightToLeft, makeBaseWritingDirectionRightToLeft:),
	CMD_CONST(CmdMakeTextWritingDirectionNatural, makeTextWritingDirectionNatural:),
	CMD_CONST(CmdMakeTextWritingDirectionLeftToRight, makeTextWritingDirectionLeftToRight:),
	CMD_CONST(CmdMakeTextWritingDirectionRightToLeft, makeTextWritingDirectionRightToLeft:),

	// Not part of NSResponder, something custom we need for Windows
	CMD_CONST(CmdToggleOverwriteMode, rbToggleOverwriteMode),
	CMD_CONST(CmdCopy, rbCopy),
	CMD_CONST(CmdCut, rbCut),
	CMD_CONST(CmdPaste, rbPaste),
	CMD_CONST(CmdUndo, rbUndo)
};

static REALcontrolBehaviour sInputCanvasBehavior = {
	Initializer,
	nullptr, // destructorFunction
	FirePaintStub,
	MouseDown,
	MouseDrag,
	MouseUp,
	GotFocus,
	LostFocus,
	KeyDownEvent,
	OpenEvent,
	CloseEvent,
	nullptr, // backgroundIdleFunction
	nullptr, // drawOffscreenFunction
	nullptr, // setSpecialBackground
	nullptr, // constantChanging
	nullptr, // droppedNewInstance
	nullptr, // mouseEnterFunction
	nullptr, // mouseExitFunction
	nullptr, // mouseMoveFunction
	nullptr, // stateChangedFunction
	nullptr, // actionEventFunction
	HandleGetter,
	nullptr, // mouseWheelFunction
	EnableMenuItems,
	nullptr, // menuItemActionFunction
	nullptr, // controlAcceptFocus
	nullptr, // keyUpFunction,
	FirePaint,
#if WIDGET_GTK
	UnfilteredKeyDownEvent,
#else
	nullptr,
#endif
	ScaleFactorChanged,
};

static REALcontrol sInputCanvasControl = {
	kCurrentREALControlVersion,
	"TextInputCanvas",
	sizeof(InputCanvasData),
	REALcontrolAcceptFocus | REALcontrolHandlesKeyboardNavigation, // flags
	0, 0, // unused (toolbar pict)
	100, 100, // default width/height
	nullptr, 0, // no properties
	sInputCanvasMethods, sizeof(sInputCanvasMethods) / sizeof(sInputCanvasMethods[0]),
	sInputCanvasEvents, sizeof(sInputCanvasEvents) / sizeof(sInputCanvasEvents[0]),
	&sInputCanvasBehavior,
	0, // forSystemUse
	nullptr, 0, // event instances
	nullptr, // interfaces
	nullptr, 0, // attributes
	sInputCanvasConstants, sizeof(sInputCanvasConstants) / sizeof(sInputCanvasConstants[0])
};

static REALcontrol sDesktopInputCanvasControl = {
	kCurrentREALControlVersion,
	"DesktopTextInputCanvas",
	sizeof(InputCanvasData),
	REALdesktopControl | REALcontrolAcceptFocus | REALcontrolHandlesKeyboardNavigation, // flags
	0, 0, // unused (toolbar pict)
	100, 100, // default width/height
	nullptr, 0, // no properties
	sInputCanvasMethods, sizeof(sInputCanvasMethods) / sizeof(sInputCanvasMethods[0]),
	sDesktopInputCanvasEvents, sizeof(sDesktopInputCanvasEvents) / sizeof(sDesktopInputCanvasEvents[0]),
	&sInputCanvasBehavior,
	0, // forSystemUse
	nullptr, 0, // event instances
	nullptr, // interfaces
	nullptr, 0, // attributes
	sInputCanvasConstants, sizeof(sInputCanvasConstants) / sizeof(sInputCanvasConstants[0])
};

void RegisterTextInputCanvasControl()
{
	REALRegisterControl( &sInputCanvasControl );
	REALRegisterControl( &sDesktopInputCanvasControl );
	sRectClass = REALGetClassRef( "REALbasic.Rect" );
	sXojoRectClass = REALGetClassRef( "Xojo.Rect" );
	sDesktopUIControl = REALGetClassRef( "DesktopUIControl" );
}

// MARK: - TextInputCanvas event forwarding

// FireBaselineAtIndex
// 
//  Triggers the user's BaselineAtIndex event. The user should return the view-local
//  y coordinate of the baseline for the text at the given character index.
// 
// Gets: control - the control
//       index - the zero-based character index
// Returns: the y coordinate, in the control's coordinate space
int FireBaselineAtIndex( REALcontrolInstance control, int index )
{
	RBInteger (* fp)(REALcontrolInstance, RBInteger);
	fp = (RBInteger (*)(REALcontrolInstance, RBInteger))GetEventHook( control, &sDesktopInputCanvasEvents[0], &sInputCanvasEvents[0] );
	if (fp) {
		return fp( control, index );
	}
	
	return 0;
}

// FireCharacterAtPoint
// 
//  Triggers the user's FireCharacterAtPoint event. The user should return the
//  zero-based character index that exists at the given point in view-local 
//  coordinates.
// 
// Gets: control - the control
//       x - the x position
//       y - the y position
// Returns: the zero-based character index closest to the point
int FireCharacterAtPoint( REALcontrolInstance control, double x, double y )
{
	if (IsDesktopUIControl(control)) {
		RBInteger (* fp)(REALcontrolInstance, double, double);
		fp = (RBInteger (*)(REALcontrolInstance, double, double))REALGetEventInstance(control, &sDesktopInputCanvasEvents[1]);
		if (fp) {
			return fp( control, x, y );
		}
	} else {
		RBInteger (* fp)(REALcontrolInstance, RBInteger, RBInteger);
		fp = (RBInteger (*)(REALcontrolInstance, RBInteger, RBInteger))REALGetEventInstance(control, &sInputCanvasEvents[1]);
		if (fp) {
			return fp( control, x, y );
		}
	}
	
	return 0;
}

// FireCharacterAtPoint
// 
//  Triggers the user's DiscardIncompleteText event. This is called when the
//  system wishes to discard the incomplete text.
// 
// Gets: control - the control
// Returns: nothing
void FireDiscardIncompleteText( REALcontrolInstance control )
{
	void (* fp)(REALcontrolInstance);
	fp = (void (*)(REALcontrolInstance ))GetEventHook( control, &sDesktopInputCanvasEvents[2], &sInputCanvasEvents[2] );
	if (fp) {
		return fp( control );
	}
}

// FireDoCommand
// 
//  Triggers the user's DoCommand event. This is used for sending arbitrary
//  higher-level commands to the control like "move to the beginning of the
//  document". The list of expected commands are exposed as class constants
//  beginning with Cmd (e.g. CmdMoveWordForward). 
// 
// Gets: control - the control
//       command - the command to perform
// Returns: whether or not the implementor handled the command
bool FireDoCommand( REALcontrolInstance control, const char *command )
{
	RBBoolean (* fp)(REALcontrolInstance, REALstring);
	fp = (RBBoolean (*)(REALcontrolInstance, REALstring))GetEventHook( control, &sDesktopInputCanvasEvents[3], &sInputCanvasEvents[3] );
	if (fp) {
		REALstring commandStr = REALBuildString( command, strlen( command ), kREALTextEncodingUTF8 );
		bool result = fp( control, commandStr );
		REALUnlockString( commandStr );
		return result;
	}
	
	return false;
}

// FireFontNameAtLocation
// 
//  Triggers the user's FontNameAtLocation event. The implementor should return
//  the font used for rendering the specified character.
// 
// Gets: control - the control
//       location - the zero-based character index
// Returns: the font name
REALstring FireFontNameAtLocation( REALcontrolInstance control, int location )
{
	REALstring (* fp)(REALcontrolInstance, int);
	fp = (REALstring (*)(REALcontrolInstance, int))GetEventHook( control, &sDesktopInputCanvasEvents[4], &sInputCanvasEvents[4] );
	if (fp) {
		return fp( control, location );
	}
	
	return nullptr;
}

// FireFontSizeAtLocation
// 
//  Triggers the user's FontNameAtLocation event. The implementor should return
//  the font size used for rendering the specified character.
// 
// Gets: control - the control
//       location - the zero-based character index
// Returns: the font size
int FireFontSizeAtLocation( REALcontrolInstance control, int location )
{
	RBInteger (* fp)(REALcontrolInstance, RBInteger);
	fp = (RBInteger (*)(REALcontrolInstance, RBInteger))GetEventHook( control, &sDesktopInputCanvasEvents[5], &sInputCanvasEvents[5] );
	if (fp) {
		return fp( control, location );
	}
	
	return 0;
}

// FireIncompleteTextRange
// 
//  Triggers the user's FireIncompleteTextRange event. The implementor should return
//  the range of the current incomplete text, or nil if there is no incomplete text.
// 
// Gets: control - the control
// Returns: the range of incomplete text (as a TextRange)
REALobject FireIncompleteTextRange( REALcontrolInstance control )
{
	REALobject  (* fp)(REALcontrolInstance);
	fp = (REALobject  (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[6], &sInputCanvasEvents[6] );
	if (fp) {
		return fp( control );
	}
	
	return nullptr;
}

// FireInsertSetIncompleteText
// 
//  Triggers the user's SetIncompleteText event. This is fired when the system
//  has started (or is continuing) international text input and wishes to display
//  'incomplete text'. Incomplete text (marked text, in Cocoa terms) is a temporary
//  string displayed in the text during composition of the final input and is
//  not actually part of the content until it has been committed (via InsertText).
//  
// Gets: control - the control
//       text - the marked text (replaces any previous marked text)
//       replacementRange - the range of text to replace
//       relativeSelection - the new selection, relative to the start of the marked
//                           text
// Returns: nothing
void FireSetIncompleteText( REALcontrolInstance control, REALstring text,
	REALobject replacementRange, REALobject relativeSelection )
{
	void  (* fp)(REALcontrolInstance, REALstring, REALobject, REALobject);
	fp = (void  (*)(REALcontrolInstance, REALstring, REALobject, REALobject))GetEventHook( control, &sDesktopInputCanvasEvents[7], &sInputCanvasEvents[7] );
	if (fp) {
		fp( control, text, replacementRange, relativeSelection );
	}
}

// FireInsertText
// 
//  Triggers the user's InsertText event. The implementor should replace the content
//  in its document at `range` with the given text. If `range` is nil, the 
//  implementor should insert the text at its current selection.
// 
// Gets: control - the control
//       text - the text to insert
//       range - the content range to replace (may be NULL)
// Returns: nothing
void FireInsertText( REALcontrolInstance control, REALstring text, REALobject range )
{
	void  (* fp)(REALcontrolInstance, REALstring, REALobject);
	fp = (void  (*)(REALcontrolInstance, REALstring, REALobject))GetEventHook( control, &sDesktopInputCanvasEvents[8], &sInputCanvasEvents[8] );
	if (fp) {
		fp( control, text, range );
	}
}

// FireIsEditable
// 
//  Triggers the user's IsEditable event. The implementor should return whether
//  or not its content can be edited.
// 
// Gets: control - the control
// Returns: whether or not the content is editable
bool FireIsEditable( REALcontrolInstance control )
{
	RBBoolean (* fp)(REALcontrolInstance);
	fp = (RBBoolean (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[9], &sInputCanvasEvents[9] );
	if (fp) {
		return fp( control );
	}
	
	return false;
}

bool FireKeyFallsThrough( REALcontrolInstance control, REALstring key )
{
	RBBoolean (* fp)(REALcontrolInstance, REALstring);
	fp = (RBBoolean (*)(REALcontrolInstance, REALstring))GetEventHook( control, &sDesktopInputCanvasEvents[10], &sInputCanvasEvents[10] );
	if (fp) {
		return fp( control, key );
	}
	
	return false;
}

/**
 * Converts a Rect into a REALbasic.Rect object.
 */
static REALobject CreateRectObject( Rect rect )
{
	typedef void (* ConstructorTy)( REALobject, RBInteger, RBInteger, RBInteger, RBInteger );
	
	REALobject rectObject = REALnewInstance( sRectClass );
	ConstructorTy constructor = (ConstructorTy)REALLoadObjectMethod( rectObject, "Constructor( x as integer, y as integer, w as integer, h as integer)" );
	constructor( rectObject, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top );
	return rectObject;
}

/**
 * Converts a Rect into a Xojo.Rect object.
 */
static REALobject CreateXojoRectObject( Rect rect )
{
	typedef void (* ConstructorTy)( REALobject, RBInteger, RBInteger, RBInteger, RBInteger );
	
	REALobject rectObject = REALnewInstance( sXojoRectClass );
	ConstructorTy constructor = (ConstructorTy)REALLoadObjectMethod( rectObject, "Constructor( x as integer, y as integer, w as integer, h as integer)" );
	constructor( rectObject, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top );
	return rectObject;
}

// FirePaint
//  
//  Triggers the user's Paint event.
// 
// Gets: control - the control
//       context - the Graphcis object
// Returns: nothing
void FirePaint( REALcontrolInstance control, REALgraphics context, const Rect *rects, int rectCount )
{
	if (!REALinRuntime()) {
		Rect bounds;
		REALGetControlBounds(control, &bounds);

		REALSetPropValueColor(context, "ForeColor", 0x00FFFFFF);
		auto fillRect =
		    reinterpret_cast<void (*)(REALobject, RBInteger, RBInteger, RBInteger, RBInteger)>(
		        REALLoadObjectMethod(
		            context,
		            "FillRect(x as Integer, y as Integer, Width as Integer, Height as Integer)"));
		fillRect(context, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top);

		REALSetPropValueColor(context, "ForeColor", 0x00000000);
		auto drawRect =
		    reinterpret_cast<void (*)(REALobject, RBInteger, RBInteger, RBInteger, RBInteger)>(
		        REALLoadObjectMethod(
		            context,
		            "DrawRect(x as Integer, y as Integer, Width as Integer, Height as Integer)"));
		drawRect(context, 0, 0, bounds.right - bounds.left, bounds.bottom - bounds.top);
		return;
	}

	void  (* fp)(REALcontrolInstance, REALgraphics, REALarray);
	fp = (void  (*)(REALcontrolInstance, REALgraphics, REALarray))GetEventHook( control, &sDesktopInputCanvasEvents[11], &sInputCanvasEvents[11] );
	if (fp) {
		REALarray rectArray = REALCreateArray( kTypeObject, rectCount - 1 );
		for (int i = 0; i < rectCount; i++) {
			REALobject rectObject;
			if (IsDesktopUIControl(control)) {
				rectObject = CreateXojoRectObject( rects[i] );
			} else {
				rectObject = CreateRectObject( rects[i] );
			}
			REALSetArrayValue( rectArray, i, rectObject );
			REALUnlockObject( rectObject );
		}
		
		fp( control, context, rectArray );
		REALUnlockObject( (REALobject)rectArray );
	}
}

// FireRectForRange
// 
//  Triggers the user's RectForRange event. The implementor should return the
//  rectangle occupied by the given range, relative to the control. If needed,
//  the implementor can adjust the range to represent the text that was actually
//  represented in the range (to account for word wrapping or other client-side
//  features).
// 
// Gets: control - the control
//       range - the requested text range
// Returns: the rect the text takes in the control
REALobject FireRectForRange( REALcontrolInstance control, REALobject *range )
{
	REALobject  (* fp)(REALcontrolInstance, REALobject*);
	fp = (REALobject  (*)(REALcontrolInstance, REALobject *))GetEventHook( control, &sDesktopInputCanvasEvents[12], &sInputCanvasEvents[12] );
	if (fp) {
		return fp( control, range );
	}
	
	return nullptr;
}

// FireSelectedRange
// 
//  Triggers the user's SelectedRange event. The implementor should return a valid
//  range specifying which portion of the content is selected.
// 
// Gets: control - the control
// Returns: the range of the selection
REALobject FireSelectedRange( REALcontrolInstance control )
{
	REALobject  (* fp)(REALcontrolInstance);
	fp = (REALobject  (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[13], &sInputCanvasEvents[13] );
	if (fp) {
		return fp( control );
	}
	
	return nullptr;
}

// FireTextForRange
// 
//  Triggers the user's TextForRange event. The implementor should return the substring
//  of its content at the given range.
// 
// Gets: control - the control
//       range - the range of text to return
// Returns: the substring of the content
REALstring FireTextForRange( REALcontrolInstance control, REALobject range )
{
	REALstring  (* fp)(REALcontrolInstance, REALobject);
	fp = (REALstring  (*)(REALcontrolInstance, REALobject))GetEventHook( control, &sDesktopInputCanvasEvents[14], &sInputCanvasEvents[14] );
	if (fp) {
		return fp( control, range );
	}
	
	return nullptr;
}

// FireTextLength
// 
//  Triggers the user's TextLength event. The implementor should return the length
//  of its content (in characters).
//
// Gets: control - the control
// Returns: the content length
int FireTextLength( REALcontrolInstance control )
{
	RBInteger  (* fp)(REALcontrolInstance);
	fp = (RBInteger  (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[15], &sInputCanvasEvents[15] );
	if (fp) {
		return fp( control );
	}
	
	return 0;
}

// MARK: -

// Initializer
// 
//  The control needs to be created. Do so.
// 
// Gets: control - the TextInputCanvas
// Returns: nothing
static void Initializer( REALcontrolInstance control )
{
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
#if COCOA
	if (REALinRuntime()) {
		data->view = [[XOJTextInputView alloc] initWithControlInstance:control];
	}
#elif WINDOWS
	data->view = new TextInputWindows( control );
#elif WIDGET_GTK
	data->view = new TextInputGTK( control );
#endif
}

// CloseEvent
// 
//  The control is going away, clean up anything we need to.
// 
// Gets: control - the TextInputCanvas
// Returns: nothing
static void CloseEvent( REALcontrolInstance control )
{
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
#if COCOA
	[data->view cleanup];
	[data->view release];
	data->view = nil;
#elif WINDOWS || WIDGET_GTK
	delete data->view;
	data->view = nil;
#endif
}

// OpenEvent
// 
//  The control is has been setup (i.e. at this point the SubPane is created)
// 
// Gets: control - the TextInputCanvas
// Returns: nothing
static void OpenEvent( REALcontrolInstance control )
{
#if WINDOWS || WIDGET_GTK
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
	data->view->Setup();
#endif
}

#if !TARGET_MACOS
// InterpretKeyEvent
// 
//  Given a keyboard event, interprets them into higher level commands like "move
//  insertion point up" and sends them to the control.
// 
//  This is not used on Cocoa, where we just let NSView tell us what to do.
// 
// Gets: keycode - the input keycode
//       modifiers - the keyboard modifiers (alt, control, etc)
// Returns: whether or not the event was handled
static bool InterpretKeyEvent( REALcontrolInstance control, int keycode, int modifiers )
{
	switch (keycode) {
		case 1 /* home */: {
			if (modifiers & controlKey) {
				if (modifiers & shiftKey) {
					return FireDoCommand( control, "moveToBeginningOfDocumentAndModifySelection:" );
				}
				return FireDoCommand( control, "moveToBeginningOfDocument:" );
			} else {
				if (modifiers & shiftKey) {
					return FireDoCommand( control, "moveToLeftEndOfLineAndModifySelection:" );
				}
				return FireDoCommand( control, "moveToLeftEndOfLine:" );
			}
		} break;
			
		case 4 /* end */: {
			if (modifiers & controlKey) {
				if (modifiers & shiftKey) {
					return FireDoCommand( control, "moveToEndOfDocumentAndModifySelection:" );
				}
				return FireDoCommand( control, "moveToEndOfDocument:" );
			} else {
				if (modifiers & shiftKey) {
					return FireDoCommand( control, "moveToRightEndOfLineAndModifySelection:" );
				}
				return FireDoCommand( control, "moveToRightEndOfLine:" );
			}
		} break;
			
		case 11 /* page up */: {
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "pageUpAndModifySelection:" );
			}
			return FireDoCommand( control, "pageUp:" );
		} break;
			
		case 12 /* page down */: {
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "pageDownAndModifySelection:" );
			}
			return FireDoCommand( control, "pageDown:" );
		} break;
			
		case 8 /* backspace */: {
#if WINDOWS
			if (modifiers & optionKey) {
				return FireDoCommand( control, "rbUndo" );
			}
#endif
			
			return FireDoCommand( control, "deleteBackward:" );
		} break;
			
		case 127 /* forward delete */: {
#if WINDOWS
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "rbCut" );
			}
#endif	
			
			return FireDoCommand( control, "deleteForward:" );
		} break;
			
		case 5 /* insert */: {
#if WINDOWS
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "rbPaste" );
			} else if (modifiers & controlKey) {
				return FireDoCommand( control, "rbCopy" );
			} else {
				return FireDoCommand( control, "rbToggleOverwriteMode" );
			}
#endif
		} break;

		case 13 /* return */:
		case 3  /* enter */:
		{
			// On Windows Ctrl+C also triggers this keycode (3), so make sure the controlKey isn't down
			if (!(modifiers & controlKey)) {
				return FireDoCommand( control, "insertNewline:" );
			}
		} break;

		case 28 /* left arrow */: {
			if ((modifiers & shiftKey) && (modifiers & controlKey)) {
				return FireDoCommand( control, "moveWordLeftAndModifySelection:" );
			} else if (modifiers & shiftKey) {
				return FireDoCommand( control, "moveLeftAndModifySelection:" );
			} else if (modifiers & controlKey) {
				return FireDoCommand( control, "moveWordLeft:" );
			} else {
				return FireDoCommand( control, "moveLeft:" );
			}
		} break;
			
		case 29 /* right arrow */: {
			if ((modifiers & shiftKey) && (modifiers & controlKey)) {
				return FireDoCommand( control, "moveWordRightAndModifySelection:" );
			} else if (modifiers & shiftKey) {
				return FireDoCommand( control, "moveRightAndModifySelection:" );
			} else if (modifiers & controlKey) {
				return FireDoCommand( control, "moveWordRight:" );
			} else {
				return FireDoCommand( control, "moveRight:" );
			}
		} break;
			
		case 30 /* up arrow */: {
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "moveUpAndModifySelection:" );
			} else if (modifiers & controlKey) {
				return FireDoCommand( control, "scrollLineUp:" );
			} else {
				return FireDoCommand( control, "moveUp:" );
			}
		} break;
			
		case 31 /* down arrow */: {
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "moveDownAndModifySelection:" );
			} else if (modifiers & controlKey) {
				return FireDoCommand( control, "scrollLineDown:" );
			} else {
				return FireDoCommand( control, "moveDown:" );
			}
		} break;
			
		case 9 /* tab */: {
			if (modifiers & shiftKey) {
				return FireDoCommand( control, "insertBacktab:" );
			} else {
				return FireDoCommand( control, "insertTab:" );
			}
		} break;
	}

	// Keys are translated in a separate event, returning true here just prevents
	// our framework from dispatching this further (i.e. to ContainerControls, and Window)
	return true;
}
#endif  // !TARGET_MACOS

#if WIDGET_GTK
// IsTextKey
// 
//  Return whether the given key is a bit of text input, as opposed to a menu
//  shortcut or some such. This generally involves checking the current keyboard
//  modifiers, but the details vary from platform to platform.
// 
//  This is not used on Cocoa, where we just let NSView tell us what to do.
// 
// Gets: keycode - the keycode to check
//       modifiers - the keyboard modifiers (alt, control, etc)
// Returns: is this a text key
static bool IsTextKey( int keycode, int modifiers )
{
	// Rule 1: if it's some control character (ASCII 0-31), then it's not text.
	// IMPORTANT: don't exclude other non-ASCII characters here!  Some people find
	// those characters pretty important.
	if (keycode < 32) return false;

	// Windows/Linux rule: if the control key is down without the alt key, or
	// vice versa, then it's a shortcut and not keyboard input. (Note: we must be
	// sure to allow AltGr combinations, i.e. Ctrl+Alt+SomeKey.)
	if ((modifiers & controlKey) && !(modifiers & optionKey)) return false;
	if ((modifiers & optionKey) && !(modifiers & controlKey)) return false;

	return true;
}
#endif  // WIDGET_GTK

static RBBoolean KeyDownEvent( REALcontrolInstance control, int charCode, int keyCode, int modifiers )
{
	#if !TARGET_MACOS
		return InterpretKeyEvent( control, charCode, modifiers );
	#else
		return false;
	#endif
}

#if WIDGET_GTK
static RBBoolean UnfilteredKeyDownEvent( REALcontrolInstance control, int keyCode, int modifiers )
{
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
	data->view->KeyDown();
	return false;
}
#endif

// InvalidateTextRects
// 
//  Invalidates any cached text rectangles. The control should call this whenever
//  content moves around on screen.
// 
// Gets: control - the control
// Returns: nothing
static void InvalidateTextRects( REALcontrolInstance control )
{
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
#if COCOA
	[[data->view inputContext] invalidateCharacterCoordinates];
#endif
}

static void LinuxSelectInputMethod( REALcontrolInstance control )
{
#if WIDGET_GTK
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
	data->view->SelectInputMethod();
#endif
}

static RBBoolean MouseDown( REALcontrolInstance inst, int x, int y, int modifiers )
{
	RBBoolean (*fp)(REALcontrolInstance, RBInteger, RBInteger) = (RBBoolean (*)(REALcontrolInstance, RBInteger, RBInteger))GetEventHook( inst, &sDesktopInputCanvasEvents[16], &sInputCanvasEvents[16] );
	if (fp) {
		Rect rBounds;
		REALGetControlBounds( inst, &rBounds );

		return fp( inst, x - rBounds.left, y - rBounds.top );
	}
	
	return false;
}

static void MouseDrag( REALcontrolInstance inst, int x, int y )
{
	void (*fp)(REALcontrolInstance, RBInteger, RBInteger) = (void (*)(REALcontrolInstance, RBInteger, RBInteger))GetEventHook( inst, &sDesktopInputCanvasEvents[17], &sInputCanvasEvents[17] );
	if (fp) {
		Rect rBounds;
		REALGetControlBounds( inst, &rBounds );
		
		fp( inst, x - rBounds.left, y - rBounds.top );
	}
}

static void MouseUp( REALcontrolInstance inst, int x, int y )
{
	void (*fp)(REALcontrolInstance, RBInteger, RBInteger) = (void (*)(REALcontrolInstance, RBInteger, RBInteger))GetEventHook( inst, &sDesktopInputCanvasEvents[18], &sInputCanvasEvents[18] );
	if (fp) {
		Rect rBounds;
		REALGetControlBounds(inst, &rBounds);
		
		fp( inst, x - rBounds.left, y - rBounds.top);
	}
}


static void GotFocus( REALcontrolInstance control )
{
#if WIDGET_GTK
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
	data->view->GotFocus();
#endif

	void (*fp)(REALcontrolInstance) = (void (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[19], &sInputCanvasEvents[19] );
	if (fp) {
		fp( control );
	}
}

static void LostFocus( REALcontrolInstance control )
{
#if WIDGET_GTK
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
	data->view->LostFocus();
#endif

	void (*fp)(REALcontrolInstance) = (void (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[20], &sInputCanvasEvents[20] );
	if (fp) {
		fp( control );
	}
}
static void EnableMenuItems( REALcontrolInstance control )
{
	void (*fp)(REALcontrolInstance) = (void (*)(REALcontrolInstance))GetEventHook( control, &sDesktopInputCanvasEvents[21], &sInputCanvasEvents[21] );
	if (fp) {
		fp( control );
	}
}

static void *HandleGetter( REALcontrolInstance control )
{
	InputCanvasData	*data = (InputCanvasData *)GetControlData( control, &sDesktopInputCanvasControl, &sInputCanvasControl );
#if COCOA
	return data->view;
#else
	return 0;
#endif
}

static void ScaleFactorChanged( REALcontrolInstance control, double newScaleFactor )
{
	if (IsDesktopUIControl(control)) {
		void (*fp)(REALcontrolInstance) = (void (*)(REALcontrolInstance))REALGetEventInstance( control, &sDesktopInputCanvasEvents[22] );
		if (fp) {
			fp( control );
		}
	}
}
