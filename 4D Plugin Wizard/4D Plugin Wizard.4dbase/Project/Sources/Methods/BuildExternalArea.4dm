//%attributes = {}
$2->:=$2->+ColorizeText ("void ";gKeyWordColor)
$2->:=$2->+($1+"( PA_PluginParameters params )"+vcr+"{"+vcr)

$3->:=$3->+ColorizeText ("void ";gKeyWordColor)
$3->:=$3->+($1+"( PA_PluginParameters params );"+vcr)


$2->:=$2->+put (vtab+"PA_AreaEvent event;")

$2->:=$2->+put (vtab+"PA_PluginProperties props;")
$2->:=$2->+kw (vtab+"void")
$2->:=$2->+("* privateData;"+vcr)

If (([ExternalAreas:5]UpdateEvt:9) | ([ExternalAreas:5]DesignUpdateEvt:16) | ([ExternalAreas:5]Dropable:6))
	$2->:=$2->+put (vtab+"PA_Rect rect;")
End if 

If (([ExternalAreas:5]MouseDownEvt:10) | ([ExternalAreas:5]Dropable:6) | (([ExternalAreas:5]WebEvents:13>2) & [ExternalAreas:5]WebImageMap:14))
	$2->:=$2->+kw (vtab+"short")
	$2->:=$2->+(" x, y;"+vcr)
End if 

If ([ExternalAreas:5]KeyDownEvt:11)
	$2->:=$2->+(vtab+"PA_Unichar key;"+vcr)
	$2->:=$2->+(vtab+"PA_KeyCode keycode;"+vcr)
	$2->:=$2->+kw (vtab+"char")
	$2->:=$2->+(vtab+" altkey;"+vcr)
	$2->:=$2->+kw (vtab+"char")
	$2->:=$2->+(vtab+" ctrlkey;"+vcr)
	$2->:=$2->+kw (vtab+"char")
	$2->:=$2->+(vtab+" shiftkey;"+vcr)
End if 

If ([ExternalAreas:5]AdvancedProperties:8)
	$2->:=$2->+kw (vtab+"void")
	$2->:=$2->+("* advancedProperties;"+vcr+vtab)
	$2->:=$2->+kw ("PA_long32")
	$2->:=$2->+(" advancedPropertiesSize;"+vcr)
End if 

If ([ExternalAreas:5]Dropable:6)
	$2->:=$2->+put (vtab+"PA_DragAndDropInfo dropInfo;")
End if 

If ([ExternalAreas:5]WebEvents:13>1)
	$2->:=$2->+kw (vtab+"void")
	$2->:=$2->+("* webData;"+vcr)
	$2->:=$2->+kw (vtab+"void")
	$2->:=$2->+("* webBuffer;"+vcr+vtab)
	$2->:=$2->+kw ("PA_long32")
	$2->:=$2->+(" webBufferLen;"+vcr)
End if 

put (vcr+vtab+"event = PA_GetAreaEvent( params );"+vcr+vtab)
$2->:=$2->+kw (vtab+"switch")
$2->:=$2->+(" ( event )"+vcr+vtab+"{"+vcr)

$2->:=$2->+ColorizeText (vtab+"// --- Initialisation";gCommentColor)+vcr
$2->:=$2->+kw (vtab+vtab+"case")
$2->:=$2->+(" eAE_InitArea :"+vcr+vtab+vtab+vtab+"privateData = (")
$2->:=$2->+kw ("void")
$2->:=$2->+("*) 1234;"+vtab)
$2->:=$2->+ColorizeText ("// allocate here any private data for this instance of the area ";gCommentColor)+vcr
$2->:=$2->+(vtab+vtab+vtab+"PA_SetAreaReference( params, privateData );"+vcr)
$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)

$2->:=$2->+ColorizeText (vtab+"// --- Deinitialisation";gCommentColor)+vcr
$2->:=$2->+kw (vtab+vtab+"case")
$2->:=$2->+(" eAE_Deinit :"+vcr+vtab+vtab+vtab+"privateData = PA_GetAreaReference( params );"+vcr)
$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// free any privateData allocated at init";gCommentColor)+vcr

$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)

