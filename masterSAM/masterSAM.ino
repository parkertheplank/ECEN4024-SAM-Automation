//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP-------------------------------
#include <Adafruit_ADS1X15.h> 
#include <LiquidCrystal.h>

//------------PINS----------------------
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
//Communication
#define sda 20
#define scl 21

//---------------CONSTRUCTORS---------------
//Main
const int on=LOW; //for relays which are inverted
const int off=HIGH;
int j; //iteration for 3 air cycles
int i; //iteration for 2 water fillups
bool start = false; //track whether main test has started
//Data
float pVals[5]; //psi before punch after bleeding
float eVals[5]; //psi of equiblibrium pressure
float samVal; //SAM value
float airVol; //air volume
//LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void lcdPrint(int,int);
#define manFlag 1
#define waterFlag 2
#define pressFlag 3
#define bleedFlag 4
#define equibFlag 5
#define pValFlag 6
#define eValFlag 7
#define usbFlag 8
//Air
Adafruit_ADS1115 ads;
float targ;
float calib;
int16_t adc0;
float volts0;
const int len=5;
float psi[len];
float psi_avg;
float run_sum=0;
int tail = 0;

void setup() 
{
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
  attachInterrupt(digitalPinToInterrupt(startBut), start_test, HIGH);   //interupt for start button
}

void start_test(){start = true;} //ISR for buttons
void loop() { if(start){ mainSAM();start = false;}} 

void testing()
{
   airPressurize();
   while (psi_avg < 56) {airAverage(); delay(100); serialPrintAll("Pressurizing: ");}
   airHalt();
   digitalWrite(airBleeder,off);
}

void mainSAM(){
//-----------------FINAL PROCESS-------------------------------
  //two main iterations
  for(i=0; i<2; i++){
//-----------------WATER FILL UP-----------------------------
    lcdPrint(waterFlag,0);
    waterFill(i);//on second iteration i causes delay to depressurize
    tilt(on);
    delay(10000);
    waterClose();
    tilt(off);

    for(j=0; j<3; j++) //three iterations for 3 pressures
    {
//-----------------AIR PRESSURIZATION---------------------
      lcdPrint(pressFlag,0);
      targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45); //set target pressure 
      calib = (j==0) ? 2.5 : ((j==1) ? 3.5 : 4.5);   //and calibration offset
      airPressurize(); 
      while (psi_avg < targ + calib) 
      {
         airAverage(); delay(100); 
         serialPrintAll("Pressurizing: ");
      }
      airHalt();
      delayAndUpdate(1000, 10000); //give time for psi to settle, update average

//---------------------AIR BLEED---------------------------
      lcdPrint(bleedFlag,0);
      airBleed();
      delayAndUpdate(200, 5000);
      pVals[(3*i)+j] = psi_avg; //record pre-punch psi
      serialPrintAll("Pre-Punch Val: ");
      
//----------------AIR PUNCH AND VIBRATE-----------------------
      lcdPrint(equibFlag, pValFlag); //display pre punch psi
      airEqualize(on);
      for(int k=0;k<3;k++){
        vibrate(on);
        delay(1000);
        vibrate(off);
        delay(1000);
      }
      delayAndUpdate(200, 5000);
      eVals[(3*i)+j] = psi_avg; //record equib psi
      serialPrintAll("Equib Val: ");
      lcdPrint(equibFlag, eValFlag); //display equib psi
      airEqualize(off);
      delay(3000); //time to see value
    }
  }
 
  samVal = (eVals[0]+eVals[1]+eVals[2])-(eVals[3]+eVals[4]+eVals[5]); //calculate SAM num
  usbWrite(); 
  lcdPrint(manFlag, manFlag);
}
