//motor.h
/*
  Motor.h - Library for using a motor back/forward with a potentiometer to reach a position
  Created by Roger 23/01/2017
  Released into the public domain.
*/
#ifndef Motor_h
#define Motor_h

#define BACK 0
#define FORWARD 1
#define STOP 2 

#include "Arduino.h"
#include <shiftreg.h>

class Motor{
	private :
		int MOTOR_BACK; //pin assigned to motor back
		int MOTOR_FORWARD; //pin assigned to motor forward
		int POTENTIOMETER ; //pin assigned to potentiometer which determines the position
		int MARGIN_MOTOR ;

        ShiftReg *myShiftReg ; 


	public :
	
		int go_forward(); //activate motors to move to the chosen position
		int go_backward(); //activate motors to move to the chosen position
		int go_stop(); //activate motors to move to the chosen position

		int get_position();//basically just returns the value of the pot
		bool is_arrived(int position);
		int select_direction(int position);
		
        byte moving_status ;
		short position ; //position of the motor. Varies from 0 to 1023

		Motor(int _MOTOR_BACK,int _MOTOR_FORWARD, int _POTENTIOMETER, int _MARGIN_MOTOR, ShiftReg *SR);
		Motor();
		Motor(const Motor &mot);
		~Motor();

		void move_to(int new_pos); //activate motors to move to the chosen position
};

#endif
