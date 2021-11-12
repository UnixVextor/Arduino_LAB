#include <EEPROM.h>
#include <TimerOne.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// Defining frequency of each music note
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

int durations[] = {
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,

  250, 125,
  //Rpeat of First Part
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 125,

  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 250, 125, 125,
  125, 125, 125, 250, 125,

  125, 125, 250, 125, 125,
  250, 125, 250, 125,
  125, 125, 250, 125, 125,
  125, 125, 375, 375,
  //End of Repeat

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500,

  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 125, 125, 125, 375,
  250, 125, 375, 250, 125, 375,
  125, 125, 125, 125, 125, 500
};
// buzzer
const int buzzer = 8;
int count = 0;
const float songSpeed = 1.0;
// updateTime
int I = 0;  // second
int address = 0;

int led[4] = {2, 3, 9, 4}; //LED pin

//LDR sensor
int LDR = A0;
int LDR_value = 0;

//Switches
int sw[4] = {7, 6, 5, 12}; //Switches pin
int NewStateOfbuttonMode;
int NewStateOfbuttonA;
int NewStateOfbuttonB;
int NewStateOfbuttonC;
int StateOfbuttonMode = 0;
int StateOfbuttonA = 0;
int StateOfbuttonB = 0;
int StateOfbuttonC = 0;
boolean buttonMode = false;
boolean buttonA = false;
boolean buttonB = false;
boolean buttonC = false;
boolean stopWatch = false;
boolean setupCount = true;
boolean Start = false;

// mode
int Mode = 0;
int cur = 3; // alarm cursor
int cur2 = 3; // countDown cursor
int cur3 = 3; // DisplayTime cursor

char Name[4][75] = {"Clock", "Alarm", "Stop watch", "Count Down"};

unsigned int timeDigit[4] = {0, 0, 0, 0};
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
String ch = "00:00";  // ch time
String ch_alarm = "00:00";
String ch_stopWatch = "00:00";
String ch_countDown = "00:00";
int alarmDigit[4] = {0, 0, 0, 0};
int stopWatchDigit[4] = {0, 0, 0, 0};
int countDownDigit[4] = {0, 0, 0, 0};
int number[10] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57};
int Status_1 = 0;
int speed_scroll = 50;
void setup(void)
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  EEPROM.get(address, timeDigit);

  P.begin();
  P.displayScroll("Mini-Clock          Clock", PA_LEFT, PA_SCROLL_LEFT, speed_scroll);
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(updateTime);
  for (int i = 0; i < 4 ; i++) {
    pinMode(led[i], OUTPUT);
    pinMode(sw[i], INPUT_PULLUP);
  }
  pinMode(LDR, INPUT);
}

void loop()
{
  LDR_value = analogRead(LDR);
  Serial.println(LDR_value);
  LDR_value = map(LDR_value, 200, 600, 14 , 1);

  if (LDR_value < 1)
  {
    LDR_value = 1;
  }
  if (LDR_value > 14)
  {
    LDR_value = 14;
  }
  P.setIntensity(LDR_value);
  NewStateOfbuttonMode = digitalRead(sw[0]);
  NewStateOfbuttonA = digitalRead(sw[1]);
  NewStateOfbuttonB = digitalRead(sw[2]);
  NewStateOfbuttonC = digitalRead(sw[3]);
  buttonModeisPressed();
  buttonAisPressed();
  buttonBisPressed();
  buttonCisPressed();

  if (P.displayAnimate()) {
    if (P.displayAnimate() && buttonMode) {
      Mode++;
      if (Mode > 3) {
        Mode = 0;
      }
      buttonMode = false;
    }
    Status_1 = 1;
  }
  if (Mode == 0 && !buttonMode && Status_1 == 1) {
    displayTime();
    P.displayScroll(Name[1], PA_LEFT, PA_SCROLL_LEFT, speed_scroll);
  }
  if (Mode == 1 && !buttonMode) {
    alarmMode();
    P.displayScroll(Name[2], PA_LEFT, PA_SCROLL_LEFT, speed_scroll);
  }
  if (Mode == 2 && !buttonMode) {
    StopWatch();
    P.displayScroll(Name[3], PA_LEFT, PA_SCROLL_LEFT, speed_scroll);
  }
  if (Mode == 3 && !buttonMode) {
    CountDown();
    P.displayScroll(Name[0], PA_LEFT, PA_SCROLL_LEFT, speed_scroll);
  }

  interrupts();
  float wait = durations[count] / songSpeed;
  if (timeDigit[3] == 0 && timeDigit[2] == 0) // minute
  {
    alarmsong(count, wait);     count++;
  }
  else if (timeDigit[3] == alarmDigit[3] && timeDigit[2] == alarmDigit[2] && timeDigit[1] == alarmDigit[1] && timeDigit[0] == alarmDigit[0] && I < 20) {
    alarmsong(count, wait);
    count++;
  } else {
    count = 0;
  }
  delay(wait);
}

