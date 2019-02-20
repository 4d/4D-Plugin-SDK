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
// File : EntryPoints.h
// Description : Describes the entrypoints numbers as implemented in 4D
//
// rev : 13
//
// ---------------------------------------------------------------



#ifndef __ENTRYPOINTS__
#define __ENTRYPOINTS__

//       EntryPoint
// ===============================
#define  EX_CREATE_RECORD                           -1      //PA_CreateRecord
#define  EX_SAVE_RECORD                             -2      // PA_SaveRecord
#define  EX_NEXT_RECORD                             -3      // PA_NextRecord
#define  EX_PREVIOUS_RECORD                         -4      // PA_PreviousRecord
#define  EX_GOTO_SELECTED_RECORD                    -5      // PA_GotoSelectedRecord
#define  EX_RECORDS_IN_SELECTION                    -6      // PA_RecordsInSelection
#define  EX_DELETE_SELECTION                        -7      // PA_DeleteSelection
#define  EX_FIRST_RECORD                            -8      // PA_FirstRecord
#define  EX_COUNT_TABLES                            -9      // PA_CountTables (ThreadSafe)
#define  EX_COUNT_FIELDS                            -10     // PA_CountFields (PA_UseVirtualStructure mode) (ThreadSafe)
#define  EX_COUNT_INVISIBLE_FIELDS_TOO              -717    // PA_CountFields (PA_UseRealStructure mode) (ThreadSafe)
#define  EX_GET_TABLE_NAME                          -11     // PA_GetTableName, PA_IsTableVisible (ThreadSafe)
#define  EX_FIELD_ATTRIBUTES                        -12     // PA_GetFieldName, PA_GetFieldProperties (ThreadSafe)
#define  EX_ORDER_BY                                -13     // PA_CloseOrderBy, PA_OrderBy, PA_OrderByDialog
#define  EX_QUERY                                   -14     // PA_CloseQuery, PA_QueryDialog
#define  EX_BEFORE_SELECTION                        -15     // PA_BeforeSelection
#define  EX_END_SELECTION                           -16     // PA_EndSelection
#define  EX_GET_FIELD                               -17     // PA_GetXXXField (ThreadSafe)
#define  EX_SET_FIELD                               -18     // PA_SetXXXField
#define  EX_GET_VARIABLE                            -19     // PA_GetVariable (ThreadSafe)
#define  EX_SET_VARIABLE                            -20     // PA_SetVariable
#define  EX_GET_EXPORT_FILTER                       -24     // PA_GetExportFilter
#define  EX_GET_IMPORT_FILTER                       -25     // PA_GetImportFilter
#define  EX_GET_PACKED_RECORD                       -26     // PA_GetPackedRecord
#define  EX_SET_PACKED_RECORD                       -27     // PA_SetPackedRecord
#define  EX_CREATE_SET                              -28     // PA_CreateSet
#define  EX_USE_SET                                 -29     // PA_UseSet
#define  EX_EXECUTE_METHOD                          -30     // PA_ExecuteMethod
#define  EX_EXECUTE_FUNCTION                        -31     // PA_ExecuteFunction
#define  EX_CREATE_METHOD                           -32     // PA_CreateMethod
#define  EX_LOCKED                                  -40     // PA_Locked
#define  EX_READ_WRITE                              -41     // PA_ReadWrite, PA_ReadOnly
#define  EX_LOAD_RECORD                             -42     // PA_LoadRecord
#define  EX_RELATE_ONE                              -43     // PA_RelateOne
#define  EX_RELATE_MANY                             -44     // PA_RelateMany
#define  EX_STRING                                  -47     // PA_FormatString
#define  EX_POPUP_FIELD_LIST                        -48     // PA_FieldPopup
#define  EX_POPUP_TABLE_LIST                        -49     // PA_TableAndFieldPopup
#define  EX_GET_TABLE_FIELD_NUMBER                  -50     // PA_GetTableAndFieldNumbers (ThreadSafe)
#define  EX_SELECTED_RECORD_NUMBER                  -51     // PA_SelectedRecordNumber
#define  EX_GET_INFORMATION                         -56     // PA_Get4DVersion, PA_IsDemoVersion, (ThreadSafe)
                                                            // PA_IsCompiled, PA_Is4DClient,
                                                            // PA_IsWebProcess, PA_GetMessagesStatus,
                                                            //PA_IsDatabaseLocked, PA_Is4DServer
