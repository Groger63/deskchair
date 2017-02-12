//motor.cpp
/*
  Motor.cpp - Library for using a motor back/forward with a potentiometer to reach a position
  Created by Roger 23/01/2017
  Released into the public domain.
*/

#include "motor.h"
#include "Arduino.h"


Motor::Motor(int _MOTOR_BACK,int _MOTOR_FORWARD, int _POTENTIOMETER, int _MARGIN_MOTOR)
{
	POTENTIOMETER=_POTENTIOMETER;
	MOTOR_BACK = _MOTOR_BACK;
	MOTOR_FORWARD = _MOTOR_FORWARD;
	MARGIN_MOTOR = _MARGIN_MOTOR;
	position = 0;

  	pinMode(MOTOR_BACK, OUTPUT);     
 	pinMode(MOTOR_FORWARD, OUTPUT);   
};



Motor::~Motor(){};

void Motor::move_to(int new_pos)
{
	int position_pot =0 ;
	int arrived =0 ;

    while(!arrived){
          
      position_pot = analogRead(POTENTIOMETER);    // read the value from the sensor
      
      if(position_pot < new_pos + MARGIN_MOTOR && position_pot >new_pos - MARGIN_MOTOR)
      {
        arrived = 1 ;
       
      }
      if(position_pot < (new_pos -MARGIN_MOTOR)){
        digitalWrite(MOTOR_BACK, HIGH);   // Activate motor back
        digitalWrite(MOTOR_FORWARD, LOW);    // stop motor forward
      }     
      if(position_pot > (new_pos + MARGIN_MOTOR)){
        digitalWrite(MOTOR_FORWARD, HIGH);    // Activate motor forward
        digitalWrite(MOTOR_BACK, LOW);    // stop motor back
      }
    }

    digitalWrite(MOTOR_FORWARD, LOW);    // stop both motors once arrived
    digitalWrite(MOTOR_BACK, LOW);    
  
};