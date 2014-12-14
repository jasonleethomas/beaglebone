NAME
	bbbmorse - beaglebone black morse flasher
	
SYNOPSIS
	bbbmorse [filename]	[charlimit] [ledindex]
	
DESCRIPTION
	bbbmorse flashes iterates through "filename", parses and
	flashes each character on the beaglebone's usr led.
	
OPTIONS
	filename - absolute filepath of textfile containing message.
		if not specified, programs looks for ./message.txt, then
		exits if not found.
	
	charlimit - character limit of message, program will flash
		less than or equal to this number. 
		if not specified, program assumes sets limit to 512
		
	ledindex - chooses beaglebone's led, usr(0-3). 
		if not specified, program uses usr 0.
		
		