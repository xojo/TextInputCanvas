/** @file rb_plugin.h
 *  @brief This file contains functions to register data structures and functions to communicate with the Xojo framework.
 *	Include this file at the top of your plugin source files
 *	@copyright Xojo, Inc. -- All Rights Reserved.
 */

#ifndef RB_PLUGIN_H
#define RB_PLUGIN_H

#include "REALplugin.h"
#if defined( __cplusplus )
	#include "rb_plugin_cpp.h"
#endif

// This will be used in stage 2 of phasing out the 
// deprecated functionality.  It's not used now because
// it would require users to have to modify existing 
// projects so they compile.
#include "rb_plugin_deprecated.h"
#include <stdbool.h>

#if defined( __cplusplus )
extern "C" {
#endif

/**
* @brief Accesses the data of a string.
*
* This retrieves the contents of the string in a known encoding. If it is not
* possible to convert the string, it will return false.
*
* This function is more efficient than REALGetStringContents in the typical use
* case where a plugin needs the string's data in a known encoding.
*
* You are required to dispose of this string using REALDisposeStringContents when
* you are done with it.
*
* @note The behavior of this function changed in 2014r2.  Previously it would
*       return false given a NULL string.
* @param str The REALbasic string.
* @param encoding A REALbasic encoding identifier.
* @param outData The REALstringData structure to fill. Must not be NULL.
* @result Whether or not the data could be read.
* @version 2012r1
*/
bool REALGetStringData(REALstring str, uint32_t encoding, REALstringData *outData) RB_WARN_UNUSED_RESULT RB_NON_NULL(3);

/**
* Diposes of the contents of a REALstringData.
*
* @param data The string data. Must not be NULL.
* @version 2012r1
*/
void REALDisposeStringData(REALstringData *data) RB_NON_NULL(1);

bool REALGetPropValueCurrency(REALobject object, const char *propName, REALcurrency *outValue);
bool REALGetPropValueString(REALobject object, const char *propName, REALstring *outValue);
bool REALGetPropValueObject(REALobject object, const char *propName, REALobject *outValue);
bool REALGetPropValueColor(REALobject object, const char *name, RBColor *outValue);
bool REALGetPropValueInteger(REALobject object, const char *propName, RBInteger *outValue);
bool REALGetPropValueUInteger(REALobject object, const char *propName, RBUInteger *outValue);
bool REALGetPropValueInt8(REALobject object, const char *name, char *value);
bool REALGetPropValueUInt8(REALobject object, const char *name, unsigned char *value);
bool REALGetPropValueInt16(REALobject object, const char *name, int16_t *value);
bool REALGetPropValueUInt16(REALobject object, const char *name, uint16_t *value);
bool REALGetPropValueInt32(REALobject object, const char *propName, int32_t *outValue);
bool REALGetPropValueUInt32(REALobject object, const char *name, uint32_t *value);
bool REALGetPropValueInt64(REALobject object, const char *name, RBInt64 *value);
bool REALGetPropValueUInt64(REALobject object, const char *name, RBUInt64 *value);
bool REALGetPropValueDouble(REALobject object, const char *propName, double *outValue);
bool REALGetPropValueSingle(REALobject object, const char *name, float *value);
bool REALGetPropValuePtr(REALobject object, const char *propName, void **outValue);
bool REALGetPropValueCString(REALobject object, const char *propName, const char **outValue);
bool REALGetPropValueWString(REALobject object, const char *propName, const wchar_t **outValue);
bool REALGetPropValuePString(REALobject object, const char *propName, const unsigned char **outValue);
bool REALGetPropValueBoolean(REALobject object, const char *propName, bool *outValue);
#if TARGET_CARBON || TARGET_COCOA
bool REALGetPropValueCFStringRef(REALobject object, const char *propName, CFStringRef *outValue);
#endif

bool REALSetPropValueCurrency(REALobject object, const char *propName, REALcurrency value);
bool REALSetPropValueInt32(REALobject object, const char *propName, int32_t value);
bool REALSetPropValueString(REALobject object, const char *propName, REALstring value);
bool REALSetPropValueDouble(REALobject object, const char *propName, double value);
bool REALSetPropValueObject(REALobject object, const char *propName, REALobject value);
bool REALSetPropValueBoolean(REALobject object, const char *propName, bool value);
bool REALSetPropValueColor(REALobject object, const char *name, RBColor value);
bool REALSetPropValueInteger(REALobject object, const char *propName, RBInteger value);
bool REALSetPropValueUInteger(REALobject object, const char *propName, RBUInteger value);
bool REALSetPropValueUInt32(REALobject object, const char *propName, uint32_t value);
bool REALSetPropValueUInt64(REALobject object, const char *propName, RBUInt64 value);
bool REALSetPropValueInt64(REALobject object, const char *propName, RBInt64 value);
bool REALSetPropValueInt16(REALobject object, const char *propName, int16_t value);
bool REALSetPropValueUInt16(REALobject object, const char *propName, uint16_t value);
bool REALSetPropValueInt8(REALobject object, const char *propName, char value);
bool REALSetPropValueUInt8(REALobject object, const char *propName, unsigned char value);
bool REALSetPropValueSingle(REALobject object, const char *propName, float value);
bool REALSetPropValuePtr(REALobject object, const char *propName, void *value);
bool REALSetPropValueCString(REALobject object, const char *propName, const char *value);
bool REALSetPropValueWString(REALobject object, const char *propName, const wchar_t *value);
bool REALSetPropValuePString(REALobject object, const char *propName, const unsigned char *value);
#if TARGET_CARBON || TARGET_COCOA
bool REALSetPropValueCFStringRef(REALobject object, const char *propName, CFStringRef value);
#endif

void REALInsertArrayValueInt64(REALarray arr, RBInteger index, RBInt64 value);
void REALInsertArrayValueInt32(REALarray arr, RBInteger index, int32_t value);
void REALInsertArrayValueInt16(REALarray arr, RBInteger index, int16_t value);
void REALInsertArrayValueInt8(REALarray arr, RBInteger index, char value);
void REALInsertArrayValueUInt64(REALarray arr, RBInteger index, RBUInt64 value);
void REALInsertArrayValueUInt32(REALarray arr, RBInteger index, uint32_t value);
void REALInsertArrayValueUInt16(REALarray arr, RBInteger index, uint16_t value);
void REALInsertArrayValueUInt8(REALarray arr, RBInteger index, unsigned char value);
void REALInsertArrayValueSingle(REALarray arr, RBInteger index, float value);
void REALInsertArrayValueDouble(REALarray arr, RBInteger index, double value);
void REALInsertArrayValueBoolean(REALarray arr, RBInteger index, bool value);
void REALInsertArrayValueObject(REALarray arr, RBInteger index, REALobject value);
void REALInsertArrayValueString(REALarray arr, RBInteger index, REALstring value);
void REALInsertArrayValueText(REALarray arr, RBInteger index, REALtext value);
void REALInsertArrayValueColor(REALarray arr, RBInteger index, RBColor value);
void REALInsertArrayValueInteger(REALarray arr, RBInteger index, RBInteger value);
void REALInsertArrayValueUInteger(REALarray arr, RBInteger index, RBUInteger value);
void REALInsertArrayValueCurrency(REALarray arr, RBInteger index, REALcurrency value);

void REALGetArrayValueInt64(REALarray arr, RBInteger index, RBInt64 *value);
void REALGetArrayValueInt32(REALarray arr, RBInteger index, int32_t *value);
void REALGetArrayValueInt16(REALarray arr, RBInteger index, int16_t *value);
void REALGetArrayValueInt8(REALarray arr, RBInteger index, char *value);
void REALGetArrayValueUInt64(REALarray arr, RBInteger index, RBUInt64 *value);
void REALGetArrayValueUInt32(REALarray arr, RBInteger index, uint32_t *value);
void REALGetArrayValueUInt16(REALarray arr, RBInteger index, uint16_t *value);
void REALGetArrayValueUInt8(REALarray arr, RBInteger index, unsigned char *value);
void REALGetArrayValueSingle(REALarray arr, RBInteger index, float *value);
void REALGetArrayValueDouble(REALarray arr, RBInteger index, double *value);
void REALGetArrayValueBoolean(REALarray arr, RBInteger index, bool *value);
void REALGetArrayValueObject(REALarray arr, RBInteger index, REALobject *value);
void REALGetArrayValueString(REALarray arr, RBInteger index, REALstring *value);
void REALGetArrayValueText(REALarray arr, RBInteger index, REALtext *value);
void REALGetArrayValueColor(REALarray arr, RBInteger index, RBColor *value);
void REALGetArrayValueInteger(REALarray arr, RBInteger index, RBInteger *value);
void REALGetArrayValueUInteger(REALarray arr, RBInteger index, RBUInteger *value);
void REALGetArrayValueCurrency(REALarray arr, RBInteger index, REALcurrency *value);

void REALSetArrayValueInt64(REALarray arr, RBInteger index, RBInt64 value);
void REALSetArrayValueInt32(REALarray arr, RBInteger index, int32_t value);
void REALSetArrayValueInt16(REALarray arr, RBInteger index, int16_t value);
void REALSetArrayValueInt8(REALarray arr, RBInteger index, char value);
void REALSetArrayValueUInt64(REALarray arr, RBInteger index, RBUInt64 value);
void REALSetArrayValueUInt32(REALarray arr, RBInteger index, uint32_t value);
void REALSetArrayValueUInt16(REALarray arr, RBInteger index, uint16_t value);
void REALSetArrayValueUInt8(REALarray arr, RBInteger index, unsigned char value);
void REALSetArrayValueSingle(REALarray arr, RBInteger index, float value);
void REALSetArrayValueDouble(REALarray arr, RBInteger index, double value);
void REALSetArrayValueBoolean(REALarray arr, RBInteger index, bool value);
void REALSetArrayValueObject(REALarray arr, RBInteger index, REALobject value);
void REALSetArrayValueString(REALarray arr, RBInteger index, REALstring value);
void REALSetArrayValueText(REALarray arr, RBInteger index, REALtext value);
void REALSetArrayValueColor(REALarray arr, RBInteger index, RBColor value);
void REALSetArrayValueInteger(REALarray arr, RBInteger index, RBInteger value);
void REALSetArrayValueUInteger(REALarray arr, RBInteger index, RBUInteger value);
void REALSetArrayValueCurrency(REALarray arr, RBInteger index, REALcurrency value);

/**
 * Converts a string to a CFString.
 *
 * @param str A REALbasic string or NULL.
 * @return A CFString object that you are responsible for calling CFRelease on.
 *         If the string could not be converted, this will return NULL.
 * @version 2011r4
 */
#if TARGET_CARBON || TARGET_COCOA
	CFStringRef REALCopyStringCFString( REALstring str ) RB_WARN_UNUSED_RESULT;
#endif

/**
 * Access data of an object defined in a different plugin.
 *
 * @param obj The object to get data from.
 * @param classRef The class you are interested in.
 * @return The data pointer or NULL if classRef is not from a plugin.
 * @version 2012r1
 */
void * REALGetPluginData( REALobject obj, REALclassRef classRef );

/**
 * Determines if an object is an instance of a given class.
 *
 * This is equivalent to the IsA operator in REALbasic.
 *
 * @param obj The object to check.
 * @param classRef The class to check with.
 * @result Whether or not the object is an instance of the class.
 * @version 2011r4
 */
bool REALObjectIsA( REALobject obj, REALclassRef classRef );

/**
 * Calls an arbitrary function, wrapped in a REALbasic exception handler.
 *
 * @param callback The function to call.
 * @param userData Arbitrary data to pass to the callback when it is invoked.
 * @result The exception object, if one is raised. The caller is responsible for
 *         unlocking this object.
 * @version 2012r1
 */
REALobject REALCallFunctionWithExceptionHandler( void (*callback)(void*), void *userData ) RB_WARN_UNUSED_RESULT RB_NON_NULL(1);

/**
 * Compares two strings as if one of the equality operators had been used in
 * REALbasic code.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @result The result.
 * @version 2011r4
 */
RBInteger REALCompareStrings( REALstring str1, REALstring str2 );

/**
 * Concatenates two strings.
 *
 * @param str1 The first string.
 * @param str2 The second string.
 * @result The resulting string.
 * @version 2011r4
 */
REALstring REALAddStrings( REALstring str1, REALstring str2 );

/**
 * Gets the function pointer needed to call the delegate.
 *
 * @param delegate The delegate object.
 * @result The function pointer or NULL on error.
 * @version 2013r1
 *
 * @note The function pointer returned requires passing in the delegate as the
 *       first parameter. For example, if the delegate type were declared as
 *       "Test( i as integer ) as string", the C/C++ function signature would
 *       look like "REALstring (*)(REALobject, RBInteger)".
 */
void * REALGetDelegateInvoker( REALobject delegate );

/**
 * Increments reference count on the Text object
 *
 * @param value The Text object to lock
 * @version 2015r1
 */
void REALLockText(REALtext value);

/**
 * Decrements reference count on the Text object
 *
 * @param value The Text object to unlock
 * @version 2015r1
 */
void REALUnlockText(REALtext value);

/**
 * Builds a Text object from the supplied data
 *
 * @param data The string data
 * @param size The size, in bytes, of the string data.
 *             Not including the NULL terminator
 * @param encodingName The IANA encoding name of the string data
 * @return Text object, or nullptr on error/invalid data
 * @version 2015r1
 */
REALtext REALBuildText(const void *data,
	size_t size,
	const char *encodingName) RB_WARN_UNUSED_RESULT;

/**
 * Compares two Text objects
 *
 * @param value1 Text to compare with
 * @param value2 Text to compare against
 * @param options Can be 0 case insensitve, or 1 case sensitive
 * @return Positive value if value1 > value2, or 0 if value1 = value2,
 *			or negative if value1 < value2
 * @version 2015r1
 */
RBInteger REALCompareText(REALtext value1, REALtext value2, RBInteger options);

/**
 * Compares two Text objects
 *
 * @param value1 Text to concat to
 * @param value2 Text to concat with
 * @return Concatenated Text object
 * @version 2015r1
 */
REALtext REALConcatenateText(REALtext value1, REALtext value2) RB_WARN_UNUSED_RESULT;

typedef struct {
	const void *data;
	size_t size;
	const char *encoding;
} REALtextData;

/**
 * Converts the Text object to a more useful data form
 *
 * @param value Text object
 * @param encodingName The desired encoding
 * @param allowLossyConversion true to allow lossy conversion, false otherwise
 * @return The convert Text stored in a REALtextData object, or nullptr on error
 * @version 2015r1
 */
REALtextData * REALGetTextData(REALtext value,
	const char *encodingName,
	bool allowLossyConversion) RB_WARN_UNUSED_RESULT RB_NON_NULL(2);

/**
 * Frees memory allocated by REALGetTextData
 *
 * @param value REALtextData to dispose of
 * @version 2015r1
 */
void REALDisposeTextData(REALtextData *value);

/**
 * Copies the Text value to a CFStringRef
 *
 * @param value Text to copy
 * @return A copy of the Text data as a CFStringRef
 * @version 2015r1
 */
#if defined(TARGET_OS_MAC) && TARGET_OS_MAC
CFStringRef REALCopyTextCFString(REALtext value);
#endif

typedef void (*BackgroundTaskProc)(void *data);
int32_t REALRegisterBackgroundTask( BackgroundTaskProc proc, uint32_t period, void *data );
void REALUnregisterBackgroundTask( int32_t id );

bool REALinRuntime(void);

void REALRegisterControl(REALcontrol *defn);

void REALRegisterMobileControl(REALmobileControl *defn);

void REALRegisterDBEngine(REALdbEngineDefinition *defn);

void REALRegisterDBTable(REALdbTableDefinition *defn);

void REALRegisterDBCursor(REALdbCursorDefinition *defn);

void REALRegisterClass(REALclassDefinition *defn);

REALstring REALBuildStringWithEncoding( const char *contents, int byteCount, uint32_t encoding ) RB_WARN_UNUSED_RESULT;

void REALLockObject(REALobject obj);

void REALUnlockObject(REALobject obj);

void REALLockString(REALstring str);

void REALUnlockString(REALstring str);

REALpicture REALBuildPictureFromPictureDescription(REALpictureDescription *description, bool bPassOwnership) RB_WARN_UNUSED_RESULT;

void REALUnlockPictureDescription(REALpicture pic);

REALdbCursor REALdbCursorFromDBCursor(dbCursor *cursor, REALdbCursorDefinition *defn);

/**
 * This function creates a new RowSet from the given dbCursor definition.  Since it
 * returns a REALdbCursor you can use it on other functions that accept it
 *
 * @param cursor Your dbCursor object which you'll need to maintain until closeCursor is called
 * @param defn Your cursor definition structure
 * @result A new object.
 * @version 2019r2
 */
REALdbCursor REALNewRowSetFromDBCursor(dbCursor *cursor, REALdbCursorDefinition *defn);

REALdbDatabase REALdbDatabaseFromDBDatabase(dbDatabase *database, REALdbEngineDefinition *defn);

void *REALGetEventInstance(REALcontrolInstance instance, REALevent *event);

void *REALGetControlData(REALcontrolInstance instance, REALcontrol *defn);

void *REALGetMobileControlData(REALcontrolInstance instance, REALmobileControl *defn);

void *REALGetClassData(REALobject instance, REALclassDefinition *defn);

#if TARGET_CARBON || TARGET_COCOA
REALappleEvent REALBuildAppleEvent(const AppleEvent *event, bool bPassOwnership) RB_WARN_UNUSED_RESULT;
#endif

#if TARGET_CARBON || TARGET_COCOA
REALappleEvent REALBuildAEDescList(const AppleEvent *event, bool bPassOwnership) RB_WARN_UNUSED_RESULT;
#endif

#if TARGET_CARBON || TARGET_COCOA
REALappleEvent REALBuildAEObjSpecifier(const AppleEvent *event, bool bPassOwnership) RB_WARN_UNUSED_RESULT;
#endif

#if TARGET_CARBON || TARGET_COCOA
AppleEvent *REALAccessAppleEvent(REALappleEvent event);
#endif

#if TARGET_CARBON || TARGET_COCOA
AppleEvent *REALAccessAppleEventReply(REALappleEvent event);
#endif

void REALRegisterDataSourceInterface(const char *szMenuName, REALDataSourceInterfaceProc proc);

void REALRegisterDataSource(const char *szDatasourceName, REALDataSourceProc proc);

void REALDesignAddDataSource(const char *baseName, const char *szDataSourceName, void *data, int dataLen);

void REALRegisterDatabaseConnection(REALDatabaseConnectionDefinition *defn);

#if TARGET_WIN32
REALpicture REALBuildPictureFromDIB(HANDLE hDIB, bool bPassOwnership) RB_WARN_UNUSED_RESULT;
#endif

double REALGetRBVersion(void);

void REALRaiseException(REALobject exception);

RBInteger REALGetArrayUBound(void*array);

void REALGetArrayStructure( REALarray array, RBInteger index, void *structure );

bool REALGetVariantStructure( REALobject variant, void *buffer, size_t length );

void REALYieldToRB(void);

REALclassRef REALGetClassRef(const char *className);

REALobject REALnewInstanceWithClass( REALclassRef classRef ) RB_WARN_UNUSED_RESULT;

/**
 * This function creates a new instance from the given class definition. Space
 * for the object is allocated, and the object's initializer (the class
 * definition's "constructor" member) is called before the object is returned.
 *
 * No constructor methods are invoked automatically. If one needs to be invoked,
 * REALLoadObjectMethod should be used to get the appropriate function pointer.
 *
 * @param classDefn The non-NULL class definition.
 * @result A new object.
 * @version 2014r2
 */
REALobject REALnewInstanceOfClass(REALclassDefinition *classDefn) RB_WARN_UNUSED_RESULT RB_NON_NULL(1);

void REALRegisterInterface(REALinterfaceDefinition *defn);

REALstring REALGetDBHost(REALdbDatabase db) RB_WARN_UNUSED_RESULT;

REALstring REALGetDBDatabaseName(REALdbDatabase db) RB_WARN_UNUSED_RESULT;

REALstring REALGetDBPassword(REALdbDatabase db) RB_WARN_UNUSED_RESULT;

REALstring REALGetDBUserName(REALdbDatabase db) RB_WARN_UNUSED_RESULT;

dbDatabase *REALGetDBFromREALdbDatabase(REALdbDatabase db);

void REALConstructDBDatabase(REALdbDatabase db, dbDatabase *mydb, REALdbEngineDefinition *engine);

REALDBConnectionDialogRef REALDBConnectionDialogCreate(void) RB_WARN_UNUSED_RESULT;

void REALDBConnectionDialogAddField(REALDBConnectionDialogRef dialogRef, REALstring label, REALstring defaultText, bool maskField);

REALstring REALDBConnectionDialogShow(REALDBConnectionDialogRef dialogRef, REALstring title);

void REALDBConnectionDialogDelete(REALDBConnectionDialogRef dialogRef);

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
REALpicture REALBuildPictureFromBuffer(uint32_t width, uint32_t height, RBPixelType pixelType, void*buffer, uint32_t rowBytes) RB_WARN_UNUSED_RESULT;
#endif

#if TARGET_CARBON || TARGET_WIN32 || X_WINDOW || TARGET_COCOA
bool REALInDebugMode(void);
#endif

void REALStripAmpersands(REALstring*  ioString);

REALobject REALGetProjectFolder(void) RB_WARN_UNUSED_RESULT;

void REALRegisterModule(REALmoduleDefinition*defn);

void *REALLoadFrameworkMethod( const char *prototype );

void *REALLoadObjectMethod(REALobject object, const char *prototype);

REALproc REALInterfaceRoutine(REALobject obj, const char *interfaceName, const char *methodName);

/**
 * Loads a shared method the plugin can invoke.
 *
 * @param classRef A class reference that was obtained by calling REALGetClassRef
 *                 during the plugin's main function.
 * @param prototype The method prototype in Xojo syntax.
 * @return A function pointer to invoke. Unlike instance methods loaded via
 *         REALLoadObjectMethod, there is no implicit 0th parameter required.
 * @version 2016r1
 */
void *REALLoadSharedMethod(REALclassRef classRef, const char *prototype);

/**
 * Gets the value of a property on an object
 *
 * @param value Object of interest
 * @param propName Property name to get
 * @param result Receives value
 * @return true if successful, false otherwise
 * @version 2015r1
 */
bool REALGetPropValueText(REALobject value,
	const char *propName,
	REALtext *result) RB_NON_NULL(3);

void REALSetDBIsConnected(REALdbDatabase database, bool connected);

/**
 * Creates a new Variant that holds a Text object
 *
 * @param value The Text object
 * @return Variant object which holds the Text object
 * @version 2015r1
 */
REALobject REALNewVariantText(REALtext value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantString(REALstring value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantInt32(int32_t value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantInteger(RBInteger value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantDouble(double value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantBoolean(bool value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantColor(RBColor value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantStructure( const void *data, size_t len ) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantPtr( void *value ) RB_WARN_UNUSED_RESULT;
REALobject REALNewVariantCString( const char *value ) RB_WARN_UNUSED_RESULT;
REALobject REALNewVariantWString( const wchar_t *value ) RB_WARN_UNUSED_RESULT;
REALobject REALNewVariantPString( const unsigned char *value ) RB_WARN_UNUSED_RESULT;
REALobject REALNewVariantOSType( uint32_t value ) RB_WARN_UNUSED_RESULT;
#if TARGET_CARBON || TARGET_COCOA
	REALobject REALNewVariantCFStringRef( CFStringRef value ) RB_WARN_UNUSED_RESULT;
#endif


REALarray REALCreateArray(REALArrayType type, RBInteger bounds) RB_WARN_UNUSED_RESULT;

/**
 * Sets the value of a Text property on an object.
 *
 * @param object The object whose property is being set.
 * @param propName Property name to set.
 * @param value The Text value to assign to the property.
 * @return Whether or not the property was successfully set.
 * @version 2015r1
 */
bool REALSetPropValueText(REALobject object,
	const char *propName,
	REALtext value);

dbCursor *REALGetCursorFromREALdbCursor(REALdbCursor cursor);

bool REALLockPictureDescription(REALpicture pic, REALpictureDescription *description, uint32_t picType);
void REALLockPictureDescriptionWithNativeType(REALpicture pic, REALpictureDescription *description);	

REALobject REALNewVariantUInt32(uint32_t value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantInt64(RBInt64 value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantUInt64(RBUInt64 value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantSingle(float value) RB_WARN_UNUSED_RESULT;

REALobject REALNewVariantCurrency(REALcurrency value) RB_WARN_UNUSED_RESULT;

uint32_t REALstringToOSType(REALstring id);

size_t REALStringLength(REALstring str);

void *REALGetStringContents(REALstring str, size_t *numBytes);

uint32_t REALGetStringEncoding(REALstring str);

REALstring REALConvertString(REALstring str, uint32_t encoding) RB_WARN_UNUSED_RESULT;

uint32_t REALWin32CodePageToEncoding(uint32_t codePage);

void REALGetControlBounds(REALcontrolInstance instance, Rect *rBounds);

bool REALGetControlVisible(REALcontrolInstance instance);
void REALSetControlVisible(REALcontrolInstance instance, bool visible);

#if TARGET_WIN32 || X_WINDOW
/**
* Sets the accelerator/shortcut key for the control
*
* @param instance The control to set the accelerator key for
* @param key The desired accelerator key with the ampersand, i.e. pass "&a", not just "a"
*			It is ok to pass the full caption, i.e. Win&dows would be parsed for &d
*/
void REALSetAccelerator(REALcontrolInstance instance, REALstring key);
#endif

/**
* Gets the graphic's drawing origin relative to the control that owns the context
*
* @param context The graphics context to obtain the drawing origin from
* @param originX Drawing is offset by this much in the horizontal direction
* @param originY Drawing is offset by this much in the vertical direction
*/
void REALGetGraphicsOrigin(REALgraphics context, int32_t *originX, int32_t *originY);

/**
* Sets the graphic's drawing origin relative to the control that owns the context
*
* @param context The graphics context to set the drawing origin to
* @param originX Drawing to be offset by this much in the horizontal direction
* @param originY Drawing to be offset by this much in the vertical direction
*/
void REALSetGraphicsOrigin(REALgraphics context, int32_t originX, int32_t originY);

#if TARGET_WIN32
/**
* Releases the device context from memory, and more importantly, flushes the drawing
* done on handle to our Direct2D backed surface.  This must be paired with each call
* to Graphics.Handle or REALGraphicsDC
*
* @param context The graphics context to release HDC from
*/
void REALGraphicsReleaseDC(REALgraphics context);
#endif

#if TARGET_WIN32
void REALSetSpecialBackground(REALcontrolInstance instance, COLORREF *pcolor);
#endif

/**
* Gets the default system text encoding for the platform
*
* @return on OS X and Linux this returns kREALTextEncodingUTF8, on Windows it will vary
*/
uint32_t REALGetSystemTextEncoding(void);

void REALPictureClearCache(REALpicture pic);

/**
 * Retrieves a Picture's Type property, which is completely unrelated to the
 * type in REALpictureDescription.
 *
 * @version 2016r1
 */
RBInteger REALGetPictureType(REALpicture pic) RB_NON_NULL(1);


/**
* Returns the number of properties for the specified object
*
* @version Added in 2019r2, but this API can be used in 2012 or even earlier
*/
RBInteger REALCountClassProperties(REALobject obj);

/**
* Retreives the property getter, setter, and declaration
*
* @param obj The object to get the property for
* @param index The number of the property to be extracted
* @param getter Address of a variable to contain the property's getter
* @param setter Address of a variable to contain the property's setter
* @param param Address of a variable to contain the parameter to pass the getter and setter
* @param declaration Address of a string to contain its declaration
* @return True if the property was found, false if it was not
*
* @version Added in 2019r2, but this API can be used in 2012 or even earlier
*/
RBBoolean REALGetClassProperty(REALobject obj, uint32_t index, void **getter, void **setter, long *param, REALstring *declaration);

/**
* Adds event implementation for the specified event handler
*
* @param obj The object to add a handler for
* @param eventName The name of the event to add a handler for
* @param handler A pointer to the handler function with at least the first parameter being the object
* @return True if the event was found and the handler was successfully added, false otherwise
*
* @version Added in 2020r1
*/
RBBoolean REALAddEventHandler(REALobject obj, REALstring eventName, void *handler);

/**
* Removes an event implementation for the specified object
*
* @param obj The object to remove a handler for
* @param eventName The name of the event to remove a handler for
* @param handler A pointer to the handler function that was used in REALAddEventHandler
* @return True if the event was found and the handler was successfully removed, false otherwise
*
* @version Added in 2020r1
*/
RBBoolean REALRemoveEventHandler(REALobject obj, REALstring eventName, void *handler);

/**
* Checks to see if an event is handled
*
* @param obj The object to check against
* @param eventName The name of the event to check
* @return True if the event is handled, false otherwise
*
* @version Added in 2020r1
*/
RBBoolean REALIsEventHandled(REALobject obj, REALstring eventName);

/**
* Checks to see if the app is running in dark mode
*
* @return True if the app is running with dark mode support, false otherwise
* @note This is the optimized version of the same dynamic access call through Color.IsDarkMode
*
* @version Added in 2021r3
*/
RBBoolean REALIsDarkMode();

/**
* Gets the top-level Window of the specified control
*
* @return The host Window, or DesktopWindow, of the specified control.
* @note The returned object is not locked upon return.
*
* @version Undeprecated in 2021r3
*/
REALwindow REALGetControlWindow(REALcontrolInstance instance) RB_WARN_UNUSED_RESULT;

/**
* Gets state of current thread execution
*
* @return True is current execution is in a Xojo thread (but not Xojo's main thread)
* @note This is only supported for Console (including Web) and Desktop.
*
* @version Added in 2021r3
*/
RBBoolean REALIsXojoThread() RB_WARN_UNUSED_RESULT;

/**
* Gets state of current thread execution
*
* @return True is current execution is in Xojo's main thread
* @note This is only supported for Console (including Web) and Desktop.
*
* @version Added in 2021r3
*/
RBBoolean REALIsXojoMainThread() RB_WARN_UNUSED_RESULT;

#if defined(__cplusplus)
	}
#endif


#endif	// RB_PLUGIN_H
