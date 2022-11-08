
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
  //lcd.setCursor(0,0); //col 0 row 0
  //lcd.setCursor(0,1); //col 0 row 1 
  //first line logic
  if(manFlag==true) //water pump message
  {
    lcd.setCursor(0,0);
    lcd.print("Prepare SAM");
    lcd.setCursor(0,1);
    lcd.print("Then Press Start");  
  }
  if(bleedFlag==true) //water pump message
  {
    lcd.setCursor(0,0);
    lcd.print("Bleeding Air"); 
  }
  if(waterFlag==true) //water pump message
  {
    lcd.setCursor(0,0);
    lcd.print("Water Pumping");   
  }
  if(pressFlag==true) //water pump message
  {
    lcd.setCursor(0,0);
    lcd.print("Pressurizing");   
  }
  if(equibFlag==true) //water pump message
  {
    lcd.setCursor(0,0);
    lcd.print("Equalizing");   
  }
  //Second line logic
  if(pValFlag==true) 
  {
    lcd.setCursor(0,1);
     msg = "Before:" + String(eVals[(i*3)+j],2);
    lcd.print(msg);
  }
  else if(eValFlag==true) 
  {
    lcd.setCursor(0,1);
    msg = "After:" + String(pVals[(i*3)+j],2);
    lcd.print(msg);  
  }
  else if(!usbFlag) 
  {
    lcd.setCursor(0,1);
    lcd.print("Insert Flashdrive");  
  }
  else
  {lcd.clear();}
}

void usbWrite() 
{

} 
