//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP-------------------------------
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

//Relay Pins
#define waterPump 22
#define waterValveIn 23
#define waterValveOut 24
//air
#define airValve 26
#define airBleed 25
#define airPump 5
#define airLever 6
//mech
#define vibrator 7
#define tilter 8
//Input and UI Pins
#define startButton 3
#define vibButton 49
#define lcd 48
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
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
  attachInterrupt(digitalPinToInterrupt(startButton), mainSAM, LOW); //triggers whenever pin is low
  //attachInterrupt(digitalPinToInterrupt(vibButton), vibrateOn, FALLING); 
  
  //testing
  //feedBackLoop(14.5);
}

void loop() 
{
  airRead();
}

void mainSAM(){
  //testing
  feedBackLoop(14.5);

  //Finial process
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
  
}
