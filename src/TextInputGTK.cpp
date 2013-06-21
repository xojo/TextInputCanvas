//
//  TextInputGTK.cpp
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#include <string.h>
#include "TextInputCanvas.h"
#include "TextInputGTK.h"
#include "TextRange.h"


TextInputGTK::TextInputGTK( REALcontrolInstance control )
{
	mControl = control;
	mContext = NULL;
}

TextInputGTK::~TextInputGTK()
{
	TearDown();
}


void TextInputGTK::GotFocus()
{
	if (mContext) gtk_im_context_focus_in( mContext );
}

void TextInputGTK::LostFocus()
{
	if (mContext) gtk_im_context_focus_out( mContext );
}

void TextInputGTK::CommitCallback( GtkIMContext *context, gchar *str, gpointer userData )
{
	REALstring key = REALBuildString( str, strlen(str), kREALTextEncodingUTF8 );
	FireInsertText( reinterpret_cast<REALcontrolInstance>(userData), key, NULL );
	REALUnlockString( key );
}

bool TextInputGTK::FilterKeyPress()
{
	if (mContext) {
		GdkEvent *keyEvent = gtk_get_current_event();
		bool result = gtk_im_context_filter_keypress( mContext, reinterpret_cast<GdkEventKey *>(keyEvent) );
		gdk_event_free( keyEvent );
		return result;
	}

	return false;
}

void TextInputGTK::PreEditChangedCallback( GtkIMContext *context, gpointer userData )
{
	gchar *str = NULL;
	gtk_im_context_get_preedit_string( context, &str, NULL, NULL );

	if (str) {
		REALstring key = REALBuildString( str, strlen(str), kREALTextEncodingUTF8 );
		FireSetIncompleteText( reinterpret_cast<REALcontrolInstance>(userData), key, NULL, NULL );
		REALUnlockString( key );
		g_free( str );
	}
}

void TextInputGTK::PreEditStartCallback( GtkIMContext *context, gpointer userData )
{
	REALcontrolInstance control = reinterpret_cast<REALcontrolInstance>(userData);

	REALobject selRange = FireSelectedRange( control );
	if (selRange) {
		REALobject rect = FireRectForRange( control, &selRange );
		if (rect) {
			long left = 0;
			long top = 0;
			long width = 0;
			long height = 0;
			REALGetPropValue( rect, "Left", &left );
			REALGetPropValue( rect, "Top", &top );
			REALGetPropValue( rect, "Width", &width );
			REALGetPropValue( rect, "Height", &height );
			REALUnlockObject( rect );

			GdkRectangle area = {0};
			area.x = left;
			area.y = top;
			area.width = width;
			area.height = height;
			gtk_im_context_set_cursor_location( context, &area );
		}
		REALUnlockObject( selRange );
	}
}

void TextInputGTK::SelectInputMethod()
{
	GtkWidget *menu = gtk_menu_new();
	gtk_im_multicontext_append_menuitems( reinterpret_cast<GtkIMMulticontext *>(mContext), GTK_MENU_SHELL(menu) );
	gtk_menu_popup( GTK_MENU(menu), NULL, NULL, NULL, NULL, 0, 0 );
	gtk_im_context_reset( mContext );
}

void TextInputGTK::Setup()
{
	mContext = gtk_im_multicontext_new();
	if (mContext) {
		long handle = 0;
		if (REALGetPropValue( (REALobject)mControl, "Handle", &handle ) && handle != 0) {
			g_signal_connect_data( mContext, "commit", (GCallback)CommitCallback, mControl, NULL, GConnectFlags(0) );
			g_signal_connect_data( mContext, "preedit_changed", (GCallback)PreEditChangedCallback, mControl, NULL, GConnectFlags(0) );
			g_signal_connect_data( mContext, "preedit_start", (GCallback)PreEditStartCallback, mControl, NULL, GConnectFlags(0) );

			GtkWidget *widget = reinterpret_cast<GtkWidget *>(handle);
			gtk_im_context_set_client_window( mContext, widget->window );
		}
	}
}

void TextInputGTK::TearDown()
{
	if (mContext) g_object_unref( mContext );
	mContext = NULL;
}