#define  EX_MESSAGES                                -57     // PA_SetMessagesStatus
#define  EX_COMPARE_MACSTRINGS                      -60     // PA_CompareMacStrings
#define  EX_EVAL_NUMBER                             -61     // PA_EvalReal, PA_EvalLongint
#define  EX_FIND_PACKAGE                            -64     // PA_FindPackage
#define  EX_UPDATE_VARIABLES                        -65     // PA_UpdateVariables
#define  EX_CHANGE_TITLE                            -66     // PA_SetPluginWindowTitle
#define  EX_OPEN_PLUGIN_WINDOW                      -67     // PA_OpenPluginWindow
#define  EX_CLOSE_PLUGIN_WINDOW                     -68     // PA_ClosePluginWindow
#define  EX_RECORD_NUMBER                           -73     // PA_RecordNumber
#define  EX_ADD_TO_SET                              -74     // PA_AddToSet
#define  EX_CREATE_EMPTY_SET                        -75     // PA_CreateEmptySet
#define  EX_DELETE_SET                              -76     // PA_DeleteSet
#define  EX_GET_CURRENT_PAGE                        -80     // PA_GetCurrentPage
#define  EX_GET_CURRENT_TEHANDLE                    -81     // PA_GetCurrentTEHandle
#define  EX_TOKENIZE                                -82     // PA_Tokenize
#define  EX_EXEC_TOKEN_PROC                         -83     // PA_ExecuteTokens
#define  EX_EXEC_TOKEN_FUNC                         -84     // PA_ExecuteTokensAsFunction
#define  EX_KILL_PROCESS                            -86     // PA_KillProcess
#define  EX_FREEZE_PROCESS                          -87     // PA_FreezeProcess (ThreadSafe)
#define  EX_UNFREEZE_PROCESS                        -88     // PA_UnfreezeProcess
#define  EX_PUT_PROCESS_TO_SLEEP                    -89     // PA_PutProcessToSleep (ThreadSafe)
#define  EX_GET_WINDOW_PROCESS                      -90     // PA_GetWindowProcess, PA_GetFrontWindowProcess
#define  EX_CURRENT_PROCESS_NUMBER                  -91     // PA_GetCurrentProcessNumber (ThreadSafe)
#define  EX_SET_WINDOW_PROCESS                      -92     // PA_SetWindowProcess
#define  EX_YIELD                                   -93     // PA_Yield (ThreadSafe)
#define  EX_GET_PROCESS_INFO                        -94     // PA_GetProcessInfo (ThreadSafe)
#define  EX_NB_PROCESS                              -95     // PA_CountActiveProcess (ThreadSafe)
#define  EX_GOTO_RECORD                             -96     // PA_GotoRecord
#define  EX_BRING_PROCESS_TO_FRONT                  -97     // PA_BringProcessToFront
#define  EX_READ_DOCUMENT                           -100    // PA_ReceiveDocument
#define  EX_SAVE_DOCUMENT                           -101    // PA_SendDocument
#define  EX_READ_DATA                               -102    // PA_ReceiveDataFromServer
#define  EX_WRITE_DATA                              -103    // PA_SendDataToServer
#define  EX_LOCK_DATABASE                           -107    // PA_LockDatabase
#define  EX_UNLOCK_DATABASE                         -108    // PA_UnlockDatabase
#define  EX_GET_USER_LIST                           -112    // PA_CountUsers
#define  EX_GET_PREF_FILE                           -115    // PA_Get4DPreferencesFilePath
#define  EX_CALL_BY_PROCID                          -118    // PA_ExecuteMethodByID
#define  EX_THERMOMETER                             -121    // PA_SetThermometerRect
#define  EX_YIELD_ABSOLUTE                          -122    // PA_YieldAbsolute (ThreadSafe)
#define  EX_GET_INTERPROCESS_VARIABLE               -125    // PA_GetVariable
#define  EX_SET_INTERPROCESS_VARIABLE               -126    // PA_SetVariable
#define  EX_EXIST_DOCUMENT_ON_SERVER                -127    // PA_DocumentExistOnServer
#define  EX_GET_PRINT_INFO                          -130    // PA_GetWindowsPRINTDLG, PA_GetWindowsPrintingDC (ThreadSafe)
                                                            // PA_GetCarbonPageFormat, PA_GetCarbonPrintSettings
