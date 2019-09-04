/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         servo_ultrasonic_avoid.c
* @author       Danny
* @version      V1.0
* @date         2017.07.26
* @brief        Ultrasonic obstacle avoidance with pan head of steering engine
* @details
* @par History  See the following instructions
*
*/
#include <Servo.h>  //The servo library file of the system

#define ON  1       //enable LED
#define OFF 0       //prohibit LED

Servo myservo;      //Define steering gear-myservo

//Define pin
int LED_R = 11;     //LED_R connect interface 11 on the arduino
int LED_G = 10;     //LED_G connect interface 10 on the arduino
int LED_B = 9;      //LED_B connect interface 9 on the arduino

/*Steering engine*/
int ServoPin = 3;

int Left_motor_go = 8;        // AIN1
int Left_motor_back = 7;      // AIN2

int Right_motor_go = 2;       // BIN1
int Right_motor_back = 4;     // BIN2

int Left_motor_pwm = 6;       //left motor speed control PWMA
int Right_motor_pwm = 5;      //right motor speed control PWMB

int key = A0;                 //Define button as analog port A0 with Arduino 

int EchoPin = 12;             //define EchoPin is 12 on the  Arduino  
int TrigPin = 13;             //define TriPin is 13 on the  Arduino 

float distance = 0;           //define ultrasonic measure distance

const int AvoidSensorLeft =  A3;   //Define left infrared obstacle avoidance sensor pin A3
const int AvoidSensorRight = A1;   //Define right infrared obstacle avoidance sensor pin A1
int LeftSensorValue ;              //Define variables to store data collected by photosensitive resistor
int RightSensorValue ;
/**
* Function       setup
* @author        Danny
* @date          2017.07.25
* @brief          Initialization configuration
* @param[in]     void
* @retval        void
* @par History   no
*/
void setup()
{
  //Set the baud rate of serial data transmission is 9600
  Serial.begin(9600);

  //Initialize the motor PIN IO 
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);

  //Initialize RGB pin as output interface
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  //Define the buzzer interface as the output interface
  pinMode(key, INPUT);
  
  //Initialize button is level high
  digitalWrite(key, HIGH);

  //Define ultrasonic pin
  pinMode(EchoPin, INPUT);    
  pinMode(TrigPin, OUTPUT);   

  //Set the  Steering engine control pin to 3
  myservo.attach(ServoPin);
  int ServoPos = 90;
  myservo.write(ServoPos);

  //Define sensor as input interface
  pinMode(AvoidSensorLeft, INPUT);
  pinMode(AvoidSensorRight, INPUT);

  //Initialize sensor is level high
  digitalWrite(AvoidSensorLeft, HIGH);
  digitalWrite(AvoidSensorRight, HIGH);

  //Call key scan fuction
  key_scan();
}

/**
* Function       servo_color_carstate
* @author        Danny
* @date          2017.07.26
* @brief         Steering engine turn ultrasonic obstacle avoidance, 
                 led according to the status of the car to display the corresponding color
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void servo_color_carstate()
{
  //Define the position variables of the steering gear and the front, left and right distances of the car
  int iServoPos = 0;
  int LeftDistance = 0;    
  int RightDistance = 0;   
  int FrontDistance = 0;   
  corlor_led(ON, OFF, OFF);
  back(80);                //Avoid a sudden stop, can not brake the car
  brake();

  //The steering gear rotates to 0 degrees,is right, ranging
  myservo.write(0);
  delay(500);
  Distance_test();         
  RightDistance = distance;

  //The steering gear rotates to 180 degrees,is backside, ranging
  myservo.write(180);
  delay(500);
  Distance_test();        
  LeftDistance = distance;

  //The steering gear rotates to 90 degrees,is left, ranging
  myservo.write(90);
  delay(500);
  Distance_test();
  FrontDistance = distance;

  if (LeftDistance < 10 && RightDistance < 10 && FrontDistance < 10  )
  {
    //light_R,turn around
    corlor_led(ON, OFF, ON);
    spin_right(625);
    brake();
  }
  else if ( LeftDistance >= RightDistance) //When the left distance is greater than the right, turn left in place
{
    //light_B
    corlor_led(OFF, OFF, ON);
    spin_left(315);
    brake();
  }
  else if (LeftDistance < RightDistance ) //When the right distance is greater than the left, the turn right is in place
  {
    //light_R,turn right
    corlor_led(ON, OFF, ON);
    spin_right(315);
    brake();
  }
}

/**
* Function       run
* @author        Danny
* @date          2017.07.26
* @brief         car run
* @param[in1]    LeftSpeed
* @param[in2]    RightSpeed
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(int LeftSpeed, int RightSpeed)
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_pwm, LeftSpeed);

   //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, RightSpeed);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         car brake
* @param[in]
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
* @brief         Turn left (left wheel stop, right wheel go)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void left()
{
  //left motor stop
  digitalWrite(Left_motor_go, LOW);    //prohibit left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, 0);

   //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 80);
}

/**
* Function       right
* @author        Danny
* @date          2017.07.25
* @brief         Turn right (right wheel stop, left wheel go)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void right()
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, 80);

  //right motor stop
  digitalWrite(Right_motor_go, LOW);   //prohibit right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 0);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.07.25
* @brief         Turn left in place(left wheel back,right wheel go)
* @param[in]     time:delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_left(int time)
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable left motor back
  analogWrite(Left_motor_pwm, 200);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 200);

  delay(time);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.07.25
* @brief         Turn right in place(right wheel back,left wheel go)
* @param[in]     time:delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_right(int time)
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);    //enable left motor go
  digitalWrite(Left_motor_back, LOW);   //prohibit left motor back
  analogWrite(Left_motor_pwm, 200);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH); //enable right motor back
  analogWrite(Right_motor_pwm, 200);

  delay(time);
}

/**
* Function       back
* @author        Danny
* @date          2017.07.25
* @brief         car back
* @param[in]     time:delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void back(int time)
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable lef tmotor back
  analogWrite(Left_motor_pwm, 80);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH);  //enable right motor back
  analogWrite(Right_motor_pwm, 80);

  delay(time);
}

/**
* Function       color_led
* @author        Danny
* @date          2017.07.25
* @brief        The different combinations of R, G, B tricolor form 7 different colors
* @param[in1]    v_iRed:Red ON/OFF
* @param[in2]    v_iGreen:Green ON/OFF
* @param[in3]    v_iBlue:Blue ON/OFF
* @retval        void
* @par History   no
*/
void corlor_led(int v_iRed, int v_iGreen, int v_iBlue)
{
  //红色LED
  if (v_iRed == ON)
  {
    digitalWrite(LED_R, HIGH);
  }
  else
  {
    digitalWrite(LED_R, LOW);
  }
  //绿色LED
  if (v_iGreen == ON)
  {
    digitalWrite(LED_G, HIGH);
  }
  else
  {
    digitalWrite(LED_G, LOW);
  }
  //蓝色LED
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
* Function       Distance
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic measurement of the distance ahead
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance()
{
   digitalWrite(TrigPin, LOW);               //Give trigger pin low level 2 s
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);              //Give trigger pin high level 10 s,at least 10 s
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  float Fdistance = pulseIn(EchoPin, HIGH); // Read high level time(uint：us)
  Fdistance = Fdistance / 58;
  Serial.print("Distance:");               //（uint ：cm）
  Serial.print(Fdistance);                 //display distance
  Serial.println("cm");
  distance = Fdistance;
}

