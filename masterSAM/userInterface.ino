void uiSetup(){
  
  Serial.begin(9600);
  Serial.print("start");
}

void monPrintData()
{
  Serial.print("adcval:  ");
  Serial.print(adcVal,8);
  Serial.print("   adcVolt:  ");
  Serial.print(adcVolt,4);
  Serial.print("\tpsi:   ");
  Serial.println(psi);
}
void airUSBwrite() {} //Record the air pressure data.
