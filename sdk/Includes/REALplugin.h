/** @file REALplugin.h
 *  @brief This file includes all the data structures that can be created and registereed.
 *	It's included automatically by "rb_plugin.h", so normally you'll just include the latter.
 *	@copyright Xojo, Inc. -- All Rights Reserved.
 */

#ifndef REALPLUGIN_H
#define REALPLUGIN_H

#if defined(__APPLE__)
    #if !TARGET_OS_IPHONE
	    #include <Carbon/Carbon.h>
        #if !defined(CARBON) && !defined(COCOA)
            #define COCOA 1
        #endif
        #if !defined(TARGET_CARBON)
            #define TARGET_CARBON 1
        #endif

        #if defined(COCOA) && COCOA
            #if defined(__OBJC__)
                #import <Cocoa/Cocoa.h>
            #else
                struct NSWindow;
                struct NSView;
            #endif
            #define TARGET_COCOA 1
        #endif

        #if !defined(TARGET_MACOS)
            #define TARGET_MACOS 1
        #endif
    #endif

#elif defined(__MINGW32__) || defined(_MSC_VER)
	#include <windows.h>
	#if !defined(WINDOWS)
		#define WINDOWS 1
	#endif
	#if !defined(TARGET_WIN32)
		#define TARGET_WIN32 1
	#endif

#elif defined(__linux__)
	#if !defined(UNIX_ANSI)
		#define UNIX_ANSI 1
	#endif
	#if !defined(X_WINDOW)
		#define X_WINDOW 1
	#endif
#endif

#if defined(__LP64__) || defined(_WIN64)
	#define TARGET_64BIT 1
#else
	#define TARGET_64BIT 0
#endif

#include <stdint.h>
#include <stddef.h>

typedef int64_t RBInt64;
typedef uint64_t RBUInt64;
typedef int8_t RBBoolean;
typedef uint32_t RBColor;

#if TARGET_64BIT
	typedef int64_t RBInteger;
	typedef uint64_t RBUInteger;
#else
	typedef int32_t RBInteger;
	typedef uint32_t RBUInteger;
#endif

#if defined(__clang__) || defined(__GCC__)
	#define RB_WARN_UNUSED_RESULT __attribute((warn_unused_result))
	#define RB_NON_NULL(x) __attribute__((nonnull(x)))
#else
	#define RB_WARN_UNUSED_RESULT
	#define RB_NON_NULL(x)
#endif

#if !defined(__APPLE__) && !defined(MACTYPESFORANSI_H)
	// On OS X, we get this structure from <MacTypes.h>.
	struct Rect {
		int16_t top, left, bottom, right;
	};
	typedef struct Rect Rect;
#endif

/// The Currency data type is a signed 64-bit integer value that
/// is expressed in "mils."  This means that if you would like to
/// convert from a currency value into a regular Int64 value, you 
/// would simply divide by 10000.  To convert an Int64 into a
/// currency value, you would multiply by 10000. -- Jun 23 2006 -- AJB (1)
typedef RBInt64 REALcurrency;

typedef enum REALArrayType {
	kTypeSInt32 = 0,					/// 32-bit integer, or color
	kTypeFloat32 = 1,					/// 32-bit float type (single)
	kTypeFloat64 = 2,					/// 64-bit float type (double)
	kTypeString = 3,					/// string (32 bit pointer, refcounted)
	kTypeObject = 4,					/// object (32 bit pointer, refcounted)
	kTypeUInt8 = 5,						/// 8-bit unsigned integer, or boolean
	kTypeSInt8 = 6,						/// 8-bit signed integer
	kTypeUInt16 = 7,					/// 16-bit unsigned integer
	kTypeSInt16 = 8,					/// 16-bit signed integer
	kTypeUInt32 = 9,					/// 32-bit unsigned integer
	kTypeUInt64 = 10,					/// 64-bit unsigned integer
	kTypeSInt64 = 11,					/// 64-bit signed integer, or Currency
	kTypeStructure = 12,				/// Arbitrary-sized structure
	kTypePtr = 13,						/// 32-bit signed integer
	kTypeColor = 14,					/// 32-bit signed integer
	kTypeCurrency = 15,					/// 64-bit currency integer
	kTypeBoolean = 16,					/// 8-bit signed integer
	kTypeText = 17,						/// Text (pointer, refcounted)
#if TARGET_64BIT
	kTypeInteger = kTypeSInt64,
#else
	kTypeInteger = kTypeSInt32,
#endif
} REALArrayType;

