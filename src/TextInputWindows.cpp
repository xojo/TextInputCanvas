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
	RBInteger handle = 0;
	if (REALGetPropValueInteger( (REALobject)mControl, "Handle", &handle ) && handle != 0) {
		mWindow = reinterpret_cast<HWND>(handle);
		OldWndProc = ::GetWindowLongPtrW(mWindow, GWLP_WNDPROC);
		::SetWindowLongPtrW(mWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProc));
		::SetPropW(mWindow, L"TextInputWindow", this );
	}
}

void TextInputWindows::TearDown()
{
	// Make sure we haven't already destroyed the window handle
	if (mWindow) {
		::RemovePropW(mWindow, L"TextInputWindow");
		::SetWindowLongPtrW(mWindow, GWLP_WNDPROC, OldWndProc);
		mWindow = nullptr;
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
			RBInteger left = 0;
			RBInteger top = 0;
			REALGetPropValueInteger( rect, "Left", &left );
			REALGetPropValueInteger( rect, "Top", &top );
			form.ptCurrentPos.x = left;
			form.ptCurrentPos.y = top;
			::ImmSetCompositionWindow( icHandle, &form );
			REALUnlockObject( rect );

		    // Make sure that the IME font matches the canvas's
			int begin, length;
			GetTextRangeInfo( selRange, &begin, &length );
			REALstring fontName = FireFontNameAtLocation( mControl, begin + length );
			REALstringData strData;
			if (REALGetStringData(fontName, kREALTextEncodingUTF16, &strData)) {
				int fontSize = FireFontSizeAtLocation(mControl, begin + length);

				LOGFONTW logFont = { 0 };
				logFont.lfHeight = fontSize;

				size_t copyBytes = min(sizeof(logFont.lfFaceName) - sizeof(WCHAR), strData.length);
				memcpy(logFont.lfFaceName, strData.data, copyBytes);
				::ImmSetCompositionFontW(icHandle, &logFont);

				REALDisposeStringData(&strData);
			}

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
		case WM_IME_CHAR:
			if (wParam == VK_RETURN) {
				ti->InsertNewLine();
				return 0;
			} else if (wParam > 31) {
				ti->InsertUnicodeChar( wParam );
				return 0;
			}
			break;
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
