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

bool TextInputGTK::KeyDown()
{
	bool retVal = false;

	if (mContext) {
		GdkEvent *event = gtk_get_current_event();
		if (event) {
			if (GDK_KEY_PRESS == event->type) {
				retVal = gtk_im_context_filter_keypress( mContext, (GdkEventKey *)event );
			}
			gdk_event_free( event );
		}
	}

	return retVal;
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
			double left = 0;
			double top = 0;
			double width = 0;
			double height = 0;
			REALGetPropValueDouble( rect, "Left", &left );
			REALGetPropValueDouble( rect, "Top", &top );
			REALGetPropValueDouble( rect, "Width", &width );
			REALGetPropValueDouble( rect, "Height", &height );
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
		g_signal_connect_data( mContext, "commit", (GCallback)CommitCallback, mControl, NULL, GConnectFlags(0) );
		g_signal_connect_data( mContext, "preedit_changed", (GCallback)PreEditChangedCallback, mControl, NULL, GConnectFlags(0) );
		g_signal_connect_data( mContext, "preedit_start", (GCallback)PreEditStartCallback, mControl, NULL, GConnectFlags(0) );

		RBInteger handle = 0;
		if (REALGetPropValueInteger( (REALobject)mControl, "Handle", &handle ) && handle != 0) {
			GtkWidget *widget = (GtkWidget *)handle;
			gtk_im_context_set_client_window( mContext, gtk_widget_get_window(widget) );
		}
	}
}

void TextInputGTK::TearDown()
{
	if (mContext) g_object_unref( mContext );
	mContext = NULL;
}
