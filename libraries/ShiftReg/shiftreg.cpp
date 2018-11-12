//ShiftReg.cpp
/*
  ShiftReg.cpp - Library for using a 8 bit ShiftReg 
  Created by Roger 10/11/2018
  Inspired from https://retroetgeek.com/geek/arduino/arduino-augmenter-le-nombre-de-sortie-avec-un-registre-a-decalage-74hc595/
  Released into the public domain.
*/

#include "shiftreg.h"
#include "Arduino.h"



ShiftReg::ShiftReg(int DS , int STCP, int SHCP) : pin_DS(DS) , pin_STCP(STCP), pin_SHCP(SHCP)
{
  pinMode(pin_DS, OUTPUT);
  pinMode(pin_STCP, OUTPUT);
  pinMode(pin_SHCP, OUTPUT);

  clearRegisters();

  writeRegisters();

}


ShiftReg::~ShiftReg()
{

};

ShiftReg::ShiftReg()
{

};


void ShiftReg::clearRegisters()
{
  for(int i =7; i >=  0; i--){
     registers[i] = HIGH;
  }
  writeRegisters();
}

void ShiftReg::writeRegisters()
{
 // Tant que LOW les modifications ne seront pas affectés
 // Until LOW modification will not be apply
  digitalWrite(pin_STCP, LOW);
 
 // boucle pour affecter chaque pin des 74hc595
 // loop for aplly all value for each pin 74hc595
  for(int i = 7; i >=  0; i--){

 
    //doit etre a l'etat bas pour changer de colonne plus tard
    //need to be low for change column soon
    digitalWrite(pin_SHCP, LOW);
 
    // recuperation de la valeur dans le tableau registers
    // catch value insinde array registers
    int val = registers[i];
 
    //affecte la valeur sur le pin DS correspondant a un pin du 74hc595
    //apply the value to a pin of 74hc595
    digitalWrite(pin_DS, val);
    //colonne suivante
    // next column
    digitalWrite(pin_SHCP, HIGH);
 
  }
  //applique toutes les valeurs au 74hc595
  // apply value to all pin of 74hc595
  digitalWrite(pin_STCP, HIGH);
}


void ShiftReg::setRegisterPin(int index, int value)
{
  registers[index] = value;
}

