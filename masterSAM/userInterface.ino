#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void uiSetup(){
  
  Serial.begin(9600);
  Serial.println("-----START-------");
  pinMode(startBut, INPUT);
  pinMode(vibBut, INPUT);
  lcd.begin(16, 2);
  lcd.print("TEAM SOLENOID");
}

//ISRs for buttons
void vibOn(){manVib = 1;}
void vibOff(){manVib = 2;}

void monPrintData()
{
  Serial.print("adcval:  ");
  Serial.print(adc0,8);
  Serial.print("   adcVolt:  ");
  Serial.print(volts0,4);
  Serial.print("\tpsi:   ");
  Serial.println(psi_avg);
}
void lcdPrint()
{
  //lcd.setCursor(0,0); //col 0 row 0
  //lcd.setCursor(0,1); //col 0 row 1 

  if(manFlag==true) //water pump message
  {
    lcd.setCursor(0,0);
    lcd.print("Prepare SAM");
    lcd.setCursor(0,1);
    lcd.print("Press Start");  
   
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
  
  if(pValFlag==true) 
    {
      lcd.setCursor(0,1);
      lcd.print("Before: ", pVals[(i*3)+j]);   
    }
    else{lcd.clear();}
    
  if(eValFlag==true) 
    {
      lcd.setCursor(0,1);
      lcd.print("After: ", pVals[(i*3)+j]);  
    }
    else{lcd.clear();}


}
void airUSBwrite() 
{

} //Record the air pressure data.