struct REALstringStruct;
typedef struct REALstringStruct *REALstring;

typedef const struct RuntimeText *REALtext;

typedef void (*REALproc)(void);

struct REALdbDatabaseStruct;
typedef struct REALdbDatabaseStruct *REALdbDatabase;

struct REALdbCursorStruct;
typedef struct REALdbCursorStruct *REALdbCursor;

struct REALobjectStruct;
typedef struct REALobjectStruct *REALobject;

typedef REALobject REALcontrolInstance;

struct REALarrayStruct;
typedef struct REALarrayStruct *REALarray;

struct REALclassRefStruct;
typedef struct REALclassRefStruct *REALclassRef;

typedef unsigned long REALDBConnectionDialogRef;

#define REALnoImplementation ((REALproc) nil)

// Deprecated target flags
#define REALpropRuntimeOnly				(1 << 1U)
#define	REALconsoleSafe					(1 << 2U)
#define REALconsoleOnly					(1 << 3U)
#define REALwebOnly						(1 << 8U)

// New Target support flags
// You can use the following definitions (in combination) when setting
// the "flags" property for methods, properties, classes and controls
#define REALTargetRuntimeOnly			(1 << 1U)
#define REALTargetAll					(1 << 2U)
#define REALTargetConsole				(1 << 3U)
#define REALTargetWeb					(1 << 8U)
#define REALTargetDesktop				(1 << 9U)
#define REALTargetIOS					(1 << 10U)

// You can use the following definitions when setting
// the flags property for methods, properties, structures
// and enumerations.
#define	REALScopeGlobal					(1 << 4U)
#define	REALScopePublic					(1 << 5U)
#define	REALScopeProtected				(1 << 6U)
#define	REALScopePrivate				(1 << 7U)

typedef struct
{
	const char *mName;
	const char *mValue;
} REALattribute;

typedef struct
{
	REALproc function;
	REALproc setterFunction;
	const char *declaration;
	uint32_t mFlags;
	size_t attributeCount;
	REALattribute *attributes;
} REALmethodDefinition;

#define REALstandardGetter ((REALproc) -1)
#define REALstandardSetter ((REALproc) -1)

typedef struct
{
	const char *group;
	const char *name;
	const char *type;
	uint32_t flags;
	REALproc getter;
	REALproc setter;
	int32_t param;
	REALproc editor;
	size_t enumCount;
	const char **enumEntries;
	size_t attributeCount;
	REALattribute *attributes;
} REALproperty;

typedef struct 
{
	const char *declaration;
	RBInteger forSystemUse;
	size_t attributeCount;
	REALattribute *attributes;
} REALevent;

typedef struct
{
	const char *name;
	REALproc implementation;
} REALeventInstance;

typedef struct 
{
	const char *declaration;
	void* reserved1;
	uint32_t mFlags;
	size_t attributeCount;
	REALattribute *attributes;
} REALconstant;

typedef struct REALstructure	// Jun 23 2006 -- AJB (1)
{
	/// Specifies the name of the structure
	const char *name;
	
	/// Flags for the structure, such as scope
	uint32_t mFlags;
	
	/// The fields of the structure as 
	/// spelled out in REALbasic code
	/// eg) "Foobar as Integer"
	const char **fields;
	
	/// The number of fields in the 
	/// structure, as specified by 
	/// the fields member.
	size_t numFields;

	size_t attributeCount;
	REALattribute *attributes;
} REALstructure;

typedef struct REALenum		// Jun 23 2006 -- AJB (1)
{
	/// Specifies the name of the enumeration
	const char *name;
	
	/// Specifies the type of the enumeration
	/// as an RB intrinsic datatype.
	/// eg) Integer or UInt8 (currently, integer
	/// types are the only types supported).  If
	/// you leave it blank, it means Integer
	const char *type;
	
	/// Flags for the enumeration, such as scope
	uint32_t mFlags;
	
	/// The fields of the enumeration
	/// as spelled out in REALbasic code
	/// eg) "Apple" or "Pear = 12"
	const char **fields;
	
	/// The number of fields in the
	/// enumeration, as specified by
	/// the fields member.
	size_t numFields;

	size_t attributeCount;
	REALattribute *attributes;
} REALenum;

// Added in 2013r1 (Control Version 11)
typedef struct REALdelegate
{
	const char *declaration;
	uint32_t mFlags;
	size_t attributeCount;
	REALattribute *attributes;
} REALdelegate;

