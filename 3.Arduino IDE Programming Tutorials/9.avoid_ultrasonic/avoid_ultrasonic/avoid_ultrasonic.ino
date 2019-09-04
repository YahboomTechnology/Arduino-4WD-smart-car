/**
* @par Copyright (C): 2010-2019, Shenzhen Yahboom Tech
* @file         avoid_ultrasonic.c
* @author       Danny
* @version      V1.0
* @date         2017.07.26
* @brief        Ultrasonic obstacle avoidance
* @details
* @par History  See the following instructions
*
*/
//Define pin
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
  //Set the baud rate of serial data transmission is 9600
  Serial.begin(9600);

  //Initialize the motor PIN IO 
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);

  //Define button pin as input interface
  pinMode(key, INPUT);
  
  //Define sensor as input interface
  digitalWrite(key, HIGH);

  //Define ultrasonic pin
  pinMode(EchoPin, INPUT);  
  pinMode(TrigPin, OUTPUT);   

  //Call key scan fuction
  key_scan();
}

/**
* Function       run
* @author        Danny
* @date          2017.07.26
* @brief         car run
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void run(int left_speed, int right_speed)
{
  //left motor go
  digitalWrite(Left_motor_go, HIGH);   //enable left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motro back
  analogWrite(Left_motor_pwm, left_speed );
  
  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, right_speed);
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
* @param[in1]    left_speed
* @param[in2]    right_speed
* @param[out]    void
* @retval        void
* @par History   no
*/
void left(int left_speed, int right_speed)
{
  //left motor stop
  digitalWrite(Left_motor_go, LOW);    //prohibit left motor go
  digitalWrite(Left_motor_back, LOW);  //prohibit left motor back
  analogWrite(Left_motor_pwm, left_speed);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, right_speed);
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
  analogWrite(Left_motor_pwm, 150);

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
  analogWrite(Left_motor_pwm, 160);

  //right motor go
  digitalWrite(Right_motor_go, HIGH);  //enable right motor go
  digitalWrite(Right_motor_back, LOW); //prohibit right motor back
  analogWrite(Right_motor_pwm, 160);
  
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
  analogWrite(Left_motor_pwm,160);

   //right motor back
  digitalWrite(Right_motor_go, LOW);    //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH); //enable right motor back
  analogWrite(Right_motor_pwm, 160);
  
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
  analogWrite(Left_motor_pwm, 40);

  //right motor back
  digitalWrite(Right_motor_go, LOW);     //prohibit right motor go
  digitalWrite(Right_motor_back, HIGH);  //enable right motor back
  analogWrite(Right_motor_pwm, 40);

  delay(time );
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
    //Filter out the error data in the test distance is greater than 500
    while (distance >= 500)
    {
      brake(0);
      Distance();
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
* Function       loop
* @author        Danny
* @date          2017.07.25
* @brief         Call setup to initialize the key scan function inside the configuration,
*                Ultrasonic obstacle avoidance mode opens
* @param[in]     void
* @retval        void
* @par History   no
*/
void loop()
{
  Distance_test();
  if (distance > 50)
  {
    run(200, 200);      //When the distance is far away, run at full speed
  }
  else if (distance >= 25 && distance <= 50)
  {
    run(100, 100);      //When the distance is near,slow down 
  }
  else if (distance < 25)
  {
    spin_right(3.5);    //When approaching obstacles,turn right at about 90 degrees
    brake(1);
    Distance_test();    //Test again to determine the distance ahead
    if (distance >= 25)
    {
      run(100, 100);    //When the distance is greater than 25cm after turning,run
    }
    else if (distance < 25)
    {
      spin_left(7);    //When the distance less than 25cm after turning,turn left 180 degrees in place
      brake(1);
      Distance_test(); //Test again to determine the distance ahead
      if (distance >= 25)
      {
        run(100, 100); //When the distance is greater than 25cm after turning,run
      }
      else if (distance < 25)
      {
        spin_left(3.5);//When the distance less than 25cm after turning,turn right 90 degrees in place
        brake(1);
      }
    }
  }
}
