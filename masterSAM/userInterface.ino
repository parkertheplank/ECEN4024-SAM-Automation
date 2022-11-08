void uiSetup(){
  
  Serial.begin(9600);
  Serial.println("-----START-------");
  pinMode(startBut, INPUT);
  pinMode(vibBut, INPUT);
}

//ISRs for buttons
void start_test(){start = true;}

void monPrintData()
{
  Serial.print("adcval:  ");
  Serial.print(adc0,8);
  Serial.print("   adcVolt:  ");
  Serial.print(volts0,4);
  Serial.print("\tpsi:   ");
  Serial.println(psi_avg);
}

void airUSBwrite() {} //Record the air pressure data.
