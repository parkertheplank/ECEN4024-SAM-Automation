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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pneumSetup();
}


//----------CONTROL FLOW FUNCTIONS------------------------
void loop() {
  // put your main code here, to run repeatedly:
  feedbackLoop(14.5);
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
