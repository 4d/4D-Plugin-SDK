/**
 * Copyright (c) 2017-present, 4D, Inc.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */


// ---------------------------------------------------------------
//
// 4D Plugin API
//
// File : Flags.h
// Description : 
// define compiler directive for the two supported platforms : 
// Windows and Mac PPC/Intel
//
// rev : v13
//
// ---------------------------------------------------------------

#ifndef __FLAGS__
#define __FLAGS__

#define PA_SMALLENDIAN 1
#define PA_BIGENDIAN 0
#define PA_64BITS_ARCHITECTURE 0

// _WIN32 is defined when compiling for Windows target (32bits as 64 bits)
#if defined(_WIN32)
	// See http://msdn.microsoft.com/en-us/library/b0084kay(v=VS.80).aspx for predefined macros on Visual
	#define VERSIONWIN 1
	#define VERSIONMAC 0
	#if defined (_WIN64)
		#undef PA_64BITS_ARCHITECTURE
		#define PA_64BITS_ARCHITECTURE 1
	#endif
// __APPLE__ is defined when compiling for Macintosh target
#elif defined(__APPLE__)
	#define VERSIONWIN 0
	#define VERSIONMAC 1
	#if defined(__BIG_ENDIAN__)
		#define PA_SMALLENDIAN 0
		#define PA_BIGENDIAN 1
	#endif 

    #undef PA_64BITS_ARCHITECTURE
    #define PA_64BITS_ARCHITECTURE __LP64__

#else
	#error "architecture not supported"
#endif

#endif
