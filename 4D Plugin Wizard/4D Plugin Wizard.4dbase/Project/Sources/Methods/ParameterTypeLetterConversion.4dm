//%attributes = {}
Case of 
	: ($1="D")
		$0:="Date"
	: ($1="H")
		$0:="Hour"
	: ($1="I")
		$0:="Integer"
	: ($1="L")
		$0:="Long"
	: ($1="O")
		$0:="Blob"
	: ($1="C")
		$0:="Collection"  // ACI0097817
	: ($1="J")
		$0:="Object"  // ACI0097817
	: ($1="P")
		$0:="Picture"
	: ($1="R")
		$0:="Real"
	: ($1="8")
		$0:="Double"  // ACI0088076
	: ($1="S")
		$0:="String"  // ACI0088076
	: ($1="U")
		$0:="UTXT"  // ACI0088076
	: ($1="T")
		$0:="Text"
	: (($1="X") | ($1="Y"))
		$0:="Array"
	: ($1="Z")
		$0:="Pointer"
	Else 
		$0:="None"  // ACI0088076
End case 
