/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         voltage_detection.c
* @author       Danny
* @version      V1.0
* @date         2017.07.26
* @brief        voltage detection
* @details
* @par History   See the following instructions
*
*/

float Voltage = 0;
float time = 0;

/**
* Function       setup
* @author        Danny
* @date          2017.07.25
* @brief         Initialization configuration
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  Serial.begin(9600);
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         Read the collected voltage value, 
                 print once every second through the serial port
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
  time =  time + 1;
  Voltage = analogRead(A0);                      
  Voltage = (Voltage / 1023) * 4.98 * 1.68 - 0.35 ; 
  //Voltage is port A0 collect value of ad（0-1023），
  //（R14+R15）/R15=1.68，R14=68K,R15=100K）。
  //The 0.35 is the adjustment value due to the accuracy of the resistor。

  Serial.print("U:");
  Serial.print(Voltage , 3);
  Serial.print("V");
  Serial.print("  time:");
  Serial.print(time);
  Serial.println("s");

  delay(1000);
}
