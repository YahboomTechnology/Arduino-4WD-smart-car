/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         IR_contorl.c
* @author       liusen
* @version      V1.0
* @date         2017.08.02
* @brief        Infrared remote control trolley
* @details
* @par History  
*
*/
#include <Servo.h>  //Header file that comes with the system
#include "./IRremote.h"


#define ON  1       //Enable LED
#define OFF 0       //Disable LED

#define run_car     '1'
#define back_car    '2'
#define left_car    '3'
#define right_car   '4'
#define stop_car    '0'

/*Car running status enumeration*/
enum {
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT
} enCarState;

Servo myservo;      //Define the servo object myservo

int LED_R = 11;     //LED_R is connected to the number 11 on the UNO
int LED_G = 10;     //LED_G is connected to the number 10 on the UNO
int LED_B = 9;      //LED_B is connected to the number 9 on the UNO

int Left_motor_go = 8;        //Left motor advance AIN1
int Left_motor_back = 7;      //Left motor back AIN2

int Right_motor_go = 2;       //Right motor advance BIN1
int Right_motor_back = 4;     //Right motor back BIN2

int Left_motor_pwm = 6;       //Left motor speed control PWMA
int Right_motor_pwm = 5;      //Right motor speed control PWMB

int buzzer = A0;             //Buzzer is connected to the A0 on the UNO
 
int servopin = 3;           //Servo is connected to the number 3 on the UNO

//Infrared remote control
int RECV_PIN = A5;         //Infrared integrated receiver is connected to the A5 on the UNO
IRrecv irrecv(RECV_PIN);
decode_results results;    //The object used to store the encoded result
unsigned long last = millis();

int Fire = A5;

int Distance = 0;
int Echo = 12;                //Echo of ultrasonic module is connected to the number 12 on the UNO
int Trig = 13;                //Trig of ultrasonic module is connected to the number 13 on the UNO

/*Variables used in the protocol*/
int control = 150;          //PWM control
int g_carstate = enSTOP;   
int g_colorlight = 0;

/**
* Function       setup
* @author        liusen
* @date          2017.08.02
* @brief         Initialization settings
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  //Initialize motor drive IO as output mode
  pinMode(Left_motor_go, OUTPUT); 
  pinMode(Left_motor_back, OUTPUT); 
  pinMode(Right_motor_go, OUTPUT); 
  pinMode(Right_motor_back, OUTPUT); 
  pinMode(buzzer, OUTPUT); 
  digitalWrite(buzzer, HIGH);   
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);   

  Serial.begin(9600);	

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  myservo.attach(servopin);
  irrecv.enableIRIn();                 //Initialize infrared decoding
  pinMode(RECV_PIN, INPUT_PULLUP);     //Set pin 2 as input and internal pull-up mode
}

/**
* Function       Distance_test
* @author        liusen
* @date          2017.07.26
* @brief         Ultrasonic ranging
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance_test()  
{
  digitalWrite(Trig, LOW); 
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance = Fdistance / 58;    
  Distance = Fdistance;
}

/**
* Function       run
* @author        Danny
* @date          2017.07.26
* @brief         run
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void run()
{
  //left motor advance
  digitalWrite(Left_motor_go, HIGH);   
  digitalWrite(Left_motor_back, LOW); 
  analogWrite(Left_motor_pwm, control);

  //right motor advance
  digitalWrite(Right_motor_go, HIGH);  
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, control);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         brake
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   no
*/
void brake()
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
}

/**
* Function       left
* @author        Danny
* @date          2017.07.25
* @brief         turn left(left wheel stop，right wheel advance)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no 
*/

void left()
{
  //left motor stop 
  digitalWrite(Left_motor_go, LOW);    
  digitalWrite(Left_motor_back, LOW);  
  analogWrite(Left_motor_pwm, 0);

  //right motor advance
  digitalWrite(Right_motor_go, HIGH);  
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, 150);
}
/**
* Function       right
* @author        Danny
* @date          2017.07.25
* @brief         turn right(left wheel advance,right wheel stop)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void right()
{
  //left motor advance
  digitalWrite(Left_motor_go, HIGH); 
  digitalWrite(Left_motor_back, LOW);  
  analogWrite(Left_motor_pwm, 150);

  //right motor stop
  digitalWrite(Right_motor_go, LOW);   
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, 0);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.07.25
* @brief         turn left in place(left wheel back,right wheel advance)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_left()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     
  digitalWrite(Left_motor_back, HIGH);  
  analogWrite(Left_motor_pwm, control);

  //right motor advance
  digitalWrite(Right_motor_go, HIGH);  
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, control);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.07.25
* @brief         turn rigth in place(left wheel advance,right wheel back)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   no
*/

void spin_right()
{
  //left motor advance
  digitalWrite(Left_motor_go, HIGH);   
  digitalWrite(Left_motor_back, LOW);  
  analogWrite(Left_motor_pwm, control);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    
  digitalWrite(Right_motor_back, HIGH); 
  analogWrite(Right_motor_pwm, control);
}


/**
* Function       back
* @author        Danny
* @date          2017.07.25
* @brief         back
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void back()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);    
  digitalWrite(Left_motor_back, HIGH);  
  analogWrite(Left_motor_pwm, control);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     
  digitalWrite(Right_motor_back, HIGH);  
  analogWrite(Right_motor_pwm, control);

}

/**
* Function       whistle
* @author        liusen
* @date          2017.07.25
* @brief         
* @param[in1]    void
* @retval        void
* @par History   
*/

