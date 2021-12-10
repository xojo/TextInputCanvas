// PluginMain.cpp
//
// This file is part of the REALbasic plugin API.
//
// (c) 2013 Xojo, Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#include <stdlib.h>
#include <string.h>

#include "REALplugin.h"
#include "rb_plugin.h"

#ifdef WIN32
extern "C" void __declspec(dllexport) REALPluginMain(void *(*resolver)(const char *entryName));
#elif defined(__GNUC__)
extern "C" __attribute__((visibility("default"))) void REALPluginMain(void *(*resolver)(const char *entryName));
#else
extern "C" void REALPluginMain(void *(*resolver)(const char *entryName));
#endif

extern void *(*gResolver)(const char *entryName);
void *(*gResolver)(const char *entryName);

inline void* CallResolver(const char *entryName)
{
	return gResolver(entryName);
}

bool REALGetStringData( REALstring str, uint32_t encoding, REALstringData *outData )
{
	static RBBoolean (*pGetStringData)(REALstring, uint32_t, REALstringData*);
	if (!pGetStringData)
		pGetStringData = (RBBoolean(*)(REALstring, uint32_t, REALstringData*)) CallResolver("PluginGetStringData");
	if (pGetStringData) {
		return pGetStringData(str, encoding, outData);
	}
	return false;
}

void REALDisposeStringData( REALstringData *data )
{
	static void (*pDisposeStringData)(REALstringData*);
	if (!pDisposeStringData)
		pDisposeStringData = (void (*)(REALstringData*)) CallResolver("PluginDisposeStringData");
	if (pDisposeStringData) {
		pDisposeStringData(data);
	}
}

