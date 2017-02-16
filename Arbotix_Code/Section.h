#include <ax12.h>

#ifndef SECTION_H
#define SECTION_H

#define KP  20
#define KI 1000
#define KD 0

class Section
{
  public:

    Section(int ID, int ServoID1, int ServoID2, int ServoID3, int FlexSensorPin1);

    double m_PIDLoop(double Setpoint);
    void   m_GetFlexPosition();
    void   m_GetSerialData();
    void   m_GetTension();
    void   m_ServoSetup();
    void   m_GetServoPositions();
    void   m_brakeControl(int Position);

    int    m_iID;
    int    m_iServoID1;
    int    m_iServoID2;
    int    m_iServoID3;
    int    m_iLeftFlex;
    int    m_iRightFlex;
    int    m_iFlexSensorRight;
    int    m_iFlexSensorLeft;
    
    int    m_iMotorLoadRight;
    int    m_iMotorLoadLeft;
    int    m_iNegativeAngle;
     
    
    double PIDSetpoint;
    double m_dTargetPosition;
    double m_dCurrentPosition;
    double m_dServoPosition1;
    double m_dServoPosition2;
    

    double m_dError;
    double m_dLastError;
    double m_PIDOutput;
    double m_dIntSum;
    double m_dProportional;
    double m_dIntegral;
    double m_dDerivative;

};


extern Section Section1;
extern Section Section2;
extern Section Section3;


#endif


