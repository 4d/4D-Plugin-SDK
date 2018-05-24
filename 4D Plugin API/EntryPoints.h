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

//		 EntryPoint											// used in :													ThreadSafe:
// =================================================================================================================================================												
#define  EX_CREATE_RECORD							-1		// PA_CreateRecord											|| false
#define  EX_SAVE_RECORD								-2		// PA_SaveRecord											|| false
#define  EX_NEXT_RECORD								-3		// PA_NextRecord											|| false
#define  EX_PREVIOUS_RECORD							-4		// PA_PreviousRecord										|| false
#define  EX_GOTO_SELECTED_RECORD					-5		// PA_GotoSelectedRecord									|| false
#define  EX_RECORDS_IN_SELECTION					-6		// PA_RecordsInSelection									|| false
#define  EX_DELETE_SELECTION						-7		// PA_DeleteSelection										|| false
#define  EX_FIRST_RECORD							-8		// PA_FirstRecord											|| false
#define  EX_COUNT_TABLES							-9		// PA_CountTables											|| false
#define  EX_COUNT_FIELDS							-10		// PA_CountFields (PA_UseVirtualStructure mode)				|| false
#define  EX_COUNT_INVISIBLE_FIELDS_TOO				-717	// PA_CountFields (PA_UseRealStructure mode)				|| false
#define  EX_GET_TABLE_NAME							-11		// PA_GetTableName, PA_IsTableVisible						|| false
#define  EX_FIELD_ATTRIBUTES						-12		// PA_GetFieldName, PA_GetFieldProperties					|| false
#define  EX_ORDER_BY								-13		// PA_CloseOrderBy, PA_OrderBy, PA_OrderByDialog			|| false
#define  EX_QUERY									-14		// PA_CloseQuery, PA_QueryDialog							|| false
#define  EX_BEFORE_SELECTION						-15		// PA_BeforeSelection										|| false
#define  EX_END_SELECTION							-16		// PA_EndSelection											|| false
#define  EX_GET_FIELD								-17		// PA_GetXXXField											|| false
#define  EX_SET_FIELD								-18		// PA_SetXXXField											|| false
#define  EX_GET_VARIABLE							-19		// PA_GetVariable											|| false
#define  EX_SET_VARIABLE							-20		// PA_SetVariable											|| false
	// 21, 22, 23 obsoletes																								
#define  EX_GET_EXPORT_FILTER						-24		// PA_GetExportFilter										|| false
#define  EX_GET_IMPORT_FILTER						-25		// PA_GetImportFilter										|| false
#define  EX_GET_PACKED_RECORD						-26		// PA_GetPackedRecord										|| false
#define  EX_SET_PACKED_RECORD						-27		// PA_SetPackedRecord										|| false
#define  EX_CREATE_SET								-28		// PA_CreateSet												|| false
#define  EX_USE_SET									-29		// PA_UseSet												|| false
#define  EX_EXECUTE_METHOD							-30		// PA_ExecuteMethod											|| false
#define  EX_EXECUTE_FUNCTION						-31		// PA_ExecuteFunction										|| false
#define  EX_CREATE_METHOD							-32		// PA_CreateMethod											|| false
	// 31->38 unused																									
#define  EX_LOCKED									-40		// PA_Locked												|| false
#define  EX_READ_WRITE								-41		// PA_ReadWrite, PA_ReadOnly								|| false
#define  EX_LOAD_RECORD								-42		// PA_LoadRecord											|| false
#define  EX_RELATE_ONE								-43		// PA_RelateOne												|| false
#define  EX_RELATE_MANY								-44		// PA_RelateMany											|| false
	// 45, 46 obsolete																									
#define  EX_STRING									-47		// PA_FormatString											|| false
#define  EX_POPUP_FIELD_LIST						-48		// PA_FieldPopup											|| false
#define  EX_POPUP_TABLE_LIST						-49		// PA_TableAndFieldPopup									|| false
#define  EX_GET_TABLE_FIELD_NUMBER					-50		// PA_GetTableAndFieldNumbers								|| false
#define  EX_SELECTED_RECORD_NUMBER					-51		// PA_SelectedRecordNumber									|| false
	// 52->55 : hotlinks, obsoletes																						|| false
#define  EX_GET_INFORMATION							-56		// PA_Get4DVersion, PA_IsDemoVersion, PA_IsDatabaseLocked, 	|| false
															// PA_IsCompiled, PA_Is4DClient, PA_Is4DServer				
															// PA_IsWebProcess, PA_GetMessagesStatus					
