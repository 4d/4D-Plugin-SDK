//%attributes = {}
$0:=[Constants:6]Name:3
$0:=$0+":"
Case of 
	: ([Constants:6]Type:4="L")
		$0:=$0+String:C10([Constants:6]LongintValue:7)+":L"
	: ([Constants:6]Type:4="8")
		$0:=$0+String:C10([Constants:6]RealValue:6)+":R"
	: ([Constants:6]Type:4="S")
		$0:=$0+[Constants:6]StringValue:5+":S"
End case 