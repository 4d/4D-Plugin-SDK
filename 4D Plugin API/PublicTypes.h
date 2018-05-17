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
// File : PublicTypes.h
// Description : all the structures needed to use 4D Plugin API
//
// rev : v13
//
// ---------------------------------------------------------------

#ifndef __PUBLICTYPES__
#define __PUBLICTYPES__

// all the 4D Application structures use 2 bytes alignment
#pragma pack(push,2)

#if VERSIONWIN
typedef __int64 PA_long64;
typedef unsigned __int64 PA_ulong64;
typedef unsigned long PA_ulong32;
typedef long PA_long32;
#elif VERSIONMAC
    #if PA_64BITS_ARCHITECTURE
    typedef unsigned int PA_ulong32;
    typedef signed int PA_long32;
    #else
    typedef unsigned long PA_ulong32;
    typedef signed long PA_long32;
    #endif
typedef signed long long PA_long64;
typedef unsigned long long PA_ulong64;
#endif

typedef char** PA_Handle;
typedef void* PA_WindowRef;
typedef void* PA_HDC;
typedef void* PA_PortRef;
typedef void* PA_PluginRef;
typedef PA_Handle PA_QueryRef;
typedef PA_Handle PA_OrderByRef;
typedef void* PA_Dial4D;
typedef unsigned short PA_Unichar;
typedef void* PA_Picture;
typedef void* PA_DragContextRef;
typedef void* PA_PasteboardRef;
typedef void* PA_ObjectRef;
typedef void* PA_CollectionRef;
typedef struct
{
	PA_long32			fLength;
	PA_Unichar*		fString;
	PA_long32			fReserved1;
	PA_long32			fReserved2;
} PA_Unistring;

// need a crossplatform type for 64 bits integers
#if PA_64BITS_ARCHITECTURE
	typedef PA_long64 sLONG_PTR;
	typedef PA_ulong64 uLONG_PTR;
#else
	typedef PA_long32 sLONG_PTR;
	typedef PA_ulong32 uLONG_PTR;
#endif

// function pointers to pass to PA_RunInMainProcess
typedef void (*PA_RunInMainProcessProcPtr) (void*);

// opaque structure to store Plugin call parameters
typedef struct PluginBlock
{
	void*			fParameters;
	void*			fResult;
	void**			fData;
} PluginBlock;
typedef PluginBlock* PA_PluginParameters;


// different selectors that can be sent to plugin for different event

// MoB:ACI0034535
#define  kNotifyDemoPlugins			-250

#define  kServerDeinitPlugin		-220
#define  kServerCleanUp				-207
#define  kWriteOnServer				-206
#define  kReadOnServer				-205
#define  kClientDisconnect			-202
#define  kClientConnect				-201
#define  kServerInitPlugin			-200
#define  kPackageGiveMemory			-10
#define  kCreateProcess				-3		// called each time a process is created
#define  kCloseProcess				-4		// called each time a process is removed
#define  kClientDeinitPlugin		-2 
#define  kDeinitPlugin				kClientDeinitPlugin
#define  kClientInitPlugin			-1
#define  kInitPlugin				kClientInitPlugin




// --------------------------------------------------------------------------------
// Event Record (same as Macintosh EventRecord)
// --------------------------------------------------------------------------------

typedef struct PA_Event
{
	short			fWhat;
	sLONG_PTR		fMessage;
	PA_long32			fWhen;
	short			fWhereV;
	short			fWhereH;
	short			fModifiers;
} PA_Event;


// --------------------------------------------------------------------------------
// Plugin area events
// --------------------------------------------------------------------------------