typedef struct
{
	void (*constructorFunction)(REALcontrolInstance);
	void (*destructorFunction)(REALcontrolInstance);
	void (*redrawFunction)(REALcontrolInstance, REALobject context);
	RBBoolean (*clickFunction)(REALcontrolInstance, int x, int y, int modifiers);
	void (*mouseDragFunction)(REALcontrolInstance, int x, int y);
	void (*mouseUpFunction)(REALcontrolInstance, int x, int y);
	void (*gainedFocusFunction)(REALcontrolInstance);
	void (*lostFocusFunction)(REALcontrolInstance);
	RBBoolean (*keyDownFunction)(REALcontrolInstance, int charCode, int keyCode, int modifiers);
	void (*openFunction)(REALcontrolInstance);
	void (*closeFunction)(REALcontrolInstance);
	void (*backgroundIdleFunction)(REALcontrolInstance);
	void(*drawOffscreenFunction)(REALcontrolInstance, REALobject context);
	void (*setSpecialBackground)(REALcontrolInstance);
	void (*constantChanging)(REALcontrolInstance, REALstring);
	void (*droppedNewInstance)(REALcontrolInstance);
	void (*mouseEnterFunction)(REALcontrolInstance);
	void (*mouseExitFunction)(REALcontrolInstance);
	void (*mouseMoveFunction)(REALcontrolInstance, int x, int y);
	void (*stateChangedFunction)(REALcontrolInstance, uint32_t changedField);
	void (*actionEventFunction)(REALcontrolInstance, uint32_t reserved);
	void *(*controlHandleGetter)(REALcontrolInstance);
	RBBoolean (*mouseWheelFunction)(REALcontrolInstance, int x, int y, int delta);
	void (*enableMenuItemsFunction)(REALcontrolInstance);
	RBBoolean(*menuItemActionFunction)(REALcontrolInstance, REALobject menuitem);
	RBBoolean(*controlAcceptFocus)(REALcontrolInstance);
	RBBoolean(*keyUpFunction)(REALcontrolInstance, int charCode, int keyCode, int modifiers);
	
	/// This function is preferred to the original redrawFunction because it exposes
	/// the array of rectangles that need to be redrawn. If there are no rectangles
	/// passed, the control must redraw all of its contents.
	///
	/// In Real Studio 2012r2 and later, this function is invoked if it is available
	/// instead of redrawFuntion. If this function is not present, or the plugin
	/// is loaded into an older version of Real Studio, redrawFunction will be
	/// invoked.
	void(*redrawWithRectsFunction)(REALcontrolInstance, REALobject context, const Rect *dirtyRects, int dirtyRectCount);

	/// In Xojo 2013r3 and later, this function is invoked before any key translation takes place
	RBBoolean (*unfilteredKeyDownFunction)(REALcontrolInstance, int keyCode, int modifiers);

	/// In Xojo 2016r1 and later, this function is invoked when the backing store scale factor has changed
	void (*scaleFactorChangedFunction)(REALcontrolInstance, double newScaleFactor);

	void *reserved2;
	void *reserved3;
} REALcontrolBehaviour;

typedef struct
{
    void (*constructorFunction)(REALcontrolInstance);
    void (*destructorFunction)(REALcontrolInstance);
	void *(*controlHandleGetter)(REALcontrolInstance);
    void (*redrawFunction)(REALcontrolInstance, REALobject context);
    /// PointerDown(position As Xojo.Point, pointerInfoArray() As PointerEvent)
    RBBoolean (*pointerDownFunction)(REALcontrolInstance, REALobject position, REALobject pointerInfoArray);
    /// PointerUp(position As Xojo.Point, pointerInfoArray() As PointerEvent)
    RBBoolean (*pointerUpFunction)(REALcontrolInstance, REALobject position, REALobject pointerInfoArray);
    /// PointerDrag(position As Xojo.Point, pointerInfoArray() As PointerEvent)
    RBBoolean (*pointerDragFunction)(REALcontrolInstance, REALobject position, REALobject pointerInfoArray);
    void *reserved1;
    void *reserved2;
    void *reserved3;
    void *reserved4;
    void *reserved5;
    void *reserved6;
    void *reserved7;
    void *reserved8;
    void *reserved9;
    void *reserved10;
} REALmobileControlBehaviour;

#ifndef kCurrentREALControlVersion
	#define kCurrentREALControlVersion 13
#endif

