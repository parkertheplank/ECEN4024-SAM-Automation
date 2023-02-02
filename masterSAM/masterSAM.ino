//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP------------------------------
#include <Adafruit_ADS1X15.h> 
#include <LiquidCrystal_I2C.h> //https://github.com/johnrickman/LiquidCrystal_I2C
#include <Wire.h>
#include <SD.h>
//----------------CONSTANTS-------------------------------
//Pins
const int waterPump = 33, waterValveIn = 50, waterValveOut = 52;       //Water
const int airValve = 46, airBleeder = 32, airPump = 31, airLever  = 48; //Air
const int vib = 30, tilt1 = 51, tilt2 = 53, sBut = 3, vBut = 18 ;       //Mech and Button                     
//Other
const int manFlag  = 1, waterFlag = 2, pressFlag = 3, bleedFlag = 4, equibFlag = 5; //LCD Status flags
const int pValFlag = 6, eValFlag  = 7, usbFlag   = 8, noDatFlag = 9, balanFlag = 10, finalFlag = 11;
const int len = 5;                //running average array length
const int on = LOW, off = HIGH;   //for inverted relays
//----------------GLOBAL VARS----------------------------
Adafruit_ADS1115 ads;
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
bool start = false;                       //track whether main test has started
bool vibrating = false;                   //track whether vibrate button is pressed
float targ, volts;                //target psi, calibration offset, and adc volts respectively
float pVal[6], eVal[6], samVal, airVol; //Overall output Data
float psi[len], psi_avg;                  //running average array and average
int i, j, tail = 0;                       //loop indexes and running average index
File myFile;                              //file is created and can be written to
String line[3];
void lcdPrint(int state,int dataState = noDatFlag);

void setup() 
{
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
}

void start_test(){start = true;} //ISR for buttons
void start_vib() {vibrating = true;}

void loop() 
{
  if(start)
  { 
    //mainSAM(); 
    start = false;
  }
  if(vibrating)
  {
    vibrate(on);
    delay(2000);
    vibrate(off);
    vibrating = false;
  }
} 



void mainSAM(){
//-----------------FINAL PROCESS-------------------------------
  for(i=0; i<2; i++)
  {
//-----------------WATER FILL UP-----------------------------
    lcdPrint(waterFlag);
    waterFill(i); //change to this when not testing. Shouldn't need to depressurize between uses. on second iteration i causes delay to depressurize
    tilt(on);
    delay(8000);
    waterClose();
    tilt(off);

    for(j=0; j<3; j++) //three iterations for 3 pressures
    {
//-----------------AIR PRESSURIZATION---------------------
      lcdPrint(pressFlag);
      targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45); //set target pressure 
      float calib =(j==0) ? .8 : ((j==1) ? .9 : 1.2);   //and calibration offset

      do
      {
        airPressurize(); 
        while (psi_avg < targ + calib) 
        {
           airAverage(); 
           delay(50); 
           sPrint("Pressurizing: ");
           lcdPSI();
        }
        airHalt();
        delayAndUpdate(100, 4000); //give time for psi to settle, update average
  
  //---------------------AIR BLEED---------------------------
        lcdPrint(bleedFlag);
        airBleed();
      } while (psi_avg < targ -.05);//don't need to check upperbound as airBleed ensure this
      
      pVal[(3*i)+j] = psi_avg; //record pre-punch psi
      sPrint("Pre-Punch Val: ");
      
//----------------AIR PUNCH AND VIBRATE-----------------------
      lcdPrint(equibFlag, pValFlag); //display pre punch psi
      airEqualize(on);
      for(int k=0;k<3;k++)
      {
        vibrate(on);
        delay(1000);
        vibrate(off);
        delay(1000);
      }
      
      delayAndUpdate(50, 2000);
      eVal[(3*i)+j] = psi_avg; //record equib psi
      sPrint("Equib Val: ");
      lcdPrint(equibFlag, eValFlag); //display equib psi
      airEqualize(off);
      delay(3000); //time to see value
    }
  }
  //calculate vals
  samVal = eVal[5]-eVal[2]; //calculate SAM num
  airVol = ((100*376)/7140)*((((14.5/14.69595)+1)/((eVal[0]/14.69595)+1))-1)*(1+(14.69595/eVal[0])); //Vc=376 “volume of top chamber in mL” P1V1=P2V2 P1=Vc+airVol V1=eVal (Corrected)
  Serial.println("airVol: ");
  Serial.println(airVol);
  Serial.println("samVal");
  Serial.println(samVal);
  //sdWrite(samVal);
  lcdPrint(equibFlag, finalFlag);
  digitalWrite(airBleeder, off);
  //Depressurize after final test
  digitalWrite(waterValveOut, on);
  delay(4000);
  airEqualize(on);
  delay(5000);
  airEqualize(off);
  digitalWrite(waterValveOut, off);
  delay(7000);
  delayAndUpdate(50, 2000); //reset vals for next test
  
  vibrating = false; //necessary due to vBut interrupt being triggered by tilt relays
  lcdPrint(manFlag, manFlag);
}