typedef enum
{
	eAE_Idle							= 0,
	eAE_MouseDown						= 1,
	eAE_MouseUp							= 2,
	eAE_KeyDown							= 3,
	eAE_KeyUp							= 4,
	eAE_AutoKey							= 5,
	eAE_Update							= 6,
	eAE_InitArea						= 16,	// first call to area, time to call PA_SetAreaReference
	eAE_Deinit							= 17,
	eAE_Cursor							= 18,	// mouse has moved (even if your area is not selected)
	eAE_IsFocusable						= 20,
	eAE_Select							= 21,
	eAE_Deselect						= 22,
	eAE_Scroll							= 25,	// area rect has changed, call PA_GetAreaRect
	eAE_DesignUpdate					= 26,
	eAE_TestPrintSize					= 27,
	eAE_GetPrintSize					= 28,
	eAE_PrintBand						= 29,
	eAE_UndoCommand						= 30,
	eAE_CutCommand						= 31,
	eAE_CopyCommand						= 32,
	eAE_PasteCommand					= 33,
	eAE_ClearCommand					= 34,
	eAE_SelectAllCommand				= 35,
	eAE_RedoCommand						= 36,	// added in 4D 2004.4
	eAE_MouseWheel						= 39,
	eAE_UpdateEditCommands				= 43,	// you need to call PA_UpdateEditMenu 
	eAE_LoadRecord						= 69,
	eAE_SaveRecord						= 70,
	eAE_PageChange						= 71,
	eAE_EndExecutionCycle				= 72,
	eAE_AllowDrop						= 80,
	eAE_Drag							= 81,
	eAE_Drop							= 82,
	eAE_BeginDrag						= 85,	// added in v11
	eAE_ShowHide						= 86,	// added in v14
	eAE_WebPublish						= 128,
	eAE_WebPublishPicture				= 129,
	eAE_WebDisposeData					= 130,
	eAE_WebClick						= 131,
	eAE_EditAdvancedProperties			= 600,	// user clicks on "Advanced Properties" button
	eAE_DisposeAdvancedProperties		= 601,	// 4D has stored the advanced plugin properties, you can now dispose your data
	eAE_InitAdvancedProperties			= 602,	// it's time to read the advanced properties
	eAE_AreAdvancedPropertiesEditable	= 603,	// 4D wants to know if you want to use "Advanced Properties" button
	eAE_GetMenuIcon						= 604,	// 4D requests the id of the 'cicn' resources used in plugin menu
	eAE_DesignInit						= 605	// called when layout is opened in design mode
} PA_AreaEvent;


// --------------------------------------------------------------------------------
// Keycode returned by PA_GetKey
// --------------------------------------------------------------------------------

typedef enum PA_KeyCode
{
	KEY_DUMMY = -1,
	// special value for system characters and unhandled keys
	KEY_OTHER = 0,
	// mixed keys
	KEY_RETURN = 0x0D,
	KEY_TAB = 0x09,
	KEY_SPACE = ' ',
	// numeric keys
	KEY_ZERO = '0',
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_DECIMAL = '.',
	KEY_DIVIDE = 0x2F,
	KEY_MULTIPLY = '*',
	KEY_SUBSTRACT = '-',
	KEY_ADD = '+',
	KEY_EQUAL = '=',
	KEY_A = 'A',
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	// numeric pad
	KEY_PAD_0 = 0x0080, // 128
	KEY_PAD_1,
	KEY_PAD_2, // 130
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7, // 135
	KEY_PAD_8,
	KEY_PAD_9,
	// navigation keys
	KEY_HOME, // 138
	KEY_END,
	KEY_PAGEUP, // 140
	KEY_PAGEDOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN, // 145
	// modifier keys
	KEY_CTRL,
	KEY_ALT,
	KEY_SHIFT,
	// action keys
	KEY_BACKSPACE,
	KEY_DELETE, // 150
	KEY_INSERT,
	KEY_ENTER,
	KEY_ESCAPE,
	KEY_HELP,
	KEY_PRINT, // F13 (155)
	KEY_BREAK, // F14
	KEY_PAUSE, // F15
	KEY_F1,
	KEY_F2,
	KEY_F3, // 160
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8, // 165
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12
} PA_KeyCode;

// --------------------------------------------------------------------------------
// Plugin area properties blocks
// --------------------------------------------------------------------------------

