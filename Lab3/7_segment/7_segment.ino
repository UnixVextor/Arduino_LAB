#define button1 11
#define button2 12
int number;
int randNumber = 0;
const byte numPins = 8;
int select = 0;

int bitPattern[9] = {
  B1111001,//1
  B0100100,//2
  B0110000,//3
  B0011001,//4
  B0010010,//5
  B0000010,//6
  B1111000,//7
  B0000000,//8
  B0010000 //9
};

int check[3]{
  B1000000,//0
  B1000010,//G
  B1000111//L
};
int lastsw1 = 0;
int lastsw2 = 0;
//              1  2  3  4  5  6  7  8
//              A  B  C  D  E  F  G  dp
const int segmentPins[7] = {3, 4, 5, 6,7,8,9};
int count = 0;

void switchA(){
  if (digitalRead(button1) == LOW) {
    Serial.print('A');
      if ((millis() - lastsw1) >= 400) { //จับเวลา
        select++;
        if (select > 8) {
          select = 0;
        }
        lastsw1 = millis();//เก็บเวลาล่าสุด
      }
    }
}

void switchB(){
  int isBitSet;
  if (digitalRead(button2) == LOW) {
        Serial.print('B');
      if ((millis() - lastsw2) >= 400) { //จับเวลา
        //random thingggggggggggggggggggggggggg
        lastsw2 = millis();//เก็บเวลาล่าสุด
        if(randNumber == select+1){
            for (int segment = 0; segment < 7; segment++) {
                isBitSet = bitRead(check[0], segment);
                digitalWrite(segmentPins[segment], isBitSet);
              }
              delay (2000);
              randNumber=random(1,10);
              select = 0;
        }
        else if (randNumber < select+1){
            for (int segment = 0; segment < 7; segment++) {
                isBitSet = bitRead(check[1], segment);
                digitalWrite(segmentPins[segment], isBitSet);
              }
              delay (2000);
        }
        else if (randNumber > select+1){
            for (int segment = 0; segment < 7; segment++) {
                isBitSet = bitRead(check[2], segment);
                digitalWrite(segmentPins[segment], isBitSet);
              }
              delay (2000);
        }
      }
    }
}

void setup()
{
  randomSeed(analogRead(A0));
  randNumber = random(1,10);
  pinMode(button1, INPUT);//pinMode(12,INPUT)
  pinMode(button2, INPUT);//pinMode(13,INPUT)
  Serial.begin(9600);
  for (int i = 0; i < numPins; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
}

void loop()
{

  switchA();
  switchB();
  delay(100);
  int isBitSet;
  for (int segment = 0; segment < 7; segment++) {
    isBitSet = bitRead(bitPattern[select], segment);
    digitalWrite(segmentPins[segment], isBitSet);
  }
    Serial.println(randNumber);
}
