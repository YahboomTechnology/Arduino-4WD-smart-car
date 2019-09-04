/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         advance.c
* @author       Danny
* @version      V1.0
* @date         2017.07.25
* @brief        Car run
* @details
* @par History  See the following instructions
*
*/
//Define pin
int Left_motor_go = 8;       // AIN1
int Left_motor_back = 7;     // AIN2

int Right_motor_go = 2;      // BIN1
int Right_motor_back = 4;    // BIN2

int Left_motor_pwm = 6;      //Left motor speed control PWMA
int Right_motor_pwm = 5;     //Right motor speed control PWMB

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
  // Initialize the motor PIN IO 
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
* @param[in]     time(delay)
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(int time)
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);    //enable left motor go
  digitalWrite(Left_motor_back, LOW);   //prohibit left motor back
  analogWrite(Left_motor_pwm, 200);    

  //right motor go
  digitalWrite(Right_motor_go, HIGH);   //enable right motor go
  digitalWrite(Right_motor_back, LOW);  //prohibit right motor back
  analogWrite(Right_motor_pwm, 200);   

 
  delay(time * 100); //uint:ms
}

/**
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         Delay 2s first, then run 1s
* @param[in]     void
* @retval        void
* @par History   none
*/
void loop()
{
  delay(2000);     //delay 2s
  run(10);         //run 1s
}
