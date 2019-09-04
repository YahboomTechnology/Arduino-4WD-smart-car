/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         ColorLED.c
* @author       Danny
* @version      V1.0
* @date         2017.07.25
* @brief       Colorful searchlight
* @details
* @par History  
*/

#define ON 1     //enable LED
#define OFF 0    //prohibit LED

//Define pin
int LED_R = 11;  //LED_R connect interface 11 on the arduino
int LED_G = 10;  //LED_G connect interface 10 on the arduino
int LED_B =  9;  //LED_B connect interface 9 on the arduino

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
  //Initialize RGB pin as output interface
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

//void breath_LED(int LED_type)
// {
//	int i = 0;
//	for (i = 0; i < 20; i++)
//	{
//		digitalWrite(LED_type, HIGH);
//		delay(i);
//		digitalWrite(LED_type, LOW);
//		delay(20 - i);
//	}
//
//	for (i = 0; i < 20; i++)
//	{
//		digitalWrite(LED_type, LOW);
//		delay(i);
//		digitalWrite(LED_type, HIGH);
//		delay(20 - i);
//    }
//
//    //delay 1s
//    delay(1000);
// }
//

// void light_LED()
// {
//	digitalWrite(LED_R, HIGH);   //light LED_R
//	delay(1000);                 //delay 1s
//	digitalWrite(LED_R, LOW);    //no LED_R
//	delay(1000);                 //delay 1s
//
//	digitalWrite(LED_G, HIGH);   //light LED_G
//	delay(1000);                 //delay 1s
//	digitalWrite(LED_G, LOW);    //no LED_G
//	delay(1000);                 //delay 1s
//
//	digitalWrite(LED_B, HIGH);   //light L_B
//	delay(1000);                 //delay 1s
//	digitalWrite(LED_B, LOW);    //no LED_B
//	delay(1000);                 //delay 1s
//}

/**
* Function       color_led
* @author        Danny
* @date          2017.07.25
* @brief         The different combinations of R, G, B tricolor form 7 different colors
* @param[in1]    Red ON/OFF
* @param[in2]    Green ON/OFF
* @param[in3]    Blue ON/OFF
* @retval        void
* @par History   no
*/
void color_led(int v_iRed, int v_iGreen, int v_iBlue)
{
  //LED_R
  if (v_iRed == ON)
  {
    digitalWrite(LED_R, HIGH);
  }
  else
  {
    digitalWrite(LED_R, LOW);
  }
  //LED_G
  if (v_iGreen == ON)
  {
    digitalWrite(LED_G, HIGH);
  }
  else
  {
    digitalWrite(LED_G, LOW);
  }
  //LED_B
  if (v_iBlue == ON)
  {
    digitalWrite(LED_B, HIGH);
  }
  else
  {
    digitalWrite(LED_B, LOW);
  }
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         LED delay 2,recirculating display Colorful LED
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
  //        //breath_LED twinkle 5
  //	int i;
  //        for (i = 0; i < 5; i++)
  //        {
  //           breath_LED(LED_B);
  //	}
  //        delay(2000);
  //
  //  
  //        for (i = 0; i < 3; i++)
  //        {
  //	   light_LED();
  //        }
  //        delay(2000);
  //
  delay(2000);
  
  while (1)
  {                          //LED_R    LED_G    LED_B
    color_led(ON, OFF, OFF); //   1        0        0
    delay(1000);
    color_led(OFF, ON, OFF); //   0        1        0
    delay(1000);
    color_led(OFF, OFF, ON); //   0        0        1
    delay(1000);
    color_led(ON, ON, OFF);  //   1        1        0
    delay(1000);
    color_led(ON, OFF, ON);  //   1        0        1
    delay(1000);
    color_led(OFF, ON, ON);  //   0        1        1
    delay(1000);
    color_led(ON, ON, ON);   //   1        1        1
  }
}