#define REALcontrolAcceptFocus 			(1 << 0U)
#define REALcontrolFocusRing 			(1 << 1U)
#define REALinvisibleControl 			(1 << 2U)
#define REALopaqueControl 				(1 << 3U)
#define REALcontrolOwnsCursor 			(1 << 5U)
#define REALdontEraseBackground			(1 << 8U)	 /// Set this flag if you don't want REALbasic to automatically erase the control's background.
													 /// Note, this only affects Windows, use it to reduce flicker if your control already handles background painting.
#define REALcontrolHandlesKeyboardNavigation uint32_t(1 << 10) /// This flag is only used on Windows, use it to suppress the default keyboard navigation.
											     /// For example, pressing the arrow keys or pressing a keyboard mnemonic moves focus to another control, but this
												 /// is not always desirable, especially when writing your own edit control.
#define REALcontrolHandlesTabKey		uint32_t(1 << 11)	/// This flag specifies whether or not the control handles the tab key, especially useful if
															/// the control has subcontrols and you want to handle focus navigation yourself
#define REALdesktopControl				uint32_t(1 << 12)	/// Use this flag to indicate that this control is a DesktopControl/DesktopUIControl


typedef struct
{
	uint32_t version;
	const char *name;
	size_t dataSize;
	uint32_t flags;
	uint32_t toolbarPICT, toolbarDownPICT;
	uint32_t defaultWidth, defaultHeight;
	REALproperty *properties;
	size_t propertyCount;
	REALmethodDefinition *methods;
	size_t methodCount;
	REALevent *events;
	size_t eventCount;
	REALcontrolBehaviour *behaviour;
	RBInteger forSystemUse;
	REALeventInstance *eventInstances;
	size_t eventInstanceCount;
	const char *interfaces;
	REALattribute *attributes;			// Re-using this in 2011r3 (control version 10 and later)
	size_t attributeCount;
	REALconstant *constants;
	size_t constantCount;
#if kCurrentREALControlVersion >= 11
	REALproperty *sharedProperties;		// Begin Added in RB2006r4 (control version 9 and later) -- Jun 23 2006 -- AJB (1)
	size_t sharedPropertyCount;
	REALmethodDefinition *sharedMethods;
	size_t sharedMethodCount;				// End RB2006r4 stuff
	REALdelegate *delegates;			/// 2013r1 (Control version 11 or later)
	size_t delegateCount;
	REALenum *enums;					/// 2013r1 (Control version 11 or later)
	size_t enumCount;
#endif
} REALcontrol;

// For ease of use this mimics the REALcontrol definition except for the behaviour
typedef struct
{
    uint32_t version;
    const char *name;
    size_t dataSize;
    uint32_t flags;
    uint32_t toolbarPICT, toolbarDownPICT;
    uint32_t defaultWidth, defaultHeight;
    REALproperty *properties;
    size_t propertyCount;
    REALmethodDefinition *methods;
    size_t methodCount;
    REALevent *events;
    size_t eventCount;
    REALmobileControlBehaviour *behaviour;
    RBInteger forSystemUse;
    REALeventInstance *eventInstances;
    size_t eventInstanceCount;
    const char *interfaces;
    REALattribute *attributes;
    size_t attributeCount;
    REALconstant *constants;
    size_t constantCount;
    REALproperty *sharedProperties;
    size_t sharedPropertyCount;
    REALmethodDefinition *sharedMethods;
    size_t sharedMethodCount;
    REALdelegate *delegates;
    size_t delegateCount;
    REALenum *enums;
    size_t enumCount;
} REALmobileControl;

typedef struct
{
	uint32_t version;
	const char *name;
	const char *superName;
	size_t dataSize;
	RBInteger forSystemUse;
	REALproc constructor;
	REALproc destructor;
	REALproperty *properties;
	size_t propertyCount;
	REALmethodDefinition *methods;
	size_t methodCount;
	REALevent *events;
	size_t eventCount;
	REALeventInstance *eventInstances;
	size_t eventInstanceCount;
	const char *interfaces;
	REALattribute *attributes;			// Re-using this in 2011r3 (control version 10 and later)
	size_t attributeCount;
	REALconstant *constants;
	size_t constantCount;
	uint32_t mFlags;
	REALproperty *sharedProperties;		// Begin Added in RB2006r4 (control version 9 and later) -- Jun 23 2006 -- AJB (1)
	size_t sharedPropertyCount;
	REALmethodDefinition *sharedMethods;
	size_t sharedMethodCount;				// End RB2006r4 stuff
#if kCurrentREALControlVersion >= 11
	REALdelegate *delegates;			/// 2013r1 (Control version 11 or later)
	size_t delegateCount;
	REALenum *enums;					/// 2013r1 (Control version 11 or later)
	size_t enumCount;
#endif
} REALclassDefinition;