#define  EX_MESSAGES								-57		// PA_SetMessagesStatus										|| false
	// 58, 59 : hotlinks, obsoletes																						
#define  EX_COMPARE_MACSTRINGS						-60		// PA_CompareMacStrings										|| false
#define  EX_EVAL_NUMBER								-61		// PA_EvalReal, PA_EvalLongint								|| false
	// 62, 63 : hotlinks, obsoletes																						
#define  EX_FIND_PACKAGE							-64		// PA_FindPackage											|| false
#define  EX_UPDATE_VARIABLES						-65		// PA_UpdateVariables										|| false
#define  EX_CHANGE_TITLE							-66		// PA_SetPluginWindowTitle									|| false
#define  EX_OPEN_PLUGIN_WINDOW						-67		// PA_OpenPluginWindow										|| false
#define  EX_CLOSE_PLUGIN_WINDOW						-68		// PA_ClosePluginWindow										|| false
	// 69 deprecated																									
	// 70, 71, 72 : hotlinks, obsoletes																					
#define  EX_RECORD_NUMBER							-73		// PA_RecordNumber											|| false
#define  EX_ADD_TO_SET								-74		// PA_AddToSet												|| false
#define  EX_CREATE_EMPTY_SET						-75		// PA_CreateEmptySet										|| false
#define  EX_DELETE_SET								-76		// PA_DeleteSet												|| false
	// 77 obsolete																										
	// 78 hotlinks, obsolete																							
	// 79 obsolete																										
#define  EX_GET_CURRENT_PAGE						-80		// PA_GetCurrentPage										|| false
#define  EX_GET_CURRENT_TEHANDLE					-81		// PA_GetCurrentTEHandle									|| false
#define  EX_TOKENIZE								-82		// PA_Tokenize												|| false
#define  EX_EXEC_TOKEN_PROC							-83		// PA_ExecuteTokens											|| false
#define  EX_EXEC_TOKEN_FUNC							-84		// PA_ExecuteTokensAsFunction								|| false
	// 85 obsolete																										
#define  EX_KILL_PROCESS							-86		// PA_KillProcess											|| false
#define  EX_FREEZE_PROCESS							-87		// PA_FreezeProcess											|| false
#define  EX_UNFREEZE_PROCESS						-88		// PA_UnfreezeProcess										|| false
#define  EX_PUT_PROCESS_TO_SLEEP					-89		// PA_PutProcessToSleep										|| false
#define  EX_GET_WINDOW_PROCESS						-90		// PA_GetWindowProcess, PA_GetFrontWindowProcess			|| false
#define  EX_CURRENT_PROCESS_NUMBER					-91		// PA_GetCurrentProcessNumber								|| false
#define  EX_SET_WINDOW_PROCESS						-92		// PA_SetWindowProcess										|| false
#define  EX_YIELD									-93		// PA_Yield													|| false
#define  EX_GET_PROCESS_INFO						-94		// PA_GetProcessInfo										|| false
#define  EX_NB_PROCESS								-95		// PA_CountActiveProcess									|| false
#define  EX_GOTO_RECORD								-96		// PA_GotoRecord											|| false
#define  EX_BRING_PROCESS_TO_FRONT					-97		// PA_BringProcessToFront									|| false
	// 98,99 unused																										
#define  EX_READ_DOCUMENT							-100	// PA_ReceiveDocument										|| false
#define  EX_SAVE_DOCUMENT							-101	// PA_SendDocument											|| false
#define  EX_READ_DATA								-102	// PA_ReceiveDataFromServer									|| false
#define  EX_WRITE_DATA								-103	// PA_SendDataToServer										|| false
	// 104, 105, 106 obsolete																							
#define  EX_LOCK_DATABASE							-107	// PA_LockDatabase											|| false
#define  EX_UNLOCK_DATABASE							-108	// PA_UnlockDatabase										|| false
	// 109,110,111 reserved																								
#define  EX_GET_USER_LIST							-112	// PA_CountUsers											|| false
	// 113, 114 reserved																								
#define  EX_GET_PREF_FILE							-115	// PA_Get4DPreferencesFilePath								|| false
	// 116, 117 obsolete																								
#define  EX_CALL_BY_PROCID							-118	// PA_ExecuteMethodByID										|| false
	// 119, 120 obsolete																								
#define  EX_THERMOMETER								-121	// PA_SetThermometerRect									|| false
#define  EX_YIELD_ABSOLUTE							-122	// PA_YieldAbsolute											|| false
	// 123, 124 reserved																								
