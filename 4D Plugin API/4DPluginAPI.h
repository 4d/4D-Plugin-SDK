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
// File : 4DPluginAPI.h
//
// rev : v13
//
// ---------------------------------------------------------------

#ifndef __4DPLUGINAPI__
#define __4DPLUGINAPI__

#include "Flags.h"
#include "PublicTypes.h"
#ifndef NULL
#define NULL 0
#endif
#ifdef __cplusplus
extern "C" {
#endif



// ---------------------------------------------------------------
// interface that must be provided by user
// ---------------------------------------------------------------
void PluginMain( PA_long32 selector, PA_PluginParameters params );


// ---------------------------------------------------------------
// Returns the last error returned by any call to the API
// ---------------------------------------------------------------

PA_ErrorCode PA_GetLastError();


// ---------------------------------------------------------------
// After a call to PA_UseVirtualStructure(), all pending calls to
// the API will use the virtual structure references if supported.
// Use PA_UseRealStructure() to switch back to real structure.
// By default, real structure is used. 
// ---------------------------------------------------------------

// reads or set 4D Plugin API internal flag
void PA_UseVirtualStructure      ();
void PA_UseRealStructure         ();

// asks 4D to know if a virtual structure is defined
char  PA_VirtualStructureDefined ( );
short PA_GetTrueTableNumber      ( short virtualTable );
void  PA_GetTrueFieldNumber      ( short virtualTable, short virtualField, short* trueTable, short* trueField );
short PA_GetVirtualTableNumber   ( short trueTable );
void  PA_GetVirtualFieldNumber   ( short trueTable, short trueField, short* virtualTable, short* virtualField );


// ---------------------------------------------------------------
// These functions are used to manage 4D Application handle.
// they are used internally by 4D Plugin API.
// On Macintosh, you can use these handles as regular Mac handles.
// ---------------------------------------------------------------

PA_Handle	PA_NewHandle		( PA_long32 len );
void		PA_DisposeHandle	( PA_Handle handle );
PA_long32		PA_GetHandleSize	( PA_Handle handle );
char		PA_SetHandleSize	( PA_Handle handle, PA_long32 newlen );
char*		PA_LockHandle		( PA_Handle handle );
void		PA_UnlockHandle		( PA_Handle handle );
void		PA_MoveBlock		( void *source, void *dest, PA_long32 len );
char		PA_GetHandleState	( PA_Handle handle );
void		PA_SetHandleState	( PA_Handle handle, char state );

// ---------------------------------------------------------------
// strings manipulations 
// ---------------------------------------------------------------

PA_Unichar*		PA_GetUnistring( PA_Unistring* ustr );
PA_long32			PA_GetUnistringLength( PA_Unistring* ustr );
PA_Unistring	PA_CreateUnistring( PA_Unichar* ustr );
void			PA_DisposeUnistring( PA_Unistring* ustr );
void			PA_SetUnistring( PA_Unistring* ustr, PA_Unichar* str );
char			PA_CompareUniBuffers( PA_Unichar* text1, PA_long32 len1, PA_Unichar* text2, PA_long32 len2, char diacritic, char useWildChar );
char			PA_CompareUnichars( PA_Unichar* string1, PA_Unichar* string2, char diacritic, char useWildChar );
char			PA_CompareUnistrings( PA_Unistring* ustr1, PA_Unistring* ustr2, char diacritic, char useWildChar );
PA_long32			PA_GetUnicharsLength( PA_Unichar* str );
void			PA_CopyUnichars( PA_Unichar* source, PA_Unichar* dest, PA_long32 maxbytesize );


// ---------------------------------------------------------------
// pictures manipulations 
// ---------------------------------------------------------------
//The pictures are manipulated by reference
PA_Picture   PA_CreatePicture( void* buffer, PA_long32 len );
void*        PA_CreateNativePictureForScreen( PA_Picture picture );	// creates a CGImageRef on Mac, and a gdiplus::bitmap* on Windows
void*        PA_CreateNativePictureForPrinting( PA_Picture picture );	// creates a CGPDFDocumentRef on Mac, and a gdiplus::metafile* on Windows
void         PA_DisposePicture( PA_Picture picture );
PA_Unistring PA_GetPictureData( PA_Picture picture, PA_long32 index, PA_Handle handle );
PA_Picture   PA_DuplicatePicture( PA_Picture picture, char retainOnly );


// ---------------------------------------------------------------
// object manipulations | From 17.x 
// ---------------------------------------------------------------
//The objects are manipulated by reference
PA_ObjectRef     PA_CreateObject ( void );
void             PA_DisposeObject ( PA_ObjectRef object );

PA_ObjectRef     PA_DuplicateObject ( PA_ObjectRef object );
PA_Variable  	 PA_GetObjectProperty		(PA_ObjectRef object, PA_Unistring* key);
void		 	 PA_SetObjectProperty		(PA_ObjectRef object, PA_Unistring* key, PA_Variable value);
void             PA_RemoveObjectProperty( PA_ObjectRef object, PA_Unistring* key );
char             PA_HasObjectProperty( PA_ObjectRef object, PA_Unistring* key );
PA_VariableKind  PA_GetObjectPropertyType( PA_ObjectRef object, PA_Unistring* key );
PA_Variable      PA_JsonParse( PA_Unistring* json, PA_VariableKind kind );
PA_Unistring     PA_JsonStringify( PA_Variable value, char prettyPrint );

// ---------------------------------------------------------------
// Collection manipulations| From 17.x 
// ---------------------------------------------------------------
//The collections are manipulated by reference
PA_CollectionRef PA_CreateCollection(void);
PA_Variable  	 PA_GetCollectionElement(PA_CollectionRef collection, long index);
void		 	 PA_SetCollectionElement(PA_CollectionRef collection, long index, PA_Variable value);
void             PA_DisposeCollection(PA_CollectionRef collection);
PA_long32		 PA_GetCollectionLength(PA_CollectionRef collection);


// ---------------------------------------------------------------
// these commands returns or set informations 
// on 4D Application
// ---------------------------------------------------------------

PA_ulong32 PA_Get4DVersion ();
char PA_IsDemoVersion         ();
char PA_IsDatabaseLocked      ();	// may happen when running on CD-ROM
char PA_IsCompiled            ( char hostDatabase);	// pass hostDatabase to 0 to ask about calling database, 1 to ask about host database.

char PA_Is4DClient ();
char PA_Is4DServer ();
char PA_Is4DMono   ();

char PA_IsWebProcess ();

char PA_GetMessagesStatus  ( );
void PA_SetMessagesStatus  ( char showMessages );
void PA_SetThermometerRect ( PA_Rect rect );

void PA_GetStructureName   ( PA_Unichar* structName );
void PA_GetDataName        ( PA_Unichar* dataName );

PA_Unistring PA_GetApplicationFullPath();
PA_Unistring PA_GetStructureFullPath();

short PA_FindPackage ( short packageID );
void  PA_PackageInfo ( short packageNumber, short* packageID, void** procPtr, void** data, short* resfile, PA_Unichar* name );

void  PA_Quit4D();

PA_Unistring	PA_Get4DPreferencesFilePath ();
PA_Unistring	PA_Get4Dfolder ( PA_FolderKind folderKind, char createIfNotFound );

PA_PlatformInterface PA_GetPlatformInterface();

void PA_GetToolBarInfo  ( char* displayed, short* toolbarHeight );
void PA_ShowHideToolBar ( char displayed );

char PA_GetTipsEnabled  ( );
void PA_SetTipsEnabled  ( char enabled );

PA_long32 PA_CheckFreeStack  ( PA_long32 requestedStack );

void* PA_Get4DHInstance();

void PA_GetCenturyInfo( PA_long32* pivotYear, PA_long32* defaultCentury );

// ---------------------------------------------------------------
// those commands can format strings, numbers, date and time
// as the String command in 4D. FormatNumber are the same as
// in 4D for formating date and times
// (see String command documentation)
// ---------------------------------------------------------------

void PA_FormatString   ( PA_Unichar* string, PA_Unichar* format, PA_Unichar* result );
void PA_FormatReal     ( double value, PA_Unichar* format, PA_Unichar* result );
void PA_FormatLongint  ( PA_long32 value,   PA_Unichar* format, PA_Unichar* result );
void PA_FormatDate     ( short day, short month, short year, short formatNumber, PA_Unichar* result );
void PA_FormatTime     ( PA_long32 time, short formatNumber, PA_Unichar* result );


PA_long32   PA_EvalLongint  ( PA_Unichar* string );
double PA_EvalReal     ( PA_Unichar* string );
void   PA_EvalDate     ( PA_Unichar* string, short* day, short* month, short* year );
PA_long32   PA_EvalTime     ( PA_Unichar* string );

void PA_ConvertStrings ( char* string1, PA_StringKind kind1, PA_CharSet charset1, 
					     char* string2, PA_StringKind kind2, PA_CharSet charset2 );

PA_long32 PA_ConvertCharsetToCharset ( char* fromString, PA_long32 fromSize, PA_4DCharSet fromCharset,
								 char* toString, PA_long32 toSize, PA_4DCharSet toCharset);

// ---------------------------------------------------------------
// some functions needs to know if you want automatic relations to be activated.
// Default is both automatic relations On.
// calling this function tells explicitly 4D if you want automatic relations activated or not.
// ---------------------------------------------------------------

void PA_UseAutomaticRelations ( char manyToOne, char oneToMany );

// ---------------------------------------------------------------
// inforation about import/export filters.
// ---------------------------------------------------------------

char* PA_GetExportFilter ();
char* PA_GetImportFilter ();

// ---------------------------------------------------------------
// Selection
// Warning : these functions do not use virtual structure.
// they can use automatic relations depending 
// of the use of PA_UseAutomaticRelations().
// ---------------------------------------------------------------

PA_long32 PA_SelectedRecordNumber ( short table );
void PA_GotoSelectedRecord   ( short table, PA_long32 record );
PA_long32 PA_RecordsInSelection   ( short table );
void PA_DeleteSelection      ( short table );
void PA_FirstRecord          ( short table );
void PA_NextRecord           ( short table );
void PA_PreviousRecord       ( short table );
char PA_BeforeSelection      ( short table );
char PA_EndSelection         ( short table );

// ---------------------------------------------------------------
// Tables, fields, indexes & relations
// This command allows you to create tables, fields, indexes and/or
// relations using an xml definition.
// ---------------------------------------------------------------

void PA_CreateElementsFromXMLDefinition( PA_Unistring* xmlDefinition);

// ---------------------------------------------------------------
// Records
// ---------------------------------------------------------------

void PA_CreateRecord ( short table );
void PA_SaveRecord   ( short table );
void PA_GotoRecord   ( short table, PA_long32 record );
PA_long32 PA_RecordNumber ( short table );


// ---------------------------------------------------------------
// Relations
// ---------------------------------------------------------------

void PA_RelateOne  ( short table );
void PA_RelateMany ( short table );
void PA_RelateOneSelection  ( short manyTable, short oneTable  );
void PA_RelateManySelection ( short manyTable, short manyField );


// ---------------------------------------------------------------
// Records locking
// ---------------------------------------------------------------

char PA_Locked     ( short table );
void PA_ReadWrite  ( short table );
void PA_ReadOnly   ( short table );
void PA_LoadRecord ( short table );


// ---------------------------------------------------------------
// Named selections
// ---------------------------------------------------------------

void PA_CopyNamedSelection  ( short table, PA_Unichar* name );
void PA_CutNamedSelection   ( short table, PA_Unichar* name );
void PA_UseNamedSelection   ( PA_Unichar* name );
void PA_ClearNamedSelection ( PA_Unichar* name );


// ---------------------------------------------------------------
// Sets
// ---------------------------------------------------------------

void PA_CreateEmptySet ( short table, PA_Unichar* name );
void PA_CreateSet	   ( short table, PA_Unichar* name );
void PA_AddToSet       ( short table, PA_Unichar* name );
void PA_UseSet         ( PA_Unichar* name );
void PA_DeleteSet      ( PA_Unichar* name );


// ---------------------------------------------------------------
// database informations
// ---------------------------------------------------------------

// ---- tables
short PA_CountTables    ( );
void  PA_GetTableName   ( short table, PA_Unichar* tableName );
char  PA_IsTableVisible ( short table );

//	tableAndFieldNames should be as "[Table1]field1"
void  PA_GetTableAndFieldNumbers( PA_Unichar* tableAndFieldNames, short* table, short* field );

// ---- fields
short PA_CountFields        ( short table);
void  PA_GetFieldName       ( short table, short field, PA_Unichar* fieldName );
void  PA_GetFieldProperties ( short table, short field, PA_FieldKind* kind,
                              short* stringlength, char* indexed, PA_long32* attributes );
void  PA_GetFieldRelation   ( short table, short field, 
                              short* relatedTable, short* relatedField );
void  PA_GetFieldList       ( short table, short field, PA_Unichar* listName );

void PA_TableAndFieldPopup  ( short x, short y, short* table, short* field );
void PA_FieldPopup          ( short x, short y, short table, short* field );


// ---------------------------------------------------------------
// Order by
// ---------------------------------------------------------------

// easy function used to order selection using only one field, located in the table
void PA_OrderBy ( short table, short field, char ascending );

// set of functions used to order selection using multiple fields
PA_OrderByRef PA_OpenOrderBy ( short table );
void PA_AddFieldToOrderBy ( PA_OrderByRef orderby, short table, short field, char ascending );
void PA_CloseOrderBy      ( PA_OrderByRef orderby );



// ---------------------------------------------------------------
// Query 
// ---------------------------------------------------------------

// set of functions used to query in selection using multiple criterias

PA_QueryRef PA_OpenQuery ( short table );
void PA_QueryString   ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, PA_Unichar* string );
void PA_QueryReal     ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, double value );
void PA_QueryTime     ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, PA_long32 value );
void PA_QueryDate     ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short day, short month, short year );
void PA_QueryLongint  ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, PA_long32 value );
void PA_QueryInteger  ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, short value );
void PA_QueryBoolean  ( PA_QueryRef query, short table, short field, PA_QueryOperator qo, PA_QueryComparison qc, char value );
void PA_CloseQuery    ( PA_QueryRef query );



