	Readme File for Code Example:
--------------------------------------------------

PIC32_LedMessage\led_message

--------------------------------------------------
Code Example Description:
--------------------------------------------------

Example defines a short and long delay to be used in loop to delay LED blinking.

An array is defined which contains patterns that correspond to the letters in the word "HELLO"

For example:
	H:
		0xff 0x08 0x08 0xff
		1              1
		1              1
		1              1
		1              1
		1    1    1    1	
		1              1
		1              1
		1              1
	E:
		0xff 0x89 0x89 0x81,
		1    1    1    1
		1	 	 	
		1	 	 	
		1	 	 	
		1    1    1		
		1	 	 	
		1	 	 	
		1    1    1    1

	AND SO ON:

This array is then applies to PORTA which controlls LED output (0 for off, 1 for on). In between each letter it adds a shord delay and at the end of the word it adds a long delay. 

When running you have to pick the board up and shift it left and right at varieing speeds untill you are able to view the display "HELLO".

--------------------------------------------------
Suggested Development Resources:
--------------------------------------------------
Explorer-16 demo board with 

Processors:	PIC32MX###F512L
		       360
		       460
		       795
		PIC32MX430F064L 
		PIC32MX450F256L 
		PIC32MX470F512L		

Complier:	MPLAB C32 v1 or higher
		MPLAB XC32 

IDE:		MPLAB IDE v8 or higher
            	MPLAB X IDE

--------------------------------------------------
Further Documentation:
--------------------------------------------------
For more information on this topic see Programming "32-bit Microcontrollers in C" Book by Lucio Di Jasso. Example was written by him and is featured in chapter 3 Page 50.