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
float psiVals[5];
float equibVals[5];
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
bool second;
int manVib = 0;

void setup() {
  pinMode(13, OUTPUT); //turns LED on
  uiSetup();
  airSetup();
  waterSetup();
  mechSetup();
  //attachInterrupt(digitalPinToInterrupt(startBut), start_test, HIGH); //triggers whenever pin is low
  delay(2000);
  digitalWrite(airValve, on);
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

}

void mainSAM(){
//-----------------FINAL PROCESS-------------------------------
  //two main iterations
  for(int i=0; i<2; i++){
    Serial.print("\nIteration i:");
    Serial.println(i);
    Serial.println();
    //WATER FILL UP
    second = (i==0) ? false : true;
    waterFill(second);
    tilt(on);
    delay(10000);
    waterClose();
    tilt(off);

    //three iterations for 3 pressures
    for(int j=0; j<3; j++){
      Serial.print("\nIteration j:");
      Serial.println(j);
      Serial.println();
    //AIR PRESSURIZATION 
      //set target pressure and calibration offset
      targ = (j==0) ? 14.5 : ((j==1) ? 30 : 45);
      calib = (j==0) ? 1.5 : ((j==1) ? 2.5 : 3.5);
      Serial.print("targ psi:  "); Serial.println(targ);
      
      airPressurize();
      while (psi_avg < targ + calib) {airAverage; delay(100); monPrintData();}
      airHalt();

      //give time for pressure to settle and update
      delay(6000);
      Serial.println("update:  "); 
      for (int n=1; n<len*8;n++){
        airAverage();
        monPrintData();
        delay(100);
       }
      Serial.println("update:  "); 
      delay(3000);

    //AIR BLEED
      Serial.println("---BLEEDING---- ");
      airBleed();

      //give time for psi to settle and 
      delay(7000);
      for (int n=1; n<len*8;n++){
        airAverage();
        monPrintData();
        delay(100);
       }
      Serial.print("final_psi: ");
      monPrintData();
      psiVals[(3*i)+j] = psi_avg; //save pressure before release

    //AIR PUNCH an VIBRATE
      airEqualize(on);
      vibrate(on);
      delay(3000);
      vibrate(off);
      delay(7000);
      for (int n=1; n<len*8;n++){
        airAverage();
        monPrintData();
        delay(100);
       }
    //RECORD EQUIB PRESSURE
      airAverage();
      Serial.print("final_equib: ");
      monPrintData();
      equibVals[(3*i)+j] = psi_avg; //save equilibrium pressure
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