// ---------------------------------------------------------------
// Get fields from database
// ---------------------------------------------------------------

PA_Unistring PA_GetStringField     ( short table, short field );
PA_long32         PA_GetBlobField       ( short table, short field, void* blob );
PA_Handle    PA_GetBlobHandleField ( short table, short field );
PA_Picture   PA_GetPictureField    ( short table, short field );
double       PA_GetRealField       ( short table, short field );
PA_long32         PA_GetLongintField    ( short table, short field );
short        PA_GetIntegerField    ( short table, short field );
PA_long32         PA_GetTimeField       ( short table, short field );
void         PA_GetDateField       ( short table, short field, short* day, short* month, short* year );
char         PA_GetBooleanField    ( short table, short field );
PA_ObjectRef PA_GetObjectField     ( short table, short field );

// ---------------------------------------------------------------
// Set fields in database
// ---------------------------------------------------------------

void PA_SetStringField     ( short table, short field, PA_Unistring* ustr );
void PA_SetBlobField       ( short table, short field, void* blob, PA_long32 len );
void PA_SetBlobHandleField ( short table, short field, PA_Handle hblob );
void PA_SetPictureField    ( short table, short field, PA_Picture picture );
void PA_SetRealField       ( short table, short field, double value );
void PA_SetLongintField    ( short table, short field, PA_long32  value );
void PA_SetIntegerField    ( short table, short field, short value );
void PA_SetTimeField       ( short table, short field, PA_long32  value );
void PA_SetDateField       ( short table, short field, short day, short month, short year );
void PA_SetBooleanField    ( short table, short field, char value );
void PA_SetObjectField     ( short table, short field, PA_ObjectRef object );