#define  EX_GET_INTERPROCESS_VARIABLE				-125	// PA_GetVariable											|| false
#define  EX_SET_INTERPROCESS_VARIABLE				-126	// PA_SetVariable											|| false
#define  EX_EXIST_DOCUMENT_ON_SERVER				-127	// PA_DocumentExistOnServer									|| false
	// 128 reserved																										
#define  EX_GET_PRINT_INFO							-130	// PA_GetWindowsPRINTDLG, PA_GetWindowsPrintingDC			|| false
															// PA_GetCarbonPageFormat, PA_GetCarbonPrintSettings		
#define  EX_GET_FIELD_RELATIONS						-131	// PA_GetFieldRelation										|| false
	// 132 reserved																										
	// 133, 134 obsolete																								
#define  EX_UPDATE_PROCESS_VARIABLE					-135	// PA_UpdateProcessVariable									|| false
#define  EX_QUIT4D									-136	// PA_Quit4D												|| false
	// 137->144 obsolete																								
#define  EX_GET_GROUP_LIST							-145	// PA_CountUserGroups										|| false
	// 146, 144, 149->157  obsolete																						
#define  EX_NEW_PROCESS								-158	// PA_NewProcess											|| false
	// 159 deprecated																									
	// 160 reserved																										
#define  EX_GET_HWND								-161	// PA_GetHWND, PA_GetHDC									|| false
#define  EX_BYTE_SWAP_TOKENS						-162	// PA_ByteSwapTokens										|| false
	// 163	obsolete																									
#define  EX_GET_PLATFORM_INTERFACE					-164	// PA_GetPlatformInterface									|| false
#define  EX_POST_EVENT								-165	// PA_PostEvent												|| false
	// 166 reserved																										
	// 167, 168 obsolete																								
#define  EX_PACKAGE_INFO							-169	// PA_PackageInfo											|| false
#define  EX_START_WEB_SERVER						-172	// PA_StartWebServer										|| false
#define  EX_STOP_WEB_SERVER							-173	// PA_StopWebServer											|| false
#define  EX_GET_WEB_SERVER_INFO						-174	// PA_GetWebServerInfo										|| false
#define  EX_SET_WEB_TCP_PORT						-175	// PA_SetWebTCPport											|| false
#define  EX_GET_WEB_CONTEXT							-176	// PA_GetWebContext											|| false
#define  EX_WEB_TIMEOUT								-177	// PA_GetWebTimeOut, PA_SetWebTimeOut						|| false
#define  EX_GET_TOOLBAR_INFO						-178	// PA_GetToolBarInfo										|| false
#define  EX_SHOW_HIDE_TOOLBAR						-179	// PA_ShowHideToolBar										|| false
#define  EX_DRAG_AND_DROP							-180	// PA_DragAndDrop											|| false
	// 181, 182 obsolete																								
#define  EX_INSTALL_4DWRITE_SPELLER					-183	// PA_Install4DWriteSpeller									|| false
#define  EX_GET_4DWRITE_SPELLER						-184	// PA_Get4DWriteSpellerProcPtr								|| false
#define  EX_DETOKENIZE								-185	// PA_Detokenize											|| false
	// 186 reserved																										
	// 187->199 unused																									
#define  EX_DIAL4D_NEW_EMPTY_DIALOG					-200	// PA_NewDialog												|| false
#define  EX_DIAL4D_OPEN_DIALOG						-201	// PA_OpenDialog											|| false
#define  EX_DIAL4D_CLOSE_DIALOG						-202	// PA_CloseDialog											|| false
#define  EX_DIAL4D_MODAL_DIALOG						-203	// PA_ModalDialog											|| false
#define  EX_DIAL4D_SET_ENABLE						-204	// PA_Dial4DSetEnable										|| false
#define  EX_DIAL4D_GET_VARIABLE						-205	// PA_Dial4DGetVariable										|| false
#define  EX_DIAL4D_SET_VARIABLE						-206	// PA_Dial4DSetVariable										|| false
#define  EX_DIAL4D_GET_LONG							-207	// PA_Dial4DGetLong											|| false
#define  EX_DIAL4D_SET_LONG							-208	// PA_Dial4DSetLong											|| false
#define  EX_DIAL4D_GET_REAL							-209	// PA_Dial4DGetReal											|| false
#define  EX_DIAL4D_SET_REAL							-210	// PA_Dial4DSetReal											|| false
#define  EX_DIAL4D_GET_STRING						-211	// PA_Dial4DGetString										|| false
#define  EX_DIAL4D_SET_STRING						-212	// PA_Dial4DSetString										|| false
#define  EX_DIAL4D_SET_PICTURE						-213	// PA_Dial4DSetPictureHandle								|| false
	// 214 unused																										