#define  EX_GET_FIELD_RELATIONS                     -131    // PA_GetFieldRelation
#define  EX_UPDATE_PROCESS_VARIABLE                 -135    // PA_UpdateProcessVariable
#define  EX_QUIT4D                                  -136    // PA_Quit4D
#define  EX_GET_GROUP_LIST                          -145    // PA_CountUserGroups
#define  EX_NEW_PROCESS                             -158    // PA_NewProcess (ThreadSafe)
#define  EX_GET_HWND                                -161    // PA_GetHWND, PA_GetHDC
#define  EX_BYTE_SWAP_TOKENS                        -162    // PA_ByteSwapTokens
#define  EX_GET_PLATFORM_INTERFACE                  -164    // PA_GetPlatformInterface
#define  EX_POST_EVENT                              -165    // PA_PostEvent
#define  EX_PACKAGE_INFO                            -169    // PA_PackageInfo
#define  EX_START_WEB_SERVER                        -172    // PA_StartWebServer
#define  EX_STOP_WEB_SERVER                         -173    // PA_StopWebServer
#define  EX_GET_WEB_SERVER_INFO                     -174    // PA_GetWebServerInfo
#define  EX_SET_WEB_TCP_PORT                        -175    // PA_SetWebTCPport
#define  EX_GET_WEB_CONTEXT                         -176    // PA_GetWebContext
#define  EX_WEB_TIMEOUT                             -177    // PA_GetWebTimeOut, PA_SetWebTimeOut
#define  EX_GET_TOOLBAR_INFO                        -178    // PA_GetToolBarInfo
#define  EX_SHOW_HIDE_TOOLBAR                       -179    // PA_ShowHideToolBar
#define  EX_DRAG_AND_DROP                           -180    // PA_DragAndDrop
#define  EX_INSTALL_4DWRITE_SPELLER                 -183    // PA_Install4DWriteSpeller
#define  EX_GET_4DWRITE_SPELLER                     -184    // PA_Get4DWriteSpellerProcPtr
#define  EX_DETOKENIZE                              -185    // PA_Detokenize
#define  EX_DIAL4D_NEW_EMPTY_DIALOG                 -200    // PA_NewDialog
#define  EX_DIAL4D_OPEN_DIALOG                      -201    // PA_OpenDialog
#define  EX_DIAL4D_CLOSE_DIALOG                     -202    // PA_CloseDialog
#define  EX_DIAL4D_MODAL_DIALOG                     -203    // PA_ModalDialog
#define  EX_DIAL4D_SET_ENABLE                       -204    // PA_Dial4DSetEnable
#define  EX_DIAL4D_GET_VARIABLE                     -205    // PA_Dial4DGetVariable
#define  EX_DIAL4D_SET_VARIABLE                     -206    // PA_Dial4DSetVariable
#define  EX_DIAL4D_GET_LONG                         -207    // PA_Dial4DGetLong
#define  EX_DIAL4D_SET_LONG                         -208    // PA_Dial4DSetLong
#define  EX_DIAL4D_GET_REAL                         -209    // PA_Dial4DGetReal
#define  EX_DIAL4D_SET_REAL                         -210    // PA_Dial4DSetReal
#define  EX_DIAL4D_GET_STRING                       -211    // PA_Dial4DGetString
#define  EX_DIAL4D_SET_STRING                       -212    // PA_Dial4DSetString
#define  EX_DIAL4D_SET_PICTURE                      -213    // PA_Dial4DSetPictureHandle
#define  EX_DIAL4D_SAVE_VARIABLES                   -215    // PA_Dial4DSaveVariables
#define  EX_DIAL4D_RESTORE_VARIABLES                -216    // PA_Dial4DRestoreVariables
#define  EX_DIAL4D_GET_ARRAY_REAL                   -217    // PA_Dial4DGetArrayReal
#define  EX_DIAL4D_GET_ARRAY_STRING                 -218    // PA_Dial4DGetArrayString
#define  EX_DIAL4D_GET_ARRAY_LONG                   -219    // PA_Dial4DGetArrayLong
#define  EX_ABOUT_DIALOG                            -220    // PA_AboutDialog
#define  EX_DIAL4D_SET_AREA_HANDLER                 -221    // PA_Dial4DSetAreaHandler
#define  EX_DIAL4D_GET_VARIABLE_RECT                -222    // PA_Dial4DGetVariableRect
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_RESOURCE     -223    // PA_Dial4DSetArrayTextFromResource
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_TABLE_LIST   -224    // PA_Dial4DSetArrayTextFromTableList
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_FIELD_LIST   -225    // PA_Dial4DSetArrayTextFromFieldList
#define  EX_DIAL4D_BEGIN_UPDATE_VARIABLES           -226    // PA_Dial4DBeginUpdateVariables
#define  EX_DIAL4D_END_UPDATE_VARIABLES             -227    // PA_Dial4DEndUpdateVariables
#define  EX_DIAL4D_NEW_ARRAY_STRING                 -228    // PA_Dial4DNewArrayString
#define  EX_DIAL4D_SET_ARRAY_STRING                 -229    // PA_Dial4DSetArrayString
#define  EX_DIAL4D_SET_DATE                         -230    // PA_Dial4DSetDate
#define  EX_DIAL4D_GET_DATE                         -231    // PA_Dial4DGetDate
#define  EX_DIAL4D_SHOW_HIDE_VARIABLE               -232    // PA_Dial4DShowHideVariable
#define  EX_DIAL4D_GOTO_PAGE                        -233    // PA_Dial4DGotoPage
#define  EX_DIAL4D_GET_CURRENT_PAGE                 -234    // PA_Dial4DGetCurrentPage
#define  EX_DIAL4D_NEW_TABLE_FIELD_HLIST            -235    // PA_Dial4DNewTableFieldHList
#define  EX_DIAL4D_DISPOSE_HLIST                    -236    // PA_Dial4DDisposeHList
#define  EX_DIAL4D_GET_DRAG_INFO                    -237    // PA_Dial4DGetDragInfo
#define  EX_DIAL4D_GET_TABLE_FIELD_HLIST_CURRENT    -238    // PA_Dial4DGetTableFieldHListCurrent
#define  EX_DIAL4D_NEW_ARRAY_LONG                   -239    // PA_Dial4DNewArrayLong
#define  EX_DIAL4D_SET_ARRAY_LONG                   -240    // PA_Dial4DSetArrayLong
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_TTR          -241    // PA_Dial4DSetArrayTextFromTTR
#define  EX_DIAL4D_GET_ARRAY_SIZE                   -242    // PA_Dial4DGetArraySize
#define  EX_DIAL4D_DELETE_ARRAY_ELEMENTS            -243    // PA_Dial4DDeleteArrayElements
#define  EX_DIAL4D_REDRAW_EXTERNAL_AREA             -244    // PA_Dial4DRedrawExternalArea
#define  EX_DIAL4D_NEW_ARRAY_PICTURE                -245    // PA_Dial4DNewArrayPicture
#define  EX_DIAL4D_SET_ARRAY_PICTURE                -246    // PA_Dial4DSetArrayPicture,
                                                            // PA_Dial4DSetArrayPictureHandle
