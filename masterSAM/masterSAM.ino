//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP-------------------------------
//-------LIBRARIES-----
#include <Adafruit_ADS1X15.h> 
#include <LiquidCrystal.h>

//----PINS----
//Water
#define waterPump 33
#define waterValveIn 50
#define waterValveOut 52
//Air
#define airValve 46
#define airBleeder 32
#define airPump 31
#define airLever 48
//Mechanical
#define vib 30
#define tilt1 51
#define tilt2 53
//Input Pins
#define startBut 3
//LCD
#define rs 12
#define en 11
#define d4 10
#define d5 9
#define d6 8
#define d7 7
//Comm
#define sda 20
#define scl 21

//----CONSTRUCTORS-----
//Main
const int on=LOW; //for relays which are inverted
const int off=HIGH;
int j; //iteration for 3 air cycles
int i; //iteration for 2 water fillups
bool start = false; //track whether main test has started
bool written = false; //check whether lcd written to yet
bool second; //tracks whether second main iteration
//Data
float pVals[5]; //psi before punch after bleeding
float eVals[5]; //psi of equiblibrium pressure
float samVal; //SAM value
float airVol; //air volume
//LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
String msg;
bool manFlag = true; //flags for phases of Sam test
bool waterFlag = false;
bool pressFlag = false;
bool bleedFlag = false;
bool equibFlag = false;
bool pValFlag = false;
bool eValFlag = false;
bool usbFlag = false;
//Air
Adafruit_ADS1115 ads;
float targ;
float calib;
int16_t adc0;
float volts0;
const int len=10;
float psi[len];
float psi_avg;
float run_sum=0;
int tail = 0;

void setup() {
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
  //interupt for start button
  attachInterrupt(digitalPinToInterrupt(startBut), start_test, HIGH);  
}

void loop() 
{
  if(!written){lcdPrint();written = true;} //write begining message t lcd
  if(start){ mainSAM();start = false;} //start test when button pressed
}

void testing()
{
  
}

void mainSAM(){
//-----------------FINAL PROCESS-------------------------------
  //two main iterations
  for(i=0; i<2; i++){
    //WATER FILL UP
    equibFlag = false; waterFlag = true; eValFlag=false;
    lcdPrint();
    
    second = (i==0) ? false : true;
    waterFill(second);
    tilt(on);
    delay(10000);
    waterClose();
    tilt(off);

    //three iterations for 3 pressures
    for(j=0; j<3; j++){
    //AIR PRESSURIZATION 
      waterFlag = false; pressFlag = true; eValFlag=false;
      lcdPrint();
      
      //set target pressure and calibration offset
      targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45);
      calib = (j==0) ? 1.5 : ((j==1) ? 2.5 : 3.5);
      
      airPressurize();
      while (psi_avg < targ + calib) {airAverage; delay(100); serialPrintAll("Pressurizing: ");}
      airHalt();

      //give time for pressure to settle and update
      delayAndUpdate(100, 7000);

    //AIR BLEED
      pressFlag = false; bleedFlag = true;
      lcdPrint();
      
      airBleed();
      
      delayAndUpdate(100, 5000); //give time for psi to settle and 
      pVals[(3*i)+j] = psi_avg; //save pressure before release
      serialPrintAll("Pre-Punch Val: ");
      
      pValFlag=true; 
      lcdPrint();
      
    //AIR PUNCH AND VIBRATE
      bleedFlag = false; equibFlag = true;
      lcdPrint();
      
      airEqualize(on);
      for(int k=0;k<3;k++){
        vibrate(on);
        delay(1000);
        vibrate(off);
        delay(1000);
      }
      delayAndUpdate(100, 7000);
    
    //RECORD EQUIB PRESSURE
      eVals[(3*i)+j] = psi_avg; //save equilibrium pressure
      serialPrintAll("Equib Val: ");

      pValFlag=false; eValFlag=true;
      lcdPrint();
      airEqualize(off);
      delay(3000);
    }
  }
 
  samVal = (eVals[0]+eVals[1]+eVals[2])-(eVals[3]+eVals[4]+eVals[5]); //calculate SAM num
  usbWrite(); 

  equibFlag=false; manFlag=true;
  lcdPrint();
}
