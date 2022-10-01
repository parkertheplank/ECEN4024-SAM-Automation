//-----------------INFO-------------------------------
//Mega 2560 6 external interrupts, which are 0-5 on pins 2, 3, 21, 20, 19, 18

//-----------------SETUP-------------------------------
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

//Relay Pins
#define waterPump 22
#define waterValve1 23
#define waterValve2 24

#define airValve 26
#define airBleed 25
#define airPump 5
#define airLever 6

#define vibrator 7

//Input and UI Pins
#define startButton 3
#define vibButton 49
#define lcd 48

//Comm pins
#define sda 20
#define scl 21

const int on=LOW;
const int off=HIGH;

//Constructors
const int numReadings = 10;
const int inputPin = A12;

int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;
float vref = 4.99;

/*
Adafruit_ADS1115 ads1115;  // Construct an ads1115 
int16_t adcVal;
int16_t adcRef;
float adcVolt;
float vRef;
*/
float psi;

void setup() {
  Serial.begin(9600);
  Serial.println("\nfart");
  
  pinMode(13, OUTPUT); //turns LED on
  pneumSetup();
  
  
  delay(5000);
  feedbackLoop(30);
}


//----------CONTROL FLOW FUNCTIONS------------------------
void loop() {

  // put your main code here, to run repeatedly:
  
}


/*void main(){}
void airMaster() {}


//-------PROCESS COMPONENT FUNCTIONS-----------------------
void vibrate() {} // to vibrate the SAM.
void waterFill() {} //Turn on water pump and water valves to begin filling the SAM with water for 10 seconds.
void tilt() {} // Tilt the SAM to allow the air bubbles to exit.
void waterClose(){} //After 10 seconds, shut off water pump and close water valves.
void airUSBwrite() {} //Record the air pressure data.
*/

//--------DISPLAY FUNCTIONS-------------------------------
