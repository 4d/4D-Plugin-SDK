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
// File : 4DPluginAPI.c
//
// rev : v14
//
// ---------------------------------------------------------------

#include "4DPluginAPI.h"
#include "PrivateTypes.h"
#include "EntryPoints.h"

#include <stdlib.h>
#include <string.h>

// gCall4D stores the address of a callback routine in 4D.
// this address is given by 4D when it calls the plugin for the first time.
Call4DProcPtr gCall4D = 0;


// 4D always calls to the plugin by calling a "Main" function on Macintosh
// and a "FourDPack" function on Windows.
// 4DPluginAPI implements this call in order to store the callback address
// then call a "PluginMain" function you must provide.

FOURDCALL FourDPackex( PA_long32 selector, void *parameters, void **data, void *result )
{
	PluginBlock params;
	params.fParameters = parameters;
	params.fResult = result;
	params.fData = data;

	// Package is initialized. It's time to read the callback address
	if ( ( selector == kServerInitPlugin ) || ( selector ==  kInitPlugin ) )
	{
		// get the callback address
		gCall4D = ( (PackInitBlock*)parameters )->fCall4Dex;
		( (PackInitBlock*)parameters )->fSupportedVersion = 0x00001400;
	}

	// Call "PluginMain" provided by the user of the 4DPluginAPI
	PluginMain( selector, &params );
}


// -----------------------------------------
//
// Error codes
//
// -----------------------------------------

#if TARGET_RT_MAC_MACHO
static PA_long32 sBinaryFormat = 'MACH';	// MachO only
#else
static PA_long32 sBinaryFormat = 0;	// CFM or Windows
#endif

#ifdef _WIN32
#ifdef _MSC_VER
#define THREAD_LOCAL __declspec(thread)
#else
#define THREAD_LOCAL __thread
#endif
#elif __APPLE__
#define THREAD_LOCAL __thread
#endif

THREAD_LOCAL static short sErrorCode = 0;

PA_ErrorCode PA_GetLastError()
{
	return (PA_ErrorCode) sErrorCode;
}

// -----------------------------------------
//
// 4D Application memory manager
//
// -----------------------------------------


PA_Handle PA_NewHandle( PA_long32 len )
{
	EngineBlock eb;

	eb.fParam1 = 1;
	eb.fParam2 = len;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fHandle;
}


void PA_DisposeHandle( PA_Handle handle )
{
	EngineBlock eb;

	eb.fError  = eER_NoErr;

	if ( handle )
	{
		eb.fParam1 = 2;
		eb.fHandle = handle;
		Call4D( EX_HANDLE_MANAGER, &eb );
	}

	sErrorCode = (PA_ErrorCode) eb.fError;
}


// return true if successfull
char PA_SetHandleSize( PA_Handle handle, PA_long32 newlen )
{
	EngineBlock eb;

	eb.fParam1 = 3;
	eb.fParam2 = newlen;
	eb.fHandle = handle;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) ( eb.fError ? 0 : 1 );
}


PA_long32 PA_GetHandleSize( PA_Handle handle )
{
	EngineBlock eb;

	eb.fParam1 = 4;
	eb.fParam2 = 0;
	eb.fHandle = handle;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam2;
}


char* PA_LockHandle( PA_Handle handle )
{
	EngineBlock eb;

	eb.fParam1 = 5;
	eb.fHandle = handle;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char*) eb.fParam3;
}


char PA_GetHandleState( PA_Handle handle )
{
	EngineBlock eb;

	eb.fParam1 = 7;
	eb.fHandle = handle;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam2;
}


void PA_SetHandleState( PA_Handle handle, char state )
{
	EngineBlock eb;

	eb.fParam1 = 8;
	eb.fParam2 = (sLONG_PTR)state;
	eb.fHandle = handle;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UnlockHandle( PA_Handle handle )
{
	EngineBlock eb;

	eb.fParam1 = 6;
	eb.fHandle = handle;
	eb.fError  = eER_NoErr;

	Call4D( EX_HANDLE_MANAGER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_MoveBlock( void *source, void *dest, PA_long32 len )
{
	char* ptsource;
	char* ptdest;

	if ( source == 0 || dest == 0 )
		return;

	ptsource = (char*) source;
	ptdest   = (char*) dest;

	if ( (ptsource < ptdest) && (ptsource + len > ptdest) )
	{
		ptsource += len - 1;
		ptdest += len - 1;

		while (--len >= 0)
			*ptdest-- = *ptsource--;
	}
	else
	{
		while (--len >= 0)
			*ptdest++ = *ptsource++;
	}
}

// -----------------------------------------
//
// Strings
//
// -----------------------------------------

// returns a NULL terminated UTF16 string
// you should NOT modify the content of the given string
// use PA_SetUnistring to modify the content of the string
PA_Unichar* PA_GetUnistring( PA_Unistring* ustr )
{
	return ustr->fString;
}

// returns the number of unicode characters in a string
// (do not count NULL ending character)
PA_long32 PA_GetUnistringLength( PA_Unistring* ustr )
{
	return ustr->fLength;
}

// creates a unistring from a NULL terminated UTF16 string 
PA_Unistring PA_CreateUnistring( PA_Unichar* ustr )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fHandle = (PA_Handle) ustr;
	Call4D( EX_CREATE_UNISTRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return eb.fUniString1;
}

void PA_DisposeUnistring( PA_Unistring* ustr )
{
	EngineBlock eb;
	eb.fUniString1 = *ustr;
	eb.fError = eER_NoErr;
	Call4D( EX_DISPOSE_UNISTRING, &eb );
	ustr->fLength = 0;
	ustr->fString = 0;
	ustr->fReserved1 = 0;
	ustr->fReserved2 = 0;
	sErrorCode = (PA_ErrorCode)eb.fError;

}

void PA_SetUnistring( PA_Unistring* ustr, PA_Unichar* str )
{
	EngineBlock eb;
	eb.fUniString1 = *ustr;
	eb.fHandle = (PA_Handle) str;
	eb.fError = eER_NoErr;
	Call4D( EX_SET_UNISTRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	*ustr = eb.fUniString1;
}

// return values:
// -1000 if the comparison is unrelevant
// -1 if text1 is smaller than text2,
// 0 if the strings are equals,
// 1 if text1 is greater than text2
//
// the only possible wild char is @
// xyz@ means "start with xyz"
// @xyz means "ends with xyz"
// @xyz@ means "contains xyz"
char PA_CompareUniBuffers( PA_Unichar* text1, PA_long32 len1, PA_Unichar* text2, PA_long32 len2, char diacritic, char useWildChar )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	sErrorCode = eER_NoErr;

	eb.fParam1 = (sLONG_PTR)text1;
	eb.fParam2 = (sLONG_PTR)text2;

	eb.fParam3 = len1;
	eb.fParam4 = len2;

	eb.fManyToOne = useWildChar;
	eb.fOneToMany = diacritic;

	if ( sErrorCode == eER_NoErr )
		Call4D( EX_COMPARE_UNIBUFFERS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam1;
}

char PA_CompareUnichars( PA_Unichar* string1, PA_Unichar* string2, char diacritic, char useWildChar )
{
	EngineBlock eb;
	sErrorCode = eER_NoErr;

	eb.fParam1 = (sLONG_PTR)string1;
	eb.fParam2 = (sLONG_PTR)string2;

	eb.fParam3 = PA_GetUnicharsLength( string1 );
	eb.fParam4 = PA_GetUnicharsLength( string2 );

	eb.fManyToOne = useWildChar;
	eb.fOneToMany = diacritic;

	if ( sErrorCode == eER_NoErr )
		Call4D( EX_COMPARE_UNIBUFFERS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam1;
}

char PA_CompareUnistrings( PA_Unistring* ustr1, PA_Unistring* ustr2, char diacritic, char useWildChar )
{
	EngineBlock eb;
	sErrorCode = eER_NoErr;
	eb.fError = eER_NoErr;
	eb.fParam1 = (sLONG_PTR)PA_GetUnistring( ustr1 );
	eb.fParam2 = (sLONG_PTR)PA_GetUnistring( ustr2 );

	eb.fParam3 = PA_GetUnistringLength( ustr1 );
	eb.fParam4 = PA_GetUnistringLength( ustr2 );

	eb.fManyToOne = useWildChar;
	eb.fOneToMany = diacritic;

	if ( sErrorCode == eER_NoErr )
		Call4D( EX_COMPARE_UNIBUFFERS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam1;
}

PA_long32 PA_GetUnicharsLength( PA_Unichar* str )
{
	PA_long32 ret=0;
	if (str)
	{
		PA_Unichar* pt = str;
		while ( *pt != 0 )
		{
			pt++;
		}
		ret = (PA_long32) (pt - str);
	}
	return ret;
}

void PA_CopyUnichars( PA_Unichar* source, PA_Unichar* dest, PA_long32 maxbytesize )
{
	if (dest)
	{
		while ( source && ( *source != 0 ) && ( maxbytesize > sizeof(PA_Unichar) ) )
		{
			*dest = *source;
			source++;
			dest++;
			maxbytesize -= sizeof(PA_Unichar);
		}

		*dest = 0;
	}
}


// -----------------------------------------
//
// Pictures
//
// -----------------------------------------

PA_Picture PA_CreatePicture( void* buffer, PA_long32 len )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;

	eb.fPtr1 = buffer;
	eb.fParam1 = len;
	Call4D( EX_CREATE_PICTURE, &eb );
	sErrorCode = eb.fError;

	return (void*) eb.fPicture;
}

// creates a CGImageRef on Mac, and a gdiplus::bitmap* on Windows
void* PA_CreateNativePictureForScreen( PA_Picture picture )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;

	eb.fPicture = picture;
	Call4D( EX_CREATE_NATIVE_PICTURE_FOR_SCREEN, &eb );
	sErrorCode = eb.fError;

	return (void*) eb.fHandle;
}

// creates a CGPDFDocumentRef on Mac, and a gdiplus::metafile* on Windows
void* PA_CreateNativePictureForPrinting( PA_Picture picture )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;

	eb.fPicture = picture;
	Call4D( EX_CREATE_NATIVE_PICTURE_FOR_PRINTING, &eb );
	sErrorCode = eb.fError;

	return (void*) eb.fHandle;
}

void PA_DisposePicture( PA_Picture picture )
{
	EngineBlock eb;
	sErrorCode = eER_NoErr;

	eb.fPicture = picture;
	Call4D( EX_DISPOSE_PICTURE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

}

// a PA_Picture can contains various representation of the same picture in different formats
// to ensure the best display depending on which platform the picture is displayed,
// or if the picture is displayed on screen or on printer

// you can iterate throw those representations by incrementing index until an error is returned
// the returned string will give informations about the format of the picture (JPEG, PNG...)
// if you pass a NULL handle, you are just collecting informations about the picture by examining the returned string
// when you pass a valid handle allocated by PA_NewHandle, the handle will be filled by the specific data of the picture,
// in the indexed picture format.

PA_Unistring PA_GetPictureData( PA_Picture picture, PA_long32 index, PA_Handle handle )
{
	EngineBlock eb;
	eb.fPicture = picture;
	eb.fHandle = handle;
	eb.fParam1 = index;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_PICTURE_DATA, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	return eb.fUniString1;
}

// pass 0 to retainOnly to duplicate the picture into a new one, or 1 to just retain the current picture
PA_Picture PA_DuplicatePicture( PA_Picture picture, char retainOnly )
{
	EngineBlock eb;
	eb.fPicture = picture;
	eb.fParam1 = retainOnly != 0 ? 0 : 1;
	eb.fError = eER_NoErr;

	Call4D( EX_DUPLICATE_PICTURE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fPicture;
}


// -----------------------------------------
//
// Objects
//
// -----------------------------------------

PA_ObjectRef PA_CreateObject()
{
	PA_ulong32 version = PA_Get4DVersion() & 0x0000FFFF;
	PA_Variable params[2], result;
	PA_ObjectRef object = NULL;
	PA_Unistring string;
	PA_Unichar json[] = {'{','}',0};
	
	if( version >= 0x00001630 )
	{
		result = PA_ExecuteCommandByID( 1471, NULL, 0 );	// New Object
		object = PA_GetObjectVariable( result );
	}
	else if( version >= 0x00001400 )
	{
		string = PA_CreateUnistring( json );
		PA_SetStringVariable( &params[0], &string );
		PA_SetLongintVariable( &params[1], eVK_Object );
		result = PA_ExecuteCommandByID( 1218, params, 2 );	// JSON Parse
		object = PA_GetObjectVariable( result );
		PA_DisposeUnistring( &string );
	}
	
	return object;
}

void PA_DisposeObject( PA_ObjectRef object )
{
	if( object )
	{
		PA_Variable	var;
		PA_SetObjectVariable( &var, object );
		PA_ClearVariable( &var );
	}
}

void PA_DisposeCollection(PA_CollectionRef collection)
{
	if (collection)
	{
		PA_Variable	var;
		PA_SetCollectionVariable(&var, collection);
		PA_ClearVariable(&var);
	}
}


PA_ObjectRef PA_DuplicateObject( PA_ObjectRef object )
{
	PA_Variable	params[1], result;
	
	if( object == NULL )
		return NULL;
	
	PA_SetObjectVariable( &params[0], object );
	result = PA_ExecuteCommandByID ( 1225, params, 1 );	// OB Copy
	return PA_GetObjectVariable ( result );
}

PA_CollectionRef PA_GetCollectionVariable(PA_Variable variable)
{
	PA_CollectionRef collection = NULL;

	if (variable.fType == eVK_Collection)
	{
		collection = (PA_CollectionRef)variable.uValue.fCollection;
	}

	return collection;
}


PA_CollectionRef PA_CreateCollection(void)
{
	PA_Variable result = PA_ExecuteCommandByID(1472, NULL, 0);	// New Collection
	return PA_GetCollectionVariable(result);
}


//The PA_Variable should be cleared after use
PA_Variable PA_GetCollectionElement(PA_CollectionRef collection, long index)
{
	PA_Variable value;
	EngineBlock eb;
	eb.fLongint = index;
	eb.fPtr1 = collection;
	eb.fPtr2 = &value;
	eb.fShort = eVK_Collection;
	eb.fError = eER_NoErr;

	Call4D(EX_GET_OBJ_VALUE, &eb);
	sErrorCode = eb.fError;

	return value;
}

void PA_SetCollectionElement(PA_CollectionRef collection, long index, PA_Variable value)
{
	EngineBlock eb;
	eb.fLongint = index;
	eb.fPtr1 = collection;
	eb.fPtr2 = &value;
	eb.fShort = eVK_Collection;
	eb.fError = eER_NoErr;

	Call4D(EX_SET_OBJ_VALUE, &eb);
	sErrorCode = eb.fError;

}

PA_long32 PA_GetCollectionLength(PA_CollectionRef collection)
{
	PA_Variable value;
	EngineBlock eb;
	PA_Unichar length[] = { 'l', 'e', 'n', 'g','t','h', 0 };
	PA_Unistring key = PA_CreateUnistring(&length[0]);
	eb.fLongint = -1;
	eb.fUniString1 = key;
	eb.fPtr1 = collection;
	eb.fPtr2 = &value;
	eb.fShort = eVK_Collection;
	eb.fError = eER_NoErr;

	Call4D(EX_GET_OBJ_VALUE, &eb);
	sErrorCode = eb.fError;
	PA_DisposeUnistring(&key);
	return value.uValue.fLongint;
}

//The PA_Variable should be cleared after use
PA_Variable PA_GetObjectProperty( PA_ObjectRef object, PA_Unistring* key )
{
	PA_Variable value;
	EngineBlock eb;
	eb.fUniString1 = *key;
	eb.fPtr1 = object;
	eb.fPtr2 = &value;
	eb.fShort = eVK_Object;
	eb.fError = eER_NoErr;

	Call4D(EX_GET_OBJ_VALUE, &eb);
	sErrorCode = eb.fError;

	return value;
}


void PA_SetObjectProperty( PA_ObjectRef object, PA_Unistring* key, PA_Variable value )
{

	EngineBlock eb;
	eb.fUniString1 = *key;
	eb.fPtr1 = object;
	eb.fPtr2 = &value;
	eb.fShort = eVK_Object;
	eb.fError = eER_NoErr;

	Call4D(EX_SET_OBJ_VALUE, &eb);
	sErrorCode = eb.fError;

}


void PA_RemoveObjectProperty( PA_ObjectRef object, PA_Unistring* key )
{
	PA_Variable params[2];
	
	PA_SetObjectVariable( &params[0], object );
	PA_SetStringVariable( &params[1], key );
	
	PA_ExecuteCommandByID( 1226, params, 2 );	// OB REMOVE
}


char PA_HasObjectProperty( PA_ObjectRef object, PA_Unistring* key )
{
	PA_Variable params[2], result;
	
	PA_SetObjectVariable( &params[0], object );
	PA_SetStringVariable( &params[1], key );
	
	result = PA_ExecuteCommandByID( 1231, params, 2 );	// OB Is defined
	
	return PA_GetBooleanVariable( result );
}


PA_VariableKind  PA_GetObjectPropertyType( PA_ObjectRef object, PA_Unistring* key )
{
	PA_Variable params[2], result;
	PA_VariableKind type;
	
	PA_SetObjectVariable( &params[0], object );
	PA_SetStringVariable( &params[1], key );
	
	result = PA_ExecuteCommandByID( 1230, params, 2 );	// OB Get type
	type = (PA_VariableKind) PA_GetLongintVariable( result );
	
	if( type == eVK_Text )
		type = eVK_Unistring;
	
	return type;
}


PA_Variable PA_JsonParse( PA_Unistring* json, PA_VariableKind kind )
{
	PA_Variable params[2], result;
	
	if( kind == eVK_ArrayObject )
	{
		PA_SetStringVariable( &params[0], json );
		params[1] = PA_CreateVariable( eVK_ArrayObject );
		PA_ExecuteCommandByID( 1219, params, 2 );	// JSON PARSE ARRAY
		result = params[1];
	}
	else
	{
		PA_SetStringVariable( &params[0], json );
		PA_SetLongintVariable( &params[1], kind );
		result = PA_ExecuteCommandByID( 1218, params, 2 );	// JSON Parse
	}
	
	return result;
}


PA_Unistring PA_JsonStringify( PA_Variable value, char prettyPrint )
{
	PA_Variable params[2], result;
	
	params[0] = value;
	PA_SetOperationVariable( &params[1], '*' );

	if( PA_IsArrayVariable( &value ) )
	{
		result = PA_ExecuteCommandByID( 1228, params, prettyPrint ? 2 : 1 );	// JSON Stringify array
	}
	else
	{
		result = PA_ExecuteCommandByID( 1217, params, prettyPrint ? 2 : 1 );	// JSON Stringify
	}
	
	return PA_GetStringVariable( result );
}


// -----------------------------------------
//
// static internal functions
//
// -----------------------------------------

static PA_long32 ToUserData( PA_Handle handle, void* dest )
{
	EngineBlock eb;

	if ( handle == 0 )
		return 0;

	eb.fParam1 = 4;
	eb.fParam2 = 0;
	eb.fParam3 = (sLONG_PTR)dest;
	eb.fParam4 = 0;
	eb.fHandle = handle;
	eb.fError = eER_NoErr;
	Call4D( EX_CONVERT_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam4;
}


static PA_Handle FromUserData( void* data, PA_long32 len )
{
	EngineBlock eb;

	if ( data == 0 )
		return 0;

	eb.fParam1 = 5;
	eb.fParam2 = 0;
	eb.fParam3 = (sLONG_PTR)data;
	eb.fParam4 = len;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_CONVERT_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fHandle;
}


static char StripInterprocessVariableName( PA_Unichar* variableName )
{
	// check interprocess variable
	if ( ( variableName[0] == '<' ) && ( variableName[1] == '>' ) )
	{
		// remove '<>' before variable name
		PA_CopyUnichars( variableName + 2, variableName, 256 );
		return 1;
	}
	else
		return 0;
}


// --------------------------------------------
//
// 4D Application informations
//
// --------------------------------------------


PA_ulong32 PA_Get4DVersion()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_ulong32)eb.fParam2;
}


char PA_IsDemoVersion()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( ( (uLONG_PTR) eb.fHandle ) & 0x20000000 ) != 0 ) ? 1 : 0 );
}


char PA_IsDatabaseLocked()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( ( (uLONG_PTR) eb.fHandle ) & 0x40000000 ) != 0 ) ? 1 : 0 );
}


char PA_IsCompiled( char hostDatabase)
{
	PA_long32 flag = hostDatabase ? 0x00800000 : 0x08000000;

	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( eb.fParam1 & flag ) != 0 ) ? 1 : 0 );
}


char PA_Is4DClient()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( eb.fParam1 & 0x04000000 ) != 0 ) ? 1 : 0 );
}


char PA_Is4DServer()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( eb.fParam1 & 0x02000000 ) != 0 ) ? 1 : 0 );
}


char PA_IsWebProcess()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( eb.fParam1 & 0x01000000 ) != 0 ) ? 1 : 0 );
}


char PA_GetMessagesStatus()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( eb.fParam1 & 0x10000000 ) != 0 ) ? 1 : 0 );
}


void PA_SetMessagesStatus( char showMessages )
{
	EngineBlock eb;
	eb.fParam1 = showMessages;
	eb.fError = eER_NoErr;
	Call4D( EX_MESSAGES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_SetThermometerRect( PA_Rect rect )
{
	EngineBlock eb;
	eb.fParam1 = rect.fLeft;
	eb.fParam2 = rect.fTop;
	eb.fParam3 = rect.fRight;
	eb.fParam4 = rect.fBottom;
	eb.fError  = eER_NoErr;
	Call4D( EX_THERMOMETER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_Is4DMono()
{
	EngineBlock eb;
	eb.fParam3=(sLONG_PTR)'FAST';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( ( ( eb.fParam1 & 0x06000000 ) == 0 ) ? 1 : 0 );
}


void PA_GetStructureName( PA_Unichar* structName )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUName, structName, sizeof(eb.fUName) );
}


void PA_GetDataName( PA_Unichar* dataName )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_INFORMATION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, dataName, sizeof(eb.fUString) );
}


// On MacOS, returns the path of the package
PA_Unistring PA_GetApplicationFullPath()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_APPLICATION_FULLPATH, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}


PA_Unistring PA_GetStructureFullPath()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_STRUCTURE_FULLPATH, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}


// warning : two packages can share the same ID,
// exept for modules with ID under 15000 like 4D Write
short PA_FindPackage( short packageID )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR)packageID;
	eb.fParam2 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_FIND_PACKAGE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (short) eb.fParam2;
}


void PA_PackageInfo( short packageNumber, short* packageID, void** procPtr, void** data, short* resfile, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fParam1 = packageNumber;
	eb.fError = eER_NoErr;
	Call4D( EX_PACKAGE_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( packageID )
		*packageID = (short) eb.fParam1;

	if ( procPtr )
		*procPtr = (void*) eb.fParam2;

	if ( resfile )
		*resfile = (short) eb.fParam4;

	if ( data )
		*data = (void*) eb.fHandle;

	PA_CopyUnichars( eb.fUName, name, sizeof(eb.fUName) );
}


void PA_Quit4D()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_QUIT4D, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_Unistring PA_Get4DPreferencesFilePath()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_PREF_FILE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}


