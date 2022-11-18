//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP------------------------------
#include <Adafruit_ADS1X15.h> 
#include <LiquidCrystal.h>
//----------------CONSTANTS-------------------------------
//Pins
const int waterPump = 33,waterValveIn = 50,waterValveOut = 52;       //Water
const int airValve = 46,airBleeder = 32,airPump = 31,airLever  = 48; //Air
const int vib = 30,tilt1 = 51,tilt2 = 53;                            //Mechanical                                                                                
const int rs = 8,en = 2,d4 = 4,d5 = 5,d6 = 6,d7 = 7;                 //LCD    
const int sda = 20,scl = 21, sBut = 3 ,vBut = 18 ;                   //Comm and Button                     
//Other
const int manFlag  = 1, waterFlag = 2, pressFlag = 3, bleedFlag = 4, equibFlag = 5; //LCD Status flags
const int pValFlag = 6, eValFlag  = 7, usbFlag   = 8, noDatFlag = 9;
const int len = 5;                //running average array length
const int on = LOW, off = HIGH;   //for inverted relays
//----------------GLOBAL VARS----------------------------
Adafruit_ADS1115 ads;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
bool start = false;                       //track whether main test has started
bool vibrating = false;                   //track whether vibrate button is pressed
float targ, calib, volts0;                //target psi, calibration offset, and adc volts respectively
float pVals[5], eVals[5], samVal, airVol; //Overall output Data
float psi[len], psi_avg;                  //running average array and average
int i, j, tail = 0;                       //loop indexes and running average index

void lcdPrint(int state,int dataState = noDatFlag);

void setup() 
{
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
}

void start_test(){start = true;} //ISR for buttons
void start_vib(){vibrating = true;}
void loop() 
{ 
  testing();
  if(start)
  { 
    testing(); 
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

void testing()
{
   lcdPrint(waterFlag);
   delay(4000);
   lcdPrint(pressFlag);
   delay(4000);
   lcdPrint(equibFlag, eValFlag);\
   delay(4000);
}

void mainSAM(){
//-----------------FINAL PROCESS-------------------------------
  for(i=0; i<2; i++){
//-----------------WATER FILL UP-----------------------------
    lcdPrint(waterFlag);
    waterFill(i);//on second iteration i causes delay to depressurize
    tilt(on);
    delay(10000);
    waterClose();
    tilt(off);

    for(j=0; j<3; j++) //three iterations for 3 pressures
    {
//-----------------AIR PRESSURIZATION---------------------
      lcdPrint(pressFlag);
      targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45); //set target pressure 
      calib = (j==0) ? 3 : ((j==1) ? 3.5 : 4.5);   //and calibration offset
      airPressurize(); 
      while (psi_avg < targ + calib) 
      {
         airAverage(); delay(100); 
         sPrint("Pressurizing: ");
      }
      airHalt();
      delayAndUpdate(100, 4000); //give time for psi to settle, update average

//---------------------AIR BLEED---------------------------
      lcdPrint(bleedFlag);
      airBleed();
      delayAndUpdate(200, 5000);
      pVals[(3*i)+j] = psi_avg; //record pre-punch psi
      sPrint("Pre-Punch Val: ");
      
//----------------AIR PUNCH AND VIBRATE-----------------------
      lcdPrint(equibFlag, pValFlag); //display pre punch psi
      airEqualize(on);
      for(int k=0;k<3;k++){
        vibrate(on);
        delay(1000);
        vibrate(off);
        delay(1000);
      }
      
      delayAndUpdate(100, 3000);
      eVals[(3*i)+j] = psi_avg; //record equib psi
      sPrint("Equib Val: ");
      lcdPrint(equibFlag, eValFlag); //display equib psi
      airEqualize(off);
      delay(3000); //time to see value
    }
  }
 
  samVal = (eVals[0]+eVals[1]+eVals[2])-(eVals[3]+eVals[4]+eVals[5]); //calculate SAM num
  usbWrite(); 
  lcdPrint(manFlag, manFlag);
}