void SetClassConsoleSafe( REALclassDefinition *def );
void SetClassWebOnly( REALclassDefinition *def );

typedef struct
{
	uint32_t version;					/// just pass kCurrentREALControlVersion
	const char* name;					/// interface name
	REALmethodDefinition *methods;		/// list of methods the interface requires
	size_t methodCount;					/// how many methods there are
	REALattribute *attributes;
	size_t attributeCount;
} REALinterfaceDefinition;

struct REALmoduleDefinition {
	uint32_t version;					/// use kCurrentREALControlVersion
	const char *name;					/// name of the module
	REALmethodDefinition *methods;		/// list of public module methods
	size_t methodCount;					/// number of entries in the method list
	REALconstant* constants;			/// list of constants
	size_t constantCount;				/// number of constants in the list
	REALproperty *properties;			// Begin Added in RB2006r4 (control version 9 and later) -- Jun 23 2006 -- AJB (1)
	size_t propertyCount;				//
	REALstructure *structures;			//
	size_t structureCount;				// 
	REALenum *enums;					//
	size_t enumCount;					//
	REALattribute *attributes;
	size_t attributeCount;
#if kCurrentREALControlVersion >= 11
	REALdelegate *delegates;			/// 2013r1 (Control version 11 or later)
	size_t delegateCount;
#endif
#if kCurrentREALControlVersion >= 13	// 2013r4
	REALclassDefinition **classes;		// Assumes this module's namespace
	size_t classesCount;
	REALinterfaceDefinition *interfaces;// Assumes this module's namespace
	size_t interfaceCount;
	struct REALmoduleDefinition **modules;		// Assumes this module's namespace
	size_t moduleCount;
	REALcontrol **controls;				// Assumes this module's namespace
	size_t controlCount;
#endif
#if kCurrentREALControlVersion >= 14
	uint32_t flags;						/// Compatibility/target flags
#endif
#if kCurrentREALControlVersion >= 15
    REALmobileControl **mobileControls;
    size_t mobileControlCount;
#endif
};
typedef struct REALmoduleDefinition REALmoduleDefinition;

#define kCurrentREALDatabaseVersion 2
#define kDatabaseWithRowSetVersion 2

typedef void (*REALDataSourceInterfaceProc)(void);
typedef REALdbDatabase (*REALDataSourceProc)(void *data, int dataLen);

typedef struct dbDatabase dbDatabase;
typedef struct dbTable dbTable;
typedef struct dbCursor dbCursor;

typedef struct
{
	short year;
	short month;
	short day;
} dbDate;

typedef struct
{
	short hour;
	short minute;
	short second;
} dbTime;

typedef struct 
{
	short year;
	short month;
	short day;
	short hour;
	short minute;
	short second;
} dbTimeStamp;

typedef enum dbFieldType
{
	dbTypeNull = 0,		///  0
	dbTypeByte,			///  1	// also dbTypeUInt8
	dbTypeShort,		///  2
	dbTypeLong,			///  3	// Treated as a long type, which is variable depending on platform and architecture but it always converted to an int32
	dbTypeChar,			///  4
	dbTypeText,			///  5
	dbTypeFloat,		///  6
	dbTypeDouble,		///  7
	dbTypeDate,			///  8
	dbTypeTime,			///  9
	dbTypeTimeStamp,	/// 10
	dbTypeCurrency,		/// 11
	dbTypeBoolean,		/// 12
	dbTypeDecimal,		/// 13
	dbTypeBinary,		/// 14
	dbTypeLongText,		/// 15	// deprecated; use dbTypeText
	dbTypeLongBinary,	/// 16	// deprecated; use dbTypeBinary
	dbTypeMacPICT,		/// 17
	dbTypeREALstring,	/// 18
	dbTypeInt64,		/// 19
	dbTypeUInt64,		/// 20
	dbTypeInt8,			/// 21
	dbTypeUInt16,		/// 22
	dbTypeInt32,		/// 23
	dbTypeUInt32,		/// 24
	dbTypeUInt8 = 1,
	dbTypeInt16 = 2,
	dbTypeUnknown = 255
} dbFieldType;