PA_Unistring PA_Get4Dfolder( PA_FolderKind folderKind, char createIfNotFound )
{
	EngineBlock eb;
	eb.fParam1 = (sLONG_PTR) folderKind;
	eb.fParam2 = (sLONG_PTR) createIfNotFound;
	eb.fParam3 = (sLONG_PTR) 'G4DF';
	eb.fError = eER_NoErr;
	Call4D( EX_GET_4D_FOLDER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}


PA_PlatformInterface PA_GetPlatformInterface()
{
	EngineBlock eb;
	eb.fParam1 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_PLATFORM_INTERFACE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (PA_PlatformInterface) eb.fParam1;
}


void PA_GetToolBarInfo( char* displayed, short* toolbarHeight )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_TOOLBAR_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( displayed )
		*displayed = (char) eb.fParam1;

	if ( toolbarHeight )
		*toolbarHeight = (short) eb.fParam2;
}


// 0 : hide, 1: show, 2 : Switch
void PA_ShowHideToolBar( char displayed )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = displayed;
	Call4D( EX_SHOW_HIDE_TOOLBAR, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


char PA_GetTipsEnabled()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = 0;
	Call4D( EX_GET_TIPS_ENABLED, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam1;
}


void PA_SetTipsEnabled( char enabled )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = enabled;
	Call4D( EX_SET_TIPS_ENABLED, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// this function checks if the stack can hold the amount of stack passed in parameter
// if it can't 4D displays an alert. The amount of stack available is returned.
// if you just want to know the amount of available stack, pass 0 as requested stack
// error code
PA_long32 PA_CheckFreeStack( PA_long32 requestedStack )
{
	EngineBlock eb;

	eb.fParam1 = requestedStack;
	eb.fParam2 = 0;
	eb.fError  = eER_NoErr;
	Call4D( EX_CHECK_FREE_STACK, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam2;
}


void PA_GetCenturyInfo( PA_long32* pivotYear, PA_long32* defaultCentury )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_CENTURY_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	*pivotYear      = (PA_long32)eb.fParam1;
	*defaultCentury = (PA_long32)eb.fParam2;
}

// on Windows only, returns 4D application HINSTANCE
void* PA_Get4DHInstance()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_4D_HINSTANCE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (void*)eb.fParam1;
}

// -----------------------------------------
//
// Virtual structure
//
// -----------------------------------------


static short sVirtualStructureMode = 0;


void PA_UseVirtualStructure()
{
	sVirtualStructureMode = 911;
}


void PA_UseRealStructure()
{
	sVirtualStructureMode = 0;
}


char PA_VirtualStructureDefined()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_VIRTUAL_STRUCTURE_DEFINED, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam1;
}