#define  EX_DIAL4D_SAVE_VARIABLES					-215	// PA_Dial4DSaveVariables									|| false
#define  EX_DIAL4D_RESTORE_VARIABLES				-216	// PA_Dial4DRestoreVariables								|| false
#define  EX_DIAL4D_GET_ARRAY_REAL					-217	// PA_Dial4DGetArrayReal									|| false
#define  EX_DIAL4D_GET_ARRAY_STRING					-218	// PA_Dial4DGetArrayString									|| false
#define  EX_DIAL4D_GET_ARRAY_LONG					-219	// PA_Dial4DGetArrayLong									|| false
#define  EX_ABOUT_DIALOG							-220	// PA_AboutDialog											|| false
#define  EX_DIAL4D_SET_AREA_HANDLER					-221	// PA_Dial4DSetAreaHandler									|| false
#define  EX_DIAL4D_GET_VARIABLE_RECT				-222	// PA_Dial4DGetVariableRect									|| false
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_RESOURCE		-223	// PA_Dial4DSetArrayTextFromResource						|| false
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_TABLE_LIST	-224	// PA_Dial4DSetArrayTextFromTableList						|| false
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_FIELD_LIST	-225	// PA_Dial4DSetArrayTextFromFieldList						|| false
#define  EX_DIAL4D_BEGIN_UPDATE_VARIABLES			-226	// PA_Dial4DBeginUpdateVariables							|| false
#define  EX_DIAL4D_END_UPDATE_VARIABLES				-227	// PA_Dial4DEndUpdateVariables								|| false
#define  EX_DIAL4D_NEW_ARRAY_STRING					-228	// PA_Dial4DNewArrayString									|| false
#define  EX_DIAL4D_SET_ARRAY_STRING					-229	// PA_Dial4DSetArrayString									|| false
#define  EX_DIAL4D_SET_DATE							-230	// PA_Dial4DSetDate											|| false
#define  EX_DIAL4D_GET_DATE							-231	// PA_Dial4DGetDate											|| false
#define  EX_DIAL4D_SHOW_HIDE_VARIABLE				-232	// PA_Dial4DShowHideVariable								|| false
#define  EX_DIAL4D_GOTO_PAGE						-233	// PA_Dial4DGotoPage										|| false
#define  EX_DIAL4D_GET_CURRENT_PAGE					-234	// PA_Dial4DGetCurrentPage									|| false
#define  EX_DIAL4D_NEW_TABLE_FIELD_HLIST			-235	// PA_Dial4DNewTableFieldHList								|| false
#define  EX_DIAL4D_DISPOSE_HLIST					-236	// PA_Dial4DDisposeHList									|| false
#define  EX_DIAL4D_GET_DRAG_INFO					-237	// PA_Dial4DGetDragInfo										|| false
#define  EX_DIAL4D_GET_TABLE_FIELD_HLIST_CURRENT	-238	// PA_Dial4DGetTableFieldHListCurrent						|| false
#define  EX_DIAL4D_NEW_ARRAY_LONG					-239	// PA_Dial4DNewArrayLong									|| false
#define  EX_DIAL4D_SET_ARRAY_LONG					-240	// PA_Dial4DSetArrayLong									|| false
#define  EX_DIAL4D_SET_ARRAY_TEXT_FROM_TTR			-241	// PA_Dial4DSetArrayTextFromTTR								|| false
#define  EX_DIAL4D_GET_ARRAY_SIZE					-242	// PA_Dial4DGetArraySize									|| false
#define  EX_DIAL4D_DELETE_ARRAY_ELEMENTS			-243	// PA_Dial4DDeleteArrayElements								|| false
#define  EX_DIAL4D_REDRAW_EXTERNAL_AREA				-244	// PA_Dial4DRedrawExternalArea								|| false
#define  EX_DIAL4D_NEW_ARRAY_PICTURE				-245	// PA_Dial4DNewArrayPicture									|| false
#define  EX_DIAL4D_SET_ARRAY_PICTURE				-246	// PA_Dial4DSetArrayPicture, 								|| false
															// PA_Dial4DSetArrayPictureHandle							
