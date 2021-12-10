//
//  TextRange.h
//
//  (c) 2021 Xojo, Inc. -- All Rights Reserved
//

#ifndef IDEUtils_TextInputRange_h
#define IDEUtils_TextInputRange_h

#include "rb_plugin.h"

REALobject CreateTextRange( int begin, int length );
void GetTextRangeInfo( REALobject range, int *begin, int *length );
void RegisterTextRangeClass();

#endif
