/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         CarRun.c
* @author       Danny
* @version      V1.0
* @date         2017.07.25
* @brief       Car run,back,left,right
* @details
* @par History   See the following instructions
*
*/
//Define pin
int Left_motor_go = 8;        // AIN1
int Left_motor_back = 7;      // AIN2

int Right_motor_go = 2;       //BIN1
int Right_motor_back = 4;     // BIN2

int Left_motor_pwm = 6;       //Left motor speed control PWMA
int Right_motor_pwm = 5;      //Right motor speed control PWMB

/**
* Function       setup
* @author        Danny
* @date          2017.07.25
* @brief        Initialization configuration
* @param[in]     void
* @retval        void
* @par History   无
*/
void setup()
{
  //Initialize the motor PIN IO 
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
}

/**
* Function       run
* @author        Danny
* @date          2017.07.25
* @brief         Car run
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(int time)
{

  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, 200);    

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 200);   

  //延时
  delay(time * 100);
}

/**
* Function       brake
* @author        Danny
* @date          2017.07.25
* @brief         car brake
* @param[in]     time
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
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   no
*/
void left(int time)
{
   //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, LOW);   //prohibit left motor back
  analogWrite(Left_motor_pwm, 0);       

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 200);   

 
  delay(time * 100);
}

/**
* Function       right
* @author        Danny
* @date          2017.07.25
* @brief          Turn right (right wheel stop, left wheel go)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   no
*/
void right(int time)
{
   //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, 150);

  //right motor back 
  digitalWrite(Right_motor_go, LOW);   //prohibit right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 0);

  delay(time * 100);
}

/**
* Function       spin_left
* @author        Danny
* @date          2017.07.25
* @brief          Turn left in place(left wheel back,right wheel go)
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   无
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

  //延时
  delay(time * 100);
}

/**
* Function       spin_right
* @author        Danny
* @date          2017.07.25
* @brief        Turn right in place(right wheel back,left wheel go)
* @param[in]     time
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
* @param[in]     time
* @param[out]    void
* @retval        void
* @par History   no
*/
void back(int time)
{
  //left motor back
  digitalWrite(Left_motor_go, LOW);     //prohibit left motor go
  digitalWrite(Left_motor_back, HIGH);  //enable left motor back
  analogWrite(Left_motor_pwm, 200);

  //right motor back
  digitalWrite(Right_motor_go, LOW);    //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH); //enable right motor back
  analogWrite(Right_motor_pwm, 200);

 
  delay(time * 100);
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         delay 2s，run 1s，back 1s,turn left 2s,turn right2s,
*                turn left in place 3s,turn right in place 3s,stop 0.5s
* @param[in]     void
* @retval        void
* @par History   无
*/
void loop()
{
  delay(2000);      //delay 2s
  run(10);          //run 1s(10 * 100ms)
  back(10);         //back 1s
  left(20);         //turn left 2s
  right(20);        //turn right 2s
  spin_left(30);    //turn left in place 3s
  spin_right(30);   //turn right in place 3s
  brake(5);         //stop 0.5s
}