#define  EX_DIAL4D_SET_ARRAY_PICT_FROM_RESOURCES	-247	// PA_Dial4DSetArrayPictureFromResources					|| false
#define  EX_FORMULA_EDITOR							-248	// PA_FormulaEditor											|| false
#define  EX_ALERT									-249	// PA_Alert													|| false
#define  EX_CONFIRM									-250	// PA_Confirm												|| false
#define  EX_DIAL4D_SET_ENTERABLE					-251	// PA_Dial4DSetEnterable									|| false
#define  EX_DIAL4D_SET_MIN							-252	// PA_Dial4DSetMin, PA_Dial4DSetMinDate						|| false
#define  EX_DIAL4D_SET_MAX							-253	// PA_Dial4DSetMax, PA_Dial4DSetMaxDate						|| false
#define  EX_PICTURE_CONVERSION						-254	// PA_PictureToJPEG, PA_PictureToGIF, PA_PictureToEMF		|| false
#define  EX_CHECK_FREE_STACK						-255	// PA_CheckFreeStack										|| false
#define  EX_GET_TIPS_ENABLED						-256	// PA_GetTipsEnabled										|| false
#define  EX_SET_TIPS_ENABLED						-257	// PA_SetTipsEnabled										|| false
#define  EX_DIAL4D_SET_USER_DATA					-258	// PA_Dial4DSetUserData										|| false
#define  EX_DIAL4D_GET_USER_DATA					-259	// PA_Dial4DGetUserData										|| false
#define  EX_DIAL4D_SET_3_STATES_CHECKBOX			-260	// PA_Dial4DSet3StatesCheckBox								|| false
#define  EX_DIAL4D_DIAL4D_FROM_WINDOW				-261	// PA_Dial4DDial4DFromWindow								|| false
#define  EX_DIAL4D_WINDOW_FROM_DIAL4D				-262	// PA_Dial4DWindowFromDial4D								|| false
#define  EX_DIAL4D_SET_TABLE_FIELD_HLIST_CURRENT	-263	// PA_Dial4DSetTableFieldHListCurrent						|| false
#define  EX_OPEN_URL								-264	// PA_OpenURL												|| false
	// 265 to 299 : unused																								
	// 300 obsolete																										
#define  EX_VIRTUAL_STRUCTURE_DEFINED				-301	// PA_VirtualStructureDefined								|| false
#define  EX_GET_TRUE_TABLE_NUMBER					-302	// PA_GetTrueTableNumber									|| false
#define  EX_GET_TRUE_FIELD_NUMBER					-303	// PA_GetTrueFieldNumber									|| false
	// 304 : reserved																									
#define  EX_CREATE_TIP								-305	// PA_CreateTip												|| false
#define  EX_GOTO_AREA								-306	// PA_GotoArea												|| false
#define  EX_UPDATE_EDIT_MENU						-307	// PA_UpdateEditMenu										|| false
#define  EX_DIAL4D_SET_TEXT							-308	// PA_Dial4DSetText											|| false
#define  EX_DIAL4D_GET_TEXT							-309	// PA_Dial4DGetText											|| false
#define  EX_CREATE_RESOURCE							-310	// PA_CreateResource, PA_CreateResourceFromHandle			|| false
#define  EX_REMOVE_RESOURCE							-311	// PA_RemoveResourceByID,PA_RemoveResourceByName			|| false
#define  EX_GET_RESOURCE							-312	// PA_GetResource, PA_SetResource, 							|| false
															// PA_LockResource, PA_GetResourceHandle					
