/*
  Desk chair prototype
  Memorise one position
  There's 4 different motors on the chair and each one has one position
 */
#include <motor.h> 


const int MOTOR_BACK = 7;
const int MOTOR_FORWARD = 6;

const int BUTTON_MEM = 5;     // the number of the pushbutton pin
const int BUTTON_SET = 4;     // the number of the pushbutton pin

const int POTENTIOMETER = A0;    // select the input pin for the potentiometer

const int MARGIN_MOTOR = 20 ;

int position_pot = 0 ;

int memory_state = 512;

Motor motor1(MOTOR_BACK,MOTOR_FORWARD,POTENTIOMETER,MARGIN_MOTOR) ;


// the setup routine runs once when you press reset:
void setup() {                
  // // initialize the digital pin as an output.
  // pinMode(MOTOR_BACK, OUTPUT);     
  // pinMode(MOTOR_FORWARD, OUTPUT);     
  // // initialize the pushbutton pin as an input:
   pinMode(BUTTON_MEM, INPUT);
   pinMode(BUTTON_SET, INPUT);

}

// the loop routine runs over and over again forever:
void loop() {
  
  
  if (digitalRead(BUTTON_MEM) == HIGH) {
    memory_state = analogRead(POTENTIOMETER) ;
  }
  
  if (digitalRead(BUTTON_SET) == HIGH) {

    motor1.move_to(memory_state);
   
  }
}
