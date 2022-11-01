//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP-------------------------------
#include <Wire.h>
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
#define vibBut 2
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

//Constants
const int on=LOW;
const int off=HIGH;

//Constructors
//data to save
float psiVals[6];
float equibVals[6];
float samVal;
float airVol;
//airVars
int adcVal;
float adcVolt;
const float vRef = 5.05;
const float adcOffset = -.28;
float psi;
float targ;
float calib;
Adafruit_ADS1115 ads1115;  // Construct an ads1115 
//flags
bool start = false;
int manVib = 0;

void setup() {
  pinMode(13, OUTPUT); //turns LED on
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
  //attachInterrupt(digitalPinToInterrupt(startBut), start_test, HIGH); //triggers whenever pin is low
  //attachInterrupt(digitalPinToInterrupt(vibBut), vibOn, HIGH); 
  //attachInterrupt(digitalPinToInterrupt(vibBut), vibOff, LOW); 

  //testing
  delay(6000);
  testing();
  //mainSAM();
}

void loop() 
{
  /*
  if(start){mainSAM();}
  start = false;

  if(manVib ==1){vibrate(on);}
  while(manVib){ delay(100);}
  if(manVib ==2){vibrate(off);}
  manVib == 0;
  */
}



void testing(){
  int j =0;
  targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45);
  calib = (j==0) ? 15 : ((j==1) ? 10 : 4);
  Serial.println(targ);
  Serial.println(psi);
  airPressurize();
  while (psi < targ +calib) {airRead(); delay(100); Serial.println(psi);}
  airHalt();

  //AIR BLEED
  airBleed();
  Serial.println("Post Bleed:  ");
  Serial.print(psi);
  delay(3000);

  //AIR PUNCH an VIBRATE
  airEqualize(on);
  delay(3000);
  Serial.println(psi);
  airEqualize(off);
}

void mainSAM(){
//-----------------FINAL PROCESS-------------------------------
  //two main iterations
  for(int i=0; i<2; i++){
    
    //WATER FILL UP
    bool second = (i==0)? false:true;
    waterFill(second);
    tilt(on);
    delay(10000);
    waterClose();
    tilt(off);

    //three iterations for 3 pressures
    for(int j=0; j<3; j++){
    //AIR PRESSURIZATION 
      //set target pressure and calibration offset
      targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45);
      calib = (j==0) ? 2 : ((j==1) ? 1 : .5);
      
      airPressurize();
      while (psi < targ + calib) {airRead; delay(100);}
      airHalt();

    //AIR BLEED
      airBleed();
      psiVals[(3*i)+j] = psi; //save pressure before release

    //AIR PUNCH an VIBRATE
      airEqualize(on);
      vibrate(on);
      delay(3000);
      vibrate(off);
      delay(3000);
    //RECORD EQUIB PRESSURE
      airRead();
      equibVals[(3*i)+j] = psi; //save equilibrium pressure
      airEqualize(off);
    }
  }

  for(int n=0; n<6;n++){
    Serial.print("PSI: ");
    Serial.print(psiVals[n]);
    Serial.print("\tEquibPressure: ");
    Serial.println(equibVals[n]);
  }
  samVal=2;
  airVol=2;
}
