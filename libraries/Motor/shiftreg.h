//ShiftReg.h
/*
  ShiftReg.h - Library for using a 8 bit ShiftReg 
  Created by Roger 10/11/2018
  Released into the public domain.
*/
#ifndef ShiftReg_h
#define ShiftReg_h

#include "Arduino.h"

class ShiftReg{
	private :
		int pin_DS ;
		int pin_STCP ;
		int pin_SHCP ;
		boolean registers[8];
	public :

		void clearRegisters();

		void writeRegisters();


		void setRegisterPin(int index, int value) ;
		
		ShiftReg(int DS , int STCP, int SHCP);
		~ShiftReg();

};

#endif
