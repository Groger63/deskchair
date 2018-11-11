#include <ArduinoJson.h>
#include <seat.h>
#include <shiftreg.h>


/*
  Desk chair prototype
  Memorise one position
  There's 4 different motors on the chair and each one has one position
 */

#define ASSISE 0
#define AVANCEMENT 1
#define HAUTEUR 2
#define DOSSIER 3

//-------------------Motors-------------------

// Pins plugged to the shift reg
#define DOSSIER_BACK 0
#define DOSSIER_FORWARD 1

#define HAUTEUR_BACK 2
#define HAUTEUR_FORWARD 3

#define AVANCEMENT_BACK 4
#define AVANCEMENT_FORWARD 5

#define ASSISE_BACK 6//36;
#define ASSISE_FORWARD 7//34;


//-------------------Potentiometers-------------------

#define POT_ASSISE A1   // select the input pin for the potentiometer
#define POT_AVANCEMENT A2    // select the input pin for the potentiometer
#define POT_HAUTEUR A3    // select the input pin for the potentiometer
#define POT_DOSSIER A4    // select the input pin for the potentiometer


//-------------------Buttons-------------------


//#define BUTTON_MEM = 11;     // the number of the pushbutton pin
//#define BUTTON_SET = 12;     // the number of the pushbutton pin

//#define BUTTON_SELECT_MOTOR = 10;     // the number of the pushbutton pin
//#define BUTTON_MOTOR_BACK = 11;     // the number of the pushbutton pin
//#define BUTTON_MOTOR_FORWARD = 12;     // the number of the pushbutton pin

//#define LED_MOTOR_SELECT = 13 ;



// Rotary Encoder

#define ROTARY_SW 2
#define ROTARY_DT 3
#define ROTARY_CLK 4

#define BUTTON_HAUTEUR_FORWARD  27  //23     // the number of the pushbutton pin
#define BUTTON_HAUTEUR_BACK  29      //25    // the number of the pushbutton pin

#define BUTTON_ASSISE_FORWARD 35//31     // the number of the pushbutton pin
#define BUTTON_ASSISE_BACK 37   //33  // the number of the pushbutton pin

#define BUTTON_DOSSIER_FORWARD 23    // the number of the pushbutton pin
#define BUTTON_DOSSIER_BACK 25    // the number of the pushbutton pin


#define BUTTON_AVANCEMENT_FORWARD 33    // the number of the pushbutton pin
#define BUTTON_AVANCEMENT_BACK 31     // the number of the pushbutton pin


//-------------------Misc-------------------

#define baudrate 9600
#define MARGIN_MOTOR 70 

//-------------------ShiftReg-------------------

#define PIN_DS 8// = 8 ;   //pin 14  75HC595    
#define PIN_STCP 9// = 9 ;  //pin 12  75HC595
#define PIN_SHCP 10 //= 10 ; //pin 11  75HC595

ShiftReg *myShiftRegPtr = new ShiftReg(PIN_DS,PIN_STCP,PIN_SHCP) ;

//-------------------Motor units-------------------

Motor assise(ASSISE_BACK,ASSISE_FORWARD,POT_ASSISE,MARGIN_MOTOR,myShiftRegPtr) ;
Motor avancement(AVANCEMENT_BACK,AVANCEMENT_FORWARD,POT_AVANCEMENTETER2,MARGIN_MOTOR,myShiftRegPtr) ;
Motor hauteur(HAUTEUR_BACK,HAUTEUR_FORWARD,POT_HAUTEUR,MARGIN_MOTOR,myShiftRegPtr) ;
Motor dossier(DOSSIER_BACK,DOSSIER_FORWARD,POT_DOSSIER,MARGIN_MOTOR,myShiftRegPtr) ;

//-------------------Seat-------------------

int position_asked[4] ={ 512, 512 , 512, 512 } ;
bool move_asked = false ;
bool aborting = false ;
Seat * seat ;

//-------------------Setup routine-------------------

void setup() {        

  Serial.begin(baudrate);  

    pinMode(BUTTON_DOSSIER_FORWARD, INPUT);
    pinMode(BUTTON_DOSSIER_BACK, INPUT);
    
    pinMode(BUTTON_ASSISE_FORWARD, INPUT);
    pinMode(BUTTON_ASSISE_BACK, INPUT);
    
    pinMode(BUTTON_AVANCEMENT_FORWARD, INPUT);
    pinMode(BUTTON_AVANCEMENT_BACK, INPUT);
    
    pinMode(BUTTON_HAUTEUR_FORWARD, INPUT);
    pinMode(BUTTON_HAUTEUR_BACK, INPUT);

    delay(1000);

    seat = new Seat(assise,avancement,hauteur,dossier);
    
    pinMode (ROTARY_DT,INPUT);
    pinMode (ROTARY_CLK,INPUT);
    pinMode (ROTARY_SW,INPUT);
    digitalWrite(ROTARY_SW, HIGH);

    attachInterrupt(digitalPinToInterrupt(ROTARY_SW), abort_move, HIGH);

}

//-------------------Loop-------------------

