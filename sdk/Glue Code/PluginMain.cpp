// PluginMain.cpp
//
// This file is part of the REALbasic plugin API.
//
// (c) 2013 Xojo, Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

// If we're using visual studio, we need
// to include winheader++.h
#ifdef _MSC_VER
	#include "WinHeader++.h"
#endif

#include <stdlib.h>
#include <string.h>

#include "REALplugin.h"
#include "rb_plugin.h"

#if !FLAT_C_PLUGIN_HEADERS
	#define REALnewInstanceWithClass REALnewInstance
	#define REALGetPropValueInt32 REALGetPropValue
	#define REALGetPropValueString REALGetPropValue
	#define REALGetPropValueDouble REALGetPropValue
	#define REALGetPropValueObject REALGetPropValue
	#define REALGetPropValueUInt8 REALGetPropValue
	#define REALGetPropValueInt64 REALGetPropValue
	#define REALGetPropValueUInt64 REALGetPropValue
	#define REALGetPropValueUInt32 REALGetPropValue
	#define REALGetPropValueUInt16 REALGetPropValue
	#define REALGetPropValueInt16 REALGetPropValue
	#define REALGetPropValueInt8 REALGetPropValue
	#define REALGetPropValueSingle REALGetPropValue
	#define REALGetPropValuePtr REALGetPropValue
	#define REALGetPropValueCString REALGetPropValue
	#define REALGetPropValueWString REALGetPropValue
	#define REALGetPropValuePString REALGetPropValue
#if TARGET_CARBON || TARGET_COCOA
	#define REALGetPropValueCFStringRef REALGetPropValue
#endif
	#define REALSetPropValueInt32 REALSetPropValue
	#define REALSetPropValueString REALSetPropValue
	#define REALSetPropValueDouble REALSetPropValue
	#define REALSetPropValueObject REALSetPropValue
	#define REALSetPropValueUInt8 REALSetPropValue
	#define REALSetPropValueUInt32 REALSetPropValue
	#define REALSetPropValueUInt64 REALSetPropValue
	#define REALSetPropValueInt64 REALSetPropValue
	#define REALSetPropValueInt16 REALSetPropValue
	#define REALSetPropValueUInt16 REALSetPropValue
	#define REALSetPropValueInt8 REALSetPropValue
	#define REALSetPropValueSingle REALSetPropValue
	#define REALSetPropValueCString REALSetPropValue
	#define REALSetPropValueWString REALSetPropValue
	#define REALSetPropValuePString REALSetPropValue
#if TARGET_CARBON || TARGET_COCOA
	#define REALSetPropValueCFStringRef REALSetPropValue
#endif

	#define REALInsertArrayValueInt64 REALInsertArrayValue
	#define REALInsertArrayValueInt32 REALInsertArrayValue
	#define REALInsertArrayValueInt16 REALInsertArrayValue
	#define REALInsertArrayValueInt8 REALInsertArrayValue
	#define REALInsertArrayValueUInt64 REALInsertArrayValue
	#define REALInsertArrayValueUInt32 REALInsertArrayValue
	#define REALInsertArrayValueUInt16 REALInsertArrayValue
	#define REALInsertArrayValueUInt8 REALInsertArrayValue
	#define REALInsertArrayValueString REALInsertArrayValue
	#define REALInsertArrayValueObject REALInsertArrayValue
	#define REALInsertArrayValueSingle REALInsertArrayValue
	#define REALInsertArrayValueDouble REALInsertArrayValue
	#define REALInsertArrayValueBoolean REALInsertArrayValue

	#define REALGetArrayValueInt64 REALGetArrayValue
	#define REALGetArrayValueInt32 REALGetArrayValue
	#define REALGetArrayValueInt16 REALGetArrayValue
	#define REALGetArrayValueInt8 REALGetArrayValue
	#define REALGetArrayValueUInt64 REALGetArrayValue
	#define REALGetArrayValueUInt32 REALGetArrayValue
	#define REALGetArrayValueUInt16 REALGetArrayValue
	#define REALGetArrayValueUInt8 REALGetArrayValue
	#define REALGetArrayValueString REALGetArrayValue
	#define REALGetArrayValueObject REALGetArrayValue
	#define REALGetArrayValueSingle REALGetArrayValue
	#define REALGetArrayValueDouble REALGetArrayValue
	#define REALGetArrayValueBoolean REALGetArrayValue

	#define REALSetArrayValueInt64 REALSetArrayValue
	#define REALSetArrayValueInt32 REALSetArrayValue
	#define REALSetArrayValueInt16 REALSetArrayValue
	#define REALSetArrayValueInt8 REALSetArrayValue
	#define REALSetArrayValueUInt64 REALSetArrayValue
	#define REALSetArrayValueUInt32 REALSetArrayValue
	#define REALSetArrayValueUInt16 REALSetArrayValue
	#define REALSetArrayValueUInt8 REALSetArrayValue
	#define REALSetArrayValueString REALSetArrayValue
	#define REALSetArrayValueObject REALSetArrayValue
	#define REALSetArrayValueSingle REALSetArrayValue
	#define REALSetArrayValueDouble REALSetArrayValue
	#define REALSetArrayValueBoolean REALSetArrayValue

	#define REALLockPictureDescriptionWithNativeType	REALLockPictureDescription
	#define REALBuildStringWithEncoding	REALBuildString
#endif

#ifdef WIN32
extern "C" void __declspec(dllexport) REALPluginMain(void *(*resolver)(const char *entryName));
#else
extern "C" void REALPluginMain(void *(*resolver)(const char *entryName));
#endif

void *(*gResolver)(const char *entryName);

inline void* CallResolver(const char *entryName)
{
	return gResolver(entryName);
}

Boolean REALGetStringData( REALstring str, unsigned long encoding, REALstringData *outData )
{
	static Boolean (*pGetStringData)(REALstring, unsigned long, REALstringData*);
	if (!pGetStringData)
		pGetStringData = (Boolean (*)(REALstring, unsigned long, REALstringData*)) CallResolver("PluginGetStringData");
	if (pGetStringData) {
		return pGetStringData(str, encoding, outData);
	} else {
		static unsigned long (*pCString)(REALstring) = nil;
		static RBInteger (*pLenB)(REALstring);
		if (!pCString)
			pCString = (unsigned long (*)(REALstring)) CallResolver("StringGetCString");
		if (!pLenB)
			pLenB = (RBInteger (*)(REALstring))REALLoadFrameworkMethod( "LenB( s as string ) as integer" );
		if (!pCString || !pLenB) return false;
		
		// Empty strings return false.
		if (!str) return false;
		
		// We don't have an implementation of this in the framework, but let's
		// emulate it so that we can run on 2011r4. The 2012r1+ implementation is
		// more optimized than this, don't worry.
		unsigned long length;
		char *data;
		if (encoding == kREALTextEncodingUnknown) {
			length = pLenB( str );
			data = (char *)malloc( outData->length + 4 );
			memcpy( data, (void *)pCString( str ), length );
			memset( data + length, 0, 4 );
		} else if (encoding == REALGetStringEncoding( str )) {
			length = pLenB( str );
			data = (char *)malloc( length + 4 );
			memcpy( data, (void *)pCString( str ), length );
			memset( data + length, 0, 4 );
		} else {
			REALstring converted = REALConvertString( str, encoding );
			if (!converted) return false;
			
			length = pLenB( converted );
			data = (char *)malloc( length + 4 );
			memcpy( data, (void *)pCString( converted ), length );
			memset( data + length, 0, 4 );
			REALUnlockString( converted );
		}
		
		outData->encoding = encoding;
		outData->length = length;
		outData->data = data;
		return true;
	}
}

void REALDisposeStringData( REALstringData *data )
{
	static void (*pDisposeStringData)(REALstringData*);
	if (!pDisposeStringData)
		pDisposeStringData = (void (*)(REALstringData*)) CallResolver("PluginDisposeStringData");
	if (pDisposeStringData) {
		pDisposeStringData(data);
	} else {
		// Again, emulate this so we can run on 2011r4.
		free( (void *)data->data );
	}
}

#if TARGET_CARBON || TARGET_COCOA
CFStringRef REALCopyStringCFString( REALstring str )
{
	static CFStringRef (*pGetStringCFString)(REALstring, Boolean) = nil;
	if (!pGetStringCFString)
		pGetStringCFString = (CFStringRef (*)(REALstring, Boolean)) CallResolver("REALGetStringCFString");
	if (pGetStringCFString) return pGetStringCFString(str, false);
	else return (CFStringRef)0;
}
#endif

void * REALGetPluginData( REALobject obj, REALclassRef classRef )
{
	static void * (*pGetPluginData)(REALobject, REALclassRef);
	if (!pGetPluginData)
		pGetPluginData = (void * (*)(REALobject, REALclassRef)) CallResolver("PluginGetPluginData");
	if (pGetPluginData) return pGetPluginData(obj, classRef);
	else return NULL;
}

Boolean REALObjectIsA( REALobject obj, REALclassRef classRef )
{
	static Boolean (*pObjectIsA)(REALobject, REALclassRef);
	if (!pObjectIsA)
		pObjectIsA = (Boolean (*)(REALobject, REALclassRef)) CallResolver("RuntimeObjectIsa");
	if (pObjectIsA) return pObjectIsA(obj, classRef);
	else return (Boolean)0;
}

REALobject REALCallFunctionWithExceptionHandler( void (*callback)(void*), void *userData )
{
	static REALobject (*pCallFunctionWithExceptionHandler)(void (*)(void*), void*);
	if (!pCallFunctionWithExceptionHandler)
		pCallFunctionWithExceptionHandler = (REALobject (*)(void (*)(void*), void*)) CallResolver("PluginCallFunctionWithExceptionHandler");
	if (pCallFunctionWithExceptionHandler) {
		return pCallFunctionWithExceptionHandler(callback, userData);
	}
	
	return NULL;
}

RBInteger REALCompareStrings( REALstring str1, REALstring str2 )
{
	static RBInteger (*pCompareStrings)(REALstring, REALstring);
	if (!pCompareStrings)
		pCompareStrings = (RBInteger (*)(REALstring, REALstring)) CallResolver("RuntimeStringCompare");
	if (pCompareStrings) return pCompareStrings(str1, str2);
	else return 0;
}

REALstring REALAddStrings( REALstring str1, REALstring str2 )
{
	static REALstring (*pAddStrings)(REALstring, REALstring);
	if (!pAddStrings)
		pAddStrings = (REALstring (*)(REALstring, REALstring)) CallResolver("RuntimeAddString");
	if (pAddStrings) return pAddStrings(str1, str2);
	else return NULL;
}

void * REALGetDelegateInvoker( REALobject delegate )
{
	static void * (*pGetDelegateInvoker)(REALobject);
	if (!pGetDelegateInvoker)
		pGetDelegateInvoker = (void * (*)(REALobject)) CallResolver("PluginGetDelegateInvoker");
	if (pGetDelegateInvoker) return pGetDelegateInvoker(delegate);
	else return NULL;
}

void SetClassConsoleSafe( REALclassDefinition *def )	// Nov 04 2004 -- AJB (1)
{
	def->mFlags |= REALconsoleSafe;
}

void SetClassWebOnly( REALclassDefinition *def )	// Nov 04 2004 -- AJB (1)
{
	def->mFlags = (def->mFlags | REALwebOnly) & ~REALconsoleOnly;
}

Boolean REALinRuntime(void)
{
	static Boolean (*pInRuntime)(void) = nil;
	if (!pInRuntime)
		pInRuntime = (Boolean (*)(void)) CallResolver("REALinRuntime");
	if (pInRuntime) return pInRuntime();
	else return (Boolean)0;
}

long REALRegisterBackgroundTask( BackgroundTaskProc proc, unsigned long period, void *data )
{
	static long (*pRuntimeRegisterBackgroundTask)( BackgroundTaskProc, unsigned long, void * );
	if (!pRuntimeRegisterBackgroundTask)
		pRuntimeRegisterBackgroundTask = (long (*)( BackgroundTaskProc, unsigned long, void * )) CallResolver("PluginRegisterBackgroundTask");
	if (pRuntimeRegisterBackgroundTask) return pRuntimeRegisterBackgroundTask( proc, period, data );
	return -1;
}

void REALUnregisterBackgroundTask( long id )
{
	static void (*pRuntimeUnregisterBackgroundTask)( long id );
	if (!pRuntimeUnregisterBackgroundTask)
		pRuntimeUnregisterBackgroundTask = (void (*)(long)) CallResolver("PluginUnregisterBackgroundTask");
	if (pRuntimeUnregisterBackgroundTask) pRuntimeUnregisterBackgroundTask( id );
}

void REALRegisterControl(REALcontrol *defn)
{
	static void (*pRuntimeRegisterControl)(REALcontrol *defn);
	if (!pRuntimeRegisterControl)
		pRuntimeRegisterControl = (void (*)(REALcontrol *)) CallResolver("PluginRegisterControl");
	
	if (pRuntimeRegisterControl) pRuntimeRegisterControl(defn);
}

void REALRegisterDBEngine(REALdbEngineDefinition *defn)
{
	static void (*pRegisterDatabaseEngine)(REALdbEngineDefinition *defn);
	if (!pRegisterDatabaseEngine)
		pRegisterDatabaseEngine = (void (*)(REALdbEngineDefinition *)) CallResolver("PluginRegisterDBEngine");
	
	if (pRegisterDatabaseEngine)
		pRegisterDatabaseEngine(defn);
}

void REALRegisterDBTable(REALdbTableDefinition *defn)
{
	static void (*pRegisterDBTable)(REALdbTableDefinition *defn);
	if (!pRegisterDBTable)
		pRegisterDBTable = (void (*)(REALdbTableDefinition *)) CallResolver("PluginRegisterDBTable");
	if (pRegisterDBTable)
		pRegisterDBTable(defn);
}

