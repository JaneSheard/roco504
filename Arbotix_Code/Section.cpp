#include "Section.h"




Section::Section(int ID, int ServoID1, int ServoID2, int ServoID3, int FlexSensorPin1)
{
  m_iID = ID;
  m_iServoID1 = ServoID1;
  m_iServoID2 = ServoID2;
  m_iServoID3 = ServoID3;
  m_iFlexSensorRight = FlexSensorPin1;
  m_iFlexSensorLeft = FlexSensorPin1 + 1;
  m_iLeftFlex = 0;
  m_iRightFlex = 0;
  m_iMotorLoadRight = 0;
  m_iMotorLoadLeft = 0;
  PIDSetpoint = 0.0;
  m_dTargetPosition = 90.0;
  m_dCurrentPosition = 0.0;
  m_iNegativeAngle = 0;
  m_PIDOutput = 0;


}

void Section::m_ServoSetup()
{
  //set both servos Clockwise and Counter-Clockwise limits to 0. this sets them in wheel mode and allows continuous rotation.
  for (int registerID = 6; registerID <= 9; registerID += 2)
  {
    ax12SetRegister2(m_iServoID1, registerID, 0); // write two bytes to memory (ADDR, ADDR+1)
    ax12SetRegister2(m_iServoID2, registerID, 0); // write two bytes to memory (ADDR, ADDR+1)

  }

}

void Section::m_GetServoPositions()
{
  m_dServoPosition1 = GetPosition(m_iServoID1);
  m_dServoPosition2 = GetPosition(m_iServoID2);
}

double Section::m_PIDLoop(double Setpoint)
{
  //Calculate angle
  m_GetFlexPosition();
  //calculate tension
  // m_GetTension();

  //pull harder if angle is greater, while also letting go less, should help to keep tension on both sides
  m_dError = Setpoint - m_dCurrentPosition;

  if (abs(m_dError) < 2)
  {
    m_PIDOutput = 0;
  }
  else
  {
    m_dIntSum = m_dIntSum + (m_dError * 0.01);
    m_dProportional = (KP * m_dError);
    m_dIntegral = (KI * m_dIntSum);
    m_dDerivative = (KD * ((m_dError - m_dLastError) / 0.01));

    if (m_dIntegral >= 250)
    {
      m_dIntegral = 250;
    }
    else if (m_dIntegral <= -250)
    {
      m_dIntegral = -250;
    }

    m_PIDOutput = m_dProportional + m_dIntegral + m_dDerivative;

    if (m_PIDOutput >= 1023)
    {
      m_PIDOutput = 1023;
    }

    if (m_PIDOutput < 0)
    {
      m_PIDOutput = -m_PIDOutput;
      m_iNegativeAngle = 1;
    }
    else
    {
      m_iNegativeAngle = 0;
    }
  }

  if (m_iNegativeAngle == 0)
  {
    ax12SetRegister2(m_iServoID1, 32, 1024 + (m_PIDOutput / 2));

    ax12SetRegister2(m_iServoID2, 32, 1024 + m_PIDOutput);
  }
  else
  {
    ax12SetRegister2(m_iServoID1, 32, m_PIDOutput);

    ax12SetRegister2(m_iServoID2, 32, (m_PIDOutput / 2));
  }
  m_dLastError = m_dError;
  return m_dError;
}

void Section::m_brakeControl(int Position)
{
  SetPosition(m_iServoID3, Position);
}

void Section::m_GetFlexPosition()
{
  int flexAngle = 0;
  m_iRightFlex = analogRead(m_iFlexSensorRight);
  m_iLeftFlex  = analogRead(m_iFlexSensorLeft);

  m_dCurrentPosition = 7 + ((m_iRightFlex * 1.2256) - 535) * 0.5 + ((m_iLeftFlex * -1.2789) + 757) * 0.5;
   
}

void Section::m_GetTension()
{
  m_iMotorLoadRight = ax12GetRegister(m_iServoID1, AX_PRESENT_LOAD_L, 2);
  m_iMotorLoadLeft =  ax12GetRegister(m_iServoID2, AX_PRESENT_LOAD_L, 2);

}

void Section::m_GetSerialData()
{
  m_dTargetPosition = Serial.parseFloat();
}
