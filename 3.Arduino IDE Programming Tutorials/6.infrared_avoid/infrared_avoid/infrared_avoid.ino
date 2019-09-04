/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         infrared_avoid.c
* @author       Danny
* @version      V1.0
* @date         2017.07.25
* @brief        Infrared obstacle avoidance
* @details
* @par History  See the following instructions
*
*/
//Define pin
int Left_motor_go = 8;        //AIN1
int Left_motor_back = 7;      // AIN2

int Right_motor_go = 2;       // BIN1
int Right_motor_back = 4;     // BIN2

int Left_motor_pwm = 6;       //left motor speed control PWMA
int Right_motor_pwm = 5;      //right motor speed control PWMB

int key = A0;                 //Define button as analog port A0 with Arduino 

const int AvoidSensorLeft =  A3;   //Define left infrared obstacle avoidance sensor pin A3
const int AvoidSensorRight = A1;   //Define right infrared obstacle avoidance sensor pin A1
int LeftSensorValue ;              //Define variables to store data collected by photosensitive resistor
int RightSensorValue ;

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

  //Define sensor as input interface
  pinMode(AvoidSensorLeft, INPUT);
  pinMode(AvoidSensorRight, INPUT);

  //Initialize button is level high
  digitalWrite(key, HIGH);

  //Initialize sensor is level high
  digitalWrite(AvoidSensorLeft, HIGH);
  digitalWrite(AvoidSensorRight, HIGH);
  
  //Call key scan fuction
  key_scan();   
}
/**
* Function       run
* @author        Danny
* @date          2017.07.25
* @brief        car run
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
  analogWrite(Left_motor_pwm, 250);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 250);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         car brake
* @param[in]     time:delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void brake(int time)
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  delay(time * 100);
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
* @param[in]     time：delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void spin_left(int time)
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable left motor back
  analogWrite(Left_motor_pwm, 150);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 150);

  delay(time * 100);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.07.25
* @brief         Turn right in place(right wheel back,left wheel go)
* @param[in]     time：delay
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
* @param[in]     time：delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void back(int time)
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable lef tmotor back
  analogWrite(Left_motor_pwm, 150);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH);  //enable right motor back
  analogWrite(Right_motor_pwm, 150);

  delay(time * 100);
}

/**
* Function       key_scan
* @author        Danny
* @date          2017.07.25
* @brief        Button detection (including software key to remove jitter)
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
* @brief         First call the setup initialization configuration inside the key scan function, 
                 and then infrared obstacle avoidance mode open
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{ 
    //When the obstacle is encountered, the infrared obstacle avoidance module light up,the port level is LOW
    //When no obstacle, the infrared obstacle avoidance module light out, the port level is HIGH
    LeftSensorValue  = digitalRead(AvoidSensorLeft);
    RightSensorValue = digitalRead(AvoidSensorRight);

    if (LeftSensorValue == HIGH && RightSensorValue == HIGH)
    {
      run();//When the obstacle is not detected on both sides, the forward function is called
    }
    else if (LeftSensorValue == HIGH && RightSensorValue == LOW)
    {
      spin_left(2); //There's an obstacle on the right.There's a signal to go back and turn left
    }
    else if (RightSensorValue == HIGH && LeftSensorValue == LOW)
    {
      spin_right(2);//There's an obstacle on the left.There's a signal to go back and turn right
    }
    else
    {
      spin_right(2);//When the obstacle is detected on both sides, 
                    //the obstacle avoidance in the fixed direction is called(turn right in place)
    }
}