#define  EX_GET_INDEXED_RESOURCE					-313	// PA_GetIndexedResource									|| false
#define  EX_RELEASE_RESOURCE						-314	// PA_ReleaseResourceHandle, PA_ReleaseResource				|| false
#define  EX_DETACH_RESOURCE							-315	// PA_DetachResource										|| false
#define  EX_WRITE_RESOURCE							-316	// PA_SetResource											|| false
#define  EX_COUNT_RESOURCES							-317	// PA_CountResources										|| false
#define  EX_GET_RESOURCE_INFO						-318	// PA_GetResourceName, PA_GetResourceSize					|| false
#define  EX_SET_RESOURCE_NAME						-319	// PA_SetResourceName										|| false
#define  EX_COUNT_RESOURCE_KINDS					-320	// PA_CountResourceKinds									|| false
#define  EX_GET_INDEXED_RESOURCE_KIND				-321	// PA_GetIndexedResourceKind								|| false
#define  EX_LOCK_RESOURCE							-322	// PA_LockResourceHandle									|| false
#define  EX_SET_RES_LOAD							-323	// PA_SetResLoad											|| false
#define  EX_GET_RESOURCE_ID_LIST					-324	// PA_GetResourceIDList										|| false
#define  EX_GET_RESOURCE_NAME_LIST					-325	// PA_GetResourceNameList									|| false
#define  EX_GET_RESOURCE_KIND_LIST					-326	// PA_GetResourceKindList									|| false
#define  EX_DIAL4D_NEW_ARRAY_REAL					-327	// PA_Dial4DNewArrayReal									|| false
#define  EX_DIAL4D_SET_ARRAY_REAL					-328	// PA_Dial4DSetArrayReal									|| false
#define  EX_DIAL4D_SET_ENTRY_FILTER					-329	// PA_Dial4DSetEntryFilter									|| false
#define  EX_DIAL4D_SET_DISPLAY_FORMAT				-330	// PA_Dial4DSetDisplayFormat								|| false
#define  EX_DIAL4D_NEW_ARRAY_TEXT					-331	// PA_Dial4DNewArrayText									|| false
#define  EX_DIAL4D_SET_ARRAY_TEXT					-332	// PA_Dial4DSetArrayText									|| false
#define  EX_DIAL4D_GET_ARRAY_TEXT					-333	// PA_Dial4DGetArrayText									|| false
// 334 deprecated																										
#define  EX_GET_APPLICATION_FULLPATH				-335	// PA_GetApplicationFullPath								|| false
#define  EX_DIAL4D_DISPOSE_EMPTY_DIALOG				-336	// PA_Dial4DDisposeEmptyDialog								|| false
#define  EX_DIAL4D_SHOW_HIDE_OBJECT					-337	// PA_Dial4DShowHideObject									|| false
#define  EX_DIAL4D_SET_TIME							-338	// PA_Dial4DSetTime											|| false
#define  EX_DIAL4D_GET_TIME							-339	// PA_Dial4DGetTime											|| false
#define  EX_DIAL4D_SET_OBJECT_TITLE					-340	// PA_Dial4DSetObjectTitle									|| false
#define  EX_DIAL4D_UPDATE_OBJECT_TITLE				-341	// PA_Dial4DUpdateObjectTitle								|| false
#define  EX_OPEN_RESFILE							-342	// PA_OpenResFile											|| false
#define  EX_CLOSE_RESFILE							-343	// PA_CloseResFile											|| false
	// 344,345 unused 																									|| false
#define  EX_CREATE_RESFILE							-346	// PA_CreateResFile											|| false
#define  EX_GET_DATABASE_RESFILE					-347	// PA_GetDatabaseResFile									|| false
#define  EX_UPDATE_RESFILE							-348	// PA_UpdateResFile											|| false
	// 349->353 reserved																								
#define  EX_COPY_CUT_NAMED_SELECTION				-354	// PA_CopyNamedSelection, PA_CutNamedSelection				|| false
#define  EX_USE_NAMED_SELECTION						-355	// PA_UseNamedSelection										|| false
#define  EX_CLEAR_NAMED_SELECTION					-356	// PA_ClearNamedSelection									|| false
#define  EX_NEXT_WINDOW_LEVEL						-357	// PA_NewWindow												|| false
	// 358->360 reserved																								
#define  EX_WAIT_NEXT_EVENT							-361	// PA_WaitNextEvent											|| false
	// 362->378 unused																									
#define  EX_NEW_WINDOW								-379	// PA_NewWindow												|| false
#define  EX_CLOSE_WINDOW							-380	// PA_CloseWindow											|| false
	// 381 reserved																										
#define  EX_GET_VIRTUAL_TABLE_NUMBER				-382	// PA_GetVirtualTableNumber									|| false
#define  EX_GET_VIRTUAL_FIELD_NUMBER				-383	// PA_GetVirtualFieldNumber									|| false
#define  EX_USE_RESFILE								-384	// PA_UseResFile()											|| false
	// 385->391 reserved																								
