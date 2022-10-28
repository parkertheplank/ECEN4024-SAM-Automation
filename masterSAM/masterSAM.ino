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
#define airValve 26
#define airBleed 25
#define airPump 5
#define airLever 6
//mech
#define vib 7
#define tilt1 51
#define tilt2 53
//Input and UI Pins
#define startButton 3
#define vibButton 2
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
int adcVal;
float adcVolt;
const float vRef = 5.05;
const float adcOffset = -.28;
float psi;
Adafruit_ADS1115 ads1115;  // Construct an ads1115 

void setup() {
  pinMode(13, OUTPUT); //turns LED on
  //uiSetup();
  //airSetup();
  waterSetup();
  mechSetup();
  //attachInterrupt(digitalPinToInterrupt(startButton), mainSAM, LOW); //triggers whenever pin is low
  //attachInterrupt(digitalPinToInterrupt(vibButton), vibrateOn, FALLING); 

  delay(1000);
  //testing
  mainSAM();
  
}

void loop() {}

void testing(){
  waterFill();
  delay(10000);
  waterClose();
}

void mainSAM(){
//-----------------Final Process-------------------------------
  //Water Fill-Up
  tilt(on);
  waterFill();
  delay(10000);
  waterClose();
  tilt(off);

  //Air Pressurization
  
  /*
  float targetPsi = 14.5;
  
  for(int n=0; n++;n<3)
  {
    if(n==1){targetPsi=30;}
    if(n==2){targetPsi=45;}
    
    feedBackLoop(targetPsi);
    
    airEqualize(on);
    delay(5000);
    airEqualize(off);
  }
  */
  
}
