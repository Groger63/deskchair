


/*
  Desk chair prototype
  Memorise one position
  There's 4 different motors on the chair and each one has one position
 */

//-------------------Motors-------------------

const int MOTOR1_BACK = 2;
const int MOTOR1_FORWARD = 3;

const int MOTOR2_BACK = 4;
const int MOTOR2_FORWARD = 5;

const int MOTOR3_BACK = 6;
const int MOTOR3_FORWARD = 7;

const int MOTOR4_BACK = 8;
const int MOTOR4_FORWARD = 9;


//-------------------Potentiometers-------------------

const int POTENTIOMETER1 = A1;    // select the input pin for the potentiometer
const int POTENTIOMETER2 = A2;    // select the input pin for the potentiometer
const int POTENTIOMETER3 = A3;    // select the input pin for the potentiometer
const int POTENTIOMETER4 = A4;    // select the input pin for the potentiometer


//-------------------Buttons-------------------


const int BUTTON_MEM = 11;     // the number of the pushbutton pin
const int BUTTON_SET = 12;     // the number of the pushbutton pin


//-------------------Misc-------------------


const int MARGIN_MOTOR = 20 ;

int position_pot = 0 ;

int memory_state1 = 512;
int memory_state2 = 512;
int memory_state3 = 512;
int memory_state4 = 512;


//-------------------Motor units-------------------

Motor motor1(MOTOR1_BACK,MOTOR1_FORWARD,POTENTIOMETER1,MARGIN_MOTOR) ;
Motor motor2(MOTOR2_BACK,MOTOR2_FORWARD,POTENTIOMETER2,MARGIN_MOTOR) ;
Motor motor3(MOTOR3_BACK,MOTOR3_FORWARD,POTENTIOMETER3,MARGIN_MOTOR) ;
Motor motor4(MOTOR4_BACK,MOTOR4_FORWARD,POTENTIOMETER4,MARGIN_MOTOR) ;


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
    memory_state1 = analogRead(POTENTIOMETER1) ;
    memory_state2 = analogRead(POTENTIOMETER2) ;
    memory_state3 = analogRead(POTENTIOMETER3) ;
    memory_state4 = analogRead(POTENTIOMETER4) ;
  }
  
  if (digitalRead(BUTTON_SET) == HIGH) {

    motor1.move_to(memory_state1);
    motor2.move_to(memory_state2);
    motor3.move_to(memory_state3);
    motor4.move_to(memory_state4);
   
  }
}
