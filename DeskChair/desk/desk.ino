#include <seat.h>




/*
  Desk chair prototype
  Memorise one position
  There's 4 different motors on the chair and each one has one position
 */

#define GET 1
#define SET 2

//-------------------Motors-------------------

const int MOTOR1_BACK = 22;
const int MOTOR1_FORWARD = 24;

const int MOTOR2_BACK = 26;
const int MOTOR2_FORWARD = 28;

const int MOTOR3_BACK = 30;
const int MOTOR3_FORWARD = 32;

const int MOTOR4_BACK = 34;
const int MOTOR4_FORWARD = 36;


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



const int BUTTON_DOSSIER_FORWARD = 23;     // the number of the pushbutton pin
const int BUTTON_DOSSIER_BACK = 25;     // the number of the pushbutton pin

const int BUTTON_ASSISE_FORWARD = 0;     // the number of the pushbutton pin
const int BUTTON_ASSISE_BACK = 1;     // the number of the pushbutton pin

const int BUTTON_AVANCEMENT_FORWARD = 2;     // the number of the pushbutton pin
const int BUTTON_AVANCEMENT_BACK = 3;     // the number of the pushbutton pin

const int BUTTON_HAUTEUR_FORWARD = 5;     // the number of the pushbutton pin
const int BUTTON_HAUTEUR_BACK = 6 ;    // the number of the pushbutton pin
//const int BUTTON_ASSISE_FORWARD = 27;     // the number of the pushbutton pin
//const int BUTTON_ASSISE_BACK = 29;     // the number of the pushbutton pin

//const int BUTTON_AVANCEMENT_FORWARD = 31;     // the number of the pushbutton pin
//const int BUTTON_AVANCEMENT_BACK = 33;     // the number of the pushbutton pin

//const int BUTTON_HAUTEUR_FORWARD = 35;     // the number of the pushbutton pin
//const int BUTTON_HAUTEUR_BACK = 37 ;    // the number of the pushbutton pin


//-------------------Misc-------------------


const int baudrate = 9600;
const int MARGIN_MOTOR = 20 ;

int position_pot = 0 ;

int motor_selected = 0 ;

int memory_state1 = 512;
int memory_state2 = 512;
int memory_state3 = 512;
int memory_state4 = 512;


//-------------------Motor units-------------------

Motor assise(MOTOR1_BACK,MOTOR1_FORWARD,POTENTIOMETER1,MARGIN_MOTOR) ;
Motor avancement(MOTOR2_BACK,MOTOR2_FORWARD,POTENTIOMETER2,MARGIN_MOTOR) ;
Motor hauteur(MOTOR3_BACK,MOTOR3_FORWARD,POTENTIOMETER3,MARGIN_MOTOR) ;
Motor dossier(MOTOR4_BACK,MOTOR4_FORWARD,POTENTIOMETER4,MARGIN_MOTOR) ;


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
  
   
   if (digitalRead(BUTTON_DOSSIER_FORWARD) == HIGH) {
       
       seat.motor_dossier.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_DOSSIER_BACK) == HIGH) {
       
       seat.motor_dossier.go_backward();
     
     
   } 
   
   if (digitalRead(BUTTON_ASSISE_FORWARD) == HIGH) {
       
       seat.motor_assise.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_ASSISE_BACK) == HIGH) {
       
       seat.motor_assise.go_backward();
          
     
   } 
   if (digitalRead(BUTTON_AVANCEMENT_FORWARD) == HIGH) {
       
       seat.motor_avancement.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_AVANCEMENT_BACK) == HIGH) {
       
       seat.motor_avancement.go_backward();
     
     
   }
   
     if (digitalRead(BUTTON_HAUTEUR_FORWARD) == HIGH) {
       
       seat.motor_hauteur.go_forward();
     
     
   } 
   else if (digitalRead(BUTTON_HAUTEUR_BACK) == HIGH) {
       
       seat.motor_hauteur.go_backward();
       
   }
   
   if(  digitalRead(BUTTON_DOSSIER_FORWARD) == LOW
     && digitalRead(BUTTON_DOSSIER_BACK) == LOW 
     && digitalRead(BUTTON_ASSISE_FORWARD) == LOW 
     && digitalRead(BUTTON_ASSISE_BACK) == LOW 
     && digitalRead(BUTTON_AVANCEMENT_FORWARD) == LOW 
     && digitalRead(BUTTON_AVANCEMENT_BACK) == LOW 
     && digitalRead(BUTTON_HAUTEUR_FORWARD) == LOW 
     && digitalRead(BUTTON_HAUTEUR_BACK) == LOW )
     {
       seat.motor_hauteur.go_stop();
       seat.motor_dossier.go_stop();
       seat.motor_assise.go_stop();
       seat.motor_avancement.go_stop();

     }
  
  
}




void serialEvent() {
  
  //while ( !Serial.available() );
  while (Serial.available() > 0) {
    int serial_read = Serial.parseInt();
    if(serial_read==GET){
      
    digitalWrite(2, HIGH); 
   // if (digitalRead(BUTTON_MEM) == HIGH) {
      memory_state1 = analogRead(POTENTIOMETER1) ;
      memory_state2 = analogRead(POTENTIOMETER2) ;
      memory_state3 = analogRead(POTENTIOMETER3) ;
      memory_state4 = analogRead(POTENTIOMETER4) ;
      
      //delay(500);
      //String str1 = String(memory_state1) + ":" ;
      //String str2 = String(memory_state2) + ":" ;
      //String str3 = String(memory_state3) + ":" ;
      //String str4 = String(memory_state4) + "\n" ;
      
      //String str11 = str1 + str2 ;
      //String str12 = str3 + str4 ;
      //String str13 = str11 + str12 ;
      Serial.println("memory_state1");
      Serial.println(memory_state1);
      Serial.println("memory_state2");
      Serial.println(memory_state2);
      Serial.println("memory_state3");
      Serial.println(memory_state3);
      Serial.println("memory_state4");
      Serial.println(memory_state4);
      
     // Serial.print(str13);
      
      //Serial.println(memory_state1 + ':' + memory_state2 + ':' + memory_state3 + ':' + memory_state4 + ':' );
        
    
    }
    else if(serial_read==SET){

    digitalWrite(2, LOW); 
   // if (digitalRead(BUTTON_SET) == HIGH) {
     
     Serial.println("Moving to selected position : ");
    //while (Serial.available() < 1) ;
      //memory_state1 = Serial.parseInt();
      //memory_state2 = Serial.parseInt();
      //memory_state3 = Serial.parseInt();
      //memory_state4 = Serial.parseInt();
      //motor1.move_to(memory_state1);
      //motor2.move_to(memory_state2);
      //motor3.move_to(memory_state3);
      //motor4.move_to(memory_state4);
     
     Serial.println("Moving to selected position : ");
     
     Serial.println("memory_state1");
      Serial.println(memory_state1);
      Serial.println("memory_state2");
      Serial.println(memory_state2);
      Serial.println("memory_state3");
      Serial.println(memory_state3);
      Serial.println("memory_state4");
      Serial.println(memory_state4);
      
     
    }
  }
 }
