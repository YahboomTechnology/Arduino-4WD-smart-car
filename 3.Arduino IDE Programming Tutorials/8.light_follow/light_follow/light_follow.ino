/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         light_follow.c
* @author       Danny
* @version      V1.0
* @date         2017.07.25
* @brief        seek light
* @details
* @par History  See the following instructions
*
*/
//Define pin
int Left_motor_go = 8;         // AIN1
int Left_motor_back = 7;       // AIN2

int Right_motor_go = 2;        // BIN1
int Right_motor_back = 4;      // BIN2

int Left_motor_pwm = 6;        //left motor speed control  PWMA
int Right_motor_pwm = 5;       //right motor speed control  PWMB

int key = A0;                  //Define button as analog port A0 with Arduino
const int LdrSensorLeft =  A4;   //Define the left side of the photosensitive resistor pin as A4
const int LdrSensorRight = A2;   //Define the right side of the photosensitive resistor pin as A2

int LdrSersorLeftValue ;         //Define variables to store the data collected by the photosensitive resistor
int LdrSersorRightValue ;

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
  //Initialize the motor PIN IO 
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);

  //Define button pin as input interface
  pinMode(key, INPUT);
  
  //Define photosensitive resistor as input interface
  pinMode(LdrSensorLeft, INPUT);
  pinMode(LdrSensorRight, INPUT);

  //Initialize button is level high
  digitalWrite(key, HIGH);
  
  //Initialize photosensitive resistor is level high
  digitalWrite(LdrSensorLeft, HIGH);
  digitalWrite(LdrSensorRight, HIGH);

  //Call key scan fuction
  key_scan();
}

/**
* Function       run
* @author        Danny
* @date          2017.07.25
* @brief         car run
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void run()
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_pwm, 255);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 255);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         car brake
* @param[in]     void
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
  analogWrite(Right_motor_pwm, 255);
}

/**
* Function       right
* @author        Danny
* @date          2017.07.25
* @brief          Turn right (right wheel stop, left wheel go)
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
  analogWrite(Left_motor_pwm, 255);

  //right motor stop
  digitalWrite(Right_motor_go, LOW);   //prohibit right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 0);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.07.25
* @brief          Turn left in place(left wheel back,right wheel go)
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

  delay(time * 100);
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

  delay(time * 100);
}

/**
* Function       back
* @author        Danny
* @date          2017.07.25
* @brief         car back
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   no
*/
void back()
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable lef tmotor back
  analogWrite(Left_motor_pwm, 80);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH);  //enable right motor back
  analogWrite(Right_motor_pwm, 80);
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
  while (!digitalRead(key))       //When the button pres
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
* @brief         First call the setup initialization configuration inside the key scan function, 
                 and then seek light mode open
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
  //When the light is encountered, the indicator of the optical module is turned off and the port level is HIGH
  //When the light is not encountered, the indicator of the optical module is turned off and the port level is LOW
  LdrSersorRightValue = digitalRead(LdrSensorRight);
  LdrSersorLeftValue  = digitalRead(LdrSensorLeft);

  if (LdrSersorLeftValue == HIGH && LdrSersorRightValue == HIGH)
  {
    run();   //When both sides have light, the signal is HIGH,
             // the photosensitive resistor light out, and the car run                                  
  }
  else if (LdrSersorLeftValue == HIGH && LdrSersorRightValue == LOW)       
  {
    left(); //There's light on the left.There's a signal to go back and turn left
  }
  else if (LdrSersorRightValue == HIGH && LdrSersorLeftValue == LOW)       
  {
    right();//There's light on the right.There's a signal to go back and turn right
  }
  else         
  {
    brake();//There's not light,car brake
  }
}
