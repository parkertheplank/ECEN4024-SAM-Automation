
float filteredVal = 512.0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH); 
  float value = (float)analogRead(A1);
  filteredVal = (0.95 * filteredVal) + ((0.05) * value);
  float printvalue = ((value/1024.0) * 5.09);
  float realValue = (((filteredVal / 1024.0)* 5.09) + 0.01);
  float psi = ((realValue - 0.99595617)/0.066932271);
  Serial.print("Actual Voltage: ");
  Serial.print(printvalue, 3);
  Serial.print(" ");
  Serial.print("Filtered Voltage: ");
  Serial.print(realValue, 3);
  Serial.print(" ");
  Serial.print("PSI: ");
  Serial.print(psi, 2);
  Serial.print('\n');
  delay(100);
  
  
  
  
  

  
  

}
