#include <MovingAverageFilter.h>

#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include "Types.h"
#include "Motors.h"
#include "Ultrasonic.h"

IRrecv irrecv(2);
IRsend irsend;
decode_results results;

NewPing sonar1(trig_1, echo_1, Max_Dist); 
NewPing sonar2(trig_2, echo_2, Max_Dist); 
NewPing sonar3(trig_3, echo_3, Max_Dist);

uint16 value;
uint16 debug;
uint16 timer_=millis();
uint8 green=39, red=41, blue=43;

uint8 flag_left =1;
uint8 flag_right =1;
uint8 flag_center =1;
uint8 st, dr, fr, arg1, arg2, dir,state=1, valoareF, valoareL, valoareR, speed_val, lastval, start_time, co=0,crt; 
MovingAverageFilter a(30);
MovingAverageFilter b(30);
MovingAverageFilter c(30);
void setup() {
  // put your setup code here, to run once:

    irrecv.enableIRIn(); // Start the receiver

    pingTimer1 = millis() + pingSpeed; // Sensor 1 fires after 1 second (pingSpeed)
    pingTimer2 = pingTimer1 + delay_ping; // Sensor 2 fires 35ms later
    pingTimer3 = pingTimer2 + delay_ping;
   // pinMode(MOTOR_Left_1,OUTPUT);

Serial.begin(9600);
delay(5000);  //asteapta 5 sec la inceput
}

void go(uint8 arg1, uint8 arg2,uint8 dir)
{
  analogWrite(MOTOR_Left_1,0);
  analogWrite(MOTOR_Right_1,arg1);
  analogWrite(MOTOR_Left_2,arg2);
  analogWrite(MOTOR_Right_2,0);
  if(dir)
  {
  analogWrite(MOTOR_Left_1,arg1);
  analogWrite(MOTOR_Right_1,0);
  analogWrite(MOTOR_Left_2,0);
  analogWrite(MOTOR_Right_2,arg2);
  }
  
}

void loop() {
 st = sonar1.ping_cm();
dr= sonar2.ping_cm();
fr=sonar3.ping_cm();

/*if(fr!=0)
lastval=fr;*/

valoareF=a.process(fr);
valoareL=b.process(st);
valoareR=c.process(dr);

speed_val=80+10 * (6-valoareL);
    // irrecv.resume();  
Serial.print(st);//stanga
Serial.print("   ");
Serial.print(dr); //dreapta
Serial.print("   ");
Serial.print(fr); //fata
Serial.print("   "); 
/*
 
  */
// put your main code here, to run repeatedly:


if(co%5==0)
digitalWrite(blue, HIGH);
else
digitalWrite(blue, LOW);
co++;
//Serial.print(state);
if ((irrecv.decode(&results)))
   {
     value= results.value;
     
   // Serial.println(results.value);
   }
  Serial.println(state);
  
  switch(state){
    case 0 : go(0,0,0);
    break; 
     case 1:
     go(speed_val,80,0);
    // crt=millis();
    if(valoareL>15 ||valoareL<2)
    {
      state=2;   
    }
    else 
       if((valoareR>15 ||valoareR<2)&& (valoareF>2&&valoareF<23))
                {
                  timer_= millis();
                  state=3;
                 }
      if(valoareF<15 && valoareF>6 && valoareL<10 && valoareR<10)
      {
        state=4;
      }
      if(4294967295==results.value)
      state=5;
     break;
     
     case 2: 
     go(30,120,0);
        if(valoareL<10&&valoareL>2)
        {
         state = 1;
        }
     break;

        case 3:
         go(150,30,0);
// if(valoareR<10&&valoareR>2)
   if(millis()-timer_>1200)
        {
          
          state=1;
       
        }
       
       break; 

       case 4:

   analogWrite(MOTOR_Left_1,100);
  analogWrite(MOTOR_Right_1,0);
  analogWrite(MOTOR_Left_2,100);
  analogWrite(MOTOR_Right_2,0);

       
       if(millis()-timer_>3400)
       {
        state=1;
       }
       break;
case 5:

go(0,0,0);
digitalWrite(green, HIGH);

break;
       
     default:

     break;
    }
}