#define  EX_DIAL4D_SET_ARRAY_PICT_FROM_RESOURCES    -247    // PA_Dial4DSetArrayPictureFromResources
#define  EX_FORMULA_EDITOR                          -248    // PA_FormulaEditor
#define  EX_ALERT                                   -249    // PA_Alert (ThreadSafe)
#define  EX_CONFIRM                                 -250    // PA_Confirm
#define  EX_DIAL4D_SET_ENTERABLE                    -251    // PA_Dial4DSetEnterable
#define  EX_DIAL4D_SET_MIN                          -252    // PA_Dial4DSetMin, PA_Dial4DSetMinDate
#define  EX_DIAL4D_SET_MAX                          -253    // PA_Dial4DSetMax, PA_Dial4DSetMaxDate
#define  EX_PICTURE_CONVERSION                      -254    // PA_PictureToJPEG, PA_PictureToGIF, PA_PictureToEMF
#define  EX_CHECK_FREE_STACK                        -255    // PA_CheckFreeStack
#define  EX_GET_TIPS_ENABLED                        -256    // PA_GetTipsEnabled
#define  EX_SET_TIPS_ENABLED                        -257    // PA_SetTipsEnabled
#define  EX_DIAL4D_SET_USER_DATA                    -258    // PA_Dial4DSetUserData
#define  EX_DIAL4D_GET_USER_DATA                    -259    // PA_Dial4DGetUserData
#define  EX_DIAL4D_SET_3_STATES_CHECKBOX            -260    // PA_Dial4DSet3StatesCheckBox
#define  EX_DIAL4D_DIAL4D_FROM_WINDOW               -261    // PA_Dial4DDial4DFromWindow
#define  EX_DIAL4D_WINDOW_FROM_DIAL4D               -262    // PA_Dial4DWindowFromDial4D
#define  EX_DIAL4D_SET_TABLE_FIELD_HLIST_CURRENT    -263    // PA_Dial4DSetTableFieldHListCurrent
#define  EX_OPEN_URL                                -264    // PA_OpenURL
#define  EX_VIRTUAL_STRUCTURE_DEFINED               -301    // PA_VirtualStructureDefined
#define  EX_GET_TRUE_TABLE_NUMBER                   -302    // PA_GetTrueTableNumber
#define  EX_GET_TRUE_FIELD_NUMBER                   -303    // PA_GetTrueFieldNumber
#define  EX_CREATE_TIP                              -305    // PA_CreateTip
#define  EX_GOTO_AREA                               -306    // PA_GotoArea
#define  EX_UPDATE_EDIT_MENU                        -307    // PA_UpdateEditMenu
#define  EX_DIAL4D_SET_TEXT                         -308    // PA_Dial4DSetText
#define  EX_DIAL4D_GET_TEXT                         -309    // PA_Dial4DGetText
#define  EX_CREATE_RESOURCE                         -310    // PA_CreateResource, PA_CreateResourceFromHandle
#define  EX_REMOVE_RESOURCE                         -311    // PA_RemoveResourceByID,PA_RemoveResourceByName
#define  EX_GET_RESOURCE                            -312    // PA_GetResource, PA_SetResource,
                                                            // PA_LockResource, PA_GetResourceHandle