#define  EX_GET_UNIQUE_RESID						-392	// PA_GetUniqueResID										|| false
#define  EX_DIAL4D_SET_VARIABLE_RECT				-393	// PA_Dial4DSetVariableRect									|| false
#define  EX_DIAL4D_ALLOW_X_RESIZE					-394	// PA_Dial4DAllowXResize									|| false
#define  EX_DIAL4D_ALLOW_Y_RESIZE					-395	// PA_Dial4DAllowYResize									|| false
#define  EX_DIAL4D_GET_WINDOW_MIN_MAX_INFO			-396	// PA_Dial4DGetWindowMinMaxInfo								|| false
#define  EX_DIAL4D_SET_WINDOW_SIZE					-397	// PA_Dial4DSetWindowSize									|| false
#define  EX_DIAL4D_FIND_ARRAY_LONG					-398	// PA_Dial4DFindArrayLong									|| false
#define  EX_DIAL4D_FIND_ARRAY_REAL					-399	// PA_Dial4DFindArrayReal									|| false
#define  EX_GET_CURRENT_USER_ID						-400	// PA_GetCurrentUserID										|| false
	// 401 reserved																										
#define  EX_PICTURE_EDITOR							-402	// PA_PictureEditor											|| false
#define  EX_GET_USER_NAME							-403	// PA_GetUserName											|| false
	// 404 reserved																										
#define  EX_DIAL4D_GET_LAST_OBJECT					-405	// PA_Dial4DGetLastObject									|| false
	// 406 reserved																										
#define  EX_REQUEST									-407	// PA_Request												|| false
	// 408,409 reserved																									
#define  EX_CLEAR_VARIABLE							-410	// PA_ClearVariable											|| true
#define  EX_GET_STRUCTURE_FULLPATH					-411	// PA_GetStructureFullPath									|| false
#define  EX_RELATE_ONE_SELECTION					-412	// PA_RelateOneSelection									|| false
#define  EX_RELATE_MANY_SELECTION					-413	// PA_RelateManySelection									|| false
#define  EX_IS_PROCESS_DYING						-414	// PA_IsProcessDying										|| false
	// 415 reserved																										|| false
#define  EX_DIAL4D_GOTO_VARIABLE					-416	// PA_Dial4DGotoVariable									|| false
#define  EX_GET_RESOURCE_TIMESTAMP					-417	// PA_GetResourceTimeStamp									|| false
#define  EX_SET_WINDOW_FOCUSABLE					-418	// PA_SetWindowFocusable									|| false
#define  EX_IS_WINDOW_FOCUSABLE						-419	// PA_IsWindowFocusable										|| false
#define  EX_GET_WINDOW_FOCUSED						-420	// PA_GetWindowFocused										|| false
#define  EX_SET_WINDOW_FOCUSED						-421	// PA_SetWindowFocused										|| false
#define  EX_IS_WINDOW_FOCUSED						-422	// PA_IsWindowFocused										|| false
#define  EX_METHOD_EDITOR							-423	// PA_MethodEditor											|| false
#define  EX_CONVERT_STRING							-424	// PA_ConvertString											|| false
#define  EX_DIAL4D_CANCEL_VALIDATE					-425	// PA_Dial4DCancelValidate									|| false
	// 426	obsolete																									
#define  EX_GET_4D_FOLDER							-427	// PA_Get4DFolder											|| false
#define  EX_DIAL4D_HIGHLIGHT_TEXT					-428	// PA_Dial4DHighlightText									|| false
#define  EX_GET_SERIAL_KEY							-429	// PA_GetSerialKey											|| false
#define  EX_GET_REGISTRATION_INFO					-430	// PA_GetRegistredUserName									|| false
#define  EX_GET_USERS_INFO							-431	// PA_CountConnectedUsers									|| false
#define  EX_GET_4D_HINSTANCE						-432	// PA_Get4DHInstance										|| false
	// 433 reserved																										
#define  EX_HANDLE_MANAGER							-434	// PA_NewHandle, PA_DisposeHandle,							|| false
															// PA_SetHandleSize, PA_GetHandleSize,						|| false
															// PA_SetHandleState, PA_GetHandleState,					|| false
															// PA_LockHandle, PA_UnlockHandle							|| false
	// 435,436,437 reserved																								
#define  EX_RESIZE_ARRAY							-438	// PA_ResizeArray											|| false
#define  EX_UNLOCK_RESOURCE							-439	// PA_UnlockResource, PA_UnlockResourceHandle				|| false
#define  EX_METHOD_NAMES							-442	// PA_MethodNames											|| false
																														
#define  EX_GET_COMMAND_ID							-506	// PA_GetCommandID											|| false
#define  EX_GET_COMMAND_NAME						-507	// PA_GetCommandName										|| false
#define  EX_GET_METHOD_ID							-508	// PA_GetMethodID											|| false
#define  EX_GET_CENTURY_INFO						-531	// PA_GetCenturyInfo										|| false
#define  EX_OPEN_PRINTER_SESSION					-556	// PA_OpenPrinterSession									|| false
#define  EX_CLOSE_PRINTER_SESSION					-557	// PA_ClosePrinterSession									|| false
#define  EX_TRY_TO_OPEN_PRINTER_SESSION				-558	// PA_TryToOpenPrinterSession								|| false
#define  EX_GET_TEMPLATE							-593	// PA_GetTemplate											|| false
																														
