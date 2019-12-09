//%attributes = {}
C_TEXT:C284($1)
C_TEXT:C284($xliffPath)
$xliffPath:=$1

C_TEXT:C284($xliffRef)
ARRAY OBJECT:C1221($constArray;0)
ARRAY TEXT:C222($transUnitArray;0)
C_TEXT:C284($xliffStr)
C_TEXT:C284($strValue)
C_LONGINT:C283($arraySize)
C_LONGINT:C283($pos)
C_TEXT:C284($found)

C_TEXT:C284($groupName)

  // Open the file
$xliffRef:=DOM Parse XML source:C719($xliffPath)

If (OK=1)
	$found:=DOM Find XML element:C864($xliffRef;"/xliff/group/trans-unit";$transUnitArray)
End if 

$arraySize:=Size of array:C274($transUnitArray)
If ((OK=1) & ($arraySize>0))
	For ($i;1;$arraySize)
		$numAttributes:=DOM Count XML attributes:C727($transUnitArray{$i})
		$attrName:=""
		$stuffID:=0
		DOM GET XML ATTRIBUTE BY NAME:C728($transUnitArray{$i};"id";$stuffID)
		For ($index;1;$numAttributes)
			DOM GET XML ATTRIBUTE BY INDEX:C729($transUnitArray{$i};$index;$attrName;$strValue)
			If ($attrName="d4:value")
				$index:=$numAttributes+1
			End if 
		End for 
		
		  // Get the name
		$found:=DOM Get XML element:C725($transUnitArray{$i};"source";1;$name)
		If ($attrName#"d4:value")
			  // It's a group
			CREATE RECORD:C68([GroupConstants:7])
			[GroupConstants:7]Project:2:=[Projects:2]ID:1
			[GroupConstants:7]Name:3:=$name
			[GroupConstants:7]ID:1:=$stuffID
			SAVE RECORD:C53([GroupConstants:7])
		Else 
			  // It's a constant
			CREATE RECORD:C68([Constants:6])
			[Constants:6]GroupConst:2:=[GroupConstants:7]ID:1
			[Constants:6]ID:1:=$stuffID
			[Constants:6]Name:3:=$name
			_SaveConstantFromString ($strValue)
			SAVE RECORD:C53([Constants:6])
		End if 
	End for 
End if 

DOM CLOSE XML:C722($xliffRef)