// -----------------------------------------
// 4D Application Packed Records
// -----------------------------------------

PA_long32 PA_GetPackedRecord ( short table, void* buffer );
void PA_SetPackedRecord ( short table, void* buffer, PA_long32 len );



// -----------------------------------------------------
// Read parameters when 4D Application calls
// a plugin command
// NOTE: the first parameter starts at index 1
// -----------------------------------------------------

short			PA_GetShortParameter         ( PA_PluginParameters params, short index );
PA_long32			PA_GetLongParameter          ( PA_PluginParameters params, short index );
double			PA_GetDoubleParameter        ( PA_PluginParameters params, short index );
PA_Unistring*	PA_GetStringParameter        ( PA_PluginParameters params, short index );
PA_long32			PA_GetBlobParameter          ( PA_PluginParameters params, short index, void* blob );
PA_Handle		PA_GetBlobHandleParameter    ( PA_PluginParameters params, short index );
PA_Picture		PA_GetPictureParameter       ( PA_PluginParameters params, short index );
void			PA_GetDateParameter          ( PA_PluginParameters params, short index, short* day, short* month, short* year );
PA_long32		PA_GetTimeParameter          ( PA_PluginParameters params, short index );
PA_Variable		PA_GetVariableParameter      ( PA_PluginParameters params, short index );
PA_ObjectRef    PA_GetObjectParameter(PA_PluginParameters params, short index);
PA_CollectionRef PA_GetCollectionParameter(PA_PluginParameters params, short index);


PA_Pointer		PA_GetPointerParameter       ( PA_PluginParameters params, short index );
PA_Variable		PA_GetPointerValue           ( PA_Pointer pointer ); // Call PA_ClearVariable to clear the returned PA_Variable.
PA_PointerKind	PA_GetPointerKind			 ( PA_Pointer pointer );
void			PA_GetPointerValueProperties ( PA_Pointer inPointer, PA_VariableKind* outKind, PA_long32* outStringLength, PA_long32* outArraySize);
void			PA_SetPointerValue           ( PA_Pointer pointer, PA_Variable variable );


// -----------------------------------------
// Return value when 4D Application calls
// a plugin command
// -----------------------------------------

void PA_ReturnShort    ( PA_PluginParameters params, short value );
void PA_ReturnLong     ( PA_PluginParameters params, PA_long32 value );
void PA_ReturnDouble   ( PA_PluginParameters params, double value );
void PA_ReturnString   ( PA_PluginParameters params, PA_Unichar* string );
void PA_ReturnBlob     ( PA_PluginParameters params, void* blob, PA_long32 len );
void PA_ReturnPicture  ( PA_PluginParameters params, PA_Picture picture );
void PA_ReturnDate     ( PA_PluginParameters params, short day, short month, short year );
void PA_ReturnTime     ( PA_PluginParameters params, PA_long32 value );
void PA_ReturnObject		( PA_PluginParameters params, PA_ObjectRef object);
void PA_ReturnCollection	( PA_PluginParameters params, PA_CollectionRef object);
void PA_ReturnBlobHandle    ( PA_PluginParameters params, PA_Handle hblob );


// -----------------------------------------------------
// Set parameters when 4D Application calls
// a plugin command
// NOTE: the first parameter starts at index 1
// -----------------------------------------------------

void PA_SetShortParameter	   ( PA_PluginParameters params, short index, short value );
void PA_SetLongParameter	   ( PA_PluginParameters params, short index, PA_long32 value );
void PA_SetDoubleParameter     ( PA_PluginParameters params, short index, double value );
void PA_SetBlobParameter	   ( PA_PluginParameters params, short index, void* blob, PA_long32 len );
void PA_SetBlobHandleParameter ( PA_PluginParameters params, short index, PA_Handle hblob );
void PA_SetPictureParameter	   ( PA_PluginParameters params, short index, PA_Picture picture );
void PA_SetDateParameter	   ( PA_PluginParameters params, short index, short day, short month, short year );
void PA_SetTimeParameter	   ( PA_PluginParameters params, short index, PA_long32 value );
void PA_SetVariableParameter   ( PA_PluginParameters params, short index, PA_Variable variable, char clearOldValue );


