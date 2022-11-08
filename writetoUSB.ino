//NOT TESTED
#include <Ch376msc.h>
// Connect to SPI port: MISO, MOSI, SCK

// use this if no other device are attached to SPI port(MISO pin used as interrupt)
Ch376msc flashDrive(10); // chipSelect
// buffer for reading
char adatBuffer[255];// max length 255 = 254 char + 1 NULL character
unsigned long totSect = 0;
unsigned long freeSect = 0;
byte percentg = 0;
byte tmpCommand; //used to store data coming from serial port
boolean readMore;

void setup() {
  Serial.begin(115200);
  flashDrive.init();
}

void loop() {
  if(flashDrive.checkIntMessage()){
    if(flashDrive.getDeviceStatus()){
      Serial.println(F("Flash drive attached!"));
    } else {
      Serial.println(F("Flash drive detached!"));
    }
  }
  if(Serial.available()){
    tmpCommand = Serial.read();                      //read incoming bytes from the serial monitor
//Enable SD card below this comment, remove it from comment only after reading warning message in sketch header
//and delete(put in comment) the Serial print message line
    //if(tmpCommand == 115) flashDrive.setSource(1);//SD card enable
      if(tmpCommand == 115) Serial.println(F("Please first read the Warning message in sketch header before you change source to SD card!"));
    if(tmpCommand == 117) flashDrive.setSource(0);//USB enable
    if(((tmpCommand > 48)&&(tmpCommand < 58))  && !flashDrive.driveReady()){ // if the data is ASCII 1 - 9 and no flash drive are attached
       printInfo("Attach flash/SD drive first!");
      tmpCommand = 10; // change the command byte
    }
     switch (tmpCommand) {

      case 49: //1
        printInfo("COMMAND1: Create and write data to file : SAMINFO.TXT");    // Create a file called SAMINFO.TXT
          flashDrive.setFileName("SAMINFO.TXT");  //set the file name
          flashDrive.openFile();                //open the file

          for(int a = 0; a < datarecieved.length; a++){          //write data from each SAM test to flash drive 
            flashDrive.writeFile(datarecieved[a]); //string, string length
          }
          flashDrive.closeFile();               //at the end, close the file
        printInfo("Done!");
        break;
//*****************************************************************************************************************************************************
      case 50: //2
        printInfo("COMMAND2: Append data to file: SAMINFO.TXT");               // Append data to the end of the file.
        flashDrive.setFileName("SAMINFO.TXT");  //set the file name
        if(flashDrive.openFile() == ANSW_USB_INT_SUCCESS){               //open the file
        	flashDrive.moveCursor(CURSOREND);     //if the file exist, move the "virtual" cursor at end of the file, with CURSORBEGIN we actually rewrite our old file
        	//flashDrive.moveCursor(flashDrive.getFileSize()); // is almost the same as CURSOREND, because we put our cursor at end of the file
        }
        for(int a = 0; a <data.length; a++){          //write data from SAM Tests to flash drive
        	if(flashDrive.getFreeSectors()){ //check the free space on the drive
        		flashDrive.writeFile(data[a]); //string, string length
        	} else {
        		printInfo("Disk full");
        	}
        }
        flashDrive.closeFile();               //at the end, close the file
        printInfo("Done!");
        break;