#define  EX_GET_INDEXED_RESOURCE                    -313    // PA_GetIndexedResource
#define  EX_RELEASE_RESOURCE                        -314    // PA_ReleaseResourceHandle, PA_ReleaseResource
#define  EX_DETACH_RESOURCE                         -315    // PA_DetachResource
#define  EX_WRITE_RESOURCE                          -316    // PA_SetResource
#define  EX_COUNT_RESOURCES                         -317    // PA_CountResources
#define  EX_GET_RESOURCE_INFO                       -318    // PA_GetResourceName, PA_GetResourceSize
#define  EX_SET_RESOURCE_NAME                       -319    // PA_SetResourceName
#define  EX_COUNT_RESOURCE_KINDS                    -320    // PA_CountResourceKinds
#define  EX_GET_INDEXED_RESOURCE_KIND               -321    // PA_GetIndexedResourceKind
#define  EX_LOCK_RESOURCE                           -322    // PA_LockResourceHandle
#define  EX_SET_RES_LOAD                            -323    // PA_SetResLoad
#define  EX_GET_RESOURCE_ID_LIST                    -324    // PA_GetResourceIDList
#define  EX_GET_RESOURCE_NAME_LIST                  -325    // PA_GetResourceNameList
#define  EX_GET_RESOURCE_KIND_LIST                  -326    // PA_GetResourceKindList
#define  EX_DIAL4D_NEW_ARRAY_REAL                   -327    // PA_Dial4DNewArrayReal
#define  EX_DIAL4D_SET_ARRAY_REAL                   -328    // PA_Dial4DSetArrayReal
#define  EX_DIAL4D_SET_ENTRY_FILTER                 -329    // PA_Dial4DSetEntryFilter
#define  EX_DIAL4D_SET_DISPLAY_FORMAT               -330    // PA_Dial4DSetDisplayFormat
#define  EX_DIAL4D_NEW_ARRAY_TEXT                   -331    // PA_Dial4DNewArrayText
#define  EX_DIAL4D_SET_ARRAY_TEXT                   -332    // PA_Dial4DSetArrayText
#define  EX_DIAL4D_GET_ARRAY_TEXT                   -333    // PA_Dial4DGetArrayText
#define  EX_GET_APPLICATION_FULLPATH                -335    // PA_GetApplicationFullPath
#define  EX_DIAL4D_DISPOSE_EMPTY_DIALOG             -336    // PA_Dial4DDisposeEmptyDialog
#define  EX_DIAL4D_SHOW_HIDE_OBJECT                 -337    // PA_Dial4DShowHideObject
#define  EX_DIAL4D_SET_TIME                         -338    // PA_Dial4DSetTime
#define  EX_DIAL4D_GET_TIME                         -339    // PA_Dial4DGetTime
#define  EX_DIAL4D_SET_OBJECT_TITLE                 -340    // PA_Dial4DSetObjectTitle
#define  EX_DIAL4D_UPDATE_OBJECT_TITLE              -341    // PA_Dial4DUpdateObjectTitle
#define  EX_OPEN_RESFILE                            -342    // PA_OpenResFile
#define  EX_CLOSE_RESFILE                           -343    // PA_CloseResFile
#define  EX_CREATE_RESFILE                          -346    // PA_CreateResFile
#define  EX_GET_DATABASE_RESFILE                    -347    // PA_GetDatabaseResFile
#define  EX_UPDATE_RESFILE                          -348    // PA_UpdateResFile
#define  EX_COPY_CUT_NAMED_SELECTION                -354    // PA_CopyNamedSelection, PA_CutNamedSelection
#define  EX_USE_NAMED_SELECTION                     -355    // PA_UseNamedSelection
#define  EX_CLEAR_NAMED_SELECTION                   -356    // PA_ClearNamedSelection
#define  EX_NEXT_WINDOW_LEVEL                       -357    // PA_NewWindow
#define  EX_WAIT_NEXT_EVENT                         -361    // PA_WaitNextEvent
#define  EX_NEW_WINDOW                              -379    // PA_NewWindow
#define  EX_CLOSE_WINDOW                            -380    // PA_CloseWindow
#define  EX_GET_VIRTUAL_TABLE_NUMBER                -382    // PA_GetVirtualTableNumber
#define  EX_GET_VIRTUAL_FIELD_NUMBER                -383    // PA_GetVirtualFieldNumber
#define  EX_USE_RESFILE                             -384    // PA_UseResFile()
#define  EX_GET_UNIQUE_RESID                        -392    // PA_GetUniqueResID
#define  EX_DIAL4D_SET_VARIABLE_RECT                -393    // PA_Dial4DSetVariableRect
#define  EX_DIAL4D_ALLOW_X_RESIZE                   -394    // PA_Dial4DAllowXResize
#define  EX_DIAL4D_ALLOW_Y_RESIZE                   -395    // PA_Dial4DAllowYResize
#define  EX_DIAL4D_GET_WINDOW_MIN_MAX_INFO          -396    // PA_Dial4DGetWindowMinMaxInfo
#define  EX_DIAL4D_SET_WINDOW_SIZE                  -397    // PA_Dial4DSetWindowSize
#define  EX_DIAL4D_FIND_ARRAY_LONG                  -398    // PA_Dial4DFindArrayLong
#define  EX_DIAL4D_FIND_ARRAY_REAL                  -399    // PA_Dial4DFindArrayReal
#define  EX_GET_CURRENT_USER_ID                     -400    // PA_GetCurrentUserID
#define  EX_PICTURE_EDITOR                          -402    // PA_PictureEditor
#define  EX_GET_USER_NAME                           -403    // PA_GetUserName
#define  EX_DIAL4D_GET_LAST_OBJECT                  -405    // PA_Dial4DGetLastObject
#define  EX_REQUEST                                 -407    // PA_Request
#define  EX_CLEAR_VARIABLE                          -410    // PA_ClearVariable     (ThreadSafe)
#define  EX_GET_STRUCTURE_FULLPATH                  -411    // PA_GetStructureFullPath (ThreadSafe)
#define  EX_RELATE_ONE_SELECTION                    -412    // PA_RelateOneSelection
#define  EX_RELATE_MANY_SELECTION                   -413    // PA_RelateManySelection
#define  EX_IS_PROCESS_DYING                        -414    // PA_IsProcessDying
#define  EX_DIAL4D_GOTO_VARIABLE                    -416    // PA_Dial4DGotoVariable
#define  EX_GET_RESOURCE_TIMESTAMP                  -417    // PA_GetResourceTimeStamp
#define  EX_SET_WINDOW_FOCUSABLE                    -418    // PA_SetWindowFocusable
#define  EX_IS_WINDOW_FOCUSABLE                     -419    // PA_IsWindowFocusable
#define  EX_GET_WINDOW_FOCUSED                      -420    // PA_GetWindowFocused
#define  EX_SET_WINDOW_FOCUSED                      -421    // PA_SetWindowFocused
#define  EX_IS_WINDOW_FOCUSED                       -422    // PA_IsWindowFocused
#define  EX_METHOD_EDITOR                           -423    // PA_MethodEditor
#define  EX_CONVERT_STRING                          -424    // PA_ConvertString         (ThreadSafe)
#define  EX_DIAL4D_CANCEL_VALIDATE                  -425    // PA_Dial4DCancelValidate
#define  EX_GET_4D_FOLDER                           -427    // PA_Get4DFolder (ThreadSafe)
#define  EX_DIAL4D_HIGHLIGHT_TEXT                   -428    // PA_Dial4DHighlightText
#define  EX_GET_SERIAL_KEY                          -429    // PA_GetSerialKey
#define  EX_GET_REGISTRATION_INFO                   -430    // PA_GetRegistredUserName
#define  EX_GET_USERS_INFO                          -431    // PA_CountConnectedUsers
#define  EX_GET_4D_HINSTANCE                        -432    // PA_Get4DHInstance
#define  EX_HANDLE_MANAGER                          -434    // PA_NewHandle, PA_DisposeHandle,      (ThreadSafe)
                                                            // PA_SetHandleSize, PA_GetHandleSize,  
                                                            // PA_SetHandleState, PA_GetHandleState,
                                                            // PA_LockHandle, PA_UnlockHandle