void alarmsong(int i, float wait) {

  int notes[] = {
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C4, 0,
    NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, 0,
    NOTE_G5, NOTE_C4, NOTE_E4, NOTE_G4, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C4, 0,
    NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, 0,
    NOTE_G5, NOTE_C4, NOTE_E4, NOTE_G4, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C4, 0,
    NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, 0,
    NOTE_G5, NOTE_C4, NOTE_E4, NOTE_G4, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C4, 0,
    NOTE_E4, NOTE_G4, NOTE_C5, NOTE_E5, 0,
    NOTE_G5, NOTE_C4, NOTE_E4, NOTE_G4, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
    NOTE_A5, NOTE_A5, NOTE_A5, NOTE_A5, 0,
  };

  // Durations (in ms) of each music note of the song
  // Quarter Note is 250 ms when songSpeed = 1.0


  // Loop through each note

  const int currentNote = notes[i];

  // Play tone if currentNote is not 0 frequency, otherwise pause (noTone)

  if (currentNote != 0)
  {
    tone(buzzer, notes[i], wait); // tone(pin, frequency, duration)
  }
  else
  {
    noTone(buzzer);
  }
}

void updateTime()
{
  I++;   // current second
  if (stopWatch) // Stopwatch mode
  {
    stopWatchDigit[3]++;
  }
  if (Start)  // count down mode
  {
    countDownDigit[3]--;
  }
  if (I >= 60)
  {
    timeDigit[3]++;
    I = 0;
    EEPROM.put(address, timeDigit);
  }

  if (timeDigit[3] > 9)
  {
    timeDigit[3] = 0;
    timeDigit[2]++;
  }

  if (timeDigit[2] > 5)
  {
    timeDigit[2] = 0;
    timeDigit[1]++;
  }

  if (timeDigit[1] > 9 && timeDigit[0] < 2)
  {
    timeDigit[1] = 0;
    timeDigit[0]++;
  }

  if (timeDigit[0] == 2 && timeDigit[1] > 3)
  {
    timeDigit[3] = 0;
    timeDigit[2] = 0;
    timeDigit[1] = 0;
    timeDigit[0] = 0;
  }

}