#if TARGET_CARBON || TARGET_COCOA
CFStringRef REALCopyStringCFString( REALstring str )
{
	static CFStringRef (*pGetStringCFString)(REALstring, RBBoolean) = nil;
	if (!pGetStringCFString)
		pGetStringCFString = (CFStringRef (*)(REALstring, RBBoolean)) CallResolver("REALGetStringCFString");
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

bool REALObjectIsA( REALobject obj, REALclassRef classRef )
{
	static RBBoolean (*pObjectIsA)(REALobject, REALclassRef);
	if (!pObjectIsA)
		pObjectIsA = (RBBoolean (*)(REALobject, REALclassRef)) CallResolver("RuntimeObjectIsa");
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

uint32_t REALGetSystemTextEncoding(void)
{
	static uint32_t (*pGetSystemTextEncoding)() = nil;
	if (!pGetSystemTextEncoding)
		pGetSystemTextEncoding = (uint32_t(*)()) CallResolver("REALGetSystemTextEncoding");
	if (pGetSystemTextEncoding)
		return pGetSystemTextEncoding();
	else
		return kREALTextEncodingUTF8;
}

void SetClassConsoleSafe( REALclassDefinition *def )	// Nov 04 2004 -- AJB (1)
{
	def->mFlags |= REALconsoleSafe;
}

void SetClassWebOnly( REALclassDefinition *def )	// Nov 04 2004 -- AJB (1)
{
	def->mFlags = (def->mFlags | REALwebOnly) & ~REALconsoleOnly;
}

bool REALinRuntime(void)
{
	static RBBoolean (*pInRuntime)(void) = nil;
	if (!pInRuntime)
		pInRuntime = (RBBoolean (*)(void)) CallResolver("REALinRuntime");
	if (pInRuntime) return pInRuntime();
	else return false;
}

int32_t REALRegisterBackgroundTask( BackgroundTaskProc proc, uint32_t period, void *data )
{
	static int32_t(*pRuntimeRegisterBackgroundTask)(BackgroundTaskProc, uint32_t, void *);
	if (!pRuntimeRegisterBackgroundTask)
		pRuntimeRegisterBackgroundTask = (int32_t(*)(BackgroundTaskProc, uint32_t, void *)) CallResolver("PluginRegisterBackgroundTask");
	if (pRuntimeRegisterBackgroundTask) return pRuntimeRegisterBackgroundTask( proc, period, data );
	return -1;
}

void REALUnregisterBackgroundTask( int32_t id )
{
	static void(*pRuntimeUnregisterBackgroundTask)(int32_t id);
	if (!pRuntimeUnregisterBackgroundTask)
		pRuntimeUnregisterBackgroundTask = (void(*)(int32_t)) CallResolver("PluginUnregisterBackgroundTask");
	if (pRuntimeUnregisterBackgroundTask) pRuntimeUnregisterBackgroundTask( id );
}

void REALRegisterControl(REALcontrol *defn)
{
	static void (*pRuntimeRegisterControl)(REALcontrol *defn);
	if (!pRuntimeRegisterControl)
		pRuntimeRegisterControl = (void (*)(REALcontrol *)) CallResolver("PluginRegisterControl");
	
	if (pRuntimeRegisterControl) pRuntimeRegisterControl(defn);
}

void REALRegisterMobileControl(REALmobileControl *defn)
{
    static void (*pRuntimeRegisterControl)(REALmobileControl *defn);
    if (!pRuntimeRegisterControl)
        pRuntimeRegisterControl = (void (*)(REALmobileControl *)) CallResolver("PluginRegisterMobileControl");
    
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
	static REALstring (*pDefaultControlFont)(void) = nil;
	if (!pDefaultControlFont)
		pDefaultControlFont = (REALstring(*)(void)) CallResolver("PluginDefaultControlFont");
	if (pDefaultControlFont) return pDefaultControlFont();
	else return 0;
}

uint32_t REALDefaultControlFontSize(void)
{
	static uint32_t(*pDefaultControlFontSize)(void) = nil;
	if (!pDefaultControlFontSize)
		pDefaultControlFontSize = (uint32_t(*)(void)) CallResolver("PluginDefaultControlFontSize");
	if (pDefaultControlFontSize) return pDefaultControlFontSize();
	else return 0;
}

REALstring REALBuildString(const char *contents, int length)
{
	static REALstring (*pBuildString)(const char *, int) = nil;
	if (!pBuildString)
		pBuildString = (REALstring(*)(const char *, int)) CallResolver("REALBuildString");
	if (pBuildString) return pBuildString(contents, length);
	else return 0;
}

REALstring REALBuildStringWithEncoding( const char *contents, int byteCount, uint32_t encoding )
{
	static REALstring (*pBuildString)(const char *, int, uint32_t) = nil;
	if (!pBuildString)
		pBuildString = (REALstring(*)(const char *, int, uint32_t)) CallResolver("REALBuildStringWithEncoding");
	if (pBuildString) return pBuildString(contents, byteCount, encoding);
	else return 0;
}

REALstring REALBuildString( const char *contents, int byteCount, uint32_t encoding )
{
	return REALBuildStringWithEncoding(contents, byteCount, encoding);
}

void *REALGetStringContents( REALstring str, size_t *numBytes )
{
	if (!str) return NULL;

	static void *(*pCString)(REALstring) = nil;
	static RBInteger (*pLenB)(REALstring);
	if (!pCString)
		pCString = (void *(*)(REALstring)) CallResolver("StringGetCString");
	if (!pLenB)
		pLenB = (RBInteger (*)(REALstring))REALLoadFrameworkMethod( "LenB( s as string ) as integer" );
	
	if (numBytes && pLenB) {
		*numBytes = pLenB(str);
	}
	
	if (pCString) return pCString(str);

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

void REALLockText(REALtext value)
{
	static void(*pLockText)(REALtext) = NULL;
	if (!pLockText) pLockText = (void(*)(REALtext)) CallResolver("PluginLockText");
	if (pLockText) pLockText(value);
}

void REALUnlockText(REALtext value)
{
	static void(*pUnlockText)(REALtext) = NULL;
	if (!pUnlockText) pUnlockText = (void(*)(REALtext)) CallResolver("PluginUnlockText");
	if (pUnlockText) pUnlockText(value);
}

REALtext REALBuildText(const void *data,
	size_t size,
	const char *encodingName)
{
	static REALtext(*pBuildText)(const void *data, size_t, const char *) = NULL;
	if (!pBuildText) pBuildText = (REALtext (*)(const void *data, size_t, const char *)) CallResolver("PluginBuildText");
	if (pBuildText) return pBuildText(data, size, encodingName);
	return NULL;
}

RBInteger REALCompareText(REALtext value1, REALtext value2, RBInteger options)
{
	static RBInteger(*pCompareText)(REALtext, REALtext, RBInteger) = NULL;
	if (!pCompareText) pCompareText = (RBInteger(*)(REALtext, REALtext, RBInteger)) CallResolver("PluginCompareText");
	if (pCompareText) return pCompareText(value1, value2, options);
	return 0;
}

REALtext REALConcatenateText(REALtext value1, REALtext value2)
{
	static REALtext(*pConcatenateText)(REALtext, REALtext) = NULL;
	if (!pConcatenateText) pConcatenateText = (REALtext(*)(REALtext, REALtext)) CallResolver("PluginConcatenateText");
	if (pConcatenateText) return pConcatenateText(value1, value2);
	return NULL;
}

REALobject REALNewVariantText(REALtext value)
{
	static REALobject(*pTextToVariant)(REALtext) = NULL;
	if (!pTextToVariant) pTextToVariant = (REALobject(*)(REALtext)) CallResolver("PluginNewVariantText");
	if (pTextToVariant) return pTextToVariant(value);
	return NULL;
}

bool REALGetPropValueText(REALobject value,
	const char *propName,
	REALtext *result)
{
	static RBBoolean(*pGetPropValueText)(REALobject, const char *, REALtext *) = NULL;
	if (!pGetPropValueText) pGetPropValueText = (RBBoolean(*)(REALobject, const char *, REALtext *)) CallResolver("PluginGetPropValueText");
	if (pGetPropValueText) return pGetPropValueText(value, propName, result);
	return false;
}

REALtextData *REALGetTextData(REALtext value,
	const char *encodingName,
	bool allowLossyConversion)
{
	static REALtextData *(*pGetTextData)(REALtext, const char *, RBBoolean) = NULL;
	if (!pGetTextData) pGetTextData = (REALtextData *(*)(REALtext, const char *, RBBoolean)) CallResolver("PluginGetTextData");
	if (pGetTextData) return pGetTextData(value, encodingName, allowLossyConversion);
	return NULL;
}

void REALDisposeTextData(REALtextData *value)
{
	static void(*pDisposeTextData)(REALtextData *) = NULL;
	if (!pDisposeTextData) pDisposeTextData = (void(*)(REALtextData *)) CallResolver("PluginDisposeTextData");
	if (pDisposeTextData) pDisposeTextData(value);
}

#if defined(TARGET_OS_MAC) && TARGET_OS_MAC
CFStringRef REALCopyTextCFString(REALtext value)
{
	static CFStringRef(*pCopyTextCFString)(REALtext) = NULL;
	if (!pCopyTextCFString) pCopyTextCFString = (CFStringRef(*)(REALtext)) CallResolver("PluginCopyTextCFString");
	if (pCopyTextCFString) return pCopyTextCFString(value);
	return NULL;
}
#endif

REALpicture REALBuildPictureFromPictureDescription(REALpictureDescription *description, bool bPassOwnership)
{
	static REALpicture (*pBuildPictureFromPictureDescription)(REALpictureDescription *, RBBoolean) = nil;
	if (!pBuildPictureFromPictureDescription)
		pBuildPictureFromPictureDescription = (REALpicture(*)(REALpictureDescription *, RBBoolean)) CallResolver("REALBuildPictureFromPictureDescription");
	if (pBuildPictureFromPictureDescription) return pBuildPictureFromPictureDescription(description, bPassOwnership);
	else return 0;
}

void REALLockPictureDescriptionWithNativeType(REALpicture pic, REALpictureDescription *description)
{
	static void (*pLockPictureDescription)(REALpicture, REALpictureDescription *) = nil;
	if (!pLockPictureDescription)
		pLockPictureDescription = (void (*)(REALpicture, REALpictureDescription *)) CallResolver("lockPictureDescription");
	if (pLockPictureDescription) pLockPictureDescription(pic, description);
}

void REALLockPictureDescription(REALpicture pic, REALpictureDescription *description)
{
	REALLockPictureDescriptionWithNativeType(pic, description);
}

void REALUnlockPictureDescription(REALpicture pic)
{
	static void (*pUnlockPictureDescription)(REALpicture) = nil;
	if (!pUnlockPictureDescription)
		pUnlockPictureDescription = (void (*)(REALpicture)) CallResolver("unlockPictureDescription");
	if (pUnlockPictureDescription) pUnlockPictureDescription(pic);
}

REALdbCursor REALdbCursorFromDBCursor(dbCursor *cursor, REALdbCursorDefinition *defn)
{
	static REALdbCursor (*pDbCursorFromDBCursor)(dbCursor *, REALdbCursorDefinition *) = nil;
	if (!pDbCursorFromDBCursor)
		pDbCursorFromDBCursor = (REALdbCursor (*)(dbCursor *, REALdbCursorDefinition *)) CallResolver("REALdbCursorFromDBCursor");
	if (pDbCursorFromDBCursor) return pDbCursorFromDBCursor(cursor, defn);
	else return (REALdbCursor)0;
}

REALdbCursor REALNewRowSetFromDBCursor(dbCursor *cursor, REALdbCursorDefinition *defn)
{
	static REALdbCursor(*pNewRowSetFromDBCursor)(dbCursor *, REALdbCursorDefinition *) = nil;
	if (!pNewRowSetFromDBCursor)
		pNewRowSetFromDBCursor = (REALdbCursor(*)(dbCursor *, REALdbCursorDefinition *)) CallResolver("NewRowSetFromDBCursor");
	if (pNewRowSetFromDBCursor) return pNewRowSetFromDBCursor(cursor, defn);
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
	static void *(*pGetEventInstance)(REALcontrolInstance instance, RBInteger);
	if (!pGetEventInstance)
		pGetEventInstance = (void *(*)(REALcontrolInstance,RBInteger)) CallResolver("GetEventInstance");
	if (pGetEventInstance) return (void *) pGetEventInstance(instance, event->forSystemUse);

	return nil;
}

void *REALGetControlData(REALcontrolInstance instance, REALcontrol *defn)
{
	return ((char *)instance) + defn->forSystemUse;
}

void *REALGetClassData(REALobject instance, REALclassDefinition *defn)
{
	return ((char *)instance) + defn->forSystemUse;
}

void *REALGetMobileControlData(REALcontrolInstance instance, REALmobileControl *defn)
{
	return ((char *)instance) + defn->forSystemUse;
}


#if TARGET_CARBON
REALappleEvent REALBuildAppleEvent(const AppleEvent *event, bool bPassOwnership)
{
	static REALappleEvent (*pBuildAppleEvent)(const AppleEvent *, RBBoolean) = nil;
	if (!pBuildAppleEvent)
		pBuildAppleEvent = (REALappleEvent (*)(const AppleEvent *, RBBoolean)) CallResolver("REALBuildAppleEvent");
	if (pBuildAppleEvent) return pBuildAppleEvent(event, bPassOwnership);
	else return 0;
}
#endif

#if TARGET_CARBON
REALappleEvent REALBuildAEDescList(const AppleEvent *event, bool bPassOwnership)
{
	static REALappleEvent (*pBuildAEDescList)(const AppleEvent *, RBBoolean) = nil;
	if (!pBuildAEDescList)
		pBuildAEDescList = (REALappleEvent (*)(const AppleEvent *, RBBoolean)) CallResolver("REALBuildAEDescList");
	if (pBuildAEDescList) return pBuildAEDescList(event, bPassOwnership);
	else return 0;
}
#endif

#if TARGET_CARBON
REALappleEvent REALBuildAEObjSpecifier(const AppleEvent *event, bool bPassOwnership)
{
	static REALappleEvent (*pBuildAEObjSpecifier)(const AppleEvent *, RBBoolean) = nil;
	if (!pBuildAEObjSpecifier)
		pBuildAEObjSpecifier = (REALappleEvent (*)(const AppleEvent *, RBBoolean)) CallResolver("REALBuildAEObjSpecifier");
	if (pBuildAEObjSpecifier) return pBuildAEObjSpecifier(event, bPassOwnership);
	else return 0;
}
#endif

#if TARGET_CARBON
AppleEvent *REALAccessAppleEvent(REALappleEvent event)
{
	static AppleEvent *(*pAccessAppleEvent)(REALappleEvent) = nil;
	if (!pAccessAppleEvent)
		pAccessAppleEvent = (AppleEvent *(*)(REALappleEvent)) CallResolver("REALAccessAppleEvent");
	if (pAccessAppleEvent) return pAccessAppleEvent(event);
	else return 0;
}
#endif

#if TARGET_CARBON
AppleEvent *REALAccessAppleEventReply(REALappleEvent event)
{
	static AppleEvent *(*pAccessAppleEventReply)(REALappleEvent) = nil;
	if (!pAccessAppleEventReply)
		pAccessAppleEventReply = (AppleEvent *(*)(REALappleEvent)) CallResolver("REALAccessAppleEventReply");
	if (pAccessAppleEventReply) return pAccessAppleEventReply(event);
	else return 0;
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

void REALDesignAddDataSource(const char *baseName, const char *szDataSourceName, void *data, int dataLen)
{
	static void (*pDesignAddDataSource)(const char *, const char *, void *, int) = nil;
	if (!pDesignAddDataSource)
		pDesignAddDataSource = (void (*)(const char *, const char *, void *, int)) CallResolver("DesignAddDataSource");
	if (pDesignAddDataSource) pDesignAddDataSource(baseName, szDataSourceName, data, dataLen);
}

#if TARGET_WIN32
REALpicture REALBuildPictureFromDIB(HANDLE hDIB, bool bPassOwnership)
{
	static REALpicture (*pBuildPictureFromDIB)(HANDLE, RBBoolean) = nil;
	if (!pBuildPictureFromDIB)
		pBuildPictureFromDIB = (REALpicture (*)(HANDLE, RBBoolean)) CallResolver("REALBuildPictureFromDIB");
	if (pBuildPictureFromDIB) return pBuildPictureFromDIB(hDIB, bPassOwnership);
	else return 0;
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

RBInteger REALGetArrayUBound(void*array)
{
	static RBInteger (*pGetArrayUBound)(void*) = nil;
	if (!pGetArrayUBound)
		pGetArrayUBound = (RBInteger (*)(void*)) CallResolver("RuntimeUBound");
	if (pGetArrayUBound) return pGetArrayUBound(array);
	else return (RBInteger)0;
}

void REALGetArrayStructure( REALarray array, RBInteger index, void *structure )
{
	static void(*pGetStructureObject)(REALarray, RBInteger, void *) = nil;
	if (!pGetStructureObject)
		pGetStructureObject = (void (*)(REALarray, RBInteger, void *)) CallResolver("RuntimeDirectReadStructureArray");
	if (pGetStructureObject) pGetStructureObject(array, index, structure);
}

bool REALGetVariantStructure( REALobject variant, void *buffer, size_t length )
{
	static RBBoolean(*pGetStructureObject)(REALobject, void *, size_t) = nil;
	if (!pGetStructureObject)
		pGetStructureObject = (RBBoolean(*)(REALobject, void *, size_t)) CallResolver("PluginStructureFromVariant");
	if (pGetStructureObject) return pGetStructureObject( variant, buffer, length );
	return false;
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
	static REALobject (*pNewInstance)(REALclassRef) = nil;
	if (!pNewInstance)
		pNewInstance = (REALobject (*)(REALclassRef)) CallResolver("CreateInstance");
	if (pNewInstance and classRef) return pNewInstance(classRef);
	else return 0;
}

REALobject REALnewInstance(REALclassRef classRef)
{
	return REALnewInstanceWithClass(classRef);
}

REALobject REALnewInstanceOfClass(REALclassDefinition *classDefn)
{
	static REALobject(*pNewInstance)(REALclassDefinition *) = nil;
	if (!pNewInstance)
		pNewInstance = (REALobject(*)(REALclassDefinition *)) CallResolver("PluginNewInstanceOfClass");

	if (pNewInstance)
		return pNewInstance(classDefn);

	return (REALobject)0;
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

REALDBConnectionDialogRef REALDBConnectionDialogCreate(void)
{
	static REALDBConnectionDialogRef (*pDBConnectionDialogCreate)(void) = nil;
	if (!pDBConnectionDialogCreate)
		pDBConnectionDialogCreate = (REALDBConnectionDialogRef (*)(void)) CallResolver("REALDBConnectionDialogCreate");
	if (pDBConnectionDialogCreate) return pDBConnectionDialogCreate();
	else return (REALDBConnectionDialogRef)0;
}

void REALDBConnectionDialogAddField(REALDBConnectionDialogRef dialogRef, REALstring label, REALstring defaultText, bool maskField)
{
	static void (*pDBConnectionDialogAddField)(REALDBConnectionDialogRef, REALstring, REALstring, RBBoolean) = nil;
	if (!pDBConnectionDialogAddField)
		pDBConnectionDialogAddField = (void (*)(REALDBConnectionDialogRef, REALstring, REALstring, RBBoolean)) CallResolver("REALDBConnectionDialogAddField");
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
REALpicture REALBuildPictureFromBuffer(uint32_t width, uint32_t height, RBPixelType pixelType, void*buffer, uint32_t rowBytes)
{
	static REALpicture(*pBuildPictureFromBuffer)(uint32_t, uint32_t, RBPixelType, void*, uint32_t) = nil;
	if (!pBuildPictureFromBuffer)
		pBuildPictureFromBuffer = (REALpicture(*)(uint32_t, uint32_t, RBPixelType, void*, uint32_t)) CallResolver("REALBuildPictureFromBuffer");
	if (pBuildPictureFromBuffer) return pBuildPictureFromBuffer(width, height, pixelType, buffer, rowBytes);
	else return 0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
bool REALInDebugMode(void)
{
	static RBBoolean (*pInDebugMode)(void) = nil;
	if (!pInDebugMode)
		pInDebugMode = (RBBoolean (*)(void)) CallResolver("REALInDebugMode");
	if (pInDebugMode) return pInDebugMode();
	else return false;
}
#endif

void REALStripAmpersands(REALstring*  ioString)
{
	static void (*pStripAmpersands)(REALstring* ) = nil;
	if (!pStripAmpersands)
		pStripAmpersands = (void (*)(REALstring* )) CallResolver("REALStripAmpersands");
	if (pStripAmpersands) pStripAmpersands(ioString);
}

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

void *REALLoadSharedMethod(REALclassRef classRef, const char *prototype)
{
	static void *(*pLoadSharedMethod)(REALclassRef, const char *);
	if (!pLoadSharedMethod)
		pLoadSharedMethod =
		    (void *(*)(REALclassRef, const char *))CallResolver("PluginLoadSharedMethod");
	if (pLoadSharedMethod)
		return pLoadSharedMethod(classRef, prototype);
	return NULL;
}

bool REALGetPropValueInt32(REALobject object, const char *propName, int32_t *outValue)
{
	static long(*pGetPropValue)(REALobject, const char *, int32_t *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long(*)(REALobject, const char *, int32_t *)) CallResolver("REALGetPropValueInt");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValueString(REALobject object, const char *propName, REALstring *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, REALstring *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, REALstring *)) CallResolver("REALGetPropValueString");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValueDouble(REALobject object, const char *propName, double *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, double *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, double *)) CallResolver("REALGetPropValueDouble");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValueObject(REALobject object, const char *propName, REALobject *outValue)
{
	static long (*pGetPropValue)(REALobject, const char *, REALobject *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, REALobject *)) CallResolver("REALGetPropValueObject");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValuePtr( REALobject object, const char *propName, void **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, void **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, void **)) CallResolver("REALGetPropValuePtr");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValueCString( REALobject object, const char *propName, const char **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, const char **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, const char **)) CallResolver("REALGetPropValueCString");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValueWString( REALobject object, const char *propName, const wchar_t **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, const wchar_t **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, const wchar_t **)) CallResolver("REALGetPropValueWString");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

bool REALGetPropValuePString( REALobject object, const char *propName, const unsigned char **outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, const unsigned char **) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, const unsigned char **)) CallResolver("REALGetPropValuePString");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}

#if TARGET_CARBON || TARGET_COCOA
bool REALGetPropValueCFStringRef( REALobject object, const char *propName, CFStringRef *outValue )
{
	static long (*pGetPropValue)(REALobject, const char *, CFStringRef *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (long (*)(REALobject, const char *, CFStringRef *)) CallResolver("REALGetPropValueCFStringRef");
	if (pGetPropValue) return (bool)pGetPropValue(object, propName, outValue);
	else return false;
}
#endif

void REALSetDBIsConnected(REALdbDatabase database, bool connected)
{
	static void (*pSetDBIsConnected)(REALdbDatabase, RBBoolean) = nil;
	if (!pSetDBIsConnected)
		pSetDBIsConnected = (void (*)(REALdbDatabase, RBBoolean)) CallResolver("REALSetDBIsConnected");
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

REALobject REALNewVariantStructure( const void *data, size_t len )
{
	static REALobject(*pNewVariantStructure)(const void *, size_t) = nil;
	if (!pNewVariantStructure)
		pNewVariantStructure = (REALobject(*)(const void *, size_t)) CallResolver("PluginStructureToVariant");
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

REALobject REALNewVariantOSType(uint32_t value)
{
	static REALobject(*pNewVariant)(uint32_t) = nil;
	if (!pNewVariant)
		pNewVariant = (REALobject(*)(uint32_t)) CallResolver("OSTypeToVariant");
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

REALobject REALNewVariantInt32(int32_t value)
{
	static REALobject(*pNewVariantInteger)(int32_t) = nil;
	if (!pNewVariantInteger)
		pNewVariantInteger = (REALobject(*)(int32_t)) CallResolver("Int32ToVariant");
	if (pNewVariantInteger) return pNewVariantInteger(value);
	else return (REALobject)0;
}

REALobject REALNewVariantInteger(RBInteger value)
{
	static REALobject(*pNewVariantInteger)(RBInteger) = nil;
	if (!pNewVariantInteger)
		pNewVariantInteger = (REALobject(*)(RBInteger)) CallResolver("IntegerToVariant");
#if !TARGET_64BIT
	// Pre-2015r3
	if (!pNewVariantInteger)
		pNewVariantInteger = (REALobject(*)(RBInteger)) CallResolver("Int32ToVariant");
#endif
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

REALobject REALNewVariantBoolean(bool value)
{
	static REALobject (*pNewVariantBoolean)(RBBoolean) = nil;
	if (!pNewVariantBoolean)
		pNewVariantBoolean = (REALobject (*)(RBBoolean)) CallResolver("BooleanToVariant");
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

bool REALSetPropValueInt32(REALobject object, const char *propName, int32_t value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, int32_t) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, int32_t)) CallResolver("REALSetPropValueInt");
	if (pSetPropValue) return (bool)pSetPropValue(object, propName, value);
	else return false;
}

bool REALSetPropValueString(REALobject object, const char *propName, REALstring value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, REALstring) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, REALstring)) CallResolver("REALSetPropValueString");
	if (pSetPropValue) return (bool)pSetPropValue(object, propName, value);
	else return false;
}

bool REALSetPropValueDouble(REALobject object, const char *propName, double value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, double) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, double)) CallResolver("REALSetPropValueDouble");
	if (pSetPropValue) return (bool)pSetPropValue(object, propName, value);
	else return false;
}