void REALRegisterDBCursor(REALdbCursorDefinition *defn)
{
	static void (*pRegisterDBCursor)(REALdbCursorDefinition *defn);
	if (!pRegisterDBCursor)
		pRegisterDBCursor = (void (*)(REALdbCursorDefinition *)) CallResolver("PluginRegisterDBCursor");
	
	if (pRegisterDBCursor)
		pRegisterDBCursor(defn);
}

void REALRegisterClass(REALclassDefinition *defn)
{
	static void (*pRegisterClass)(REALclassDefinition *defn);
	if (!pRegisterClass)
		pRegisterClass = (void (*)(REALclassDefinition *)) CallResolver("PluginRegisterClass");
	
	if (pRegisterClass)
		pRegisterClass(defn);
}

REALstring REALDefaultControlFont(void)
{
	static unsigned long (*pDefaultControlFont)(void) = nil;
	if (!pDefaultControlFont)
		pDefaultControlFont = (unsigned long (*)(void)) CallResolver("PluginDefaultControlFont");
	if (pDefaultControlFont) return (REALstring)pDefaultControlFont();
	else return (REALstring)0;
}

unsigned long REALDefaultControlFontSize(void)
{
	static unsigned long (*pDefaultControlFontSize)(void) = nil;
	if (!pDefaultControlFontSize)
		pDefaultControlFontSize = (unsigned long (*)(void)) CallResolver("PluginDefaultControlFontSize");
	if (pDefaultControlFontSize) return pDefaultControlFontSize();
	else return (unsigned long)0;
}

REALstring REALBuildString(const char *contents, int length)
{
	static unsigned long (*pBuildString)(const char *, int) = nil;
	if (!pBuildString)
		pBuildString = (unsigned long (*)(const char *, int)) CallResolver("REALBuildString");
	if (pBuildString) return (REALstring)pBuildString(contents, length);
	else return (REALstring)0;
}

REALstring REALBuildStringWithEncoding( const char *contents, int byteCount, unsigned long encoding )
{
	static unsigned long (*pBuildString)(const char *, int, unsigned long) = nil;
	if (!pBuildString)
		pBuildString = (unsigned long (*)(const char *, int, unsigned long)) CallResolver("REALBuildStringWithEncoding");
	if (pBuildString) return (REALstring)pBuildString(contents, byteCount, encoding);
	else return (REALstring)0;
}

void *REALGetStringContents( REALstring str, size_t *numBytes )
{
	if (!str) return NULL;

	static unsigned long (*pCString)(REALstring) = nil;
	static RBInteger (*pLenB)(REALstring);
	if (!pCString)
		pCString = (unsigned long (*)(REALstring)) CallResolver("StringGetCString");
	if (!pLenB)
		pLenB = (RBInteger (*)(REALstring))REALLoadFrameworkMethod( "LenB( s as string ) as integer" );
	
	if (numBytes && pLenB) {
		*numBytes = pLenB(str);
	}
	
	if (pCString) return (void *)pCString(str);

	return NULL;
}

void REALLockObject(REALobject obj)
{
	static void (*pLockObject)(REALobject) = nil;
	if (!pLockObject)
		pLockObject = (void (*)(REALobject)) CallResolver("REALLockObject");
	if (pLockObject) pLockObject(obj);
}

void REALUnlockObject(REALobject obj)
{
	static void (*pUnlockObject)(REALobject) = nil;
	if (!pUnlockObject)
		pUnlockObject = (void (*)(REALobject)) CallResolver("REALUnlockObject");
	if (pUnlockObject) pUnlockObject(obj);
}

void REALLockString(REALstring str)
{
	static void (*pLockString)(REALstring) = nil;
	if (!pLockString)
		pLockString = (void (*)(REALstring)) CallResolver("REALLockString");
	if (pLockString) pLockString(str);
}

void REALUnlockString(REALstring str)
{
	static void (*pUnlockString)(REALstring) = nil;
	if (!pUnlockString)
		pUnlockString = (void (*)(REALstring)) CallResolver("REALUnlockString");
	if (pUnlockString) pUnlockString(str);
}

#if TARGET_CARBON && !TARGET_COCOA
REALpicture REALBuildPictureFromPicHandle(PicHandle pic, Boolean bPassOwnership)
{
	static unsigned long (*pBuildPictureFromPicHandle)(PicHandle, Boolean) = nil;
	if (!pBuildPictureFromPicHandle)
		pBuildPictureFromPicHandle = (unsigned long (*)(PicHandle, Boolean)) CallResolver("REALBuildPictureFromPicHandle");
	if (pBuildPictureFromPicHandle) return (REALpicture)pBuildPictureFromPicHandle(pic, bPassOwnership);
	else return (REALpicture)0;
}
#endif

REALpicture REALBuildPictureFromGWorld(void *world, Boolean bPassOwnership)
{
	static unsigned long (*pBuildPictureFromGWorld)(void *, Boolean) = nil;
	if (!pBuildPictureFromGWorld)
		pBuildPictureFromGWorld = (unsigned long (*)(void *, Boolean)) CallResolver("REALBuildPictureFromGWorld");
	if (pBuildPictureFromGWorld) return (REALpicture)pBuildPictureFromGWorld(world, bPassOwnership);
	else return (REALpicture)0;
}

REALpicture REALBuildPictureFromPictureDescription(REALpictureDescription *description, Boolean bPassOwnership)
{
	static unsigned long (*pBuildPictureFromPictureDescription)(REALpictureDescription *, Boolean) = nil;
	if (!pBuildPictureFromPictureDescription)
		pBuildPictureFromPictureDescription = (unsigned long (*)(REALpictureDescription *, Boolean)) CallResolver("REALBuildPictureFromPictureDescription");
	if (pBuildPictureFromPictureDescription) return (REALpicture)pBuildPictureFromPictureDescription(description, bPassOwnership);
	else return (REALpicture)0;
}

void REALLockPictureDescriptionWithNativeType(REALpicture pic, REALpictureDescription *description)
{
	static void (*pLockPictureDescription)(REALpicture, REALpictureDescription *) = nil;
	if (!pLockPictureDescription)
		pLockPictureDescription = (void (*)(REALpicture, REALpictureDescription *)) CallResolver("lockPictureDescription");
	if (pLockPictureDescription) pLockPictureDescription(pic, description);
}

void REALUnlockPictureDescription(REALpicture pic)
{
	static void (*pUnlockPictureDescription)(REALpicture) = nil;
	if (!pUnlockPictureDescription)
		pUnlockPictureDescription = (void (*)(REALpicture)) CallResolver("unlockPictureDescription");
	if (pUnlockPictureDescription) pUnlockPictureDescription(pic);
}

void REALLockSoundDescription(REALsound sound, REALsoundDescription *description)
{
	static void (*pLockSoundDescription)(REALsound, REALsoundDescription *) = nil;
	if (!pLockSoundDescription)
		pLockSoundDescription = (void (*)(REALsound, REALsoundDescription *)) CallResolver("");
	if (pLockSoundDescription) pLockSoundDescription(sound, description);
}

void REALUnlockSoundDescription(REALsound sound)
{
	static void (*pUnlockSoundDescription)(REALsound) = nil;
	if (!pUnlockSoundDescription)
		pUnlockSoundDescription = (void (*)(REALsound)) CallResolver("unlockPictureDescription");
	if (pUnlockSoundDescription) pUnlockSoundDescription(sound);
}

REALdbCursor REALdbCursorFromDBCursor(dbCursor *cursor, REALdbCursorDefinition *defn)
{
	static REALdbCursor (*pDbCursorFromDBCursor)(dbCursor *, REALdbCursorDefinition *) = nil;
	if (!pDbCursorFromDBCursor)
		pDbCursorFromDBCursor = (REALdbCursor (*)(dbCursor *, REALdbCursorDefinition *)) CallResolver("REALdbCursorFromDBCursor");
	if (pDbCursorFromDBCursor) return pDbCursorFromDBCursor(cursor, defn);
	else return (REALdbCursor)0;
}

REALdbDatabase REALdbDatabaseFromDBDatabase(dbDatabase *database, REALdbEngineDefinition *defn)
{
	static REALdbDatabase (*pDbDatabaseFromDBDatabase)(dbDatabase *, REALdbEngineDefinition *) = nil;
	if (!pDbDatabaseFromDBDatabase)
		pDbDatabaseFromDBDatabase = (REALdbDatabase (*)(dbDatabase *, REALdbEngineDefinition *)) CallResolver("REALdbDatabaseFromDBDatabase");
	if (pDbDatabaseFromDBDatabase) return pDbDatabaseFromDBDatabase(database, defn);
	else return (REALdbDatabase)0;
}

void *REALGetEventInstance(REALcontrolInstance instance, REALevent *event)
{
	static void *(*pGetEventInstance)(REALcontrolInstance instance, int builtHook);
	if (!pGetEventInstance)
		pGetEventInstance = (void *(*)(REALcontrolInstance,int)) CallResolver("GetEventInstance");
	if (pGetEventInstance) return (void *) pGetEventInstance(instance, event->forSystemUse);

	return nil;
}

void *REALGetControlData(REALcontrolInstance instance, REALcontrol *defn)
{
	return ((Ptr) instance) + defn->forSystemUse;
}

void *REALGetClassData(REALobject instance, REALclassDefinition *defn)
{
	return ((Ptr) instance) + defn->forSystemUse;
}

#if TARGET_CARBON && !TARGET_64BIT
REALfolderItem REALFolderItemFromFSSpec(const FSSpec *spec)
{
	static unsigned long (*pFolderItemFromFSSpec)(const FSSpec *) = nil;
	if (!pFolderItemFromFSSpec)
		pFolderItemFromFSSpec = (unsigned long (*)(const FSSpec *)) CallResolver("FolderItemFromFSSpec");
	if (pFolderItemFromFSSpec) return (REALfolderItem)pFolderItemFromFSSpec(spec);
	else return (REALfolderItem)0;
}
#endif

