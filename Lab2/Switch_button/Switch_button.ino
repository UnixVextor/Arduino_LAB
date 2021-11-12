int switch1 = 2;
int switch2 = 3;
int switch3 = 4;

int Led1 = 8;
int Led2 = 9;
int Led3 = 10;

unsigned long premil_1 = 0;
unsigned long premil_2 = 0;
unsigned long premil_3 = 0;

int ledState_1 = LOW;
int ledState_2 = LOW;
int ledState_3 = LOW;

bool isPress_3 = false;
bool isPress_2 = false;
bool isPress_1 = true;
#define pressed_PULLUP LOW
#define pressed_PULLDOWN HIGH


void Activity1(){
  int Readswitch_1 = digitalRead(switch1);
  int Readswitch_2 = digitalRead(switch2);
  int Readswitch_3 = digitalRead(switch3); 
  
  // Switch 1
  if(Readswitch_1 == pressed_PULLDOWN)
  {
   if(isPress_1 == true){
    digitalWrite(Led1,HIGH);
    isPress_1 = false;
   }else{
    digitalWrite(Led1,LOW);
    isPress_1 = true;
   }
   Serial.println("Press Switch switch 1");
  }


 //switch 2
  if(Readswitch_2 == pressed_PULLUP)
  {
   if(isPress_2 == false){
    digitalWrite(Led2,HIGH);
    isPress_2 = true;
   }else{
    digitalWrite(Led2,LOW);
    isPress_2 = false;
   }
   Serial.println("Press Switch switch 2");
  }

  
  //switch 3
  if(Readswitch_3 == pressed_PULLUP)
  {
   if(isPress_3 == false){
    digitalWrite(Led3,HIGH);
    isPress_3 = true;
   }else{
    digitalWrite(Led3,LOW);
    isPress_3 = false;
   }
   Serial.println("Press Switch switch 3");
  }
  
}

void setup() {
  Serial.begin(9600);
  pinMode(switch1,INPUT);
  pinMode(switch2,INPUT);
  pinMode(switch3,INPUT_PULLUP);
  pinMode(Led1,OUTPUT);
  pinMode(Led2,OUTPUT);
  pinMode(Led3,OUTPUT); 
}

void loop() {
  
  int readSwitch_1 = digitalRead(switch1);
  int readSwitch_2 = digitalRead(switch2);
  int readSwitch_3 = digitalRead(switch3);
  unsigned long ms = millis();
  // LED 1
  if((ms-premil_1) >= 3000){
    premil_1 = ms;
    if(readSwitch_1 == pressed_PULLDOWN){
        ledState_1 = HIGH;  
    }else{
        ledState_1 = LOW;
     }
//    if(ledState_1 == LOW){
//      ledState_1 = HIGH;
//    }else{
//      ledState_1 = LOW;
//     }
    digitalWrite(Led1, ledState_1);
  }

  //LED 2
  if((ms-premil_2) >= 500){
    premil_2 = ms;

    if(ledState_2 == LOW){
      ledState_2 = HIGH;
    }else{
      ledState_2 = LOW;
     }
    digitalWrite(Led2, ledState_2);
  }
  
  // LED 3
  if((ms-premil_3) >= 3000){
    premil_3 = ms;
    if(readSwitch_3 == pressed_PULLUP){
        ledState_3 = HIGH;  
    }else{
        ledState_3 = LOW;
     }

//    if(ledState_3 == LOW){
//      ledState_3 = HIGH;
//    }else{
//      ledState_3 = LOW;
//     }
    digitalWrite(Led3, ledState_3);
  }
  

}