typedef struct PA_PluginProperties
{
	short			fVersion;
	short			fFontID;			//	Macintosh font ID
	short			fJustification;
	char			fFontSize;
	char			fFontFace;
	PA_long32			fForeColor;
	PA_long32			fBackColor;
	void*			fAdvancedProperties;
	PA_long32			fAdvancedPropertiesSize;
	char			fPageMode;		//	0: List, 1: Page, 2: Page non enterable.
	char			fPrintingMode;	//	0: Not printing, -1: Printing, -2: Print line.
	short			fPage;
	short			fTable;
	void*			fUnused;
	char			fDraggable;
	char			fDroppable;
	short			fLook;
	void*			fMacWindow;
	void*			fMacPort;
	void*			fWinHWND;
	void*			fWinHDC;
	char			fInterProcessVariable;
} PA_PluginProperties;



// --------------------------------------------------------------------------------
// Rectangle Record
// --------------------------------------------------------------------------------

typedef struct PA_Rect
{
	short			fTop;
	short			fLeft;
	short			fBottom;
	short			fRight;
} PA_Rect;


// --------------------------------------------------------------------------------
// Point Record
// --------------------------------------------------------------------------------

typedef struct PA_Point
{
	short			fv;
	short			fh;
} PA_Point;


// --------------------------------------------------------------------------------
// 4D Server and 4D Client, Read/Write data structure
// --------------------------------------------------------------------------------

typedef struct PA_ReadWriteBlock
{
   PA_ulong32	fDataType;
   short			fDataID;
   PA_long32				fDataSize;
   sLONG_PTR		fPackID;
   PA_long32				fProcessID;
} PA_ReadWriteBlock;



// --------------------------------------------------------------------------------
// 4D Application Arrays 
// --------------------------------------------------------------------------------

typedef struct PA_Array
{
	PA_long32			fNbElements;	// Number of elements
	PA_Handle		fData;			// Handle to elements
	short			fCurrent;		// Selected element number
} PA_Array;

// --------------------------------------------------------------------------------
// 4D Application Array Variable
// --------------------------------------------------------------------------------

typedef struct PA_ArrayVariable
{
      char                    fType;
      char                    fFiller;
      PA_Array                fArray;
} PA_ArrayVariable;

// --------------------------------------------------------------------------------
// 4D Application Dates 
// --------------------------------------------------------------------------------

typedef struct PA_Date
{
	short			fDay;
	short			fMonth;
	short			fYear;
} PA_Date;

// --------------------------------------------------------------------------------
// 4D Application Blob expression 
// --------------------------------------------------------------------------------
typedef struct PA_Blob
{
   PA_long32				fSize;
	PA_Handle		fHandle;
} PA_Blob;

// --------------------------------------------------------------------------------
// 4D Application pointers
// --------------------------------------------------------------------------------
typedef enum
{
	ePK_InvalidPointer = -1,
	ePK_PointerToVariable = 0,
	ePK_PointerToTable,
	ePK_PointerToField
} PA_PointerKind;

typedef struct PointerToVariable
{
	char			fName[ 32 ];
	PA_long32			fIndice;		// used for pointer to array elements
	PA_long32			fTag;			// added in 2004.1
} PointerToVariable;


typedef struct PointerToTableField
{
	short			fField;			// field number. 0 if pointer to table
	short			fTable;
	short			fNbSubTables;
	short			fSubTables[5];
} PointerToTableField;

typedef struct PointerBlock
{
	char			fClass;			// 0 : field,   1 : variable
	char			fScope;			// 0 : local, 1 : process, 2 : interprocess
	union
	{
		PointerToVariable		fVariable;
		PointerToTableField		fTableField;
	} uValue;
} PointerBlock;

typedef PointerBlock* PA_Pointer;


// --------------------------------------------------------------------------------
// 4D Application Drag and Drop info
// --------------------------------------------------------------------------------

typedef enum
{
	eDK_InvalidDrag = -1,
	eDK_DragVariable = 0,
	eDK_DragTable,
	eDK_DragField
} PA_DragKind;


