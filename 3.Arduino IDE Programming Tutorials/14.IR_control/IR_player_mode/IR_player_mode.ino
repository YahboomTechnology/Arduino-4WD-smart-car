/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         IR_player_mode.c
* @author       liusen
* @version      V1.0
* @date         2017.08.02
* @brief        IR_player_mode
* @details
* @par History  
*
*/
#include <Servo.h>  //Header file that comes with the system
#include "./IRremote.h"


#define ON  1       //Enable LED
#define OFF 0        //Disable LED

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
int RECV_PIN = A5;          //Infrared integrated receiver is connected to the A5 on the UNO
IRrecv irrecv(RECV_PIN);
decode_results results;     //The object used to store the encoded result
unsigned long last = millis();

int Fire = A5;

int distance = 0;

int EchoPin = 12;         //Echo of ultrasonic module is connected to the number 12 on the UNO
int TrigPin = 13;         //Trig of ultrasonic module is connected to the number 13 on the UNO

//TrackSensorLeftPin1 TrackSensorLeftPin2 TrackSensorRightPin1 TrackSensorRightPin2
//      A2                  A1                  A3                   A4
const int TrackSensorLeftPin1  =  A2;  //Define the first tracking infrared sensor pin on the left as A2
const int TrackSensorLeftPin2  =  A1;  //Define the second tracking infrared sensor pin on the left as A1
const int TrackSensorRightPin1 =  A3;  //Define the first tracking infrared sensor pin on the right as A3
const int TrackSensorRightPin2 =  A4;  //Define the second tracking infrared sensor pin on the right as A4

//Define variables for data collected by each tracking infrared pin
bool TrackSensorLeftValue1;
bool TrackSensorLeftValue2;
bool TrackSensorRightValue1;
bool TrackSensorRightValue2;

const int AvoidSensorLeft =  A3;   //Define the infrared obstacle avoidance sensor pin on the left as A3
const int AvoidSensorRight = A1;   //Define the infrared obstacle avoidance sensor pin on the right as A1
const int FollowSensorLeft =  A3;  //Define the infrared follow sensor pin on the left as A3
const int FollowSensorRight = A1;  //Define the infrared follow sensor pin on the right as A1

int LeftSensorValue ;              //Define variables to save the size of the data collected by the infrared sensor
int RightSensorValue ;

const int LdrSensorLeft =  A4;   //Define the photoresistor sensor pin on the left as A4
const int LdrSensorRight = A2;   //Define the photoresistor sensor pin on the right as A2

int LdrSersorLeftValue ;         //Define variables to save the size of the data collected by the photoresistor sensor
int LdrSersorRightValue ;

int LDR_pin = A5;

//1:left90 ; 2: right90; 3: 180  4:run
char mapLocation[38][2] = {
  {0, 2},  {1, 1},  {2, 2}, {3, 3},
  {4, 2},  {5, 4},  {6, 3},
  {7, 2},  {8, 2},  {9, 2}, {10, 3},
  {11, 4}, {12, 4}, {13, 3},
  {14, 2}, {15, 1}, {16, 1}, {17, 3},
  {18, 2}, {19, 1}, {20, 3},
  {21, 1}, {22, 1}, {23, 1}, {24, 2}, {25, 3},
  {26, 2}, {27, 2}, {28, 3},
  {29, 2}, {30, 2}, {31, 3},
  {32, 2}, {33, 1}, {34, 3},
  {35, 1}, {36, 2}, {37, 3}
};
int position = 0; 

/*Variables used in the protocol*/
int CarSpeedControl = 150;    //PWM control 
int g_carstate = enSTOP;     
int g_colorlight = 0;
int g_modeSelect = 0; 
/*
0 is the default state; 
1: Infrared remote control 
2: Patrol mode 
3: Ultrasonic obstacle avoidance 
4: Colorful search 
5: Light finding mode 
6: Infrared tracking
*/
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
  pinMode(EchoPin, INPUT);   
  pinMode(TrigPin, OUTPUT); 

  Serial.begin(9600);	

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);

  pinMode(AvoidSensorLeft, INPUT);
  pinMode(AvoidSensorRight, INPUT);
  pinMode(FollowSensorLeft, INPUT);
  pinMode(FollowSensorRight, INPUT);
  pinMode(LdrSensorLeft, INPUT);
  pinMode(LdrSensorRight, INPUT);

  myservo.attach(servopin);
  irrecv.enableIRIn();                 //Initialize infrared decoding
  pinMode(RECV_PIN, INPUT_PULLUP);     //Set pin 2 as input and internal pull-up mode
}

