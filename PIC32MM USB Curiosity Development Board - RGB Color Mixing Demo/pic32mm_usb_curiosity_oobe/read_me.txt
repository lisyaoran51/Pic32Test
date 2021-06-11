================================================================================
PIC32MM USB Curiosity Board - Out of box demo 
================================================================================


Folder Structure
--------------------------------------------------------------------------------
/pic32mm_usb_curiosity_demo.X
  The MPLAB X project folder that holds the source files and demo project

/utilities
  Folder hold the USB drivers and PC examples to run the USB portion of the demo

/read_me.txt
  This document.


Demo Overview
--------------------------------------------------------------------------------
This demo includes two main portions.  
1) Generic I/O operations
2) Example USB transfer

The generic I/O operations show the user how to use the various basic board 
features.  

Turn the potentiometer to control the RGB LED color.  On reset the potentiometer
will control the red portion of the LED.  Press either S1 or S2 to move to the 
green LED.  Press either S1 or S2 again to move to controlling the blue LED.

For the USB portion of the demo, the board will show up as a vendor class
device.  A custom protocol is used to transmit the S3 button status to a custom
PC application.  The PC application can also be used to control the LED2 LED on 
the board.  LED1 displays the current USB bus state.  It will be off for
suspended (or not powered), a slow blink for enumerating (the process of 
connecting to the bus), and a fast blink for when it is ready to transmit data.


USB Demo
--------------------------------------------------------------------------------

Windows Users
~~~~~~~~~~~~~
For Windows users Windows 7 and earlier, a driver installation may be required
in order to run the demo.  If prompted for a driver when the device is plugged
in, point to the /utilities/inf folder.

When the USB device is loaded correctly, LED1 on the board should be blinking
rapidly.

Open the /utilities/plug_and_play_example/bin/plug_and_play_example.exe file.
This PC application will give you the device status.  If the board is connected
and the driver is loaded properly, it should say that the device is connected.

You can click the "Toggle LED(s)" button to toggle LED2 on the board.  The
"Pushbutton Status" label should show the status of the S3 button on the board.

Linux/Mac OS Users
~~~~~~~~~~~~~~~~~~
A source example is provided that utilizes QT framework.  Download QT from
https://www.qt.io/ide/ to build the source code for your target platform.

Note: On Linux and Mac systems, LED1 may not blink rapidly until the PC
application is open and connected to the device.  This is caused because the OS
holds the USB device in an unconfigured state until the PC application connects
to it, unlike a Windows system that automatically configures the device for 
configuration 1.