bool REALSetPropValueObject(REALobject object, const char *propName, REALobject value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, REALobject) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, REALobject)) CallResolver("REALSetPropValueObject");
	if (pSetPropValue) return (bool)pSetPropValue(object, propName, value);
	else return false;
}

dbCursor *REALGetCursorFromREALdbCursor(REALdbCursor cursor)
{
	static dbCursor *(*pGetCursorFromREALdbCursor)(REALdbCursor) = nil;
	if (!pGetCursorFromREALdbCursor)
		pGetCursorFromREALdbCursor = (dbCursor *(*)(REALdbCursor)) CallResolver("REALGetCursorFromREALdbCursor");
	if (pGetCursorFromREALdbCursor) return pGetCursorFromREALdbCursor(cursor);
	else return (dbCursor *)0;
}

bool REALLockPictureDescription(REALpicture pic, REALpictureDescription *description, uint32_t picType)
{
	static RBBoolean(*pLockPictureDescription)(REALpicture, REALpictureDescription *, uint32_t) = nil;
	if (!pLockPictureDescription)
		pLockPictureDescription = (RBBoolean(*)(REALpicture, REALpictureDescription *, uint32_t)) CallResolver("REALLockPictureDescription");
	if (pLockPictureDescription) return pLockPictureDescription(pic, description, picType);
	else return false;
}

REALobject REALNewVariantUInt32(uint32_t value)
{
	static REALobject(*pNewVariantUInt32)(uint32_t) = nil;
	if (!pNewVariantUInt32)
		pNewVariantUInt32 = (REALobject(*)(uint32_t)) CallResolver("UInt32ToVariant");
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

bool REALGetPropValueInt64(REALobject object, const char *name, RBInt64 *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, RBInt64 *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, RBInt64 *)) CallResolver("REALGetPropValueInt64");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueUInt64(REALobject object, const char *param, RBUInt64 *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, RBUInt64 *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, RBUInt64 *)) CallResolver("REALGetPropValueUInt64");
	if (pGetPropValue) return (bool)pGetPropValue(object, param, value);
	else return false;
}

bool REALGetPropValueUInt32(REALobject object, const char *name, uint32_t *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, uint32_t *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, uint32_t *)) CallResolver("REALGetPropValueUInt32");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueUInt16(REALobject object, const char *name, uint16_t *value)
{
	static RBBoolean(*pGetPropValue)(REALobject, const char *, uint16_t *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, uint16_t *)) CallResolver("REALGetPropValueUInt16");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueInt16(REALobject object, const char *name, int16_t *value)
{
	static RBBoolean(*pGetPropValue)(REALobject, const char *, int16_t *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, int16_t *)) CallResolver("REALGetPropValueInt16");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueInt8(REALobject object, const char *name, char *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, char *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, char *)) CallResolver("REALGetPropValueInt8");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueUInt8(REALobject object, const char *name, unsigned char *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, unsigned char *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, unsigned char *)) CallResolver("REALGetPropValueUInt8");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}


bool REALGetPropValueBoolean(REALobject object, const char *name, bool *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, RBBoolean *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, RBBoolean *)) CallResolver("REALGetPropValueBool");

	if (pGetPropValue) {
		RBBoolean retValue;
		bool result = (bool)pGetPropValue(object, name, &retValue);
		*value = retValue;
		return result;
	}
	
	return false;
}

bool REALGetPropValueSingle(REALobject object, const char *name, float *value)
{
	static RBBoolean (*pGetPropValue)(REALobject, const char *, float *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean (*)(REALobject, const char *, float *)) CallResolver("REALGetPropValueSingle");
	if (pGetPropValue) return (bool)pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueColor(REALobject object, const char *name, RBColor *value)
{
	static RBBoolean(*pGetPropValue)(REALobject, const char *, RBColor *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, RBColor *)) CallResolver("REALGetPropValueColor");
#if !TARGET_64BIT
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, RBColor *)) CallResolver("REALGetPropValueInt");
#endif
	if (pGetPropValue) return pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueCurrency(REALobject object, const char *name, REALcurrency *value)
{
	static RBBoolean(*pGetPropValue)(REALobject, const char *, REALcurrency *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, REALcurrency *)) CallResolver("REALGetPropValueCurrency");
#if !TARGET_64BIT
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, REALcurrency *)) CallResolver("REALGetPropValueInt64");
#endif
	if (pGetPropValue) return pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueInteger(REALobject object, const char *name, RBInteger *value)
{
	static RBBoolean(*pGetPropValue)(REALobject, const char *, RBInteger *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, RBInteger *)) CallResolver("REALGetPropValueInteger");
#if !TARGET_64BIT
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, RBInteger *)) CallResolver("REALGetPropValueInt");
#endif
	if (pGetPropValue) return pGetPropValue(object, name, value);
	else return false;
}

bool REALGetPropValueUInteger(REALobject object, const char *name, RBUInteger *value)
{
	static RBBoolean(*pGetPropValue)(REALobject, const char *, RBUInteger *) = nil;
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, RBUInteger *)) CallResolver("REALGetPropValueUInteger");
#if !TARGET_64BIT
	if (!pGetPropValue)
		pGetPropValue = (RBBoolean(*)(REALobject, const char *, RBUInteger *)) CallResolver("REALGetPropValueUInt32");
#endif
	if (pGetPropValue) return pGetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueColor(REALobject object, const char *name, RBColor value)
{
	static RBBoolean(*pSetPropValue)(REALobject, const char *, RBColor) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, RBColor)) CallResolver("REALSetPropValueColor");
#if !TARGET_64BIT
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, RBColor)) CallResolver("REALSetPropValueInt");
#endif
	if (pSetPropValue) return pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueCurrency(REALobject object, const char *name, REALcurrency value)
{
	static RBBoolean(*pSetPropValue)(REALobject, const char *, REALcurrency) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, REALcurrency)) CallResolver("REALSetPropValueCurrency");