// -----------------------------------------------------
// manage events on a plugin area
// -----------------------------------------------------

PA_AreaEvent	PA_GetAreaEvent   ( PA_PluginParameters params );
PA_Rect			PA_GetAreaRect    ( PA_PluginParameters params );
PA_Unistring*   PA_GetAreaName    ( PA_PluginParameters params );
PA_Unistring*	PA_GetAreaObjectName(PA_PluginParameters params );
void    PA_GetPluginProperties    ( PA_PluginParameters params, PA_PluginProperties* properties );
// If PA_GetPluginProperties needed please call it first.
void    PA_SetAreaReference       ( PA_PluginParameters params, void* ref );
void*   PA_GetAreaReference       ( PA_PluginParameters params );
void    PA_SetAreaFocusable       ( PA_PluginParameters params, char focusable );
char    PA_IsActivated            ( PA_PluginParameters params );
char    PA_IsDeActivated          ( PA_PluginParameters params );
void    PA_AcceptSelect           ( PA_PluginParameters params, char accept );
void    PA_AcceptDeselect         ( PA_PluginParameters params, char accept );
void    PA_GetClick               ( PA_PluginParameters params, short* x, short* y );
short	PA_GetMouseWheelIncrement ( PA_PluginParameters params );
char    PA_GetKey                 ( PA_PluginParameters params, PA_Unichar* unichar, PA_KeyCode* keycode, char* altKey, char* ctrlKey, char* shiftKey );
void    PA_CustomizeDesignMode    ( PA_PluginParameters params );
void    PA_DontTakeEvent          ( PA_PluginParameters params );
void    PA_CallPluginAreaMethod   ( PA_PluginParameters params );
void    PA_SetMenuIcon            ( PA_PluginParameters params, short id );
PA_HDC	PA_GetHDC				  (	PA_PluginParameters params);
void	PA_PublishWebPicture      ( PA_PluginParameters params, char pictureMap );
void	PA_SendHTML               ( PA_PluginParameters params, void* webData, char* HTMLbuffer, PA_long32 len );
void	PA_SendWebPicture         ( PA_PluginParameters params, void* webData, void* picture, PA_long32 len, PA_WebPictureKind kind );
void*	PA_GetWebDataToDispose    ( PA_PluginParameters params );
void	PA_GotoNextField          ( PA_PluginParameters params );
void	PA_GotoPreviousField      ( PA_PluginParameters params );
void	PA_GetPageChange          ( PA_PluginParameters params, short *pageFrom, short *pageTo );
void	PA_RequestRedraw          ( PA_PluginParameters params );
void	PA_RedrawArea	          ( PA_PluginParameters params, char inNow,PA_Rect* inRect);
PA_Rect	PA_GetAreaPortBounds	  ( PA_PluginParameters params);
char	PA_IsAreaVisible			( PA_PluginParameters params );
// ---------------------------------------------------------------
// CGContext tools
// ---------------------------------------------------------------
	
void PA_UseQuartzAxis( PA_PluginParameters params, short *outAreaX, short *outAreaY, short* outAreaWidth, short *outAreaHeight);
void PA_UseQuickdrawAxis( PA_PluginParameters params, short *outAreaX, short *outAreaY, short* outAreaWidth, short *outAreaHeight);


// -----------------------------------------------------
// manage dropping events on a plugin area
// -----------------------------------------------------

PA_DragAndDropInfo	PA_GetDragAndDropInfo		( PA_PluginParameters params );
void				PA_AllowDrop				( PA_PluginParameters params, char allow );
void				PA_GetDragPositions			( PA_PluginParameters params, PA_Rect* rect, short* x, short* y );
void				PA_CustomizeDragOver		( PA_PluginParameters params );
PA_DragContextRef	PA_GetDragAndDropContext	( PA_PluginParameters params );


// -----------------------------------------------------
// Getting informations on a Drag And Drop Info structure
// -----------------------------------------------------

PA_DragKind PA_GetDragAndDropKind     ( PA_DragAndDropInfo info );
PA_Variable PA_GetDragAndDropVariable ( PA_DragAndDropInfo info, PA_long32* indice );
void PA_GetDragAndDropTableField      ( PA_DragAndDropInfo info, short* table, short* field );


// -----------------------------------------------------
// Initiate a drag and drop
// to be called on a eAE_MouseDown event
// -----------------------------------------------------

void PA_DragAndDrop( short startX, short startY, char useCustomRect, PA_Rect customRect );


// -----------------------------------------------------
// manage the drag and drop context
// -----------------------------------------------------
PA_PasteboardRef PA_GetDragAndDropPasteboard ( PA_DragContextRef context );


// -----------------------------------------------------
// manage the pasteboard
// -----------------------------------------------------

char			PA_IsPasteboardDataAvailable	( PA_PasteboardRef pasteboard, PA_Unichar *kind );
PA_Handle		PA_GetPasteboardData			( PA_PasteboardRef pasteboard, PA_Unichar *kind );
PA_ulong32	PA_GetPasteboardDataSize		( PA_PasteboardRef pasteboard, PA_Unichar *kind );
char			PA_SetPasteboardData			( PA_PasteboardRef pasteboard, PA_Unichar *kind, void *data, PA_ulong32 size );


// -----------------------------------------------------
// Advanced Properties of a plugin area
// -----------------------------------------------------

void  PA_SetAdvancedPropertiesEditable  ( PA_PluginParameters params, char editable );
void  PA_SetAdvancedProperties          ( PA_PluginParameters params, void* data, PA_long32 datasize );
void* PA_GetAdvancedPropertiesToDispose ( PA_PluginParameters params );
void* PA_GetAdvancedProperties          ( PA_PluginProperties* properties, PA_long32* datasize );



// ---------------------------------------------------------------
// Get 4D Application variables
// ---------------------------------------------------------------

PA_Variable	PA_CreateVariable          ( PA_VariableKind kind );
PA_Variable	PA_GetVariable             ( PA_Unichar* variableName );
PA_VariableKind PA_GetVariableKind     ( PA_Variable variable );
PA_VariableKind PA_GetPointerValueKind ( PA_Pointer inVarPtr );

PA_Unistring PA_GetStringVariable      ( PA_Variable variable );
PA_long32    PA_GetBlobVariable        ( PA_Variable variable, void* blob );
PA_Handle    PA_GetBlobHandleVariable  ( PA_Variable variable );
PA_Picture   PA_GetPictureVariable     ( PA_Variable variable );
double       PA_GetRealVariable        ( PA_Variable variable );
PA_long32    PA_GetLongintVariable     ( PA_Variable variable );
PA_long32    PA_GetTimeVariable        ( PA_Variable variable );
void         PA_GetDateVariable        ( PA_Variable variable, short* day, short* month, short* year );
char         PA_GetBooleanVariable     ( PA_Variable variable );
PA_ObjectRef PA_GetObjectVariable      ( PA_Variable variable );
PA_CollectionRef PA_GetCollectionVariable	   (PA_Variable variable);
void		 PA_CopyVariable		   (PA_Variable *source, PA_Variable *destination);




