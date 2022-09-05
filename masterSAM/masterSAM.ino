//-----------------SETUP-------------------------------
//Relay Pins
#define waterPump 1
#define waterValve1 2
#define waterValve2 2
#define airValve1 2
#define airBleed 2
#define airTrans 2
#define airPump 2
#define airLever 2
#define vibrator 3

//Input and UI Pins
#define airTransIn 2
#define startButton 3
#define vibButton 5
#define lcd 4
#define 

void setup() {
  // put your setup code here, to run once:

}


//----------CONTROL FLOW FUNCTIONS------------------------
void loop() {
  // put your main code here, to run repeatedly:

}
void main(){}
void airMaster() {}


//-------PROCESS COMPONENT FUNCTIONS-----------------------
void vibrate() {} // to vibrate the SAM.
void waterFill() {} //Turn on water pump and water valves to begin filling the SAM with water for 10 seconds.
void tilt() {} // Tilt the SAM to allow the air bubbles to exit.
void waterClose(){} //After 10 seconds, shut off water pump and close water valves.
void airPressurize(){} //Turn on air pump and two-way air valve to begin pressurizing the SAM.
void airRead() {} //Read the pressure sensor analog output to match 14.5 psi. 
void airHalt() {} //raise air pressure to desired value
void airBleed() {} //Decrease air pressure to desired value
void airLever() {} //Push top lever to release pressure into the chamber for 10-20 seconds to allow the pressure to reach equilibrium. 
void airUSBwrite() {} //Record the air pressure data.


//--------DISPLAY FUNCTIONS-------------------------------