#if TARGET_CARBON && !TARGET_64BIT
Boolean REALFSSpecFromFolderItem(FSSpec *spec, REALfolderItem item)
{
	static Boolean (*pFSSpecFromFolderItem)(FSSpec *, REALfolderItem) = nil;
	if (!pFSSpecFromFolderItem)
		pFSSpecFromFolderItem = (Boolean (*)(FSSpec *, REALfolderItem)) CallResolver("REALFSSpecFromFolderItem");
	if (pFSSpecFromFolderItem) return pFSSpecFromFolderItem(spec, item);
	else return (Boolean)0;
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
void REALSelectGraphics(REALgraphics context)
{
	static void (*pSelectGraphics)(REALgraphics) = nil;
	if (!pSelectGraphics)
		pSelectGraphics = (void (*)(REALgraphics)) CallResolver("SelectGraphics");
	if (pSelectGraphics) pSelectGraphics(context);
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
void REALGraphicsDrawOffscreenMacControl(REALgraphics context, ControlHandle mh)
{
	static void (*pGraphicsDrawOffscreenMacControl)(REALgraphics, ControlHandle) = nil;
	if (!pGraphicsDrawOffscreenMacControl)
		pGraphicsDrawOffscreenMacControl = (void (*)(REALgraphics, ControlHandle)) CallResolver("DrawOffscreenMacControl");
	if (pGraphicsDrawOffscreenMacControl) pGraphicsDrawOffscreenMacControl(context, mh);
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
REALsound REALBuildSoundFromHandle(Handle sound, Boolean bPassOwnership)
{
	static REALsound (*pBuildSoundFromHandle)(Handle, Boolean) = nil;
	if (!pBuildSoundFromHandle)
		pBuildSoundFromHandle = (REALsound (*)(Handle, Boolean)) CallResolver("REALBuildSoundFromHandle");
	if (pBuildSoundFromHandle) return pBuildSoundFromHandle(sound, bPassOwnership);
	else return (REALsound)0;
}
#endif

#if TARGET_CARBON
REALappleEvent REALBuildAppleEvent(const AppleEvent *event, Boolean bPassOwnership)
{
	static unsigned long (*pBuildAppleEvent)(const AppleEvent *, Boolean) = nil;
	if (!pBuildAppleEvent)
		pBuildAppleEvent = (unsigned long (*)(const AppleEvent *, Boolean)) CallResolver("REALBuildAppleEvent");
	if (pBuildAppleEvent) return (REALappleEvent)pBuildAppleEvent(event, bPassOwnership);
	else return (REALappleEvent)0;
}
#endif

#if TARGET_CARBON
REALappleEvent REALBuildAEDescList(const AppleEvent *event, Boolean bPassOwnership)
{
	static unsigned long (*pBuildAEDescList)(const AppleEvent *, Boolean) = nil;
	if (!pBuildAEDescList)
		pBuildAEDescList = (unsigned long (*)(const AppleEvent *, Boolean)) CallResolver("REALBuildAEDescList");
	if (pBuildAEDescList) return (REALappleEvent)pBuildAEDescList(event, bPassOwnership);
	else return (REALappleEvent)0;
}
#endif

#if TARGET_CARBON
REALappleEvent REALBuildAEObjSpecifier(const AppleEvent *event, Boolean bPassOwnership)
{
	static unsigned long (*pBuildAEObjSpecifier)(const AppleEvent *, Boolean) = nil;
	if (!pBuildAEObjSpecifier)
		pBuildAEObjSpecifier = (unsigned long (*)(const AppleEvent *, Boolean)) CallResolver("REALBuildAEObjSpecifier");
	if (pBuildAEObjSpecifier) return (REALappleEvent)pBuildAEObjSpecifier(event, bPassOwnership);
	else return (REALappleEvent)0;
}
#endif

#if TARGET_CARBON
AppleEvent *REALAccessAppleEvent(REALappleEvent event)
{
	static unsigned long (*pAccessAppleEvent)(REALappleEvent) = nil;
	if (!pAccessAppleEvent)
		pAccessAppleEvent = (unsigned long (*)(REALappleEvent)) CallResolver("REALAccessAppleEvent");
	if (pAccessAppleEvent) return (AppleEvent *)pAccessAppleEvent(event);
	else return (AppleEvent *)0;
}
#endif

#if TARGET_CARBON
AppleEvent *REALAccessAppleEventReply(REALappleEvent event)
{
	static unsigned long (*pAccessAppleEventReply)(REALappleEvent) = nil;
	if (!pAccessAppleEventReply)
		pAccessAppleEventReply = (unsigned long (*)(REALappleEvent)) CallResolver("REALAccessAppleEventReply");
	if (pAccessAppleEventReply) return (AppleEvent *)pAccessAppleEventReply(event);
	else return (AppleEvent *)0;
}
#endif

#if (TARGET_CARBON && !TARGET_COCOA) && !TARGET_64BIT
REALmovie REALbuildMovie(QT_NAMESPACE Movie movie, int resRefNum, int bNew)
{
	static unsigned long (*pBuildMovie)(QT_NAMESPACE Movie, int, int) = nil;
	if (!pBuildMovie)
		pBuildMovie = (unsigned long (*)(QT_NAMESPACE Movie, int, int)) CallResolver("buildMovie");
	if (pBuildMovie) return (REALmovie)pBuildMovie(movie, resRefNum, bNew);
	else return (REALmovie)0;
}
#endif

#if (TARGET_CARBON || TARGET_COCOA) && TARGET_64BIT
void REALmarkMovieDirty(REALmovie movie)
{
	static void (*pMarkMovieDirty)(REALmovie) = nil;
	if (!pMarkMovieDirty)
		pMarkMovieDirty = (void (*)(REALmovie)) CallResolver("markMovieDirty");
	if (pMarkMovieDirty) pMarkMovieDirty(movie);
}
#endif

#if (TARGET_CARBON || TARGET_WIN32 || TARGET_COCOA) && !TARGET_64BIT
int REALenterMovies(void)
{
	static int (*pEnterMovies)(void) = nil;
	if (!pEnterMovies)
		pEnterMovies = (int (*)(void)) CallResolver("REALenterMovies");
	if (pEnterMovies) return pEnterMovies();
	else return (int)0;
}
#endif

void REALRegisterDataSourceInterface(const char *szMenuName, REALDataSourceInterfaceProc proc)
{
	static void (*pRegisterDataSourceInterface)(const char *, REALDataSourceInterfaceProc) = nil;
	if (!pRegisterDataSourceInterface)
		pRegisterDataSourceInterface = (void (*)(const char *, REALDataSourceInterfaceProc)) CallResolver("RegisterDataSourceInterface");
	if (pRegisterDataSourceInterface)
		pRegisterDataSourceInterface(szMenuName, proc);
}

void REALRegisterDataSource(const char *szDatasourceName, REALDataSourceProc proc)
{
	static void (*pRegisterDataSource)(const char *, REALDataSourceProc);
	if (!pRegisterDataSource)
		pRegisterDataSource = (void (*)(const char *, REALDataSourceProc)) CallResolver("RegisterDataSource");
	if (pRegisterDataSource) {
	pRegisterDataSource(szDatasourceName, proc);
	}
}

void REALRegisterDatabaseConnection(REALDatabaseConnectionDefinition *defn)
{
	static void (*pRegisterDatabaseConnection)(REALDatabaseConnectionDefinition *);
	if (!pRegisterDatabaseConnection)
		pRegisterDatabaseConnection = (void (*)(REALDatabaseConnectionDefinition *)) CallResolver("RegisterDatabaseConnection");
	if (pRegisterDatabaseConnection) {
	pRegisterDatabaseConnection(defn);
	}
}

void REALDesignAddDataSource(const char *baseName, const char *szDataSourceName, Ptr data, int dataLen)
{
	static void (*pDesignAddDataSource)(const char *, const char *, Ptr, int) = nil;
	if (!pDesignAddDataSource)
		pDesignAddDataSource = (void (*)(const char *, const char *, Ptr, int)) CallResolver("DesignAddDataSource");
	if (pDesignAddDataSource) pDesignAddDataSource(baseName, szDataSourceName, data, dataLen);
}

#if TARGET_WIN32
REALpicture REALBuildPictureFromDIB(HANDLE hDIB, Boolean bPassOwnership)
{
	static unsigned long (*pBuildPictureFromDIB)(HANDLE, Boolean) = nil;
	if (!pBuildPictureFromDIB)
		pBuildPictureFromDIB = (unsigned long (*)(HANDLE, Boolean)) CallResolver("REALBuildPictureFromDIB");
	if (pBuildPictureFromDIB) return (REALpicture)pBuildPictureFromDIB(hDIB, bPassOwnership);
	else return (REALpicture)0;
}
#endif

double REALGetRBVersion(void)
{
	static double (*pGetRBVersion)(void) = nil;
	if (!pGetRBVersion)
		pGetRBVersion = (double (*)(void)) CallResolver("REALGetRBVersion");
	if (pGetRBVersion) return pGetRBVersion();
	else return (double)0;
}

void REALRaiseException(REALobject exception)
{
	static void (*pRaiseException)(REALobject) = nil;
	if (!pRaiseException)
		pRaiseException = (void (*)(REALobject)) CallResolver("RuntimeRaiseException");
	if (pRaiseException) pRaiseException(exception);
}

int REALGetArrayUBound(void*array)
{
	static int (*pGetArrayUBound)(void*) = nil;
	if (!pGetArrayUBound)
		pGetArrayUBound = (int (*)(void*)) CallResolver("RuntimeUBound");
	if (pGetArrayUBound) return pGetArrayUBound(array);
	else return (int)0;
}

void REALGetArrayStructure( REALarray array, int index, void *structure )
{
	static void(*pGetStructureObject)(REALarray, int, void *) = nil;
	if (!pGetStructureObject)
		pGetStructureObject = (void (*)(REALarray, int, void *)) CallResolver("RuntimeDirectReadStructureArray");
	if (pGetStructureObject) pGetStructureObject(array, index, structure);
}

Boolean REALGetVariantStructure( REALobject variant, void *buffer, unsigned long length )
{
	static Boolean(*pGetStructureObject)(REALobject, void *, unsigned long) = nil;
	if (!pGetStructureObject)
		pGetStructureObject = (Boolean (*)(REALobject, void *, unsigned long)) CallResolver("PluginStructureFromVariant");
	if (pGetStructureObject) return pGetStructureObject( variant, buffer, length );
	return (Boolean)0;
}

void REALYieldToRB(void)
{
	static void (*pYieldToRB)(void) = nil;
	if (!pYieldToRB)
		pYieldToRB = (void (*)(void)) CallResolver("REALYieldToRB");
	if (pYieldToRB) pYieldToRB();
}

REALclassRef REALGetClassRef(const char *className)
{
	static REALclassRef (*pGetClassRef)(const char *) = nil;
	if (!pGetClassRef)
		pGetClassRef = (REALclassRef (*)(const char *)) CallResolver("pluginObjectType");
	if (pGetClassRef) return pGetClassRef(className);
	else return (REALclassRef)0;
}

REALobject REALnewInstanceWithClass(REALclassRef classRef)
{
	static unsigned long (*pNewInstance)(REALclassRef) = nil;
	if (!pNewInstance)
		pNewInstance = (unsigned long (*)(REALclassRef)) CallResolver("CreateInstance");
	if (pNewInstance and classRef) return (REALobject)pNewInstance(classRef);
	else return (REALobject)0;
}

void REALRegisterInterface(REALinterfaceDefinition *defn)
{
	static void (*pRegisterInterface)(REALinterfaceDefinition *defn);
	if (!pRegisterInterface)
		pRegisterInterface = (void (*)(REALinterfaceDefinition *)) CallResolver("PluginRegisterInterface");
	
	if (pRegisterInterface)
		pRegisterInterface(defn);
}

REALstring REALGetDBHost(REALdbDatabase db)
{
	static REALstring (*pGetDBHost)(REALdbDatabase) = nil;
	if (!pGetDBHost)
		pGetDBHost = (REALstring (*)(REALdbDatabase)) CallResolver("REALGetDBHost");
	if (pGetDBHost) return pGetDBHost(db);
	else return (REALstring)0;
}

REALstring REALGetDBDatabaseName(REALdbDatabase db)
{
	static REALstring (*pGetDBDatabaseName)(REALdbDatabase) = nil;
	if (!pGetDBDatabaseName)
		pGetDBDatabaseName = (REALstring (*)(REALdbDatabase)) CallResolver("REALGetDBDatabaseName");
	if (pGetDBDatabaseName) return pGetDBDatabaseName(db);
	else return (REALstring)0;
}

REALstring REALGetDBPassword(REALdbDatabase db)
{
	static REALstring (*pGetDBPassword)(REALdbDatabase) = nil;
	if (!pGetDBPassword)
		pGetDBPassword = (REALstring (*)(REALdbDatabase)) CallResolver("REALGetDBPassword");
	if (pGetDBPassword) return pGetDBPassword(db);
	else return (REALstring)0;
}

REALstring REALGetDBUserName(REALdbDatabase db)
{
	static REALstring (*pGetDBUserName)(REALdbDatabase) = nil;
	if (!pGetDBUserName)
		pGetDBUserName = (REALstring (*)(REALdbDatabase)) CallResolver("REALGetDBUserName");
	if (pGetDBUserName) return pGetDBUserName(db);
	else return (REALstring)0;
}

dbDatabase *REALGetDBFromREALdbDatabase(REALdbDatabase db)
{
	static dbDatabase *(*pGetDBFromREALdbDatabase)(REALdbDatabase) = nil;
	if (!pGetDBFromREALdbDatabase)
		pGetDBFromREALdbDatabase = (dbDatabase *(*)(REALdbDatabase)) CallResolver("REALGetDBFromREALdbDatabase");
	if (pGetDBFromREALdbDatabase) return pGetDBFromREALdbDatabase(db);
	else return (dbDatabase *)0;
}

void REALConstructDBDatabase(REALdbDatabase db, dbDatabase *mydb, REALdbEngineDefinition *engine)
{
	static void (*pConstructDBDatabase)(REALdbDatabase, dbDatabase *, REALdbEngineDefinition *) = nil;
	if (!pConstructDBDatabase)
		pConstructDBDatabase = (void (*)(REALdbDatabase, dbDatabase *, REALdbEngineDefinition *)) CallResolver("REALConstructDBDatabase");
	if (pConstructDBDatabase) pConstructDBDatabase(db, mydb, engine);
}

#if TARGET_CARBON || TARGET_COCOA
Boolean REALFSRefFromFolderItem(REALfolderItem f, FSRef*outRef, HFSUniStr255*outName)
{
	static Boolean (*pFSRefFromFolderItem)(REALfolderItem, FSRef*, HFSUniStr255*) = nil;
	if (!pFSRefFromFolderItem)
		pFSRefFromFolderItem = (Boolean (*)(REALfolderItem, FSRef*, HFSUniStr255*)) CallResolver("REALFSRefFromFolderItem");
	if (pFSRefFromFolderItem) return pFSRefFromFolderItem(f, outRef, outName);
	else return (Boolean)0;
}
#endif

#if TARGET_CARBON || TARGET_COCOA
	#if FLAT_C_PLUGIN_HEADERS
		REALfolderItem REALFolderItemFromParentFSRef(const FSRef *parent, const HFSUniStr255 *fileName)
		{
			static REALfolderItem (*pFolderItemFromParentFSRef)(const FSRef*, const HFSUniStr255*) = nil;
			if (!pFolderItemFromParentFSRef)
				pFolderItemFromParentFSRef = (REALfolderItem (*)(const FSRef*, const HFSUniStr255*)) CallResolver("REALFolderItemFromParentFSRef");
			if (pFolderItemFromParentFSRef) return pFolderItemFromParentFSRef(parent, fileName);
			else return (REALfolderItem)0;
		}
	#else
		REALfolderItem REALFolderItemFromParentFSRef(const FSRef& parent, const HFSUniStr255& fileName)
		{
			static REALfolderItem (*pFolderItemFromParentFSRef)(const FSRef&, const HFSUniStr255&) = nil;
			if (!pFolderItemFromParentFSRef)
				pFolderItemFromParentFSRef = (REALfolderItem (*)(const FSRef&, const HFSUniStr255&)) CallResolver("REALFolderItemFromParentFSRef");
			if (pFolderItemFromParentFSRef) return pFolderItemFromParentFSRef(parent, fileName);
			else return (REALfolderItem)0;
		}
	#endif
#endif

REALDBConnectionDialogRef REALDBConnectionDialogCreate(void)
{
	static REALDBConnectionDialogRef (*pDBConnectionDialogCreate)(void) = nil;
	if (!pDBConnectionDialogCreate)
		pDBConnectionDialogCreate = (REALDBConnectionDialogRef (*)(void)) CallResolver("REALDBConnectionDialogCreate");
	if (pDBConnectionDialogCreate) return pDBConnectionDialogCreate();
	else return (REALDBConnectionDialogRef)0;
}

void REALDBConnectionDialogAddField(REALDBConnectionDialogRef dialogRef, REALstring label, REALstring defaultText, Boolean maskField)
{
	static void (*pDBConnectionDialogAddField)(REALDBConnectionDialogRef, REALstring, REALstring, Boolean) = nil;
	if (!pDBConnectionDialogAddField)
		pDBConnectionDialogAddField = (void (*)(REALDBConnectionDialogRef, REALstring, REALstring, Boolean)) CallResolver("REALDBConnectionDialogAddField");
	if (pDBConnectionDialogAddField) pDBConnectionDialogAddField(dialogRef, label, defaultText, maskField);
}

REALstring REALDBConnectionDialogShow(REALDBConnectionDialogRef dialogRef, REALstring title)
{
	static REALstring (*pDBConnectionDialogShow)(REALDBConnectionDialogRef, REALstring) = nil;
	if (!pDBConnectionDialogShow)
		pDBConnectionDialogShow = (REALstring (*)(REALDBConnectionDialogRef, REALstring)) CallResolver("REALDBConnectionDialogShow");
	if (pDBConnectionDialogShow) return pDBConnectionDialogShow(dialogRef, title);
	else return (REALstring)0;
}

void REALDBConnectionDialogDelete(REALDBConnectionDialogRef dialogRef)
{
	static void (*pDBConnectionDialogDelete)(REALDBConnectionDialogRef) = nil;
	if (!pDBConnectionDialogDelete)
		pDBConnectionDialogDelete = (void (*)(REALDBConnectionDialogRef)) CallResolver("REALDBConnectionDialogDelete");
	if (pDBConnectionDialogDelete) pDBConnectionDialogDelete(dialogRef);
}

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REALpicture REALBuildPictureFromBuffer(long width, long height, RBPixelType pixelType, void*buffer, long rowBytes)
{
	static unsigned long (*pBuildPictureFromBuffer)(long, long, RBPixelType, void*, long) = nil;
	if (!pBuildPictureFromBuffer)
		pBuildPictureFromBuffer = (unsigned long (*)(long, long, RBPixelType, void*, long)) CallResolver("REALBuildPictureFromBuffer");
	if (pBuildPictureFromBuffer) return (REALpicture)pBuildPictureFromBuffer(width, height, pixelType, buffer, rowBytes);
	else return (REALpicture)0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
Boolean REALInDebugMode(void)
{
	static Boolean (*pInDebugMode)(void) = nil;
	if (!pInDebugMode)
		pInDebugMode = (Boolean (*)(void)) CallResolver("REALInDebugMode");
	if (pInDebugMode) return pInDebugMode();
	else return (Boolean)0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
void REALStripAmpersands(REALstring*  ioString)
{
	static void (*pStripAmpersands)(REALstring* ) = nil;
	if (!pStripAmpersands)
		pStripAmpersands = (void (*)(REALstring* )) CallResolver("REALStripAmpersands");
	if (pStripAmpersands) pStripAmpersands(ioString);
}
#endif

REALobject REALGetProjectFolder(void)
{
	static REALobject (*pGetProjectFolder)(void) = nil;
	if (!pGetProjectFolder)
		pGetProjectFolder = (REALobject (*)(void)) CallResolver("REALGetProjectFolder");
	if (pGetProjectFolder) return pGetProjectFolder();
	else return (REALobject)0;
}

void REALRegisterModule(REALmoduleDefinition*defn)
{
	static void (*pRegisterModule)(REALmoduleDefinition*) = nil;
	if (!pRegisterModule)
		pRegisterModule = (void (*)(REALmoduleDefinition*)) CallResolver("PluginRegisterModule");
	
	if (pRegisterModule) pRegisterModule(defn);
}

void *REALLoadFrameworkMethod( const char *prototype )
{
	static void*(*pLoadGlobalMethod)(const char *, const char *) = nil;
	if (!pLoadGlobalMethod)
		pLoadGlobalMethod = (void*(*)(const char *, const char *)) CallResolver("REALLoadGlobalMethod");
	if (pLoadGlobalMethod) return pLoadGlobalMethod( NULL, prototype );
	else return (void*)0;
}

void *REALLoadObjectMethod( REALobject object, const char *prototype )
{
	static void*(*pLoadObjectMethod)(REALobject, const char *) = nil;
	if (!pLoadObjectMethod)
		pLoadObjectMethod = (void*(*)(REALobject, const char *)) CallResolver("REALLoadObjectMethod");
	if (pLoadObjectMethod) return pLoadObjectMethod(object, prototype);
	else return (void*)0;
}

Boolean REALGetPropValueInt32(REALobject object, const char *propName, long *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, long *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, long *)) CallResolver("REALGetPropValueInt");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValueString(REALobject object, const char *propName, REALstring *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, REALstring *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, REALstring *)) CallResolver("REALGetPropValueString");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValueDouble(REALobject object, const char *propName, double *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, double *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, double *)) CallResolver("REALGetPropValueDouble");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValueObject(REALobject object, const char *propName, REALobject *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, REALobject *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, REALobject *)) CallResolver("REALGetPropValueObject");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValuePtr( REALobject object, const char *propName, void **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, void **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, void **)) CallResolver("REALGetPropValuePtr");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValueCString( REALobject object, const char *propName, const char **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, const char **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, const char **)) CallResolver("REALGetPropValueCString");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValueWString( REALobject object, const char *propName, const wchar_t **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, const wchar_t **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, const wchar_t **)) CallResolver("REALGetPropValueWString");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

