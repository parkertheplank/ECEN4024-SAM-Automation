# masterSAM

## Arduino IDE C/C++ Program which runs our device which automates the Super Air Meter Test

## Navigating the files
There are six different .ino files in this folder
* masterSAM.ino - The main file containing the core program and control loop. Open this file and the others should populate as tabs
* air.ino - Functions for air bleeding, pressurization, and air reading 
* testFunctions.ino - Several test functions that can replace mainSAM() in loop() to test various functions
* userInterface.ino - Functions for the buttons, data storage and LCD screen
* mechanical.ino - Functions for the tilt mechanism and vibrator
* water.ino - Functions for the setup and control of the water pump, intake, and outake valves.

## Running The Code
These six files must be located within a folder named 'masterSam' open masterSam.ino in the arduino IDE and other files should populate as tabs. Compile and download the masterSAM file. 

On the device itself, turn on battery power then plug USB cable in to download new changes in code. Then unplug before you turn off the switch. If you don't do it in this order it can bug out the LCD screen but no serious issue. Otherwise just turn on power switch and press start.

## Editing the Code
Always fetch origin and pull any changes before edititing, and/or create your own branch

## Outstanding Issues/Bugs
Recommendations
* Consider using micros()/millis() or some other timing module instead of the vanilla delay(). Delay() is blocking. Also millis() could be useful in measuring update rate of psi by data dumping sys time paired with psi reading and average
* Play with different lines of best fit from our MATLAB data. Implement these equations in the airRead() function
* Implement tare button - Could be an interupt which simply sets an offset float to current air reading. airRead() then would subtract this offset from equation. We were able to get .01 psi accuracy but offset error would occur between days.
* Implement some sort of reset function which will tilt back and depressurize

Bugs
* Both interupts can be triggered by relays causing noise in wire. Code currently handles this but could cause issues

Issues
* Data storage functions and variables are present but not fully implemented
* Several of the test functions require more changes to the program than simply swapping them for mainSAM() or are incomplete
* Issues could occur if proram is stopped (power switch turned off) midway through program. Some way of handling this case and running a reset funciton is needed.