// ---------------------------------------------------------------
// Set 4D Application variables
// ---------------------------------------------------------------

void PA_SetVariable ( PA_Unichar* variableName, PA_Variable variable, char clearOldValue );

void PA_SetStringVariable     ( PA_Variable* variable, PA_Unistring* ustr );
void PA_SetBlobVariable       ( PA_Variable* variable, void* blob, PA_long32 len );
void PA_SetBlobHandleVariable ( PA_Variable* variable, PA_Handle hblob );
void PA_SetPictureVariable    ( PA_Variable* variable, PA_Picture picture );
void PA_SetRealVariable       ( PA_Variable* variable, double value );
void PA_SetLongintVariable    ( PA_Variable* variable, PA_long32 value   );
void PA_SetTimeVariable       ( PA_Variable* variable, PA_long32 value   );
void PA_SetDateVariable       ( PA_Variable* variable, short day, short month, short year );
void PA_SetBooleanVariable    ( PA_Variable* variable, char value );
void PA_SetObjectVariable     ( PA_Variable* variable, PA_ObjectRef object );
void PA_SetCollectionVariable (PA_Variable* variable, PA_CollectionRef collection);

// those functions are usefull to pass parameters to 4D Commands, using PA_ExecuteCommandByID
void PA_SetOperationVariable		( PA_Variable* variable, char op );		// op can be '*', '<', '>'...
void PA_SetTableFieldVariable		( PA_Variable* variable, short table, short field );	// pass 0 to table to pass only the field
void PA_SetVariableOrFieldReference ( PA_Variable* outVariable , PA_Pointer inPointer);

// ---------------------------------------------------------------
// CleanUp of a 4D Application variable
// ---------------------------------------------------------------

void PA_ClearVariable( PA_Variable* variable );

// ---------------------------------------------------------------
// 4D Application arrays
// ---------------------------------------------------------------

// general functions on arrays
PA_long32  PA_GetArrayNbElements ( PA_Variable ar );
void  PA_ResizeArray        ( PA_Variable* ar, PA_long32 nb );

PA_long32  PA_GetArrayCurrent    ( PA_Variable ar );
void  PA_SetArrayCurrent    ( PA_Variable* ar, PA_long32 current );

char  PA_IsArrayVariable	( PA_Variable* ar );

// Reading values on arrays
short        PA_GetIntegerInArray ( PA_Variable ar, PA_long32 i );
PA_long32         PA_GetLongintInArray ( PA_Variable ar, PA_long32 i );
double       PA_GetRealInArray    ( PA_Variable ar, PA_long32 i );
void         PA_GetDateInArray    ( PA_Variable ar, PA_long32 i, short* day, short* month, short* year );
PA_long32		 PA_GetTimeInArray	  ( PA_Variable ar, PA_long32 i );
PA_Picture   PA_GetPictureInArray ( PA_Variable ar, PA_long32 i );
PA_Unistring PA_GetStringInArray  ( PA_Variable ar, PA_long32 i );
char         PA_GetBooleanInArray ( PA_Variable ar, PA_long32 i );
PA_Blob      PA_GetBlobInArray    ( PA_Variable ar, PA_long32 i );
PA_Variable  PA_GetArrayInArray   ( PA_Variable ar, PA_long32 i );
PointerBlock PA_GetPointerInArray ( PA_Variable ar, PA_long32 i );
PA_ObjectRef PA_GetObjectInArray  ( PA_Variable ar, PA_long32 i );

// Setting values on arrays
void PA_SetIntegerInArray   ( PA_Variable ar, PA_long32 i, short value );
void PA_SetLongintInArray   ( PA_Variable ar, PA_long32 i, PA_long32 value  );
void PA_SetTimeInArray      ( PA_Variable ar, PA_long32 i, PA_long32 inMilliseconds  );
void PA_SetRealInArray      ( PA_Variable ar, PA_long32 i, double value );
void PA_SetDateInArray      ( PA_Variable ar, PA_long32 i, short day, short month, short year );
void PA_SetStringInArray    ( PA_Variable ar, PA_long32 i, PA_Unistring* ustr );
void PA_SetPictureInArray   ( PA_Variable ar, PA_long32 i, PA_Picture picture );
void PA_SetBooleanInArray   ( PA_Variable ar, PA_long32 i, char value );
void PA_SetBlobInArray      ( PA_Variable ar, PA_long32 i, PA_Blob value );
void PA_SetArrayInArray     ( PA_Variable ar, PA_long32 i, PA_Variable value );
void PA_SetPointerInArray   ( PA_Variable ar, PA_long32 i, PointerBlock value );
void PA_SetObjectInArray    ( PA_Variable ar, PA_long32 i, PA_ObjectRef object );

// ---------------------------------------------------------------
// 4D Application Methods and functions
// ---------------------------------------------------------------


void			PA_CreateMethod    (PA_Unichar* methodName, PA_MethodFlags ref, PA_Unistring* methodCode, PA_Unichar* folderName );
void			PA_ModifyMethod    (PA_Unichar* methodName, PA_Unistring* methodCode );
void			PA_ExecuteMethod   ( PA_Unistring* ustr );
PA_Variable		PA_ExecuteFunction ( PA_Unistring* ustr );

PA_long32			PA_Tokenize                ( PA_Unistring* ustr, void* tokens );
PA_Unistring	PA_Detokenize              ( void* tokens, PA_long32 len );
void			PA_ByteSwapTokens          ( void* tokens, PA_long32 len );
void			PA_ExecuteTokens           ( void* tokens, PA_long32 len );
PA_Variable		PA_ExecuteTokensAsFunction ( void* tokens, PA_long32 len );

PA_long32			PA_GetMethodID             ( PA_Unichar* methodName );
PA_Variable		PA_ExecuteMethodByID       ( PA_long32 id, PA_Variable* parameters, short nbParameters );

PA_long32			PA_GetCommandID			   ( PA_Unichar* commandName );
PA_Variable		PA_ExecuteCommandByID      ( PA_long32 id, PA_Variable* parameters, short nbParameters );

// use same index as the "command name" 4D command.
void			PA_GetCommandName          ( PA_long32 index, PA_Unichar* commandName );

// ---------------------------------------------------------------
// localisation
// ---------------------------------------------------------------

PA_Unistring PA_LocaliseStringByID( PA_long32 resID, PA_long32 resIndex, char pluginResource );
PA_Unistring PA_LocaliseString( PA_Unichar* name, char pluginResource );