#define  EX_RESIZE_ARRAY                            -438    // PA_ResizeArray
#define  EX_UNLOCK_RESOURCE                         -439    // PA_UnlockResource, PA_UnlockResourceHandle
#define  EX_METHOD_NAMES                            -442    // PA_MethodNames
#define  EX_GET_COMMAND_ID                          -506    // PA_GetCommandID      (ThreadSafe)
#define  EX_GET_COMMAND_NAME                        -507    // PA_GetCommandName    (ThreadSafe)
#define  EX_GET_METHOD_ID                           -508    // PA_GetMethodID
#define  EX_GET_CENTURY_INFO                        -531    // PA_GetCenturyInfo
#define  EX_OPEN_PRINTER_SESSION                    -556    // PA_OpenPrinterSession
#define  EX_CLOSE_PRINTER_SESSION                   -557    // PA_ClosePrinterSession
#define  EX_TRY_TO_OPEN_PRINTER_SESSION             -558    // PA_TryToOpenPrinterSession
#define  EX_GET_TEMPLATE                            -593    // PA_GetTemplate
#define  EX_GET_POINTER_VALUE                       -585    // PA_GetPointerValue
#define  EX_SET_POINTER_VALUE                       -586    // PA_SetPointerValue
#define  EX_SET_PLUGINAREA_CLIPMODE                 -597    // PA_SetPluginAreaClipMode
#define  EX_COMPARE_UNIBUFFERS                      -611    // PA_CompareUniBuffers     (ThreadSafe)
#define  EX_CREATE_UNISTRING                        -612    // PA_CreateUniString       (ThreadSafe)
#define  EX_SET_UNISTRING                           -613    // PA_SetUniString          (ThreadSafe)
#define  EX_DISPOSE_UNISTRING                       -614    // PA_DisposeUniString      (ThreadSafe)
#define  EX_VARIABLE_TO_STRING                      -615    // PA_GetStringVariable     (ThreadSafe)
#define  EX_GET_PLUGIN_KEY_EVENT                    -616    // PA_GetKey
#define  EX_CREATE_PICTURE                          -617    // PA_CreatePicture         (ThreadSafe)
#define  EX_CREATE_NATIVE_PICTURE_FOR_SCREEN        -618    // PA_CreateNativePictureForScreen
#define  EX_CREATE_NATIVE_PICTURE_FOR_PRINTING      -619    // PA_CreateNativePictureForPrinting
#define  EX_DISPOSE_PICTURE                         -620    // PA_DisposePicture        (ThreadSafe)
#define  EX_PASTEBOARD_IS_DATA_AVAILABLE            -621    // PA_IsPasteboardDataAvailable
#define  EX_PASTEBOARD_GET_DATA                     -622    // PA_GetPasteboardData
#define  EX_PASTEBOARD_GET_DATA_SIZE                -623    // PA_GetPasteboardDataSize
#define  EX_PASTEBOARD_SET_DATA                     -624    // PA_SetPasteboardData
#define  EX_GET_DRAG_AND_DROP_PASTEBOARD            -631    // PA_GetDragAndDropPasteboard
#define  EX_GET_DRAG_AND_DROP_VARIABLE              -635    // PA_GetDragAndDropVariable
#define  EX_EXECUTE_COMMAND_BY_ID                   -636    // PA_ExecuteCommandByID    (ThreadSafe)
#define  EX_LOCALIZE_STRING                         -637    // PA_LocalizeStringByID, PA_LocalizeString
#define  EX_DUPLICATE_PICTURE                       -654    // PA_DuplicatePicture     (ThreadSafe)
#define  EX_CREATE_ELEMENTS                         -655    // PA_CreateElementsFromXMLDefinition
#define  EX_GET_POINTER_VALUE_PROPERTIES            -662    // PA_GetPointerValueProperties
#define  EX_RUN_IN_MAIN_PROCESS                     -663    // PA_RunInMainProcess
#define  EX_GET_PICTURE_DATA                        -671    // PA_GetPictureData    (11.3) (ThreadSafe)
#define  EX_MODIFY_METHOD                           -672    // PA_ModifyMethod
#define  EX_CONVERT_CHARSET_TO_CHARSET              -675    // PA_ConvertFromCharset (11.3) (ThreadSafe)
#define  EX_SET_OBJ_VALUE                           -701    // PA_SetObjectProperty  (v17)   (ThreadSafe)
                                                            // PA_SetCollectionProperty (v17) 
#define  EX_GET_OBJ_VALUE                           -702    // PA_GetObjectProperty  (v17)   (ThreadSafe)
                                                            // PA_GetCollectionProperty (v17) 
#define  EX_COPY_VARIABLE                           -703    // PA_CopyVariable (v17)    (ThreadSafe)
#define  EX_REDRAW_AREA                             -714    // PA_RedrawArea (14.0)
#define  EX_GET_AREA_PORT_BOUNDS                    -715    // PA_GetAreaPortBounds (14.0)
#define  EX_GET_AREA_PORT_BOUNDS                    -715    // PA_GetAreaPortBounds (14.0)
#define EX_GET_MAIN_MDI_WINDOW                      -719    // PA_GetMainWindowHWND (16.0)

#endif