/**
* Function       Distane_test
* @author        Danny
* @date          2017.07.26
* @brief         Ultrasonic measurement five times, remove the maximum value, the minimum value,
*                Taking average value to improve test accuracy
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void Distance_test()
{
  unsigned long ultrasonic[5] = {0};
  int num = 0;
  while (num < 5)
  {
    Distance();
    while (distance >= 500 || distance == 0)
    {
      brake();
      Distance();
    }
    ultrasonic[num] = distance;
    num++;
    delay(10);
  }
  num = 0;
  bubble(ultrasonic, 5);
  distance = (ultrasonic[1] + ultrasonic[2] + ultrasonic[3]) / 3;
  return;
}

/**
* Function       bubble
* @author        Danny
* @date          2017.07.26
* @brief         Five times of ultrasonic data for bubble sorting
* @param[in1]    a:The first address of ultrasonic array
* @param[in2]    n:Size of ultrasonic array
* @param[out]    void
* @retval        void
* @par History   no
*/
//冒泡排序
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
* Function       key_scan
* @author        Danny
* @date          2017.07.25
* @brief         Button detection (including software key to remove jitter)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void key_scan()
{
  while (digitalRead(key));       //When the button not press, it while
  while (!digitalRead(key))       //When the button press
  {
    delay(10);	                  //delay 10ms
    if (digitalRead(key)  ==  LOW)//The second time to determine whether the button is pressed
    {
      delay(100);
      while (!digitalRead(key));  //Test whether the key is released 
    }
  }
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         Call the key scan function ,
*                 Open the ultrasonic obstacle avoidance mode with the actuator pan tilt,
*                 Infrared obstacle avoidance module is used to assist obstacle avoidance 
*                 (ultrasonic obstacle avoidance is blind)
* @param[in]     void
* @retval        void
* @par History    no
*/
void loop()
{
  Distance_test();        
  if (distance > 30  )    //When the obstacle distance is greater than 50,
                          // the left and right infrared rays are used to assist the obstacle avoidance
  {
    //When the obstacle is encountered, the infrared obstacle avoidance module light up,the port level is LOW
    //When no obstacle, the infrared obstacle avoidance module light out, the port level is HIGH
    LeftSensorValue = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(200); ////There's an obstacle on the right.There's a signal to go back and turn left
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(200);//There's an obstacle on the left.There's a signal to go back and turn right
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(200);//When the obstacle is detected on both sides, 
                     //the obstacle avoidance in the fixed direction is called(turn right in place)
    }
    //When the distance is greater than 50, go ahead and turn on the green light
    run(200, 200);
    corlor_led(OFF, ON, OFF);
  }
  else if ((distance >= 20 && distance <= 30))
  {
    //When the obstacle is encountered, the infrared obstacle avoidance module light up,the port level is LOW
    //When no obstacle, the infrared obstacle avoidance module light out, the port level is HIGH
    LeftSensorValue = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(200); //There's an obstacle on the right.There's a signal to go back and turn left
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(200);//There's an obstacle on the left.There's a signal to go back and turn right
    }
    else if (RightSensorValue == LOW && LeftSensorValue == LOW)
    {
      spin_right(200);//When the obstacle is detected on both sides, 
                    //the obstacle avoidance in the fixed direction is called(turn right in place)
    }
    //When distance is between 30-50,run slowly
    run(100, 100);
  }
  else if (  distance < 20  )//When the distance is less than 30, call the servo color control program
  {
    servo_color_carstate();
  }
}




