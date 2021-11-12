#define button 11
#define pressed LOW 

void setup(){
  Serial.begin(9600);
  pinMode(button,INPUT_PULLUP);
}

void loop(){
  int ReadSwitch = digitalRead(button);
  if(ReadSwitch == pressed){
    Serial.println("Pressed Switch.");
    delay(500);
    }
}
