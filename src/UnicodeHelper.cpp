//
//  UnicodeHelper.cpp
//
//  (c) 2013 Xojo, Inc. -- All Rights Reserved
//

#include "UnicodeHelper.h"

#include "rb_plugin.h"

// UTF8Encode
//	
//	Encode the given Unicode code point in UTF-8 form, followed by a
//	null terminator.  This requires up to 5 bytes.
//	
// Gets: uniChar -- Unicode code point (between 0 and 0x1FFFFF, inclusive)
//		 outBuf -- pointer to buffer at least 5 bytes long
// Returns: how many bytes were used (not counting the null), i.e., character length in bytes
static long UTF8Encode(unsigned long uniChar, unsigned char *outBuf)
{
	unsigned char *c = outBuf;

	// There are four cases, depending on what range the code point is in.
	if (uniChar < 0x80) {
		*c = (unsigned char)uniChar;
		c++;
	}

	else if (uniChar < 0x800) {
		*c = ( 0xC0 | (unsigned char)(uniChar >> 6) );
		c++;
		*c = ( 0x80 | (unsigned char)(uniChar & 0x3F) );
		c++;
	}

	else if (uniChar < 0x10000) {
		*c = ( 0xE0 | (unsigned char)(uniChar >> 12) );
		c++;
		*c = ( 0x80 | ((unsigned char)(uniChar >> 6) & 0x3F) );
		c++;
		*c = ( 0x80 | (unsigned char)(uniChar & 0x3F) );
		c++;
	}
	
	else if (uniChar < 0x200000) {
		*c = ( 0xF0 | (unsigned char)(uniChar >> 18) );
		c++;
		*c = ( 0x80 | ((unsigned char)(uniChar >> 12) & 0x3F) );
		c++;
		*c = ( 0x80 | ((unsigned char)(uniChar >> 6) & 0x3F) );
		c++;
		*c = ( 0x80 | (unsigned char)(uniChar & 0x3F) );
		c++;	
	}

	*c = 0;
	return c - outBuf;	
}

// UnicodeCharToString
//
//	Returns a REALstring representation of the unicode character
//
REALstring UnicodeCharToString( unsigned long uniChar )
{
	unsigned char buf[5];
	long charSize = UTF8Encode( uniChar, buf );
	return REALBuildString( (char *)buf, charSize, kREALTextEncodingUTF8 );
}
