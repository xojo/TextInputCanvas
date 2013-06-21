//
//  TextInputCanvas.h
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#ifndef IDEUtils_TextInputCanvas_h
#define IDEUtils_TextInputCanvas_h

#include "rb_plugin.h"

int FireBaselineAtIndex( REALcontrolInstance control, int index );
int FireCharacterAtPoint( REALcontrolInstance control, int x, int y );
void FireDiscardIncompleteText( REALcontrolInstance control );
bool FireDoCommand( REALcontrolInstance control, const char *command );
REALstring FireFontNameAtLocation( REALcontrolInstance control, int location );
int FireFontSizeAtLocation( REALcontrolInstance control, int location );
REALobject FireIncompleteTextRange( REALcontrolInstance control );
void FireSetIncompleteText( REALcontrolInstance control, REALstring text,
	REALobject replacementRange, REALobject relativeSelection );
void FireInsertText( REALcontrolInstance control, REALstring text,
	REALobject range );
bool FireIsEditable( REALcontrolInstance control );
bool FireIsOverwriteMode( REALcontrolInstance control );
bool FireKeyFallsThrough( REALcontrolInstance control, REALstring key );
void FirePaint( REALcontrolInstance control, REALgraphics context, const Rect *rects, int rectCount );
REALobject FireRectForRange( REALcontrolInstance control, REALobject *range );
REALobject FireSelectedRange( REALcontrolInstance control );
REALstring FireTextForRange( REALcontrolInstance control, REALobject range );
int FireTextLength( REALcontrolInstance control );

void RegisterTextInputCanvasControl();

#endif
