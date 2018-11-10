//seat.cpp
/*
  Motor.h - Library for using a seat composed by 4 motors to adjust 4 position 
  Created by Roger 07/03/2017
  Released into the public domain.
*/

#include "seat.h"
#include "Arduino.h"


Seat::Seat(const Motor &assise, const Motor &avancement, const Motor &hauteur, const Motor &dossier)
{
	motor_dossier= dossier;
	motor_assise = assise;
	motor_hauteur = hauteur;
	motor_avancement = avancement;
};


Seat::~Seat(){};


void Seat::move_to(int assise, int avancement, int hauteur, int dossier)
{
	while(
		motor_dossier.select_direction(dossier) || 
		motor_avancement.select_direction(avancement) || 
		motor_hauteur.select_direction(hauteur) || 
		motor_assise.select_direction(assise)
		);
}; //activate motors to move to the chosen position