/**
* Function       Distance_test
* @author        Danny
* @date          2017.07.26
* @brief          Ultrasonic ranging
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance_test()
{
  digitalWrite(TrigPin, LOW);               
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);              
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  float Fdistance = pulseIn(EchoPin, HIGH); 
  Fdistance = Fdistance / 58;
  //  Serial.print("Distance:");            
  //  Serial.print(Fdistance);              
  //  Serial.println("cm");
  distance = Fdistance;
  return;
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
  analogWrite(Left_motor_pwm, CarSpeedControl);

  //right motor advance
  digitalWrite(Right_motor_go, HIGH);  
  digitalWrite(Right_motor_back, LOW); 
  analogWrite(Right_motor_pwm, CarSpeedControl);
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
  analogWrite(Left_motor_pwm, CarSpeedControl);

  //right motor advance
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_pwm, CarSpeedControl);

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
  analogWrite(Left_motor_pwm, CarSpeedControl);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    
  digitalWrite(Right_motor_back, HIGH); 
  analogWrite(Right_motor_pwm, CarSpeedControl);

}


/**
* Function       back
* @author        Danny
* @date          2017.07.25
* @brief         back
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   
*/

void back()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     
  digitalWrite(Left_motor_back, HIGH); 
  analogWrite(Left_motor_pwm, CarSpeedControl);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    
  digitalWrite(Right_motor_back, HIGH);  
  analogWrite(Right_motor_pwm, CarSpeedControl);

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
  for (i = 0; i < 10; i++)      //Output a frequency of sound
  {
    digitalWrite(buzzer, LOW);  //sound
    delay(10);
    digitalWrite(buzzer, HIGH); //nosound
    delay(1);
  }
  for (i = 0; i < 5; i++)      //Output another frequency of sound
  {
    digitalWrite(buzzer, LOW); //sound
    delay(20);
    digitalWrite(buzzer, HIGH); //no sound
    delay(2);
  }
}
/**
* Function       color_led_pwm
* @author        Danny
* @date          2017.07.25
* @brief         Colorful lights illuminate the specified color
* @param[in1]    v_iRed:（0-255）
* @param[in2]    v_iGreen:（0-255）
* @param[in3]    v_iBlue:（0-255）
* @param[out]    void
* @retval        void
* @par History   
*/
void color_led_pwm(int v_iRed, int v_iGreen, int v_iBlue)
{
  analogWrite(LED_R, v_iRed);
  analogWrite(LED_G, v_iGreen);
  analogWrite(LED_B, v_iBlue);
  delay(100);
  return;
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

/**
* Function       color_test
* @author        Danny
* @date          2017.07.26
* @brief         Color recognition test
* @param[in]     void
* @param[out]    void
* @retval        iIntensity
* @par History   no
*/
int color_test()
{
  int iIntensity;
  pinMode(LDR_pin, INPUT_PULLUP);  //The fire extinguishing pin and the gray sensor pin are multiplexed with the A5 port, which needs to be time-multiplexed by the pin setting.
  iIntensity = analogRead(LDR_pin); //Read the value of analog port A5 and store it in the Intensity variable.
  //  Serial.print("Intensity = "); 
  //  Serial.println(iIntensity);
  pinMode(LDR_pin, OUTPUT);
  return iIntensity;
}

/**
* Function       color_dis
* @author        liusen
* @date          2017.08.26
* @brief         Color recognition 
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   
*/
void color_dis()
{
  int Color = 0;
  Color = color_test();

  if (Color >= 370 && Color <= 410)
  {
    color_led_pwm(0, 0, 255);
  }
  else if (Color >= 300 && Color <= 320)
  {
    color_led_pwm(0, 255, 0);
  }
  else if (Color >= 345 && Color <= 365)
  {
    color_led_pwm(255, 0, 0);
  }
  else if (Color >= 330 && Color < 345)
  {
    color_led_pwm(255, 0, 255);
  }
  else if (Color >= 270 && Color <= 290)
  {
    color_led_pwm(0, 255, 255);
  }
  else if (Color >= 250 && Color < 270)
  {
    color_led_pwm(255, 255, 0);
  }
  else
  {
    color_led_pwm(0, 0, 0);
  }
}
/**
* Function       track_get_value
* @author        liusen
* @date          2017.07.26
* @brief         Track mode pin test
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void track_get_value()
{
  TrackSensorLeftValue1  = digitalRead(TrackSensorLeftPin1);
  TrackSensorLeftValue2  = digitalRead(TrackSensorLeftPin2);
  TrackSensorRightValue1 = digitalRead(TrackSensorRightPin1);
  TrackSensorRightValue2 = digitalRead(TrackSensorRightPin2);
}

/********************************************************************************************************/

/**
* Function       Tracking_Mode
* @author        Danny
* @date          2017.07.25
* @brief         Tracking
* @param[in1]    void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Tracking_Mode()
{

  //When the black line is detected, the corresponding indicator of the tracking module is on, and the port level is LOW.
  //When the black line is not detected, the corresponding indicator of the tracking module is off,and the port level is HIGH.
  TrackSensorLeftValue1  = digitalRead(TrackSensorLeftPin1);
  TrackSensorLeftValue2  = digitalRead(TrackSensorLeftPin2);
  TrackSensorRightValue1 = digitalRead(TrackSensorRightPin1);
  TrackSensorRightValue2 = digitalRead(TrackSensorRightPin2);

  //Four-way tracking pin level status
  // 0 0 X 0
  // 1 0 X 0
  // 0 1 X 0
  if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW)
  {
    CarSpeedControl = 150;
    spin_right();
    delay(80);
  }
  //Four-way tracking pin level status
  // 0 X 0 0
  // 0 X 0 1
  // 0 X 1 0
  else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW))
  {
    CarSpeedControl = 150;
    spin_left();
    delay(80);
  }
  // 0 X X X
  else if ( TrackSensorLeftValue1 == LOW)
  {
    CarSpeedControl = 100;
    spin_left();
    //delay(10);
  }
  // X X X 0
  else if ( TrackSensorRightValue2 == LOW )
  {
    CarSpeedControl = 100;
    spin_right();
    //delay(10);
  }
  //Four-way tracking pin level status
  // X 0 1 X
  else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH)
  {
    CarSpeedControl = 120;
    left();
  }
  //Four-way tracking pin level status
  // X 1 0 X
  else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW)
  {
    CarSpeedControl = 120;
    right();
  }
  //Four-way tracking pin level status
  // X 0 0 X
  else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW)
  {
    CarSpeedControl = 150;
    run();
  }
}
/********************************************************************************************************/
/**
* Function       bubble
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic measurement of five times of data for bubble sorting
* @param[in1]    a:Ultrasonic array first address
* @param[in2]    n:Ultrasonic array size
* @param[out]    void
* @retval        void
* @par History   no
*/
void bubble(unsigned long *a, int n)

{
  int i, j, temp;
  for (i = 0; i < n - 1; i++)
  {
    for (j = i + 1; j < n; j++)
    {
      if (a[i] > a[j])
      {
        temp = a[i];
        a[i] = a[j];
        a[j] = temp;
      }
    }
  }
}
/**
* Function       Distance
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic measurement five times, remove the maximum, minimum,
*                Average value to improve test accuracy
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance()
{
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
    Distance_test();
    while (distance >= 500)
    {
      brake();
      Distance_test();
    }
    ultrasonic[num] = distance;
    num++;
  }
  num = 0;
  bubble(ultrasonic, 5);
  distance = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;
  return;
}
/**
* Function       Ultrasonic_avoidMode
* @author        Danny
* @date          2017.07.26
* @brief          Ultrasonic_avoidMode
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Ultrasonic_avoidMode()
{
  Distance();
  printf("$4WD,CSB%d,PV8.3,GS000,LF0000,HW00,GM00#", distance);
  if (distance > 50)
  {
    CarSpeedControl = 120;
    run();      
  }
  else if (distance >= 25 && distance <= 50)
  {
    CarSpeedControl = 60;
    run();      
  }
  else if (distance < 25)
  {
    CarSpeedControl = 100;
    spin_right();    
    delay(350);
    brake();
    delay(100);
    Distance();    
    if (distance >= 25)
    {
      CarSpeedControl = 60;
      run();    
    }
    else if (distance < 25)
    {
      CarSpeedControl = 100;
      spin_left();    
      delay(700);
      brake();
      delay(100);
      Distance(); 
      if (distance >= 25)
      {
        CarSpeedControl = 60;
        run(); 
      }
      else if (distance < 25)
      {
        CarSpeedControl = 100;
        spin_left();
        delay(350);
        brake();
        delay(100);
      }
    }
  }
}
/********************************************************************************************************/

void FindColor_Mode()
{
  track_get_value();
  if ( ((TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW) || ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW)))
  {
    CarSpeedControl = 120;
    run();
    delay(60);
    brake();
    delay(200);
    //ModeBEEP(position);
    switch (mapLocation[position][1])
    {
      case 0: brake(); break;
      case 1: 
       {
           CarSpeedControl = 100;
          spin_left();
          delay(200);
          while (1)
          {
             CarSpeedControl = 100;
             spin_left();
            track_get_value();
            if (TrackSensorLeftValue2 == LOW ||  TrackSensorRightValue1 == LOW)
            {
              brake();
              break;
            }
          }
        } break;
      case 2:
        {
           CarSpeedControl = 100;
          spin_right();
          delay(200);
          while (1)
          {
            spin_right();
            track_get_value();
            if (TrackSensorLeftValue2 == LOW  ||  TrackSensorRightValue1 == LOW)
            {
              brake();
              break;
            }
          }

        } break;
      case 3:
        {

          brake();
          delay(200);
          BeepOnOffMode();
          color_dis();
          if (position == 37)
          {
            BeepOnOffMode();
            position = 0;
             g_modeSelect = 0;
          }
          CarSpeedControl = 100;
          spin_left();
          delay(200);
          while (1)
          {
            spin_left();
            track_get_value();
            if (TrackSensorLeftValue2 == LOW ||  TrackSensorRightValue1 == LOW)
            {
              brake();
              break;
            }
          }
          
        } break;
      case 4: CarSpeedControl = 100; run(); delay(50); break;//直行
    }
    position++;
    //while(1);
  }
  // 0 X X X
  else if ( TrackSensorLeftValue1 == LOW)
  {
    CarSpeedControl = 100;
    spin_left();
  }
  // X X X 0
  else if ( TrackSensorRightValue2 == LOW )
  {
    CarSpeedControl = 100;
    spin_right();
  }
  //Four-way tracking pin level status
  // X 0 1 X
  else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH)
  {
    CarSpeedControl = 100;
    left();
  }
  //Four-way tracking pin level status
  // X 1 0 X
  else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW)
  {
    CarSpeedControl = 100;
    right();
  }
  else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW)
  {
    CarSpeedControl = 120;
    run();
  }
}