If ([ExternalAreas:5]Focusable:5)
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_IsFocusable :"+vcr+vtab+vtab+vtab+"PA_SetAreaFocusable( params, 1 );"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]AdvancedProperties:8)
	$2->:=$2->+ColorizeText (vtab+"// --- Advanced Properties";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_AreAdvancedPropertiesEditable :"+vcr)
	$2->:=$2->+put (vtab+vtab+vtab+"PA_SetAdvancedPropertiesEditable( params, 1 );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_InitAdvancedProperties :"+vcr+vtab+vtab+vtab+"PA_GetPluginProperties( params, &amp;props );"+vcr)
	$2->:=$2->+(vtab+vtab+vtab+"advancedProperties = PA_GetAdvancedProperties( &amp;props")
	$2->:=$2->+put (", &amp;advancedPropertiesSize );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_EditAdvancedProperties :"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// read actual Advanced Properties parameters";gCommentColor)+vcr
	
	$2->:=$2->+put (vtab+vtab+vtab+"PA_GetPluginProperties( params, &amp;props );")
	$2->:=$2->+(vtab+vtab+vtab+"advancedProperties = PA_GetAdvancedProperties( &amp;props")
	$2->:=$2->+put (", &amp;advancedPropertiesSize );")
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+vtab+"// Handle Advanced Properties dialog here";gCommentColor)+vcr
	
	put (vtab+vtab+vtab+"// Handle Advanced Properties dialog here")
	$2->:=$2->+std (vtab+vtab+vtab+"advancedProperties = 0;"+vtab)
	$2->:=$2->+ColorizeText ("// Give a pointer to your advanced properties data";gCommentColor)+vcr
	
	$2->:=$2->+(vtab+vtab+vtab+"advancedPropertiesSize = 0;"+vtab)
	$2->:=$2->+ColorizeText ("// Give the size of your advanced properties data";gCommentColor)+vcr
	
	$2->:=$2->+put (vtab+vtab+vtab+"PA_SetAdvancedProperties( params, advancedProperties, advancedPropertiesSize );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_DisposeAdvancedProperties :"+vcr)
	$2->:=$2->+(vtab+vtab+vtab+"advancedProperties = PA_GetAdvancedPropertiesToDispose( params );"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// dispose the advancedProperties pointer you pass to PA_SetAdvancedProperties";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]WebEvents:13>1)
	$2->:=$2->+ColorizeText (vtab+"// --- Web events";gCommentColor)+vcr
	
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_WebPublish :"+vcr)
	If ([ExternalAreas:5]WebEvents:13=2)
		$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Export HTML buffer";gCommentColor)+vcr
		
		$2->:=$2->+put (vtab+vtab+vtab+"PA_SendHTML( params, webData, webBuffer, webBufferLen );")
		$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	Else 
		$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// tell 4D we want to export a picture buffer";gCommentColor)+vcr
		
		If ([ExternalAreas:5]WebImageMap:14)
			$2->:=$2->+put (vtab+vtab+vtab+"PA_PublishWebPicture( params, 1 );")
		Else 
			$2->:=$2->+put (vtab+vtab+vtab+"PA_PublishWebPicture( params, 0 );")
		End if 
		$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
		
		$2->:=$2->+kw (vtab+vtab+"case")
		$2->:=$2->+(" eAE_WebPublishPicture :"+vcr)
		$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// send the picture buffer";gCommentColor)+vcr
		$2->:=$2->+(vtab+vtab+vtab+"PA_SendWebPicture( params, webData, webBuffer, webBufferLen, ")
		Case of 
			: ([ExternalAreas:5]WebEvents:13=3)
				$2->:=$2->+put ("eWP_MacintoshPicture );")
			: ([ExternalAreas:5]WebEvents:13=4)
				$2->:=$2->+put ("eWP_Gif );")
			: ([ExternalAreas:5]WebEvents:13=5)
				$2->:=$2->+put ("eWP_JPEG );")
		End case 
		$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
		
		If ([ExternalAreas:5]WebImageMap:14)
			$2->:=$2->+kw (vtab+vtab+"case")
			$2->:=$2->+(" eAE_WebClick :"+vcr)
			$2->:=$2->+put (vtab+vtab+vtab+"PA_GetClick( params, &amp;x, &amp;y );")
			$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Manage Click in Picture Map";gCommentColor)+vcr
			$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
		End if 
		
		$2->:=$2->+kw (vtab+vtab+"case")
		$2->:=$2->+(" eAE_WebDisposeData :"+vcr)
		$2->:=$2->+put (vtab+vtab+vtab+"webData = PA_GetWebDataToDispose( params );")
		$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Dispose buffers allocated for pictures or HTML";gCommentColor)+vcr
		$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	End if 
End if 

If ([ExternalAreas:5]DesignUpdateEvt:16)
	  //CommentColor 
	$2->:=$2->+ColorizeText (vtab+"// --- Update in layout editor";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_DesignUpdate :"+vcr)
	$2->:=$2->+put (vtab+vtab+vtab+"rect = PA_GetAreaRect( params );")
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Draw the area in layout editor here";gCommentColor)+vcr
	$2->:=$2->+put (vtab+vtab+vtab+"PA_CustomizeDesignMode( params );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]MenuIcon:15)
	$2->:=$2->+ColorizeText (vtab+"// --- Set custom icon in layout editor tool palette";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_GetMenuIcon :"+vcr)
	$2->:=$2->+put (vtab+vtab+vtab+"PA_SetMenuIcon( params, 128 );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]Dropable:6)
	$2->:=$2->+ColorizeText (vtab+"// --- Handle Drop of 4D objects on the area";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_AllowDrop :"+vcr+vtab+vtab+vtab+"dropInfo = PA_GetDragAndDropInfo( params );"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Accept or refuse drop of this object on area";gCommentColor)+vcr
	$2->:=$2->+(vtab+vtab+vtab+"PA_AllowDrop( params, 1 );"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_Drag :"+vcr+vtab+vtab+vtab+"PA_GetDragPositions( params, &amp;rect, &amp;x, &amp;y );"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Handle customized dragging feedback here";gCommentColor)+vcr
	$2->:=$2->+put (vtab+vtab+vtab+"PA_CustomizeDragOver( params );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_Drop :"+vcr+vtab+vtab+vtab+"dropInfo = PA_GetDragAndDropInfo( params );"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Handle drop of object here";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]KeyDownEvt:11)
	$2->:=$2->+ColorizeText (vtab+"// --- Key down event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_KeyDown :"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"if")
	$2->:=$2->+("( PA_GetKey( params, &amp;key, &amp;keycode, &amp;altkey, &amp;ctrlkey, &amp;shiftkey  ) )"+vcr)
	$2->:=$2->+put (vtab+vtab+vtab+"{")
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+vtab+"// Handle key down event here";gCommentColor)+vcr
	$2->:=$2->+put (vtab+vtab+vtab+"}")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]MouseDownEvt:10)
	$2->:=$2->+ColorizeText (vtab+"// --- Mouse down event";gCommentColor)+vcr
	put (vtab+"// --- Mouse down event")
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+(" eAE_MouseDown :"+vcr)
	$2->:=$2->+put (vtab+vtab+vtab+"PA_GetClick( params, &amp;x, &amp;y );")
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Handle mouse down event here";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]UpdateEvt:9)
	$2->:=$2->+ColorizeText (vtab+"// --- Update and Scroll event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_Update :"+vcr)
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_Scroll :"+vcr)
	$2->:=$2->+put (vtab+vtab+vtab+"rect = PA_GetAreaRect( params );")
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Draw the area here";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]SelectEvt:12)
	$2->:=$2->+ColorizeText (vtab+"// --- Select event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_Select :"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Area is selected";gCommentColor)+vcr
	$2->:=$2->+put (vtab+vtab+vtab+"PA_AcceptSelect( params, 1 );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]DeselectEvt:7)
	$2->:=$2->+ColorizeText (vtab+"// --- Deselect event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_Deselect :"+vcr)
	$2->:=$2->+ColorizeText (vtab+vtab+vtab+"// Area is deselected";gCommentColor)+vcr
	$2->:=$2->+put (vtab+vtab+vtab+"PA_AcceptDeselect( params, 1 );")
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]IdleEvt:17)
	$2->:=$2->+ColorizeText (vtab+"// --- Idle event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_Idle :"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]CursorEvt:18)
	$2->:=$2->+ColorizeText (vtab+"// --- Cursor event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_Cursor :"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]LoadRecordEvt:19)
	$2->:=$2->+ColorizeText (vtab+"// --- Load Record event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_LoadRecord :"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

If ([ExternalAreas:5]SaveRecordEvt:20)
	$2->:=$2->+ColorizeText (vtab+"// --- Save Record event";gCommentColor)+vcr
	$2->:=$2->+kw (vtab+vtab+"case")
	$2->:=$2->+std (" eAE_SaveRecord :"+vcr)
	$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)
End if 

$2->:=$2->+kw (vtab+vtab+"default")
$2->:=$2->+std ("  :"+vcr+vtab+vtab+vtab+"PA_DontTakeEvent( params );"+vcr)
$2->:=$2->+kw (vtab+vtab+vtab+"break;"+vcr+vcr)

$2->:=$2->+put (vtab+"}"+vcr+"}"+vcr+vcr)
