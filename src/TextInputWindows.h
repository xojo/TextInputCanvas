//
//  TextInputWindows.h
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#ifndef IDEUtils_TextInputWindows_h
#define IDEUtils_TextInputWindows_h

#include "rb_plugin.h"

class TextInputWindows
{
  private:
	REALcontrolInstance mControl;
	HWND mWindow = nullptr;

  public:
	LONG_PTR OldWndProc;

	TextInputWindows( REALcontrolInstance control );
	~TextInputWindows();

	void InsertBreakCommand();
	void InsertNewLine();
	void InsertUnicodeChar( long unicodeChar );
	void StartComposition( HWND handle );

	void Setup();
	void TearDown();

	static LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
};

#endif
