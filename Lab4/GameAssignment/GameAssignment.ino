#include<LedControl.h>

LedControl lc = LedControl(11, 13, 10, 4);
int speakerPin = 4;
int moveCol = 1, col;
int moveRow = 1, row;
int playerX;
unsigned long lasttime = 0;

void buzzStart() {
  float note_freq[7] = {130.8, 164.8, 196, 261.6, 329.6, 392, 523.3};
  for (int i = 0; i < 8; i++) {
    tone(speakerPin, note_freq[i], 180);
    delay(180);
  }
}
void buzzPong() {
  tone(speakerPin, 650, 50);
}
void buzzGameOver() {
  float note_freq[8] = {1319, 1047, 880, 698.5, 349.2, 293.7, 246.9, 196.0};
  for (int i = 0; i < 8; i++) {
    tone(speakerPin, note_freq[i], 180);
    delay(180);
  }
}

void SetLed(int row, int col, boolean OpenLed) {
  int addDis = 1;
  if (row > 7) {
    row = row - 8;
    addDis = 0;
  }
  col = 7 - col;
  lc.setLed(addDis, col, row, OpenLed);
}

void setup() {
  buzzStart();
  pinMode(speakerPin, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A1));
  pinMode (A0, INPUT);

  row = 0;
  col = random(8);

  lc.shutdown(0, false);
  lc.setIntensity(0, 7);
  lc.clearDisplay(0);

  lc.shutdown(1, false);
  lc.setIntensity(1, 7);
  lc.clearDisplay(1);
  //table
  SetLed(15, playerX, true);
  SetLed(15, playerX + 1, true);
  SetLed(15, playerX + 2, true);
}

void loop()
{
  SetLed(15, playerX, false);
  SetLed(15, playerX + 1, false);
  SetLed(15, playerX + 2, false);
  playerX = map(analogRead(A0), 300, 700, 0, 5);
  if (playerX > 5) {
    playerX = 5;
  } else if (playerX < 0) {
    playerX = 0;
  }
  SetLed(15, playerX, true);
  SetLed(15, playerX + 1, true);
  SetLed(15, playerX + 2, true);
  if (millis() - lasttime >= 100) {

    SetLed(row, col, false);
    if (col == 7) {
      moveCol = -1;
      buzzPong();
    }
    else if (col == 0) {
      moveCol = 1;
      buzzPong();
    }

    if (row == 14)
    {
      if (col >= playerX && col <= playerX + 2) { //ช่วงบาร์ 3 จุด
        moveRow = -1;
        buzzPong();
      }
    }
    else if (row == 15) { //เมื่อรับลูกไม่ได้
      buzzGameOver();
      delay(500);
      setup();
      buzzPong();
    }
    else if (row == 0) {
      moveRow = 1;
      buzzPong();
    }
    col += moveCol;
    row += moveRow;
    SetLed(row, col, true);

    lasttime = millis();
  }

}