struct REALnewColumn 
{
	struct REALnewColumn *nextColumn;
	REALstring columnName;
	RBInteger columnType;				/// dbFieldType
	RBInteger bAllowNULL;				/// Specifies whether or not this column allow NULL values
	REALstring columnTypeString;	/// Since 6.0, so we can specify type constraints such as "Decimal(5,2)"
};
typedef struct REALnewColumn REALnewColumn;

struct REALcolumnValue
{
	struct REALcolumnValue *nextColumn;
	REALstring columnName;
	REALstring columnValue;
	int32_t columnType;
};
typedef struct REALcolumnValue REALcolumnValue;

enum REALcolumnOperation
{
	rcOpEquals,
	rcOpLessThan,
	rcOpGreaterThan,
	rcOpLessThanEqual,
	rcOpGreaterThanEqual,
	rcOpNotEqual,
	rcOpLike,
	rcOpNotLike,
	rcOpIsNull,
	rcOpIsNotNull,

	rcOpAnd = 64,
	rcOpOr
};

struct REALcolumnConstraints
{
	struct REALcolumnConstraints *left, *right;
	int32_t columnOperation;
	int32_t columnComparison;
	REALstring column;
	REALstring value;
};
typedef struct REALcolumnConstraints REALcolumnConstraints;

struct REALgetColumn
{
	struct REALgetColumn *next;
	REALstring column;
};
typedef struct REALgetColumn REALgetColumn;

// REALdbEngineDefinition Flags1
enum {
	dbEnginePrimaryKeySupported = 1,
	dbEngineAlterTableAddParens = 2,
	dbEngineDontUseBrackets = 4,
	dbEngineColumnDeleteSupported = 8,
	dbEngineColumnModificationSupported = 16,
	dbEngineColumnOptionsSupported = 32
};

struct REALfieldUpdate
{
	struct REALfieldUpdate *next;
	char *tableField;
	uint32_t tableFieldLen;
	char *recordKey;
	uint32_t recordKeyLen;
	REALstring value;
};
typedef struct REALfieldUpdate REALfieldUpdate;

typedef struct
{
	uint32_t version;
	unsigned char forSystemUse;
	unsigned char flags1;
	unsigned char flags2;
	unsigned char flags3;

	void (*closeDatabase)(dbDatabase *); // void (*closeDatabase)(dbDatabase *);

	REALdbCursor (*getTableSchemaCursor)(dbDatabase *); // dbCursor *(*tableCursor)(dbDatabase *)) /* optional */
	REALdbCursor (*getFieldSchemaCursor)(dbDatabase *, REALstring); // dbCursor *(*fieldCursor)(dbDatabase *, REALstring)) /* optional */

	REALdbCursor (*directSQLSelect)(dbDatabase *, REALstring); // DatabaseCursorObject (*directSQLSelect)(dbDatabase *, REALstring selectString);
	void (*directSQLExecute)(dbDatabase *, REALstring); // void (*directSQLExecute)(dbDatabase *, REALstring executeString);

	void (*createTable)(dbDatabase *, REALstring, REALnewColumn *, unsigned char *, int); // void (*createTable)(dbDatabase *, REALstring name, REALnewColumn *columns, unsigned char *primaryKey, int primaryKeyCount);
	void (*addTableRecord)(dbDatabase *, REALstring, REALcolumnValue *); // void (*addTableRecord)(dbDatabase *, REALstring tableName, REALcolumnValue *values);

	// Runs the SQL select statement with the supplied parameters
	//  sql -- the sql select statement to execute
	//  params -- variant array of parameters, can be empty or null
	REALdbCursor (*SelectSQL)(dbDatabase *, REALstring sql, REALarray params);

	void (*updateFields)(dbDatabase *, REALfieldUpdate *fields);
	void (*addTableColumn)(dbDatabase *, REALstring, REALnewColumn *);
	REALdbCursor (*getDatabaseIndexes)(dbDatabase *, REALstring table);

	long (*getLastErrorCode)(dbDatabase *);
	REALstring (*getLastErrorString)(dbDatabase *);

	void (*commit)(dbDatabase *);
	void (*rollback)(dbDatabase *);
	void (*BeginTransaction)(dbDatabase *);

	void (*getSupportedTypes)(int32_t **dataTypes, char **dataNames, size_t *count);

	/// Drops a table
	///  tableName -- the table to drop
	void (*dropTable)( dbDatabase *db, REALstring tableName );

	/// Drops a column
	///  tableName -- table that the column exists in
	///  columnName -- column to drop
	void (*dropColumn)( dbDatabase *db, REALstring tableName, REALstring columnName );

	/// Renames a table
	///  oldTableName -- old name of the table
	///  newTableName -- new name of the table
	void (*alterTableName)( dbDatabase *db, REALstring oldTableName, REALstring newTableName );

	/// Renames a column
	///  tableName -- table that the column exists in
	///  oldColumnName -- old name of column
	///  newColumnName -- new name of column
	void (*alterColumnName)( dbDatabase *db, REALstring tableName, REALstring oldColumnName, REALstring newColumnName );

	/// Changes column type
	///  tableName -- table that the column exists in
	///  columnName -- the column to modify
	///  oldType -- old column type
	///  newType -- new column type 
	void (*alterColumnType)( dbDatabase *db, REALstring tableName, REALstring columnName, REALstring oldType, REALstring newType );

	/// Changes a column's constraint, such as Primary Key, Mandatory field, etc..
	///  tableName -- table that column exists in
	///  columnName -- the column to modify
	///  whichConstraint -- "Primary Key", "Mandatory", etc... same text that displays in the
	///                     properties listbox in the database schema editor
	///  oldConstraint -- the old value (if boolean, this string will be either "True" or "False")
	///  newConstraint -- the new value (if boolean, this string will be either "True" or "False")
	void (*alterColumnConstraint)( dbDatabase *db, REALstring tableName, REALstring columnName, REALstring whichConstraint, REALstring oldConstraint, REALstring newConstraint );

	REALobject (*PrepareStatement)( dbDatabase *db, REALstring statement );

	/// Executes the SQL statement with the supplied parameters
	///  sql -- the sql statement to execute
	///  params -- variant array of parameters, can be empty
	void (*ExecuteSQL)(dbDatabase *, REALstring sql, REALarray params);
	REALproc unused4;
} REALdbEngineDefinition;

