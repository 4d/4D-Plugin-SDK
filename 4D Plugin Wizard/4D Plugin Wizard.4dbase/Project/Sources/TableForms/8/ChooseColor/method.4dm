If (Form event code:C388=On Load:K2:1)
	OBJECT SET RGB COLORS:C628(*;"oTest";0;currentColor)
	_LongToRGB (currentColor;->redThermo;->greenThermo;->blueThermo)
End if 