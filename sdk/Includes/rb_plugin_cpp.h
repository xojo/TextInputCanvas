// rb_plugin_cpp.h
//
// This file is part of the REALbasic plugin API.
//
// (c) 2014 Xojo, Inc. -- All Rights Reserved
// See file "Plug-in License SDK.txt" for details.

#ifndef RB_PLUGIN_CPP_H
#define RB_PLUGIN_CPP_H

#include "REALplugin.h"

// Per the standard we should only have to look at __cplusplus, but some vendors
// don't set this correctly. On OS X it's also possible to have Clang set to
// compile as C++11 but use libstdc++ (which isn't a C++11 stdlib).
#if __cplusplus >= 201103L
	#if defined(__clang__)
		#include <ciso646>
		#if defined(_LIBCPP_VERSION)
			#define REAL_IS_CPP11 1
		#else
			#define REAL_IS_CPP11 0
		#endif
	#else
		#define REAL_IS_CPP11 1
	#endif
#elif defined(_MSC_VER)
	#define REAL_IS_CPP11 (_MSC_VER >= 1800)
#elif defined(__GNUC__)
	#if defined(__GXX_EXPERIMENTAL_CXX0X__)
		#define REAL_IS_CPP11 1
	#else
		#define REAL_IS_CPP11 0
	#endif
#else
	#define REAL_IS_CPP11 0
#endif

#if REAL_IS_CPP11
	#include <memory>
	extern "C" void REALDisposeStringData(REALstringData *data);
	extern "C" bool REALGetStringData(REALstring str, uint32_t encoding, REALstringData *outData);

	struct REALStringDisposer {
		void operator()(REALstringData *p) const
		{
			if (p) {
				REALDisposeStringData(p);
				delete p;
			}
		}
	};

	using REALStringDataPtr = std::unique_ptr<REALstringData, REALStringDisposer>;

	inline REALStringDataPtr REALGetStringData(REALstring str, uint32_t encoding)
	{
		REALstringData stackData;
		if (REALGetStringData(str, encoding, &stackData)) {
			return REALStringDataPtr(new REALstringData(stackData));
		}

		return nullptr;
	}
#endif	// C++11 check

REALstring REALBuildString( const char *contents, int byteCount, uint32_t encoding ) RB_WARN_UNUSED_RESULT;
REALobject REALnewInstance(REALclassRef classRef) RB_WARN_UNUSED_RESULT;

void REALLockPictureDescription(REALobject pic, REALpictureDescription *description);

#if TARGET_WIN32
	REALobject REALGetControlGraphics(REALcontrolInstance instance, HDC dc);
#endif

#endif	// RB_PLUGIN_CPP_H
