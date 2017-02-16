/***************************

 ***************************/

//import ax12 library to send DYNAMIXEL commands
#include <ax12.h>

#include "Motion.h"

#define AX_GOAL_SPEED_L 32

double test = 0;
double Input = 90;


Section Section1 = Section(1, 5, 3, 8, 1);
Section Section2 = Section(2, 6, 7, 4, 3);
Section Section3 = Section(3, 9, 10, 11, 5);


void setup()
{
  Serial.begin(9600);
  Serial.print("RESET");
  pinMode(LED_BUILTIN, OUTPUT);

  Section1.m_ServoSetup();
  Section2.m_ServoSetup();
  Section3.m_ServoSetup();
  delay(1000);

}

void loop()
{
 
  Section1.m_PIDLoop(Input);
  
  Serial.print("m_PIDOutput:");
  Serial.print(Section1.m_PIDOutput);

  Serial.print("Int Sum:");
  Serial.print(Section1.m_dIntSum*KI);
  
  
  Serial.print("Current position:");
  Serial.println(Section1.m_dCurrentPosition);

  //forwardsSerpentine(&Section1, &Section2, &Section3);

  if (Serial.available() > 0);
  {
    test = Serial.parseInt(); //read int or parseFloat for ..float...
    if (test != 0)
    {
      Input = test;
    }
  }



 

}









