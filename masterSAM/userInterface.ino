void uiSetup(){
  Serial.begin(9600);
  Serial.println("-----START-------");
  pinMode(startBut, INPUT);

  lcd.begin(16, 2);
  lcdPrint(manFlag, manFlag);
}

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

void lcdPrint(int state, int dataState)
{
  String line1;
  String line2;

  switch (state)
  {
    case manFlag:       line1 = "Prepare SAM"; break;
    case bleedFlag:     line1 = "Bleeding Air"; break;
    case waterFlag:     line1= "Water Pumping"; break;
    case pressFlag:     line1 = "Pressurizing"; break;
    case equibFlag:     line1 = "Equalizing"; break;
  }

  switch (dataState)
  {
    case manFlag:       line2 ="Then Press Start"; break;
    case pValFlag:      line2 = "Before:" + String(eVals[(i*3)+j],2); break;
    case eValFlag:      line2 = "After:" + String(pVals[(i*3)+j],2); break;
    case usbFlag:       line2 = "Insert Flashdrive"; break;
    default:            line2 = ""; break;
  }

  lcd.clear();
  lcd.print(line1); 
  lcd.setCursor(0,1);
  lcd.print(line2);
}
void usbWrite() 
{

} 
