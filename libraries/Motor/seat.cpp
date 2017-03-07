//seat.cpp
/*
  Motor.h - Library for using a seat composed by 4 motors to adjust 4 position 
  Created by Roger 07/03/2017
  Released into the public domain.
*/

#include "seat.h"
#include "Arduino.h"


Seat::Seat(Motor _motor_dossier, Motor _motor_assise, Motor _motor_hauteur, Motor _motor_avancement )
{
	motor_dossier=_motor_dossier;
	motor_assise = _motor_assise;
	motor_hauteur = _motor_hauteur;
	motor_avancement = _motor_avancement;
};


Motor::~Motor(){};


void Seat::move_to(int dossier, int assise, int hauteur, int avancement){

	motor_dossier.move_to(dossier);
	motor_assise.move_to(assise);
	motor_hauteur.move_to(hauteur);
	motor_avancement.move_to(avancement);

	//version simple ou les moteurs s'activent un par un


} //activate motors to move to the chosen position