typedef struct PA_DragAndDropInfo
{
#if     PA_64BITS_ARCHITECTURE
	char            fReserved1[18];
#else
	char            fReserved1[10];
#endif

	PA_long32			fToArrayIndice;		// indice of element when destination is an array
	sLONG_PTR			fReserved2[2];
	PA_long32			fFromArrayIndice;	// indice of element when source is an array
	short				fFromProcess;
	short				fFromWhereV;		// where user clicks at first
	short				fFromWhereH;
	short				fToWhereV;			// where user release mouse button
	short				fToWhereH;
	sLONG_PTR			fReserved3;
	char				fVariableName[32];	// empty string or variable name if user drags a variable
	char				fInterProcess;
	short 				fField;
	short				fTable;				// zero or table name if user drags a field from a table
	PA_DragContextRef	fDragContext;		// the current drag and drop context (new v11 field)
} PA_DragAndDropInfo;



// --------------------------------------------------------------------------------
// 4D Application Variables
// --------------------------------------------------------------------------------

typedef struct PA_Variable
{
	char fType;
	char fFiller;
	union
	{ 
		double			fReal;		// C_REAL variable
		PA_Date			fDate;		// C_DATE variable
		char			fBoolean;	// C_BOOLEAN variable
		PA_Picture		fPicture;	// C_PICTURE variable
		PA_Blob			fBlob;		// C_BLOB variable
		PA_long32			fLongint;	// C_LONGINT variable
		PA_long32			fTime;		// C_TIME variable
		PA_Unistring	fString;	// C_STRING and C_TEXT variables
		PA_Array		fArray;		// Any array
		PA_Pointer*		fPointer;	// C_POINTER variables
		unsigned char	fOperation;	// to pass '*', '<' or '>' to PA_ExecuterCommandByID
		PA_ObjectRef	fObject;
		PA_CollectionRef fCollection;
		struct
		{
			short fFieldNumber;	// to pass a field or table to PA_ExecuteCommandByID
			short fTableNumber;	// pass 0 to pass only a field
			short fUnused1;		// always set to zero
		} fTableFieldDefinition;

		struct 
		{
			char fName[ 32 ];
			PA_long32 fIndice;		// used for pointer to array elements
			char fType;			// 0: local, 1: process, 2: interprocess, 3: param
			PA_long32 fTag;
		} fVariableDefinition;

		char fFiller[258];	// to ensure correct struct size
	} uValue;
} PA_Variable;


// --------------------------------------------------------------------------------
// strings
// --------------------------------------------------------------------------------

typedef enum
{
	eSK_CString = 0,
	eSK_PString
} PA_StringKind;

typedef enum
{
	eCS_Macintosh = 0,
	eCS_Ansi,
} PA_CharSet;