Boolean REALGetPropValuePString( REALobject object, const char *propName, const unsigned char **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, const unsigned char **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, const unsigned char **)) CallResolver("REALGetPropValuePString");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}

#if TARGET_CARBON || TARGET_COCOA
Boolean REALGetPropValueCFStringRef( REALobject object, const char *propName, CFStringRef *outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, CFStringRef *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, CFStringRef *)) CallResolver("REALGetPropValueCFStringRef");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, propName, outValue);
	else return (Boolean)0;
}
#endif

void REALSetDBIsConnected(REALdbDatabase database, Boolean connected)
{
	static void (*pSetDBIsConnected)(REALdbDatabase, Boolean) = nil;
	if (!pSetDBIsConnected)
		pSetDBIsConnected = (void (*)(REALdbDatabase, Boolean)) CallResolver("REALSetDBIsConnected");
	if (pSetDBIsConnected) pSetDBIsConnected(database, connected);
}

REALobject REALNewVariantString(REALstring value)
{
	static REALobject (*pNewVariantString)(REALstring) = nil;
	if (!pNewVariantString)
		pNewVariantString = (REALobject (*)(REALstring)) CallResolver("StringToVariant");
	if (pNewVariantString) return pNewVariantString(value);
	else return (REALobject)0;
}

REALobject REALNewVariantStructure( const void *data, unsigned long len )
{
	static REALobject (*pNewVariantStructure)( const void *, unsigned long ) = nil;
	if (!pNewVariantStructure)
		pNewVariantStructure = (REALobject (*)(const void *, unsigned long)) CallResolver( "PluginStructureToVariant" );
	if (pNewVariantStructure) return pNewVariantStructure( data, len );
	else return (REALobject)0;
}

REALobject REALNewVariantPtr( void *value )
{
	static REALobject (*pNewVariant)(void*) = nil;
	if (!pNewVariant)
		pNewVariant = (REALobject (*)(void*)) CallResolver("PtrToVariant");
	if (pNewVariant) return pNewVariant(value);
	else return (REALobject)0;
}

REALobject REALNewVariantCString( const char *value )
{
	static REALobject (*pNewVariant)(const char *) = nil;
	if (!pNewVariant)
		pNewVariant = (REALobject (*)(const char *)) CallResolver("CStringToVariant");
	if (pNewVariant) return pNewVariant(value);
	else return (REALobject)0;
}

REALobject REALNewVariantWString( const wchar_t *value )
{
	static REALobject (*pNewVariant)(const wchar_t *) = nil;
	if (!pNewVariant)
		pNewVariant = (REALobject (*)(const wchar_t *)) CallResolver("WStringToVariant");
	if (pNewVariant) return pNewVariant(value);
	else return (REALobject)0;
}

REALobject REALNewVariantPString( const unsigned char *value )
{
	static REALobject (*pNewVariant)(const unsigned char *) = nil;
	if (!pNewVariant)
		pNewVariant = (REALobject (*)(const unsigned char *)) CallResolver("PStringToVariant");
	if (pNewVariant) return pNewVariant(value);
	else return (REALobject)0;
}

REALobject REALNewVariantOSType( unsigned long value )
{
	static REALobject (*pNewVariant)(unsigned long) = nil;
	if (!pNewVariant)
		pNewVariant = (REALobject (*)(unsigned long)) CallResolver("OSTypeToVariant");
	if (pNewVariant) return pNewVariant(value);
	else return (REALobject)0;
}

#if TARGET_CARBON || TARGET_COCOA
	REALobject REALNewVariantCFStringRef( CFStringRef value )
	{
		static REALobject (*pNewVariant)(CFStringRef) = nil;
		if (!pNewVariant)
			pNewVariant = (REALobject (*)(CFStringRef)) CallResolver("CFStringRefToVariant");
		if (pNewVariant) return pNewVariant(value);
		else return (REALobject)0;
	}
#endif

REALobject REALNewVariantInteger(long value)
{
	static REALobject (*pNewVariantInteger)(long) = nil;
	if (!pNewVariantInteger)
		pNewVariantInteger = (REALobject (*)(long)) CallResolver("Int32ToVariant");
	if (pNewVariantInteger) return pNewVariantInteger(value);
	else return (REALobject)0;
}

REALobject REALNewVariantDouble(double value)
{
	static REALobject (*pNewVariantDouble)(double) = nil;
	if (!pNewVariantDouble)
		pNewVariantDouble = (REALobject (*)(double)) CallResolver("DoubleToVariant");
	if (pNewVariantDouble) return pNewVariantDouble(value);
	else return (REALobject)0;
}

REALobject REALNewVariantBoolean(Boolean value)
{
	static REALobject (*pNewVariantBoolean)(Boolean) = nil;
	if (!pNewVariantBoolean)
		pNewVariantBoolean = (REALobject (*)(Boolean)) CallResolver("BooleanToVariant");
	if (pNewVariantBoolean) return pNewVariantBoolean(value);
	else return (REALobject)0;
}

REALobject REALNewVariantColor(RBColor value)
{
	static REALobject (*pNewVariantColor)(RBColor) = nil;
	if (!pNewVariantColor)
		pNewVariantColor = (REALobject (*)(RBColor)) CallResolver("ColorToVariant");
	if (pNewVariantColor) return pNewVariantColor(value);
	else return (REALobject)0;
}

Boolean REALSetPropValueInt32(REALobject object, const char *propName, long value)
{
	static long (*pSetPropValue)(REALobject, const char *, long) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, long)) CallResolver("REALSetPropValueInt");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, propName, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueString(REALobject object, const char *propName, REALstring value)
{
	static long (*pSetPropValue)(REALobject, const char *, REALstring) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, REALstring)) CallResolver("REALSetPropValueString");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, propName, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueDouble(REALobject object, const char *propName, double value)
{
	static long (*pSetPropValue)(REALobject, const char *, double) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, double)) CallResolver("REALSetPropValueDouble");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, propName, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueObject(REALobject object, const char *propName, REALobject value)
{
	static long (*pSetPropValue)(REALobject, const char *, REALobject) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, REALobject)) CallResolver("REALSetPropValueObject");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, propName, value);
	else return (Boolean)0;
}

dbCursor *REALGetCursorFromREALdbCursor(REALdbCursor cursor)
{
	static dbCursor *(*pGetCursorFromREALdbCursor)(REALdbCursor) = nil;
	if (!pGetCursorFromREALdbCursor)
		pGetCursorFromREALdbCursor = (dbCursor *(*)(REALdbCursor)) CallResolver("REALGetCursorFromREALdbCursor");
	if (pGetCursorFromREALdbCursor) return pGetCursorFromREALdbCursor(cursor);
	else return (dbCursor *)0;
}

Boolean REALLockPictureDescription(REALpicture pic, REALpictureDescription *description, long picType)
{
	static Boolean (*pLockPictureDescription)(REALpicture, REALpictureDescription *, long) = nil;
	if (!pLockPictureDescription)
		pLockPictureDescription = (Boolean (*)(REALpicture, REALpictureDescription *, long)) CallResolver("REALLockPictureDescription");
	if (pLockPictureDescription) return pLockPictureDescription(pic, description, picType);
	else return (Boolean)0;
}

REALobject REALNewVariantUInt32(unsigned long value)
{
	static REALobject (*pNewVariantUInt32)(unsigned long) = nil;
	if (!pNewVariantUInt32)
		pNewVariantUInt32 = (REALobject (*)(unsigned long)) CallResolver("UInt32ToVariant");
	if (pNewVariantUInt32) return pNewVariantUInt32(value);
	else return (REALobject)0;
}

REALobject REALNewVariantInt64(RBInt64 value)
{
	static REALobject (*pNewVariantInt64)(RBInt64) = nil;
	if (!pNewVariantInt64)
		pNewVariantInt64 = (REALobject (*)(RBInt64)) CallResolver("Int64ToVariant");
	if (pNewVariantInt64) return pNewVariantInt64(value);
	else return (REALobject)0;
}

REALobject REALNewVariantUInt64(RBUInt64 value)
{
	static REALobject (*pNewVariantUInt64)(RBUInt64) = nil;
	if (!pNewVariantUInt64)
		pNewVariantUInt64 = (REALobject (*)(RBUInt64)) CallResolver("UInt64ToVariant");
	if (pNewVariantUInt64) return pNewVariantUInt64(value);
	else return (REALobject)0;
}

REALobject REALNewVariantSingle(float value)
{
	static REALobject (*pNewVariantSingle)(float) = nil;
	if (!pNewVariantSingle)
		pNewVariantSingle = (REALobject (*)(float)) CallResolver("SingleToVariant");
	if (pNewVariantSingle) return pNewVariantSingle(value);
	else return (REALobject)0;
}

REALobject REALNewVariantCurrency(REALcurrency value)
{
	static REALobject (*pNewVariantCurrency)(REALcurrency) = nil;
	if (!pNewVariantCurrency)
		pNewVariantCurrency = (REALobject (*)(REALcurrency)) CallResolver("CurrencyToVariant");
	if (pNewVariantCurrency) return pNewVariantCurrency(value);
	else return (REALobject)0;
}

