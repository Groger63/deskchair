#include <seat.h>
#include <shiftreg.h>



/*
  Desk chair prototype
  Memorise one position
  There's 4 different motors on the chair and each one has one position
 */

#define GET 1
#define SET 2

//-------------------Motors-------------------

const int DOSSIER_BACK = 0;
const int DOSSIER_FORWARD = 1;

const int HAUTEUR_BACK = 2;
const int HAUTEUR_FORWARD = 3;

const int AVANCEMENT_BACK = 4;
const int AVANCEMENT_FORWARD = 5;

const int ASSISE_BACK = 6;//36;
const int ASSISE_FORWARD = 7;//34;


//-------------------Potentiometers-------------------

const int POTENTIOMETER1 = A1;    // select the input pin for the potentiometer
const int POTENTIOMETER2 = A2;    // select the input pin for the potentiometer
const int POTENTIOMETER3 = A3;    // select the input pin for the potentiometer
const int POTENTIOMETER4 = A4;    // select the input pin for the potentiometer


//-------------------Buttons-------------------


//const int BUTTON_MEM = 11;     // the number of the pushbutton pin
//const int BUTTON_SET = 12;     // the number of the pushbutton pin

//const int BUTTON_SELECT_MOTOR = 10;     // the number of the pushbutton pin
//const int BUTTON_MOTOR_BACK = 11;     // the number of the pushbutton pin
//const int BUTTON_MOTOR_FORWARD = 12;     // the number of the pushbutton pin

//const int LED_MOTOR_SELECT = 13 ;



const int BUTTON_HAUTEUR_FORWARD = 27;//23     // the number of the pushbutton pin
const int BUTTON_HAUTEUR_BACK = 29; //25    // the number of the pushbutton pin

const int BUTTON_ASSISE_FORWARD = 35;//31     // the number of the pushbutton pin
const int BUTTON_ASSISE_BACK = 37;   //33  // the number of the pushbutton pin

const int BUTTON_DOSSIER_FORWARD = 25;     // the number of the pushbutton pin
const int BUTTON_DOSSIER_BACK = 23;     // the number of the pushbutton pin


const int BUTTON_AVANCEMENT_FORWARD = 31;     // the number of the pushbutton pin
const int BUTTON_AVANCEMENT_BACK = 33 ;    // the number of the pushbutton pin


//-------------------Misc-------------------


const int baudrate = 9600;
const int MARGIN_MOTOR = 20 ;

int position_pot = 0 ;

int motor_selected = 0 ;

int memory_state1 = 512;
int memory_state2 = 512;
int memory_state3 = 512;
int memory_state4 = 512;


//-------------------ShiftReg-------------------

#define PIN_DS 8// = 8 ;   //pin 14  75HC595    
#define PIN_STCP 9// = 9 ;  //pin 12  75HC595
#define PIN_SHCP 10 //= 10 ; //pin 11  75HC595


ShiftReg *myShiftRegPtr = new ShiftReg(PIN_DS,PIN_STCP,PIN_SHCP) ;


//-------------------Motor units-------------------

Motor assise(ASSISE_BACK,ASSISE_FORWARD,POTENTIOMETER1,MARGIN_MOTOR,myShiftRegPtr) ;

Motor avancement(AVANCEMENT_BACK,AVANCEMENT_FORWARD,POTENTIOMETER2,MARGIN_MOTOR,myShiftRegPtr) ;

Motor hauteur(HAUTEUR_BACK,HAUTEUR_FORWARD,POTENTIOMETER3,MARGIN_MOTOR,myShiftRegPtr) ;

Motor dossier(DOSSIER_BACK,DOSSIER_FORWARD,POTENTIOMETER4,MARGIN_MOTOR,myShiftRegPtr) ;


//-------------------Seat-------------------

Seat seat(assise,avancement,hauteur,dossier) ;

//-------------------Setup routine-------------------

void setup() {        

  Serial.begin(baudrate);  
  // // initialize the digital pin as an output.
  // pinMode(MOTOR_BACK, OUTPUT);     
  // pinMode(MOTOR_FORWARD, OUTPUT);     
  // // initialize the pushbutton pin as an input:
  // pinMode(BUTTON_MEM, INPUT);
  // pinMode(BUTTON_SET, INPUT);

    pinMode(BUTTON_DOSSIER_FORWARD, INPUT);
    pinMode(BUTTON_DOSSIER_BACK, INPUT);
    
    pinMode(BUTTON_ASSISE_FORWARD, INPUT);
    pinMode(BUTTON_ASSISE_BACK, INPUT);
    
    pinMode(BUTTON_AVANCEMENT_FORWARD, INPUT);
    pinMode(BUTTON_AVANCEMENT_BACK, INPUT);
    
    pinMode(BUTTON_HAUTEUR_FORWARD, INPUT);
    pinMode(BUTTON_HAUTEUR_BACK, INPUT);



}

//-------------------Loop-------------------

void loop() {
  /*
   
   if (digitalRead(BUTTON_DOSSIER_FORWARD) == HIGH) {
       
       seat.motor_dossier.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_DOSSIER_BACK) == HIGH) {
       
       seat.motor_dossier.go_backward();
     
     
   }
  else seat.motor_dossier.go_stop();
   
   if (digitalRead(BUTTON_ASSISE_FORWARD) == HIGH) {
       
       seat.motor_assise.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_ASSISE_BACK) == HIGH) {
       
       seat.motor_assise.go_backward();
          
     
   }else  seat.motor_assise.go_stop();
   
   
   if (digitalRead(BUTTON_AVANCEMENT_FORWARD) == HIGH) {
       
       seat.motor_avancement.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_AVANCEMENT_BACK) == HIGH) {
       
       seat.motor_avancement.go_backward();
     
     
   }else seat.motor_avancement.go_stop();
   
   
     if (digitalRead(BUTTON_HAUTEUR_FORWARD) == HIGH) {
       
       seat.motor_hauteur.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_HAUTEUR_BACK) == HIGH) {
       
       seat.motor_hauteur.go_backward();
       
   }else seat.motor_hauteur.go_stop();
*/
  
}




void serialEvent() {
  
  while (Serial.available() > 0) {

    String test = Serial.readString();

    Serial.println(test);

    
    /*
     * JSON here
     * 
     * 
     * 
     */



    

  }
 }
