#define sw1 2
#define sw2 3
#define sw3 4
#define led1 8
#define led2 9
#define led3 10
int redDelay = 3000, greenDelay = 3000  ,yellowDelay = 2000;
unsigned long pastRed = 0, pastGreen = 0, pastYellow = 0;
int redOn =0 , greenOn = 0, yellowOn = 0;
unsigned long lastsw1 = 0,lastsw3 = 0, lastsw2 = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sw1,INPUT);
  pinMode(sw2,INPUT);
  pinMode(sw3,INPUT_PULLUP);

  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);

}

void loop() {
  //RED
  
  if (digitalRead(sw1) == HIGH && redOn == 0)
  { 
    if ((millis() - lastsw1) >= 400){
      Serial.print("c1");
      digitalWrite(led1, HIGH);
      redOn = 1;
      lastsw1 = millis();
      }
   }else if(digitalRead(sw1) == HIGH && redOn == 1){
      if(millis() - lastsw1 >= 400){
        digitalWrite(led1,0);
        Serial.print("c2");
        redOn = 0;
        lastsw1 = millis();
        }
    }
  
    

  if (millis() - pastRed >= redDelay && redOn == 1)
  {
    digitalWrite(led1, LOW);
    redOn = 0;
    pastRed = millis();
  }


 
  
  //GREEN
  if ((digitalRead(sw3) == LOW && greenOn == 0 && redOn != 1) || digitalRead(sw3) == LOW && yellowOn == 1)
  { 
    if ((millis() - lastsw3) >= 400){
      Serial.print("d1");
      digitalWrite(led3, HIGH);
      greenOn = 1;
      lastsw3 = millis();
      }
   }else if(digitalRead(sw3) == LOW && greenOn == 1){
      if(millis() - lastsw3 >= 400){
        digitalWrite(led3,0);
        Serial.print("d2");
        greenOn = 0;
        lastsw3 = millis();
        }
    }

  if (millis() - pastGreen >= greenDelay && greenOn == 1)
  {
    digitalWrite(led3, LOW);
    greenOn = 0;
    pastGreen = millis();
  }


 

  //YELLOW   
   if (digitalRead(sw2) == LOW && redOn != 1 && greenOn != 1)
  { 
    if ((millis() - lastsw2) >= 400){
      yellowOn = 1;
      lastsw2 = millis();
      }
   }
  
   if (yellowOn == 1)
  {
     digitalWrite(led2,HIGH);
     delay(10);
     digitalWrite(led2,LOW);
     delay(500);
     digitalWrite(led2,HIGH);
     delay(10);
     digitalWrite(led2,LOW);
     delay(500);
    yellowOn = 0;
  }
  

}