void whistle()   
{
  int i;
  for (i = 0; i < 10; i++)        //Output a frequency of sound
  {
    digitalWrite(buzzer, LOW);    //sound
    delay(10);
    digitalWrite(buzzer, HIGH);   //nosound
    delay(1);
  }
  for (i = 0; i < 5; i++) //Output another frequency of sound
  {
    digitalWrite(buzzer, LOW); //sound
    delay(20);
    digitalWrite(buzzer, HIGH); //no sound
    delay(2);
  }
}

/**
* Function       color_led
* @author        Danny
* @date          2017.07.25
* @brief         7 different colors are formed by different combinations of R, G, and B colors
* @param[in1]    Red
* @param[in2]    Green
* @param[in3]    Blue
* @retval        void
* @par History   no
*/

void corlor_led(int v_iRed, int v_iGreen, int v_iBlue)
{
  if (v_iRed == ON)
  {
    digitalWrite(LED_R, HIGH);
  }
  else
  {
    digitalWrite(LED_R, LOW);
  }
  if (v_iGreen == ON)
  {
    digitalWrite(LED_G, HIGH);
  }
  else
  {
    digitalWrite(LED_G, LOW);
  }
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
* Function       front_detection
* @author        Danny
* @date          2017.07.25
* @brief         PTZ reset
* @param[in]     void
* @retval        void
* @par History   no
*/
void front_detection()
{
  for (int i = 0; i <= 15; i++) 
  {
    myservo.write(90);
  }
}
/**
* Function       left_detection
* @author        Danny
* @date          2017.07.25
* @brief         PTZ turn left
* @param[in]     void
* @retval        void
* @par History   no
*/
void left_detection()
{
  for (int i = 0; i <= 15; i++) 
  {
    myservo.write(180);
  }
}
/**
* Function       right_detection
* @author        Danny
* @date          2017.07.25
* @brief         PTZ turn right
* @param[in]     void
* @retval        void
* @par History   no
*/
void right_detection()
{
  for (int i = 0; i <= 15; i++) 
  {
    myservo.write(0);
  }
}


void IR_Deal()
{
  if (irrecv.decode(&results))
  {
    //Serial.println(results.value, HEX);
    //Perform different operations based on different values
    //  00FF00FF    switch
    //  00FF30CF    +
    //  00FF708F    -
    //  00FFA05F     beep
    //  00FF807F     advance
    //  00FF20DF     turn left
    //  00FF00FF     stop
    //  00FF609F     turn right
    //  00FF906F     back
    //  00FF10EF     turn left in place
    //  00FF50AF     turn right in place
    //  00FFB04F     0
    //  00FF08F7     1
    //  00FF8877     2
    //  00FF48B7     3
    //  00FF28D7     4
    //  00FFA857     5
    //  00FF6897     6
    //  00FF18E7     7
    //  00FF9867     8
    //  00FF58A7     9    

    switch (results.value)
    {
      case 0X00FF00FF: g_carstate = enSTOP;  break;
      case 0x00FF40BF: g_colorlight++; if (g_colorlight > 9) g_colorlight = 0; break;
      case 0x00ff18e7: left_detection(); break;
      case 0x00ff9867: front_detection(); break;
      case 0x00ff58a7: right_detection(); break;
      default: break;
    }

      switch (results.value)
      {
        case 0x00FF30CF: control += 50; if (control > 255) control = 255; break;
        case 0x00FF708F: control -= 50; if (control < 50) control = 100; break;
        case 0x00FFA05F: whistle(); break;
        case 0x00FF807F:  g_carstate = enRUN; break;
        case 0x00FF20DF:  g_carstate = enLEFT; break;
        case 0x00FF00FF:  g_carstate = enSTOP; break;
        case 0x00FF609F:  g_carstate = enRIGHT; break;
        case 0x00FF906F:  g_carstate = enBACK; break;
        case 0x00FF10EF:  g_carstate = enTLEFT; break;
        case 0x00FF50AF:  g_carstate = enTRIGHT; break;
        default: break; 
      }
    last = millis();
    irrecv.resume(); // Receive the next code
  }
  else if (millis() - last > 120)
  {
    g_carstate = enSTOP;
    last = millis();
  }

}

void loop()
{
  IR_Deal();
  switch (g_colorlight)
  { 
    case 0: corlor_led(OFF, OFF, OFF); break;  
    case 1: corlor_led(ON, ON, ON); break;
    case 2: corlor_led(ON, OFF, OFF); break;
    case 3: corlor_led(OFF, ON, OFF); break;
    case 4: corlor_led(OFF, OFF, ON); break;
    case 5: corlor_led(OFF, ON, ON); break;
    case 6: corlor_led(ON, OFF, ON); break;
    case 7: corlor_led(ON, ON, OFF); break;
    case 8: corlor_led(OFF, OFF, OFF); break;
  }
  
  switch (g_carstate)
  {
    case enSTOP: brake(); break;
    case enRUN: run(); break;
    case enLEFT: left(); break;
    case enRIGHT: right(); break;
    case enBACK: back(); break;
    case enTLEFT:spin_left(); break;
    case enTRIGHT:spin_right();break;
    default: brake(); break;
  }
}





