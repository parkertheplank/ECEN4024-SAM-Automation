#define airValve 46
#define airBleeder 32
#define airPump 31
#define airLever 48

const int on=LOW;
const int off=HIGH;

float psi;
float targ;
float calib;

void setup() {
  // put your setup code here, to run once:
   pinMode(airPump, OUTPUT);
   pinMode(airBleeder, OUTPUT);
   pinMode(airValve, OUTPUT);
   pinMode(airLever, OUTPUT);
   
   digitalWrite(airBleeder, off); //bleeding on. 
   digitalWrite(airValve, off); 
   digitalWrite(airPump, off); 
   digitalWrite(airLever, off);

   airRead(); //initialize psi reading
   delay(2000);
   airPressurize();
   delay(3000);
}

void airPressurize()
{
  digitalWrite(airBleeder, on); //close bleeder
  digitalWrite(airValve, on); //valve open
  digitalWrite(airPump, on); //start pump
} 

void airHalt() 
{
  digitalWrite(airValve, off); //close valve
  digitalWrite(airPump, off); //stop pump
} 

void airRead() { psi = 60*((analogRead(A15)-204.8)/(1024-204.8));}

void loop() {}