// ---------------------------------------------------------------
// Log and Backup operations
// ---------------------------------------------------------------

void PA_LockDatabase( );
void PA_UnlockDatabase( );

// ---------------------------------------------------------------
// Multiprocess Printing Management
// ---------------------------------------------------------------

char PA_TryToOpenPrinterSession();
char PA_OpenPrinterSession();
void PA_ClosePrinterSession();
void* PA_GetCarbonPrintSettings();
void* PA_GetCarbonPageFormat();
void* PA_GetWindowsPRINTDLG();
void* PA_GetWindowsPrintingDC();


// ---------------------------------------------------------------
// Processes
// ---------------------------------------------------------------

PA_long32 PA_CountActiveProcess      ( );
PA_long32 PA_CountTotalProcess       ( );
void PA_GetProcessInfo          ( PA_long32 process, PA_Unichar* name, PA_long32* state, PA_long32* time );
void PA_FreezeProcess           ( PA_long32 process );
void PA_UnfreezeProcess         ( PA_long32 process );
char PA_IsProcessDying          ( );
void PA_KillProcess             ( );
void PA_PutProcessToSleep       ( PA_long32 process, PA_long32 time );
PA_long32 PA_GetCurrentProcessNumber ( );
PA_long32 PA_GetWindowProcess        ( PA_WindowRef windowRef );
PA_long32 PA_GetFrontWindowProcess   ( char withPalette );
void PA_SetWindowProcess        ( PA_WindowRef windowRef, PA_long32 process );
void PA_Yield                   ( );
void PA_YieldAbsolute           ( );
char PA_WaitNextEvent		    ( PA_Event* event );
void PA_UpdateProcessVariable   ( PA_long32 process );
void PA_BringProcessToFront     ( PA_long32 process );
PA_long32 PA_NewProcess				( void* procPtr, PA_long32 stackSize, PA_Unichar* name );
void PA_PostMacEvent            ( PA_long32 process, PA_Event* event );

// Execute some C code in Main Process. Function should be declared as void myFunc(void*)
// It may be mandatory for some API call on MacOSX like calling system dialogs
void PA_RunInMainProcess        ( PA_RunInMainProcessProcPtr procPtr, void* parameters );

// ---------------------------------------------------------------
// Sending/Receiving documents on 4D Client from 4D Server
// ---------------------------------------------------------------

PA_long32 PA_ReceiveDocumentFromServer ( PA_Unichar* docName, void* buffer );
void PA_SendDocumentToServer      ( PA_Unichar* docName, void* buffer, PA_long32 len, PA_ulong32 type, PA_ulong32 creator );
char PA_DocumentExistOnServer     ( PA_Unichar* docName, PA_ulong32 type, PA_ulong32 creator );



// ---------------------------------------------------------------
// Sending/Receiving data between 4D Client and 4D Server
// ---------------------------------------------------------------

void PA_SendDataToServer      ( PA_long32 pluginRef, PA_long32 dataRef, PA_long32 dataKind, void* buffer, PA_long32 len );
PA_long32 PA_ReceiveDataFromServer ( PA_long32 pluginRef, PA_long32 dataRef, PA_long32 dataKind, void* buffer );



// ---------------------------------------------------------------
// 4D Application internal resource manager used by 4D structure
// ---------------------------------------------------------------

// To read a resource, read first the length of the resource by passing a null buffer to PA_GetResource
// then allocate a buffer of the proper size, and finally read it in the buffer calling PA_GetResource again.
// Before manipulating a resource, you need to lock it over the network so no one can modify it
// during the time you read or modify it.

PA_long32  PA_CreateResource                 ( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name, char* data, PA_long32 len );
void  PA_RemoveResourceByID             ( short resfile, PA_ulong32 kind, PA_long32 id );
void  PA_RemoveResourceByName           ( short resfile, PA_ulong32 kind, PA_Unichar* name );
PA_long32  PA_GetResource                    ( short resfile, PA_ulong32 kind, PA_long32 id, char* data );
void  PA_SetResource                    ( short resfile, PA_ulong32 kind, PA_long32 id, char* data, PA_long32 len );
char  PA_LockResource                   ( short resfile, PA_ulong32 kind, PA_long32 id );
void  PA_UnlockResource                 ( short resfile, PA_ulong32 kind, PA_long32 id );
void  PA_ReleaseResource                ( short resfile, PA_ulong32 kind, PA_long32 id );
PA_long32  PA_GetIndexedResource             ( short resfile, PA_ulong32 kind, PA_long32 index, char* data );
void  PA_GetResourceName                ( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name );
void  PA_SetResourceName                ( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name );
PA_long32  PA_GetResourceSize                ( short resfile, PA_ulong32 kind, PA_long32 id );
PA_long32  PA_GetResourceTimeStamp           ( short resfile, PA_ulong32 kind, PA_long32 id );
PA_long32  PA_GetResourceIDList              ( short resfile, PA_ulong32 kind, PA_long32* IDlist );
PA_long32  PA_GetUniqueResID                 ( short resfile, PA_ulong32 kind );
PA_long32  PA_GetResourceKindList            ( short resfile, PA_ulong32* kindlist );
PA_long32  PA_CountResources                 ( short resfile, PA_ulong32 kind );
PA_long32  PA_CountResourceKinds             ( short resfile );
short PA_OpenResFile                    ( PA_Unistring* filename );
short PA_CreateResFile                  ( PA_Unistring* filename );
void  PA_UpdateResFile                  ( short resfile );
void  PA_UseResFile                     ( short resfile );
void  PA_CloseResFile                   ( short resfile );
short PA_GetDatabaseResFile             ( );
PA_Handle PA_GetResourceHandle          ( short resfile, PA_ulong32 kind, PA_long32 id );
void  PA_WriteResourceHandle            ( short resfile, PA_Handle handle );
PA_long32  PA_CreateResourceFromHandle       ( short resfile, PA_ulong32 kind, PA_long32 id, PA_Unichar* name, PA_Handle resourceHandle );
void  PA_DetachResource                 ( short resfile, PA_Handle resourceHandle );
void  PA_LockResourceHandle             ( short resfile, PA_Handle resourceHandle );
void  PA_UnlockResourceHandle           ( short resfile, PA_Handle resourceHandle );
void  PA_ReleaseResourceHandle          ( short resfile, PA_Handle resourceHandle );
PA_ulong32 PA_GetIndexedResourceKind ( short resfile, PA_long32 index );

// -----------------------------------------
//	4D Application users info
// -----------------------------------------

void PA_GetUserName( PA_Unichar* name );
PA_long32 PA_GetCurrentUserID();

