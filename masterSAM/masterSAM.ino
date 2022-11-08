//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP-------------------------------
#include <Adafruit_ADS1X15.h>

//Relay Pins
#define waterPump 33
#define waterValveIn 50
#define waterValveOut 52
//air
#define airValve 46
#define airBleeder 32
#define airPump 31
#define airLever 48
//mech
#define vib 30
#define tilt1 51
#define tilt2 53
//Input Pins
#define startBut 3
//LCD Pins
#define lcdRS A3
#define lcdEN A5
#define lcdD4 A9
#define lcdD5 A10
#define lcdD6 A11
#define lcdD7 A12
//Comm pins
#define sda 20
#define scl 21

//LCD Flags
bool manFlag = true;
bool waterFlag = false;
bool pressFlag = false;
bool bleedFlag = false;
bool equibFlag = false;
bool pValFlag = false;
bool eValFlag = false;

//Constants
const int on=LOW; //for relays which are inverted
const int off=HIGH;

//Constructors
//data to save
int j; //iteration for 3 air 
int i; //iteration for 2 fillups
float pVals[5]; //values
float eVals[5];
float samVal;
float airVol;

//airVars
float targ;
float calib;
Adafruit_ADS1115 ads;
int16_t adc0;
float volts0;
const int len=10;
float psi[len];
float psi_avg;
float run_sum=0;
int tail = 0;
//flags
bool start = false;
bool written =false;
bool second;
int manVib = 0;

void setup() {
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
  attachInterrupt(digitalPinToInterrupt(startBut), start_test, HIGH); //triggers whenever pin is low
}

void loop() 
{
  //write message to lcd 
  if(!written)
  {
    lcdPrint();
    written = true;
  }
  //detect if start button has been pressed
  if(start)
  { 
    //mainSAM();
    testing();
    start = false;
  }
}

void testing(){
   digitalWrite(airValve, off); 
   delay(5000);
   digitalWrite(airValve, on); 
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
      while (psi_avg < targ + calib) {airAverage; delay(100); monPrintData();}
      airHalt();

      //give time for pressure to settle and update
      delay(6000);
      for (int n=1; n<len*8;n++){
        airAverage();
        monPrintData();
        delay(100);
       }
      delay(3000);

    //AIR BLEED
      pressFlag = false; bleedFlag = true;
      lcdPrint();
      
      airBleed();
      
      delayAndUpdate(100, 7000); //give time for psi to settle and 
      pVals[(3*i)+j] = psi_avg; //save pressure before release
      
      pValFlag=true; 
      lcdPrint();
      
    //AIR PUNCH AND VIBRATE
      bleedFlag = false; equibFlag = true;
      lcdPrint();
      
      airEqualize(on);
      vibrate(on);
      delay(3000);
      vibrate(off);
      delayAndUpdate(100, 7000);
    
    //RECORD EQUIB PRESSURE
      airAverage();
      eVals[(3*i)+j] = psi_avg; //save equilibrium pressure

      pValFlag=false; eValFlag=true;
      lcdPrint();
      airEqualize(off);
      delay(3000);
    }
  }
  samVal = (eVals[0]+eVal[1]+eVal[2])-(eVals[3]+eVal[4]+eVal[5]); //calculate SAM num
}
