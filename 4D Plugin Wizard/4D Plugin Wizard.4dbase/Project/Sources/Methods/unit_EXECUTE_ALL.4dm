//%attributes = {}
  // ----------------------------------------------------
  // Project method : unit_EXECUTE_ALL
  // Database: 4D unitTest
  // ID[14535B18AEE84F1591EB10B7732940A6]
  // Created #19-12-2013 by Vincent de Lachaux
  // ----------------------------------------------------
  // Description:
  // Execute only the methods chosen, without generating error
  // ####################################################
  // WARNING:
  // This method was installed and is updated
  // by the component "4D unitTest"
  // Do not modify this code
  // ####################################################
  // ----------------------------------------------------
  // Declarations
C_BOOLEAN:C305($Boo_asserted;$Boo_OK)
C_LONGINT:C283($Lon_duration;$Lon_i;$Lon_parameters)
C_TIME:C306($Gmt_start;$Gmt_stop)
C_TEXT:C284($kTxt_component;$Txt_message)

ARRAY TEXT:C222($tTxt_components;0)
ARRAY TEXT:C222($tTxt_methods;0)

  // ----------------------------------------------------
  // Initialisations
$Lon_parameters:=Count parameters:C259

If (Asserted:C1132($Lon_parameters>=0;"Missing parameter"))
	
	  //NO PARAMETERS REQUIRED
	
	  // #9-10-2014 - verify that the 4D unitTest component is available {
	$kTxt_component:="4D unitTest"
	
	COMPONENT LIST:C1001($tTxt_components)
	
	If (Find in array:C230($tTxt_components;$kTxt_component)=-1)
		
		If (Command name:C538(1)="Somme")
			
			$Txt_message:="Le composant \"{component}\" n'est pas chargé.\r\r"\
				+"Vérifiez que ce composant est dans le dossier \"Components\" de 4D"
			
		Else 
			
			$Txt_message:="The component \"{component}\" is not loaded.\r\r"\
				+"Check that the component is in the folder \"Components\" of 4D"
			
		End if 
		
		ALERT:C41(Replace string:C233($Txt_message;"{component}";$kTxt_component))
		
		ABORT:C156
		
	End if 
	  //}
	
Else 
	
	ABORT:C156
	
End if 

  // ----------------------------------------------------
  //display the selection dialog
EXECUTE METHOD:C1007("unit_select_methods";$Boo_OK;->$tTxt_methods)

If ($Boo_OK)
	
	  //keep assertions status
	$Boo_asserted:=Get assert enabled:C1130
	
	  //enable assertions, if any
	  //---------------------------------------------------------------------
	SET ASSERT ENABLED:C1131(True:C214;*)
	
	$Gmt_start:=Current time:C178
	
	  //execute the tests {
	For ($Lon_i;1;Size of array:C274($tTxt_methods);1)
		
		EXECUTE METHOD:C1007($tTxt_methods{$Lon_i})
		
	End for 
	  //}
	
	$Gmt_stop:=Current time:C178
	
	  //restore assertions status
	  //---------------------------------------------------------------------
	SET ASSERT ENABLED:C1131($Boo_asserted;*)
	
	  //resume
	$Lon_duration:=($Gmt_stop-$Gmt_start)+0
	EXECUTE METHOD:C1007("unit_EXECUTE_ALL_RESUME";*;Choose:C955($Lon_duration<30;30;$Lon_duration))
	
End if 

  // ----------------------------------------------------
  // End