/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         keysacnStart.c
* @author       Danny
* @version      V1.0
* @date         2017.07.25
* @brief        Button control car
* @details
* @par History  See the following instructions
*
*/
//Define pin
int Left_motor_go = 8;         // AIN1
int Left_motor_back = 7;       //AIN2

int Right_motor_go = 2;        // BIN1
int Right_motor_back = 4;      // BIN2

int Left_motor_pwm = 6;        //left motor speed control PWMA
int Right_motor_pwm = 5;       //right motor speed control PWMB

int key = A0;                  //Define button as analog port A0 with Arduino 

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
  //Initialize the motor PIN IO 
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);

  //Define the button as the input interface
  pinMode(key, INPUT);

  //The button is initialized to a high level
  digitalWrite(key, HIGH);
}

/**
* Function       run
* @author        Danny
* @date          2017.07.25
* @brief         car run
* @param[in]     time：delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(int time)
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_pwm, 200);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 200);

  //delay
  delay(time * 100);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         car brake
* @param[in]     time：delay
* @param[out]    void
* @retval        void
* @par History   no
*/
//brake
void brake(int time)
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  
  //delay
  delay(time * 100);
}

/**
* Function       left
* @author        Danny
* @date          2017.07.25
* @brief         Turn left (left wheel stop, right wheel go)
* @param[in]     time：delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void left(int time)
{
  //left motor stop
  digitalWrite(Left_motor_go, LOW);    //prohibit left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, 0);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 200);

  //delay
  delay(time * 100);
}

/**
* Function       right
* @author        Danny
* @date          2017.07.25
* @brief          Turn right (right wheel stop, left wheel go)
* @param[in]     time：delay
* @param[out]    void
* @retval        void
* @par History   no
*/
void right(int time)
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, 200);

  //right motor stop
  digitalWrite(Right_motor_go, LOW);   //prohibit right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 0);

  delay(time * 100);
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
* @brief          Turn right in place(right wheel back,left wheel go)
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
  analogWrite(Left_motor_pwm, 200);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH); //enable right motor back
  analogWrite(Right_motor_pwm, 200);

  //delay
  delay(time * 100);
}

/**
* Function       key_scan
* @author        Danny
* @date          2017.07.25
* @brief         Button detection (including software key to remove jitter)
* @param[in]     void
* @param[out]    void
* @retval        void
* @par History   无
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
* @brief         Call the key scan function, run 1s, back 1s, turn left 2S, turn right 2s,
*                turn left 3S, turn right 3S, stop 0.5 in place
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
  key_scan();       //Call the key scan function

  run(10);          //run 1s(10 * 100ms)
  back(10);         //back 1s
  left(20);         //turn left2s
  right(20);        //turn right 2s
  spin_left(30);    //turn left in place 3s
  spin_right(30);   //turn right in place 3s
  brake(5);         //stop 0.5s
}
