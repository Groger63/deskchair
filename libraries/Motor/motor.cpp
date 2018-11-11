//motor.cpp
/*
  Motor.cpp - Library for using a motor back/forward with a potentiometer to reach a position
  Created by Roger 23/01/2017
  Released into the public domain.
*/

#include "motor.h"
#include "Arduino.h"


Motor::Motor(int _MOTOR_BACK,int _MOTOR_FORWARD, int _POTENTIOMETER, int _MARGIN_MOTOR, ShiftReg *SR): myShiftReg(SR)
{
	POTENTIOMETER=_POTENTIOMETER;
	MOTOR_BACK = _MOTOR_BACK;
	MOTOR_FORWARD = _MOTOR_FORWARD;
	MARGIN_MOTOR = _MARGIN_MOTOR;
	position = 0;


  go_stop();
};

Motor::Motor(){};

Motor::Motor(const Motor &mot){};

Motor::~Motor(){};

int Motor::get_position()
{
  return analogRead(POTENTIOMETER);
};//basically just returns the value of the pot


int Motor::go_forward()
{
  if(moving_status == FORWARD) return FORWARD;
  moving_status = FORWARD;

  myShiftReg->setRegisterPin(MOTOR_FORWARD, LOW);    // Activate motor forward
  myShiftReg->setRegisterPin(MOTOR_BACK, HIGH);    // stop motor back

  myShiftReg->writeRegisters() ;

  return FORWARD ;
};

int Motor::go_backward()
{
  if(moving_status == BACK) return BACK;

  moving_status = BACK;
  myShiftReg->setRegisterPin(MOTOR_FORWARD,HIGH);    // Activate motor forward
  myShiftReg->setRegisterPin(MOTOR_BACK, LOW);    // stop motor back
  
  myShiftReg->writeRegisters() ;

  return BACK ;
};

int Motor::go_stop()
{
  if(moving_status == STOP) return STOP;

  moving_status = STOP;
  myShiftReg->setRegisterPin(MOTOR_FORWARD,HIGH);    // Activate motor forward
  myShiftReg->setRegisterPin(MOTOR_BACK, HIGH);    // stop motor back
  
  myShiftReg->writeRegisters() ;

  return STOP ;
};

int Motor::select_direction(int position)
{
/*  if (is_arrived(position))
    return go_stop() ;
  else if ((get_position() < position) )
  {
    return go_backward() ;
  }
  return go_forward() ;*/
  return is_arrived(position) ? go_stop() : ((get_position() < position) ? go_backward() : go_forward() );
}

bool Motor::is_arrived(int position)
{
  return ( get_position() < position + MARGIN_MOTOR && get_position() > position - MARGIN_MOTOR ) ;
}

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
        go_backward();
      }     
      if(position_pot > (new_pos + MARGIN_MOTOR)){
        go_forward();
      }
    }

    go_stop();   
  
}; // old, can be removed