// return a string array variable that can be parsed using array commands
// array starts by Designers users, then administrators users, then regular users
PA_Variable PA_GetUsersArray( PA_long32* nbDesignerUsers, PA_long32* nbAdministratorUsers );
PA_Variable PA_GetGroupsArray( PA_long32* nbDesignerGroups, PA_long32* nbAdministratorGroups );


// -----------------------------------------
//	4D Application serial key and users info
// -----------------------------------------

PA_long32 PA_GetSerialKey             ( );
void PA_GetRegisteredUserName    ( PA_Unichar *name );
void PA_GetRegisteredUserCompany ( PA_Unichar *company );
PA_long32 PA_CountConnectedUsers      ( );
PA_long32 PA_GetMaxAllowedUsers       ( );



// -----------------------------------------
//	4D Application Plugin Windows
// -----------------------------------------


PA_PluginRef PA_OpenPluginWindow ( PA_Unichar* areaName, PA_Unichar* windowTitle, PA_Rect rect );
PA_WindowRef PA_NewWindow        ( PA_Rect rect, PA_WindowLevel level, short kind, PA_Unichar* title, char closeBox );

void  PA_SetWindowFocusable    ( PA_WindowRef windowRef, char focusable );
char  PA_IsWindowFocusable     ( PA_WindowRef windowRef );

PA_WindowRef PA_GetWindowFocused ( );
void         PA_SetWindowFocused ( PA_WindowRef windowRef );
char         PA_IsWindowFocused  ( PA_WindowRef windowRef );

void  PA_UpdateVariables       ( );
sLONG_PTR	PA_GetHWND               ( PA_WindowRef windowRef );
sLONG_PTR	PA_GetMainWindowHWND();
sLONG_PTR	PA_GetWindowPtr          ( PA_WindowRef windowRef );
void  PA_ClosePluginWindow     ( PA_PluginRef pluginRef );
void  PA_SetPluginWindowTitle  ( PA_PluginRef pluginRef, PA_Unichar* windowTitle );
void  PA_SetPluginAreaClipMode ( PA_PluginRef pluginRef, char clipChildren );
short PA_GetCurrentPage        ( );
void  PA_CreateTip             ( PA_Unistring* ustr, char useRTF, short posX, short posY, PA_Rect rect );
void  PA_GotoArea              ( PA_Unichar* variableName );
void  PA_UpdateEditMenu        ( char undo, char redo, char cut, char copy, char paste, char clear, char selectAll );
void  PA_CloseWindow           ( PA_WindowRef windowRef );


// -----------------------------------------
//	4D Application Text Editors
// -----------------------------------------

PA_Handle PA_GetCurrentTEHandle();


// -----------------------------------------
//	4D Application Web Server
// -----------------------------------------

void PA_StartWebServer   ( );
void PA_StopWebServer    ( );
void PA_GetWebServerInfo ( PA_long32* webServerProcess, PA_long32* TCPport );
void PA_SetWebTCPport    ( PA_long32 TCPport );
void PA_GetWebContext    ( PA_long32* context, PA_long32* subContext, PA_Unichar* name );
PA_long32 PA_GetWebTimeOut    ( );
void PA_SetWebTimeOut    ( PA_long32 timeOut );
void PA_OpenURL          ( PA_Unistring* url );


// -----------------------------------------
//	4D Application SpellCheckers
// -----------------------------------------

void  PA_Install4DWriteSpeller    ( void* spellcheckerProcPtr );
void* PA_Get4DWriteSpellerProcPtr ( );


// -----------------------------------------
//	4D Application Editors
// -----------------------------------------

char PA_FormulaEditor      ( short defaultTable, PA_Unistring* formula );
void PA_QueryDialog        ( short table );
void PA_OrderByDialog      ( short table );
void PA_Alert              ( PA_Unichar* message, PA_WindowRef ref );
void PA_CharAlert		   ( const char* message, PA_WindowRef ref );
char PA_Confirm            ( PA_Unichar* message, PA_WindowRef ref );
char PA_Request            ( PA_Unichar* message, PA_Unichar* value, PA_Unichar* okButton, PA_Unichar* cancelButton );
PA_Handle PA_PictureEditor ( PA_Unichar* windowTitle,  void* picture, PA_long32 len );
void PA_MethodEditor       ( PA_Unichar* methodName, PA_long32 lineNumber );
void PA_AboutDialog        ( );


// -----------------------------------------
//	4D Application Pictures
// -----------------------------------------

// Quicktime is mandatory for this routine, format needs to be a valid quicktime format
PA_Handle PA_ConvertPicture( void* picture, PA_long32 len, PA_ulong32 format );

// returns a EMF handle
void* PA_PictureToEMF ( void* picture, PA_long32 len );

PA_Handle PA_PictureToGIF  ( void* picture, PA_long32 len );


// -----------------------------------------
//	4D Application Dialogs
// -----------------------------------------

// dialog
PA_Dial4D   PA_NewDialog           ( );
PA_Dial4D   PA_OpenDialog          ( PA_Dial4D dialog, PA_Unichar* dialogName, PA_Unichar* windowTitle, char closeBox );
char PA_ModalDialog                ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_CloseDialog                ( PA_Dial4D dialog );
void PA_Dial4DCancelValidate       ( PA_Dial4D dialog, char cancel );
void PA_Dial4DDisposeEmptyDialog   ( PA_Dial4D dialog );

// variables
PA_long32         PA_Dial4DGetLong      ( PA_Dial4D dialog, PA_Unichar* variableName );
double       PA_Dial4DGetReal      ( PA_Dial4D dialog, PA_Unichar* variableName );
void         PA_Dial4DGetDate      ( PA_Dial4D dialog, PA_Unichar* variableName, short* day, short* month, short* year );
PA_long32         PA_Dial4DGetTime      ( PA_Dial4D dialog, PA_Unichar* variableName );
void         PA_Dial4DGetString    ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unichar* string );
PA_Unistring PA_Dial4DGetText      ( PA_Dial4D dialog, PA_Unichar* variableName );
PA_Variable  PA_Dial4DGetVariable  ( PA_Dial4D dialog, PA_Unichar* variableName );

void PA_Dial4DSetLong              ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 value );
void PA_Dial4DSetReal              ( PA_Dial4D dialog, PA_Unichar* variableName, double value );
void PA_Dial4DSetDate              ( PA_Dial4D dialog, PA_Unichar* variableName, short day, short month, short year );
void PA_Dial4DSetTime              ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 time );
void PA_Dial4DSetString            ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unichar* string );
void PA_Dial4DSetText              ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unistring* text );
void PA_Dial4DSetPicture           ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Picture picture );
void PA_Dial4DSetVariable          ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Variable var );

// arrays 
void PA_Dial4DNewArrayLong         ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_Dial4DNewArrayReal         ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_Dial4DNewArrayString       ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_Dial4DNewArrayPicture      ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_Dial4DNewArrayText         ( PA_Dial4D dialog, PA_Unichar* variableName );

