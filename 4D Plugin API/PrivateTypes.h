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
// File : PrivateTypes.h
// Description : This file stores all the internal structures
//				 used by 4D Plugin API and useless for API user.
//
// rev : v13
//
// ---------------------------------------------------------------

#ifndef __PRIVATETYPES__
#define __PRIVATETYPES__


#ifdef __cplusplus
extern "C" {
#endif

// all the 4D Application internal structures use 2 bytes alignment
#pragma pack(push,2)

// used for query operations.
typedef struct LineBlock
{
	char				fOperator;
	short				fField;
	short				fTable;
	char				fComparison;
	union
	{
		char				fString[42];
		double				fReal;			
		PA_Date				fDate;
		PA_long32				fLongint;
		short				fInteger;
		char				fBoolean;
		PA_Unichar*			fUnichars;
	} uValue;
} LineBlock;

	
// This structure is always sent when calling back 4D Application.
// the different fields are used depending the kind of
// the action required.
typedef struct EngineBlock
{
// New v11 fields for Unicode support
	PA_Unichar			fUName[256];
	PA_Unichar			fUString[256];
	PA_Unistring		fUniString1;
	PA_Unistring		fUniString2;
	PA_Picture			fPicture;
	void*				fPtr1;
	void*				fPtr2;
	void*				fPtr3;
// all the following records are identical
// to the EngineBlock of 4D 2004
	short				fTable;
	short				fField;
	PA_long32				fRecord;
	char				fManyToOne;
	char				fOneToMany;
	char				fName[256];
	PA_Handle			fHandle;
	short				fError;
	sLONG_PTR			fParam1;
	sLONG_PTR			fParam2;
	sLONG_PTR			fParam3;
	sLONG_PTR			fParam4;
	double				fReal;	
	short				fFiller;
	PA_Date				fDate;
	PA_long32				fLongint;
	short				fShort;		
	char				fString[82];		
	short				fTextSize;
	PA_Handle			fTextHandle;
	char				fClearOldVariable;
	char				fNativeReal;
	short				fNbSearchLines;
} EngineBlock;

// facility to call back 4D more easily using the proc pointer 
#define Call4D(s,p) (*gCall4D)(s,p)

#if VERSIONMAC
	#define FOURDCALL __attribute__((visibility("default"))) void
#elif VERSIONWIN
	#define FOURDCALL void __stdcall
#endif

#if VERSIONMAC
	typedef void (*Call4DProcPtr)( short, EngineBlock* );
#elif VERSIONWIN
	typedef void (__stdcall *Call4DProcPtr)( short, EngineBlock* );
#endif

FOURDCALL FourDPackex( PA_long32 selector, void* params, void** data, void* result );

extern Call4DProcPtr gCall4D;

// this structure is sent to Plugin at init.
typedef struct PackInitBlock
{
	PA_long32			fVersion;
	PA_long32			fLength;
	PA_long32			fCPUType;
	Call4DProcPtr	fCall4D;
 	PA_long32			fSupportedVersion;
	Call4DProcPtr	fCall4Dex;
} PackInitBlock;

// reset struct alignment
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
