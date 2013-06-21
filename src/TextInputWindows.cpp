//
//  TextInputWindows.cpp
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#include "TextInputWindows.h"

#include "TextInputCanvas.h"
#include "TextRange.h"
#include "UnicodeHelper.h"


TextInputWindows::TextInputWindows( REALcontrolInstance control )
{
	mControl = control;
	OldWndProc = NULL;
}

TextInputWindows::~TextInputWindows()
{
	TearDown();
}


void TextInputWindows::Setup()
{
	long handle = 0;
	if (REALGetPropValue( (REALobject)mControl, "Handle", &handle ) && handle != 0) {
		OldWndProc = ::GetWindowLongW( (HWND)handle, GWL_WNDPROC );
		::SetWindowLongW( (HWND)handle, GWL_WNDPROC, (LONG)WindowProc );
		::SetPropW( (HWND)handle, L"TextInputWindow", this );
	}
}

void TextInputWindows::TearDown()
{
	// Make sure we haven't already destroyed the window handle
	long handle = 0;
	if (REALGetPropValue( (REALobject)mControl, "Handle", &handle ) && handle != 0) {
		::RemovePropW( (HWND)handle, L"TextInputWindow" );
		::SetWindowLongW( (HWND)handle, GWL_WNDPROC, OldWndProc );
	}
}

void TextInputWindows::InsertNewLine()
{
	FireDoCommand( mControl, "insertNewline:" );
}

void TextInputWindows::InsertUnicodeChar( long unicodeChar )
{
	REALstring key = UnicodeCharToString( unicodeChar );
	FireInsertText( mControl, key, NULL );
	REALUnlockString( key );
}

void TextInputWindows::StartComposition( HWND handle )
{
	HIMC icHandle = ::ImmGetContext( handle );
	if (0 == icHandle || !::ImmGetOpenStatus( icHandle )) return;

	REALobject selRange = FireSelectedRange( mControl );
	if (selRange) {
		REALobject rect = FireRectForRange( mControl, &selRange );
		if (rect) {
			// Move the IME window so that's at our cursor position
			COMPOSITIONFORM form = {0};
			form.dwStyle = CFS_FORCE_POSITION;
			long left = 0;
			long top = 0;
			REALGetPropValue( rect, "Left", &left );
			REALGetPropValue( rect, "Top", &top );
			form.ptCurrentPos.x = left;
			form.ptCurrentPos.y = top;
			::ImmSetCompositionWindow( icHandle, &form );
			REALUnlockObject( rect );

		    // Make sure that the IME font matches the canvas's
			int begin, length;
			GetTextRangeInfo( selRange, &begin, &length );
			REALstring fontName = FireFontNameAtLocation( mControl, begin + length );
			size_t numBytes = 0;
			void *fontNameContents = REALGetStringContents( fontName, &numBytes );
			int fontSize = FireFontSizeAtLocation( mControl, begin + length );

			LOGFONTA logFont = {0};
			logFont.lfHeight = fontSize;
			memcpy( logFont.lfFaceName, fontNameContents, numBytes );
			::ImmSetCompositionFontA( icHandle, &logFont );

			REALUnlockString( fontName );
		}

		REALUnlockObject( selRange );
	}
}

LRESULT CALLBACK TextInputWindows::WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	TextInputWindows *ti = reinterpret_cast<TextInputWindows *>( ::GetPropW( hWnd, L"TextInputWindow" ) );
	if (!ti) return 0;

	switch (message) {
		case WM_CHAR:
			if (wParam == VK_RETURN) ti->InsertNewLine();
			else if (wParam > 31) ti->InsertUnicodeChar( wParam );
			break;
		case WM_IME_STARTCOMPOSITION:
			ti->StartComposition( hWnd );
			break;
	}

	return CallWindowProcW( (WNDPROC)ti->OldWndProc, hWnd, message, wParam, lParam );
}
