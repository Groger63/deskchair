//ShiftReg.cpp
/*
  ShiftReg.cpp - Library for using a 8 bit ShiftReg 
  Created by Roger 10/11/2018
  Released into the public domain.
*/

#include "shiftreg.h"
#include "Arduino.h"



ShiftReg::ShiftReg(int DS , int STCP, int SHCP) : pin_DS(DS) , pin_STCP(STCP), pin_SHCP(SHCP)
{
  pinMode(PIN_DS, OUTPUT);
  pinMode(PIN_STCP, OUTPUT);
  pinMode(PIN_SHCP, OUTPUT);

  clearRegisters();

  writeRegisters();

  registers = {LOW};
}


ShiftReg::~ShiftReg()
{
	delete registers ;
};


void ShiftReg::clearRegisters()
{

}

void ShiftReg::writeRegisters()
{

}


void ShiftReg::setRegisterPin(int index, int value)
{

}

