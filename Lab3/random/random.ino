int randomno = 0;
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  randomno = random(1,10);
 
}

void loop() {
  Serial.print(randomno);
  Serial.print("\t");
  Serial.println(random(1,10));
  delay(1000);
}
