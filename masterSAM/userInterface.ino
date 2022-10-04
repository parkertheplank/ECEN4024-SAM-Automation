void uiSetup(){
  
  Serial.begin(9600);
  Serial.print("start");
}

void monPrintData()
{
  Serial.print("adcval:  ");
  Serial.print(adcVal);
  Serial.print("\tpsi:   ");
  Serial.println(psi);
}
void airUSBwrite() {} //Record the air pressure data.
