#include <SD.h>
 
File myFile;
 
void setup()
{
  Serial.begin(9600);
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT); //required pin 10
 
  if (!SD.begin(10)) { //if SD card not inserted
    Serial.println("initialization failed!"); //print it out to the LCD screen tho in the real code
    return;
  }
 
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("SAM_RESULTS.txt", FILE_WRITE); //writes to output file
 
  // if the file opened okay, write to it:
  if (myFile) {
    myFile.println("INSERT SAM NUMBERS");
	// close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening SAM_RESULTS.txt"); //output to LCD
  }
}
 
void loop()
{
	// nothing happens after setup
}