#if !TARGET_64BIT
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, REALcurrency)) CallResolver("REALSetPropValueInt64");
#endif
	if (pSetPropValue) return pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueInteger(REALobject object, const char *name, RBInteger value)
{
	static RBBoolean(*pSetPropValue)(REALobject, const char *, RBInteger) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, RBInteger)) CallResolver("REALSetPropValueInteger");
#if !TARGET_64BIT
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, RBInteger)) CallResolver("REALSetPropValueInt");
#endif
	if (pSetPropValue) return pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueUInteger(REALobject object, const char *name, RBUInteger value)
{
	static RBBoolean(*pSetPropValue)(REALobject, const char *, RBUInteger) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, RBUInteger)) CallResolver("REALSetPropValueUInteger");
#if !TARGET_64BIT
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, RBUInteger)) CallResolver("REALSetPropValueUInt32");
#endif
	if (pSetPropValue) return pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueUInt32(REALobject object, const char *name, uint32_t value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, uint32_t) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, uint32_t)) CallResolver("REALSetPropValueUInt32");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueUInt64(REALobject object, const char *name, RBUInt64 value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, RBUInt64) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, RBUInt64)) CallResolver("REALSetPropValueUInt64");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueInt64(REALobject object, const char *name, RBInt64 value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, RBInt64) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, RBInt64)) CallResolver("REALSetPropValueInt64");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueInt16(REALobject object, const char *name, int16_t value)
{
	static RBBoolean(*pSetPropValue)(REALobject, const char *, int16_t) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, int16_t)) CallResolver("REALSetPropValueInt16");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueUInt16(REALobject object, const char *name, uint16_t value)
{
	static RBBoolean(*pSetPropValue)(REALobject, const char *, uint16_t) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean(*)(REALobject, const char *, uint16_t)) CallResolver("REALSetPropValueUInt16");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueInt8(REALobject object, const char *name, char value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, char) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, char)) CallResolver("REALSetPropValueInt8");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueUInt8(REALobject object, const char *name, unsigned char value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, unsigned char) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, unsigned char)) CallResolver("REALSetPropValueBoolean");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueBoolean(REALobject object, const char *name, bool value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, RBBoolean) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, RBBoolean)) CallResolver("REALSetPropValueBoolean");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueSingle(REALobject object, const char *name, float value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, float) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, float)) CallResolver("REALSetPropValueSingle");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValuePtr(REALobject object, const char *name, void *value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, void *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, void *)) CallResolver("REALSetPropValuePtr");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueCString(REALobject object, const char *name, const char *value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, const char *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, const char *)) CallResolver("REALSetPropValueCString");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValueWString(REALobject object, const char *name, const wchar_t *value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, const wchar_t *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, const wchar_t *)) CallResolver("REALSetPropValueWString");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

bool REALSetPropValuePString(REALobject object, const char *name, const unsigned char *value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, const unsigned char *) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, const unsigned char *)) CallResolver("REALSetPropValuePString");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}

#if TARGET_CARBON || TARGET_COCOA
bool REALSetPropValueCFStringRef(REALobject object, const char *name, CFStringRef value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, CFStringRef) = nil;
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, CFStringRef)) CallResolver("REALSetPropValueCFStringRef");
	if (pSetPropValue) return (bool)pSetPropValue(object, name, value);
	else return false;
}
#endif

bool REALSetPropValueText(REALobject object, const char *name, REALtext value)
{
	static RBBoolean (*pSetPropValue)(REALobject, const char *, REALtext);
	if (!pSetPropValue)
		pSetPropValue = (RBBoolean (*)(REALobject, const char *, REALtext)) CallResolver("PluginSetPropValueText");
	if (pSetPropValue) return pSetPropValue(object, name, value);
	else return 0;
}

void REALInsertArrayValueColor(REALarray arr, RBInteger index, RBColor value)
{
	static void *(*pInsertArrayValueProc)(REALarray) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginColorArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBColor) = (void(*)(REALarray, RBInteger, RBColor))pInsertArrayValueProc(arr);
		if (fp)	fp(arr, index, value);
	}
}

void REALInsertArrayValueInteger(REALarray arr, RBInteger index, RBInteger value)
{
	static void *(*pInsertArrayValueProc)(REALarray) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginIntegerArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBInteger) = (void(*)(REALarray, RBInteger, RBInteger))pInsertArrayValueProc(arr);
		if (fp)	fp(arr, index, value);
	}
}

void REALInsertArrayValueUInteger(REALarray arr, RBInteger index, RBUInteger value)
{
	static void *(*pInsertArrayValueProc)(REALarray) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUIntegerArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBUInteger) = (void(*)(REALarray, RBInteger, RBUInteger))pInsertArrayValueProc(arr);
		if (fp)	fp(arr, index, value);
	}
}

void REALInsertArrayValueBoolean(REALarray arr, RBInteger index, bool value)
{
	static void *(*pInsertArrayValueProc)(REALarray) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginBooleanArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBBoolean) = (void(*)(REALarray, RBInteger, RBBoolean))pInsertArrayValueProc(arr);
		if (fp)	fp(arr, index, value);
	}
}

void REALInsertArrayValueCurrency(REALarray arr, RBInteger index, REALcurrency value)
{
	static void *(*pInsertArrayValueProc)(REALarray) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginCurrencyArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, REALcurrency) = (void(*)(REALarray, RBInteger, REALcurrency))pInsertArrayValueProc(arr);
		if (fp)	fp(arr, index, value);
	}
}

void REALInsertArrayValueInt64(REALarray arr, RBInteger index, RBInt64 value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "PluginInt64ArrayInsertProc" );
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBInt64) = (void(*)(REALarray, RBInteger, RBInt64))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueInt32(REALarray arr, RBInteger index, int32_t value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray )) CallResolver( "PluginInt32ArrayInsertProc" );
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, int32_t) = (void(*)(REALarray, RBInteger, int32_t))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueInt16(REALarray arr, RBInteger index, int16_t value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt16ArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, int16_t) = (void(*)(REALarray, RBInteger, int16_t))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueInt8(REALarray arr, RBInteger index, char value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt8ArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, char) = (void(*)(REALarray, RBInteger, char))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt64(REALarray arr, RBInteger index, RBUInt64 value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt64ArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBUInt64) = (void(*)(REALarray, RBInteger, RBUInt64))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt32(REALarray arr, RBInteger index, uint32_t value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt32ArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, uint32_t) = (void(*)(REALarray, RBInteger, uint32_t))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt16(REALarray arr, RBInteger index, uint16_t value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt16ArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, uint16_t) = (void(*)(REALarray, RBInteger, uint16_t))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueUInt8(REALarray arr, RBInteger index, unsigned char value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt8ArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, unsigned char) = (void(*)(REALarray, RBInteger, unsigned char))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueSingle(REALarray arr, RBInteger index, float value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginSingleArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, float) = (void(*)(REALarray, RBInteger, float))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueDouble(REALarray arr, RBInteger index, double value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginDoubleArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, double) = (void(*)(REALarray, RBInteger, double))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueString(REALarray arr, RBInteger index, REALstring value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginStringArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, REALstring) = (void(*)(REALarray, RBInteger, REALstring))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALInsertArrayValueText(REALarray arr, RBInteger index, REALtext value)
{
	static void *(*pInsertArrayValueProc)(REALarray) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginTextArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, REALtext) = (void(*)(REALarray, RBInteger, REALtext))pInsertArrayValueProc(arr);
		if (fp)	fp(arr, index, value);
	}
}

void REALInsertArrayValueObject(REALarray arr, RBInteger index, REALobject value)
{
	static void *(*pInsertArrayValueProc)( REALarray ) = nil;
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginObjectArrayInsertProc");
#if !TARGET_64BIT
	if (!pInsertArrayValueProc)
		pInsertArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetInsertProc");
#endif
	if (pInsertArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, REALobject) = (void(*)(REALarray, RBInteger, REALobject))pInsertArrayValueProc(arr);
		if (fp)	fp( arr, index, value );
	}
}

