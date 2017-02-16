#ifndef MOTION_H
#define MOTION_H

#define BRAKEUPUP 0
#define BRAKEDOWN 55

#include <Arduino.h> //needed for Serial.println
#include "Section.h"

int angleQuadrant;

double sineAngle1;
double sineAngle2;
double sineAngle3;
double PIDError;


double Section1Angle;
double Section2Angle;
double Section3Angle;

void forwardsSerpentine(Section* pSection1, Section* pSection2, Section* pSection3)
{ //sine wave 0-180. out of phase for each section to produce serpentine motion.

  //wave generation
  switch (angleQuadrant)
  {
    case 1:
      Section1Angle = (asin(sineAngle1) * (180 / PI)) + 90;
      sineAngle1 += 0.01;
      Section2Angle = (asin(sineAngle2) * (180 / PI)) + 90;
      sineAngle2 -= 0.01;
      Section3Angle = (asin(sineAngle3) * (180 / PI)) + 90;
      sineAngle3 -= 0.01;
      if (sineAngle1 >= 0.99)
      {
        angleQuadrant = 2;
      }
      break;

    case 2:
      Section1Angle = (asin(sineAngle1) * (180 / PI)) + 90;
      sineAngle1 -= 0.01;
      Section2Angle = (asin(sineAngle2) * (180 / PI)) + 90;
      sineAngle2 -= 0.01;
      Section3Angle = (asin(sineAngle3) * (180 / PI)) + 90;
      sineAngle3 += 0.01;
      if (sineAngle1 < 0.01)
      {
        angleQuadrant = 3;
      }
      break;

    case 3:
      Section1Angle = (asin(sineAngle1) * (180 / PI)) + 90;
      sineAngle1 -= 0.01;
      Section2Angle = (asin(sineAngle2) * (180 / PI)) + 90;
      sineAngle2 += 0.01;
      Section3Angle = (asin(sineAngle3) * (180 / PI)) + 90;
      sineAngle3 += 0.01;
      if (sineAngle1 < -0.99)
      {
        angleQuadrant = 4;
      }
      break;

    case 4:
      Section1Angle = (asin(sineAngle1) * (180 / PI)) + 90;
      sineAngle1 += 0.01;
      Section2Angle = (asin(sineAngle2) * (180 / PI)) + 90;
      sineAngle2 += 0.01;
      Section3Angle = (asin(sineAngle3) * (180 / PI)) + 90;
      sineAngle3 -= 0.01;
      if (sineAngle1 >= -0.01)
      {
        angleQuadrant = 1;
      }
      break;
    default:
      Section1Angle = 90;
      Section2Angle = 90;
      Section3Angle = 90;
      digitalWrite(LED_BUILTIN, HIGH);
  }
  
  pSection1->m_PIDLoop(Section1Angle);
  pSection2->m_PIDLoop(Section2Angle);
  pSection3->m_PIDLoop(Section3Angle);


}

void forwardsConcertina() //https://40.media.tumblr.com/438afeb6fd3df610ae8b9530b92ae8d5/tumblr_nxzciquyAN1ulq7lso1_1280.jpg
{
  //Section1.m_brakeControl(BRAKEDOWN); //this value needs testing before it DESTROYS the snake! (that would be bad)
  //Section1.m_PIDLoop(180);

  Section2.m_PIDLoop(0);    //curl up, nice and snug, in a danger noodle ball
  Section3.m_PIDLoop(180);
  delay(1000);
  //Section3.m_brakeControl(BRAKEDOWN);  //plant the arse, prep the snoot for booping
  //Section1.m_brakeControl(BRAKEUP);
  //Section1.m_PIDLoop(90);
  Section2.m_PIDLoop(90);   //uncurl, no longer snug. sad snek.
  Section3.m_PIDLoop(90);
  delay(1000);
  //Section3.m_brakeControl(BRAKEUP);
}

void SIDEWINDER()
{
  //Section1.m_brakeControl(BRAKEDOWN);
  //Section3.m_brakeControl(BREAKITDOWN);   //front&rear brakes down
  do  {
    PIDError = Section3.m_PIDLoop(180);
  } while (PIDError > 50);
  do {                                      //middle section to one side
    PIDError = Section2.m_PIDLoop(0);
  } while (PIDError > 50);
  //Section2.m_brakeControl(BRAKEDOWN);
  //Section1.m_brakeControl(BRAKEUP);       //middle section brake down, front brake up
  do {
    PIDError = Section2.m_PIDLoop(90);        //move front section to line with middle
  } while (PIDError > 50);
  //Section1.m_brakeControl(BRAKEDOWN);
  //Section3.m_brakeControl(BRAKEUP);       //front brake down, rear break up
  do  {
    PIDError = Section3.m_PIDLoop(90);        //bring rear section up to front
  } while (PIDError > 50);
  //Section1.m_brakeControl(BRAKEUP);       //all brakes up, in new position, readyb to start again
  //Section2.m_brakeControl(BRAKEUP);

}

void backwards()
{
  //reverse sine wave?
}
void left()
{
  //adds offset to sine wave to create left turn?
}

void right()
{
  //adds offset to sinewave to create right turn?
}



#endif
