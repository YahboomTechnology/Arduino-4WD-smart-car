/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         color_recognition.c
* @author       Danny
* @version      V1.0
* @date         2017.07.26
* @brief        Color recognition
* @details
* @par History  See the following instructions
*
*/
//define pin
int LDR_pin = A5;      //Define photosensitive resistor analog port A5

int Intensity = 0;     //illumination value

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
  Serial.begin(9600);//Set baud rate is 9600
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         Read the analog value of the photosensitive resistor, 
                 print it once per second through the serial port
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
  Intensity = analogRead(LDR_pin);  //Read the value of the analog port A5, stored in the Intensity variable
  Serial.print("Intensity = ");    //Serial printf"Intensity = "
  Serial.println(Intensity);       //Serial printf Intensity and wrap
  delay(1000);                      //delay 1000ms
} 