short PA_GetTrueTableNumber( short virtualTable )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fTable = virtualTable;
	Call4D( EX_GET_TRUE_TABLE_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


void PA_GetTrueFieldNumber( short virtualTable, short virtualField, short* trueTable, short* trueField )
{
	EngineBlock eb;

	eb.fTable = virtualTable;
	eb.fField = virtualField;

	Call4D( EX_GET_TRUE_FIELD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( trueTable )
		*trueTable = eb.fTable;

	if ( trueField )
		*trueField = eb.fField;
}


short PA_GetVirtualTableNumber( short trueTable )
{
	EngineBlock eb;

	eb.fTable = trueTable;
	Call4D( EX_GET_VIRTUAL_TABLE_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


void PA_GetVirtualFieldNumber( short trueTable, short trueField, short* virtualTable, short* virtualField )
{
	EngineBlock eb;

	eb.fTable = trueTable;
	eb.fField = trueField;

	Call4D( EX_GET_VIRTUAL_FIELD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( virtualTable )
		*virtualTable = eb.fTable;

	if ( virtualField )
		*virtualField = eb.fField;
}




// -----------------------------------------
//
// automatic relations
//
// -----------------------------------------


static char sManyToOne = 1;
static char sOneToMany = 1;

void PA_UseAutomaticRelations( char manyToOne, char oneToMany )
{
	sManyToOne = manyToOne;
	sOneToMany = oneToMany;
}


// -----------------------------------------
//
// Selection
//
// -----------------------------------------


void PA_CreateRecord( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = eER_NoErr;

	Call4D( EX_CREATE_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SaveRecord( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = eER_NoErr;

	Call4D( EX_SAVE_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_NextRecord( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = eER_NoErr;

	Call4D( EX_NEXT_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_PreviousRecord( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = eER_NoErr;

	Call4D( EX_PREVIOUS_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

// record is from 1 to number of records in selection
void PA_GotoSelectedRecord( short table, PA_long32 record )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fRecord    = record - 1;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = eER_NoErr;

	Call4D( EX_GOTO_SELECTED_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_SelectedRecordNumber( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fRecord    = 0;
	eb.fError     = eER_NoErr;

	Call4D( EX_SELECTED_RECORD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam1;
}


PA_long32 PA_RecordsInSelection( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_RECORDS_IN_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam1;
}


void PA_DeleteSelection( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = eER_NoErr;

	Call4D( EX_DELETE_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_FirstRecord( short table )
{
	EngineBlock eb;

	eb.fTable     = table;
	eb.fManyToOne = sManyToOne;
	eb.fOneToMany = sOneToMany;
	eb.fError     = 0;

	Call4D( EX_FIRST_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_BeforeSelection( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_BEFORE_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;	
}


char PA_EndSelection( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_END_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


void PA_CreateElementsFromXMLDefinition( PA_Unistring* xmlDefinition)
{
	EngineBlock eb;

	eb.fUniString1	= *xmlDefinition;
	eb.fError		= 0;

	Call4D( EX_CREATE_ELEMENTS, &eb);

	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_GotoRecord( short table, PA_long32 record )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fRecord = record;
	eb.fError  = eER_NoErr;

	Call4D( EX_GOTO_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_RecordNumber( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fRecord = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_RECORD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fRecord;
}


char PA_Locked( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = eER_NoErr;

	Call4D( EX_LOCKED, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


void PA_ReadWrite( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_READ_WRITE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_ReadOnly( short table )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fParam1 = 1;
	eb.fError  = eER_NoErr;

	Call4D( EX_READ_WRITE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_LoadRecord( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = eER_NoErr;

	Call4D( EX_LOAD_RECORD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RelateOne( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = eER_NoErr;

	Call4D( EX_RELATE_ONE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RelateMany( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fError = eER_NoErr;

	Call4D( EX_RELATE_MANY, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RelateOneSelection( short manyTable, short oneTable )
{
	EngineBlock eb;

	eb.fTable = manyTable;
	eb.fField = oneTable;
	eb.fError = eER_NoErr;
	Call4D( EX_RELATE_ONE_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_RelateManySelection( short manyTable, short manyField )
{
	EngineBlock eb;

	eb.fTable = manyTable;
	eb.fField = manyField;
	eb.fError = eER_NoErr;

	Call4D( EX_RELATE_MANY_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// Tables
//
// -----------------------------------------


short PA_CountTables()
{
	EngineBlock	eb;
	
	eb.fParam1 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError = eER_NoErr;
	Call4D( EX_COUNT_TABLES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (short) eb.fParam1;
}


void PA_GetTableAndFieldNumbers( PA_Unichar* tableAndFieldNames, short* table, short* field )
{
	EngineBlock	eb;
	
	eb.fTable = 0;
	eb.fField = 0;
	PA_CopyUnichars( tableAndFieldNames, eb.fUName, sizeof(eb.fUName) );
	eb.fParam4 = sVirtualStructureMode;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_TABLE_FIELD_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( table )
		*table = eb.fTable;

	if ( field )
		*field = eb.fField;
}


void PA_GetTableName( short table, PA_Unichar* tableName )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_TABLE_NAME, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( tableName )
	{
		if ( sErrorCode == eER_NoErr )
			PA_CopyUnichars( eb.fUName, tableName, sizeof(eb.fUName) );
		else
			*tableName = 0;
	}
}


char PA_IsTableVisible( short table )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_TABLE_NAME, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


// -----------------------------------------
//
// Fields
//
// -----------------------------------------

short PA_CountFields( short table)
{
	EngineBlock	eb;
	
	eb.fTable  = table;
	eb.fParam1 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = eER_NoErr;

	Call4D(sVirtualStructureMode ? EX_COUNT_FIELDS:EX_COUNT_INVISIBLE_FIELDS_TOO, &eb);
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (short) eb.fParam1;
}


void PA_GetFieldName( short table, short field, PA_Unichar* fieldName )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_FIELD_ATTRIBUTES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( fieldName )
	{
		if ( sErrorCode == eER_NoErr )
			PA_CopyUnichars( eb.fUName, fieldName, sizeof(eb.fUName) );
		else
			*fieldName = 0;
	}
}

PA_VariableKind UnifyVarKind(int inKind)
{
	PA_VariableKind ret=eVK_Undefined;

	switch(inKind)
	{
	case 0:		// Field of type alpha
	case 2:		// C_TEXT
	case 24:	// C_String
		ret=eVK_Unistring;
		break;

	case 18:	// C_TEXT
	case 21:	// C_String
		ret=eVK_ArrayUnicode;
		break;

	default:
		ret=(PA_VariableKind)inKind;
		break;
	}

	return ret;
}

void PA_GetPointerValueProperties( PA_Pointer inPointer, PA_VariableKind* outKind, PA_long32* outStringLength, PA_long32* outArraySize)
{
	EngineBlock	eb;
	sErrorCode = eER_NoErr;
	if (outKind)
		*outKind=eVK_Undefined;
	if (PA_GetPointerKind(inPointer)==ePK_PointerToField)
	{
		short stringLength=0;
		PA_FieldKind fieldKind=eFK_InvalidFieldKind;
		PA_GetFieldProperties(inPointer->uValue.fTableField.fTable,inPointer->uValue.fTableField.fField,&fieldKind,&stringLength,0,0);
		if (outStringLength)
			*outStringLength=stringLength;
		if (outKind)
		{
			switch(fieldKind)
			{
			case eFK_TextField:
			case eFK_AlphaField:
				*outKind=eVK_Unistring;
				break;

			case eFK_RealField:
			case eFK_FloatField:
				*outKind=eVK_Real;
				break;

			case eFK_PictureField:
				*outKind=eVK_Picture;
				break;

			case eFK_DateField:
				*outKind=eVK_Date;
				break;

			case eFK_BooleanField:
				*outKind=eVK_Boolean;
				break;

			case eFK_IntegerField:
				*outKind=eVK_Integer;
				break;

			case eFK_LongintField:
			case eFK_Long8:
				*outKind=eVK_Longint;
				break;

			case eFK_TimeField:
				*outKind=eVK_Time;
				break;

			case eFK_BlobField:
				*outKind=eVK_Blob;
				break;
					
			case eFK_ObjectField:
				*outKind=eVK_Object;
				break;

			default:
				*outKind=eVK_Undefined;
				break;
			}
		}
	}
	else
	{

		if ( inPointer )
		{
			eb.fParam1 = (sLONG_PTR)inPointer;
			Call4D( EX_GET_POINTER_VALUE_PROPERTIES, &eb );
			sErrorCode = eb.fError;

			if ( outKind )
				*outKind=UnifyVarKind((short) eb.fParam1);

			if ( outStringLength )
				*outStringLength = (PA_long32)eb.fParam2;

			if ( outArraySize )
				*outArraySize = (PA_long32)eb.fParam3;
		}

	}
}

void PA_GetFieldProperties( short table, short field, PA_FieldKind* kind, short* stringlength, char* indexed, PA_long32* attributes )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam1 = eFK_InvalidFieldKind;
	eb.fParam2 = 0;
	eb.fParam3 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fHandle = 0;		// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_FIELD_ATTRIBUTES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( kind )
		*kind = (PA_FieldKind) (short) eb.fParam1;

	if ( stringlength )
		*stringlength = (short) eb.fParam2;

	if ( indexed )
		*indexed = (char) eb.fParam3;

	if ( attributes )
		*attributes = (PA_long32)eb.fParam4;
}


void PA_GetFieldRelation( short table, short field, short* relatedTable, short* relatedField )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam1 = 0;
	eb.fParam2 = 0;
	eb.fHandle = 0;		// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD_RELATIONS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( relatedTable )
		*relatedTable = (short) eb.fParam1;

	if ( relatedField )
		*relatedField = (short) eb.fParam2;
}


void PA_GetFieldList( short table, short field, PA_Unichar* listName )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;		// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD_RELATIONS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( listName )
	{
		if ( sErrorCode == eER_NoErr )
			PA_CopyUnichars( eb.fUName, listName, sizeof(eb.fUName) );
		else
			*listName = 0;
	}
}


void PA_TableAndFieldPopup( short x, short y, short* table, short* field )
{
	EngineBlock eb;

	eb.fTable = 0;
	eb.fField = 0;
	eb.fParam1 = x;
	eb.fParam2 = y;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError  = eER_NoErr;

	Call4D( EX_POPUP_TABLE_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( table )
		*table = eb.fTable;

	if ( field )
		*field = eb.fField;

}


void PA_FieldPopup( short x, short y, short table, short* field )
{
	EngineBlock eb;

	eb.fTable  = table;
	eb.fField  = 0;
	eb.fParam1 = x;
	eb.fParam2 = y;
	eb.fParam4 = sVirtualStructureMode;
	eb.fError  = eER_NoErr;

	Call4D( EX_POPUP_FIELD_LIST, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( field )
		*field = eb.fField;
}



// -----------------------------------------
//
//  Order by
//
// -----------------------------------------


void PA_OrderBy( short table, short field, char ascending )
{
	EngineBlock* eb;
	LineBlock* lineblock;
	PA_OrderByRef h;
	
	h = PA_NewHandle( sizeof( EngineBlock ) + sizeof( LineBlock ) );
	if ( h )
	{
		eb = (EngineBlock*) PA_LockHandle( h );
		eb->fTable = table;
		eb->fField = field;
		eb->fNbSearchLines = 1;
		eb->fError  = 0;

		lineblock = (LineBlock*) ( sizeof( EngineBlock ) + (char*) eb );

		lineblock->fTable = table;
		lineblock->fField = field;

		if ( ascending == 0 )
			lineblock->fComparison = (char) eQC_IsLess;
		else
			lineblock->fComparison = (char) eQC_IsGreater;

		Call4D( EX_ORDER_BY, eb );

		PA_UnlockHandle( h );
		PA_DisposeHandle( h );

		sErrorCode = (PA_ErrorCode) eb->fError;
	}
}


// create an empty PA_OrderByRef
PA_OrderByRef PA_OpenOrderBy( short table )
{
	PA_OrderByRef ref = 0;
	EngineBlock* pt;

	ref = PA_NewHandle( sizeof( EngineBlock ) );
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		pt->fTable  = table;
		pt->fError  = 0;
		pt->fNbSearchLines = 0;
		PA_UnlockHandle( ref );
		sErrorCode = eER_NoErr;
	}

	return ref;
}


// add a specific order criteria
void PA_AddFieldToOrderBy( PA_OrderByRef ref, short table, short field, char ascending )
{
	EngineBlock* pt;
	LineBlock* lineblock;
	short nb;
	
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		nb = pt->fNbSearchLines;
		PA_UnlockHandle( ref );

		if ( nb < 32000 )
		{
			if ( PA_SetHandleSize( ref, (PA_long32)( PA_GetHandleSize( ref ) + sizeof( LineBlock ) ) ) )
			{
				pt = (EngineBlock*) PA_LockHandle( ref );
				pt->fNbSearchLines++;

				lineblock = (LineBlock*) ( ( nb * (PA_long32) sizeof( LineBlock ) ) + sizeof( EngineBlock ) + (char*) pt );

				lineblock->fTable = table;
				lineblock->fField = field;

				if ( ascending == 0 )
					lineblock->fComparison = (char) eQC_IsLess;
				else
					lineblock->fComparison = (char) eQC_IsGreater;
			
				PA_UnlockHandle( ref );
				sErrorCode = eER_NoErr;
			}
		}
		else
			sErrorCode = eER_TooManyOrderBy;
	}
	else
		sErrorCode = eER_InvalidOrderByRef;
}


// perform the order by, and dispose the PA_OrderBy ref
void PA_CloseOrderBy( PA_OrderByRef ref )
{
	EngineBlock* pt;
	short tmpErr;

	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		if ( sErrorCode == eER_NoErr )
		{
			Call4D( EX_ORDER_BY, pt );

			tmpErr = pt->fError;

			PA_UnlockHandle( ref );
			PA_DisposeHandle( ref );

			sErrorCode = tmpErr;
		}
	}
	else
		sErrorCode = eER_InvalidOrderByRef;
}


// -----------------------------------------
//
// Query
//
// -----------------------------------------


PA_QueryRef PA_OpenQuery( short table )
{
	PA_QueryRef ref = 0;
	EngineBlock* pt;

	ref = PA_NewHandle( sizeof( EngineBlock ) );
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		pt->fTable = table;
		pt->fError = 0;
		pt->fNbSearchLines = 0;
		PA_UnlockHandle( ref );
		sErrorCode = eER_NoErr;
	}

	return ref;
}


static LineBlock* AddQuery( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc )
{
	EngineBlock* pt;
	LineBlock* lineblock = 0;
	short nb;
	
	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );
		nb = pt->fNbSearchLines;
		PA_UnlockHandle( ref );

		if ( nb < 32000 )
		{
			if ( PA_SetHandleSize( ref, (PA_long32)( PA_GetHandleSize( ref ) + sizeof( LineBlock ) ) ) )
			{
				pt = (EngineBlock*) PA_LockHandle( ref );
				pt->fNbSearchLines++;

				lineblock = (LineBlock*) ( ( nb * (PA_long32) sizeof( LineBlock ) ) + sizeof( EngineBlock ) + (char*) pt );

				lineblock->fTable = table;
				lineblock->fField = field;

				// first query operator should always be eQO_NoOperator
				if ( nb == 0 )
					lineblock->fOperator = (char) eQO_NoOperator;
				else
					lineblock->fOperator = (char) qo;

				lineblock->fComparison = (char) qc;
			}
		}
		else
			sErrorCode = eER_TooManyQueries;
	}
	else
		sErrorCode = eER_InvalidQueryRef;

	return lineblock;
}


// add a query for a string value
// the string pointer should NOT be disposed until the request is performed because the pointer is passed to the query,
// not the content.
void PA_QueryString( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, PA_Unichar* string )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fUnichars = string;
		PA_UnlockHandle( ref );
	}
}

// add a query for a real value
void PA_QueryReal( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, double value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fReal = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for a time value
void PA_QueryTime( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, PA_long32 value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fLongint = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for a date value
void PA_QueryDate( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short day, short month, short year )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fDate.fDay   = day;
		lineblock->uValue.fDate.fMonth = month;
		lineblock->uValue.fDate.fYear  = year;
		PA_UnlockHandle( ref );
	}
}


// add a query for a longint value
void PA_QueryLongint( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, PA_long32 value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fLongint = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for an integer value
void PA_QueryInteger( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fInteger = value;
		PA_UnlockHandle( ref );
	}
}


// add a query for a Boolean value
void PA_QueryBoolean( PA_QueryRef ref, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, char value )
{
	LineBlock* lineblock = AddQuery( ref, table, field, qo, qc );
	if ( lineblock )
	{
		lineblock->uValue.fBoolean = value;
		PA_UnlockHandle( ref );
	}
}


// stop recording, and perform the query.
void PA_CloseQuery( PA_QueryRef ref )
{
	EngineBlock* pt;
	short tmpErr;

	if ( ref )
	{
		pt = (EngineBlock*) PA_LockHandle( ref );

		if ( sErrorCode == eER_NoErr )
		{
			Call4D( EX_QUERY, pt );
			tmpErr = (PA_ErrorCode) pt->fError;

			PA_UnlockHandle( ref );
			PA_DisposeHandle( ref );

			sErrorCode = tmpErr;
		}
	}
	else
		sErrorCode = eER_InvalidQueryRef;
}



// --------------------------------------------------------------
//
// Get Parameters from a plugin commmand (start at index 1)
//
// --------------------------------------------------------------


short PA_GetShortParameter( PA_PluginParameters params, short index )
{
	return *( ( (short**) params->fParameters ) [ index - 1 ] );
}


PA_long32 PA_GetLongParameter( PA_PluginParameters params, short index )
{
	return *( ( (PA_long32**) params->fParameters ) [ index - 1 ] );
}


double PA_GetDoubleParameter( PA_PluginParameters params, short index )
{
	return *( ( (double**) params->fParameters ) [ index - 1 ] );
}

// Use PA_SetUnistring or PA_GetUnistring to set or read the parameter content
PA_Unistring* PA_GetStringParameter( PA_PluginParameters params, short index )
{
	return ( ( (PA_Unistring**) params->fParameters ) [ index - 1 ] );
}


PA_Picture PA_GetPictureParameter( PA_PluginParameters params, short index )
{
	return *( ( (PA_Picture**) params->fParameters ) [ index - 1 ] );
}


PA_long32 PA_GetBlobParameter( PA_PluginParameters params, short index, void* blob )
{
	return ToUserData( *( ( (PA_Handle**) params->fParameters ) [ index - 1 ] ), blob );
}


PA_Handle PA_GetBlobHandleParameter( PA_PluginParameters params, short index )
{
	return *( ( (PA_Handle**) params->fParameters ) [ index - 1 ] );
}


void PA_GetDateParameter( PA_PluginParameters params, short index, short* day, short* month, short* year )
{
	PA_Date* ptdate;

	ptdate = ( (PA_Date**) params->fParameters ) [ index - 1 ];

	if ( day )
		*day   = ptdate->fDay;

	if ( month )
		*month = ptdate->fMonth;

	if ( year )
		*year  = ptdate->fYear;
}


PA_long32 PA_GetTimeParameter( PA_PluginParameters params, short index )
{
	return *( ( (PA_long32**) params->fParameters ) [ index - 1 ] );
}


PA_Variable PA_GetVariableParameter( PA_PluginParameters params, short index )
{
	PA_Variable var;

	var = *( ( (PA_Variable**) params->fParameters ) [ index - 1 ] );
	
	return var;
}


PA_Pointer PA_GetPointerParameter( PA_PluginParameters params, short index )
{
	if ( PA_IsCompiled( 1) )
	{
		PA_Variable* ptvar;

		ptvar = ( (PA_Variable**) params->fParameters ) [ index - 1 ];

		if ( ptvar->fType == eVK_Pointer )
		{
			if (( ptvar->uValue.fPointer == 0 ))	// m.c
				return 0;
			
			return *ptvar->uValue.fPointer;
		}
		else
			return 0;
	}
	else
		return ( (PA_Pointer*) params->fParameters ) [ index - 1 ];
}


PA_Variable PA_GetPointerValue( PA_Pointer pointer )
{
	PA_Variable variable;
	EngineBlock eb;

	sErrorCode = eER_NoErr;

	if ( pointer )
	{
		eb.fHandle = (PA_Handle) &variable;
		eb.fParam1 = (sLONG_PTR)pointer;
		Call4D( EX_GET_POINTER_VALUE, &eb );
		sErrorCode = eb.fError;
	}
	else
		variable.fType = eVK_Undefined;

	return variable;
}

PA_PointerKind PA_GetPointerKind( PA_Pointer pointer )
{
	if ( pointer == 0 )
		return ePK_InvalidPointer;

	if ( pointer->fClass == 1 )
		return ePK_PointerToVariable;
	else if ( pointer->uValue.fTableField.fField == 0 )
		return ePK_PointerToTable;
	else
		return ePK_PointerToField;
}

PA_ObjectRef PA_GetObjectParameter(PA_PluginParameters params, short index)
{
	return *(((PA_ObjectRef**)params->fParameters)[index - 1]);
}

PA_CollectionRef PA_GetCollectionParameter(PA_PluginParameters params, short index)
{
	return *(((PA_CollectionRef**)params->fParameters)[index - 1]);
}

// do NOT call PA_ClearVariable after this call, the variable content now belongs to 4D...
void PA_SetPointerValue( PA_Pointer pointer, PA_Variable variable )
{
	EngineBlock eb;

	sErrorCode = eER_NoErr;

	if ( pointer )
	{
		eb.fHandle = (PA_Handle) &variable;
		eb.fParam1 = (sLONG_PTR)pointer;
		Call4D( EX_SET_POINTER_VALUE, &eb );
		sErrorCode = eb.fError;
	}
	else
		sErrorCode = eER_NotAVariablePointer;
}

// ---------------------------------------------------------------
//
// Set Parameters from a plugin command (start at index 1)
//
// ---------------------------------------------------------------


void PA_SetShortParameter( PA_PluginParameters params, short index, short value )
{
	*( ( (short**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetLongParameter( PA_PluginParameters params, short index, PA_long32 value )
{
	*( ( (PA_long32**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetDoubleParameter( PA_PluginParameters params, short index, double value )
{
	*( ( (double**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetPictureParameter( PA_PluginParameters params, short index, PA_Picture picture )
{
	PA_Picture* ptpict;
	
	ptpict = ( (PA_Picture**) params->fParameters ) [ index - 1 ];

	if ( *ptpict )
		PA_DisposePicture( *ptpict );

	*ptpict = picture;
}


void PA_SetBlobParameter( PA_PluginParameters params, short index, void* blob, PA_long32 len )
{
	PA_Handle* pth;
	
	pth = ( (PA_Handle**) params->fParameters ) [ index - 1 ];

	*pth = FromUserData( blob, len );
}


void PA_SetBlobHandleParameter( PA_PluginParameters params, short index, PA_Handle hblob )
{
	PA_Handle* pth;
	
	pth = ( (PA_Handle**) params->fParameters ) [ index - 1 ];

	*pth = hblob;
}


void PA_SetDateParameter( PA_PluginParameters params, short index, short day, short month, short year )
{
	PA_Date* ptdate;

	ptdate = ( (PA_Date**) params->fParameters ) [ index - 1 ];

	ptdate->fDay   = day;
	ptdate->fMonth = month;
	ptdate->fYear  = year;
}


void PA_SetTimeParameter( PA_PluginParameters params, short index, PA_long32 value )
{
	*( ( (PA_long32**) params->fParameters ) [ index - 1 ] ) = value;
}


void PA_SetVariableParameter( PA_PluginParameters params, short index, PA_Variable variable, char clearOldValue )
{
	PA_Variable *paramPtr =  ( (PA_Variable**) params->fParameters ) [ index - 1 ];
	
	// set clearOldValue when you create the new 4D variable using PA_CreateVariable.
	// in this case, you need to dispose the previous content of the 4D variable
	if ( clearOldValue )
		PA_ClearVariable( ( (PA_Variable**) params->fParameters) [ index - 1 ] );

	paramPtr->fType = variable.fType;
	paramPtr->fFiller = variable.fFiller;
	
	// We perform a member-to-member copy in order to avoid a stack overflow
	switch( paramPtr->fType )
	{

		case eVK_Real:
			paramPtr->uValue.fReal = variable.uValue.fReal;
			break;

		case eVK_Date:
			paramPtr->uValue.fDate.fDay = variable.uValue.fDate.fDay;
			paramPtr->uValue.fDate.fMonth = variable.uValue.fDate.fMonth;
			paramPtr->uValue.fDate.fYear = variable.uValue.fDate.fYear;
			break;

		case eVK_Longint:
			paramPtr->uValue.fLongint = variable.uValue.fLongint;
			break;

		case eVK_Boolean:
			paramPtr->uValue.fBoolean = variable.uValue.fBoolean;
			break;

		case eVK_Time:
			paramPtr->uValue.fTime = variable.uValue.fTime;
			break;

		case eVK_ArrayOfArray:
		case eVK_ArrayReal:
		case eVK_ArrayInteger:
		case eVK_ArrayLongint:
		case eVK_ArrayDate:
		case eVK_ArrayPicture:
		case eVK_ArrayBlob:
		case eVK_ArrayPointer:
		case eVK_ArrayBoolean:
		case eVK_ArrayUnicode:
		case eVK_ArrayTime:
		case eVK_ArrayObject:
			paramPtr->uValue.fArray.fCurrent = variable.uValue.fArray.fCurrent;
			paramPtr->uValue.fArray.fNbElements = variable.uValue.fArray.fNbElements;
			paramPtr->uValue.fArray.fData = variable.uValue.fArray.fData;
			break;

		case eVK_Blob:
			paramPtr->uValue.fBlob.fSize = variable.uValue.fBlob.fSize;
			paramPtr->uValue.fBlob.fHandle = variable.uValue.fBlob.fHandle;
			break;

		case eVK_Unistring:
			paramPtr->uValue.fString = variable.uValue.fString;
			break;

		case eVK_Picture:
			paramPtr->uValue.fPicture = variable.uValue.fPicture;
			break;

		case eVK_Pointer:
			paramPtr->uValue.fPointer = variable.uValue.fPointer;
			break;
	}
}


// -----------------------------------------
//
// Return a value from a plugin command
//
// -----------------------------------------


void PA_ReturnShort( PA_PluginParameters params, short value )
{
	*((sLONG_PTR*)params->fResult) = (sLONG_PTR)value;
}


void PA_ReturnLong( PA_PluginParameters params, PA_long32 value )
{
	*((sLONG_PTR*)params->fResult) = (sLONG_PTR)value;
}


void PA_ReturnDouble( PA_PluginParameters params, double value )
{
	**(double**) params->fResult = value;
}


void PA_ReturnString( PA_PluginParameters params, PA_Unichar* ustr )
{
	PA_SetUnistring( *(PA_Unistring**) params->fResult, ustr );
}


void PA_ReturnPicture( PA_PluginParameters params, PA_Picture picture )
{
	*(PA_Picture*) params->fResult = picture;
}


void PA_ReturnBlob( PA_PluginParameters params, void* blob, PA_long32 len )
{
	*(PA_Handle*) params->fResult = FromUserData( blob, len );
}


void PA_ReturnBlobHandle( PA_PluginParameters params, PA_Handle hblob )
{
	*(PA_Handle*) params->fResult = hblob;
}


void PA_ReturnDate( PA_PluginParameters params, short day, short month, short year )
{
	PA_Date* ptdate;

	ptdate = *(PA_Date**) params->fResult;

	ptdate->fDay   = day;
	ptdate->fMonth = month;
	ptdate->fYear  = year;
}


void PA_ReturnTime( PA_PluginParameters params, PA_long32 value )
{
	*((sLONG_PTR*)params->fResult) = (sLONG_PTR)value;
}

void PA_ReturnObject(PA_PluginParameters params, PA_ObjectRef object)
{
	*(PA_ObjectRef*)params->fResult = object;

}

void PA_ReturnCollection(PA_PluginParameters params, PA_CollectionRef collection)
{
	*(PA_CollectionRef*)params->fResult = collection;

}
// -----------------------------------------
//
// Get events in a plugin area
//
// -----------------------------------------

PA_AreaEvent PA_GetAreaEvent( PA_PluginParameters params )
{
	return (PA_AreaEvent) ( ( (PA_Event**) params->fParameters )[ 0 ] )->fWhat;
}


PA_Rect PA_GetAreaRect( PA_PluginParameters params )
{
	return *(PA_Rect*) ( ( (PA_Rect**) params->fParameters )[ 1 ] );
}


PA_Unistring* PA_GetAreaName( PA_PluginParameters params )
{
	return ( ( (PA_Unistring**) params->fParameters ) [ 2 ] );
}


void PA_GetPluginProperties( PA_PluginParameters params, PA_PluginProperties* properties )
{
	PA_Event* ev;
	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_InitArea || ev->fWhat == eAE_InitAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		if ( properties && ev->fMessage )
			*properties = *(PA_PluginProperties*) ( ev->fMessage );

		ev->fModifiers = 0x600;	// tell 4D that message is handled
	}
	else if ( ev->fWhat == eAE_DesignUpdate )
	{
		sErrorCode = eER_NoErr;
#if PA_64BITS_ARCHITECTURE
		// In x64 arch you can't call PA_CustomizeDesignMode before PA_GetPluginProperties
		if ( properties && ev->fMessage )
			*properties = *(PA_PluginProperties*)( ev->fMessage );
		ev->fMessage = 0;  // fMessage used out of here. x64 MUST call PA_GetPluginProperties.
#else
		if ( properties && ev->fWhen )
			*properties = *(PA_PluginProperties*)( ev->fWhen );
#endif
	}
	else if ( ev->fWhat == eAE_EditAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		if ( properties && ev->fMessage )
			*properties = *(PA_PluginProperties*) ( ev->fMessage );
	}
	else
		sErrorCode = eER_BadEventCall;
}


// need to be called when area event == eAE_EditAdvancedProperties to give 4D the private data
// you want to keep. Area event eAE_EditAdvancedProperties will be followed by the 
// eAE_DisposeAdvancedProperties to tell you to release the buffer given to 4D.
void PA_SetAdvancedProperties( PA_PluginParameters params, void* data, PA_long32 datasize )
{
	PA_PluginProperties* properties;
	PA_Event* ev;
	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_EditAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		properties = (PA_PluginProperties*) ( ev->fMessage );
		if ( properties )
		{
			properties->fAdvancedProperties = data;
			properties->fAdvancedPropertiesSize = datasize;
		
			ev->fModifiers = 0x0600;
		}
		else
			ev->fModifiers = 0;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called after a eAE_DisposeAdvancedProperties event. Gives you back the
// pointer passed in PA_SetPluginParameters, so you can dispose the memory.
void* PA_GetAdvancedPropertiesToDispose( PA_PluginParameters params )
{
	void* data;
	PA_Event* ev;

	data = 0;
	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_DisposeAdvancedProperties )
	{
		sErrorCode = eER_NoErr;
		data = (void*)(ev->fMessage);	
	}
	else
		sErrorCode = eER_BadEventCall;

	return data;
}


void* PA_GetAdvancedProperties( PA_PluginProperties* properties, PA_long32* datasize )
{
	if ( properties )
	{
		if ( datasize )
			*datasize = properties->fAdvancedPropertiesSize;

		return properties->fAdvancedProperties;
	}
	else
	{
		if ( datasize )
			*datasize = 0;
	
		return 0;
	}
}


// to be called when AreaEvent == eAE_InitArea
void PA_SetAreaReference( PA_PluginParameters params, void* ref )
{	
	*( ( (void***) params->fParameters )[ 3 ] ) = ref;
}


// to get back the reference passed at init
void* PA_GetAreaReference( PA_PluginParameters params )
{
	return *( ( (void***) params->fParameters )[ 3 ] );
}

// to be called when AreaEvent == eAE_IsFocusable
void PA_SetAreaFocusable( PA_PluginParameters params, char focusable )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_IsFocusable )
	{
		sErrorCode = eER_NoErr;
		ev->fMessage = focusable ? 101 : 0;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called when AreaEvent == eAE_Select to know if the window is also activated
char PA_IsActivated( PA_PluginParameters params )
{
	PA_Event* ev;
	char activated = 0;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_Select )
	{
		sErrorCode = eER_NoErr;
		activated = (char)( ( ev->fWhen == 1 ) ? 1 : 0 );
	}
	else
		sErrorCode = eER_BadEventCall;

	return activated;
}


// to be called when AreaEvent == eAE_Deselect to know if the window is also deactivated
char PA_IsDeActivated( PA_PluginParameters params )
{
	PA_Event* ev;
	char deactivated = 0;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_Deselect )
	{
		sErrorCode = eER_NoErr;
		deactivated = (char)( ( ev->fWhen == 1 ) ? 1 : 0 );
	}
	else
		sErrorCode = eER_BadEventCall;

	return deactivated;
}


// can be called when AreaEvent == eAE_Select
void PA_AcceptSelect( PA_PluginParameters params, char accept )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_Select )
	{
		sErrorCode = eER_NoErr;
		ev->fMessage = accept ? 0 : 103;
	}
	else
		sErrorCode = eER_BadEventCall;
}



// can be called when AreaEvent == eAE_Deselect
void PA_AcceptDeselect( PA_PluginParameters params, char accept )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_Deselect )
	{
		sErrorCode = eER_NoErr;
		ev->fMessage = accept ? 0 : 103;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called when AreaEvent == eAE_Drop or eAE_AllowDrop
PA_DragAndDropInfo PA_GetDragAndDropInfo( PA_PluginParameters params )
{
	PA_Event			*ev;
	PA_DragAndDropInfo	dropinfo = {0};

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_Drop || ev->fWhat == eAE_AllowDrop )
	{
		sErrorCode = eER_NoErr;
		dropinfo = *(PA_DragAndDropInfo *)ev->fMessage;
	}
	else
		sErrorCode = eER_BadEventCall;

	return dropinfo;
}


// to be called when AreaEvent is eAE_Drag or eAE_BeginDrag, to have information on dragging position of object
// you received informations on object itself when you receive eAE_AllowDrop
void PA_GetDragPositions( PA_PluginParameters params, PA_Rect* rect, short* x, short* y )
{
	PA_Event* ev;
	PA_Rect r = {0,0,0,0};
	short h = 0;
	short v = 0;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_Drag || ev->fWhat == eAE_BeginDrag )
	{
		sErrorCode = eER_NoErr;
		h = ev->fWhereH;
		v = ev->fWhereV;
		if (ev->fWhat == eAE_Drag)
			r = *(PA_Rect*) ( ev->fMessage );
	}
	else
		sErrorCode = eER_BadEventCall;
	
	if ( x )
		*x = h;

	if ( y )
		*y = v;

	if ( rect )
		*rect = r;
}


void PA_DragAndDrop( short startX, short startY, char useCustomRect, PA_Rect customRect )
{
	EngineBlock eb;
	eb.fParam1 = (sLONG_PTR)&customRect;
	eb.fParam2 = useCustomRect ? 0 : -1;
	eb.fParam3 = startX;
	eb.fParam4 = startY;
	eb.fLongint = 0x00000400;
	eb.fError = eER_NoErr;
	Call4D( EX_DRAG_AND_DROP, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_PasteboardRef PA_GetDragAndDropPasteboard( PA_DragContextRef context )
{
	EngineBlock eb;

	eb.fPtr1 = context;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_DRAG_AND_DROP_PASTEBOARD, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (eb.fError == 0) ? (PA_PasteboardRef) eb.fPtr2 : NULL;
}


char PA_IsPasteboardDataAvailable( PA_PasteboardRef pasteboard, PA_Unichar *kind )
{
	EngineBlock eb;

	eb.fPtr1 = pasteboard;
	PA_CopyUnichars( kind, eb.fUName, sizeof( eb.fUName ) );
	eb.fError = eER_NoErr;
	Call4D( EX_PASTEBOARD_IS_DATA_AVAILABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (eb.fParam2 == 1) ? 1 : 0;
}


PA_Handle PA_GetPasteboardData( PA_PasteboardRef pasteboard, PA_Unichar *kind )
{
	EngineBlock eb;

	eb.fPtr1 = pasteboard;
	PA_CopyUnichars( kind, eb.fUName, sizeof( eb.fUName ) );
	eb.fError = eER_NoErr;
	Call4D( EX_PASTEBOARD_GET_DATA, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fHandle;
}


PA_ulong32 PA_GetPasteboardDataSize( PA_PasteboardRef pasteboard, PA_Unichar *kind )
{
	EngineBlock eb;

	eb.fPtr1 = pasteboard;
	PA_CopyUnichars( kind, eb.fUName, sizeof( eb.fUName ) );
	eb.fError = eER_NoErr;
	Call4D( EX_PASTEBOARD_GET_DATA_SIZE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_ulong32) eb.fParam2;
}


char PA_SetPasteboardData( PA_PasteboardRef pasteboard, PA_Unichar *kind, void *data, PA_ulong32 size )
{
	EngineBlock eb;
	
	eb.fPtr1 = pasteboard;
	PA_CopyUnichars( kind, eb.fUName, sizeof( eb.fUName ) );
	eb.fHandle = (PA_Handle) data;
	eb.fParam2 = (PA_long32) size;
	eb.fError = eER_NoErr;
	Call4D( EX_PASTEBOARD_SET_DATA, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (eb.fError == 0) ? 0 : 1;
}


// to be called when AreaEvent == eAE_Drag to tell 4D you will handle yourself
// the way your area will show that something is dragging over.
// Otherwise, 4D will highlight a 2 pixels frame around your area if you tell that it allows drop.
void PA_CustomizeDragOver( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_Drag )
	{
		sErrorCode = eER_NoErr;
		ev->fModifiers = 1;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// AreaEvent can be eAE_AllowDrop, eAE_Drag, eAE_Drop or eAE_BeginDrag
PA_DragContextRef PA_GetDragAndDropContext( PA_PluginParameters params )
{
	PA_DragContextRef context = NULL;
	PA_Event *ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_AllowDrop || ev->fWhat == eAE_Drag || ev->fWhat == eAE_Drop )
	{
		PA_DragAndDropInfo dropinfo;
		dropinfo = *(PA_DragAndDropInfo *)ev->fMessage;
		context = dropinfo.fDragContext;
		sErrorCode = eER_NoErr;
	}
	else if ( ev->fWhat == eAE_BeginDrag )
	{
		context = (PA_DragContextRef) ev->fMessage;
		sErrorCode = eER_NoErr;
	}
	else
	{
		sErrorCode = eER_BadEventCall;
	}
	return context;
}


// to be called when AreaEvent == eAE_AllowDrop to accept or refuse drop
void PA_AllowDrop( PA_PluginParameters params, char allow )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_AllowDrop )
	{
		sErrorCode = eER_NoErr;
		ev->fModifiers = (short)( allow ? 'OK' : 'NO' );
	}
	else
		sErrorCode = eER_BadEventCall;
}


// Getting informations on a Drag And Drop Info structure
PA_DragKind PA_GetDragAndDropKind( PA_DragAndDropInfo info )
{
	if ( info.fVariableName[0] )
		return eDK_DragVariable;
	else if ( info.fField )
		return eDK_DragField;
	else if ( info.fTable )
		return eDK_DragTable;
	else
		return eDK_InvalidDrag;
}


PA_Variable PA_GetDragAndDropVariable( PA_DragAndDropInfo info, PA_long32* indice )
{
	PA_Variable variable;
	EngineBlock eb;

	sErrorCode = eER_NoErr;
	
	if ( indice )
		*indice = 0;

	eb.fError = eER_NoErr;
	eb.fPtr1 = &info;
	eb.fHandle = (PA_Handle) &variable;
	Call4D( EX_GET_DRAG_AND_DROP_VARIABLE, &eb);
	if ( indice )
		*indice = (PA_long32)eb.fParam1;
	sErrorCode = (PA_ErrorCode) eb.fError;

	return variable;
}


void PA_GetDragAndDropTableField( PA_DragAndDropInfo info, short* table, short* field )
{
	if ( table )
		*table = info.fTable;

	if ( field )
		*field = info.fField;
}


// to be called on Windows when AreaEvent == eAE_Update
PA_HDC PA_GetHDC(PA_PluginParameters params)
{
	PA_HDC retHDC = NULL;
	PA_Event* eventPtr = (((PA_Event**)params->fParameters)[0]);
	if (eventPtr)
		retHDC = (PA_HDC)eventPtr->fMessage;
	return retHDC;
}


// to be called when AreaEvent == eAE_MouseDown, eAE_MouseUp or eAE_WebClick or eAE_MouseWheel
void PA_GetClick( PA_PluginParameters params, short* x, short* y )
{
	short h, v;
	PA_Event* ev;

	h = v = 0;
	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if (    ev->fWhat == eAE_MouseDown
		 || ev->fWhat == eAE_MouseUp
		 || ev->fWhat == eAE_WebClick
		 || ev->fWhat == eAE_MouseWheel
		 || ev->fWhat == eAE_Cursor
	   )
	{
		sErrorCode = eER_NoErr;
		h = ev->fWhereH;
		v = ev->fWhereV;
	}
	else
		sErrorCode = eER_BadEventCall;

	if ( x )
		*x = h;

	if ( y )
		*y = v;
}

// to be called when AreaEvent == eAE_MouseWheel
short PA_GetMouseWheelIncrement( PA_PluginParameters params )
{
	short i;
	PA_Event* ev;

	i = 0;
	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_MouseWheel )
	{
		sErrorCode = eER_NoErr;
		i = (short) ev->fMessage;
	}
	else
		sErrorCode = eER_BadEventCall;

	return i;
}

// to be called when AreaEvent == eAE_KeyDown
// warning, the returned values are valid only if the function returns a non zero value
// because for multibytes characters, you will receive multiple events that need to be
// aggregate to build a valid unicode character.
char PA_GetKey( PA_PluginParameters params, PA_Unichar* unichar, PA_KeyCode* keycode, char* altKey, char* ctrlKey, char* shiftKey )
{
	char c;
	PA_Event* ev;

	c = 0;
	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_KeyDown || ev->fWhat == eAE_AutoKey )
	{
		EngineBlock eb;
		eb.fHandle = (PA_Handle) ev;
		Call4D( EX_GET_PLUGIN_KEY_EVENT, &eb );
		sErrorCode = (PA_ErrorCode)eb.fError;

		c = eb.fManyToOne;
		if ( c != 0 )
		{
			if ( NULL != unichar )
				*unichar = (PA_Unichar) eb.fParam1;
			
			if ( NULL != keycode )
				*keycode = (PA_KeyCode)eb.fLongint;

			if ( NULL != altKey )
				*altKey = (char) eb.fParam2;

			if ( NULL != ctrlKey )
				*ctrlKey = (char) eb.fParam3;

			if ( NULL != shiftKey )
				*shiftKey = (char) eb.fParam4;
		}
	}
	else
		sErrorCode = eER_BadEventCall;

	return c;
}

// to be called if AreaEvent == eAE_DesignUpdate 
// if you want to draw yourself your plugin in design mode
// In x64 arch you can't call PA_CustomizeDesignMode before PA_GetPluginProperties, it scatches the fMessage pointer on the PluginProperties.
void PA_CustomizeDesignMode( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_DesignUpdate )
	{
		sErrorCode = eER_NoErr;
		ev->fMessage = 102;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called if AreaEvent == eAE_GetMenuIcon 
// to give the id of the 'cicn' resource that will appears
// beside the area name in plugin area popup of the the tools palette.
void PA_SetMenuIcon( PA_PluginParameters params, short id )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_GetMenuIcon )
	{
		sErrorCode = eER_NoErr;
		ev->fMessage = 604;
		ev->fWhen = (PA_long32) id;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called when AreaEvent == eAE_ArePropertiesEditable
// to tell 4D you want to use the "Advanced Properties" Button
void PA_SetAdvancedPropertiesEditable( PA_PluginParameters params, char editable )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	if ( ev->fWhat == eAE_AreAdvancedPropertiesEditable )
	{
		sErrorCode = eER_NoErr;
		if ( editable )
			ev->fMessage = 603;
		else
			ev->fMessage = 0;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// if you don't take an area event, call this routine to tell 4D to handle it.
void PA_DontTakeEvent( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];
	ev->fMessage = 112;
}


void PA_CallPluginAreaMethod( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_MouseDown || ev->fWhat == eAE_AutoKey || ev->fWhat == eAE_KeyDown || ev->fWhat == eAE_Idle )
	{
		ev->fMessage = 301;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

void PA_GotoNextField( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_MouseDown || ev->fWhat == eAE_AutoKey || ev->fWhat == eAE_KeyDown || ev->fWhat == eAE_Idle )
	{
		ev->fMessage = 110;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

void PA_GotoPreviousField( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_MouseDown || ev->fWhat == eAE_AutoKey || ev->fWhat == eAE_KeyDown || ev->fWhat == eAE_Idle )
	{
		ev->fMessage = 111;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_WebPublish
void PA_PublishWebPicture( PA_PluginParameters params, char pictureMap )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_WebPublish )
	{
		ev->fModifiers = (short)( ( pictureMap == 0 ) ? 2 : 3 );
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// to be called at eAE_WebPublish
void PA_SendHTML( PA_PluginParameters params, void* webData, char* HTMLbuffer, PA_long32 len )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_WebPublish )
	{
		ev->fMessage = (sLONG_PTR)HTMLbuffer;
		ev->fWhen = len;
		*(void**)( & ev->fWhereV ) = webData;
		ev->fModifiers = 1;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_WebPublishPicture
void PA_SendWebPicture( PA_PluginParameters params, void* webData, void* picture, PA_long32 len, PA_WebPictureKind kind )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_WebPublishPicture )
	{
		ev->fMessage = (sLONG_PTR)picture;
		ev->fWhen = len;
		*(void**)( & ev->fWhereV ) = webData;
		ev->fModifiers = (short) kind;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_WebDisposeData
void* PA_GetWebDataToDispose( PA_PluginParameters params )
{
	PA_Event* ev;
	void* data = 0;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_WebDisposeData )
	{
		data = * (void**) ( & ev->fWhereV );
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;

	return data;
}


// to be called at eAE_PageChange.
// The form is changing page to or from your area page.
// If PA_GetCurrentPage() == *pageTo, the page is opening. You should take this opportunity to install your native controls.
// If PA_GetCurrentPage() == *pageFrom, the page is closing. You should take this opportunity to remove your native controls.
void PA_GetPageChange( PA_PluginParameters params, short *pageFrom, short *pageTo )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_PageChange )
	{
		*pageFrom = ev->fWhereH;
		*pageTo = (short) ev->fWhen;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}


// to be called at eAE_EndExecutionCycle.
// something just happened in the form so you may take this opportunity
// to ask 4D to redraw your area because user code may have changed the data you rely on.
void PA_RequestRedraw( PA_PluginParameters params )
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_EndExecutionCycle )
	{
		ev->fModifiers = 1;
		sErrorCode = eER_NoErr;
	}
	else
		sErrorCode = eER_BadEventCall;
}

// will invalidate whole or a part of the plugin area
// if inNow is 1, the area will be receveive an update event during this call
// v14 or higher
void	PA_RedrawArea ( PA_PluginParameters params, char inNow,PA_Rect* inRect)
{
	PA_Event* ev;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat != eAE_Update && ev->fWhat != eAE_Scroll)
	{
		EngineBlock	eb;
		eb.fHandle = (PA_Handle)PA_GetAreaReference(params);
		eb.fParam1 = inNow ;
		eb.fParam2 = (sLONG_PTR) inRect;
		eb.fError = eER_NoErr;
		Call4D( EX_REDRAW_AREA, &eb );
		sErrorCode = (PA_ErrorCode)eb.fError;

	}
	else
	{
		sErrorCode = eER_BadEventCall;
	}
}

// to call during eAE_UpdateEvent only 
// return the bounds of the graphic port that host the plug in area
// needed to swap CGContext axis
// v14 or higher

PA_Rect	PA_GetAreaPortBounds ( PA_PluginParameters params)
{
	PA_Rect result;
	if(PA_GetAreaEvent(params)==eAE_DesignUpdate)
	{
		PA_Rect designBounds = PA_GetAreaRect( params );
		result.fLeft = 0;
		result.fTop = 0;
		result.fRight = designBounds.fRight - designBounds.fLeft;
		result.fBottom = designBounds.fBottom - designBounds.fTop;
	}
	else 
	{

		EngineBlock	eb;
		eb.fHandle = (PA_Handle)PA_GetAreaReference(params);
		eb.fParam1 = (sLONG_PTR) &result ;
		eb.fError = eER_NoErr;
		Call4D( EX_GET_AREA_PORT_BOUNDS, &eb );
		sErrorCode = (PA_ErrorCode)eb.fError;

	}
	return result;
}

// -----------------------------------------
//
// Get fields from database
//
// -----------------------------------------

PA_Unistring PA_GetStringField( short table, short field )
{
	EngineBlock	eb;
	
  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fUniString1.fString = 0;
	eb.fUniString1.fLength = 0;
	eb.fUniString1.fReserved1 = 0;
	eb.fUniString1.fReserved2 = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fUniString1;
}


// if blob size is unknow, call the function a first time with buffer equal to zero,
// the value returned will be the size of the needed buffer.
// Allocate the buffer and call again the function with the correct buffer
PA_long32 PA_GetBlobField( short table, short field, void* blob )
{
	EngineBlock	eb;
	PA_long32 len = 0;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		len = ToUserData( eb.fTextHandle, blob );

	return len;
}


PA_Handle PA_GetBlobHandleField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		return eb.fTextHandle;
	else
		return 0;
}


PA_Picture PA_GetPictureField ( short table, short field )
{
	EngineBlock	eb;
	PA_Picture picture = 0;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fTextHandle = 0;	// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		picture = eb.fPicture;

	return picture;
}


double PA_GetRealField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fReal = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fReal;
}


PA_long32 PA_GetLongintField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable   = table;
	eb.fField   = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fLongint;
}


PA_long32 PA_GetTimeField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint  = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fLongint;
}


short PA_GetIntegerField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fShort = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fShort;
}


void PA_GetDateField( short table, short field, short* day, short* month, short* year )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;

	eb.fDate.fDay   = 0;
	eb.fDate.fMonth = 0;
	eb.fDate.fYear  = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( day )
		*day = eb.fDate.fDay;

	if ( month )
		*month = eb.fDate.fMonth;

	if ( year )
		*year = eb.fDate.fYear;
}

char PA_GetBooleanField( short table, short field )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fShort = 0;

	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fShort;
}


// This function does not work
PA_ObjectRef PA_GetObjectField ( short table, short field )
{
	EngineBlock	eb;
	PA_ObjectRef object = 0;
	
	eb.fTable  = table;
	eb.fField  = field;
	eb.fHandle = 0;	// subtable
	eb.fTextHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fPtr1 = 0;
	
	Call4D( EX_GET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	
	if ( sErrorCode == eER_NoErr )
		object = (PA_ObjectRef) eb.fPtr1;
	
	return object;
}


// -----------------------------------------
//
// Set fields in database
//
// -----------------------------------------

// the string is not owned by this function, you should call PA_DisposeUnistring if needed
void PA_SetStringField( short table, short field, PA_Unistring* ustr )
{
	EngineBlock	eb;
	
  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fUniString1 = *ustr;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetBlobField( short table, short field, void* blob, PA_long32 len )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fTextHandle = FromUserData( blob, len );
	eb.fTextSize = -1;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetBlobHandleField( short table, short field, PA_Handle hblob )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fTextSize = -1;
	eb.fTextHandle = hblob;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetPictureField ( short table, short field, PA_Picture picture )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fPicture = picture;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}



void PA_SetRealField( short table, short field, double value )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fReal  = value;
	eb.fError  = eER_NoErr;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetLongintField( short table, short field, PA_long32 value )
{
	EngineBlock	eb;

  	eb.fTable   = table;
	eb.fField   = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint = value;
	eb.fError  = eER_NoErr;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetIntegerField( short table, short field, short value )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fShort = value;
	eb.fError  = eER_NoErr;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetTimeField( short table, short field, PA_long32 value )
{
	EngineBlock	eb;

  	eb.fTable   = table;
	eb.fField   = field;
	eb.fHandle = 0;	// subtable
	eb.fLongint = value;
	eb.fError  = eER_NoErr;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetDateField( short table, short field, short day, short month, short year )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;

	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetBooleanField( short table, short field, char value )
{
	EngineBlock	eb;

  	eb.fTable = table;
	eb.fField = field;
	eb.fShort = value;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;

	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

// This function does not work
void PA_SetObjectField ( short table, short field, PA_ObjectRef object )
{
	EngineBlock	eb;
	
	eb.fTable = table;
	eb.fField = field;
	eb.fHandle = 0;	// subtable
	eb.fError  = eER_NoErr;
	eb.fPtr1 = object;
	
	Call4D( EX_SET_FIELD, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4D Application Variables
//
// -----------------------------------------


PA_Variable PA_CreateVariable( PA_VariableKind kind )
{
	PA_Variable variable;
	char* pt;
	PA_Unichar uchar = 0;

	variable.fType = (char) kind;
	variable.fFiller = 0;
	
	switch ( kind )
	{
		case eVK_Real :
			variable.uValue.fReal = 0.0;
			break;
		
		case eVK_Date:
			variable.uValue.fDate.fDay   = 0;
			variable.uValue.fDate.fMonth = 0;
			variable.uValue.fDate.fYear  = 0;
			break;

		case eVK_Boolean :
		case eVK_Longint :
			variable.uValue.fLongint = 0;
			break;

		case eVK_Picture :
			variable.uValue.fPicture = PA_CreatePicture( 0, 0 );
			break;

		case eVK_Time :
			variable.uValue.fTime = 0;
			break;
			
		case eVK_ArrayReal :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( double ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(double*) pt = 0.0;
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;
		
		case eVK_ArrayBoolean :
		case eVK_ArrayInteger :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( short ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(short*) pt = 0;
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayLongint :
		case eVK_ArrayTime :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_long32 ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(PA_long32*) pt = 0;
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayDate :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Date ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
			{
				( (PA_Date*) pt )->fDay = 0;
				( (PA_Date*) pt )->fMonth = 0;
				( (PA_Date*) pt )->fYear = 0;
			}
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayPicture :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Handle ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(PA_Picture*) pt  = PA_CreatePicture( 0, 0 );
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_ArrayBlob :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Blob ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
			{
				PA_Blob* blobby=(PA_Blob*)pt;
				blobby->fHandle=0;
				blobby->fSize=0;
			}
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_Blob :
			variable.uValue.fBlob.fHandle = PA_NewHandle( 0 );
			variable.uValue.fBlob.fSize = 0;
			break;

		case eVK_Unistring:
			variable.uValue.fString = PA_CreateUnistring( &uchar );
			break;
		
		case eVK_ArrayUnicode :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_Unistring ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(PA_Unistring*) pt = PA_CreateUnistring( &uchar );
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;

		case eVK_Object :
			variable.uValue.fObject = 0;
			break;
			
		case eVK_ArrayObject :
			variable.uValue.fArray.fCurrent = 0;
			variable.uValue.fArray.fData = PA_NewHandle( sizeof( PA_ObjectRef ) );
			variable.uValue.fArray.fNbElements = 0;
			pt = PA_LockHandle( variable.uValue.fArray.fData );
			if ( pt )
				*(PA_ObjectRef*) pt  = PA_CreateObject();
			PA_UnlockHandle( variable.uValue.fArray.fData );
			break;
			
		case eVK_Undefined :
		case eVK_ArrayOfArray :
		case eVK_Pointer :
		case eVK_ArrayPointer:
		case eVK_Integer :
			// not supported
			break;
	}
	return variable;
}


PA_Variable PA_GetVariable( PA_Unichar* variableName )
{
	PA_Variable vb;
	EngineBlock eb;
	char interprocess = 0;

	eb.fHandle = (PA_Handle) &vb;
	eb.fError = eER_NoErr;

	PA_CopyUnichars( variableName, eb.fUName, sizeof( eb.fUName ) );
	interprocess = StripInterprocessVariableName( eb.fUName );

	if ( interprocess )
		Call4D( EX_GET_INTERPROCESS_VARIABLE, &eb );
	else
		Call4D( EX_GET_VARIABLE, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return vb;
}


PA_VariableKind PA_GetVariableKind( PA_Variable variable )
{
	return UnifyVarKind(variable.fType);
}

PA_VariableKind PA_GetPointerValueKind( PA_Pointer inVarPtr )
{
	PA_VariableKind pk = eVK_Undefined;
	PA_GetPointerValueProperties(inVarPtr,&pk,0,0);
	return pk;
}

PA_Unistring PA_GetStringVariable( PA_Variable variable )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) &variable;
	eb.fUniString1.fLength = 0;
	eb.fUniString1.fString = 0;
	eb.fUniString1.fReserved1 = 0;
	eb.fUniString1.fReserved2 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_VARIABLE_TO_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}

// if blob size is unknow, call the function a first time with buffer equal to zero,
// the value returned will be the size of the needed buffer.
// Allocate the buffer and call again the function with the correct buffer
PA_long32 PA_GetBlobVariable( PA_Variable variable, void* blob )
{
	PA_long32 len = 0;

	if ( variable.fType == eVK_Blob )
		len = ToUserData( variable.uValue.fBlob.fHandle, blob );

	return len;
}


// the handle still belongs to the variable, you cannot dispose it.
PA_Handle PA_GetBlobHandleVariable( PA_Variable variable )
{
	if ( variable.fType == eVK_Blob )
		return variable.uValue.fBlob.fHandle;
	else
		return 0;
}


// the picture still belongs to the variable, you cannot dispose it
PA_Picture PA_GetPictureVariable( PA_Variable variable )
{
	if ( variable.fType == eVK_Picture )
		return variable.uValue.fPicture;
	else
		return 0;
}


double PA_GetRealVariable( PA_Variable variable )
{
	double value = 0.0;

	if ( variable.fType == eVK_Real )
		value = variable.uValue.fReal;

	return value;
}


PA_long32 PA_GetLongintVariable( PA_Variable variable )
{
	PA_long32 value = 0;

	if ( variable.fType == eVK_Longint )
		value = variable.uValue.fLongint;

	return value;
}


PA_long32 PA_GetTimeVariable( PA_Variable variable )
{
	PA_long32 value = 0;

	if ( variable.fType == eVK_Time )
		value = variable.uValue.fTime;

	return value;
}


void PA_GetDateVariable( PA_Variable variable, short* day, short* month, short* year )
{
	if ( variable.fType == eVK_Date )
	{
		if ( day )
			*day = variable.uValue.fDate.fDay;

		if ( month )
			*month = variable.uValue.fDate.fMonth;

		if ( year )
			*year = variable.uValue.fDate.fYear;
	}
}


char PA_GetBooleanVariable( PA_Variable variable )
{
	char value = 0;

	if ( variable.fType == eVK_Boolean )
		value = variable.uValue.fBoolean;

	return value;
}


PA_ObjectRef PA_GetObjectVariable ( PA_Variable variable )
{
	PA_ObjectRef object = NULL;
	
	if ( variable.fType == eVK_Object )
	{
		object = variable.uValue.fObject;
	}
	
	return object;
}


// -----------------------------------------
//
// Set 4D Application variables
//
// -----------------------------------------


void PA_SetVariable( PA_Unichar* variableName, PA_Variable variable, char clearOldValue )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) &variable;
	eb.fError  = eER_NoErr;

	eb.fClearOldVariable = clearOldValue;

	PA_CopyUnichars( variableName, eb.fUName, sizeof( eb.fUName ) );

	if ( StripInterprocessVariableName( eb.fUName ) )
		Call4D( EX_SET_INTERPROCESS_VARIABLE, &eb );
	else
		Call4D( EX_SET_VARIABLE, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetStringVariable( PA_Variable* variable, PA_Unistring* ustr )
{
	variable->fType = eVK_Unistring;
	variable->fFiller = 0;
	variable->uValue.fString = *ustr;
}


void PA_SetBlobVariable( PA_Variable* variable, void* blob, PA_long32 len )
{
	variable->fType = eVK_Blob;
	variable->fFiller = 0;
	variable->uValue.fBlob.fHandle = FromUserData( blob, len );
	variable->uValue.fBlob.fSize = len;
}


void PA_SetBlobHandleVariable( PA_Variable* variable, PA_Handle hblob )
{
	variable->fType = eVK_Blob;
	variable->fFiller = 0;
	variable->uValue.fBlob.fHandle = hblob;
	variable->uValue.fBlob.fSize = (PA_long32)PA_GetHandleSize( hblob );
}


void PA_SetPictureVariable( PA_Variable* variable, PA_Picture picture )
{
	variable->fType = eVK_Picture;
	variable->fFiller = 0;
	variable->uValue.fPicture = picture;
}
 

void PA_SetRealVariable( PA_Variable* variable, double value )
{
	variable->fType = eVK_Real;
	variable->fFiller = 0;
	variable->uValue.fReal = value;
}


void PA_SetLongintVariable( PA_Variable* variable, PA_long32 value )
{
	variable->fType = eVK_Longint;
	variable->fFiller = 0;
	variable->uValue.fLongint = value;
}


void PA_SetTimeVariable( PA_Variable* variable, PA_long32 value )
{
	variable->fType = eVK_Time;
	variable->fFiller = 0;
	variable->uValue.fTime = value;
}


void PA_SetDateVariable( PA_Variable* variable, short day, short month, short year )
{
	variable->fType = eVK_Date;
	variable->fFiller = 0;
	variable->uValue.fDate.fDay   = day;
	variable->uValue.fDate.fMonth = month;
	variable->uValue.fDate.fYear  = year;
}

void PA_SetBooleanVariable( PA_Variable* variable, char value )
{
	variable->fType = eVK_Boolean;
	variable->fFiller = 0;
	variable->uValue.fBoolean = value;
}


void PA_SetObjectVariable( PA_Variable* variable, PA_ObjectRef object )
{
	variable->fType = eVK_Object;
	variable->fFiller = 0;
	variable->uValue.fObject = object;
}

void PA_SetCollectionVariable(PA_Variable* variable, PA_CollectionRef collection)
{
	variable->fType = eVK_Collection;
	variable->fFiller = 0;
	variable->uValue.fCollection = collection;
}


void PA_SetOperationVariable( PA_Variable* variable, char op )
{
	variable->fType = 0;
	variable->fFiller = 1;
	if ( op == '*' )
		variable->uValue.fOperation = 3;
	else if ( op == '<' )
		variable->uValue.fOperation = 4;
	else if ( op == '>' )
		variable->uValue.fOperation = 5;

}

void PA_CopyVariable(PA_Variable *source, PA_Variable *destination)
{
	EngineBlock eb;
	eb.fPtr1 = source;
	eb.fPtr2 = destination;
	Call4D(EX_COPY_VARIABLE, &eb);
	sErrorCode = eb.fError;
}


void PA_SetTableFieldVariable( PA_Variable* variable, short table, short field )
{
	variable->fType = 3;
	variable->fFiller = 1;
	variable->uValue.fTableFieldDefinition.fFieldNumber = field;
	variable->uValue.fTableFieldDefinition.fTableNumber = table;
	variable->uValue.fTableFieldDefinition.fUnused1 = 0;
}

void PA_SetVariableOrFieldReference( PA_Variable* outVariable , PA_Pointer inPointer)
{
//	const static char pointerScopeTo4DScope[4]={26,9,35,13};
	switch (PA_GetPointerKind(inPointer))
	{
	case ePK_PointerToVariable:
//		outVariable->fType = pointerScopeTo4DScope[inPointer->fScope];
		outVariable->fType = 9; // Always use 9, doesn't work with others
		outVariable->fFiller = 1;
		memcpy(outVariable->uValue.fVariableDefinition.fName,inPointer->uValue.fVariable.fName,sizeof(inPointer->uValue.fVariable.fName));
		outVariable->uValue.fVariableDefinition.fIndice = inPointer->uValue.fVariable.fIndice;
		outVariable->uValue.fVariableDefinition.fType = inPointer->fScope;
		outVariable->uValue.fVariableDefinition.fTag = inPointer->uValue.fVariable.fTag;
		break;

	case ePK_PointerToTable:
	case ePK_PointerToField:
		PA_SetTableFieldVariable(outVariable,inPointer->uValue.fTableField.fTable,inPointer->uValue.fTableField.fField);
		break;

	default:
		break;
	}
}

void PA_ClearVariable( PA_Variable* variable )
{
	EngineBlock eb;

	eb.fPtr1 = variable;
	eb.fError = eER_NoErr;
	Call4D( EX_CLEAR_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// -----------------------------------------
//
// 4D Application arrays
//
// -----------------------------------------


PA_long32 PA_GetArrayNbElements( PA_Variable ar )
{
	if ( PA_IsArrayVariable(&ar) )
		return ar.uValue.fArray.fNbElements;
	else
		return -1;
}


void PA_ResizeArray( PA_Variable *ar, PA_long32 nb )
{
	PA_long32				size = 0;
	PA_long32				nb1 = nb + 1;	// we have to count element number zero
	PA_long32				i;
	PA_Unistring		*ptUnistrings;		
	PA_Picture			*ptPictureHandle;
	char				*pt;
	char				*ptmax;
	PA_long32				oldSize = 0;
	PA_long32				oldCount;
	PA_Variable			var;
	PA_VariableKind		kind;
	PA_ArrayVariable	*ptArray;

	if (    nb >= 0 
		 && ( PA_IsArrayVariable(ar) )
		 && ar->uValue.fArray.fNbElements != nb
		 && ar->uValue.fArray.fData
	   )
	{
		oldCount = ar->uValue.fArray.fNbElements + 1;
		switch ( ar->fType )
		{
			case eVK_ArrayOfArray :
				// dispose in memory arrays that will be removed
				// if array become smaller
				if ( nb < ar->uValue.fArray.fNbElements )
				{
					ptArray = (PA_ArrayVariable*) PA_LockHandle( ar->uValue.fArray.fData );

					for ( i = nb + 1; i <= ar->uValue.fArray.fNbElements; i++ )
						PA_ClearVariable( (PA_Variable*) &( ptArray[ i ] ) );

					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
				size = nb1 * (PA_long32) sizeof( PA_ArrayVariable );
				oldSize = oldCount * (PA_long32) sizeof( PA_ArrayVariable );
				break;

			case eVK_ArrayReal :
				size = nb1 * (PA_long32) sizeof( double );
				oldSize = oldCount * (PA_long32) sizeof( double );
				break;

			case eVK_ArrayInteger :
				size = nb1 * (PA_long32) sizeof( short );
				oldSize = oldCount * (PA_long32) sizeof( short );
				break;

			case eVK_ArrayLongint :
			case eVK_ArrayTime :
				size = nb1 * (PA_long32) sizeof( PA_long32 );
				oldSize = oldCount * (PA_long32) sizeof( PA_long32 );
				break;
			
			case eVK_ArrayDate :
				size = nb1 * (PA_long32) sizeof( PA_Date );
				oldSize = oldCount * (PA_long32) sizeof( PA_Date );
				break;

			case eVK_ArrayPicture :
				// dispose in memory picture handles that will be removed
				// if array become smaller
				if ( nb < ar->uValue.fArray.fNbElements )
				{
					ptPictureHandle = (PA_Picture*) PA_LockHandle( ar->uValue.fArray.fData );

					for ( i = nb + 1; i <= ar->uValue.fArray.fNbElements; i++ )
						PA_DisposePicture( ptPictureHandle[ i ] );
						
					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
				size = nb1 * (PA_long32) sizeof( PA_Handle );
				oldSize = oldCount * (PA_long32) sizeof( PA_Handle );
				break;

			case eVK_ArrayBlob :
				// dispose in memory picture handles that will be removed
				// if array become smaller
				if ( nb < ar->uValue.fArray.fNbElements )
				{
					PA_Blob** ptBlobHandle = (PA_Blob**) PA_LockHandle( ar->uValue.fArray.fData );

					for ( i = nb + 1; i <= ar->uValue.fArray.fNbElements; i++ )
					{
						PA_DisposeHandle(ptBlobHandle[i]->fHandle);
					}
					
					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
				size = nb1 * (PA_long32) sizeof(PA_Blob);
				oldSize = oldCount * (PA_long32) sizeof(PA_Blob);
				break;

			case eVK_ArrayUnicode :
				// dispose in memory strings that will be removed
				// if array become smaller
				if ( nb < ar->uValue.fArray.fNbElements )
				{
					ptUnistrings = (PA_Unistring*) PA_LockHandle( ar->uValue.fArray.fData );

					for ( i = nb + 1; i <= ar->uValue.fArray.fNbElements; i++ )
						PA_DisposeUnistring( & ( ptUnistrings[ i ] ) );
						
					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
				size = nb1 * (PA_long32) sizeof( PA_Unistring );
				oldSize = oldCount * (PA_long32) sizeof( PA_Unistring );
				break;

			case eVK_ArrayPointer :
				size = nb1 * (PA_long32) sizeof( PointerBlock );
				oldSize = oldCount * (PA_long32) sizeof( PointerBlock );
				break;

			case eVK_ArrayBoolean :
				size = ( nb1 + 7 ) / 8;
				oldSize = ( oldCount + 7 ) / 8;
				break;
				
			case eVK_ArrayObject :
				// dispose in memory object handles that will be removed
				// if array become smaller
				if ( nb < ar->uValue.fArray.fNbElements )
				{
					PA_ObjectRef *ptObject = (PA_ObjectRef*) PA_LockHandle( ar->uValue.fArray.fData );
					
					for ( i = nb + 1; i <= ar->uValue.fArray.fNbElements; i++ )
						PA_DisposeObject( ptObject[ i ] );
					
					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
				size = nb1 * (PA_long32) sizeof( PA_ObjectRef );
				oldSize = oldCount * (PA_long32) sizeof( PA_ObjectRef );
				break;
		}

		if ( PA_SetHandleSize( ar->uValue.fArray.fData, size ) )
		{
			if ( oldSize < size )
			{
				if ( ar->fType == eVK_ArrayOfArray )
				{
					// read infos on arrays variables
					var = PA_GetArrayInArray( *ar, 0 );
					kind = PA_GetVariableKind( var );

					ptArray = (PA_ArrayVariable*) PA_LockHandle( ar->uValue.fArray.fData );

					// Initialize new elements of array of arrays.

					for ( i = ar->uValue.fArray.fNbElements + 1; i <= nb; i++ )
					{
						var = PA_CreateVariable( kind );
						ptArray[ i ] = *(PA_ArrayVariable*) &var;
					}
					
					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
				else
				{
					// fill with zero the new allocated bytes
					pt = PA_LockHandle( ar->uValue.fArray.fData );

					ptmax = pt + size;
					pt += oldSize;
					
					while ( pt < ptmax )
						*pt++ = 0;

					PA_UnlockHandle( ar->uValue.fArray.fData );
				}
			}
		}
		ar->uValue.fArray.fNbElements = nb;
	}
}


PA_long32 PA_GetArrayCurrent( PA_Variable ar )
{
	if ( PA_IsArrayVariable(&ar) )
		return ar.uValue.fArray.fCurrent;
	else
		return -1;
}


void PA_SetArrayCurrent( PA_Variable* ar, PA_long32 current )
{
	if ( PA_IsArrayVariable(ar) )
		ar->uValue.fArray.fCurrent = (short) current;
}

char  PA_IsArrayVariable( PA_Variable* ar )
{
	char isArray=0;
	if (ar != NULL)
	{
		switch(ar->fType)
		{
		case eVK_ArrayOfArray:
		case eVK_ArrayReal:
		case eVK_ArrayInteger:
		case eVK_ArrayLongint:
		case eVK_ArrayDate:
		case eVK_ArrayPicture:
		case eVK_ArrayPointer:
		case eVK_ArrayBoolean:
		case eVK_ArrayBlob:
		case eVK_ArrayTime:
		case eVK_ArrayUnicode:
		case eVK_ArrayObject:
			isArray=-1;
			break;
		}
	}
	return isArray;
}


short PA_GetIntegerInArray( PA_Variable ar, PA_long32 i )
{
	short value = 0;
	
	if (	ar.fType == eVK_ArrayInteger 
		 && ar.uValue.fArray.fData 
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		value = ( * (short**) (ar.uValue.fArray.fData) )[i];

	return value;
}


PA_long32 PA_GetLongintInArray( PA_Variable ar, PA_long32 i )
{
	PA_long32 value = 0;
	
	switch(ar.fType)
	{
	case eVK_ArrayLongint:
	case eVK_ArrayTime:
		if (ar.uValue.fArray.fData && i >= 0 && i <= ar.uValue.fArray.fNbElements)
			value = ( * (PA_long32**) (ar.uValue.fArray.fData) )[i];
		break;
	}
	
	return value;
}

PA_long32 PA_GetTimeInArray( PA_Variable ar, PA_long32 i )
{
	PA_long32 retMilliseconds = 0;
	
	if (	ar.fType == eVK_ArrayTime
		 && ar.uValue.fArray.fData  
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		retMilliseconds = ( * (PA_long32**) (ar.uValue.fArray.fData) )[i];
	
	return retMilliseconds;
}


double PA_GetRealInArray( PA_Variable ar, PA_long32 i )
{
	double value = 0.0;

	if (	ar.fType == eVK_ArrayReal
		 && ar.uValue.fArray.fData  
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		value = ( * (double**) (ar.uValue.fArray.fData) )[i];

	return value;
}


void PA_GetDateInArray( PA_Variable ar, PA_long32 i, short* day, short* month, short* year )
{
	PA_Date date;

	date.fDay   = 0;
	date.fMonth = 0;
	date.fYear  = 0;

	if (	ar.fType == eVK_ArrayDate
		 && ar.uValue.fArray.fData  
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		date = ( * (PA_Date**) (ar.uValue.fArray.fData) )[i];

	if ( day )
		*day   = date.fDay;

	if ( month )
		*month = date.fMonth;

	if ( year )
		*year  = date.fYear;
}

// the string cannot be modified !
// to modify the string in the array, use PA_SetStringInArray
PA_Unistring PA_GetStringInArray( PA_Variable ar, PA_long32 i )
{
	PA_Unistring str;

	str.fString = 0;
	str.fLength = 0;
	str.fReserved1 = 0;
	str.fReserved2 = 0;

	if (	ar.fType == eVK_ArrayUnicode
		 && ar.uValue.fArray.fData  
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		str = ( * (PA_Unistring**) (ar.uValue.fArray.fData) )[i];

	return str;
}

PA_Picture PA_GetPictureInArray( PA_Variable ar, PA_long32 i )
{
	PA_Picture picture = 0;
	
	if (	ar.fType == eVK_ArrayPicture 
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
	{
		picture = ( * (PA_Picture**) ar.uValue.fArray.fData ) [ i ];
	}
	return picture;
}

PA_Blob PA_GetBlobInArray( PA_Variable ar, PA_long32 i )
{
	PA_Blob retBlob = {0,0};
	
	if (	ar.fType == eVK_ArrayBlob 
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
	{
		retBlob = ( * (PA_Blob**) ar.uValue.fArray.fData ) [ i ];
	}
	return retBlob;
}


static unsigned char tabTrue [ 8 ] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
static unsigned char tabFalse[ 8 ] = { 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F };

static char GetBitInTable( unsigned char *bitTable, PA_long32 index)
{
	return (char) ( ( *( bitTable + ( index >> 3 ) ) & tabTrue[ index & 7 ] ) ? 1 : 0 );
}


static void SetBitInTable( unsigned char *bitTable, PA_long32 index, char value)
{
	PA_long32 bytePos;

	bytePos = index >> 3;
	index &= 7;
	if ( value )
		*( bitTable + bytePos ) |= tabTrue[ index ];
	else
		*( bitTable + bytePos ) &= tabFalse[ index ];
}


char PA_GetBooleanInArray( PA_Variable ar, PA_long32 i )
{
	char value = 0;

	if (	ar.fType == eVK_ArrayBoolean 
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		value = GetBitInTable( *(unsigned char**) (ar.uValue.fArray.fData), i );

	return value;
}


PA_Variable PA_GetArrayInArray( PA_Variable ar, PA_long32 i )
{
	PA_ArrayVariable arrayVar;
	arrayVar.fType = eVK_Undefined;

	if (    ar.fType == eVK_ArrayOfArray
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
		)
		arrayVar = ( * (PA_ArrayVariable**) ar.uValue.fArray.fData ) [ i ];

	return *(PA_Variable*) &arrayVar;
}


PointerBlock PA_GetPointerInArray( PA_Variable ar, PA_long32 i )
{
	PointerBlock value = {0};

	if (    ar.fType == eVK_ArrayPointer
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
		)
		value = ( *(PointerBlock**) ar.uValue.fArray.fData ) [ i ];

	return value;
}


PA_ObjectRef PA_GetObjectInArray ( PA_Variable ar, PA_long32 i )
{
	PA_ObjectRef object = NULL;
	
	if (	ar.fType == eVK_ArrayObject
		&& ar.uValue.fArray.fData
		&& i >= 0
		&& i <= ar.uValue.fArray.fNbElements
		)
	{
		object = ( * (PA_ObjectRef**) ar.uValue.fArray.fData ) [ i ];
	}
	
	return object;
}


void PA_SetIntegerInArray( PA_Variable ar, PA_long32 i, short value )
{
	if (	ar.fType == eVK_ArrayInteger
		 && ar.uValue.fArray.fData 
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		( * (short**) (ar.uValue.fArray.fData) )[i] = value;
}


void PA_SetLongintInArray( PA_Variable ar, PA_long32 i, PA_long32 value ) 
{
	switch(ar.fType)
	{
	case eVK_ArrayLongint:
	case eVK_ArrayTime:
		if (ar.uValue.fArray.fData && i >= 0 && i <= ar.uValue.fArray.fNbElements)
			( * (PA_long32**) (ar.uValue.fArray.fData) )[i] = value;
	}
}

void PA_SetTimeInArray( PA_Variable ar, PA_long32 i, PA_long32 inMilliseconds ) 
{
	switch(ar.fType)
	{
	case eVK_ArrayTime:
		if (ar.uValue.fArray.fData && i >= 0 && i <= ar.uValue.fArray.fNbElements)
			( * (PA_long32**) (ar.uValue.fArray.fData) )[i] = inMilliseconds;
	}
}

void PA_SetRealInArray( PA_Variable ar, PA_long32 i, double value )
{
	if (	ar.fType == eVK_ArrayReal  
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		( * (double**) (ar.uValue.fArray.fData) )[i] = value;
}


void PA_SetDateInArray( PA_Variable ar, PA_long32 i, short day, short month, short year )
{
	PA_Date date;

	date.fDay   = day;
	date.fMonth = month;
	date.fYear  = year;

	if (	ar.fType == eVK_ArrayDate 
		 && ar.uValue.fArray.fData 
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		( * (PA_Date**) (ar.uValue.fArray.fData) )[i] = date;
}


void PA_SetStringInArray( PA_Variable ar, PA_long32 i, PA_Unistring* ustr )
{
	PA_Unistring* pt;

	if (	ar.fType == eVK_ArrayUnicode
		 && ar.uValue.fArray.fData 
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
		 && ustr
	   )
	{
		pt = &( * (PA_Unistring**) (ar.uValue.fArray.fData) )[i];
		PA_DisposeUnistring( pt );
		*pt = *ustr;
	}
}


void PA_SetPictureInArray( PA_Variable ar, PA_long32 i, PA_Picture picture )
{
	PA_Picture* pth;
	
	if (	ar.fType == eVK_ArrayPicture 
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
		 && picture
	   )
	{
		// lock the array handle and get a pointer on the element
		pth = (PA_Picture*)(PA_Picture) PA_LockHandle( ar.uValue.fArray.fData );
		pth += i;

		// remove existing picture
		if ( *pth )
			PA_DisposePicture( *pth );

		// set new picture in array
		*pth = picture;

		PA_UnlockHandle( ar.uValue.fArray.fData );
	}
}


void PA_SetBlobInArray( PA_Variable ar, PA_long32 inIndex, PA_Blob blob )
{
	if (	ar.fType == eVK_ArrayBlob 
		 && ar.uValue.fArray.fData
		 && inIndex >= 0
		 && inIndex <= ar.uValue.fArray.fNbElements
	   )
	{
		// lock the array handle and get a pointer on the element
		PA_Blob* blobby = (PA_Blob*) PA_LockHandle(ar.uValue.fArray.fData);
		blobby+=inIndex;
		
		// remove handle
		if ( blobby->fHandle )
			PA_DisposeHandle(blobby->fHandle);

		// Set the blob
		blobby->fHandle=blob.fHandle;
		blobby->fSize=blob.fSize;

		PA_UnlockHandle(ar.uValue.fArray.fData);
	}
}


void PA_SetBooleanInArray( PA_Variable ar, PA_long32 i, char value )
{
	if (	ar.fType == eVK_ArrayBoolean
		 && ar.uValue.fArray.fData 
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
	   )
		SetBitInTable( *(unsigned char**) ar.uValue.fArray.fData, i, value );
}


void PA_SetArrayInArray( PA_Variable ar, PA_long32 i, PA_Variable value )
{
	PA_Variable firstElement;
	PA_Variable lastValue;

	if (    ar.fType == eVK_ArrayOfArray
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements )
	{

		firstElement = PA_GetArrayInArray( ar, 0 );

		// checks arrays kind
		if ( PA_GetVariableKind( firstElement ) == PA_GetVariableKind( value ) )
		{
			lastValue = PA_GetArrayInArray( ar, i );

			// checks if it's the same handle
			if ( lastValue.uValue.fArray.fData != value.uValue.fArray.fData )
				PA_ClearVariable( &lastValue );

			( *(PA_ArrayVariable**) ar.uValue.fArray.fData ) [ i ] = *(PA_ArrayVariable*) &value;
		}
	}
}

void PA_SetPointerInArray( PA_Variable ar, PA_long32 i, PointerBlock value )
{
	if (    ar.fType == eVK_ArrayPointer
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
		)
		( *(PointerBlock**) ar.uValue.fArray.fData ) [ i ] = value;
}


void PA_SetObjectInArray ( PA_Variable ar, PA_long32 i, PA_ObjectRef object )
{
	PA_Handle* pth;
	
	if (	ar.fType == eVK_ArrayObject
		 && ar.uValue.fArray.fData
		 && i >= 0
		 && i <= ar.uValue.fArray.fNbElements
		)
	{
		// lock the array handle and get a pointer on the element
		pth = (PA_Handle*)(PA_ObjectRef*) PA_LockHandle( ar.uValue.fArray.fData );
		pth += i;
		
		// remove existing object
		if ( *pth && *pth != object)
			PA_DisposeObject( *pth );
		
		// set new object in array
		*pth = (PA_Handle)object;
		
		PA_UnlockHandle( ar.uValue.fArray.fData );
	}
}


// -----------------------------------------
//
// formating strings, values, date or time
//
// -----------------------------------------

void PA_FormatString( PA_Unichar* string, PA_Unichar* format, PA_Unichar* result )
{
	EngineBlock eb;

	eb.fParam1 = eFK_AlphaField;
	PA_CopyUnichars( string, eb.fUString, sizeof(eb.fUString) );
	PA_CopyUnichars( format, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;
	Call4D( EX_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, result, sizeof(eb.fUString) );
}


void PA_FormatReal( double value, PA_Unichar* format, PA_Unichar* result )
{
	EngineBlock eb;

	eb.fParam1 = eFK_RealField;
	eb.fReal   = value;
	PA_CopyUnichars( format, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, result, sizeof(eb.fUString) );
}


void PA_FormatLongint( PA_long32 value, PA_Unichar* format, PA_Unichar* result )
{
	EngineBlock eb;

	eb.fParam1  = eFK_LongintField;
	eb.fLongint = value;
	PA_CopyUnichars( format, eb.fUString, sizeof(eb.fUString) );

	eb.fError = eER_NoErr;
	Call4D(EX_STRING, &eb);
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, result, sizeof(eb.fUString) );
}


void PA_FormatDate( short day, short month, short year, short formatNumber, PA_Unichar* result )
{
	EngineBlock eb;

	eb.fParam1      = eFK_DateField;
	eb.fParam2      = formatNumber;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;

	eb.fError = eER_NoErr;
	Call4D(EX_STRING, &eb);
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, result, sizeof(eb.fUString) );
}


void PA_FormatTime( PA_long32 time, short formatNumber, PA_Unichar* result )
{
	EngineBlock eb;

	eb.fParam1  = eFK_TimeField;
	eb.fParam2  = formatNumber;
	eb.fLongint = time;

	eb.fError = eER_NoErr;
	Call4D(EX_STRING, &eb);
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, result, sizeof(eb.fUString) );
}


double PA_EvalReal( PA_Unichar* string )
{
	EngineBlock eb;

	eb.fParam1 = eVK_Real;
	eb.fReal   = 0.0;
	PA_CopyUnichars( string, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_EVAL_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fReal;
}


PA_long32 PA_EvalLongint( PA_Unichar* string )
{
	EngineBlock eb;

	eb.fParam1  = eVK_Longint;
	eb.fLongint = 0;
	PA_CopyUnichars( string, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_EVAL_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fLongint;
}


void PA_EvalDate( PA_Unichar* string, short* day, short* month, short* year )
{
	EngineBlock eb;

	eb.fParam1      = eVK_Date;
	eb.fDate.fDay   = 0;
	eb.fDate.fMonth = 0;
	eb.fDate.fYear  = 0;
	PA_CopyUnichars( string, eb.fUString, sizeof(eb.fUString) );

	eb.fError = eER_NoErr;
	Call4D(EX_EVAL_NUMBER, &eb);
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( day )
		*day = eb.fDate.fDay;

	if ( month )
		*month = eb.fDate.fMonth;

	if ( year )
		*year = eb.fDate.fYear;
}


PA_long32 PA_EvalTime( PA_Unichar* string )
{
	EngineBlock eb;

	eb.fParam1  = eVK_Time;
	eb.fLongint = 0;
	PA_CopyUnichars( string, eb.fUString, sizeof(eb.fUString) );

	eb.fError = eER_NoErr;
	Call4D(EX_EVAL_NUMBER, &eb);
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fLongint;
}


char PA_CompareMacStrings( char* text1, PA_long32 len1, char* text2, PA_long32 len2, char diacritic )
{
	EngineBlock eb;
	sErrorCode = eER_NoErr;

	eb.fParam1 = (sLONG_PTR)text1;
	eb.fParam2 = (sLONG_PTR)text2;

	eb.fParam3 = len1;
	eb.fParam4 = len2;
	eb.fError = eER_NoErr;
	if ( diacritic )
		eb.fParam3 = -eb.fParam3;

	if ( sErrorCode == eER_NoErr )
		Call4D( EX_COMPARE_MACSTRINGS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam1;
}


void PA_ConvertStrings( char* string1, PA_StringKind kind1, PA_CharSet charset1, 
					    char* string2, PA_StringKind kind2, PA_CharSet charset2 )
{
	EngineBlock eb;

	eb.fParam1 = 4;	// no conversion;

	if (    ( charset1 == eCS_Macintosh )
		 && ( charset2 == eCS_Ansi ) )
		eb.fParam1 = 2;

	if (    ( charset1 == eCS_Ansi )
		 && ( charset2 == eCS_Macintosh ) )
		eb.fParam1 = 3;

	eb.fParam2 = -1;
	if ( kind1 == eSK_CString )
	{
		if ( kind2 == eSK_CString )
			eb.fParam2 = 4;
		else if ( kind2 == eSK_PString )
			eb.fParam2 = 3;
	}
	else if ( kind1 == eSK_PString )
	{
		if ( kind2 == eSK_CString )
			eb.fParam2 = 2;
		else if ( kind2 == eSK_PString )
			eb.fParam2 = 1;
	}

	eb.fParam3 = (sLONG_PTR)string1;
	eb.fParam4 = (sLONG_PTR)string2;
	eb.fError = eER_NoErr;

	Call4D( EX_CONVERT_STRING, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

// Returns the byte size used in the toString.
PA_long32 PA_ConvertCharsetToCharset ( char* fromString, PA_long32 fromSize, PA_4DCharSet fromCharset,
								 char* toString, PA_long32 toSize, PA_4DCharSet toCharset)
{
	EngineBlock parms;
	parms.fPtr1=(void*)fromString;
	parms.fPtr2=(void*)toString;
	parms.fParam1=fromSize;
	parms.fParam2=fromCharset;
	parms.fParam3=toSize;
	parms.fParam4=toCharset;
	parms.fError=0;

	Call4D( EX_CONVERT_CHARSET_TO_CHARSET, &parms );
	sErrorCode = (PA_ErrorCode) parms.fError;

	return (PA_long32)parms.fParam3;
}


// -----------------------------------------
//
// 4D Application import/export filters
//
// -----------------------------------------

char* PA_GetExportFilter()
{
	EngineBlock eb;

	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_EXPORT_FILTER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char*) eb.fHandle;
}


char* PA_GetImportFilter()
{
	EngineBlock eb;

	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_IMPORT_FILTER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char*) eb.fHandle;
}


// -----------------------------------------
//
// 4D Application Packed Records
//
// -----------------------------------------


PA_long32 PA_GetPackedRecord( short table, void* buffer )
{
	PA_long32 len = 0;
	EngineBlock eb;

	eb.fTable = table;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_PACKED_RECORD, &eb );

	if ( eb.fHandle )
	{
		len = ToUserData( eb.fHandle, buffer );
		PA_DisposeHandle( eb.fHandle );
	}

	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return len;
}


void PA_SetPackedRecord( short table, void* buffer, PA_long32 len )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fHandle = FromUserData( buffer, len );
	eb.fError = eER_NoErr;
	Call4D( EX_SET_PACKED_RECORD, &eb );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4D Application selections
//
// -----------------------------------------


void PA_CopyNamedSelection( short table, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fManyToOne = 1;
	eb.fTable = table;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_COPY_CUT_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_CutNamedSelection( short table, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fManyToOne = 0;
	eb.fTable = table;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_COPY_CUT_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UseNamedSelection( PA_Unichar* name )
{
	EngineBlock eb;

	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;

	Call4D( EX_USE_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_ClearNamedSelection( PA_Unichar* name )
{
	EngineBlock eb;

	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;

	Call4D( EX_CLEAR_NAMED_SELECTION, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4D Application sets
//
// -----------------------------------------


void PA_CreateEmptySet( short table, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable = table;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_CREATE_EMPTY_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_CreateSet( short table, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable = table;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_CREATE_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UseSet( PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable = 1;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_USE_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

void PA_DeleteSet( PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable = 1;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_DELETE_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

void PA_AddToSet( short table, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fHandle = 0;
	eb.fTable = table;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	Call4D( EX_ADD_TO_SET, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
// 4D Application methods
//
// -----------------------------------------


void PA_CreateMethod(PA_Unichar* methodName, PA_MethodFlags ref, PA_Unistring* methodCode, PA_Unichar* folderName )
{
	EngineBlock eb;
	
	PA_CopyUnichars( methodName, eb.fUName, sizeof(eb.fUName) );
	PA_CopyUnichars( folderName, eb.fUString, sizeof(eb.fUString) );
	eb.fUniString1 = *methodCode;
	
	eb.fRecord = '11.0';
	eb.fParam1 = ref.f4DAction;
	eb.fParam2 = ref.fInvisible;
	eb.fParam3 = ref.fSoap;
	eb.fParam4 = ref.fWsdl;
	eb.fTable  = (short) ref.fSQL;
	eb.fField  = (short) ref.fShared;
	
	eb.fError = eER_NoErr;
	
	Call4D( EX_CREATE_METHOD, &eb );
	
	sErrorCode = (PA_ErrorCode) eb.fError;
}

void PA_ModifyMethod(PA_Unichar* methodName, PA_Unistring* methodCode )
{
	EngineBlock eb;
	
	PA_CopyUnichars( methodName, eb.fUName, sizeof(eb.fUName) );
	eb.fUniString1 = *methodCode;
	
	eb.fRecord = '11.0';
	
	eb.fError = eER_NoErr;
	
	Call4D( EX_MODIFY_METHOD, &eb );
	
	sErrorCode = (PA_ErrorCode) eb.fError;
}

void PA_ExecuteMethod( PA_Unistring* ustr )
{
	EngineBlock eb;

	eb.fError = eER_NoErr;
	eb.fUniString1 = *ustr;

	Call4D( EX_EXECUTE_METHOD, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_Variable PA_ExecuteFunction( PA_Unistring* ustr )
{
	EngineBlock eb;
	PA_Variable vb;

	vb.fType = eVK_Undefined;

	eb.fError = eER_NoErr;
	eb.fUniString1 = *ustr;
	eb.fParam1 = (sLONG_PTR)&vb;

	Call4D( EX_EXECUTE_FUNCTION, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;

	return vb;
}


PA_long32 PA_Tokenize( PA_Unistring* ustr, void* tokens )
{
	EngineBlock eb;
	PA_long32 tokenslen = 0;

	eb.fUniString1 = *ustr;
	eb.fError = eER_NoErr;
	eb.fParam1 = 0;

	Call4D( EX_TOKENIZE, &eb );

	if ( eb.fParam1 && sErrorCode == eER_NoErr )
		tokenslen = ToUserData( (PA_Handle) eb.fParam1, tokens );

	if ( eb.fParam1 )
		PA_DisposeHandle( (PA_Handle) eb.fParam1 );

	sErrorCode = (PA_ErrorCode) eb.fError;

	return tokenslen;
}


PA_Unistring PA_Detokenize( void* tokens, PA_long32 len )
{
	EngineBlock eb;
	eb.fUniString1.fLength = 0;
	eb.fUniString2.fString = 0;

	eb.fParam1 = (sLONG_PTR)FromUserData( tokens, len );
	eb.fError  = eER_NoErr;
	eb.fHandle = 0;

	Call4D( EX_DETOKENIZE, &eb );

	if ( eb.fParam1 )
		PA_DisposeHandle( (PA_Handle) eb.fParam1 );

	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fUniString1;
}


void PA_ByteSwapTokens( void* tokens, PA_long32 len )
{
	EngineBlock eb;
	eb.fHandle = FromUserData( tokens, len );
	eb.fError  = eER_NoErr;
	Call4D( EX_BYTE_SWAP_TOKENS, &eb );

	if ( eb.fHandle )
	{
		ToUserData( eb.fHandle, tokens );
		PA_DisposeHandle( eb.fHandle );
	}

	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_ExecuteTokens ( void* tokens, PA_long32 len )
{
	EngineBlock eb;

	eb.fHandle = FromUserData( tokens, len );
	eb.fError = eER_NoErr;

	Call4D( EX_EXEC_TOKEN_PROC, &eb );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_Variable PA_ExecuteTokensAsFunction( void* tokens, PA_long32 len )
{
	EngineBlock eb;
	PA_Variable vb;

	vb.fType = eVK_Undefined;

	eb.fError = eER_NoErr;
	eb.fHandle = FromUserData( tokens, len );
	eb.fParam1 = (sLONG_PTR)&vb;

	Call4D( EX_EXEC_TOKEN_FUNC, &eb );

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	sErrorCode = (PA_ErrorCode) eb.fError;

	return vb;
}


PA_long32 PA_GetMethodID( PA_Unichar* methodName )
{
	EngineBlock eb;

	eb.fError  = eER_NoErr;
	PA_CopyUnichars( methodName, eb.fUName, sizeof(eb.fUName) );
	
	Call4D( EX_GET_METHOD_ID, &eb );

	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam1;
}


PA_Variable PA_ExecuteMethodByID( PA_long32 id, PA_Variable* parameters, short nbParameters )
{
	EngineBlock eb;
	PA_Variable* pt;
	PA_Variable returned;
	PA_long32 i;

	returned.fType = eVK_Undefined;
	returned.fFiller = 0;

	eb.fHandle = PA_NewHandle( ( 1 + nbParameters ) * (PA_long32) sizeof( PA_Variable ) );
	if ( eb.fHandle )
	{
		pt = (PA_Variable*) PA_LockHandle( eb.fHandle );
		
		// set the PA_Variable used for function return
		*pt++ = returned;

		// fill the handle with the user parameters
		for ( i = 0; i < nbParameters; i++ )
			*pt++ = *parameters++;

		PA_UnlockHandle( eb.fHandle );

		eb.fError  = eER_NoErr;
		eb.fParam1 = id;
		eb.fParam2 = nbParameters;

		Call4D( EX_CALL_BY_PROCID, &eb );

		pt = (PA_Variable*) PA_LockHandle( eb.fHandle );
		returned = *pt;
		PA_UnlockHandle( eb.fHandle );
		PA_DisposeHandle( eb.fHandle );

		sErrorCode = (PA_ErrorCode) eb.fError;
	}

	return returned;
}


PA_long32 PA_GetCommandID( PA_Unichar* commandName )
{
	EngineBlock eb;

	PA_CopyUnichars( commandName, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;
	Call4D( EX_GET_COMMAND_ID, &eb );

	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}

void PA_GetCommandName( PA_long32 commandIndex, PA_Unichar* commandName )
{
	EngineBlock eb;

	eb.fShort = (short) commandIndex;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_COMMAND_NAME, &eb );

	PA_CopyUnichars( eb.fUString, commandName, sizeof(eb.fUString) );

	sErrorCode = (PA_ErrorCode)eb.fError;
}

PA_Variable PA_ExecuteCommandByID( PA_long32 id, PA_Variable* parameters, short nbParameters )
{
	EngineBlock eb;
	PA_Variable returned = {0};
	PA_Handle h;
	PA_Variable** ptvar;
	PA_long32 i;

	h = PA_NewHandle( nbParameters * sizeof( PA_Variable* ) );
	if ( h )
	{
		returned.fType = eVK_Undefined;
		returned.fFiller = 0;
		
		eb.fPtr1 = PA_LockHandle( h );
		eb.fPtr2 = &returned;

		for ( i = 0, ptvar = (PA_Variable**) eb.fPtr1; i < nbParameters; i++, ptvar++, parameters++ )
			*ptvar = parameters;

		eb.fError  = eER_NoErr;
		eb.fParam1 = id;
		eb.fParam2 = nbParameters;

		Call4D( EX_EXECUTE_COMMAND_BY_ID, &eb );

		PA_UnlockHandle( h );
		PA_DisposeHandle( h );

		sErrorCode = (PA_ErrorCode) eb.fError;
	}

	return returned;
}

// ---------------------------------------------------------------
//
// Localization
//
// ---------------------------------------------------------------

// search a text into resource file
// pluginResource = 1 -> looking in plugin resources
// pluginResource = 0 -> looking in current database resources
PA_Unistring PA_LocaliseStringByID( PA_long32 resID, PA_long32 resIndex, char pluginResource )
{
	EngineBlock eb;

	if ( pluginResource != 0 )
		eb.fParam1 = 'PLUG';
	else
		eb.fParam1 = 0;

	eb.fParam2 = 'STR#';
	eb.fParam3 = resID;
	eb.fParam4 = resIndex;
	eb.fError = eER_NoErr;
	Call4D( EX_LOCALIZE_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}

// search a text into XLIFF file
// pluginResource = 1 -> looking in plugin resources
// pluginResource = 0 -> looking in current database resources
PA_Unistring PA_LocaliseString( PA_Unichar* name, char pluginResource )
{
	EngineBlock eb;

	if ( pluginResource != 0 )
		eb.fParam1 = 'PLUG';
	else
		eb.fParam1 = 0;

	eb.fParam2 = 0;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;
	Call4D( EX_LOCALIZE_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}

// ---------------------------------------------------------------
//
// Log and Backup operations
//
// ---------------------------------------------------------------

void PA_LockDatabase()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_LOCK_DATABASE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_UnlockDatabase()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_UNLOCK_DATABASE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}



// ---------------------------------------------------------------
//
// Multiprocess Printing Management
//
// ---------------------------------------------------------------

// This functions allows the plugin
// to test if the printer is available.
// If the printer is available, it will
// return 1 and open a print session on it.
// If the printer is already printing, 
// it will return 0.
char PA_TryToOpenPrinterSession()
{
	EngineBlock eb={0};
	eb.fError = eER_NoErr;
	Call4D( EX_TRY_TO_OPEN_PRINTER_SESSION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) ( eb.fParam1 ? 0 : 1 );
}


// This function allows the plugin to 
// open a print session in 4D, and to lock
// the printer for other processes
// If 4D is already printing in an other process,
// this command will wait the end of printing before
// returning.
// if the current process is already printing,
// function will return 0.
// If nobody is printing, function will return 1.
// Each successfull call to PA_OpenPrinterSession needs
// to be balanced by a call to PA_ClosePrinterSession
// PA_OpenPrinterSession calls internally PrOpen
// so you should not call yourself PrOpen before printing
char PA_OpenPrinterSession()
{
	EngineBlock eb={0};

	Call4D( EX_OPEN_PRINTER_SESSION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (char) ( eb.fParam2 ? 1 : 0 );
}


// To be called when printing is done.
// Calls internally PrClose, so you don't
// need to call PrClose yourself after printing.
void PA_ClosePrinterSession()
{
	EngineBlock eb={0};

	Call4D( EX_CLOSE_PRINTER_SESSION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// returns the PMPrintSettings used by 4D.
// you should not dispose it
void* PA_GetCarbonPrintSettings()
{
	EngineBlock eb;
	eb.fParam1 = 0;
	eb.fError  = eER_NoErr;

#if VERSIONMAC
	{
		char opened = PA_TryToOpenPrinterSession();	
		Call4D( EX_GET_PRINT_INFO, &eb );
		if ( opened )
			PA_ClosePrinterSession();
	}
#endif

	sErrorCode = (PA_ErrorCode) eb.fError;
	return (void*) eb.fParam1;
}

// returns the PMPageFormat used by 4D.
// you should not dispose it
void* PA_GetCarbonPageFormat()
{
	EngineBlock eb;
	eb.fParam2 = 0;
	eb.fError  = eER_NoErr;

#if VERSIONMAC
	{
		char opened = PA_TryToOpenPrinterSession();
		Call4D( EX_GET_PRINT_INFO, &eb );
		if ( opened )
			PA_ClosePrinterSession();
	}
#endif

	sErrorCode = (PA_ErrorCode) eb.fError;
	return (void*) eb.fParam2;
}

// on Windows only, returns a HGLOBAL that holds
// the PRINDLG structure used by 4D.
// This gives access to the HDEVMODE
// Warning, this HGLOBAL belongs to 4D,
// do not dispose it.
void* PA_GetWindowsPRINTDLG()
{
	EngineBlock eb;
	eb.fParam1 = 0;
	eb.fError  = eER_NoErr;

#if VERSIONWIN
	{
		char opened = PA_TryToOpenPrinterSession();	
		Call4D( EX_GET_PRINT_INFO, &eb );
		if ( opened )
			PA_ClosePrinterSession();
	}
#endif

	sErrorCode = (PA_ErrorCode) eb.fError;
	return (void*) eb.fParam1;
}

// on Windows only, returns the
// current printing DC.
// Can be called only when 4D
// is printing
void* PA_GetWindowsPrintingDC()
{
	EngineBlock eb;

	eb.fParam2 = 0;
	eb.fError  = eER_NoErr;

#if VERSIONWIN
	Call4D( EX_GET_PRINT_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
#endif

	return (void*) eb.fParam2;
}


// ---------------------------------------------------------------
//
// Processes
//
// ---------------------------------------------------------------


// return number of running process
PA_long32 PA_CountActiveProcess()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam2 = 0;
	Call4D( EX_NB_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam2;
}


// return number of process, including aborted process
PA_long32 PA_CountTotalProcess()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = 0;
	Call4D( EX_NB_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}


void PA_GetProcessInfo( PA_long32 process, PA_Unichar* name, PA_long32* state, PA_long32* time )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fString[0] = 0;
	eb.fParam2 = 0;
	eb.fParam3 = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_PROCESS_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
	{
		PA_CopyUnichars( eb.fUString, name, sizeof(eb.fUString) );

		if ( state )
			*state = (PA_long32)eb.fParam2;

		if ( time )
			*time = (PA_long32)eb.fParam3;
	}
}


void PA_FreezeProcess( PA_long32 process )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fError  = eER_NoErr;
	Call4D( EX_FREEZE_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_UnfreezeProcess( PA_long32 process )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fError  = eER_NoErr;
	Call4D( EX_UNFREEZE_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_IsProcessDying()
{
	EngineBlock eb;
	eb.fParam1 = 0;	// or process number
	Call4D( EX_IS_PROCESS_DYING, &eb );
	sErrorCode = eER_NoErr;

	return eb.fManyToOne;
}


void PA_KillProcess()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_KILL_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_PutProcessToSleep( PA_long32 process, PA_long32 time )
{
	EngineBlock eb;

	eb.fParam1 = process;
	eb.fParam2 = time;
	eb.fError  = eER_NoErr;
	Call4D( EX_PUT_PROCESS_TO_SLEEP, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_GetCurrentProcessNumber()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_CURRENT_PROCESS_NUMBER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}


PA_long32 PA_GetWindowProcess( PA_WindowRef windowRef )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) windowRef;
	eb.fParam2 = -1;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_WINDOW_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}


PA_long32 PA_GetFrontWindowProcess( char withPalette )
{
	EngineBlock eb;
	eb.fHandle = 0;
	eb.fParam2 = withPalette ? 1 : 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_WINDOW_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}


void PA_SetWindowProcess( PA_WindowRef windowRef, PA_long32 process )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) windowRef;
	eb.fParam1 = process;
	eb.fError = eER_NoErr;
	Call4D( EX_SET_WINDOW_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Yield()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_YIELD, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_YieldAbsolute()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_YIELD_ABSOLUTE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


char PA_WaitNextEvent( PA_Event* ev )
{
	EngineBlock eb;
	eb.fParam1 = 0xFFFF;	// everyEvent
	eb.fParam2 = 3;
	eb.fHandle = (PA_Handle) ev;
	eb.fParam3 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_WAIT_NEXT_EVENT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (char) eb.fParam4;
}


void PA_UpdateProcessVariable( PA_long32 process )
{
	EngineBlock eb;
	eb.fParam1 = process;
	eb.fError = eER_NoErr;
	Call4D( EX_UPDATE_PROCESS_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_BringProcessToFront( PA_long32 process )
{
	EngineBlock eb;
	eb.fParam1 = process;
	eb.fError = eER_NoErr;
	Call4D( EX_BRING_PROCESS_TO_FRONT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_long32 PA_NewProcess( void* procPtr, PA_long32 stackSize, PA_Unichar* name )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) procPtr;
	eb.fParam3 = stackSize;
	PA_CopyUnichars( name, eb.fUString, sizeof(eb.fUString) );
	eb.fParam1 = sBinaryFormat;
	eb.fError = eER_NoErr;
	Call4D( EX_NEW_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam1;
}


void PA_PostMacEvent( PA_long32 process, PA_Event* ev )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) ev;
	eb.fParam1 = process;
	eb.fError = eER_NoErr;
	Call4D( EX_POST_EVENT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_RunInMainProcess( PA_RunInMainProcessProcPtr procPtr, void* parameters )
{
	EngineBlock eb;
	eb.fPtr1 = (void*) procPtr;
	eb.fPtr2 = parameters;
	eb.fError = eER_NoErr;
	Call4D( EX_RUN_IN_MAIN_PROCESS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

}


// ---------------------------------------------------------------
//
// Sending/Receiving documents from 4D Server
//
// ---------------------------------------------------------------


PA_long32 PA_ReceiveDocumentFromServer( PA_Unichar* docName, void* buffer )
{
	PA_long32 len = 0;
	EngineBlock eb;

	PA_CopyUnichars( docName, eb.fUName, sizeof(eb.fUName) );
	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_READ_DOCUMENT, &eb );

	if ( eb.fHandle )
	{
		len = ToUserData( eb.fHandle, buffer );
		PA_DisposeHandle( eb.fHandle );
	}

	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return len;
}


void PA_SendDocumentToServer( PA_Unichar* docName, void* buffer, PA_long32 len, PA_ulong32 type, PA_ulong32 creator )
{
	EngineBlock eb;

	PA_CopyUnichars( docName, eb.fUName, sizeof(eb.fUName) );
	eb.fHandle = (PA_Handle) buffer;
	eb.fParam2 = len;
	eb.fParam3 = (PA_long32) type;
	eb.fParam4 = (PA_long32) creator;

	Call4D( EX_SAVE_DOCUMENT, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


char PA_DocumentExistOnServer( PA_Unichar* docName, PA_ulong32 type, PA_ulong32 creator )
{
	EngineBlock eb;
	PA_CopyUnichars( docName, eb.fUName, sizeof(eb.fUName) );
	eb.fParam1 = 0;
	eb.fParam3 = (PA_long32) type;
	eb.fParam4 = (PA_long32) creator;

	Call4D( EX_EXIST_DOCUMENT_ON_SERVER, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char) eb.fParam1;
}


// ---------------------------------------------------------------
//
// Sending/Receiving data between 4D Client and 4D Server
//
// ---------------------------------------------------------------


void PA_SendDataToServer( PA_long32 pluginRef, PA_long32 dataRef, PA_long32 dataKind, void* buffer, PA_long32 len )
{
	EngineBlock eb;

	eb.fParam1 = pluginRef;
	eb.fParam2 = len;
	eb.fParam3 = dataKind;
	eb.fParam4 = dataRef;
	eb.fHandle = (PA_Handle) buffer;

	Call4D( EX_WRITE_DATA, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_ReceiveDataFromServer( PA_long32 pluginRef, PA_long32 dataRef, PA_long32 dataKind, void* buffer )
{
	EngineBlock eb;
	PA_long32 len;

	eb.fParam1 = pluginRef;
	eb.fParam3 = dataKind;
	eb.fParam4 = dataRef;
	eb.fHandle = 0;

	Call4D( EX_READ_DATA, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	len = ToUserData( eb.fHandle, buffer );
	PA_DisposeHandle( eb.fHandle );

	return len;
}


// -----------------------------------------
//
// 4D Application internal resources access
//
// -----------------------------------------


// if you want 4D to give you an unique id, set id to -1.
// The unique id created by 4D will be returned
PA_long32 PA_CreateResource( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name, char* data, PA_long32 len )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fHandle = FromUserData( data, len );
	eb.fError = eER_NoErr;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	if ( id == -1 )
	{
		eb.fParam2 = 0;
		eb.fManyToOne = 1;
	}
	else
	{
		eb.fParam2 = id;
		eb.fManyToOne = 0;
	}

	Call4D( EX_CREATE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam2;
}


PA_long32 PA_CreateResourceFromHandle( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fHandle = resourceHandle;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );

	if ( id == -1 )
	{
		eb.fParam2 = 0;
		eb.fManyToOne = 1;
	}
	else
	{
		eb.fParam2 = id;
		eb.fManyToOne = 0;
	}

	Call4D( EX_CREATE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam2;
}


void PA_RemoveResourceByID( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = 0;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fError = eER_NoErr;

	Call4D( EX_REMOVE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_RemoveResourceByName( short resfile, PA_ulong32 kind, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = 0;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = 0;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );


	Call4D( EX_REMOVE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_GetResource( short resfile, PA_ulong32 kind, PA_long32 id, char* data )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 0;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return ToUserData( eb.fHandle, data );
}


// warning, you are NOT the owner of the resource handle, unless you detach it
PA_Handle PA_GetResourceHandle( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 0;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fHandle;
}


void PA_WriteResourceHandle( short resfile, PA_Handle handle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = handle;
	eb.fError = eER_NoErr;

	Call4D( EX_WRITE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_SetResource( short resfile, PA_ulong32 kind, PA_long32 id, char* data, PA_long32 len )
{
	EngineBlock eb;
	char* pt;
	PA_long32 i;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 0;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr && eb.fHandle && data )
	{
		if ( PA_SetHandleSize( eb.fHandle, len ) )
		{
			// lock 4D resource handle
			pt = PA_LockHandle( eb.fHandle );
			
			// copy data on it
			for ( i = 0; i < len; i++ )
				*pt++ = *data++;

			// unlock it
			PA_UnlockHandle( eb.fHandle );

			// ask 4D to write the new resource content
			Call4D( EX_WRITE_RESOURCE, &eb );
			sErrorCode = (PA_ErrorCode) eb.fError;
		}
	}
}


// on 4D Server/4D Client only
// the resource is locked for others 4D Client (they can't delete or modify it)
char PA_LockResource( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fManyToOne = 1;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (char)( ( eb.fManyToOne == 0 ) ? 1 : 0 );
}


// on 4D Server/4D Client only
// the resource is now unlocked for others 4D Client
void PA_UnlockResource( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_UNLOCK_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}

// the "Lock" attribute of the resource is now set. The handle will
// stay locked, and be automatically locked in future use.
void PA_LockResourceHandle( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = resourceHandle;
	eb.fManyToOne = 1;
	eb.fError = eER_NoErr;

	Call4D( EX_LOCK_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// the 'Lock' attribute of the resource is now unset. The handle will
// further be moveable.
void PA_UnlockResourceHandle( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = resourceHandle;
	eb.fManyToOne = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_LOCK_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// the resource and the memory it uses are purged from memory
void PA_ReleaseResourceHandle( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fHandle = resourceHandle;
	eb.fError = eER_NoErr;

	Call4D( EX_RELEASE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// the resource and the memory it uses are purged from memory
void PA_ReleaseResource( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = id;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_RELEASE_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_GetIndexedResource( short resfile, PA_ulong32 kind, PA_long32 index, char* data )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2 = index;
	eb.fHandle = 0;
	eb.fError = eER_NoErr;

	Call4D( EX_GET_INDEXED_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return ToUserData( eb.fHandle, data );
}


PA_long32 PA_CountResources( short resfile, PA_ulong32 kind )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam3 = 0;
	eb.fParam1 = (PA_long32) kind;
	eb.fError = eER_NoErr;

	Call4D( EX_COUNT_RESOURCES, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam3;
}


PA_long32 PA_CountResourceKinds( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = eER_NoErr;
	eb.fParam3 = 0;

	Call4D( EX_COUNT_RESOURCE_KINDS, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam3;
}


PA_ulong32 PA_GetIndexedResourceKind( short resfile, PA_long32 index )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = eER_NoErr;
	eb.fParam1 = 0;
	eb.fParam3 = index;

	Call4D( EX_GET_INDEXED_RESOURCE_KIND, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_ulong32) eb.fParam1;
}


void PA_GetResourceName( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable   = resfile;
	eb.fParam1  = (PA_long32) kind;
	eb.fParam2  = id;
	eb.fError   = 0;
	eb.fName[0] = 0;
	eb.fHandle  = 0;

	Call4D( EX_GET_RESOURCE_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	if ( sErrorCode == eER_NoErr )
		PA_CopyUnichars( eb.fUName, name, sizeof(eb.fUName) );
}


void PA_SetResourceName( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fParam1   = (PA_long32) kind;
	eb.fParam2   = id;
	eb.fError    = 0;
	PA_CopyUnichars( name, eb.fUName, sizeof(eb.fUName) );
	eb.fHandle   = 0;

	Call4D( EX_SET_RESOURCE_NAME, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_GetResourceSize( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable   = resfile;
	eb.fParam1  = (PA_long32) kind;
	eb.fParam2  = id;
	eb.fParam3  = 0;
	eb.fError   = 0;
	eb.fHandle  = 0;
	eb.fName[0] = 0;

	Call4D( EX_GET_RESOURCE_INFO, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam3;
}


PA_long32 PA_GetResourceTimeStamp ( short resfile, PA_ulong32 kind, PA_long32 id )
{
	EngineBlock eb;

	eb.fTable   = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fParam2   = id;
	eb.fHandle  = 0;
	eb.fError   = 0;

	Call4D( EX_GET_RESOURCE_TIMESTAMP, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam3;
}


PA_long32 PA_GetResourceIDList( short resfile, PA_ulong32 kind, PA_long32* IDlist )
{
	EngineBlock eb;
	PA_long32 len = 0;
	PA_long32 i;
	PA_long32* pt;

	eb.fTable  = resfile;
	eb.fParam1 = (PA_long32) kind;
	eb.fHandle = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_RESOURCE_ID_LIST, &eb );

	if ( eb.fHandle )
	{
		// warning : return the number of id in the list, 
		// not the size in bytes of the list buffer
		len = (PA_long32)PA_GetHandleSize( eb.fHandle ) / (PA_long32) sizeof( PA_long32 );
		
		// if user provides a buffer, copy the ID in the buffer
		if ( IDlist )
		{
			pt = (PA_long32*) PA_LockHandle( eb.fHandle );
			
			for ( i = 0; i < len; i++ )
				*IDlist++ = *pt++;
			
			PA_UnlockHandle( eb.fHandle );
		}
		PA_DisposeHandle( eb.fHandle );
	}

	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return len;	
}


PA_long32 PA_GetResourceKindList( short resfile, PA_ulong32* kindlist )
{
	EngineBlock eb;
	PA_long32 len = 0;
	PA_long32 i;
	PA_ulong32* pt;

	eb.fTable  = resfile;
	eb.fHandle = 0;
	eb.fError  = eER_NoErr;

	Call4D( EX_GET_RESOURCE_KIND_LIST, &eb );

	if ( eb.fHandle )
	{
		// warning : return the number of id in the list, 
		// not the size in bytes of the list buffer
		len = (PA_long32)PA_GetHandleSize( eb.fHandle ) / (PA_long32) sizeof( PA_long32 );
		
		// if user provides a buffer, copy the ID in the buffer
		if ( kindlist )
		{
			pt = (PA_ulong32*) PA_LockHandle( eb.fHandle );
			
			for ( i = 0; i < len; i++ )
				*kindlist++ = *pt++;
			
			PA_UnlockHandle( eb.fHandle );
		}
		PA_DisposeHandle( eb.fHandle );
	}

	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return len;	
}


short PA_OpenResFile( PA_Unistring* filename )
{
	EngineBlock eb;

	eb.fUniString1 = *filename;
	eb.fManyToOne = 0;
	eb.fError     = 0;
	eb.fTable     = -1;
	eb.fParam1	  = 'PRez';	// Opens Pseudo-Resources Only

	Call4D( EX_OPEN_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


short PA_CreateResFile( PA_Unistring* filename )
{
	EngineBlock eb;

	eb.fUniString1 = *filename;
	eb.fError  = eER_NoErr;
	eb.fTable  = -1;

	Call4D( EX_CREATE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fTable;
}


short PA_GetDatabaseResFile()
{
	EngineBlock eb;

	eb.fTable = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_DATABASE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fTable;
}


void PA_UpdateResFile( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = eER_NoErr;
	Call4D( EX_UPDATE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_UseResFile( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = eER_NoErr;
	Call4D( EX_USE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_CloseResFile( short resfile )
{
	EngineBlock eb;

	eb.fTable = resfile;
	eb.fError = eER_NoErr;
	Call4D( EX_CLOSE_RESFILE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


PA_long32 PA_GetUniqueResID( short resfile, PA_ulong32 kind )
{
	EngineBlock eb;

	eb.fTable  = resfile;
	eb.fParam1 = (PA_long32)kind;
	eb.fParam2 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_UNIQUE_RESID, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return (PA_long32)eb.fParam2;
}


void PA_DetachResource( short resfile, PA_Handle resourceHandle )
{
	EngineBlock eb;

	eb.fTable  = resfile;
	eb.fHandle = resourceHandle;
	Call4D( EX_DETACH_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// -----------------------------------------
//
//	4D Application users and groups
//
// -----------------------------------------


void PA_GetUserName( PA_Unichar* name )
{
	EngineBlock eb;

	eb.fUName[0] = 0;
	Call4D( EX_GET_USER_NAME, &eb );
	sErrorCode = eER_NoErr;

	PA_CopyUnichars( eb.fUName, name, sizeof(eb.fUName) );
}


PA_long32 PA_GetCurrentUserID()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_CURRENT_USER_ID, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}

static void GetUsersOrGroupArray( short entrypoint, PA_Variable* usersArray, PA_long32* nbDesignerUsers, PA_long32* nbAdministratorUsers )
{
	EngineBlock eb;

	eb.fError = eER_NoErr;
	eb.fParam1 = 0;
	eb.fParam2 = 0;
	eb.fHandle = (PA_Handle) usersArray;

	Call4D( entrypoint, &eb );

	if ( nbDesignerUsers )
		*nbDesignerUsers = (PA_long32)eb.fParam1;

	if ( nbAdministratorUsers )
		*nbAdministratorUsers = (PA_long32)eb.fParam2;

	sErrorCode = (PA_ErrorCode) eb.fError;
}

PA_Variable PA_GetUsersArray( PA_long32* nbDesignerUsers, PA_long32* nbAdministratorUsers )
{
	PA_Variable var;
	GetUsersOrGroupArray( EX_GET_USER_LIST, &var, nbDesignerUsers, nbAdministratorUsers );
	return var;
}

PA_Variable PA_GetGroupsArray( PA_long32* nbDesignerGroups, PA_long32* nbAdministratorGroups )
{
	PA_Variable var;
	GetUsersOrGroupArray( EX_GET_GROUP_LIST, &var, nbDesignerGroups, nbAdministratorGroups );
	return var;
}


// -----------------------------------------
//
//	4D Application serial key and users info
//
// -----------------------------------------


PA_long32 PA_GetSerialKey()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_SERIAL_KEY, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}


void PA_GetRegisteredUserName( PA_Unichar *name )
{
	EngineBlock eb;

	eb.fParam1 = 'NAME';
	eb.fName[0] = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_REGISTRATION_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUName, name, sizeof(eb.fUName) );
}


void PA_GetRegisteredUserCompany( PA_Unichar *company )
{
	EngineBlock eb;

	eb.fParam1 = 'COMP';
	eb.fName[0] = 0;
	
	Call4D( EX_GET_REGISTRATION_INFO, &eb );
	sErrorCode = eER_NoErr;

	PA_CopyUnichars( eb.fUName, company, sizeof(eb.fUName) );
}


PA_long32 PA_CountConnectedUsers()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_USERS_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam1;
}


PA_long32 PA_GetMaxAllowedUsers()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_USERS_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam2;
}



// -----------------------------------------
//
//	4D Application Plugin area
//
// -----------------------------------------


// force current front window to show current values of variables
void PA_UpdateVariables()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_UPDATE_VARIABLES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// on Windows, this command can convert a 4D window reference
// into a Windows regular HWND.
// if you pass 0, return current window HWND
sLONG_PTR PA_GetHWND( PA_WindowRef windowRef )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) windowRef;
	eb.fError  = eER_NoErr;
	Call4D( EX_GET_HWND, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	return (sLONG_PTR)eb.fHandle;
}

// version 16.0 or greater 
// on Windows, this command return the HWND of the Main MDI window

sLONG_PTR	PA_GetMainWindowHWND()
{
	sLONG_PTR result = NULL;
	EngineBlock	eb = {0};
	Call4D( EX_GET_MAIN_MDI_WINDOW, &eb);
	sErrorCode = (PA_ErrorCode)eb.fError;

	if(eb.fError==0)
		result = eb.fParam1;
	return result;
}

sLONG_PTR PA_GetWindowPtr( PA_WindowRef windowRef )
{
    EngineBlock eb;
    eb.fHandle = (PA_Handle) windowRef;
    eb.fError  = 0;
    Call4D( EX_GET_HWND, &eb );
    sErrorCode = (PA_ErrorCode) eb.fError;
    return (sLONG_PTR)eb.fHandle;
}

PA_PluginRef PA_OpenPluginWindow( PA_Unichar* areaName, PA_Unichar* windowTitle, PA_Rect rect )
{
	EngineBlock eb;

	PA_CopyUnichars( areaName,    eb.fUString, sizeof(eb.fUString) );
	PA_CopyUnichars( windowTitle, eb.fUName,   sizeof(eb.fUName)   );

	eb.fParam1 = rect.fLeft  + ( rect.fTop    << 16 );
	eb.fParam2 = rect.fRight + ( rect.fBottom << 16 );

	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_OPEN_PLUGIN_WINDOW, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_PluginRef) eb.fHandle;
}


void PA_ClosePluginWindow( PA_PluginRef pluginRef )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) pluginRef;
	eb.fError = eER_NoErr;
	Call4D( EX_CLOSE_PLUGIN_WINDOW, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

// when a plugin area is in an external window area, you can use this function
// to change the window title, to reflect for example the name of the current document
// you plugin has opened
void PA_SetPluginWindowTitle( PA_PluginRef pluginRef, PA_Unichar* windowTitle )
{
	EngineBlock eb;

	PA_CopyUnichars( windowTitle, eb.fUName, sizeof(eb.fUName) );
	eb.fHandle = (PA_Handle) pluginRef;
	eb.fError = eER_NoErr;

	Call4D( EX_CHANGE_TITLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


/*
	On Windows 4D allocates an offscreen port and ask form objects,
	including plugins, to draw inside. Then the offscreen is blittered on screen.

	This may be a problem for plugin areas that cannot draw themselves in an offscreen.
	These plugins should inform 4D so that the offscreen is not blittered over their area,
	by calling PA_SetPluginAreaClipMode( ref, 1);
	
	pluginRef is your plugin data handle you provided at plugin area creation.
	set clipChildren to 1 to ask 4D not to draw over your plugin area.
	set clipChildren to 0 to restore normal behavior
	fError is non-zero if no plugin area with specified ref could be found.
*/

void PA_SetPluginAreaClipMode( PA_PluginRef pluginRef, char clipChildren)
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) pluginRef;
	eb.fManyToOne = clipChildren;
	eb.fError  = eER_NoErr;

	Call4D( EX_SET_PLUGINAREA_CLIPMODE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


// return the current page number of the current layout
short PA_GetCurrentPage()
{
	EngineBlock eb;

	eb.fError  = eER_NoErr;
	eb.fParam1 = 0;

	Call4D( EX_GET_CURRENT_PAGE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
	
	return (short) eb.fParam1;
}


PA_WindowRef PA_NewWindow( PA_Rect rect, PA_WindowLevel level, short kind, PA_Unichar* title, char closeBox )
{
	EngineBlock eb;
	
	eb.fParam1 = (PA_long32) level;
	eb.fError = eER_NoErr;
	Call4D( EX_NEXT_WINDOW_LEVEL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	eb.fParam1 = (sLONG_PTR)&rect;
	PA_CopyUnichars( title, eb.fUName, sizeof(eb.fUName) );
	eb.fParam2 = kind;
	eb.fParam3 = (char) closeBox;
	eb.fParam4 = 0;	// may be used to put a window refcon

	Call4D( EX_NEW_WINDOW, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_WindowRef) eb.fHandle;
}


void PA_CloseWindow( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fError = eER_NoErr;
	Call4D( EX_CLOSE_WINDOW, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_SetWindowFocusable( PA_WindowRef windowRef, char focusable )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fManyToOne = focusable;
	eb.fError = eER_NoErr;
	Call4D( EX_SET_WINDOW_FOCUSABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


char PA_IsWindowFocusable( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fManyToOne = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_IS_WINDOW_FOCUSABLE, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fManyToOne;
}


PA_WindowRef PA_GetWindowFocused()
{
	EngineBlock eb;

	eb.fHandle = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_WINDOW_FOCUSED, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_WindowRef) eb.fHandle;
}


void PA_SetWindowFocused( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fError = eER_NoErr;
	Call4D( EX_SET_WINDOW_FOCUSED, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


char PA_IsWindowFocused( PA_WindowRef windowRef )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) windowRef;
	eb.fManyToOne = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_WINDOW_FOCUSED, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;

	return eb.fManyToOne;
}


// if posX and posY equal both zero, tip will be by default at 16 pixels South East from mouse position
// the rectangle is the area of the object that is described by the tip. When mouse will left this area,
// the tip window will close.
// both coordinates are in the local coordinates of the window
void PA_CreateTip( PA_Unistring* ustr, char useRTF, short posX, short posY, PA_Rect rect )
{
	EngineBlock eb;

	eb.fUniString1 = *ustr;
	eb.fParam1 = (sLONG_PTR)&rect;
	eb.fParam2 = useRTF;
	eb.fParam3 = posX;
	eb.fParam4 = posY;
	eb.fError = eER_NoErr;
	Call4D( EX_CREATE_TIP, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_GotoArea( PA_Unichar* variableName )
{
	EngineBlock eb;

	PA_CopyUnichars( variableName, eb.fUName, sizeof(eb.fUName) );
	if ( StripInterprocessVariableName( eb.fUName ) )
		eb.fParam1 = 1;
	else
		eb.fParam1 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_GOTO_AREA, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_UpdateEditMenu( char undo, char redo, char cut, char copy, char paste, char clear, char selectAll )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fError = eER_NoErr;
	if ( undo )
		eb.fParam1 |= 1;

	if ( redo )
		eb.fParam1 |= 2;

	if ( cut )
		eb.fParam1 |= 4;

	if ( copy )
		eb.fParam1 |= 8;

	if ( paste )
		eb.fParam1 |= 16;

	if ( clear )
		eb.fParam1 |= 32;

	if ( selectAll )
		eb.fParam1 |= 64;

	Call4D( EX_UPDATE_EDIT_MENU, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}



// -----------------------------------------
//
//	4D Application Text Editors
//
// -----------------------------------------


PA_Handle PA_GetCurrentTEHandle()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fHandle = 0;
	Call4D( EX_GET_CURRENT_TEHANDLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fHandle;
}


// -----------------------------------------
//
//	4D Application Web Server
//
// -----------------------------------------

void PA_StartWebServer()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_START_WEB_SERVER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_StopWebServer()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_STOP_WEB_SERVER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_GetWebServerInfo( PA_long32* webServerProcess, PA_long32* TCPport )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_WEB_SERVER_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( webServerProcess )
		*webServerProcess = (PA_long32)eb.fParam2;

	if ( TCPport )
		*TCPport = (PA_long32)eb.fParam1;
}


void PA_SetWebTCPport( PA_long32 TCPport )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = TCPport;
	Call4D( EX_SET_WEB_TCP_PORT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_GetWebContext( PA_long32* context, PA_long32* subContext, PA_Unichar* name )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_WEB_CONTEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( context )
		*context = (PA_long32)eb.fParam1;
	
	if ( subContext )
		*subContext = (PA_long32)eb.fParam2;

	PA_CopyUnichars( eb.fUName, name, sizeof(eb.fUName) );
}


PA_long32 PA_GetWebTimeOut()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = -1;
	eb.fParam2 = 0;
	Call4D( EX_WEB_TIMEOUT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam2;
}


void PA_SetWebTimeOut( PA_long32 timeOut )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = timeOut;
	Call4D( EX_WEB_TIMEOUT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_OpenURL( PA_Unistring* url )
{
	EngineBlock eb;

	eb.fUniString1 = *url;
	eb.fError = eER_NoErr;
	Call4D( EX_OPEN_URL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// -----------------------------------------
//
//	4D Application SpellCheckers
//
// -----------------------------------------


void PA_Install4DWriteSpeller( void* spellcheckerProcPtr )
{
	EngineBlock eb;
	eb.fHandle = (PA_Handle) spellcheckerProcPtr;
	eb.fParam1 =  sBinaryFormat;
	eb.fError = eER_NoErr;
	Call4D( EX_INSTALL_4DWRITE_SPELLER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void* PA_Get4DWriteSpellerProcPtr()
{
	EngineBlock eb;
	eb.fHandle = 0;
	eb.fParam1 =  sBinaryFormat;
	eb.fError = eER_NoErr;
	Call4D( EX_GET_4DWRITE_SPELLER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (void*) eb.fHandle;
}



// -----------------------------------------
//
//	4D Application Editors
//
// -----------------------------------------


char PA_FormulaEditor( short defaultTable, PA_Unistring* formula )
{
	EngineBlock eb;

	eb.fUniString1 = *formula;
	eb.fParam1 = 0;
	eb.fTable = defaultTable;
	eb.fError = eER_NoErr;
	Call4D( EX_FORMULA_EDITOR, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( eb.fParam1 )
		*formula = eb.fUniString1;

	return (char) eb.fParam1;
}


void PA_QueryDialog( short table )
{
	EngineBlock eb;
	eb.fTable = table;
	eb.fNbSearchLines = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_QUERY, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_OrderByDialog( short table )
{
	EngineBlock eb;

	eb.fTable = table;
	eb.fNbSearchLines = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_ORDER_BY, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_Handle PA_PictureEditor( PA_Unichar* windowTitle, void* picture, PA_long32 len )
{
	EngineBlock eb;

	eb.fHandle = FromUserData( picture, len );
	eb.fParam1 = 0;
	PA_CopyUnichars( windowTitle, eb.fUName, sizeof(eb.fUName) );
	eb.fParam4 = 'PICT';
	eb.fError = eER_NoErr;
	Call4D( EX_PICTURE_EDITOR, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_Handle) eb.fParam1;
}


void PA_MethodEditor( PA_Unichar* methodName, PA_long32 lineNumber )
{
	EngineBlock eb;

	eb.fManyToOne = 1;
	eb.fParam1 = 0;	// or use CC4D resource ID
	PA_CopyUnichars( methodName, eb.fUName, sizeof(eb.fUName) );
	eb.fParam2 = lineNumber;
	eb.fError = eER_NoErr;

	Call4D( EX_METHOD_EDITOR, &eb );
	sErrorCode = (PA_ErrorCode) eb.fError;
}


void PA_Alert( PA_Unichar* message, PA_WindowRef ref )
{
	EngineBlock eb;

	PA_CopyUnichars( message, eb.fUName, sizeof( eb.fUName ) );
	eb.fParam1 = (sLONG_PTR)ref;
	eb.fError = eER_NoErr;
	Call4D( EX_ALERT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

void PA_CharAlert( const char* message, PA_WindowRef ref )
{
	EngineBlock eb;
	int i;
	for (i=0;i<(sizeof( eb.fUName )/sizeof(PA_Unichar)-1) && message[i];i++)
	{
		eb.fUName[i]=message[i];
	}
	eb.fUName[i]=0;
	eb.fParam1 = (sLONG_PTR)ref;
	eb.fError = eER_NoErr;
	Call4D( EX_ALERT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


char PA_Confirm( PA_Unichar* message, PA_WindowRef ref )
{
	EngineBlock eb;

	PA_CopyUnichars( message, eb.fUName, sizeof( eb.fName ) );
	eb.fParam1 = (sLONG_PTR)ref;
	eb.fError = eER_NoErr;
	Call4D( EX_CONFIRM, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (char) eb.fParam2;
}

// value should point to a 255 PA_Unichar buffer, that will contains the answer.
char PA_Request( PA_Unichar* message, PA_Unichar* value, PA_Unichar* okButton, PA_Unichar* cancelButton )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR)message;
	eb.fParam2 = (sLONG_PTR)value;
	eb.fParam3 = (sLONG_PTR)okButton;
	eb.fParam4 = (sLONG_PTR)cancelButton;
	eb.fError = eER_NoErr;
	Call4D( EX_REQUEST, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fManyToOne;
}


void PA_AboutDialog()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_ABOUT_DIALOG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// -----------------------------------------
//
//	4D Application Pictures
//
// -----------------------------------------


PA_Handle PA_ConvertPicture( void *picture, PA_long32 len, PA_ulong32 format )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fParam2 = 'QTIM';
	eb.fParam3 = format;
	eb.fHandle = FromUserData( picture, len );
	eb.fError = eER_NoErr;
	Call4D( EX_PICTURE_CONVERSION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_Handle) eb.fParam1;
}


PA_Handle PA_PictureToGIF( void* picture, PA_long32 len )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fParam2 = 'GIF ';
	eb.fHandle = FromUserData( picture, len );
	eb.fError = eER_NoErr;
	Call4D( EX_PICTURE_CONVERSION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return (PA_Handle) eb.fParam1;
}


// returns a HEMF
void* PA_PictureToEMF( void* picture, PA_long32 len )
{
	EngineBlock eb;

	eb.fParam1 = 0;
	eb.fParam2 = 'EMF ';
	eb.fHandle = FromUserData( picture, len );
	eb.fError = eER_NoErr;
	Call4D( EX_PICTURE_CONVERSION, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( eb.fHandle )
		PA_DisposeHandle( eb.fHandle );

	return (void*)eb.fParam1;
}



// -----------------------------------------
//
//	4D Application Dialogs
//
// -----------------------------------------


PA_Dial4D PA_NewDialog()
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_EMPTY_DIALOG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (PA_Dial4D)eb.fParam1;
}


PA_Dial4D PA_OpenDialog( PA_Dial4D dialog, PA_Unichar* dialogName, PA_Unichar* windowTitle, char closeBox )
{
	EngineBlock eb;
	PA_Rect rect = {0,0,0,0}; // will use default dialog rect

	eb.fParam4 = (sLONG_PTR)dialog;
	eb.fParam1 = 0;
	eb.fLongint = '6565';
	eb.fOneToMany = 0;	// hidden
	eb.fManyToOne = closeBox;
	eb.fParam2 = (sLONG_PTR) & rect;
	eb.fParam3 = eWL_Dialog;	// PA_WindowLevel
	eb.fShort = 1;	// modal
	eb.fError = eER_NoErr;

	PA_CopyUnichars( dialogName, eb.fUName, sizeof(eb.fUName) );
	PA_CopyUnichars( windowTitle, eb.fUString, sizeof(eb.fUString) );

	Call4D( EX_DIAL4D_OPEN_DIALOG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_Dial4D) eb.fParam1;
}


char PA_ModalDialog( PA_Dial4D dialog, PA_Unichar* variableName )
{
	EngineBlock eb;
	char val = 0;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = 0;
	eb.fParam3 = sBinaryFormat;
	
	Call4D( EX_DIAL4D_MODAL_DIALOG, &eb );
	
	PA_CopyUnichars( eb.fUString, variableName, sizeof(eb.fUString) );

	if ( eb.fManyToOne )		// cancel
		val = 2;
	else if( eb.fOneToMany )	// validate
		val = 1;

	return val;
}


void PA_CloseDialog( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fError = eER_NoErr;
	eb.fParam1 = (sLONG_PTR) dialog;
	Call4D( EX_DIAL4D_CLOSE_DIALOG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetEnable( PA_Dial4D dialog, PA_Unichar* variable, char enabled )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fManyToOne = enabled;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_SET_ENABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_Variable PA_Dial4DGetVariable( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;
	PA_Variable var;

	eb.fError = eER_NoErr;
	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fHandle = (PA_Handle) &var;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_GET_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return var;
}


void PA_Dial4DSetVariable( PA_Dial4D dialog, PA_Unichar* variable, PA_Variable var )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fHandle = (PA_Handle) &var;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_SET_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_long32 PA_Dial4DGetLong( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fLongint = 0;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_GET_LONG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return eb.fLongint;
}


void PA_Dial4DSetLong( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 value )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fLongint = value;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_SET_LONG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


double PA_Dial4DGetReal( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fReal = 0.0;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_GET_REAL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return eb.fReal;
}


void PA_Dial4DSetReal( PA_Dial4D dialog, PA_Unichar* variable, double value )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fReal = value;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_SET_REAL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DGetString( PA_Dial4D dialog, PA_Unichar* variable, PA_Unichar* string )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fName[0] = 0;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_GET_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	PA_CopyUnichars( eb.fUName, string, sizeof(eb.fUName) );
}


void PA_Dial4DSetString( PA_Dial4D dialog, PA_Unichar* variable, PA_Unichar* string )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	PA_CopyUnichars( string, eb.fUName, sizeof(eb.fUName) );
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	Call4D( EX_DIAL4D_SET_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

PA_Unistring PA_Dial4DGetText( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;
	eb.fError = eER_NoErr;
	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fName[0] = 0;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );

	Call4D( EX_DIAL4D_GET_TEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fUniString1;
}


void PA_Dial4DSetText( PA_Dial4D dialog, PA_Unichar* variable, PA_Unistring* ustr )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fUniString1 = *ustr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_TEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetPicture( PA_Dial4D dialog, PA_Unichar* variable, PA_Picture picture )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fPicture = picture;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_PICTURE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

// dialogs use their own variable context. If you want to access
// the current process variables, you need to switch from dialog variables
// to current process variables
void* PA_Dial4DSaveVariables( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = 0;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SAVE_VARIABLES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (void*) eb.fParam2;
}

// to switch back to the dialog variable, after calling PA_Dial4DSaveVariables
// You must pass the value returned by PA_Dial4DSaveVariables
void PA_Dial4DRestoreVariables( PA_Dial4D dialog, void* env )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (sLONG_PTR) env;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_RESTORE_VARIABLES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


double PA_Dial4DGetArrayReal( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 indice )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = indice;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_ARRAY_REAL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fReal;
}


void PA_Dial4DGetArrayString( PA_Dial4D dialog, PA_Unichar* variable, PA_Unichar* string, PA_long32 indice )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = indice;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_ARRAY_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	PA_CopyUnichars( eb.fUName, string, sizeof(eb.fUName) );
}


PA_long32 PA_Dial4DGetArrayLong( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 indice )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = indice;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_ARRAY_LONG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (PA_long32)eb.fParam3;
}


void PA_Dial4DSetAreaHandler( PA_Dial4D dialog, PA_Unichar* variable, void* handler, void* privateData )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = 6642;
	eb.fParam3 = sBinaryFormat;
	eb.fHandle = (PA_Handle) privateData;
	eb.fLongint = (sLONG_PTR) handler;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_AREA_HANDLER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DGetVariableRect( PA_Dial4D dialog, PA_Unichar* variable, PA_Rect* rect )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (sLONG_PTR) rect;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_VARIABLE_RECT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayTextFromResource( PA_Dial4D dialog, PA_Unichar* variable, short resourceID )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = 'STR#';	// also supported : 'TXT#' and 'MENU'
	eb.fParam3 = (PA_long32) resourceID;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_RESOURCE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayTextFromTableList( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam4 = sVirtualStructureMode;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_TABLE_LIST, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayTextFromFieldList( PA_Dial4D dialog, PA_Unichar* variable, short table )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = 0;
	eb.fParam4 = sVirtualStructureMode;
	eb.fTable = table;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_FIELD_LIST, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// to be called before changing variables values
void PA_Dial4DBeginUpdateVariables( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_BEGIN_UPDATE_VARIABLES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// to be called once variables values are changed
// all the modified variables will be redrawn
void PA_Dial4DEndUpdateVariables( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_END_UPDATE_VARIABLES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DNewArrayString( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_ARRAY_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayString( PA_Dial4D dialog, PA_Unichar* variable, PA_Unichar* string, PA_long32 position )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = position;
	PA_CopyUnichars( string, eb.fUName, sizeof(eb.fUName) );
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_STRING, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetDate( PA_Dial4D dialog, PA_Unichar* variable, short day, short month, short year )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_DATE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DGetDate( PA_Dial4D dialog, PA_Unichar* variable, short* day, short* month, short* year )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_DATE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( day )
		*day = eb.fDate.fDay;

	if ( month )
		*month = eb.fDate.fMonth;
	
	if ( year )
		*year = eb.fDate.fYear;
}


void PA_Dial4DShowHideVariable( PA_Dial4D dialog, PA_Unichar* variable, char visible )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) visible;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SHOW_HIDE_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DGotoPage( PA_Dial4D dialog, short page )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) page;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GOTO_PAGE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


short PA_Dial4DGetCurrentPage( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_CURRENT_PAGE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (short) eb.fParam2;
}

// to get all the tables, pass 0 to table
void PA_Dial4DNewTableFieldHList( PA_Dial4D dialog, PA_Unichar* variable, short table,
								  char showRelatedTables, char showRelatedFields, 
								  char useIcons, char sorted )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fManyToOne = showRelatedFields;	// only if table > 0
	eb.fField  = showRelatedTables;
	eb.fParam2 = 0;
	eb.fParam3 = (PA_long32) sorted;
	eb.fOneToMany = useIcons;
	eb.fParam4 = sVirtualStructureMode;
	eb.fTable  = table;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_TABLE_FIELD_HLIST, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DDisposeHList( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_DISPOSE_HLIST, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DGetDragInfo( PA_Dial4D dialog,
						   PA_Unichar* dropVariable, short* dropX, short* dropY,
						   PA_Dial4D* dragDialog,
						   PA_Unichar* dragVariable, short* dragX, short* dragY )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_DRAG_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	PA_CopyUnichars( eb.fUString, dropVariable, sizeof(eb.fUString) );

	if ( dropX )
		*dropX = ( (PA_Point*) &eb.fParam2 )->fh;

	if ( dropY )
		*dropY = ( (PA_Point*) &eb.fParam2 )->fv;

	PA_CopyUnichars( eb.fUName, dragVariable, sizeof(eb.fUName) );

	if ( dragDialog )
		*dragDialog = (PA_Dial4D) eb.fParam3;

	if ( dragX )
		*dragX = ( (PA_Point*) &eb.fParam4 )->fh;

	if ( dragY )
		*dragY = ( (PA_Point*) &eb.fParam4 )->fv;
}


void PA_Dial4DGetTableFieldHListCurrent( PA_Dial4D dialog, PA_Unichar* variable, short* table, short* field )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam4 = sVirtualStructureMode;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_TABLE_FIELD_HLIST_CURRENT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( table )
		*table = eb.fTable;

	if ( field )
		*field = eb.fField;
}


void PA_Dial4DNewArrayLong( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_ARRAY_LONG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayLong( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 value, PA_long32 position )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = position;
	eb.fParam3 = value;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_LONG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

// each element of the text array will be filled by a paragraph of the given text.
// paragraphs in source text needs to be separated by a carriage return.
void PA_Dial4DSetArrayTextFromTTR( PA_Dial4D dialog, PA_Unichar* variable, PA_Unistring* ustr )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = 0;
	eb.fUniString1 = *ustr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT_FROM_TTR, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_long32 PA_Dial4DGetArraySize( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_ARRAY_SIZE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam2;
}


void PA_Dial4DDeleteArrayElements( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 position, PA_long32 amount )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = position;
	eb.fParam3 = amount;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_DELETE_ARRAY_ELEMENTS, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DRedrawExternalArea( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_REDRAW_EXTERNAL_AREA, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DNewArrayPicture( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_ARRAY_PICTURE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayPicture( PA_Dial4D dialog, PA_Unichar* variable, PA_Picture picture, PA_long32 position )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = position;
	eb.fPicture = picture;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_PICTURE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


// idArray is an array of short that holds the ID of the PICT resources to load.
// nb is the number of ids in the array 
void PA_Dial4DSetArrayPictureFromResources( PA_Dial4D dialog, PA_Unichar* variable, short* idArray, PA_long32 nb )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fHandle = (PA_Handle) idArray;
	eb.fParam2 = nb;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_PICT_FROM_RESOURCES, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetEnterable( PA_Dial4D dialog, PA_Unichar* variable, char enterable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fManyToOne = enterable;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ENTERABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

}


void PA_Dial4DSetMin( PA_Dial4D dialog, PA_Unichar* variable, double minValue )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = eVK_Real;
	eb.fReal   = minValue;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_MIN, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetMinDate( PA_Dial4D dialog, PA_Unichar* variable, short day, short month, short year )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = eVK_Date;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_MIN, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetMax( PA_Dial4D dialog, PA_Unichar* variable, double maxValue )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = eVK_Real;
	eb.fReal   = maxValue;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_MAX, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetMaxDate( PA_Dial4D dialog, PA_Unichar* variable, short day, short month, short year )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = eVK_Date;
	eb.fDate.fDay   = day;
	eb.fDate.fMonth = month;
	eb.fDate.fYear  = year;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_MAX, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetUserData( PA_Dial4D dialog, void* userData )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (sLONG_PTR) userData;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_USER_DATA, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void* PA_Dial4DGetUserData( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_USER_DATA, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (void*) eb.fParam2;
}


void PA_Dial4DSet3StatesCheckBox( PA_Dial4D dialog, PA_Unichar* variable, char state )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) state;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_3_STATES_CHECKBOX, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_Dial4D PA_Dial4DDial4DFromWindow( PA_WindowRef window )
{
	EngineBlock eb;

	eb.fParam2 = (sLONG_PTR) window;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_DIAL4D_FROM_WINDOW, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (PA_Dial4D) eb.fParam1;
}


PA_WindowRef PA_Dial4DWindowFromDial4D( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_WINDOW_FROM_DIAL4D, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return (PA_WindowRef) eb.fParam2;
}


void PA_Dial4DSetTableFieldHListCurrent( PA_Dial4D dialog, PA_Unichar* variable, short table, short field )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fTable  = table;
	eb.fField  = field;
	eb.fParam4 = sVirtualStructureMode;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_TABLE_FIELD_HLIST_CURRENT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

void PA_Dial4DNewArrayReal( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_ARRAY_REAL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayReal( PA_Dial4D dialog, PA_Unichar* variable, double value, PA_long32 position )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fReal = value;
	eb.fParam2 = position;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_REAL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetEntryFilter( PA_Dial4D dialog, PA_Unichar* variable, PA_Unichar* entryFilter )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	PA_CopyUnichars( entryFilter, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ENTRY_FILTER, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetDisplayFormat( PA_Dial4D dialog, PA_Unichar* variable, PA_Unichar* displayFormat )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	PA_CopyUnichars( displayFormat, eb.fUName, sizeof(eb.fUName) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_DISPLAY_FORMAT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DNewArrayText( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_NEW_ARRAY_TEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetArrayText( PA_Dial4D dialog, PA_Unichar* variable, PA_Unistring* ustr, PA_long32 position )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = position;
	eb.fUniString1 = *ustr;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_ARRAY_TEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_Unistring PA_Dial4DGetArrayText( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 position )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = position;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_ARRAY_TEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	return eb.fUniString1;
}


void PA_Dial4DDisposeEmptyDialog( PA_Dial4D dialog )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_DISPOSE_EMPTY_DIALOG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DShowHideObject( PA_Dial4D dialog, PA_Unichar* objectName, char visible )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) visible;
	PA_CopyUnichars( objectName, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SHOW_HIDE_OBJECT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetTime( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 time )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fLongint = time;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_TIME, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_long32 PA_Dial4DGetTime( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_TIME, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return eb.fLongint;
}


void PA_Dial4DSetObjectTitle( PA_Dial4D dialog, PA_Unichar* objectName, PA_Unichar* title )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fManyToOne = 0;	// 1 to pass variable name
	PA_CopyUnichars( title, eb.fUName, sizeof(eb.fUName) );
	PA_CopyUnichars( objectName, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_OBJECT_TITLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DUpdateObjectTitle( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_UPDATE_OBJECT_TITLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DSetVariableRect( PA_Dial4D dialog, PA_Unichar* variable, PA_Rect rect )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (sLONG_PTR) &rect;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_VARIABLE_RECT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DAllowXResize( PA_Dial4D dialog, char allowResize )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) allowResize;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_ALLOW_X_RESIZE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DAllowYResize( PA_Dial4D dialog, char allowResize )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) allowResize;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_ALLOW_Y_RESIZE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DGetWindowMinMaxInfo( PA_Dial4D dialog,
									PA_long32* minXresize, PA_long32* maxXresize,
									PA_long32* minYresize, PA_long32* maxYresize )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_WINDOW_MIN_MAX_INFO, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	if ( minXresize )
		*minXresize = (PA_long32)eb.fParam1;

	if ( maxXresize )
		*maxXresize = (PA_long32)eb.fParam2;

	if ( minYresize )
		*minYresize = (PA_long32)eb.fParam3;

	if ( maxYresize )
		*maxYresize = (PA_long32)eb.fParam4;

}


void PA_Dial4DSetWindowSize( PA_Dial4D dialog, PA_long32 width, PA_long32 height )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = width;
	eb.fParam3 = height;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_SET_WINDOW_SIZE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


PA_long32 PA_Dial4DFindArrayLong( PA_Dial4D dialog, PA_Unichar* variable, PA_long32 value, PA_long32 startPosition )
{
	EngineBlock eb;

	eb.fParam1  = (sLONG_PTR) dialog;
	eb.fLongint = value;
	eb.fParam2  = startPosition;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_FIND_ARRAY_LONG, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam2;
}


PA_long32 PA_Dial4DFindArrayReal( PA_Dial4D dialog, PA_Unichar* variable, double value, PA_long32 startPosition )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fReal = value;
	eb.fParam2 = startPosition;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_FIND_ARRAY_REAL, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;

	return (PA_long32)eb.fParam2;
}


void PA_Dial4DGetLastObject( PA_Dial4D dialog, PA_Unichar* objectName )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GET_LAST_OBJECT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
	PA_CopyUnichars( eb.fUName, objectName, sizeof(eb.fUName) );
}


void PA_Dial4DGotoVariable( PA_Dial4D dialog, PA_Unichar* variable )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GOTO_VARIABLE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DCancelValidate( PA_Dial4D dialog, char cancel )
{
	EngineBlock eb;

	eb.fHandle = (PA_Handle) dialog;
	eb.fParam2 = (PA_long32) cancel;
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_CANCEL_VALIDATE, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}


void PA_Dial4DHighlightText( PA_Dial4D dialog, PA_Unichar* variable, short startSelection, short endSelection )
{
	EngineBlock eb;

	eb.fParam1 = (sLONG_PTR) dialog;
	eb.fParam2 = (PA_long32) startSelection;
	eb.fParam3 = (PA_long32) endSelection;
	PA_CopyUnichars( variable, eb.fUString, sizeof(eb.fUString) );
	eb.fError = eER_NoErr;
	Call4D( EX_DIAL4D_GOTO_VARIABLE, &eb );	// YT 13-Dec-2004 - ACI0033916
	Call4D( EX_DIAL4D_HIGHLIGHT_TEXT, &eb );
	sErrorCode = (PA_ErrorCode)eb.fError;
}

void PA_UseQuartzAxis( PA_PluginParameters params,short *outAreaX,short *outAreaY, short* outAreaWidth, short *outAreaHeight)
{
#if VERSIONMAC
	PA_Rect portBounds;
	PA_Rect areaBounds;
	PA_PluginProperties props;
	CGContextRef context = NULL;
	PA_AreaEvent event = PA_GetAreaEvent(params) ;
	
	switch(event)
	{
		case eAE_GetPrintSize:
		case eAE_PrintBand:	
		case eAE_Update:
		{
			context = (CGContextRef)  (( (PA_Event**) params->fParameters )[ 0 ] )->fMessage;
			break;
		}
		case eAE_DesignUpdate:
		{
			PA_GetPluginProperties( params, &props );
			context = (CGContextRef) props.fMacPort;
			break;
		}
	}
	if(context!=NULL)
	{
		portBounds = PA_GetAreaPortBounds(params);
		areaBounds = PA_GetAreaRect( params );
		
		CGContextScaleCTM(context,1.0,-1.0);
		CGContextTranslateCTM(context,0,-(portBounds.fBottom-portBounds.fTop));
		
		*outAreaX = areaBounds.fLeft;
		*outAreaWidth = (areaBounds.fRight - areaBounds.fLeft);
		*outAreaHeight = (areaBounds.fBottom - areaBounds.fTop);
		*outAreaY = (portBounds.fBottom-portBounds.fTop) - ( areaBounds.fTop + *outAreaHeight);
	}
#endif
}

void PA_UseQuickdrawAxis( PA_PluginParameters params,short *outAreaX,short *outAreaY, short* outAreaWidth, short *outAreaHeight)
{
#if VERSIONMAC
	PA_Rect portBounds;
	PA_Rect areaBounds;
	PA_PluginProperties props;
	CGContextRef context = NULL;
	PA_AreaEvent event = PA_GetAreaEvent(params) ;
	
	switch(event)
	{
		case eAE_GetPrintSize:
		case eAE_PrintBand:	
		case eAE_Update:
		{
			context = (CGContextRef)  (( (PA_Event**) params->fParameters )[ 0 ] )->fMessage;
			break;
		}
		case eAE_DesignUpdate:
		{
			PA_GetPluginProperties( params, &props );
			context = (CGContextRef) props.fMacPort;
			break;
		}
	}
	if(context!=NULL)
	{
		portBounds = PA_GetAreaPortBounds(params);
		areaBounds = PA_GetAreaRect( params );
		
		CGContextTranslateCTM(context,0,(portBounds.fBottom-portBounds.fTop));
		CGContextScaleCTM(context,1.0,-1.0);
		
		*outAreaX = areaBounds.fLeft;
		*outAreaWidth = (areaBounds.fRight - areaBounds.fLeft);
		*outAreaHeight = (areaBounds.fBottom - areaBounds.fTop);
		*outAreaY = portBounds.fTop;
	}
#endif
}

// v14 or higher
// return the external area objec name.

PA_Unistring* PA_GetAreaObjectName(PA_PluginParameters params)
{
	return ( ( (PA_Unistring**) params->fParameters ) [ 5 ] );
}

// v14 or higher
// Get the visible state of the external area, can be call during eAE_ShowHide and  eAE_PageChange
// 4D send eAE_ShowHide when Object Set Visible is call from 4d Language. This notification allow you to show or hide native control for example.
// You don't need to redraw or inval, 4D will do it for you

char PA_IsAreaVisible( PA_PluginParameters params )
{
	PA_Event* ev;
	char visible = 0;

	ev = ( (PA_Event**) params->fParameters )[ 0 ];

	if ( ev->fWhat == eAE_ShowHide || ev->fWhat == eAE_PageChange)
	{
		sErrorCode = eER_NoErr;
		visible = (char)( ( ev->fWhereV == 1 ) ? 1 : 0 );
	}
	else
		sErrorCode = eER_BadEventCall;

	return visible;
}