void displayTime() // mode 1
{
  if (buttonC) {
    cur3--;
    if (cur3 < 0)
      cur3 = 3;
    buttonC = false;
  }
  if (buttonB)
  {
    buttonB = false;
    timeDigit[cur3]--;
  }
  if (buttonA)
  {
    buttonA = false;
    timeDigit[cur3]++;
  }
  if (timeDigit[3] < 0)
  {
    timeDigit[3] = 9;
  }
  else if (timeDigit[3] > 9)
  {
    timeDigit[3] = 0;
  }
  if (timeDigit[2] > 5)
  {
    timeDigit[2] = 0;
  }
  else if (timeDigit[2] < 0)
  {
    timeDigit[2] = 5;
  }
  if (timeDigit[1] > 9 && timeDigit[0] < 2)
  {
    timeDigit[1] = 0;
  }
  else if (timeDigit[1] < 0 && timeDigit[0] < 2)
  {
    timeDigit[1] = 9;
  }
  if (timeDigit[0] == 2 )
  {
    if (timeDigit[1] > 3)
    {
      timeDigit[1] = 0;
    }
    else if (timeDigit[1] < 0)
    {
      timeDigit[1] = 3;
    }
  }
  if (timeDigit[0] > 2)
  {
    timeDigit[0] = 0;
  }
  else if (timeDigit[0] < 0)
  {
    timeDigit[0] = 2;
  }

  ch[0] = number[timeDigit[0]];
  ch[1] = number[timeDigit[1]];
  ch[2] = char(58);
  ch[3] = number[timeDigit[2]];
  ch[4] = number[timeDigit[3]];
  P.setTextAlignment(PA_CENTER);
  P.print(ch);

  for (int i = 0; i < 4; i++)
  {
    if (i != cur3)
    {
      digitalWrite(led[i], LOW);
    }
    else
    {
      digitalWrite(led[cur3], HIGH);
    }
  }
}
//Debouce Switch
void buttonModeisPressed()
{
  if (NewStateOfbuttonMode != StateOfbuttonMode)
  {
    if (NewStateOfbuttonMode == 0)
    {
      buttonMode = true;
    }
    delay(50);
  }
  StateOfbuttonMode = NewStateOfbuttonMode;
}

void buttonAisPressed()
{
  if (NewStateOfbuttonA != StateOfbuttonA)
  {
    if (NewStateOfbuttonA == 0)
    {
      buttonA = true;
    }
    delay(50);
  }
  StateOfbuttonA = NewStateOfbuttonA;
}
void buttonBisPressed()
{
  if (NewStateOfbuttonB != StateOfbuttonB)
  {
    if (NewStateOfbuttonB == 0) {
      buttonB = true;
    }
    delay(50);
  }
  StateOfbuttonB = NewStateOfbuttonB;
}

void buttonCisPressed()
{
  if (NewStateOfbuttonC != StateOfbuttonC)
  {
    if (NewStateOfbuttonC == 0) {
      buttonC = true;
    }
    delay(50);
  }
  StateOfbuttonC = NewStateOfbuttonC;
}

void alarmMode()
{
  if (buttonC) {
    cur--;
    if (cur < 0)
      cur = 3;
    buttonC = false;
  }
  if (buttonB)
  {
    buttonB = false;
    alarmDigit[cur]--;
  }
  if (buttonA)
  {
    buttonA = false;
    alarmDigit[cur]++;
  }
  if (alarmDigit[3] < 0)
  {
    alarmDigit[3] = 9;
  }
  else if (alarmDigit[3] > 9)
  {
    alarmDigit[3] = 0;
  }
  if (alarmDigit[2] > 5)
  {
    alarmDigit[2] = 0;
  }
  else if (alarmDigit[2] < 0)
  {
    alarmDigit[2] = 5;
  }
  if (alarmDigit[1] > 9 && alarmDigit[0] < 2)
  {
    alarmDigit[1] = 0;
  }
  else if (alarmDigit[1] < 0 && alarmDigit[0] < 2)
  {
    alarmDigit[1] = 9;
  }
  if (alarmDigit[0] == 2 )
  {
    if (alarmDigit[1] > 3)
    {
      alarmDigit[1] = 0;
    }
    else if (alarmDigit[1] < 0)
    {
      alarmDigit[1] = 3;
    }
  }
  if (alarmDigit[0] > 2)
  {
    alarmDigit[0] = 0;
  }
  else if (alarmDigit[0] < 0)
  {
    alarmDigit[0] = 2;
  }


  Serial.println(alarmDigit[cur]);
  ch_alarm[0] = number[alarmDigit[0]];
  ch_alarm[1] = number[alarmDigit[1]];
  ch_alarm[2] = char(58);
  ch_alarm[3] = number[alarmDigit[2]];
  ch_alarm[4] = number[alarmDigit[3]];
  P.setTextAlignment(PA_CENTER);
  P.print(ch_alarm);
  for (int i = 0; i < 4; i++)
  {
    if (i != cur)
    {
      digitalWrite(led[i], LOW);
    }
    else
    {
      digitalWrite(led[cur], HIGH);
    }
  }
  //Serial.print(cur);
}

