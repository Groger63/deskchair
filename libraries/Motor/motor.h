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

class Motor{
	private :
		int MOTOR_BACK; //pin assigned to motor back
		int MOTOR_FORWARD; //pin assigned to motor forward
		int POTENTIOMETER ; //pin assigned to potentiometer which determines the position
		int MARGIN_MOTOR ;
                

		void go_forward(); //activate motors to move to the chosen position
		void go_backward(); //activate motors to move to the chosen position
		void go_stop(); //activate motors to move to the chosen position

	public :
	
        int moving_status ;
		short position ; //position of the motor. Varies from 0 to 1023

		Motor(int _MOTOR_BACK,int _MOTOR_FORWARD, int _POTENTIOMETER, int _MARGIN_MOTOR);
		~Motor();

		void move_to(int new_pos); //activate motors to move to the chosen position
};

#endif