typedef struct
{
	uint32_t version;
	RBInteger forSystemUse;

	REALproc closeTable; // void (*closeTable)(dbTable *);
	REALproc tableCursor; // dbCursor *(*tableCursor)(dbTable *);
} REALdbTableDefinition;

struct REALcursorUpdate
{
	struct REALcursorUpdate *next;
	int32_t fieldIndex;
	REALstring columnValue;
#if kCurrentREALControlVersion >= 11
	dbFieldType type;
#endif
};
typedef struct REALcursorUpdate REALcursorUpdate;

typedef struct
{
	uint32_t version;
	RBInteger forSystemUse;

	void (*closeCursor)(dbCursor *);
	int (*cursorColumnCount)(dbCursor *); // int (*cursorColumnCount)(dbCursor *);
	REALstring (*cursorColumnName)(dbCursor *, int column); // REALstring (*cursorColumnName)(dbCursor *, int column); /* optional */
	int (*cursorRowCount)(dbCursor *); // int (*cursorRowCount)(dbCursor *); /* optional */
	void (*cursorColumnValue)(dbCursor *, int, void **, unsigned char *, int *); // void (*cursorColumnValue)(dbCursor *, int column, void **value, dbFieldType *type, int *length);
	void (*cursorReleaseValue)(dbCursor *); // void (*cursorReleaseValue)(dbCursor *); /* optional */
	RBBoolean (*cursorNextRow)(dbCursor *); // Boolean (*cursorNextRow)(dbCursor *);
	void (*cursorDelete)(dbCursor *); // void (*cursorDelete)(dbCursor *);
	void (*cursorDeleteAll)(dbCursor *); // void (*cursorDeleteAll)(dbCursor *);
	RBBoolean (*cursorFieldKey)(dbCursor *, int, void **, int *, void **, int *);
	void (*cursorUpdate)(dbCursor *, REALcursorUpdate *fields);
	void (*cursorEdit)(dbCursor *);			// called by dbCursor.Edit in RB
	void (*cursorPrevRow)(dbCursor *);
	void (*cursorFirstRow)(dbCursor *);
	void (*cursorLastRow)(dbCursor *);
	int (*cursorColumnType)(dbCursor *, int index);	// gets the column type (0 based index)
	RBBoolean (*CursorIsBOF)(dbCursor *);	// Determines whether or not we're at the beginning of the record set
	RBBoolean (*CursorIsEOF)(dbCursor *);	// Determines whether or not we're at the end of the record set
	void *dummy10;
} REALdbCursorDefinition;