void StopWatch()
{
  if (buttonC)
  {
    if (stopWatch == true)
    {
      stopWatch = false;
    }
    else
    {
      stopWatch = true;
    }
    buttonC = false;
  }
  if (stopWatchDigit[3] > 9)
  {
    stopWatchDigit[3] = 0;
    stopWatchDigit[2]++;
  }
  if (timeDigit[2] > 5)
  {
    stopWatchDigit[2] = 0;
    stopWatchDigit[1]++;
  }
  if (stopWatchDigit[1] > 9)
  {
    stopWatchDigit[1] = 0;
    stopWatchDigit[0]++;
  }
  if (stopWatchDigit[0] > 9)
  {
    stopWatchDigit[0] = 0;
    stopWatchDigit[1] = 0;
    stopWatchDigit[2] = 0;
    stopWatchDigit[3] = 0;
  }
  if (buttonB)
  {
    stopWatchDigit[0] = 0;
    stopWatchDigit[1] = 0;
    stopWatchDigit[2] = 0;
    stopWatchDigit[3] = 0;
    stopWatch = false;
    buttonB = false;
  }
  ch_stopWatch[0] = number[stopWatchDigit[0]];
  ch_stopWatch[1] = number[stopWatchDigit[1]];
  ch_stopWatch[2] = char(58);
  ch_stopWatch[3] = number[stopWatchDigit[2]];
  ch_stopWatch[4] = number[stopWatchDigit[3]];
  P.setTextAlignment(PA_CENTER);
  P.print(ch_stopWatch);
}

void CountDown()
{
  if (buttonA && buttonB)
  {
    //setupCount = false;
    buttonA = false;
    buttonB = false;
    Start = true;
  }

  if (buttonC) {
    cur2--;
    if (cur2 < 0)
      cur2 = 3;
    buttonC = false;
  }
  if (buttonB)
  {
    buttonB = false;
    countDownDigit[cur2]--;
  }
  if (buttonA)
  {
    buttonA = false;
    countDownDigit[cur2]++;
  }
  if (!Start)
  {
    if (countDownDigit[3] < 0)
    {
      countDownDigit[3] = 9;
    }
    else if (countDownDigit[3] > 9)
    {
      countDownDigit[3] = 0;
    }
    if (countDownDigit[2] > 5)
    {
      countDownDigit[2] = 0;
    }
    else if (countDownDigit[2] < 0)
    {
      countDownDigit[2] = 5;
    }
    if (countDownDigit[1] > 9)
    {
      countDownDigit[1] = 0;
    }
    else if (countDownDigit[1] < 0)
    {
      countDownDigit[1] = 9;
    }
    if (countDownDigit[0] > 9 )
    {
      countDownDigit[0] = 0;
    }
    else if (countDownDigit[0] < 0)
    {
      countDownDigit[0] = 9;
    }
  }
  else
  {
    Serial.println(countDownDigit[3]);
    if (countDownDigit[3] < 0)
    {
      Serial.println("ahhh");
      countDownDigit[2]--;
      countDownDigit[3] = 9;
    }
    if (countDownDigit[2] < 0)
    {
      countDownDigit[1]--;
      countDownDigit[2] = 5;
    }
    if (countDownDigit[1] < 0)
    {
      countDownDigit[0]--;
      countDownDigit[1] = 9;
    }
    if (countDownDigit[0] < 0)
    {
      countDownDigit[0] = 0;
      countDownDigit[1] = 0;
      countDownDigit[2] = 0;
      countDownDigit[3] = 0;
      Start = false;
    }
  }

  ch_countDown[0] = number[countDownDigit[0]];
  ch_countDown[1] = number[countDownDigit[1]];
  ch_countDown[2] = char(58);
  ch_countDown[3] = number[countDownDigit[2]];
  ch_countDown[4] = number[countDownDigit[3]];
  P.setTextAlignment(PA_CENTER);
  P.print(ch_countDown);

  for (int i = 0; i < 4; i++)
  {
    if (i != cur2)
    {
      digitalWrite(led[i], LOW);
    }
    else
    {
      digitalWrite(led[cur2], HIGH);
    }
  }
}
