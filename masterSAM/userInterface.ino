
void uiSetup(){
  
  Serial.begin(9600);
  Serial.println("-----START-------");
  pinMode(startBut, INPUT);

  lcd.begin(16, 2);
  //lcd.print("TEAM SOLENOID");
}

//ISRs for buttons
void start_test(){start = true;}

void serialPrintAll(String phase)
{
  Serial.print(phase);
  Serial.print("adcval:  ");
  Serial.print(adc0,8);
  Serial.print("  adcVolt:  ");
  Serial.print(volts0,4);
  Serial.print("  psi sample:   ");
  Serial.print(psi[tail],2);
  Serial.print("  psi avg:   ");
  Serial.println(psi_avg,2);
}

void lcdPrint()
{
  String line1;
  String line2;
  
  if(manFlag==true)      {line1 ="Prepare SAM"; line2 ="Then Press Start";}
  if(bleedFlag==true)    {line1 = "Bleeding Air";}
  if(waterFlag==true)    {line1= "Water Pumping";}
  if(pressFlag==true)    {line1 = "Pressurizing"; }
  if(equibFlag==true)    {line1 = "Equalizing"; }

  if(pValFlag==true)     {line2 = "Before:" + String(eVals[(i*3)+j],2);}
  if(eValFlag==true)     {line2 = "After:" + String(pVals[(i*3)+j],2); }
  if(!usbFlag)           {line2 = "Insert Flashdrive"; }
  else                   {line2 = "";}

  lcd.clear();
  lcd.print(line1); 
  lcd.setCursor(0,1);
  lcd.print(line2);
}

void usbWrite() 
{

} 