void loop() 
{
  if(move_asked)
  {
    move_asked = seat->move_to(
                    position_asked[ASSISE], 
                    position_asked[AVANCEMENT],
                    position_asked[HAUTEUR],
                    position_asked[DOSSIER]
                  );
    if(!move_asked)
    {
      // Send more like a Json msg
      Serial.println("stopped");
    }
  }
  if(!seat->moving)
  {
    read_position_buttons() ;
  }
  
}

static void abort_move()
{
    seat->aborts = true ;
    Serial.println("Aborting...");
}

void read_position_buttons()
{
   if (digitalRead(BUTTON_DOSSIER_FORWARD) == HIGH) 
   {
       seat->motor_dossier.go_forward();
   } 
   else if (digitalRead(BUTTON_DOSSIER_BACK) == HIGH) 
   {
       seat->motor_dossier.go_backward();
   }
   else seat->motor_dossier.go_stop();
   
   if (digitalRead(BUTTON_ASSISE_FORWARD) == HIGH) 
   {
       seat->motor_assise.go_forward();
   } 
   else if (digitalRead(BUTTON_ASSISE_BACK) == HIGH) 
   {
       seat->motor_assise.go_backward();
   }
   else  seat->motor_assise.go_stop();
   
   
   if (digitalRead(BUTTON_AVANCEMENT_FORWARD) == HIGH) 
   {
       seat->motor_avancement.go_forward(); 
   } 
   else if (digitalRead(BUTTON_AVANCEMENT_BACK) == HIGH) 
   {
       seat->motor_avancement.go_backward();
   }
   else seat->motor_avancement.go_stop();
   
   if (digitalRead(BUTTON_HAUTEUR_FORWARD) == HIGH) 
   { 
       seat->motor_hauteur.go_forward();
   } 
   else if (digitalRead(BUTTON_HAUTEUR_BACK) == HIGH) 
   { 
       seat->motor_hauteur.go_backward();
   }
   else seat->motor_hauteur.go_stop();
}


void serialEvent() {
  
  while (Serial.available() > 0) {


    StaticJsonBuffer<512> jsonBuffer ;
    JsonObject& command = jsonBuffer.parseObject(Serial);
    String cmd = command["cmd"];

    if( cmd == "abort" )
    {
      seat->aborts = true ;
      const int capacity = JSON_OBJECT_SIZE(2);
      StaticJsonBuffer<capacity> jb;
      JsonObject& obj = jb.createObject();
      obj["cmd"] = "abort";
      obj["answer"] = "ok";
      obj.printTo(Serial);
    }
    else if( cmd == "get" )
    {
      //create Json object with each seat position
      const int capacity = JSON_OBJECT_SIZE(6);
      StaticJsonBuffer<capacity> jb;
      JsonObject& obj = jb.createObject();
      obj["cmd"] = "get";
      obj["answer"] = "ok";
      obj["dossier"] = seat->motor_dossier.get_position() ;
      obj["assise"] = seat->motor_assise.get_position();
      obj["avancement"] = seat->motor_avancement.get_position()  ;
      obj["hauteur"] = seat->motor_hauteur.get_position() ;
      
      obj.printTo(Serial);
    }
    else if(cmd == "set" )
    {
      int dossier = command["dossier"] ;
      int avancement = command["avancement"] ;
      int assise = command["assise"] ;
      int hauteur = command["assise"] ;
      
      position_asked[DOSSIER] = dossier ;
      position_asked[ASSISE] = assise ;
      position_asked[AVANCEMENT] = avancement ;
      position_asked[HAUTEUR] = hauteur ;
      move_asked = true ;
      
      const int capacity = JSON_OBJECT_SIZE(2);
      StaticJsonBuffer<capacity> jb;
      JsonObject& obj = jb.createObject();
      obj["cmd"] = "set";
      obj["answer"] = "ok";
      obj.printTo(Serial);
    }
    else if( cmd == "move" )
    {
      int dossier = command["dossier"] ;
      int avancement = command["avancement"] ;
      int assise = command["assise"] ;
      int hauteur = command["assise"] ;
            
      position_asked[DOSSIER] = seat->motor_dossier.get_position() + dossier ;
      position_asked[ASSISE] = seat->motor_assise.get_position() + assise ;
      position_asked[AVANCEMENT] = seat->motor_avancement.get_position() + avancement ;
      position_asked[HAUTEUR] = seat->motor_hauteur.get_position() + hauteur ;
      
      move_asked = true ;
      
      const int capacity = JSON_OBJECT_SIZE(2);
      StaticJsonBuffer<capacity> jb;
      JsonObject& obj = jb.createObject();
      obj["cmd"] = "move";
      obj["answer"] = "ok";
      obj.printTo(Serial);
    }
    else 
    {
      const int capacity = JSON_OBJECT_SIZE(2);
      StaticJsonBuffer<capacity> jb;
      JsonObject& obj = jb.createObject();
      obj["cmd"] = cmd ;
      obj["answer"] = "unknown";
      obj.printTo(Serial);
    }
  }
 }
