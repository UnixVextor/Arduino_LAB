#define button 2
#define button_2 3
#define ledPin 10

#define ledPin_2 9

void setup(){
  pinMode(button,INPUT_PULLUP);
  pinMode(button_2,INPUT_PULLUP);
  pinMode(ledPin,OUTPUT);
  attachInterrupt(0,EXTI0_ISR,FALLING); 
  attachInterrupt(1,EXTI0_ISR,FALLING); 
  pinMode(ledPin_2,OUTPUT); 
  
}
void loop(){
  
  digitalWrite(ledPin_2,HIGH);
  delay(3000);
  digitalWrite(ledPin_2,LOW);
  delay(3000);
  }

void EXTI0_ISR(){
  digitalWrite(ledPin, !digitalRead(ledPin)); 
  digitalWrite(ledPin_2,LOW);
  delay(3000);   
}