void REALGetArrayValueColor(REALarray arr, RBInteger index, RBColor *value)
{
	static void *(*pGetArrayValueProc)(REALarray) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginColorArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		RBColor(*fp)(REALarray, RBInteger) = (RBColor(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp(arr, index);
	}
}

void REALGetArrayValueInteger(REALarray arr, RBInteger index, RBInteger *value)
{
	static void *(*pGetArrayValueProc)(REALarray) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginIntegerArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		RBInteger(*fp)(REALarray, RBInteger) = (RBInteger(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp(arr, index);
	}
}

void REALGetArrayValueUInteger(REALarray arr, RBInteger index, RBUInteger *value)
{
	static void *(*pGetArrayValueProc)(REALarray) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUIntegerArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		RBUInteger(*fp)(REALarray, RBInteger) = (RBUInteger(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp(arr, index);
	}
}

void REALGetArrayValueBoolean(REALarray arr, RBInteger index, bool *value)
{
	static void *(*pGetArrayValueProc)(REALarray) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginBooleanArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		RBBoolean(*fp)(REALarray, RBInteger) = (RBBoolean(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp(arr, index);
	}
}

void REALGetArrayValueCurrency(REALarray arr, RBInteger index, REALcurrency *value)
{
	static void *(*pGetArrayValueProc)(REALarray) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginCurrencyArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		REALcurrency(*fp)(REALarray, RBInteger) = (REALcurrency(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp(arr, index);
	}
}

void REALGetArrayValueInt64(REALarray arr, RBInteger index, RBInt64 *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt64ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		RBInt64(*fp)(REALarray, RBInteger) = (RBInt64(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueInt32(REALarray arr, RBInteger index, int32_t *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt32ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		int32_t(*fp)(REALarray, RBInteger) = (int32_t(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueInt16(REALarray arr, RBInteger index, int16_t *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt16ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		int16_t(*fp)(REALarray, RBInteger) = (int16_t(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueInt8(REALarray arr, RBInteger index, char *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt8ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		char(*fp)(REALarray, RBInteger) = (char(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt64(REALarray arr, RBInteger index, RBUInt64 *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt64ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		RBUInt64(*fp)(REALarray, RBInteger) = (RBUInt64(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt32(REALarray arr, RBInteger index, uint32_t *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt32ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		uint32_t(*fp)(REALarray, RBInteger) = (uint32_t(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt16(REALarray arr, RBInteger index, uint16_t *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt16ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		uint16_t(*fp)(REALarray, RBInteger) = (uint16_t(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueUInt8(REALarray arr, RBInteger index, unsigned char *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt8ArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		unsigned char(*fp)(REALarray, RBInteger) = (unsigned char(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueSingle(REALarray arr, RBInteger index, float *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginSingleArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		float(*fp)(REALarray, RBInteger) = (float(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueDouble(REALarray arr, RBInteger index, double *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginDoubleArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		double(*fp)(REALarray, RBInteger) = (double(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueString(REALarray arr, RBInteger index, REALstring *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginStringArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		REALstring(*fp)(REALarray, RBInteger) = (REALstring(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALGetArrayValueText(REALarray arr, RBInteger index, REALtext *value)
{
	static void *(*pGetArrayValueProc)(REALarray) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginTextArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		REALtext(*fp)(REALarray, RBInteger) = (REALtext(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp(arr, index);
	}
}

void REALGetArrayValueObject(REALarray arr, RBInteger index, REALobject *value)
{
	static void *(*pGetArrayValueProc)( REALarray ) = nil;
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginObjectArrayGetProc");
#if !TARGET_64BIT
	if (!pGetArrayValueProc)
		pGetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetGetProc");
#endif
	if (pGetArrayValueProc)	{
		REALobject(*fp)(REALarray, RBInteger) = (REALobject(*)(REALarray, RBInteger))pGetArrayValueProc(arr);
		if (fp && value)	*value = fp( arr, index );
	}
}

void REALSetArrayValueColor(REALarray arr, RBInteger index, RBColor value)
{
	static void *(*pSetArrayValueProc)(REALarray) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginColorArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, RBColor, RBInteger) = (void(*)(REALarray, RBColor, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp(arr, value, index);
	}
}

void REALSetArrayValueInteger(REALarray arr, RBInteger index, RBInteger value)
{
	static void *(*pSetArrayValueProc)(REALarray) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginIntegerArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, RBInteger, RBInteger) = (void(*)(REALarray, RBInteger, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp(arr, value, index);
	}
}

void REALSetArrayValueUInteger(REALarray arr, RBInteger index, RBUInteger value)
{
	static void *(*pSetArrayValueProc)(REALarray) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUIntegerArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, RBUInteger, RBInteger) = (void(*)(REALarray, RBUInteger, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp(arr, value, index);
	}
}

void REALSetArrayValueBoolean(REALarray arr, RBInteger index, bool value)
{
	static void *(*pSetArrayValueProc)(REALarray) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginBooleanArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, RBBoolean, RBInteger) = (void(*)(REALarray, RBBoolean, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp(arr, value, index);
	}
}

void REALSetArrayValueCurrency(REALarray arr, RBInteger index, REALcurrency value)
{
	static void *(*pSetArrayValueProc)(REALarray) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginCurrencyArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, REALcurrency, RBInteger) = (void(*)(REALarray, REALcurrency, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp(arr, value, index);
	}
}

void REALSetArrayValueInt64(REALarray arr, RBInteger index, RBInt64 value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt64ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, RBInt64, RBInteger) = (void(*)(REALarray, RBInt64, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueInt32(REALarray arr, RBInteger index, int32_t value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt32ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, int32_t, RBInteger) = (void(*)(REALarray, int32_t, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueInt16(REALarray arr, RBInteger index, int16_t value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt16ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, int16_t, RBInteger) = (void(*)(REALarray, int16_t, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueInt8(REALarray arr, RBInteger index, char value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginInt8ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, char, RBInteger) = (void(*)(REALarray, char, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt64(REALarray arr, RBInteger index, RBUInt64 value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt64ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, RBUInt64, RBInteger) = (void(*)(REALarray, RBUInt64, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt32(REALarray arr, RBInteger index, uint32_t value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt32ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, uint32_t, RBInteger) = (void(*)(REALarray, uint32_t, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt16(REALarray arr, RBInteger index, uint16_t value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt16ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, uint16_t, RBInteger) = (void(*)(REALarray, uint16_t, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueUInt8(REALarray arr, RBInteger index, unsigned char value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginUInt8ArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, unsigned char, RBInteger) = (void(*)(REALarray, unsigned char, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueSingle(REALarray arr, RBInteger index, float value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginSingleArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void (*fp)( REALarray, float, long ) = (void (*)(REALarray, float, long))pSetArrayValueProc( arr );
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueDouble(REALarray arr, RBInteger index, double value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginDoubleArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, double, RBInteger) = (void(*)(REALarray, double, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueString(REALarray arr, RBInteger index, REALstring value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginStringArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, REALstring, RBInteger) = (void(*)(REALarray, REALstring, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

void REALSetArrayValueText(REALarray arr, RBInteger index, REALtext value)
{
	static void *(*pSetArrayValueProc)(REALarray) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginTextArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, REALtext, RBInteger) = (void(*)(REALarray, REALtext, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp(arr, value, index);
	}
}

void REALSetArrayValueObject(REALarray arr, RBInteger index, REALobject value)
{
	static void *(*pSetArrayValueProc)( REALarray ) = nil;
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("PluginObjectArraySetProc");
#if !TARGET_64BIT
	if (!pSetArrayValueProc)
		pSetArrayValueProc = (void *(*)(REALarray)) CallResolver("RuntimeArrayDirectGetSetProc");
#endif
	if (pSetArrayValueProc)	{
		void(*fp)(REALarray, REALobject, RBInteger) = (void(*)(REALarray, REALobject, RBInteger))pSetArrayValueProc(arr);
		if (fp)	fp( arr, value, index );
	}
}

REALarray REALCreateArray( REALArrayType type, RBInteger bounds )
{
	static REALarray(*pCreateArray)(REALArrayType, RBInteger) = nil;
	if (!pCreateArray)
		pCreateArray = (REALarray(*)(REALArrayType, RBInteger)) CallResolver("RuntimeArrayCreateSingleDimBoundedArray");
	if (pCreateArray)	return pCreateArray( type, bounds );

	return nil;
}

uint32_t REALGetStringEncoding(REALstring str)
{
	static uint32_t(*pGetStringEncoding)(REALstring) = nil;
	if (!pGetStringEncoding)
		pGetStringEncoding = (uint32_t(*)(REALstring)) CallResolver("REALGetStringEncoding");
	if (pGetStringEncoding) return pGetStringEncoding(str);
	else return 0;
}

REALstring REALConvertString(REALstring str, uint32_t encoding)
{
	static REALstring(*pConvertString)(REALstring, uint32_t) = nil;
	if (!pConvertString)
		pConvertString = (REALstring(*)(REALstring, uint32_t)) CallResolver("REALConvertString");
	if (pConvertString) return pConvertString(str, encoding);
	else return (REALstring)0;
}

void REALSetStringEncoding(REALstring str, uint32_t encoding)
{
	static void(*pSetStringEncoding)(REALstring, uint32_t) = nil;
	if (!pSetStringEncoding)
		pSetStringEncoding = (void(*)(REALstring, uint32_t)) CallResolver("REALSetStringEncoding");
	if (pSetStringEncoding) pSetStringEncoding(str, encoding);
}

uint32_t REALstringToOSType(REALstring id)
{
	static uint32_t(*fpStringToOSType)(REALstring) = nil;
	if (!fpStringToOSType)
		fpStringToOSType = (uint32_t (*)( REALstring ))CallResolver( "StringToOSType" );
	if (fpStringToOSType) return fpStringToOSType( id );
	return 0L;
}

inline uint32_t REALWin32CodePageToEncoding(uint32_t codePage)
{
	return 0xFFFF0000 | codePage;
}

void REALGetGraphicsOrigin(REALgraphics context, int32_t *originX, int32_t *originY)
{
	static void(*pGetGraphicsOrigin)(REALgraphics, int32_t *, int32_t *) = nil;
	if (!pGetGraphicsOrigin)
		pGetGraphicsOrigin = (void(*)(REALgraphics, int32_t *, int32_t *)) CallResolver("REALGetGraphicsOrigin");
	if (pGetGraphicsOrigin) pGetGraphicsOrigin(context, originX, originY);
}

void REALSetGraphicsOrigin(REALgraphics context, int32_t originX, int32_t originY)
{
	static void(*pSetGraphicsOrigin)(REALgraphics, int32_t, int32_t) = nil;
	if (!pSetGraphicsOrigin)
		pSetGraphicsOrigin = (void(*)(REALgraphics, int32_t, int32_t)) CallResolver("REALSetGraphicsOrigin");
	if (pSetGraphicsOrigin) pSetGraphicsOrigin(context, originX, originY);
}

#if TARGET_WIN32 || X_WINDOW
void REALSetAccelerator(REALcontrolInstance instance, REALstring key)
{
	static void(*pSetAccelerator)(REALcontrolInstance, REALstring) = nil;
	if (!pSetAccelerator)
		pSetAccelerator = (void(*)(REALcontrolInstance, REALstring)) CallResolver("REALSetAccelerator");
	if (pSetAccelerator) pSetAccelerator(instance, key);
}
#endif

REALgraphics REALGetControlGraphics(REALcontrolInstance instance)
{
	static REALgraphics(*pGetControlGraphics)(REALcontrolInstance) = nil;
	if (!pGetControlGraphics)
		pGetControlGraphics = (REALgraphics(*)(REALcontrolInstance)) CallResolver("REALGetControlGraphics");
	if (pGetControlGraphics) return pGetControlGraphics(instance);
	else return 0;
}


// Main program
void REALPluginMain(void *(*resolver)(const char *entryName))
{
	void (*pRegisterPluginVersion)(int version);
	
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

void *REALLoadGlobalMethod( const char *, const char *prototype )
{
	// Call through to the real worker function
	return REALLoadFrameworkMethod( prototype );
}

REALstring REALDefaultControlCaption(void)
{
	static REALstring (*pDefaultControlCaption)(void) = nil;
	if (!pDefaultControlCaption)
		pDefaultControlCaption = (REALstring (*)(void)) CallResolver("PluginDefaultControlCaption");
	if (pDefaultControlCaption) return pDefaultControlCaption();
	else return 0;
}

REALobject REALnewInstance(const char *className)
{
	static REALobject (*pNewInstance)(const char *) = nil;
	if (!pNewInstance)
		pNewInstance = (REALobject(*)(const char *)) CallResolver("PluginNewInstance");
	if (pNewInstance) return pNewInstance(className);
	else return 0;
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

void GraphicsDrawLine(REALgraphics graphicsObject, double x1, double y1, double x2, double y2)
{
	if (REALGetRBVersion() < 2018.02) {
		static void(*pGraphicsDrawLine)(REALgraphics, RBInteger, RBInteger, RBInteger, RBInteger) = nil;
		if (!pGraphicsDrawLine)
			pGraphicsDrawLine = (void(*)(REALgraphics, RBInteger, RBInteger, RBInteger, RBInteger)) CallResolver("RuntimeGraphicsDrawLine");
		if (pGraphicsDrawLine) pGraphicsDrawLine(graphicsObject, x1, y1, x2, y2);
	} else {
		static void(*pGraphicsDrawLine)(REALgraphics, double, double, double, double) = nil;
		if (!pGraphicsDrawLine)
			pGraphicsDrawLine = (void(*)(REALgraphics, double, double, double, double)) CallResolver("RuntimeGraphicsDrawLine");
		if (pGraphicsDrawLine) pGraphicsDrawLine(graphicsObject, x1, y1, x2, y2);
	}
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
	static char *(*pCString)(REALstring) = nil;
	if (!pCString)
		pCString = (char *(*)(REALstring)) CallResolver("StringGetCString");
	if (pCString) return (const char *)pCString(str);
	else return (const char *)0;
}

size_t REALStringLength(REALstring str)
{
	static RBInteger (*pLenB)(REALstring);
	if (!pLenB)
		pLenB = (RBInteger (*)(REALstring))REALLoadFrameworkMethod( "LenB( s as string ) as integer" );
	
	if (pLenB) {
		return pLenB(str);
	} else {
		return 0;
	}
}
const unsigned char *REALPString(REALstring str)
{
	static unsigned char *(*pPString)(REALstring) = nil;
	if (!pPString)
		pPString = (unsigned char *(*)(REALstring)) CallResolver("StringGetPString");
	if (pPString) return (const unsigned char *)pPString(str);
	else return (const unsigned char *)0;
}

REALproc REALInterfaceRoutine(REALobject obj, const char *interfaceName, const char *methodName)
{
	static REALproc (*pInterfaceRoutine)(REALobject, const char *, const char *) = nil;
	if (!pInterfaceRoutine)
		pInterfaceRoutine = (REALproc (*)(REALobject, const char *, const char *)) CallResolver("GetInterfaceRoutine");
	if (pInterfaceRoutine) return pInterfaceRoutine(obj, interfaceName, methodName);
	else return 0;
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
	static void(*pDrawPicturePrimitive)(HDC, REALpicture, const Rect *, int) = nil;
	if (!pDrawPicturePrimitive)
		pDrawPicturePrimitive = (void(*)(HDC, REALpicture, const Rect *, int)) CallResolver("drawPicturePrimitive");
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

void REALGetControlBounds(REALcontrolInstance instance, Rect *rBounds)
{
	static void(*pGetControlBounds)(REALcontrolInstance, Rect *) = nil;
	if (!pGetControlBounds)
		pGetControlBounds = (void(*)(REALcontrolInstance, Rect *)) CallResolver("GetControlBounds");
	if (pGetControlBounds) pGetControlBounds(instance, rBounds);
}

bool REALGetControlVisible(REALcontrolInstance instance)
{
	static RBBoolean (*pGetControlVisible)(REALcontrolInstance) = nil;
	if (!pGetControlVisible)
		pGetControlVisible = (RBBoolean(*)(REALcontrolInstance)) CallResolver("GetControlVisible");
	if (pGetControlVisible) return pGetControlVisible(instance);
	else return false;
}

Boolean REALGetControlEnabled(REALcontrolInstance instance)
{
	static RBBoolean (*pGetControlEnabled)(REALcontrolInstance) = nil;
	if (!pGetControlEnabled)
		pGetControlEnabled = (RBBoolean (*)(REALcontrolInstance)) CallResolver("REALGetControlEnabled");
	if (pGetControlEnabled) return pGetControlEnabled(instance);
	else return 0;
}

void REALSetControlVisible(REALcontrolInstance instance, bool visible)
{
	static void (*pSetControlVisible)(REALcontrolInstance, RBBoolean) = nil;
	if (!pSetControlVisible)
		pSetControlVisible = (void(*)(REALcontrolInstance, RBBoolean)) CallResolver("SetControlVisible");
	if (pSetControlVisible) pSetControlVisible(instance, visible);
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
	static REALfolderItem (*pFolderItemFromPath)(const char *) = nil;
	if (!pFolderItemFromPath)
		pFolderItemFromPath = (REALfolderItem(*)(const char *)) CallResolver("FolderItemFromPath");
	if (pFolderItemFromPath) return pFolderItemFromPath(path);
	else return 0;
}
#endif

REALstring REALpathFromFolderItem(REALfolderItem item)
{
	static REALstring (*pPathFromFolderItem)(REALfolderItem) = nil;
	if (!pPathFromFolderItem)
		pPathFromFolderItem = (REALstring(*)(REALfolderItem)) CallResolver("REALpathFromFolderItem");
	if (pPathFromFolderItem) return pPathFromFolderItem(item);
	else return 0;
}

#if TARGET_WIN32
HDC REALGraphicsDC(REALgraphics context)
{
	static HDC (*pGraphicsDC)(REALgraphics) = nil;
	if (!pGraphicsDC)
		pGraphicsDC = (HDC (*)(REALgraphics)) CallResolver("REALGraphicsDC");
	if (pGraphicsDC) return pGraphicsDC(context);
	else return 0;
}
#endif

#if TARGET_WIN32
void REALGraphicsReleaseDC(REALgraphics context)
{
	static void (*pGraphicsReleaseDC)(REALgraphics) = nil;
	if (!pGraphicsReleaseDC)
		pGraphicsReleaseDC = (void (*)(REALgraphics)) CallResolver("REALGraphicsReleaseDC");
	if (pGraphicsReleaseDC) pGraphicsReleaseDC(context);
}
#endif

#if TARGET_COCOA
CGContextRef REALGraphicsDC(REALgraphics context)
{
	static CGContextRef (*pGraphicsCGContext)(REALgraphics) = nil;
	if (!pGraphicsCGContext)
		pGraphicsCGContext = (CGContextRef (*)(REALgraphics)) CallResolver("REALGraphicsDC");
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
	static NSWindow *(*pGetWindowHandle)(REALwindow) = nil;
	if (!pGetWindowHandle)
		pGetWindowHandle = (NSWindow * (*)(REALwindow)) CallResolver("REALGetWindowHandle");
	if (pGetWindowHandle) return pGetWindowHandle(window);
	else return 0;
}
#endif

#if TARGET_COCOA
NSView *REALGetControlHandle(REALcontrolInstance control)
{
	static NSView *(*pGetControlHandle)(REALcontrolInstance) = nil;
	if (!pGetControlHandle)
		pGetControlHandle = (NSView *(*)(REALcontrolInstance)) CallResolver("REALGetControlHandle");
	if (pGetControlHandle) return pGetControlHandle(control);
	else return 0;
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
	static REALstring (*pSocketReadAll)(REALsocket) = nil;
	if (!pSocketReadAll)
		pSocketReadAll = (REALstring(*)(REALsocket)) CallResolver("pluginSocketReadAll");
	if (pSocketReadAll) return pSocketReadAll(socket);
	else return 0;
}

REALstring REALSocketRead(REALsocket socket, int count)
{
	static REALstring(*pSocketRead)(REALsocket, int) = nil;
	if (!pSocketRead)
		pSocketRead = (REALstring(*)(REALsocket, int)) CallResolver("pluginSocketRead");
	if (pSocketRead) return pSocketRead(socket, count);
	else return 0;
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
	static REALstring (*pSocketLookahead)(REALsocket, int) = nil;
	if (!pSocketLookahead)
		pSocketLookahead = (REALstring(*)(REALsocket, int)) CallResolver("pluginSocketLookahead");
	if (pSocketLookahead) return pSocketLookahead(socket, unused);
	else return 0;
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

void REALSetControlEnabled(REALcontrolInstance instance, RBInteger unused, Boolean enable)
{
	static void(*pSetControlEnabled)(REALcontrolInstance, RBInteger, RBBoolean) = nil;
	if (!pSetControlEnabled)
		pSetControlEnabled = (void(*)(REALcontrolInstance, RBInteger, RBBoolean)) CallResolver("controlEnabledSetter");
	if (pSetControlEnabled) pSetControlEnabled(instance, unused, enable);
}

RBInteger REALGetControlPosition(REALcontrolInstance instance, RBInteger which)
{
	static RBInteger (*pGetControlPosition)(REALcontrolInstance, RBInteger) = nil;
	if (!pGetControlPosition)
		pGetControlPosition = (RBInteger(*)(REALcontrolInstance, RBInteger)) CallResolver("ctlPosGetter");
	if (pGetControlPosition) return pGetControlPosition(instance, which);
	else return 0;
}

void REALSetControlPosition(REALcontrolInstance instance, RBInteger which, RBInteger value)
{
	static void(*pSetControlPosition)(REALcontrolInstance, RBInteger, RBInteger) = nil;
	if (!pSetControlPosition)
		pSetControlPosition = (void(*)(REALcontrolInstance, RBInteger, RBInteger)) CallResolver("ctlPosSetter");
	if (pSetControlPosition) pSetControlPosition(instance, which, value);
}

REALgraphics REALGetPictureGraphics(REALpicture picture)
{
	static REALgraphics (*pGetPictureGraphics)(REALpicture) = nil;
	if (!pGetPictureGraphics)
		pGetPictureGraphics = (REALgraphics(*)(REALpicture)) CallResolver("pluginPictureGraphicsGetter");
	if (pGetPictureGraphics) return pGetPictureGraphics(picture);
	else return 0;
}

REALpicture REALNewPicture(RBInteger width, RBInteger height, RBInteger depth)
{
	static REALpicture(*pNewPicture)(RBInteger, RBInteger, RBInteger) = nil;
	if (!pNewPicture)
		pNewPicture = (REALpicture(*)(RBInteger, RBInteger, RBInteger)) CallResolver("newPicture");
	if (pNewPicture) return pNewPicture(width, height, depth);
	else return 0;
}

REALmemoryBlock REALNewMemoryBlock(int bytes)
{
	static REALmemoryBlock (*pNewMemoryBlock)(int) = nil;
	if (!pNewMemoryBlock)
		pNewMemoryBlock = (REALmemoryBlock(*)(int)) CallResolver("newMemoryBlock");
	if (pNewMemoryBlock) return pNewMemoryBlock(bytes);
	else return 0;
}

void*REALMemoryBlockGetPtr(REALmemoryBlock memBlock)
{
	static void *(*pMemoryBlockGetPtr)(REALmemoryBlock) = nil;
	if (!pMemoryBlockGetPtr)
		pMemoryBlockGetPtr = (void *(*)(REALmemoryBlock)) CallResolver("memoryBlockGetPtr");
	if (pMemoryBlockGetPtr) return pMemoryBlockGetPtr(memBlock);
	else return 0;
}

RBInteger REALMemoryBlockGetSize(REALmemoryBlock memBlock)
{
	static RBInteger(*pMemoryBlockGetSize)(REALmemoryBlock) = nil;
	if (!pMemoryBlockGetSize)
		pMemoryBlockGetSize = (RBInteger(*)(REALmemoryBlock)) CallResolver("memoryBlockGetSize");
	if (pMemoryBlockGetSize) return pMemoryBlockGetSize(memBlock);
	else return 0;
}

REALmemoryBlock REALPtrToMemoryBlock(void*data)
{
	static REALmemoryBlock(*pPtrToMemoryBlock)(void*) = nil;
	if (!pPtrToMemoryBlock)
		pPtrToMemoryBlock = (REALmemoryBlock(*)(void*)) CallResolver("PtrToMemoryBlock");
	if (pPtrToMemoryBlock) return pPtrToMemoryBlock(data);
	else return 0;
}

int REALGetArrayInt(REALintArray array, RBInteger index)
{
	static int(*pGetArrayInt)(REALintArray, RBInteger) = nil;
	if (!pGetArrayInt)
		pGetArrayInt = (int(*)(REALintArray, RBInteger)) CallResolver("RuntimeDirectReadIntArray");
	if (pGetArrayInt) return pGetArrayInt(array, index);
	else return 0;
}

REALstring REALGetArrayString(REALstringArray array, RBInteger index)
{
	static REALstring(*pGetArrayString)(REALstringArray, RBInteger) = nil;
	if (!pGetArrayString)
		pGetArrayString = (REALstring(*)(REALstringArray, RBInteger)) CallResolver("RuntimeDirectReadStringArray");
	if (pGetArrayString) return pGetArrayString(array, index);
	else return NULL;
}

REALobject REALGetArrayObject(REALobjectArray array, RBInteger index)
{
	static REALobject(*pGetArrayObject)(REALobjectArray, RBInteger) = nil;
	if (!pGetArrayObject)
		pGetArrayObject = (REALobject(*)(REALobjectArray, RBInteger)) CallResolver("RuntimeDirectReadObjectArray");
	if (pGetArrayObject) return pGetArrayObject(array, index);
	else return NULL;
}

Boolean REALGetTabPanelVisible(REALcontrolInstance instance)
{
	static RBBoolean (*pGetTabPanelVisible)(REALcontrolInstance) = nil;
	if (!pGetTabPanelVisible)
		pGetTabPanelVisible = (RBBoolean(*)(REALcontrolInstance)) CallResolver("GetTabPanelVisible");
	if (pGetTabPanelVisible) return pGetTabPanelVisible(instance);
	else return 0;
}

#if TARGET_WIN32
HWND REALGetControlHandle(REALcontrolInstance control)
{
	static HWND (*pGetControlHandle)(REALcontrolInstance) = nil;
	if (!pGetControlHandle)
		pGetControlHandle = (HWND(*)(REALcontrolInstance)) CallResolver("REALGetControlHandle");
	if (pGetControlHandle) return pGetControlHandle(control);
	else return 0;
}
#endif

#if TARGET_WIN32
HWND REALGetWindowHandle(REALwindow window)
{
	static HWND (*pGetWindowHandle)(REALwindow) = nil;
	if (!pGetWindowHandle)
		pGetWindowHandle = (HWND(*)(REALwindow)) CallResolver("REALGetWindowHandle");
	if (pGetWindowHandle) return pGetWindowHandle(window);
	else return 0;
}
#endif

Boolean REALGetControlFocus(REALcontrolInstance instance)
{
	static RBBoolean (*pGetControlFocus)(REALcontrolInstance) = nil;
	if (!pGetControlFocus)
		pGetControlFocus = (RBBoolean(*)(REALcontrolInstance)) CallResolver("REALGetControlFocus");
	if (pGetControlFocus) return pGetControlFocus(instance);
	else return 0;
}

void REALSetControlFocus(REALcontrolInstance instance, Boolean focus)
{
	static void (*pSetControlFocus)(REALcontrolInstance, RBBoolean) = nil;
	if (!pSetControlFocus)
		pSetControlFocus = (void (*)(REALcontrolInstance, RBBoolean)) CallResolver("REALSetControlFocus");
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

REALstring REALGetControlName(REALcontrolInstance control)
{
	static REALstring (*pGetControlName)(REALcontrolInstance) = nil;
	if (!pGetControlName)
		pGetControlName = (REALstring (*)(REALcontrolInstance)) CallResolver("REALGetControlName");
	if (pGetControlName) return pGetControlName(control);
	else return (REALstring)0;
}

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
uint32_t REALGetFontEncoding(const char *fontName)
{
	static uint32_t (*pGetFontEncoding)(const char *) = nil;
	if (!pGetFontEncoding)
		pGetFontEncoding = (uint32_t(*)(const char *)) CallResolver("REALGetFontEncoding");
	if (pGetFontEncoding) return pGetFontEncoding(fontName);
	else return 0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REALpicture REALGetPictureMask(REALpicture pict, Boolean createIfNil)
{
	static REALpicture (*pGetPictureMask)(REALpicture, RBBoolean) = nil;
	if (!pGetPictureMask)
		pGetPictureMask = (REALpicture (*)(REALpicture, RBBoolean)) CallResolver("REALGetPictureMask");
	if (pGetPictureMask) return pGetPictureMask(pict, createIfNil);
	else return (REALpicture)0;
}
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
void REALGraphicsDrawString(REALgraphics graphics, REALstring str, RBInteger x, RBInteger y, RBInteger width)
{
	if (REALGetRBVersion() < 2018.02) {
		static void(*pGraphicsDrawString)(REALgraphics, REALstring, RBInteger, RBInteger, RBInteger) = nil;
		if (!pGraphicsDrawString)
			pGraphicsDrawString = (void(*)(REALgraphics, REALstring, RBInteger, RBInteger, RBInteger)) CallResolver("RuntimeGraphicsDrawString");
		if (pGraphicsDrawString) pGraphicsDrawString(graphics, str, x, y, width);
	} else {
		static void(*pGraphicsDrawString)(REALgraphics, REALstring, double, double, double) = nil;
		if (!pGraphicsDrawString)
			pGraphicsDrawString = (void(*)(REALgraphics, REALstring, double, double, double)) CallResolver("RuntimeGraphicsDrawString");
		if (pGraphicsDrawString) pGraphicsDrawString(graphics, str, x, y, width);

	}
}
#endif

#if TARGET_CARBON || TARGET_COCOA
CFStringRef REALGetStringCFString(REALstring str, Boolean stripAmpersands)
{
	static CFStringRef (*pGetStringCFString)(REALstring, RBBoolean) = nil;
	if (!pGetStringCFString)
		pGetStringCFString = (CFStringRef (*)(REALstring, RBBoolean)) CallResolver("REALGetStringCFString");
	if (pGetStringCFString) return pGetStringCFString(str, stripAmpersands);
	else return (CFStringRef)0;
}
#endif

#if TARGET_CARBON || TARGET_COCOA
void REALGetStringSystemStr(REALstring str, Boolean stripAmpersands, Str255 outStr255)
{
	static void (*pGetStringSystemStr)(REALstring, RBBoolean, Str255) = nil;
	if (!pGetStringSystemStr)
		pGetStringSystemStr = (void (*)(REALstring, RBBoolean, Str255)) CallResolver("REALGetStringSystemStr");
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

double REALGraphicsStringWidth(REALgraphics graphics, REALstring str)
{
	if (REALGetRBVersion() < 2007.02) {
		static RBInteger(*pGraphicsStringWidth)(REALgraphics, REALstring) = nil;
		if (!pGraphicsStringWidth)
			pGraphicsStringWidth = (RBInteger(*)(REALgraphics, REALstring)) CallResolver("RuntimeGraphicsStringWidth");
		if (pGraphicsStringWidth) return pGraphicsStringWidth(graphics, str);
		else return 0;
	} else {
		static double(*pGraphicsStringWidth)(REALgraphics, REALstring) = nil;
		if (!pGraphicsStringWidth)
			pGraphicsStringWidth = (double(*)(REALgraphics, REALstring)) CallResolver("RuntimeGraphicsStringWidth");
		if (pGraphicsStringWidth) return pGraphicsStringWidth(graphics, str);
		else return 0;
	}
}

double REALGraphicsStringHeight(REALgraphics graphics, REALstring str, double wrapWidth)
{
	if (REALGetRBVersion() < 2018.02) {
		static RBInteger(*pGraphicsStringHeight)(REALgraphics, REALstring, RBInteger) = nil;
		if (!pGraphicsStringHeight)
			pGraphicsStringHeight = (RBInteger(*)(REALgraphics, REALstring, RBInteger)) CallResolver("RuntimeGraphicsStringHeight");
		if (pGraphicsStringHeight) return pGraphicsStringHeight(graphics, str, wrapWidth);
		else return 0;
	} else {
		static double(*pGraphicsStringHeight)(REALgraphics, REALstring, double) = nil;
		if (!pGraphicsStringHeight)
			pGraphicsStringHeight = (double(*)(REALgraphics, REALstring, double)) CallResolver("RuntimeGraphicsStringHeight");
		if (pGraphicsStringHeight) return pGraphicsStringHeight(graphics, str, wrapWidth);
		else return 0;
	}
}

double REALGraphicsTextHeight(REALgraphics graphics)
{
	if (REALGetRBVersion() < 2018.02) {
		static RBInteger(*pGraphicsTextHeight)(REALgraphics) = nil;
		if (!pGraphicsTextHeight)
			pGraphicsTextHeight = (RBInteger(*)(REALgraphics)) CallResolver("RuntimeGraphicsTextHeight");
		if (pGraphicsTextHeight) return pGraphicsTextHeight(graphics);
		else return 0;
	} else {
		static double(*pGraphicsTextHeight)(REALgraphics) = nil;
		if (!pGraphicsTextHeight)
			pGraphicsTextHeight = (double(*)(REALgraphics)) CallResolver("RuntimeGraphicsTextHeight");
		if (pGraphicsTextHeight) return pGraphicsTextHeight(graphics);
		else return 0;
	}
}

double REALGraphicsTextAscent(REALgraphics graphics)
{
	if (REALGetRBVersion() < 2018.02) {
		static RBInteger(*pGraphicsTextAscent)(REALgraphics) = nil;
		if (!pGraphicsTextAscent)
			pGraphicsTextAscent = (RBInteger(*)(REALgraphics)) CallResolver("RuntimeGraphicsTextAscent");
		if (pGraphicsTextAscent) return pGraphicsTextAscent(graphics);
		else return 0;
	} else {
		static double(*pGraphicsTextAscent)(REALgraphics) = nil;
		if (!pGraphicsTextAscent)
			pGraphicsTextAscent = (double(*)(REALgraphics)) CallResolver("RuntimeGraphicsTextAscent");
		if (pGraphicsTextAscent) return pGraphicsTextAscent(graphics);
		else return 0;
	}
}

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
REALgraphics REALGetControlGraphicsWithDC(REALcontrolInstance instance, HDC dc)
{
	static REALgraphics (*pGetControlGraphics)(REALcontrolInstance, HDC) = nil;
	if (!pGetControlGraphics)
		pGetControlGraphics = (REALgraphics (*)(REALcontrolInstance, HDC)) CallResolver("REALGetControlGraphicsWithDC");
	if (pGetControlGraphics) return pGetControlGraphics(instance, dc);
	else return 0;
}

REALgraphics REALGetControlGraphics(REALcontrolInstance instance, HDC dc)
{
	return REALGetControlGraphicsWithDC(instance, dc);
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

#if !TARGET_64BIT
Boolean REALGetPropValue(REALobject object, const char *propName, char *outValue)
{
	return REALGetPropValueInt8(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, unsigned char *outValue)
{
	return REALGetPropValueUInt8(object, propName, outValue);
}
#endif

Boolean REALGetPropValue(REALobject object, const char *propName, short *outValue)
{
	return REALGetPropValueInt16(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, unsigned short *outValue)
{
	return REALGetPropValueUInt16(object, propName, outValue);
}

#if !TARGET_64BIT
Boolean REALGetPropValue(REALobject object, const char *propName, int32_t *outValue)
{
	return REALGetPropValueInt32(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, long *outValue)
{
	int32_t value;
	Boolean result = REALGetPropValueInt32(object, propName, &value);
	if (result) *outValue = value;
	return result;
}

Boolean REALGetPropValue(REALobject object, const char *propName, unsigned long *outValue)
{
	uint32_t value32 = 0;
	Boolean result = REALGetPropValueUInt32(object, propName, &value32);
	if (result) *outValue = value32;
	return result;
}

Boolean REALGetPropValue(REALobject object, const char *propName, RBInt64 *outValue)
{
	return REALGetPropValueInt64(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, RBUInt64 *outValue)
{
	return REALGetPropValueUInt64(object, propName, outValue);
}
#endif

Boolean REALGetPropValue(REALobject object, const char *propName, double *outValue)
{
	return REALGetPropValueDouble(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, float *outValue)
{
	return REALGetPropValueSingle(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, REALstring *outValue)
{
	return REALGetPropValueString(object, propName, outValue);
}

Boolean REALGetPropValue(REALobject object, const char *propName, REALobject *outValue)
{
	return REALGetPropValueObject(object, propName, outValue);
}

#if TARGET_CARBON || TARGET_COCOA
Boolean REALGetPropValue(REALobject object, const char *propName, CFStringRef *outValue)
{
	return REALGetPropValueCFStringRef(object, propName, outValue);
}
#endif

Boolean REALSetPropValue(REALobject object, const char *propName, REALstring value)
{
	return REALSetPropValueString(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, double value)
{
	return REALSetPropValueDouble(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, REALobject value)
{
	return REALSetPropValuePtr(object, propName, value);
}

#if !TARGET_64BIT
Boolean REALSetPropValue(REALobject object, const char *propName, unsigned char value)
{
	return REALSetPropValueUInt8(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, char value)
{
	return REALSetPropValueInt8(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, unsigned long value)
{
	return REALSetPropValueUInt32(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, long value)
{
	return REALSetPropValueInt32(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, RBUInt64 value)
{
	return REALSetPropValueUInt64(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, RBInt64 value)
{
	return REALSetPropValueInt64(object, propName, value);
}
#endif

Boolean REALSetPropValue(REALobject object, const char *propName, unsigned short value)
{
	return REALSetPropValueUInt16(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, short value)
{
	return REALSetPropValueInt16(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, float value)
{
	return REALSetPropValueSingle(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, const char *value)
{
	return REALSetPropValueCString(object, propName, value);
}

Boolean REALSetPropValue(REALobject object, const char *propName, const wchar_t *value)
{
	return REALSetPropValueWString(object, propName, value);
}

#if !TARGET_64BIT
Boolean REALSetPropValue(REALobject object, const char *propName, const unsigned char *value)
{
	return REALSetPropValuePString(object, propName, value);
}
#endif

#if TARGET_CARBON || TARGET_COCOA
Boolean REALSetPropValue(REALobject object, const char *propName, CFStringRef value)
{
	return REALSetPropValueCFStringRef(object, propName, value);
}
#endif

#if !TARGET_64BIT
void REALInsertArrayValue(REALarray arr, RBInteger index, RBInt64 value)
{
	REALInsertArrayValueInt64(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, RBUInt64 value)
{
	REALInsertArrayValueUInt64(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, long value)
{
	REALInsertArrayValueInt32(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, unsigned long value)
{
	REALInsertArrayValueUInt32(arr, index, value);
}
#endif

void REALInsertArrayValue(REALarray arr, RBInteger index, short value)
{
	REALInsertArrayValueInt16(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, unsigned short value)
{
	REALInsertArrayValueUInt16(arr, index, value);
}

#if !TARGET_64BIT
void REALInsertArrayValue(REALarray arr, RBInteger index, char value)
{
	REALInsertArrayValueInt8(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, unsigned char value)
{
	REALInsertArrayValueUInt8(arr, index, value);
}
#endif

void REALInsertArrayValue(REALarray arr, RBInteger index, double value)
{
	REALInsertArrayValueDouble(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, float value)
{
	REALInsertArrayValueSingle(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, REALstring value)
{
	REALInsertArrayValueString(arr, index, value);
}

void REALInsertArrayValue(REALarray arr, RBInteger index, REALobject value)
{
	REALInsertArrayValueObject(arr, index, value);
}

#if !TARGET_64BIT
void REALGetArrayValue(REALarray arr, RBInteger index, char *value)
{
	REALGetArrayValueInt8(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, unsigned char *value)
{
	REALGetArrayValueUInt8(arr, index, value);
}
#endif

void REALGetArrayValue(REALarray arr, RBInteger index, short *value)
{
	REALGetArrayValueInt16(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, unsigned short *value)
{
	REALGetArrayValueUInt16(arr, index, value);
}

#if !TARGET_64BIT
void REALGetArrayValue(REALarray arr, RBInteger index, long *value)
{
	int32_t value32 = 0;
	REALGetArrayValueInt32(arr, index, &value32);
	*value = value32;
}

void REALGetArrayValue(REALarray arr, RBInteger index, unsigned long *value)
{
	uint32_t value32 = 0;
	REALGetArrayValueUInt32(arr, index, &value32);
	*value = value32;
}

void REALGetArrayValue(REALarray arr, RBInteger index, RBInt64 *value)
{
	REALGetArrayValueInt64(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, RBUInt64 *value)
{
	REALGetArrayValueUInt64(arr, index, value);
}
#endif

void REALGetArrayValue(REALarray arr, RBInteger index, REALstring *value)
{
	REALGetArrayValueString(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, REALtext *value)
{
	REALGetArrayValueText(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, REALobject *value)
{
	REALGetArrayValueObject(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, double *value)
{
	REALGetArrayValueDouble(arr, index, value);
}

void REALGetArrayValue(REALarray arr, RBInteger index, float *value)
{
	REALGetArrayValueSingle(arr, index, value);
}

#if !TARGET_64BIT
void REALSetArrayValue(REALarray arr, RBInteger index, char value)
{
	REALSetArrayValueInt8(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, unsigned char value)
{
	REALSetArrayValueUInt8(arr, index, value);
}
#endif

void REALSetArrayValue(REALarray arr, RBInteger index, short value)
{
	REALSetArrayValueInt16(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, unsigned short value)
{
	REALSetArrayValueUInt16(arr, index, value);
}

#if !TARGET_64BIT
void REALSetArrayValue(REALarray arr, RBInteger index, long value)
{
	REALSetArrayValueInt32(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, unsigned long value)
{
	REALSetArrayValueUInt32(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, RBInt64 value)
{
	REALSetArrayValueInt64(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, RBUInt64 value)
{
	REALSetArrayValueUInt64(arr, index, value);
}
#endif

void REALSetArrayValue(REALarray arr, RBInteger index, double value)
{
	REALSetArrayValueDouble(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, float value)
{
	REALSetArrayValueSingle(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, REALstring value)
{
	REALSetArrayValueString(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, REALtext value)
{
	REALSetArrayValueText(arr, index, value);
}

void REALSetArrayValue(REALarray arr, RBInteger index, REALobject value)
{
	REALSetArrayValueObject(arr, index, value);
}

RBInteger REALGetPictureType(REALpicture pic)
{
	static RBInteger (*pGetPictureType)(REALpicture);
	if (!pGetPictureType)
		pGetPictureType = (RBInteger(*)(REALpicture))CallResolver("PluginPictureGetType");
	if (pGetPictureType) return pGetPictureType(pic);
	return 2; // Picture.Types.MutableBitmap
}

RBInteger REALCountClassProperties(REALobject obj)
{
	static void *(*pRBScriptResolver)(const char *entryName) = (void *(*)(const char *))CallResolver("GetRBScriptResolver");
	if (!pRBScriptResolver) return 0;

	static RBInteger(*pCountClassProperties)(void *);
	static void *(*pRuntimeClassOf)(REALobject);
	if (!pCountClassProperties && !pRuntimeClassOf) {
		pCountClassProperties = (RBInteger(*)(void *))pRBScriptResolver("REALCountClassProperties");
		pRuntimeClassOf = (void *(*)(REALobject))pRBScriptResolver("RuntimeClassOf");
	}
	if (pCountClassProperties && pRuntimeClassOf) {
		void *classPtr = pRuntimeClassOf(obj);
		if (!classPtr) return 0;

		return pCountClassProperties(classPtr);
	}
	return 0;
}

RBBoolean REALGetClassProperty(REALobject obj, uint32_t index, void **getter, void **setter, long *param, REALstring *declaration)
{
	static void *(*pRBScriptResolver)(const char *entryName) = (void *(*)(const char *))CallResolver("GetRBScriptResolver");
	if (!pRBScriptResolver) return 0;

	static RBBoolean(*pGetClassProperty)(void *, uint32_t, void **, void**, long *, REALstring *);
	static void *(*pRuntimeClassOf)(REALobject);
	if (!pGetClassProperty && !pRuntimeClassOf) {
		pGetClassProperty = (RBBoolean(*)(void *, uint32_t, void **, void**, long *, REALstring *))pRBScriptResolver("REALGetClassProperty");
		pRuntimeClassOf = (void *(*)(REALobject))pRBScriptResolver("RuntimeClassOf");
	}
	if (pGetClassProperty && pRuntimeClassOf) {
		void *classPtr = pRuntimeClassOf(obj);
		if (!classPtr) return 0;

		return pGetClassProperty(classPtr, index, getter, setter, param, declaration);
	}
	return false;
}

RBBoolean REALAddEventHandler(REALobject obj, REALstring eventName, void *handler)
{
	static RBBoolean(*pAddEventHandler)(REALobject, REALstring, void*);
	if (!pAddEventHandler) {
		pAddEventHandler = (RBBoolean(*)(REALobject, REALstring, void*))CallResolver("PluginAddEventHandler");
	}
	if (pAddEventHandler) return pAddEventHandler(obj, eventName, handler);
	return false;
}

RBBoolean REALRemoveEventHandler(REALobject obj, REALstring eventName, void *handler)
{
	static RBBoolean(*pRemoveEventHandler)(REALobject, REALstring, void*);
	if (!pRemoveEventHandler) {
		pRemoveEventHandler = (RBBoolean(*)(REALobject, REALstring, void*))CallResolver("PluginRemoveEventHandler");
	}
	if (pRemoveEventHandler) return pRemoveEventHandler(obj, eventName, handler);
	return false;
}

RBBoolean REALIsEventHandled(REALobject obj, REALstring eventName)
{
	static RBBoolean(*pIsEventHandled)(REALobject, REALstring);
	if (!pIsEventHandled) {
		pIsEventHandled = (RBBoolean(*)(REALobject, REALstring))CallResolver("PluginIsEventHandled");
	}
	if (pIsEventHandled) return pIsEventHandled(obj, eventName);
	return false;
}

RBBoolean REALIsDarkMode()
{
	static RBBoolean(*pIsDarkMode)(void);
	if (!pIsDarkMode) {
		pIsDarkMode = (RBBoolean(*)(void))CallResolver("IsDarkMode");
	}
	if (pIsDarkMode) return pIsDarkMode();
	return false;
}

RBBoolean REALIsXojoThread()
{
	static RBBoolean(*pIsXojoThread)(void);
	if (!pIsXojoThread) {
		pIsXojoThread = (RBBoolean(*)(void))CallResolver("IsXojoThread");
	}
	if (pIsXojoThread) return pIsXojoThread();
	return false;
}

RBBoolean REALIsXojoMainThread()
{
	static RBBoolean(*pIsXojoMainThread)(void);
	if (!pIsXojoMainThread) {
		pIsXojoMainThread = (RBBoolean(*)(void))CallResolver("IsXojoMainThread");
	}
	if (pIsXojoMainThread) return pIsXojoMainThread();
	return false;
}

#if _MSC_VER
	#pragma warning( default: 4995 )
#endif