/********************************************************************************************************/
void LightSeeking_Mode()
{
   //encountered light, the light of the light seeking module is off, the port level is HIGH
   //No light, the light of the light seeking module is on, the port level is LOW
  LdrSersorRightValue = digitalRead(LdrSensorRight);
  LdrSersorLeftValue  = digitalRead(LdrSensorLeft);
  CarSpeedControl = 150;
  printf("$4WD,CSB120,PV8.3,GS000,LF0000,HW00,GM%d%d#", LdrSersorLeftValue, LdrSersorRightValue);
  if (LdrSersorLeftValue == HIGH && LdrSersorRightValue == HIGH)
  {
    run();   
  }
  else if (LdrSersorLeftValue == HIGH && LdrSersorRightValue == LOW)
  {
    left(); 
  }
  else if (LdrSersorRightValue == HIGH && LdrSersorLeftValue == LOW)
  {
    right();
  }
  else
  {
    brake();
  }
}
/********************************************************************************************************/

void Ir_flow_Mode()
{
   //The following object is encountered, the infrared follower module lights, the port level is LOW
   //The following object is not encountered, the indicator of the infrared follower module is off, and the port level is HIGH.
  LeftSensorValue  = digitalRead(FollowSensorLeft);
  RightSensorValue = digitalRead(FollowSensorRight);
  CarSpeedControl = 120;

  printf("$4WD,CSB120,PV8.3,GS000,LF0000,HW%d%d,GM00", LeftSensorValue, RightSensorValue);
  if (LeftSensorValue == LOW && RightSensorValue == LOW)
  {
    run();       
  }
  else if (LeftSensorValue == LOW && RightSensorValue == HIGH)
  {
    spin_left(); 
  }
  else if (RightSensorValue == LOW && LeftSensorValue == HIGH)
  {
    spin_right();
  }
  else
  {
    brake();   
  }
}