Boolean REALGetPropValueInt64(REALobject object, const char *name, RBInt64 *value)
{
	static long (*pGetPropValue)(REALobject, const char *, RBInt64 *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, RBInt64 *)) CallResolver("REALGetPropValueInt64");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueUInt64(REALobject object, const char *param, RBUInt64 *value)
{
	static long (*pGetPropValue)(REALobject, const char *, RBUInt64 *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, RBUInt64 *)) CallResolver("REALGetPropValueUInt64");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, param, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueUInt32(REALobject object, const char *name, unsigned long *value)
{
	static long (*pGetPropValue)(REALobject, const char *, unsigned long *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, unsigned long *)) CallResolver("REALGetPropValueUInt32");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueUInt16(REALobject object, const char *name, unsigned short *value)
{
	static long (*pGetPropValue)(REALobject, const char *, unsigned short *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, unsigned short *)) CallResolver("REALGetPropValueUInt16");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueInt16(REALobject object, const char *name, short *value)
{
	static long (*pGetPropValue)(REALobject, const char *, short *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, short *)) CallResolver("REALGetPropValueInt16");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueInt8(REALobject object, const char *name, char *value)
{
	static long (*pGetPropValue)(REALobject, const char *, char *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, char *)) CallResolver("REALGetPropValueInt8");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueUInt8(REALobject object, const char *name, unsigned char *value)
{
	static long (*pGetPropValue)(REALobject, const char *, unsigned char *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, unsigned char *)) CallResolver("REALGetPropValueBool");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALGetPropValueSingle(REALobject object, const char *name, float *value)
{
	static long (*pGetPropValue)(REALobject, const char *, float *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, float *)) CallResolver("REALGetPropValueSingle");
	if (pGetPropValue) return (Boolean)pGetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueUInt32(REALobject object, const char *name, unsigned long value)
{
	static long (*pSetPropValue)(REALobject, const char *, unsigned long) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, unsigned long)) CallResolver("REALSetPropValueUInt32");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueUInt64(REALobject object, const char *name, RBUInt64 value)
{
	static long (*pSetPropValue)(REALobject, const char *, RBUInt64) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, RBUInt64)) CallResolver("REALSetPropValueUInt64");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueInt64(REALobject object, const char *name, RBInt64 value)
{
	static long (*pSetPropValue)(REALobject, const char *, RBInt64) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, RBInt64)) CallResolver("REALSetPropValueInt64");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueInt16(REALobject object, const char *name, short value)
{
	static long (*pSetPropValue)(REALobject, const char *, short) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, short)) CallResolver("REALSetPropValueInt16");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueUInt16(REALobject object, const char *name, unsigned short value)
{
	static long (*pSetPropValue)(REALobject, const char *, unsigned short) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, unsigned short)) CallResolver("REALSetPropValueUInt16");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueInt8(REALobject object, const char *name, char value)
{
	static long (*pSetPropValue)(REALobject, const char *, char) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, char)) CallResolver("REALSetPropValueInt8");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueUInt8(REALobject object, const char *name, unsigned char value)
{
	static long (*pSetPropValue)(REALobject, const char *, unsigned char) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, unsigned char)) CallResolver("REALSetPropValueBoolean");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueSingle(REALobject object, const char *name, float value)
{
	static long (*pSetPropValue)(REALobject, const char *, float) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, float)) CallResolver("REALSetPropValueSingle");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValuePtr(REALobject object, const char *name, void *value)
{
	static long (*pSetPropValue)(REALobject, const char *, void *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, void *)) CallResolver("REALSetPropValuePtr");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueCString(REALobject object, const char *name, const char *value)
{
	static long (*pSetPropValue)(REALobject, const char *, const char *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, const char *)) CallResolver("REALSetPropValueCString");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValueWString(REALobject object, const char *name, const wchar_t *value)
{
	static long (*pSetPropValue)(REALobject, const char *, const wchar_t *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, const wchar_t *)) CallResolver("REALSetPropValueWString");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

Boolean REALSetPropValuePString(REALobject object, const char *name, const unsigned char *value)
{
	static long (*pSetPropValue)(REALobject, const char *, const unsigned char *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, const unsigned char *)) CallResolver("REALSetPropValuePString");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}

#if TARGET_CARBON || TARGET_COCOA
Boolean REALSetPropValueCFStringRef(REALobject object, const char *name, CFStringRef value)
{
	static long (*pSetPropValue)(REALobject, const char *, CFStringRef) = nil;
	if (!pSetPropValue)
		pSetPropValue = (long (*)(REALobject, const char *, CFStringRef)) CallResolver("REALSetPropValueCFStringRef");
	if (pSetPropValue) return (Boolean)pSetPropValue(object, name, value);
	else return (Boolean)0;
}
#endif

void REALInsertArrayValueInt64( REALarray arr, long index, RBInt64 value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, RBInt64 ) = (void (*)(REALarray, long, RBInt64))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueInt32( REALarray arr, long index, long value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, long ) = (void (*)(REALarray, long, long))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueInt16( REALarray arr, long index, short value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, short ) = (void (*)(REALarray, long, short))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueInt8( REALarray arr, long index, char value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, char ) = (void (*)(REALarray, long, char))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt64( REALarray arr, long index, RBUInt64 value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, RBUInt64 ) = (void (*)(REALarray, long, RBUInt64))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt32( REALarray arr, long index, unsigned long value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, unsigned long ) = (void (*)(REALarray, long, unsigned long ))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt16( REALarray arr, long index, unsigned short value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, unsigned short ) = (void (*)(REALarray, long, unsigned short))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt8( REALarray arr, long index, unsigned char value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, unsigned char ) = (void (*)(REALarray, long, unsigned char))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueSingle( REALarray arr, long index, float value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, float ) = (void (*)(REALarray, long, float))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueDouble( REALarray arr, long index, double value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, double ) = (void (*)(REALarray, long, double))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueString( REALarray arr, long index, REALstring value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, REALstring ) = (void (*)(REALarray, long, REALstring))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueObject( REALarray arr, long index, REALobject value )
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetInsertProc" );
	if (pInsertArrayValueProc)	{
		void (*fp)( REALarray, long, REALobject ) = (void (*)(REALarray, long, REALobject))pInsertArrayValueProc( arr );
		if (fp)	fp( arr, index, value );
	}
}