#define  EX_GET_POINTER_VALUE						-585	// PA_GetPointerValue										|| false
#define  EX_SET_POINTER_VALUE						-586	// PA_SetPointerValue										|| false
																														
#define  EX_SET_PLUGINAREA_CLIPMODE					-597	// PA_SetPluginAreaClipMode									|| false
																														
#define  EX_COMPARE_UNIBUFFERS						-611	// PA_CompareUniBuffers										|| true
#define  EX_CREATE_UNISTRING						-612	// PA_CreateUniString										|| true
#define  EX_SET_UNISTRING							-613	// PA_SetUniString											|| true
#define  EX_DISPOSE_UNISTRING						-614	// PA_DisposeUniString										|| true
#define  EX_VARIABLE_TO_STRING						-615	// PA_GetStringVariable										|| true
#define  EX_GET_PLUGIN_KEY_EVENT					-616	// PA_GetKey												|| false
																														
#define  EX_CREATE_PICTURE							-617	// PA_CreatePicture											|| false
#define  EX_CREATE_NATIVE_PICTURE_FOR_SCREEN		-618	// PA_CreateNativePictureForScreen							|| false
#define  EX_CREATE_NATIVE_PICTURE_FOR_PRINTING		-619	// PA_CreateNativePictureForPrinting						|| false
#define  EX_DISPOSE_PICTURE							-620	// PA_DisposePicture										|| false
#define  EX_PASTEBOARD_IS_DATA_AVAILABLE			-621	// PA_IsPasteboardDataAvailable								|| false
#define  EX_PASTEBOARD_GET_DATA						-622	// PA_GetPasteboardData										|| false
#define  EX_PASTEBOARD_GET_DATA_SIZE				-623	// PA_GetPasteboardDataSize									|| false
#define  EX_PASTEBOARD_SET_DATA						-624	// PA_SetPasteboardData										|| false
#define  EX_GET_DRAG_AND_DROP_PASTEBOARD			-631	// PA_GetDragAndDropPasteboard								|| false
#define  EX_GET_DRAG_AND_DROP_VARIABLE				-635	// PA_GetDragAndDropVariable								|| false
#define  EX_EXECUTE_COMMAND_BY_ID					-636	// PA_ExecuteCommandByID									|| depends on the commands called
#define  EX_LOCALIZE_STRING							-637	// PA_LocalizeStringByID, PA_LocalizeString					|| false
#define  EX_DUPLICATE_PICTURE						-654	// PA_DuplicatePicture										|| false
#define  EX_CREATE_ELEMENTS							-655	// PA_CreateElementsFromXMLDefinition						|| false
#define	 EX_GET_POINTER_VALUE_PROPERTIES			-662	// PA_GetPointerValueProperties								|| false
#define  EX_RUN_IN_MAIN_PROCESS						-663	// PA_RunInMainProcess										|| false
#define  EX_GET_PICTURE_DATA						-671	// PA_GetPictureData	(11.3)								|| false
#define  EX_MODIFY_METHOD							-672	// PA_ModifyMethod											|| false
	// -673,-674 reserved																								
#define  EX_CONVERT_CHARSET_TO_CHARSET				-675	// PA_ConvertFromCharset	March 2nd 2009   11.3			|| false
#define  EX_SET_OBJ_VALUE							-701	// PA_SetObjectProperty | PA_SetCollectionProperty (v17)	|| true
#define  EX_GET_OBJ_VALUE							-702    // PA_GetObjectProperty | PA_GetCollectionProperty (v17)	|| true
#define	 EX_COPY_VARIABLE							-703	// PA_CopyVariable (v17)									|| true
#define  EX_REDRAW_AREA								-714	// PA_RedrawArea (14.0)										|| false
#define  EX_GET_AREA_PORT_BOUNDS					-715	// PA_GetAreaPortBounds (14.0)								|| false
																														
#define  EX_GET_AREA_PORT_BOUNDS					-715	// PA_GetAreaPortBounds (14.0)								|| false
																														
#define EX_GET_MAIN_MDI_WINDOW						-719	// PA_GetMainWindowHWND (16.0)								|| false

#endif