/**
* Function       ModeBEEP
* @author        Danny
* @date          2017.08.17
* @brief         ModeBEEP
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void ModeBEEP(int mode)
{
  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < mode + 1; i++)
  {
    digitalWrite(buzzer, LOW); //sound
    delay(100);
    digitalWrite(buzzer, HIGH); //no sound
    delay(100);
  }
  delay(100);
  digitalWrite(buzzer, HIGH);   //no sound
}
/**
* Function       BeepOnOffMode
* @author        Danny
* @date          2017.08.17
* @brief         
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void BeepOnOffMode()
{
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);   //sound
  delay(1000);                 
  digitalWrite(buzzer, HIGH);  //no sound
}

/**
* Function       IR_Deal
* @author        liusen
* @date          2017.08.17
* @brief          IR_receive_Deal
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void IR_Deal()
{
  if (irrecv.decode(&results))
  {
    //Serial.println(results.value, HEX);


    //Perform different operations based on different values
    //  00FF00FF      switch
    //  00FF30CF   +
    //  00FF708F   -
    //  00FF40BF     light
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
      case 0X00FF00FF: g_carstate = enSTOP; g_modeSelect = 0; position = 0; break;
      case 0x00FF40BF: g_colorlight++; if (g_colorlight > 7) g_colorlight = 0; break;
      case 0x00ff18e7: left_detection(); break;
      case 0x00ff9867: front_detection(); break;
      case 0x00ff58a7: right_detection(); break;
      
      case 0x00FF08F7: g_modeSelect = 1; ModeBEEP(1); BeepOnOffMode();break; //mode1 Default infrared remote control mode
      case 0x00FF8877: g_modeSelect = 2; ModeBEEP(2); BeepOnOffMode();break; //mode2 Tracking mode
      case 0x00FF48B7: g_modeSelect = 3; ModeBEEP(3); BeepOnOffMode();break; //mode3: Obstacle avoidance mode
      case 0x00FF28D7: g_modeSelect = 4; ModeBEEP(4); BeepOnOffMode();break; //mode4: Colorful search (parking track)
      case 0x00FFA857: g_modeSelect = 5; ModeBEEP(5); BeepOnOffMode();break; //mode5: Seek_light mode
      case 0x00FF6897: g_modeSelect = 6; ModeBEEP(6); BeepOnOffMode();break; //mode6: followuing mode
       
      case 0x00FF30CF: CarSpeedControl += 50; if (CarSpeedControl > 255) CarSpeedControl = 255; break; 
      case 0x00FF708F: CarSpeedControl -= 50; if (CarSpeedControl < 50) CarSpeedControl = 100; break; 
      case 0x00FFA05F: whistle(); break;  
      case 0x00FF807F:  g_carstate = enRUN; break;   
      case 0x00FF20DF:  g_carstate = enLEFT; break;  

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
  
  if(g_modeSelect != 0)
  {
    switch (g_modeSelect)
    {
      case 1: g_modeSelect = 0; break; 
      case 2: Tracking_Mode(); break; 
      case 3: Ultrasonic_avoidMode();  break; 
      case 4: FindColor_Mode(); break;  
      case 5: LightSeeking_Mode(); break;  
      case 6: Ir_flow_Mode(); break; 
    }
  }
   
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
  
  if(g_modeSelect == 0)
  {
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
}