typedef struct {
	uint32_t mVersion;
	const char *mConnectionType;
	REALdbEngineDefinition *mDBEngine;

	void *(*Connect)( const char *connectionData, const size_t dataLength );
	void (*Disconnect)( void *db );
	const char *(*GetLastError)( void *db );
} REALDatabaseConnectionDefinition;

// picture types and description
enum {
	pictureUnknown,
	pictureMacintoshPICT,
	pictureMacintoshCICN,
	pictureMacintoshIconSuite,
	pictureMacintoshGWorld,
	pictureWin32DIB,		/// a HDIB, use GlobalLock on it
	pictureGdkPixmap,
	pictureWin32Bitmap,		/// a HBITMAP, but really a DIBSECTION, use GetObject on it
	pictureCGBitmapContext,
	pictureGDPtr,			/// Console Only: Pointer to a copy of GD data, which is 11 bytes meta data then pixels follow in ARGB format
	pictureGDImagePtr,		/// Console Only: Pointer to gdImageStruct
	pictureGDIPlusBitmap,	/// a GpBitmap handle
	pictureCairoContext,	/// a cairo_t ptr
	pictureNSImage,			/// an NSImage
	pictureD2DPBGRAPtr,		/// a raw pointer to the Direct2D Pre-multiplied BGRA bytes
	pictureIWICBitmap,		/// an IWICBitmap ptr
	picturePremultipliedARGB,/// a raw pointer to pre-multiplied ARGB color values, currently only used by iOS
};

// Common text encoding values
enum {
	kREALTextEncodingUnknown = 0xFFFFUL,
	kREALTextEncodingASCII = 0x0600UL,
	kREALTextEncodingUTF8 = 0x08000100UL,
	kREALTextEncodingUTF16 = 0x0100UL,
	kREALTextEncodingUTF16LE = 0x14000100UL,
	kREALTextEncodingUTF16BE = 0x10000100UL,
	kREALTextEncodingUTF32 = 0x0c000100UL,
	kREALTextEncodingUTF32LE = 0x1c000100UL,
	kREALTextEncodingUTF32BE = 0x18000100UL
};


struct REALpictureDescription
{
	uint32_t pictureType;
	void *pictureData;
	int32_t width, height, depth;			// Plugin authors can now get depth and transparency info (5.0)
	RBBoolean transparent;
};
typedef struct REALpictureDescription REALpictureDescription;

// handy macros
#define ControlData(defn, instance, typeName, data) typeName *data = (typeName *) REALGetControlData(instance, &defn)
#define ClassData(defn, instance, typeName, data) typeName *data = (typeName *) REALGetClassData(instance, &defn)
#define MobileControlData(defn, instance, typeName, data) typeName *data = (typeName *) REALGetMobileControlData(instance, &defn)

// pixel types
enum RBPixelType
{
	kRBPixelRGB24 = 1,			/// 3 bytes/pixel: Red, Green, Blue
	kRBPixelBGR24,				/// 3 bytes/pixel: Blue, Green, Red
	kRBPixelXRGB32,				/// 4 bytes/pixel: Unused, Red, Green, Blue
	kRBPixelBGRX32				/// 4 bytes/pixel: Blue, Green, Red, Unused
};
typedef enum RBPixelType RBPixelType;

// Possible values of the changedField paramter from the StateChangedFunction
enum REALControlStateChangedBits {
	kActivationChanged = 1,
	kEnabledChanged = 2,
	kVisibilityChanged = 4,
	kBoundsChanged = 8,
	kThemeChanged = 16
};


/**
* A view of the string's contents, with a known format.
*
* Care should be taken when using this structure, since it is not reference
* counted. Performing a copy of it and calling REALDisposeStringData on both
* will result in undefined behavior, crashes, and possibly summon dragons.
*
* @see REALGetStringData
*/
struct REALstringData {
	/** The bytes of the string in the required encoding. */
	const void *data;

	/** The length of the data, in bytes. */
	size_t length;

	/** The encoding of the data.
	*
	* This will always match what was passed to REALGetStringData and is
	* included here for convenience.
	*/
	uint32_t encoding;

	const void *private1;
	uint32_t private2;
};
typedef struct REALstringData REALstringData;

#ifdef __cplusplus
	extern "C" {
#endif
void PluginEntry(void);
#ifdef __cplusplus
}
#endif

#endif
