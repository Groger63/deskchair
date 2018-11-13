//seat.cpp
/*
  Motor.h - Library for using a seat composed by 4 motors to adjust 4 position 
  Created by Roger 07/03/2017
  Released into the public domain.
*/

#include "seat.h"
#include "Arduino.h"
#include <ArduinoJson.h>

Seat::Seat( Motor *assise,  Motor *avancement,  Motor *hauteur,  Motor *dossier)
{
	motor_dossier= dossier;
	motor_assise = assise;
	motor_hauteur = hauteur;
	motor_avancement = avancement;
	aborts = false ;
	moving = false ;
};


Seat::~Seat(){};


void Seat::read_buttons() 
{
	motor_dossier->read_buttons();
	motor_assise->read_buttons();
	motor_hauteur->read_buttons() ;
	motor_avancement->read_buttons() ;
}

/*void Seat::move_to(int assise, int avancement, int hauteur, int dossier)
{
	//bool abort = false ;
	moving = true ;
	while(
			(
				motor_dossier.select_direction(dossier) || 
				motor_avancement.select_direction(avancement) || 
				motor_hauteur.select_direction(hauteur) || 
				motor_assise.select_direction(assise) 
			)
			&&
			!aborts 
		)
		{
			delay(10);

			//and a panic button ?
		}
		if(aborts)
		{
			motor_dossier.go_stop();
			motor_avancement.go_stop();
			motor_hauteur.go_stop();
			motor_assise.go_stop();
			aborts = false ;
		}
	moving = false ;
}; //activate motors to move to the chosen position*/
bool Seat::move_to(int assise, int avancement, int hauteur, int dossier)
{
	if(aborts)
	{
		motor_dossier->go_stop();
		motor_avancement->go_stop();
		motor_hauteur->go_stop();
		motor_assise->go_stop();
		moving = false ;
		aborts = false ;
	}
	else {
		int doss = motor_dossier->select_direction(dossier) ;
		int av = motor_avancement->select_direction(avancement) ;
		int haut = motor_hauteur->select_direction(hauteur) ;
		int ass = motor_assise->select_direction(assise) ;
		moving =  ( doss || av || haut || ass ) ; // has to do this otherwise the functions above are not executed
	}
	return moving ;
}; //activate motors to move to the chosen position