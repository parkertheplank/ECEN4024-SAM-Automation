#include <LiquidCrystal.h>

LiquidCrystal lcd (8,2,4,5,6,7);


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);

}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.print("Arduino");
  delay(3000);

  lcd.setCursor(2,1);
  lcd.print("LCD Tutorial");
  delay(3000);

  lcd.clear();

}
