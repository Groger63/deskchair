
#include <Wire.h> // Include Wire.h to control I2C
#include <LiquidCrystal_I2C.h> //Download & include the code library can be downloaded below
#include "Timer.h"
 
 

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

boolean aLastState;  
int counter = 0; 
boolean aState;
boolean sw_clicked ;

#define BUTTON_HAUTEUR_FORWARD  27  //23     // the number of the pushbutton pin
#define BUTTON_HAUTEUR_BACK  29      //25    // the number of the pushbutton pin

#define BUTTON_ASSISE_FORWARD 35//31     // the number of the pushbutton pin
#define BUTTON_ASSISE_BACK 37   //33  // the number of the pushbutton pin

#define BUTTON_DOSSIER_FORWARD 23    // the number of the pushbutton pin
#define BUTTON_DOSSIER_BACK 25    // the number of the pushbutton pin


#define BUTTON_AVANCEMENT_FORWARD 33    // the number of the pushbutton pin
#define BUTTON_AVANCEMENT_BACK 31     // the number of the pushbutton pin

//-------------------Screen-------------------

LiquidCrystal_I2C lcd(0x27, 16, 2);
bool light_lcd = true ;
int lcd_event ;

//-------------------Misc-------------------

#define baudrate 9600
#define MARGIN_MOTOR 70 
Timer timer1 ;

//-------------------ShiftReg-------------------

#define PIN_DS 8// = 8 ;   //pin 14  75HC595    
#define PIN_STCP 9// = 9 ;  //pin 12  75HC595
#define PIN_SHCP 10 //= 10 ; //pin 11  75HC595

ShiftReg *myShiftRegPtr = new ShiftReg(PIN_DS,PIN_STCP,PIN_SHCP) ;


//-------------------Motor units-------------------


Motor assise(
  ASSISE_BACK,
  ASSISE_FORWARD,
  BUTTON_ASSISE_BACK,
  BUTTON_ASSISE_FORWARD,
  POT_ASSISE,
  MARGIN_MOTOR,
  myShiftRegPtr
  ) ;
  
Motor avancement(
  AVANCEMENT_BACK,
  AVANCEMENT_FORWARD,
  BUTTON_AVANCEMENT_BACK,
  BUTTON_ASSISE_FORWARD,
  POT_AVANCEMENT,
  MARGIN_MOTOR,
  myShiftRegPtr
  ) ;
  
Motor hauteur(
  HAUTEUR_BACK,
  HAUTEUR_FORWARD,
  BUTTON_HAUTEUR_BACK,
  BUTTON_HAUTEUR_FORWARD,
  POT_HAUTEUR,
  MARGIN_MOTOR,
  myShiftRegPtr
  ) ;
  
Motor dossier(
  DOSSIER_BACK,
  DOSSIER_FORWARD,
  BUTTON_DOSSIER_BACK,
  BUTTON_DOSSIER_FORWARD,
  POT_DOSSIER,
  MARGIN_MOTOR,
  myShiftRegPtr
  ) ;


//-------------------Seat-------------------

int position_asked[4] ={ 512, 512 , 512, 512 } ;
bool move_asked = false ;
bool aborting = false ;
Seat * seat = new Seat(assise,avancement,hauteur,dossier);


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
    
    pinMode (ROTARY_DT,INPUT);
    pinMode (ROTARY_CLK,INPUT);
    pinMode (ROTARY_SW,INPUT);
    digitalWrite(ROTARY_SW, HIGH);

    // initialize the LCD
    lcd.begin();
    lcd.clear();
    // Turn on the blacklight and print a message.
    lcd.print("Hello, world!");
    start_backlight();
    
    
    aLastState = digitalRead(ROTARY_DT);   
    sw_clicked = false ;
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
    // En attendant que je règle les faux contacts sur les boutons...
    
    //seat->read_buttons();
  }
  
  check_rotary();
  timer1.update();
}

static void start_backlight()
{
  if(light_lcd)
  {
    timer1.stop(lcd_event);
  }
  else
  {
    light_lcd = true ;
    lcd.setBacklight(light_lcd); //Set Back light turn On
  }
  lcd_event = timer1.after(5000, stop_backlight) ;
}

static void stop_backlight()
{
  light_lcd = false ;
  lcd.setBacklight(light_lcd); //Set Back light turn On
}

void check_rotary()
{
   // Reads the initial state of the outputA

    aState = digitalRead(ROTARY_DT); // Reads the "current" state of the outputA
    // If the previous and the current state of the outputA are different, that means a Pulse has occured

    
    if(digitalRead(ROTARY_SW) == LOW && !sw_clicked)
    {
      start_backlight() ;
      lcd.clear();
      lcd.print("Button clicked");
      sw_clicked = true ;
      
    }else if(digitalRead(ROTARY_SW) == HIGH && sw_clicked) sw_clicked = false ;
    if (aState != aLastState)
    {     
      
      start_backlight() ;
      // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
      if (digitalRead(ROTARY_CLK) != aState) 
      { 
         counter ++;
      } 
      else 
      {
         counter --;
      }
      lcd.clear();
      lcd.print("Position: ");
      lcd.print(counter);
    }
    aLastState = aState; // Updates the previous state of the outputA with the current state
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
