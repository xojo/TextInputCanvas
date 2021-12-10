// PluginMainCocoa.mm
//
// This file is part of the REALbasic plugin API.
//
// (c) 2013 Xojo, Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#include "rb_plugin_cocoa.h"

// Defined in PluginMain.cpp
extern void *(*gResolver)(const char *entryName);

const REALCocoaViewCallbacks * REALGetCocoaViewCallbacks()
{
	static const REALCocoaViewCallbacks * (*pGetCocoaViewCallbacks)(void);
	if (!pGetCocoaViewCallbacks) {
		pGetCocoaViewCallbacks = (const REALCocoaViewCallbacks * (*)(void)) gResolver("PluginGetCocoaViewCallbacks");
	}
	if (pGetCocoaViewCallbacks) return pGetCocoaViewCallbacks();
	else return NULL;
}
