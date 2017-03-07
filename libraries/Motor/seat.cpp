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


void Seat::move_to(int assise, int avancement, int hauteur, int dossier){

	motor_dossier.move_to(dossier);
	motor_assise.move_to(assise);
	motor_hauteur.move_to(hauteur);
	motor_avancement.move_to(avancement);

	//version simple ou les moteurs s'activent un par un




}; //activate motors to move to the chosen position