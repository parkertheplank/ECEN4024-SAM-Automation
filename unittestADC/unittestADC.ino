#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

int16_t adc0;
float volts0;


const int len=10;
float psi[len];
float psi_avg;
float run_sum=0;
int head = 0;
int tail = len - 1;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello!");
  Serial.println("Getting single-ended readings from AIN0..3");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
  }


  //initialize values
  psi[0]=airRead();
  for (int n=1; n<len;n++){
    psi[n]= psi[0];
  }
  run_sum= psi[0]*len;
}

float airRead(){
  adc0 = ads.readADC_SingleEnded(0);
  volts0 = adc0 * .0001875;
  return (14.985078*volts0)-14.99510943;
}

void loop(void)
{
  psi[tail] = airRead();
  
  run_sum += psi[tail];
  run_sum -= psi[head];
  psi_avg = run_sum/len;
  
  //Serial.println("-----------------------------------------------------------");
  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0,4); Serial.print("V");
  Serial.print("\tpsi_avg: "); Serial.print(psi_avg,2); 
  Serial.print("\t\tpsi_s: "); Serial.println(psi[tail],2);  Serial.print("\tsum: ");Serial.print(run_sum);  Serial.print("\thead: ");Serial.print(psi[head]);
  Serial.print(" ");
  for (int n=0; n<len;n++){
    Serial.print(n);
    Serial.print(": ");
    Serial.print(psi[n],2);
    Serial.print(" ");
  }
 
  if(head==len-1)
  {
    head = 0;
    tail++;
  }
  else if(tail==len-1)
  {
    tail=0;
    head++;
  }
  else
  {
    head ++;
    tail ++;
  }
  delay(100);
}