typedef enum PA_4DCharSet
{
	eVTC_UNKNOWN = 0,
	eVTC_UTF_16_BIGENDIAN = 1,
	eVTC_UTF_16_SMALLENDIAN = 2,
	
#if PA_BIGENDIAN
	eVTC_UTF_16 = eVTC_UTF_16_BIGENDIAN,
	eVTC_UTF_16_ByteSwapped = eVTC_UTF_16_SMALLENDIAN,
#else
	eVTC_UTF_16 = eVTC_UTF_16_SMALLENDIAN,
	eVTC_UTF_16_ByteSwapped = eVTC_UTF_16_BIGENDIAN,
#endif
	
	eVTC_UTF_32_BIGENDIAN = 3,
	eVTC_UTF_32_SMALLENDIAN = 4,

#if PA_BIGENDIAN
	eVTC_UTF_32 = eVTC_UTF_32_BIGENDIAN,
#else
	eVTC_UTF_32 = eVTC_UTF_32_SMALLENDIAN,
#endif

	eVTC_UTF_32_RAW_BIGENDIAN = 5,
	eVTC_UTF_32_RAW_SMALLENDIAN = 6,

#if PA_BIGENDIAN
	eVTC_UTF_32_RAW = eVTC_UTF_32_RAW_BIGENDIAN,
	eVTC_UTF_32_RAW_ByteSwapped = eVTC_UTF_32_RAW_SMALLENDIAN,
#else
	eVTC_UTF_32_RAW = eVTC_UTF_32_RAW_SMALLENDIAN,
	eVTC_UTF_32_RAW_ByteSwapped = eVTC_UTF_32_RAW_BIGENDIAN,
#endif

#if VERSIONMAC
	eVTC_WCHAR_T = eVTC_UTF_32,
#elif VERSIONWIN
	eVTC_WCHAR_T = eVTC_UTF_16,
#endif

	eVTC_UTF_8 = 7,
	eVTC_UTF_7 = 8,

	// ASCII (7 bits)
	eVTC_US_ASCII = 9,
	eVTC_US_EBCDIC = 10,

	eVTC_IBM437 = 11,
	
	// eVTC_MacOSAnsi & eVTC_Win32Ansi are the charsets used by non-unicode system apis.
#if VERSIONMAC
	eVTC_MacOSAnsi = -2,
#elif VERSIONWIN
	eVTC_Win32Ansi = -2,
#endif

	// the charset used by compilers for char* constants
#if VERSIONMAC
	eVTC_StdLib_char = eVTC_UTF_8,
#elif VERSIONWIN
	eVTC_StdLib_char = eVTC_Win32Ansi,
#endif
	
	// Platform specific
	eVTC_MAC_ROMAN	= 100,
	eVTC_WIN_ROMAN,
	eVTC_MAC_CENTRALEUROPE,
	eVTC_WIN_CENTRALEUROPE,
	eVTC_MAC_CYRILLIC,
	eVTC_WIN_CYRILLIC,
	eVTC_MAC_GREEK,
	eVTC_WIN_GREEK,
	eVTC_MAC_TURKISH,
	eVTC_WIN_TURKISH,
	eVTC_MAC_ARABIC,
	eVTC_WIN_ARABIC,
	eVTC_MAC_HEBREW,
	eVTC_WIN_HEBREW,
	eVTC_MAC_BALTIC,
	eVTC_WIN_BALTIC,
	eVTC_MAC_CHINESE_SIMP,
	eVTC_WIN_CHINESE_SIMP,
	eVTC_MAC_CHINESE_TRAD,
	eVTC_WIN_CHINESE_TRAD,
	// Internet set
	eVTC_SHIFT_JIS	= 1000,	// Japan - Shift-JIS (Mac et Windows)
	eVTC_JIS,			// Japan - JIS ou ISO-2022-JP (pour les e-mails)
	eVTC_BIG5,			// Chinese (Traditional)
	eVTC_EUC_KR,			// Corean
	eVTC_KOI8R,			// Cyrillic
	eVTC_ISO_8859_1,		// West Europe
	eVTC_ISO_8859_2,		// Central Europe CP1250
	eVTC_ISO_8859_3,		// 
	eVTC_ISO_8859_4,		// Baltic
	eVTC_ISO_8859_5,		// Cyrillic
	eVTC_ISO_8859_6,		// Arab
	eVTC_ISO_8859_7,		// Greek
	eVTC_ISO_8859_8,		// Hebrew
	eVTC_ISO_8859_9,		// Turkish
	eVTC_ISO_8859_10,	// Nordic + Baltic (not available under Windows)
	eVTC_ISO_8859_13,	// Baltic countries (not available under Windows)
	eVTC_GB2312,			// Chinese (simplified)
	eVTC_GB2312_80,		// Chinese (simplified)
	eVTC_ISO_8859_15,	// ISO-Latin-9

	eVTC_LastCharset,

#if VERSIONMAC
	eVTC_ODBC_DEFAULT = -2,
#elif VERSIONWIN
	eVTC_ODBC_DEFAULT = -2,
#endif

} PA_4DCharSet;


// --------------------------------------------------------------------------------
// window levels
// --------------------------------------------------------------------------------

typedef enum
{
	eWL_Window      = 1,
	eWL_Palette     = 2,
	eWL_Dialog      = 5,
	eWL_OnEvent     = 6,
	eWL_Toolbar     = 7,
	eWL_Combo       = 8,
	eWL_Tip         = 9,
	eWL_SuperDialog = 10
} PA_WindowLevel;


// --------------------------------------------------------------------------------
// platform interface
// --------------------------------------------------------------------------------

