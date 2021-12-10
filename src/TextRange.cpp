//
//  TextRange.cpp
//
//  (c) 2021 Xojo, Inc. -- All Rights Reserved
//

#include "TextRange.h"

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
	{ nullptr, "Location", "Integer", 0, REALstandardGetter, REALstandardSetter, FieldOffset(TextRangeData, location) },
	{ nullptr, "Length", "Integer", 0, REALstandardGetter, REALstandardSetter, FieldOffset(TextRangeData, length) },
	{ nullptr, "EndLocation", "Integer", 0, (REALproc)TextRangeGetEnd, (REALproc)TextRangeSetEnd } 
};

static REALmethodDefinition sTextRangeMethods[] = {
	{ (REALproc)TextRangeConstructor, nullptr, "Constructor()" },
	{ (REALproc)TextRangeConstructorWRange, nullptr, "Constructor( location as integer, length as integer )" }
};

static REALclassDefinition sTextRangeClass = {
	kCurrentREALControlVersion,
	"TextRange",
	nullptr, // superclass
	sizeof(TextRangeData),
	0, // forSystemUse
	nullptr, // constructor
	nullptr, // destructor
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
	static REALclassRef rangeClass = REALGetClassRef(sTextRangeClass.name);
	REALobject object = REALnewInstance(rangeClass);
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
