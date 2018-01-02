//
//  TextInputGTK.h
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#ifndef IDEUtils_TextInputGTK_h
#define IDEUtils_TextInputGTK_h

#include <gtk/gtk.h>
#include "rb_plugin.h"

class TextInputGTK
{
  private:
	REALcontrolInstance mControl;
	GtkIMContext *mContext;

  public:
	TextInputGTK( REALcontrolInstance control );
	~TextInputGTK();

	void Setup();
	void TearDown();

	bool KeyDown();
	void GotFocus();
	void LostFocus();
	void SelectInputMethod();

	static void CommitCallback( GtkIMContext *context, gchar *str, gpointer userData );
	static gboolean KeyPressCallBack( GtkWidget *widget, GdkEventKey *event, TextInputGTK *input );
	static void PreEditChangedCallback( GtkIMContext *context, gpointer userData );
	static void PreEditStartCallback( GtkIMContext *context, gpointer userData );
};

#endif