typedef enum
{
	ePI_Automatic	= -1,	// Show layouts according to platform GUI
	ePI_Macintosh	= 0,	// Show layouts using Macintosh GUI	
	ePI_Win31		= 1,	// Show layouts using Windows 3.1.x GUI
	ePI_Win95		= 2,	// Show layouts using Windows 95 GUI
	ePI_MacTheme	= 3		// Show layouts using Macintosh Appearance Manager
} PA_PlatformInterface;


// --------------------------------------------------------------------------------
// Web publishing picture kinds
// --------------------------------------------------------------------------------

typedef enum
{
	eWP_MacintoshPicture = 0,
	eWP_GIF,
	eWP_JPEG
} PA_WebPictureKind;


// --------------------------------------------------------------------------------
// 4D Application field types
// --------------------------------------------------------------------------------

typedef enum
{
  eFK_InvalidFieldKind	= -1,
  eFK_AlphaField		= 0,	//  Alphanumeric field (from 2 to 80 characters)
  eFK_RealField			= 1,	//  Numeric field (Double or Extended value)
  eFK_TextField			= 2,	//  Text field (up to 32000 characters)
  eFK_PictureField		= 3,	//  Picture field (virtually any block of data)
  eFK_DateField			= 4,	//  Date field 
  eFK_BooleanField		= 6,	//  Boolean field
  eFK_SubfileField		= 7,	//  Subfile field
  eFK_IntegerField		= 8,	//  Integer field (-32768..32767)
  eFK_LongintField		= 9,	//  Long Integer field (-2^31..(2^31)-1)
  eFK_TimeField			= 11,	//	Time field
  eFK_Long8				= 25,	
  eFK_BlobField			= 30,	//	Blob field
  eFK_FloatField		= 35,	//  Float
  eFK_ObjectField		= 38	//  Object field
} PA_FieldKind;


// --------------------------------------------------------------------------------
// 4D Application field attributes
// --------------------------------------------------------------------------------

typedef struct PA_MethodFlags
{
	unsigned fInvisible : 1;	// Invisible
	unsigned f4DAction : 1;	// Available through 4DACTION, 4DMETHOD and 4DSCRIPT
	unsigned fSoap : 1;		// Offered as a Web Service
	unsigned fWsdl : 1;		// Published in WSDL
	unsigned fShared : 1;		// Shared by components and host database
	unsigned fSQL : 1;			// Available through SQL
	unsigned fUnused : 2;
} PA_MethodFlags;



// --------------------------------------------------------------------------------
// 4D Application expression and variable types
// --------------------------------------------------------------------------------

typedef enum
{														
	eVK_Real			= 1,	// Variable declared using C_REAL
	eVK_Text			= 2,
	eVK_Date			= 4,	// Variable declared using C_DATE
	eVK_Undefined		= 5,	// Undefined variable
	eVK_Boolean			= 6,	// variable declared using C_BOOLEAN
	eVK_Integer			= 8,	// variable declared using C_INTEGER
	eVK_Longint			= 9,	// Variable declared using C_LONGINT
	eVK_Picture			= 10,	// Variable declared using C_PICTURE
	eVK_Time			= 11,	// Variable declared using C_TIME
	eVK_ArrayOfArray	= 13,	// Any two-dimensional array
	eVK_ArrayReal		= 14,	// One dimension array declared using ARRAY REAL
	eVK_ArrayInteger	= 15,	// One dimension array declared using ARRAY INTEGER
	eVK_ArrayLongint	= 16,	// One dimension array declared using ARRAY LONGINT
	eVK_ArrayDate		= 17,	// One dimension array declared using ARRAY DATE
	eVK_ArrayText		= 18,
	eVK_ArrayPicture	= 19,	// One dimension array declared using ARRAY PICTURE
	eVK_ArrayPointer	= 20,	// One dimension array declared using ARRAY POINTER
	eVK_ArrayBoolean	= 22,	// One dimension array declared using ARRAY BOOLEAN
	eVK_Pointer			= 23,	// Variable declared using C_POINTER
	eVK_Blob			= 30,	// Variable declared using C_BLOB
	eVK_ArrayBlob		= 31,	// One dimension array declared using ARRAY BLOB
	eVK_ArrayTime		= 32,	// One dimension array declared using ARRAY TIME
	eVK_Unistring		= 33,	// Variable declared using C_STRING or C_TEXT
	eVK_ArrayUnicode	= 34,	// One Dimension array declared using ARRAY STRING or ARRAY TEXT
	eVK_Object			= 38,	// Variable declared using C_OBJECT
	eVK_ArrayObject		= 39,	// Variable declared using ARRAY OBJECT
	eVK_Collection		= 42,	// Variable declared using C_COLLECTION
	eVK_Null			= 255	// Null type value
} PA_VariableKind;


