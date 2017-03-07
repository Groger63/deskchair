//seat.h
/*
  Motor.h - Library for using a seat composed by 4 motors to adjust 4 position 
  Created by Roger 06/03/2017
  Released into the public domain.
*/
#ifndef Seat_h
#define Seat_h

#define BACK 0
#define FORWARD 1
#define STOP 2 

#include "Arduino.h"
#include "Motor.h"

class Seat{
	private :
		Motor motor_dossier;
		Motor motor_assise;
		Motor motor_hauteur ;
		Motor motor_avancement ;

	public :

		Seat(Motor motor_dossier, Motor motor_assise, Motor motor_hauteur, Motor motor_avancement );
		~Seat();

		void move_to(int dossier, int assise, int hauteur, int avancement); //activate motors to move to the chosen position
};

#endif