void REALGetArrayValueInt64( REALarray arr, long index, RBInt64 *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		RBInt64 (*fp)( REALarray, long ) = (RBInt64 (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueInt32( REALarray arr, long index, long *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		long (*fp)( REALarray, long ) = (long (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueInt16( REALarray arr, long index, short *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		short (*fp)( REALarray, long ) = (short (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueInt8( REALarray arr, long index, char *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		char (*fp)( REALarray, long ) = (char (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt64( REALarray arr, long index, RBUInt64 *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		RBUInt64 (*fp)( REALarray, long ) = (RBUInt64 (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt32( REALarray arr, long index, unsigned long *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		unsigned long (*fp)( REALarray, long ) = (unsigned long (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt16( REALarray arr, long index, unsigned short *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		unsigned short (*fp)( REALarray, long ) = (unsigned short (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt8( REALarray arr, long index, unsigned char *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		unsigned char (*fp)( REALarray, long ) = (unsigned char (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueSingle( REALarray arr, long index, float *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		float (*fp)( REALarray, long ) = (float (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueDouble( REALarray arr, long index, double *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		double (*fp)( REALarray, long ) = (double (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueString( REALarray arr, long index, REALstring *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		REALstring (*fp)( REALarray, long ) = (REALstring (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueObject( REALarray arr, long index, REALobject *value )
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetGetProc" );
	if (pGetArrayValueProc)	{
		REALobject (*fp)( REALarray, long ) = (REALobject (*)(REALarray, long))pGetArrayValueProc( arr );
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALSetArrayValueInt64( REALarray arr, long index, RBInt64 value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, RBInt64, long ) = (void (*)(REALarray, RBInt64, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueInt32( REALarray arr, long index, long value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, long, long ) = (void (*)(REALarray, long, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueInt16( REALarray arr, long index, short value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, short, long ) = (void (*)(REALarray, short, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueInt8( REALarray arr, long index, char value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, char, long ) = (void (*)(REALarray, char, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt64( REALarray arr, long index, RBUInt64 value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, RBUInt64, long ) = (void (*)(REALarray, RBUInt64, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt32( REALarray arr, long index, unsigned long value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, unsigned long, long ) = (void (*)(REALarray, unsigned long, long ))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt16( REALarray arr, long index, unsigned short value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, unsigned short, long ) = (void (*)(REALarray, unsigned short, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt8( REALarray arr, long index, unsigned char value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, unsigned char, long ) = (void (*)(REALarray, unsigned char, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueSingle( REALarray arr, long index, float value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, float, long ) = (void (*)(REALarray, float, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueDouble( REALarray arr, long index, double value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, double, long ) = (void (*)(REALarray, double, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueString( REALarray arr, long index, REALstring value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, REALstring, long ) = (void (*)(REALarray, REALstring, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueObject( REALarray arr, long index, REALobject value )
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray )) CallResolver( "RuntimeArrayDirectGetSetProc" );
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, REALobject, long ) = (void (*)(REALarray, REALobject, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

REALarray REALCreateArray( REALArrayType type, long bounds )
{
	static REALarray (*pCreateArray)( REALArrayType, int ) = nil;
	if (!pCreateArray)
		pCreateArray = (REALarray (*)( REALArrayType, int )) CallResolver( "RuntimeArrayCreateSingleDimBoundedArray" );
	if (pCreateArray)	return pCreateArray( type, bounds );

	return nil;
}

unsigned long REALGetStringEncoding(REALstring str)
{
	static unsigned long (*pGetStringEncoding)(REALstring) = nil;
	if (!pGetStringEncoding)
		pGetStringEncoding = (unsigned long (*)(REALstring)) CallResolver("REALGetStringEncoding");
	if (pGetStringEncoding) return pGetStringEncoding(str);
	else return (unsigned long)0;
}

REALstring REALConvertString(REALstring str, unsigned long encoding)
{
	static REALstring (*pConvertString)(REALstring, unsigned long) = nil;
	if (!pConvertString)
		pConvertString = (REALstring (*)(REALstring, unsigned long)) CallResolver("REALConvertString");
	if (pConvertString) return pConvertString(str, encoding);
	else return (REALstring)0;
}

void REALSetStringEncoding(REALstring str, unsigned long encoding)
{
	static void (*pSetStringEncoding)(REALstring, unsigned long) = nil;
	if (!pSetStringEncoding)
		pSetStringEncoding = (void (*)(REALstring, unsigned long)) CallResolver("REALSetStringEncoding");
	if (pSetStringEncoding) pSetStringEncoding(str, encoding);
}

unsigned long REALstringToOSType(REALstring id)
{
	static unsigned long (*fpStringToOSType)( REALstring ) = nil;
	if (!fpStringToOSType)
		fpStringToOSType = (unsigned long (*)( REALstring ))CallResolver( "StringToOSType" );
	if (fpStringToOSType) return fpStringToOSType( id );
	return 0L;
}

inline unsigned long REALWin32CodePageToEncoding( unsigned long codePage )
{
	return 0xFFFF0000 | codePage;
}

// Main program
void REALPluginMain(void *(*resolver)(const char *entryName))
{
	void (*pRegisterPluginVersion)(int version);
	
	assert( !gResolver && "Plugin loaded twice?" );
	gResolver = resolver;

	pRegisterPluginVersion = (void (*)(int)) CallResolver("RegisterPluginVersion");
	pRegisterPluginVersion(kCurrentREALControlVersion);

	PluginEntry();
}

/****************** DEPRECATED FUNCTIONALITY **********************/
// Anything below this point is to be considered deprecated 
// functionality, and should be avoided except in extreme cases.
#if _MSC_VER
	#pragma warning( disable:4995 )
#endif

#if !FLAT_C_PLUGIN_HEADERS
	#define REALGetControlGraphicsWithDC REALGetControlGraphics
#endif

void *REALLoadGlobalMethod( const char *module, const char *prototype )
{
	// Call through to the real worker function
	return REALLoadFrameworkMethod( prototype );
}

REALstring REALInterpretConstantValue(REALstring value)
{
	static REALstring (*pInterpretConstantValue)(REALstring);
	if (!pInterpretConstantValue)
		pInterpretConstantValue = (REALstring (*)(REALstring)) CallResolver("PluginInterpretConstantValue");
	if (!pInterpretConstantValue)
	{
		REALLockString(value);
		return value;
	}
	return pInterpretConstantValue(value);
}

REALstring REALDefaultControlCaption(void)
{
	static unsigned long (*pDefaultControlCaption)(void) = nil;
	if (!pDefaultControlCaption)
		pDefaultControlCaption = (unsigned long (*)(void)) CallResolver("PluginDefaultControlCaption");
	if (pDefaultControlCaption) return (REALstring)pDefaultControlCaption();
	else return (REALstring)0;
}

REALobject REALnewInstance(const char *className)
{
	static unsigned long (*pNewInstance)(const char *) = nil;
	if (!pNewInstance)
		pNewInstance = (unsigned long (*)(const char *)) CallResolver("PluginNewInstance");
	if (pNewInstance) return (REALobject)pNewInstance(className);
	else return (REALobject)0;
}

void REALRegisterStructure(REALstructure *defn)
{
	static void (*pRuntimeRegisterMethod)(REALstructure *defn);
	if (!pRuntimeRegisterMethod)
		pRuntimeRegisterMethod = (void (*)(REALstructure *)) CallResolver("PluginRegisterStructure");
	if (pRuntimeRegisterMethod) pRuntimeRegisterMethod(defn);
}

void REALRegisterEnum(REALenum *defn)
{
	static void (*pRuntimeRegisterMethod)(REALenum *defn);
	if (!pRuntimeRegisterMethod)
		pRuntimeRegisterMethod = (void (*)(REALenum *)) CallResolver("PluginRegisterEnum");
	if (pRuntimeRegisterMethod) pRuntimeRegisterMethod(defn);
}

#if TARGET_CARBON && !TARGET_COCOA
int REALallocateMenuID(void)
{
	static int (*pAllocateMenuID)(void) = nil;
	if (!pAllocateMenuID)
		pAllocateMenuID = (int (*)(void)) CallResolver("allocateMenuID");
	if (pAllocateMenuID) return pAllocateMenuID();
	else return (int)0;
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
void REALreleaseMenuID(int id)
{
	static void (*pReleaseMenuID)(int) = nil;
	if (!pReleaseMenuID)
		pReleaseMenuID = (void (*)(int)) CallResolver("releaseMenuID");
	if (pReleaseMenuID) pReleaseMenuID(id);
}
#endif

void GraphicsDrawLine(REALgraphics graphicsObject, int x1, int y1, int x2, int y2)
{
	static void (*pGraphicsDrawLine)(REALgraphics, int, int, int, int) = nil;
	if (!pGraphicsDrawLine)
		pGraphicsDrawLine = (void (*)(REALgraphics, int, int, int, int)) CallResolver("RuntimeGraphicsDrawLine");
	if (pGraphicsDrawLine) pGraphicsDrawLine(graphicsObject, x1, y1, x2, y2);
}

void REALRegisterMethod(REALmethodDefinition *defn)
{
	static void (*pRuntimeRegisterMethod)(REALmethodDefinition *defn);
	if (!pRuntimeRegisterMethod)
		pRuntimeRegisterMethod = (void (*)(REALmethodDefinition *)) CallResolver("PluginRegisterMethod");
	if (pRuntimeRegisterMethod) pRuntimeRegisterMethod(defn);
}

void REALRegisterClassExtension(REALclassDefinition *defn)
{
	static void (*pRegisterClassExtension)(REALclassDefinition *defn);
	if (!pRegisterClassExtension)
		pRegisterClassExtension = (void (*)(REALclassDefinition *)) CallResolver("PluginRegisterClassExtension");
	
	if (pRegisterClassExtension)
		pRegisterClassExtension(defn);
}

const char *REALCString(REALstring str)
{
	static unsigned long (*pCString)(REALstring) = nil;
	if (!pCString)
		pCString = (unsigned long (*)(REALstring)) CallResolver("StringGetCString");
	if (pCString) return (const char *)pCString(str);
	else return (const char *)0;
}

const unsigned char *REALPString(REALstring str)
{
	static unsigned long (*pPString)(REALstring) = nil;
	if (!pPString)
		pPString = (unsigned long (*)(REALstring)) CallResolver("StringGetPString");
	if (pPString) return (const unsigned char *)pPString(str);
	else return (const unsigned char *)0;
}

REALproc REALInterfaceRoutine(REALobject obj, const char *interfaceName, const char *methodName)
{
	static unsigned long (*pInterfaceRoutine)(REALobject, const char *, const char *) = nil;
	if (!pInterfaceRoutine)
		pInterfaceRoutine = (unsigned long (*)(REALobject, const char *, const char *)) CallResolver("GetInterfaceRoutine");
	if (pInterfaceRoutine) return (REALproc)pInterfaceRoutine(obj, interfaceName, methodName);
	else return (REALproc)0;
}

void REALPictureClearCache(REALpicture pic)
{
	static void (*pPictureClearCache)(REALpicture) = nil;
	if (!pPictureClearCache)
		pPictureClearCache = (void (*)(REALpicture)) CallResolver("REALPictureClearCache");
	if (pPictureClearCache) pPictureClearCache(pic);
}

#if TARGET_WIN32
void REALDrawPicturePrimitive(HDC hDC, REALpicture pic, const Rect *rBounds, int bTransparent)
{
	static void (*pDrawPicturePrimitive)(HDC, REALpicture, const Rect *, int) = nil;
	if (!pDrawPicturePrimitive)
		pDrawPicturePrimitive = (void (*)(HDC, REALpicture, const Rect *, int)) CallResolver("drawPicturePrimitive");
	if (pDrawPicturePrimitive) pDrawPicturePrimitive(hDC, pic, rBounds, bTransparent);
}
#endif

#if TARGET_CARBON || TARGET_COCOA
void REALDrawPicturePrimitive(REALpicture pic, const Rect *rBounds, int bTransparent)
{
	static void (*pDrawPicturePrimitive)(REALpicture, const Rect *, int) = nil;
	if (!pDrawPicturePrimitive)
		pDrawPicturePrimitive = (void (*)(REALpicture, const Rect *, int)) CallResolver("drawPicturePrimitive");
	if (pDrawPicturePrimitive) pDrawPicturePrimitive(pic, rBounds, bTransparent);
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
void REALRegisterEventFilter(REALEventCallback callback, long param)
{
	static void (*pRegisterEventFilter)(REALEventCallback, long param);
	if (!pRegisterEventFilter)
		pRegisterEventFilter = (void (*)(REALEventCallback, long)) CallResolver("PluginRegisterEventFilter");
	if (pRegisterEventFilter) pRegisterEventFilter(callback, param);
}
#endif

void REALGetControlBounds(REALcontrolInstance instance, Rect *rBounds)
{
	static void (*pGetControlBounds)(REALcontrolInstance, Rect *) = nil;
	if (!pGetControlBounds)
		pGetControlBounds = (void (*)(REALcontrolInstance, Rect *)) CallResolver("GetControlBounds");
	if (pGetControlBounds) pGetControlBounds(instance, rBounds);
}

long REALGetControlVisible(REALcontrolInstance instance)
{
	static long (*pGetControlVisible)(REALcontrolInstance) = nil;
	if (!pGetControlVisible)
		pGetControlVisible = (long (*)(REALcontrolInstance)) CallResolver("GetControlVisible");
	if (pGetControlVisible) return pGetControlVisible(instance);
	else return (long)0;
}

Boolean REALGetControlEnabled(REALcontrolInstance instance)
{
	static Boolean (*pGetControlEnabled)(REALcontrolInstance) = nil;
	if (!pGetControlEnabled)
		pGetControlEnabled = (Boolean (*)(REALcontrolInstance)) CallResolver("REALGetControlEnabled");
	if (pGetControlEnabled) return pGetControlEnabled(instance);
	else return (Boolean)0;
}

void REALSetControlVisible(REALcontrolInstance instance, unsigned long visible)
{
	static void (*pSetControlVisible)(REALcontrolInstance, unsigned long) = nil;
	if (!pSetControlVisible)
		pSetControlVisible = (void (*)(REALcontrolInstance, unsigned long)) CallResolver("SetControlVisible");
	if (pSetControlVisible) pSetControlVisible(instance, visible);
}

REALgraphics REALGetControlGraphics(REALcontrolInstance instance)
{
	static unsigned long (*pGetControlGraphics)(REALcontrolInstance) = nil;
	if (!pGetControlGraphics)
		pGetControlGraphics = (unsigned long (*)(REALcontrolInstance)) CallResolver("REALGetControlGraphics");
	if (pGetControlGraphics) return (REALgraphics)pGetControlGraphics(instance);
	else return (REALgraphics)0;
}

#if TARGET_WIN32
int REALGetWin32Charset(void)
{
	static int (*pGetWin32Charset)(void) = nil;
	if (!pGetWin32Charset)
		pGetWin32Charset = (int (*)(void)) CallResolver("REALGetWin32Charset");
	if (pGetWin32Charset) return pGetWin32Charset();
	else return (int)0;
}
#endif

#if TARGET_WIN32 || UNIX_ANSI
REALfolderItem REALFolderItemFromPath(const char *path)
{
	static unsigned long (*pFolderItemFromPath)(const char *) = nil;
	if (!pFolderItemFromPath)
		pFolderItemFromPath = (unsigned long (*)(const char *)) CallResolver("FolderItemFromPath");
	if (pFolderItemFromPath) return (REALfolderItem)pFolderItemFromPath(path);
	else return (REALfolderItem)0;
}
#endif

REALstring REALpathFromFolderItem(REALfolderItem item)
{
	static unsigned long (*pPathFromFolderItem)(REALfolderItem) = nil;
	if (!pPathFromFolderItem)
		pPathFromFolderItem = (unsigned long (*)(REALfolderItem)) CallResolver("REALpathFromFolderItem");
	if (pPathFromFolderItem) return (REALstring)pPathFromFolderItem(item);
	else return (REALstring)0;
}

#if TARGET_WIN32
HDC REALGraphicsDC(REALgraphics context)
{
	static unsigned long (*pGraphicsDC)(REALgraphics) = nil;
	if (!pGraphicsDC)
		pGraphicsDC = (unsigned long (*)(REALgraphics)) CallResolver("REALGraphicsDC");
	if (pGraphicsDC) return (HDC)pGraphicsDC(context);
	else return (HDC)0;
}
#endif

#if TARGET_COCOA
CGContextRef REALGraphicsDC(REALgraphics context)
{
	static unsigned long (*pGraphicsCGContext)(REALgraphics) = nil;
	if (!pGraphicsCGContext)
		pGraphicsCGContext = (unsigned long (*)(REALgraphics)) CallResolver("REALGraphicsDC");
	if (pGraphicsCGContext) return (CGContextRef)pGraphicsCGContext(context);
	else return (CGContextRef)0;
}
#endif

#if TARGET_WIN32
HWND REALGetControlHWND(REALcontrolInstance control)
{
	static HWND (*pGetControlHWND)(REALcontrolInstance) = nil;
	if (!pGetControlHWND)
		pGetControlHWND = (HWND (*)(REALcontrolInstance)) CallResolver("GetControlHWND");
	if (pGetControlHWND) return pGetControlHWND(control);
	else return (HWND)0;
}
#endif

void REALInvalidateControl(REALcontrolInstance instance)
{
	static void (*pInvalidateControl)(REALcontrolInstance) = nil;
	if (!pInvalidateControl)
		pInvalidateControl = (void (*)(REALcontrolInstance)) CallResolver("REALInvalidateControl");
	if (pInvalidateControl) pInvalidateControl(instance);
}

void REALInvalidateControlRect(REALcontrolInstance instance, int left, int top, int right, int bottom)
{
	static void (*pInvalidateControlRect)(REALcontrolInstance, int, int, int, int) = nil;
	if (!pInvalidateControlRect)
		pInvalidateControlRect = (void (*)(REALcontrolInstance, int, int, int, int)) CallResolver("REALInvalidateControlRect");
	if (pInvalidateControlRect) pInvalidateControlRect(instance, left, top, right, bottom);
}

#if TARGET_WIN32
void REALSetSpecialBackground(REALcontrolInstance instance, COLORREF *pcolor)
{
	static void (*pSetSpecialBackground)(REALcontrolInstance, COLORREF *) = nil;
	if (!pSetSpecialBackground)
		pSetSpecialBackground = (void (*)(REALcontrolInstance, COLORREF *)) CallResolver("REALSetSpecialBackground");
	if (pSetSpecialBackground) pSetSpecialBackground(instance, pcolor);
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
void REALSetSpecialBackground(REALcontrolInstance instance)
{
	static void (*pSetSpecialBackground)(REALcontrolInstance) = nil;
	if (!pSetSpecialBackground)
		pSetSpecialBackground = (void (*)(REALcontrolInstance)) CallResolver("REALSetSpecialBackground");
	if (pSetSpecialBackground) pSetSpecialBackground(instance);
}
#endif

REALwindow REALGetControlWindow(REALcontrolInstance instance)
{
	static REALwindow (*pGetControlWindow)(REALcontrolInstance) = nil;
	if (!pGetControlWindow)
		pGetControlWindow = (REALwindow (*)(REALcontrolInstance)) CallResolver("getControlWindow");
	if (pGetControlWindow) return pGetControlWindow(instance);
	else return (REALwindow)0;
}

#if TARGET_COCOA
NSWindow *REALGetWindowHandle(REALwindow window)
{
	static unsigned long (*pGetWindowHandle)(REALwindow) = nil;
	if (!pGetWindowHandle)
		pGetWindowHandle = (unsigned long (*)(REALwindow)) CallResolver("REALGetWindowHandle");
	if (pGetWindowHandle) return (NSWindow *)pGetWindowHandle(window);
	else return (NSWindow *)0;
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
WindowPtr REALGetWindowHandle(REALwindow window)
{
	static unsigned long (*pGetWindowHandle)(REALwindow) = nil;
	if (!pGetWindowHandle)
		pGetWindowHandle = (unsigned long (*)(REALwindow)) CallResolver("REALGetWindowHandle");
	if (pGetWindowHandle) return (WindowPtr)pGetWindowHandle(window);
	else return (WindowPtr)0;
}
#endif

#if TARGET_COCOA
NSView *REALGetControlHandle(REALcontrolInstance control)
{
	static unsigned long (*pGetControlHandle)(REALcontrolInstance) = nil;
	if (!pGetControlHandle)
		pGetControlHandle = (unsigned long (*)(REALcontrolInstance)) CallResolver("REALGetControlHandle");
	if (pGetControlHandle) return (NSView *)pGetControlHandle(control);
	else return (NSView *)0;
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
ControlHandle REALGetControlHandle(REALcontrolInstance control)
{
	static unsigned long (*pGetControlHandle)(REALcontrolInstance) = nil;
	if (!pGetControlHandle)
		pGetControlHandle = (unsigned long (*)(REALcontrolInstance)) CallResolver("REALGetControlHandle");
	if (pGetControlHandle) return (ControlHandle)pGetControlHandle(control);
	else return (ControlHandle)0;
}
#endif

#if TARGET_CARBON && !TARGET_COCOA
MenuHandle REALGetPopupMenuHandle(REALpopupMenu popup)
{
	static unsigned long (*pGetPopupMenuHandle)(REALpopupMenu) = nil;
	if (!pGetPopupMenuHandle)
		pGetPopupMenuHandle = (unsigned long (*)(REALpopupMenu)) CallResolver("REALGetPopupMenuHandle");
	if (pGetPopupMenuHandle) return (MenuHandle)pGetPopupMenuHandle(popup);
	else return (MenuHandle)0;
}
#endif

#if (TARGET_CARBON || TARGET_WIN32 || TARGET_COCOA) && !TARGET_64BIT
QT_NAMESPACE MovieController REALgetMoviePlayerController(REALmoviePlayer instance)
{
	static unsigned long (*pGetMoviePlayerController)(REALmoviePlayer) = nil;
	if (!pGetMoviePlayerController)
		pGetMoviePlayerController = (unsigned long (*)(REALmoviePlayer)) CallResolver("getMoviePlayerController");
	if (pGetMoviePlayerController) return (QT_NAMESPACE MovieController)pGetMoviePlayerController(instance);
	else return (QT_NAMESPACE MovieController)0;
}
#endif

#if (TARGET_CARBON || TARGET_WIN32 || TARGET_COCOA) && !TARGET_64BIT
QT_NAMESPACE Movie REALgetMovieMovie(REALmovie instance)
{
	static unsigned long (*pGetMovieMovie)(REALmovie) = nil;
	if (!pGetMovieMovie)
		pGetMovieMovie = (unsigned long (*)(REALmovie)) CallResolver("getMovieMovie");
	if (pGetMovieMovie) return (QT_NAMESPACE Movie)pGetMovieMovie(instance);
	else return (QT_NAMESPACE Movie)0;
}
#endif

void REALMarkSocketUsage(void)
{
	static void (*pMarkSocketUsage)(void) = nil;
	if (!pMarkSocketUsage)
		pMarkSocketUsage = (void (*)(void)) CallResolver("PluginMarkSocketUsage");
	if (pMarkSocketUsage) pMarkSocketUsage();
}

void REALSocketConnect(REALsocket socket, REALstring address, int port)
{
	static void (*pSocketConnect)(REALsocket, REALstring, int) = nil;
	if (!pSocketConnect)
		pSocketConnect = (void (*)(REALsocket, REALstring, int)) CallResolver("socketDirectConnect");
	if (pSocketConnect) pSocketConnect(socket, address, port);
}

void REALSocketClose(REALsocket socket)
{
	static void (*pSocketClose)(REALsocket) = nil;
	if (!pSocketClose)
		pSocketClose = (void (*)(REALsocket)) CallResolver("socketClose");
	if (pSocketClose) pSocketClose(socket);
}

REALstring REALSocketReadAll(REALsocket socket)
{
	static unsigned long (*pSocketReadAll)(REALsocket) = nil;
	if (!pSocketReadAll)
		pSocketReadAll = (unsigned long (*)(REALsocket)) CallResolver("pluginSocketReadAll");
	if (pSocketReadAll) return (REALstring)pSocketReadAll(socket);
	else return (REALstring)0;
}

REALstring REALSocketRead(REALsocket socket, int count)
{
	static unsigned long (*pSocketRead)(REALsocket, int) = nil;
	if (!pSocketRead)
		pSocketRead = (unsigned long (*)(REALsocket, int)) CallResolver("pluginSocketRead");
	if (pSocketRead) return (REALstring)pSocketRead(socket, count);
	else return (REALstring)0;
}

void REALSocketWrite(REALsocket socket, REALstring data)
{
	static void (*pSocketWrite)(REALsocket, REALstring) = nil;
	if (!pSocketWrite)
		pSocketWrite = (void (*)(REALsocket, REALstring)) CallResolver("SocketWrite");
	if (pSocketWrite) pSocketWrite(socket, data);
}

int REALSocketLastErrorCode(REALsocket socket, int unused)
{
	static int (*pSocketLastErrorCode)(REALsocket, int) = nil;
	if (!pSocketLastErrorCode)
		pSocketLastErrorCode = (int (*)(REALsocket, int)) CallResolver("socketLastErrorCode");
	if (pSocketLastErrorCode) return pSocketLastErrorCode(socket, unused);
	else return (int)0;
}

REALstring REALSocketLookahead(REALsocket socket, int unused)
{
	static unsigned long (*pSocketLookahead)(REALsocket, int) = nil;
	if (!pSocketLookahead)
		pSocketLookahead = (unsigned long (*)(REALsocket, int)) CallResolver("pluginSocketLookahead");
	if (pSocketLookahead) return (REALstring)pSocketLookahead(socket, unused);
	else return (REALstring)0;
}

REALstring REALSocketLocalAddressGetter(REALsocket socket)
{
	static REALstring (*pSocketLocalAddressGetter)(REALsocket) = nil;
	if (!pSocketLocalAddressGetter)
		pSocketLocalAddressGetter = (REALstring (*)(REALsocket)) CallResolver("socketLocalAddressGetter");
	if (pSocketLocalAddressGetter) return pSocketLocalAddressGetter(socket);
	else return (REALstring)0;
}

void REALSocketPoll(REALsocket socket)
{
	static void (*pSocketPoll)(REALsocket) = nil;
	if (!pSocketPoll)
		pSocketPoll = (void (*)(REALsocket)) CallResolver("socketPoll");
	if (pSocketPoll) pSocketPoll(socket);
}

int REALSocketGetEvents(REALsocket socket, int unused)
{
	static int (*pSocketGetEvents)(REALsocket, int) = nil;
	if (!pSocketGetEvents)
		pSocketGetEvents = (int (*)(REALsocket, int)) CallResolver("socketGetEvents");
	if (pSocketGetEvents) return pSocketGetEvents(socket, unused);
	else return (int)0;
}

void REALMessageBox(REALstring text)
{
	static void (*pMessageBox)(REALstring) = nil;
	if (!pMessageBox)
		pMessageBox = (void (*)(REALstring)) CallResolver("RuntimeMsgBox");
	if (pMessageBox) pMessageBox(text);
}

void REALSetControlEnabled(REALcontrolInstance instance, long unused, Boolean enable)
{
	static void (*pSetControlEnabled)(REALcontrolInstance, long, Boolean) = nil;
	if (!pSetControlEnabled)
		pSetControlEnabled = (void (*)(REALcontrolInstance, long, Boolean)) CallResolver("controlEnabledSetter");
	if (pSetControlEnabled) pSetControlEnabled(instance, unused, enable);
}

long REALGetControlPosition(REALcontrolInstance instance, long which)
{
	static unsigned long (*pGetControlPosition)(REALcontrolInstance, long) = nil;
	if (!pGetControlPosition)
		pGetControlPosition = (unsigned long (*)(REALcontrolInstance, long)) CallResolver("ctlPosGetter");
	if (pGetControlPosition) return (long)pGetControlPosition(instance, which);
	else return (long)0;
}

void REALSetControlPosition(REALcontrolInstance instance, long which, long value)
{
	static void (*pSetControlPosition)(REALcontrolInstance, long, long) = nil;
	if (!pSetControlPosition)
		pSetControlPosition = (void (*)(REALcontrolInstance, long, long)) CallResolver("ctlPosSetter");
	if (pSetControlPosition) pSetControlPosition(instance, which, value);
}

#if TARGET_CARBON && !TARGET_COCOA
void REALRefreshWindow(unsigned long macWindowPtr)
{
	static void (*pRefreshWindow)(unsigned long) = nil;
	if (!pRefreshWindow)
		pRefreshWindow = (void (*)(unsigned long)) CallResolver("REALRefreshWindow");
	if (pRefreshWindow) pRefreshWindow(macWindowPtr);
}
#endif

REALgraphics REALGetPictureGraphics(REALpicture picture)
{
	static unsigned long (*pGetPictureGraphics)(REALpicture) = nil;
	if (!pGetPictureGraphics)
		pGetPictureGraphics = (unsigned long (*)(REALpicture)) CallResolver("pluginPictureGraphicsGetter");
	if (pGetPictureGraphics) return (REALgraphics)pGetPictureGraphics(picture);
	else return (REALgraphics)0;
}

REALpicture REALNewPicture(long width, long height, long depth)
{
	static unsigned long (*pNewPicture)(long, long, long) = nil;
	if (!pNewPicture)
		pNewPicture = (unsigned long (*)(long, long, long)) CallResolver("newPicture");
	if (pNewPicture) return (REALpicture)pNewPicture(width, height, depth);
	else return (REALpicture)0;
}

REALmemoryBlock REALNewMemoryBlock(int bytes)
{
	static unsigned long (*pNewMemoryBlock)(int) = nil;
	if (!pNewMemoryBlock)
		pNewMemoryBlock = (unsigned long (*)(int)) CallResolver("newMemoryBlock");
	if (pNewMemoryBlock) return (REALmemoryBlock)pNewMemoryBlock(bytes);
	else return (REALmemoryBlock)0;
}

void*REALMemoryBlockGetPtr(REALmemoryBlock memBlock)
{
	static unsigned long (*pMemoryBlockGetPtr)(REALmemoryBlock) = nil;
	if (!pMemoryBlockGetPtr)
		pMemoryBlockGetPtr = (unsigned long (*)(REALmemoryBlock)) CallResolver("memoryBlockGetPtr");
	if (pMemoryBlockGetPtr) return (void*)pMemoryBlockGetPtr(memBlock);
	else return (void*)0;
}

int REALMemoryBlockGetSize(REALmemoryBlock memBlock)
{
	static unsigned long (*pMemoryBlockGetSize)(REALmemoryBlock) = nil;
	if (!pMemoryBlockGetSize)
		pMemoryBlockGetSize = (unsigned long (*)(REALmemoryBlock)) CallResolver("memoryBlockGetSize");
	if (pMemoryBlockGetSize) return (int)pMemoryBlockGetSize(memBlock);
	else return (int)0;
}

REALmemoryBlock REALPtrToMemoryBlock(void*data)
{
	static unsigned long (*pPtrToMemoryBlock)(void*) = nil;
	if (!pPtrToMemoryBlock)
		pPtrToMemoryBlock = (unsigned long (*)(void*)) CallResolver("PtrToMemoryBlock");
	if (pPtrToMemoryBlock) return (REALmemoryBlock)pPtrToMemoryBlock(data);
	else return (REALmemoryBlock)0;
}

#if TARGET_WIN32 || X_WINDOW
void REALSetAccelerator(REALcontrolInstance instance, REALstring key)
{
	static void (*pSetAccelerator)(REALcontrolInstance, REALstring) = nil;
	if (!pSetAccelerator)
		pSetAccelerator = (void (*)(REALcontrolInstance, REALstring)) CallResolver("REALSetAccelerator");
	if (pSetAccelerator) pSetAccelerator(instance, key);
}
#endif

int REALGetArrayInt(REALintArray array, int index)
{
	static int (*pGetArrayInt)(REALintArray, int) = nil;
	if (!pGetArrayInt)
		pGetArrayInt = (int (*)(REALintArray, int)) CallResolver("RuntimeDirectReadIntArray");
	if (pGetArrayInt) return pGetArrayInt(array, index);
	else return (int)0;
}

REALstring REALGetArrayString(REALstringArray array, int index)
{
	static REALstring (*pGetArrayString)(REALstringArray, int) = nil;
	if (!pGetArrayString)
		pGetArrayString = (REALstring (*)(REALstringArray, int)) CallResolver("RuntimeDirectReadStringArray");
	if (pGetArrayString) return pGetArrayString(array, index);
	else return (REALstring)0;
}

REALobject REALGetArrayObject(REALobjectArray array, int index)
{
	static REALobject (*pGetArrayObject)(REALobjectArray, int) = nil;
	if (!pGetArrayObject)
		pGetArrayObject = (REALobject (*)(REALobjectArray, int)) CallResolver("RuntimeDirectReadObjectArray");
	if (pGetArrayObject) return pGetArrayObject(array, index);
	else return (REALobject)0;
}

long REALGetTabPanelVisible(REALcontrolInstance instance)
{
	static long (*pGetTabPanelVisible)(REALcontrolInstance) = nil;
	if (!pGetTabPanelVisible)
		pGetTabPanelVisible = (long (*)(REALcontrolInstance)) CallResolver("GetTabPanelVisible");
	if (pGetTabPanelVisible) return pGetTabPanelVisible(instance);
	else return (long)0;
}

#if TARGET_WIN32
HWND REALGetControlHandle(REALcontrolInstance control)
{
	static unsigned long (*pGetControlHandle)(REALcontrolInstance) = nil;
	if (!pGetControlHandle)
		pGetControlHandle = (unsigned long (*)(REALcontrolInstance)) CallResolver("REALGetControlHandle");
	if (pGetControlHandle) return (HWND)pGetControlHandle(control);
	else return (HWND)0;
}
#endif

#if TARGET_WIN32
HWND REALGetWindowHandle(REALwindow window)
{
	static unsigned long (*pGetWindowHandle)(REALwindow) = nil;
	if (!pGetWindowHandle)
		pGetWindowHandle = (unsigned long (*)(REALwindow)) CallResolver("REALGetWindowHandle");
	if (pGetWindowHandle) return (HWND)pGetWindowHandle(window);
	else return (HWND)0;
}
#endif

Boolean REALGetControlFocus(REALcontrolInstance instance)
{
	static unsigned long (*pGetControlFocus)(REALcontrolInstance) = nil;
	if (!pGetControlFocus)
		pGetControlFocus = (unsigned long (*)(REALcontrolInstance)) CallResolver("REALGetControlFocus");
	if (pGetControlFocus) return (Boolean)pGetControlFocus(instance);
	else return (Boolean)0;
}

void REALSetControlFocus(REALcontrolInstance instance, Boolean focus)
{
	static void (*pSetControlFocus)(REALcontrolInstance, Boolean) = nil;
	if (!pSetControlFocus)
		pSetControlFocus = (void (*)(REALcontrolInstance, Boolean)) CallResolver("REALSetControlFocus");
	if (pSetControlFocus) pSetControlFocus(instance, focus);
}

REALcontrolInstance REALGetControlParent(REALcontrolInstance instance)
{
	static REALcontrolInstance (*pGetControlParent)(REALcontrolInstance) = nil;
	if (!pGetControlParent)
		pGetControlParent = (REALcontrolInstance (*)(REALcontrolInstance)) CallResolver("REALGetControlParent");
	if (pGetControlParent) return pGetControlParent(instance);
	else return (REALcontrolInstance)0;
}

#if (TARGET_CARBON || TARGET_WIN32 || TARGET_COCOA) && !TARGET_64BIT
void REALSetMovieMovie(REALmovie obj, Movie movie)
{
	static void (*pSetMovieMovie)(REALmovie, Movie) = nil;
	if (!pSetMovieMovie)
		pSetMovieMovie = (void (*)(REALmovie, Movie)) CallResolver("REALSetMovieMovie");
	if (pSetMovieMovie) pSetMovieMovie(obj, movie);
}
#endif

REALstring REALGetControlName(REALcontrolInstance control)
{
	static REALstring (*pGetControlName)(REALcontrolInstance) = nil;
	if (!pGetControlName)
		pGetControlName = (REALstring (*)(REALcontrolInstance)) CallResolver("REALGetControlName");
	if (pGetControlName) return pGetControlName(control);
	else return (REALstring)0;
}

Boolean REALIsHIViewWindow(REALwindow window)
{
	static Boolean (*pIsHIViewWindow)(REALwindow) = nil;
	if (!pIsHIViewWindow)
		pIsHIViewWindow = (Boolean (*)(REALwindow)) CallResolver("REALIsHIViewWindow");
	if (pIsHIViewWindow) return pIsHIViewWindow(window);
	else return (Boolean)0;
}

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
unsigned long REALGetFontEncoding(const char *fontName)
{
	static unsigned long (*pGetFontEncoding)(const char *) = nil;
	if (!pGetFontEncoding)
		pGetFontEncoding = (unsigned long (*)(const char *)) CallResolver("REALGetFontEncoding");
	if (pGetFontEncoding) return pGetFontEncoding(fontName);
	else return (unsigned long)0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REALpicture REALGetPictureMask(REALpicture pict, Boolean createIfNil)
{
	static REALpicture (*pGetPictureMask)(REALpicture, Boolean) = nil;
	if (!pGetPictureMask)
		pGetPictureMask = (REALpicture (*)(REALpicture, Boolean)) CallResolver("REALGetPictureMask");
	if (pGetPictureMask) return pGetPictureMask(pict, createIfNil);
	else return (REALpicture)0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
void REALGraphicsDrawString(REALgraphics graphics, REALstring str, long x, long y, long width)
{
	static void (*pGraphicsDrawString)(REALgraphics, REALstring, long, long, long) = nil;
	if (!pGraphicsDrawString)
		pGraphicsDrawString = (void (*)(REALgraphics, REALstring, long, long, long)) CallResolver("RuntimeGraphicsDrawString");
	if (pGraphicsDrawString) pGraphicsDrawString(graphics, str, x, y, width);
}
#endif

#if TARGET_CARBON || TARGET_COCOA
CFStringRef REALGetStringCFString(REALstring str, Boolean stripAmpersands)
{
	static CFStringRef (*pGetStringCFString)(REALstring, Boolean) = nil;
	if (!pGetStringCFString)
		pGetStringCFString = (CFStringRef (*)(REALstring, Boolean)) CallResolver("REALGetStringCFString");
	if (pGetStringCFString) return pGetStringCFString(str, stripAmpersands);
	else return (CFStringRef)0;
}
#endif

#if TARGET_CARBON || TARGET_COCOA
void REALGetStringSystemStr(REALstring str, Boolean stripAmpersands, Str255 outStr255)
{
	static void (*pGetStringSystemStr)(REALstring, Boolean, Str255) = nil;
	if (!pGetStringSystemStr)
		pGetStringSystemStr = (void (*)(REALstring, Boolean, Str255)) CallResolver("REALGetStringSystemStr");
	if (pGetStringSystemStr) pGetStringSystemStr(str, stripAmpersands, outStr255);
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
void REALGetGraphicsFontStyle(REALgraphics graphics, REALfontStyle*  outStyle)
{
	static void (*pGetGraphicsFontStyle)(REALgraphics, REALfontStyle* ) = nil;
	if (!pGetGraphicsFontStyle)
		pGetGraphicsFontStyle = (void (*)(REALgraphics, REALfontStyle* )) CallResolver("REALGetGraphicsFontStyle");
	if (pGetGraphicsFontStyle) pGetGraphicsFontStyle(graphics, outStyle);
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
void REALSetGraphicsStyle(REALgraphics graphics, REALfontStyle*  styleInfo)
{
	static void (*pSetGraphicsStyle)(REALgraphics, REALfontStyle* ) = nil;
	if (!pSetGraphicsStyle)
		pSetGraphicsStyle = (void (*)(REALgraphics, REALfontStyle* )) CallResolver("REALSetGraphicsStyle");
	if (pSetGraphicsStyle) pSetGraphicsStyle(graphics, styleInfo);
}
#endif

long REALGraphicsStringWidth(REALgraphics graphics, REALstring str)
{
	static int (*pGraphicsStringWidth)(REALgraphics, REALstring) = nil;
	if (!pGraphicsStringWidth)
		pGraphicsStringWidth = (int (*)(REALgraphics, REALstring)) CallResolver("RuntimeGraphicsStringWidth");
	if (pGraphicsStringWidth) return (long)pGraphicsStringWidth(graphics, str);
	else return (long)0;
}

long REALGraphicsStringHeight(REALgraphics graphics, REALstring str, long wrapWidth)
{
	static int (*pGraphicsStringHeight)(REALgraphics, REALstring, long) = nil;
	if (!pGraphicsStringHeight)
		pGraphicsStringHeight = (int (*)(REALgraphics, REALstring, long)) CallResolver("RuntimeGraphicsStringHeight");
	if (pGraphicsStringHeight) return (long)pGraphicsStringHeight(graphics, str, wrapWidth);
	else return (long)0;
}

long REALGraphicsTextHeight(REALgraphics graphics)
{
	static int (*pGraphicsTextHeight)(REALgraphics) = nil;
	if (!pGraphicsTextHeight)
		pGraphicsTextHeight = (int (*)(REALgraphics)) CallResolver("RuntimeGraphicsTextHeight");
	if (pGraphicsTextHeight) return (long)pGraphicsTextHeight(graphics);
	else return (long)0;
}

long REALGraphicsTextAscent(REALgraphics graphics)
{
	static int (*pGraphicsTextAscent)(REALgraphics) = nil;
	if (!pGraphicsTextAscent)
		pGraphicsTextAscent = (int (*)(REALgraphics)) CallResolver("RuntimeGraphicsTextAscent");
	if (pGraphicsTextAscent) return (long)pGraphicsTextAscent(graphics);
	else return (long)0;
}

#if TARGET_CARBON && !TARGET_COCOA
void REALReleasePopupMenuHandle(REALpopupMenu popup)
{
	static void (*pReleasePopupMenuHandle)(REALpopupMenu) = nil;
	if (!pReleasePopupMenuHandle)
		pReleasePopupMenuHandle = (void (*)(REALpopupMenu)) CallResolver("REALReleasePopupMenuHandle");
	if (pReleasePopupMenuHandle) pReleasePopupMenuHandle(popup);
}
#endif

void REALSocketListen(REALsocket socket)
{
	static void (*pSocketListen)(REALsocket) = nil;
	if (!pSocketListen)
		pSocketListen = (void (*)(REALsocket)) CallResolver("pluginSocketListen");
	if (pSocketListen) pSocketListen(socket);
}

#if X_WINDOW
unsigned long REALGetControlHandle(REALcontrolInstance control)
{
	static unsigned long (*pGetControlHandle)(REALcontrolInstance) = nil;
	if (!pGetControlHandle)
		pGetControlHandle = (unsigned long (*)(REALcontrolInstance)) CallResolver("REALGetControlHandle");
	if (pGetControlHandle) return pGetControlHandle(control);
	else return (unsigned long)0;
}
#endif

#if X_WINDOW
unsigned long REALGraphicsDC(REALgraphics context)
{
	static unsigned long (*pGraphicsDC)(REALgraphics) = nil;
	if (!pGraphicsDC)
		pGraphicsDC = (unsigned long (*)(REALgraphics)) CallResolver("REALGraphicsDC");
	if (pGraphicsDC) return pGraphicsDC(context);
	else return (unsigned long)0;
}
#endif

#if X_WINDOW
void REALDrawPicturePrimitive(REALgraphics context, REALpicture pic, const Rect *rBounds, int bTransparent)
{
	static void (*pDrawPicturePrimitive)(REALgraphics, REALpicture, const Rect *, int) = nil;
	if (!pDrawPicturePrimitive)
		pDrawPicturePrimitive = (void (*)(REALgraphics, REALpicture, const Rect *, int)) CallResolver("REALDrawPicturePrimitive");
	if (pDrawPicturePrimitive) pDrawPicturePrimitive(context, pic, rBounds, bTransparent);
}
#endif

#if X_WINDOW
void *REALGraphicsGdkDrawable(REALgraphics context)
{
	static void *(*pGraphicsGdkDrawable)(REALgraphics) = nil;
	if (!pGraphicsGdkDrawable)
		pGraphicsGdkDrawable = (void *(*)(REALgraphics)) CallResolver("REALGraphicsGdkDrawable");
	if (pGraphicsGdkDrawable) return pGraphicsGdkDrawable(context);
	else return (void *)0;
}
#endif

#if TARGET_WIN32
REALgraphics REALGetControlGraphics(REALcontrolInstance instance, HDC dc)
{
	static unsigned long (*pGetControlGraphics)(REALcontrolInstance, HDC) = nil;
	if (!pGetControlGraphics)
		pGetControlGraphics = (unsigned long (*)(REALcontrolInstance, HDC)) CallResolver("REALGetControlGraphicsWithDC");
	if (pGetControlGraphics) return (REALgraphics)pGetControlGraphics(instance, dc);
	else return (REALgraphics)0;
}
#endif

#if X_WINDOW
unsigned long REALGetWindowHandle(REALwindow window)
{
	static unsigned long (*pGetWindowHandle)(REALwindow) = nil;
	if (!pGetWindowHandle)
		pGetWindowHandle = (unsigned long (*)(REALwindow)) CallResolver("REALGetWindowHandle");
	if (pGetWindowHandle) return pGetWindowHandle(window);
	else return (unsigned long)0;
}
#endif

#if TARGET_COCOA || TARGET_CARBON
CGImageRef REALCopyPictureCGImage(REALpicture pic)
{
	static CGImageRef (*pCopyPictureCGImage)(REALpicture) = nil;
	if (!pCopyPictureCGImage)
		pCopyPictureCGImage = (CGImageRef (*)(REALpicture)) CallResolver("REALCopyPictureCGImage");
	if (pCopyPictureCGImage) return pCopyPictureCGImage(pic);
	else return NULL;
}
#endif

void REALGetGraphicsOrigin(REALgraphics context, long *originX, long *originY)
{
	static void (*pGetGraphicsOrigin)(REALgraphics, long *, long *) = nil;
	if (!pGetGraphicsOrigin)
		pGetGraphicsOrigin = (void (*)(REALgraphics, long *, long *)) CallResolver("REALGetGraphicsOrigin");
	if (pGetGraphicsOrigin) pGetGraphicsOrigin(context, originX, originY);
}

void REALSetGraphicsOrigin(REALgraphics context, long originX, long originY)
{
	static void (*pSetGraphicsOrigin)(REALgraphics, long, long) = nil;
	if (!pSetGraphicsOrigin)
		pSetGraphicsOrigin = (void (*)(REALgraphics, long, long)) CallResolver("REALSetGraphicsOrigin");
	if (pSetGraphicsOrigin) pSetGraphicsOrigin(context, originX, originY);
}

long REALstringStruct::Length(void)
{
	if (this) {
		static RBInteger (*pLenB)(REALstring);
		if (!pLenB)
			pLenB = (RBInteger (*)(REALstring))REALLoadFrameworkMethod( "LenB( s as string ) as integer" );
		
		if (pLenB) {
			return pLenB(this);
		} else {
			return 0;
		}
	} else
		return 0;
}

const char *REALstringStruct::CString()
{
	if (this) {
		static unsigned long (*pCString)(REALstring) = nil;
		if (!pCString)
			pCString = (unsigned long (*)(REALstring)) CallResolver("StringGetCString");
		if (pCString) return (const char *)pCString(this);
		else return (const char *)0;
	} else
		return "";
}

const unsigned char *REALstringStruct::PString()
{
	if (this) {
		static unsigned long (*pPString)(REALstring) = nil;
		if (!pPString)
			pPString = (unsigned long (*)(REALstring)) CallResolver("StringGetPString");
		if (pPString) return (const unsigned char *)pPString(this);
		else return (const unsigned char *)0;
	} else {
		#if TARGET_OS_MAC
			return (unsigned char *)"\p";
		#else
			return (unsigned char *)"\0";
		#endif
	}
}

#if _MSC_VER
	#pragma warning( default: 4995 )
#endif