// --------------------------------------------------------------------------------
// 4D Application query and sort operators
// --------------------------------------------------------------------------------

// Values for the field fOperator of LineBlock data structure
// These values define the logical connection between the lines of a query definition
typedef enum
{
   eQO_NoOperator = 0,	// Always 1st line of a query definition
   eQO_LogicalAND,		// AND     (& operator)
   eQO_LogicalOR,		// OR      (| operator)
   eQO_Except			// EXCEPT  (# operator)
} PA_QueryOperator;

// Values for the field fComparison of LineBlock data structure
// These values define the comparison operator for the line of query definition	
typedef enum
{
   eQC_NoOperation = 0,			// No query line can have this value
   eQC_IsEqual,				// Is equal to
   eQC_IsDifferent,			// Is not equal to
   eQC_IsGreater,				// Is greater than
   eQC_IsGreaterOrEqual,		// Is greater or equal to
   eQC_IsLess,					// Is less than
   eQC_IsLessOrEqual,			// Is less or equal to
   eQC_Contains,				// Contains
   eQC_NotContains				// Does not contains
} PA_QueryComparison;


// --------------------------------------------------------------------------------
// 4D Application errors codes
// --------------------------------------------------------------------------------

typedef enum
{
  // 4D database engine errors codes
  eER_IdxPgOutOfRange					= -10004,	// Index page is out of range
  eER_RecOutOfRange						= -10003,	// Record is out of range (*)
  eER_InvalidRecStruct					= -10002,	// Invalid record structure (*)
													// ( data file needs to be repaired )
  eER_InvalidIdxPage					= -10001,	// Invalid index page (*)
													// ( index needs to be repaired or rebuilt )
													// (*) if 4D Client, maybe due to error
													// of connection reported by network component.
  eER_BadRecordAddress					= -10000,	// Bad record address
  eER_NoMoreSpaceToSave					= -9999,	// No more space available to save the record
  eER_DuplicateKeyIndex					= -9998,	// Duplicated index key
  eER_MaxNumberOfRecords				= -9997,	// Maximum number of records has been reached
  eER_StackIsFull						= -9996,	// Stack is full
  eER_DemoVersionLimit					= -9995,	// Limits of demo version have been reached
  eER_CommInterruptedByUser				= -9994,	// Serial communication interrupted by user
  eER_NoEnoughPrivilege					= -9991,	// Access privileges error
  eER_TimeOut							= -9990,	// Time-out error
  eER_InvalidStruture					= -9989,	// Invalid structure
													// ( database needs to be repaired )
  eER_RelatedRecords					= -9987,	// Other records are related to this record
  eER_RecordLockedDuringDel				= -9986,	// Record(s) locked during an automatic deletion action
  eER_RecursiveIntegrity				= -9985,	// Recursive integrity
  eER_DuplicatedKeyInTrans				= -9984,	// Transaction cancelled because of a dup. index key 
  eER_RecordNotInSelection				= -9982,	// Record not in selection and could not be loaded
  eER_BadFileFieldTable					= -9981,	// Invalid Field/File definition table sent
  eER_StructureIsLocked					= -9980,	// File cannot be created because structure is locked
  eER_BadUserName						= -9979,	// Unknown User
  eER_BadPassword						= -9978,	// Invalid Password
  eER_BadSelection						= -9977,	// The selection does not exist
  eER_BackupInProgress					= -9976,	// Backup in progress, no modifications are allowed
  eER_CannotLoadTrIdxPage				= -9975,	// Transaction index page could not be loaded
  eER_RecordAlreadyDeleted				= -9974,	// The record has already been deleted
  eER_BadTRIC							= -9973,	// TRIC resources are not the same
  eER_BadFileNumber						= -9972,	// File number is out of range
  eER_BadFieldNumber					= -9971,	// Field number is out of range
  eER_FieldNotIndexed					= -9970,	// Field is not indexed
  eER_BadFieldType						= -9969,	// Invalid field type requested
  eER_BadSelectedRecordNumber			= -9968,	// Invalid selected record number requested
  eER_RecordNotLoaded					= -9967,	// The record was not modified because
													// it could not be loaded
  eER_IncompatibleTypes					= -9966,	// Invalid type requested
  eER_BadSearchDefinition				= -9965,	// Bad search definition table sent
  eER_BadSortDefinition					= -9964,	// Bad sort definition table sent
  eER_BadRecordNumber					= -9963,	// Invalid record number requested
  eER_BackupCannotLaunched				= -9962,	// The backup cannot be launched because
													// the server is shutting down
  eER_BackupNotRunning					= -9961,	// The backup process is currently not running
  eER_BackupNotInstalled				= -9960,	// 4D Backup is not installed on the server
  eER_BackupAlreadyRunning				= -9959,	// the backup process has already been started
													// by another user or process
  eER_ProcessNotStarted					= -9958,	// Process could not be started
  eER_ListIsLocked						= -9957,	// The list is locked
  eER_QTNotInstalled					= -9955,	// QuickTime is not installed
  eER_SNoCurrentRecord					= -9954,	// There is no current record
  eER_NoLogFile							= -9953,	// There is no Log file
  eER_BadDataSegmentHeader				= -9952,	// Invalid data segment
  eER_NotRelatedField					= -9951,	// Field is not related to another one
  eER_BadDataSegmentNumber				= -9950,	// Invalid data segment number
  eER_NamedSelNotExist					= -9946,	// Unable to clear the named selection because 
													// it does not exist
  eER_CDROMvers							= -9945,	// CD-ROM 4D Runtime error, 
													// writing operations are not allowed
  eER_Passportvers						= -9943,	// 4D Passport version error
  eER_LicensingError					= -9942,	// 4D Client Licensing scheme is incompatible 
													// with this version of 4D Server
  eER_UnknownGestaltSelector			= -9941,	// Unknown EX_GESTALT selector
  eER_ExtInitFailed						= -9940,	// 4D Extension initialization failed
  eER_ExtNotFound						= -9939,	// External routine not found
  eER_NotEnoughMemory					= -108,		// Not enough Memory

  eER_NoErr								= 0,

  // Additional entry points errors codes
  eER_CallNotImplemented				= -1,		// Entry point is not implemented
  eER_InvalidFileNumber					= 4001,		// Invalid file number
  eER_InvalidRecordNumber				= 4002,		// Invalid record number
  eER_InvalidFieldNumber				= 4003,		// Ivvalid field number
  eER_NoCurrentRecord					= 4004,		// There is no current record
  eER_BadEventCall						= 4005, 
  eER_InvalidOrderByRef					= 4006,
  eER_InvalidQueryRef					= 4007,
  eER_TooManyOrderBy					= 4008,
  eER_TooManyQueries					= 4009,
  eER_InvalidCreateTableRef				= 4010,
  eER_TooManyFields						= 4011,
  eER_NotAVariablePointer				= 4012,
  eER_InvalidMethodName 				= 4013,
  eER_CouldNotCreateMethod 				= 4014,
  eER_CouldNotUpdateMethod 				= 4015


} PA_ErrorCode;

typedef enum
{
	eFK_Active4DFolder = 0,							//	Active 4D Folder
	eFK_LicencesFolder,								//	Licenses Folder
	eFK_ExtrasFolder,								//	Extras Folder
	eFK_4DClientDatabaseFolder						//	4D Client Database Folder

} PA_FolderKind;

// reset struct alignment
#pragma pack(pop)

#endif
