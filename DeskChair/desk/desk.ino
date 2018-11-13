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
#define LEFT 0
#define RIGHT 1

boolean aLastState;  
int counter = 0; 
boolean aState;
boolean sw_clicked ;
bool odd = false ;

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
#define BACKLIGHT_DURATION  3

//-------------------Misc-------------------

#define baudrate 9600
#define MARGIN_MOTOR 70 
Timer timer1 ;
Timer timer2 ;

//-------------------ShiftReg-------------------

#define PIN_DS 8// = 8 ;   //pin 14  75HC595    
#define PIN_STCP 9// = 9 ;  //pin 12  75HC595
#define PIN_SHCP 10 //= 10 ; //pin 11  75HC595

ShiftReg *myShiftRegPtr = new ShiftReg(PIN_DS,PIN_STCP,PIN_SHCP) ;


//-------------------IHM-------------------

#define MENU_HOME 0
#define MENU_MOVE 1
#define MENU_SET 2
#define MENU_GET 3

int sub_menu_level = 0 ;
int menu_selected = MENU_MOVE ;
int sub_menu_selected = DOSSIER ;


//-------------------Motor units-------------------


Motor *assise=new Motor(
  ASSISE_BACK,
  ASSISE_FORWARD,
  BUTTON_ASSISE_BACK,
  BUTTON_ASSISE_FORWARD,
  POT_ASSISE,
  MARGIN_MOTOR ,
  myShiftRegPtr
  ) ;
  
Motor *avancement=new Motor(
  AVANCEMENT_BACK,
  AVANCEMENT_FORWARD,
  BUTTON_AVANCEMENT_BACK,
  BUTTON_ASSISE_FORWARD,
  POT_AVANCEMENT,
  MARGIN_MOTOR,
  myShiftRegPtr
  ) ;
  
Motor *hauteur=new Motor(
  HAUTEUR_BACK,
  HAUTEUR_FORWARD,
  BUTTON_HAUTEUR_BACK,
  BUTTON_HAUTEUR_FORWARD,
  POT_HAUTEUR,
  MARGIN_MOTOR,
  myShiftRegPtr
  );
Motor *dossier=new Motor(
  DOSSIER_BACK,
  DOSSIER_FORWARD,
  BUTTON_DOSSIER_BACK,
  BUTTON_DOSSIER_FORWARD,
  POT_DOSSIER,
  MARGIN_MOTOR,
  myShiftRegPtr
  );

Motor *  motors[] = {assise, avancement, hauteur, dossier} ;
int moving_event ;
#define MOVING_DURATION 500

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

    write_lcd("Hello world !","----o----");
    
    
    motors[DOSSIER]->go_backward();
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
    
    seat->read_buttons();
  }
  
  check_rotary();
  timer1.update();
  timer2.update();
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
  lcd_event = timer1.after(BACKLIGHT_DURATION * 1000, stop_backlight) ;
}

static void stop_backlight()
{
  light_lcd = false ;
  lcd.setBacklight(light_lcd); //Set Back light turn On
}

static void stop_seat()
{
  Serial.print("Stop");
  for(int i = 0 ; i < 4 ; i++)
  {
    motors[i]->go_stop();
  }
  seat->moving=false;
}

void write_lcd(String line1, String line2)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
  start_backlight();
}


void update_menu()
{
  switch(sub_menu_level)
  {
  case 0 :
    if(menu_selected<=0)menu_selected = MENU_GET ;
    
    menu_selected = menu_selected % 4 ;
    
    switch(menu_selected)
    {
      case MENU_MOVE :
        write_lcd("  MOVE-SET-GET","  ----");
        break ;
      case MENU_SET :
        write_lcd("  MOVE-SET-GET  ","       ---");
        break ;
      case MENU_GET :
        write_lcd("  MOVE-SET-GET  ","           ---");
       break ;
      default :
        break ;
    }
    break ;
  case 1 :
    switch(sub_menu_selected)
    {
      case ASSISE :
        write_lcd("Ass-Avan-Hau-Dos",
                  "---");
        break ;
      case AVANCEMENT :
        write_lcd("Ass-Avan-Hau-Dos",
                  "    ---");
        break ;
      case HAUTEUR :
        write_lcd("Ass-Avan-Hau-Dos",
                  "         ---");
        break ;
      case DOSSIER :
        write_lcd("Ass-Avan-Hau-Dos",
                  "             ---");
        break ;
      default :
        break ;
    }
    break ;
  case 2 :
    write_lcd("Moving ...",
              "");
    break ;
  default :
    break ;
  }
}

void rotary_move_action(int dir)
{
  switch(sub_menu_level)
  {
    case 0 :
      dir == LEFT ? menu_selected++ : menu_selected-- ;
      break ;
    case 1 :
      dir == LEFT ? sub_menu_selected++ : sub_menu_selected-- ;
      break ;
    case 2 :
      dir == LEFT ? motors[sub_menu_selected]->go_forward() : motors[sub_menu_selected]->go_backward() ;
//      Serial.print("motor selected :");
//      Serial.println(sub_menu_selected);
      seat->moving=true;
      timer2.stop(moving_event);
      moving_event = timer2.after(MOVING_DURATION, stop_seat) ;
      break ;
    default :
      break ;
  }
}

void check_rotary()
{
   // Reads the initial state of the outputA

    aState = digitalRead(ROTARY_DT); // Reads the "current" state of the outputA
    // If the previous and the current state of the outputA are different, that means a Pulse has occured

    
    if(digitalRead(ROTARY_SW) == LOW && !sw_clicked)
    {
      sub_menu_level ++ ;
      sub_menu_level = sub_menu_level % 3 ;
      sw_clicked = true ;  
      update_menu();
    }
    else if(digitalRead(ROTARY_SW) == HIGH && sw_clicked) sw_clicked = false ;
    
    if(aState != aLastState)
    {     
      if(odd)
      {
        // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
        if (digitalRead(ROTARY_CLK) != aState) 
        { 
          //Left
           counter ++;
          rotary_move_action(LEFT) ;
        } 
        else 
        {
          //right    
          
          rotary_move_action(RIGHT) ; 
          /*
           switch(sub_menu_level)
           {
            case 0 :
              menu_selected-- ;
              break ;
            case 1 :
              sub_menu_selected--;
              break ;
            case 2 :
              motors[sub_menu_selected].go_backward();
              Serial.print("motor selected :");
              Serial.println(sub_menu_selected);
              seat->moving=true;
              timer1.stop(moving_event);
              moving_event = timer1.after(MOVING_DURATION, stop_seat) ;
              break ;
            default :
              break ;
           }
           */
        }
      }
      sub_menu_selected = sub_menu_selected % 4 ;
      menu_selected = menu_selected % 4 ;
      odd = !odd;
      update_menu();
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
      obj["dossier"] = seat->motor_dossier->get_position() ;
      obj["assise"] = seat->motor_assise->get_position();
      obj["avancement"] = seat->motor_avancement->get_position()  ;
      obj["hauteur"] = seat->motor_hauteur->get_position() ;
      
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
            
      position_asked[DOSSIER] = seat->motor_dossier->get_position() + dossier ;
      position_asked[ASSISE] = seat->motor_assise->get_position() + assise ;
      position_asked[AVANCEMENT] = seat->motor_avancement->get_position() + avancement ;
      position_asked[HAUTEUR] = seat->motor_hauteur->get_position() + hauteur ;
      
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