PA_long32 PA_Dial4DGetArraySize         ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_Dial4DDeleteArrayElements  ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 position, PA_long32 amount );

void PA_Dial4DSetArrayTextFromResource      ( PA_Dial4D dialog, PA_Unichar* variableName, short resourceID );
void PA_Dial4DSetArrayTextFromTableList     ( PA_Dial4D dialog, PA_Unichar* variableName );
void PA_Dial4DSetArrayTextFromFieldList     ( PA_Dial4D dialog, PA_Unichar* variableName, short table );
void PA_Dial4DSetArrayTextFromTTR           ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unistring* ustr );
void PA_Dial4DSetArrayPictureFromResources  ( PA_Dial4D dialog, PA_Unichar* variableName, short* idArray, PA_long32 nb );

void PA_Dial4DSetArrayLong           ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 value, PA_long32 position );
void PA_Dial4DSetArrayReal           ( PA_Dial4D dialog, PA_Unichar* variableName, double value, PA_long32 position );
void PA_Dial4DSetArrayText           ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unistring* text, PA_long32 position );
void PA_Dial4DSetArrayString         ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unichar*   string, PA_long32 position );
void PA_Dial4DSetArrayPicture        ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Picture picture, PA_long32 position );

PA_long32         PA_Dial4DGetArrayLong   ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 indice );
double       PA_Dial4DGetArrayReal   ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 indice );
PA_Unistring PA_Dial4DGetArrayText   ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 indice );
void         PA_Dial4DGetArrayString ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unichar* string, PA_long32 indice );

PA_long32 PA_Dial4DFindArrayLong          ( PA_Dial4D dialog, PA_Unichar* variableName, PA_long32 value,   PA_long32 startPosition );
PA_long32 PA_Dial4DFindArrayReal          ( PA_Dial4D dialog, PA_Unichar* variableName, double value, PA_long32 startPosition );

// context switch from process variables to dialog variables
void* PA_Dial4DSaveVariables         ( PA_Dial4D dialog );
void  PA_Dial4DRestoreVariables      ( PA_Dial4D dialog, void* env );

// update modified variables in dialog
void  PA_Dial4DBeginUpdateVariables  ( PA_Dial4D dialog );
void  PA_Dial4DEndUpdateVariables    ( PA_Dial4D dialog );

// external areas
void  PA_Dial4DSetAreaHandler        ( PA_Dial4D dialog, PA_Unichar* variableName, void* handler, void* privateData );
void  PA_Dial4DRedrawExternalArea    ( PA_Dial4D dialog, PA_Unichar* variableName );

// user datas
void  PA_Dial4DSetUserData           ( PA_Dial4D dialog, void* userData );
void* PA_Dial4DGetUserData           ( PA_Dial4D dialog );

// pages
void  PA_Dial4DGotoPage              ( PA_Dial4D dialog, short page );
short PA_Dial4DGetCurrentPage        ( PA_Dial4D dialog );

// objects
void  PA_Dial4DGetVariableRect       ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Rect* rect );
void  PA_Dial4DSetVariableRect       ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Rect rect );
void  PA_Dial4DShowHideVariable      ( PA_Dial4D dialog, PA_Unichar* variableName, char visible );
void  PA_Dial4DShowHideObject        ( PA_Dial4D dialog, PA_Unichar* objectName,   char visible );
void  PA_Dial4DSetEnterable          ( PA_Dial4D dialog, PA_Unichar* variableName, char enterable );
void  PA_Dial4DSetEnable             ( PA_Dial4D dialog, PA_Unichar* variableName, char enabled );
void  PA_Dial4DSetMin                ( PA_Dial4D dialog, PA_Unichar* variableName, double minValue );
void  PA_Dial4DSetMinDate            ( PA_Dial4D dialog, PA_Unichar* variableName, short day, short month, short year );
void  PA_Dial4DSetMax                ( PA_Dial4D dialog, PA_Unichar* variableName, double maxValue );
void  PA_Dial4DSetMaxDate            ( PA_Dial4D dialog, PA_Unichar* variableName, short day, short month, short year );
void  PA_Dial4DSet3StatesCheckBox    ( PA_Dial4D dialog, PA_Unichar* variableName, char state );
void  PA_Dial4DSetEntryFilter        ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unichar* entryFilter );
void  PA_Dial4DSetDisplayFormat      ( PA_Dial4D dialog, PA_Unichar* variableName, PA_Unichar* displayFormat );
void  PA_Dial4DSetObjectTitle        ( PA_Dial4D dialog, PA_Unichar* objectName,   PA_Unichar* title );
void  PA_Dial4DUpdateObjectTitle     ( PA_Dial4D dialog, PA_Unichar* variableName );
void  PA_Dial4DGetLastObject         ( PA_Dial4D dialog, PA_Unichar* objectName   );
void  PA_Dial4DGotoVariable          ( PA_Dial4D dialog, PA_Unichar* variableName );
void  PA_Dial4DHighlightText         ( PA_Dial4D dialog, PA_Unichar* variableName, short startSelection, short endSelection );

// hlists
void  PA_Dial4DNewTableFieldHList          ( PA_Dial4D dialog, PA_Unichar* variableName, short table, char showRelatedTables, char showRelatedFields, char useIcons, char sorted );
void  PA_Dial4DGetTableFieldHListCurrent   ( PA_Dial4D dialog, PA_Unichar* variableName, short* table, short* field );
void  PA_Dial4DSetTableFieldHListCurrent   ( PA_Dial4D dialog, PA_Unichar* variableName, short table, short field );
void  PA_Dial4DDisposeHList                ( PA_Dial4D dialog, PA_Unichar* variableName );

// drag and drop
void  PA_Dial4DGetDragInfo                 ( PA_Dial4D dialog, PA_Unichar* dropVariable, short* dropX, short* dropY, PA_Dial4D* dragDialog, PA_Unichar* dragVariable, short* dragX, short* dragY );

// windows
PA_Dial4D    PA_Dial4DDial4DFromWindow     ( PA_WindowRef window );
PA_WindowRef PA_Dial4DWindowFromDial4D     ( PA_Dial4D dialog );
void         PA_Dial4DAllowXResize         ( PA_Dial4D dialog, char allowResize );
void         PA_Dial4DAllowYResize         ( PA_Dial4D dialog, char allowResize );
void         PA_Dial4DGetWindowMinMaxInfo  ( PA_Dial4D dialog, PA_long32* minXresize, PA_long32* maxXresize, PA_long32* minYresize, PA_long32* maxYresize );
void         PA_Dial4DSetWindowSize        ( PA_Dial4D dialog, PA_long32 width, PA_long32 height );

#ifdef __cplusplus
}
#endif

#endif
