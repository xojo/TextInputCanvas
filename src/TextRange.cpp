//
//  TextRange.cpp
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#include "TextRange.h"
typedef int RBInteger;

static void TextRangeConstructor( REALobject range );
static void TextRangeConstructorWRange( REALobject range, RBInteger location, RBInteger length );
static RBInteger TextRangeGetEnd( REALobject range );
static void TextRangeSetEnd( REALobject range, RBInteger end );

struct TextRangeData
{
	RBInteger location;
	RBInteger length;
};

static REALproperty sTextRangeProperties[] = {
	{ NULL, "Location", "Integer", 0, REALstandardGetter, REALstandardSetter, FieldOffset(TextRangeData, location) },
	{ NULL, "Length", "Integer", 0, REALstandardGetter, REALstandardSetter, FieldOffset(TextRangeData, length) },
	{ NULL, "EndLocation", "Integer", 0, (REALproc)TextRangeGetEnd, (REALproc)TextRangeSetEnd } 
};

static REALmethodDefinition sTextRangeMethods[] = {
	{ (REALproc)TextRangeConstructor, NULL, "Constructor()" },
	{ (REALproc)TextRangeConstructorWRange, NULL, "Constructor( location as integer, length as integer )" }
};

static REALclassDefinition sTextRangeClass = {
	kCurrentREALControlVersion,
	"TextRange",
	NULL, // superclass
	sizeof(TextRangeData),
	0, // forSystemUse
	NULL, // constructor
	NULL, // destructor
	sTextRangeProperties, sizeof(sTextRangeProperties) / sizeof(sTextRangeProperties[0]),
	sTextRangeMethods, sizeof(sTextRangeMethods) / sizeof(sTextRangeMethods[0])
};

static void TextRangeConstructor( REALobject range )
{
}

static void TextRangeConstructorWRange( REALobject range, RBInteger location, RBInteger length )
{
	TextRangeData *data = (TextRangeData *)REALGetClassData( range, &sTextRangeClass );
	data->location = location;
	data->length = length;
}

static RBInteger TextRangeGetEnd( REALobject range )
{
	TextRangeData *data = (TextRangeData *)REALGetClassData( range, &sTextRangeClass );
	return data->location + data->length;
}

static void TextRangeSetEnd( REALobject range, RBInteger end )
{
	TextRangeData *data = (TextRangeData *)REALGetClassData( range, &sTextRangeClass );
	data->length = end - data->location;
}

REALobject CreateTextRange( int begin, int length )
{
	REALobject object = REALnewInstance( sTextRangeClass.name );
	TextRangeData *data = (TextRangeData *)REALGetClassData( object, &sTextRangeClass );
	data->location = begin;
	data->length = length;
	return object;
}

void GetTextRangeInfo( REALobject range, int *begin, int *length )
{
	TextRangeData *data = (TextRangeData *)REALGetClassData( range, &sTextRangeClass );
	if (begin) *begin = data->location;
	if (length) *length = data->length;
}

void RegisterTextRangeClass()
{
	REALRegisterClass( &sTextRangeClass );
}
