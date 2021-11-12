#include <TimerOne.h>
#include "LedControl.h"
#include "FontLEDClock.h"
#include <EEPROM.h>
#include "Font.h"
const int PIN_CS  = 10;
const int PIN_CLK = 13;
const int PIN_DIN = 11;
const int MODULES =  4;

int StateOfbuttonSave = 0;
bool buttonSave = false;

int StateOfbuttonIncrease = 0;
bool buttonIncrease = false;

int StateOfbuttonDecrease = 0;
bool buttonDecrease = false;

int StateOfbuttonDigit = 0;
bool buttonDigit = false;

int StateOfbuttonMode = 0;
bool buttonMode = false;

String example = "      Clock          ";
int digit_clock = 0;
int digit_countdown = 0;
int mode = 0;
int select = 0;
int ordermode;
bool clockDisplay = true;
bool countdownDisplay = true;
char ch[2][55]= {"      Clock          ","      CountDown          "};
bool isAnime = false;
int sw[5] = {7, 6, 5, 12, 4};
int buzzerPin = 8;
struct Timedigit {
  int secs = 0;
  int mins = 0;
  int hours = 0;
};

Timedigit Clock;
Timedigit countDown;
const long scrollDelay = 60;   // adjust scrolling speed
unsigned long bufferLong [14] = {0};
const unsigned char scrollText1[] PROGMEM = {"      ABC      "};
const unsigned char scrollText2[] PROGMEM = {"      Clock          "};
const unsigned char scrollText3[] PROGMEM = {"      CountDown          "};
unsigned long passtime1,passtime2,passtime3;
int statusDigit1,statusDigit2;
bool startCountdown = false;
int beep = 0;

LedControl led_control = LedControl(PIN_DIN, PIN_CLK, PIN_CS, MODULES);
/**
    Setup LED Matrix
**/
void ledMatrixSetup() {

  int device_count = led_control.getDeviceCount();
  for (int address = 0; address < device_count; address++) {
    led_control.shutdown(address, false);
    led_control.setIntensity(address, 1);
    led_control.clearDisplay(address);
  }
}
/**
    Turn off all LED on the matrix.
**/
void resetDisplay() {

  for (int n = 4 - 1; n >= 0; n--) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        led_control.setLed(n, i, j, false);
      }
    }
  }
}
/**
    Turn on LED on (x, y) location.
    @param x x location on the matrix.
    @param y y location on the matrix.
**/
void drawPoint(int x, int y, int value) {
  //select which matrix depending on the x coord
  uint8_t address;
  if (x >= 0 && x <= 7)   {
    address = 3;
  }
  if (x >= 8 && x <= 15)  {
    address = 2;
    x = x - 8;
  }
  if (x >= 16 && x <= 23) {
    address = 1;
    x = x - 16;
  }
  if (x >= 24 && x <= 31) {
    address = 0;
    x = x - 24;
  }

  if (value == 1) {
    led_control.setLed(address, y, x, true);
  } else {
    led_control.setLed(address, y, x, false);
  }
}
/**
    Turn off LED on (x, y) location.
    @param x x location on the matrix.
    @param y y location on the matrix.
**/

void save() {

  int address = 0;
  EEPROM.put(address, Clock);
  Serial.println("Save successful!!");
}

void load() {
  int address = 0;
  EEPROM.get(address, Clock);
}

void setup()
{
  countDown.secs = 1;
  countDown.mins = 0;
  countDown.hours = 0;
  Serial.begin(9600);
  ledMatrixSetup();
  EEPROM.get(0, Clock);
  for (int i = 0; i < 5; i++)
  {
    pinMode(sw[i], INPUT);
  }
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(UpdateTime);
}

void print_tiny_char(uint8_t x, uint8_t y, char c, int value)
{
  uint8_t dots;
  if (c >= 'A' && c <= 'Z' || (c >= 'a' && c <= 'z') ) {
    c &= 0x1F;  // A-Z maps to 1-26
  }
  else if (c >= '0' && c <= '9') {
    c = (c - '0') + 32;
  }
  else if (c == ' ') {
    c = 0;   // space
  }
  else if (c == '.') {
    c = 27;  // full stop
  }
  else if (c == ':') {
    c = 28;  // colon
  }
  else if (c == '\'') {
    c = 29;  // single quote mark
  }
  else if (c == '!') {
    c = 30;  // single quote mark
  }
  else if (c == '?') {
    c = 31;  // single quote mark
  }

  if (value == 1)
  {
    for (uint8_t col = 0; col < 3; col++) {
      dots = pgm_read_byte_near(&mytinyfont[c][col]);
      for (uint8_t row = 0; row < 5; row++) {
        if (dots & (16 >> row))
          drawPoint(x + col, y + row, 1);
        else
          drawPoint(x + col, y + row, 0);
      }
    }
  }
  else
  {
    for (uint8_t col = 0; col < 3; col++) {
      for (uint8_t row = 0; row < 5; row++) {
        if (dots & (16 >> row))
          drawPoint(x + col, y + row, 0);
        else
          drawPoint(x + col, y + row, 0);
      }
    }
  }

}

void clocks()
{
  print_tiny_char( 2, 1, Clock.hours / 10 + '0',1); //print hour
  print_tiny_char( 6, 1, Clock.hours % 10 + '0',1);
  print_tiny_char(12, 1, Clock.mins / 10 + '0',1); //print minute
  print_tiny_char(16, 1, Clock.mins % 10 + '0',1);
  print_tiny_char(22, 1, Clock.secs / 10 + '0',1); //print second
  print_tiny_char(26, 1, Clock.secs % 10 + '0',1);

  // blink : for each minute
  if (Clock.secs == 0)
  {
    drawPoint(10, 2, 0);
    drawPoint(10, 4, 0);
    drawPoint(20, 2, 0);
    drawPoint(20, 4, 0);
  }
  else
  {
    drawPoint(10, 2, 1);
    drawPoint(10, 4, 1);
    drawPoint(20, 2, 1);
    drawPoint(20, 4, 1);
  }

}

void UpdateTime() {
  Clock.secs++;
  if (Clock.secs > 59) {
    Clock.secs = 0; Clock.mins++;
  }
  if (Clock.mins > 59) {
    Clock.mins = 0; Clock.hours++;
  }
  if (Clock.hours > 23) {
    Clock.hours = 0;
  }
  if(startCountdown)
  {
    countDown.secs--;
  }
  if (countDown.secs < 0) {
    countDown.secs = 59; countDown.mins--;
  }
  if (countDown.mins < 0) {
    countDown.mins = 59; countDown.hours--;
  }
  if (countDown.hours < 0) {
    countDown.hours = 99;
  }
  if(countDown.secs == 0 && countDown.mins == 0 && countDown.hours == 0)
  {
    if(beep < 4)
    {
      tone(buzzerPin,400,200);
      beep++;
    }
    startCountdown = false;
  }
}

void scrollFont() {
  for (int counter = 0x20; counter < 0x80; counter++) {
    loadBufferLong(counter);
    delay(500);
  }
}

void scrollMessage(const unsigned char * messageString) {
  int counter = 0;
  int myChar = 0;
  do {
    // read back a char
    myChar =  pgm_read_byte_near(messageString + counter);
    if (myChar != 0) {
      loadBufferLong(myChar);
    }
    counter++;
  }
  while (myChar != 0);
  resetDisplay();
}

void loadBufferLong(int ascii) {
  if (ascii >= 0x20 && ascii <= 0x7f) {
    for (int a = 0; a < 7; a++) {               // Loop 7 times for a 5x7 font
      unsigned long c = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + a);     // Index into character table to get row data
      unsigned long x = bufferLong [a * 2];   // Load current scroll buffer
      x = x | c;                              // OR the new character onto end of current
      bufferLong [a * 2] = x;                 // Store in buffer
    }
    byte count = pgm_read_byte_near(font5x7 + ((ascii - 0x20) * 8) + 7);    // Index into character table for kerning data
    for (byte x = 0; x < count; x++) {
      rotateBufferLong();
      printBufferLong();
      delay(scrollDelay);
    }
  }
}
// Rotate the buffer
void rotateBufferLong() {
  for (int a = 0; a < 7; a++) {               // Loop 7 times for a 5x7 font
    unsigned long x = bufferLong [a * 2];   // Get low buffer entry
    byte b = bitRead(x, 31);                // Copy high order bit that gets lost in rotation
    x = x << 1;                             // Rotate left one bit
    bufferLong [a * 2] = x;                 // Store new low buffer
    x = bufferLong [a * 2 + 1];             // Get high buffer entry
    x = x << 1;                             // Rotate left one bit
    bitWrite(x, 0, b);                      // Store saved bit
    bufferLong [a * 2 + 1] = x;             // Store new high buffer
  }
}
// Display Buffer on LED matrix
void printBufferLong() {
  for (int a = 0; a < 7; a++) {             // Loop 7 times for a 5x7 font
    unsigned long x = bufferLong [a * 2 + 1]; // Get high buffer entry
    byte y = x;                             // Mask off first character
    led_control.setRow(3, a, y);                     // Send row to relevent MAX7219 chip
    x = bufferLong [a * 2];                 // Get low buffer entry
    y = (x >> 24);                          // Mask off second character
    led_control.setRow(2, a, y);                     // Send row to relevent MAX7219 chip
    y = (x >> 16);                          // Mask off third character
    led_control.setRow(1, a, y);                     // Send row to relevent MAX7219 chip
    y = (x >> 8);                           // Mask off forth character
    led_control.setRow(0, a, y);                     // Send row to relevent MAX7219 chip
  }
}

void loop()
{
//  LDRLED();
//  buttonSaveisPressed();
//  buttonIncreaseisPressed();
//  buttonDecreaseisPressed();
//  buttonDigitisPressed();
//  buttonModeisPressed();
//  if(buttonMode)
//  {
//    mode++;
//    ordermode++;
//    isAnime = true;
//    if(mode > 1 && ordermode > 1)
//    {mode = 0; ordermode = 0;}
//    buttonMode = false;
//  }
//  if(isAnime)
//  {
//    if(ordermode == 0)
//    {
//      scrollMessage(scrollText2);
//    }
//    if(ordermode == 1)
//    {
//      scrollMessage(scrollText3);
//    }
//    isAnime = false;
//  }
//  if(mode == 0)
//  {
//    clocks();
//    setTime(); 
//  }
//  if(mode == 1)
//  {
//    countdown();
//  }
  scrollMessage(scrollText3);
  
}

void buttonSaveisPressed()
{
  if (digitalRead(sw[0]) != StateOfbuttonSave)
  {
    if (digitalRead(sw[0]) == 0)
    {
      buttonSave = true;
    }
    delay(50);
  }
  StateOfbuttonSave = digitalRead(sw[0]);
}

void buttonIncreaseisPressed()
{
  if (digitalRead(sw[1]) != StateOfbuttonIncrease)
  {
    if (digitalRead(sw[1]) == 0)
    {
      buttonIncrease = true;
    }
    delay(200);
  }
  StateOfbuttonIncrease = digitalRead(sw[1]);
}

void buttonDecreaseisPressed()
{
  if (digitalRead(sw[2]) != StateOfbuttonDecrease)
  {
    if (digitalRead(sw[2]) == 0)
    {
      buttonDecrease = true;
    }
    delay(200);
  }
  StateOfbuttonDecrease = digitalRead(sw[2]);
}

void buttonDigitisPressed()
{
  if (digitalRead(sw[3]) != StateOfbuttonDigit)
  {
    if (digitalRead(sw[3]) == 0)
    {
      buttonDigit = true;
    }
    delay(50);
  }
  StateOfbuttonDigit = digitalRead(sw[3]);
}

void buttonModeisPressed()
{
  if (digitalRead(sw[4]) != StateOfbuttonMode)
  {
    if (digitalRead(sw[4]) == 0)
    {
      buttonMode = true;
    }
    delay(50);
  }
  StateOfbuttonMode = digitalRead(sw[4]);
}

void LDRLED()
{
  int LDR = analogRead(A0);
  LDR = map(LDR,200,800,14,0);
  if(LDR < 0)
  {LDR = 0;}
  if(LDR > 14)
  {LDR = 14;}
  for(int i=0;i<4;i++)
  {
    led_control.setIntensity(i,LDR);
  }
}

void setTime()
{
  if (buttonSave)
  {
    save();
    buttonSave = false;
  }
  if (buttonDigit)
  {
    if(digit_clock == 3)
    {
      digit_clock = 0; 
    }
    digit_clock++;
    buttonDigit = false;
  }
  if(digit_clock == 1)
  {
    print_tiny_char( 2, 1, Clock.hours / 10 + '0',0); 
    print_tiny_char( 6, 1, Clock.hours % 10 + '0',0);
    if(millis() - passtime1 >= 100000)
    {
      print_tiny_char( 2, 1, Clock.hours / 10 + '0',1); 
      print_tiny_char( 6, 1, Clock.hours % 10 + '0',1);
      delay(10);
      passtime1 = millis();
    }
    if(buttonIncrease)
    {
      Clock.hours++;
      if(Clock.hours > 23)
      {Clock.hours = 0;}
      buttonIncrease = false;
    }
    if(buttonDecrease)
    {
      Clock.hours--;
      if(Clock.hours < 0)
      {Clock.hours = 23;}
      buttonDecrease = false;
    }
  }
  if(digit_clock == 2)
  {
    print_tiny_char(12, 1, Clock.mins / 10 + '0',0);
    print_tiny_char(16, 1, Clock.mins % 10 + '0',0);
    if(millis() - passtime2 >= 10000)
    {
      print_tiny_char(12, 1, Clock.mins / 10 + '0',1);
      print_tiny_char(16, 1, Clock.mins % 10 + '0',1);
      passtime2 = millis();
    }
    if(buttonIncrease)
    {
      Clock.mins++;
      if(Clock.mins > 59)
      {Clock.mins = 0;}
      buttonIncrease = false;
    }
    if(buttonDecrease)
    {
      Clock.mins--;
      if(Clock.mins < 0)
      {Clock.mins = 59;}
      buttonDecrease = false;
    }
  }
 }

void countdown()
 {
  if(buttonSave)
  {
    if(startCountdown)
    {
      startCountdown = false;
    }
    else{startCountdown = true;}
    beep = 0;
    buttonSave = false;
  }
  if (buttonDigit)
  {
    if(digit_countdown > 3)
    {
      digit_countdown = 0;
    }
    digit_countdown++;
    buttonDigit = false;
  }
  if(digit_countdown == 1)
  {
    print_tiny_char( 2, 1, countDown.hours / 10 + '0',0); 
    print_tiny_char( 6, 1, countDown.hours % 10 + '0',0);
    if(millis() - passtime1 >= 100000)
    {
      print_tiny_char( 2, 1, countDown.hours / 10 + '0',1); 
      print_tiny_char( 6, 1, countDown.hours % 10 + '0',1);
      delay(10);
      passtime1 = millis();
    }
    if(buttonIncrease)
    {
      countDown.hours++;
      if(countDown.hours > 23)
      {countDown.hours = 0;}
      buttonIncrease = false;
    }
    if(buttonDecrease)
    {
      countDown.hours--;
      if(countDown.hours < 0)
      {countDown.hours = 23;}
      buttonDecrease = false;
    }
  }
  if(digit_countdown == 2)
  {
    print_tiny_char(12, 1, countDown.mins / 10 + '0',0);
    print_tiny_char(16, 1, countDown.mins % 10 + '0',0);
    if(millis() - passtime2 >= 10000)
    {
      print_tiny_char(12, 1, countDown.mins / 10 + '0',1);
      print_tiny_char(16, 1, countDown.mins % 10 + '0',1);
      passtime2 = millis();
    }
    if(buttonIncrease)
    {
      countDown.mins++;
      if(countDown.mins > 59)
      {countDown.mins = 0;}
      buttonIncrease = false;
    }
    if(buttonDecrease)
    {
      countDown.mins--;
      if(countDown.mins < 0)
      {countDown.mins = 59;}
      buttonDecrease = false;
    }
  }
    if(digit_countdown == 3)
    {
      print_tiny_char(22, 1, countDown.secs / 10 + '0',0);
      print_tiny_char(26, 1, countDown.secs % 10 + '0',0);
      if(millis() - passtime3 >= 10000)
        {
          print_tiny_char(22, 1, countDown.secs / 10 + '0',1);
          print_tiny_char(26, 1, countDown.secs % 10 + '0',1);
          passtime3 = millis();
        }
     if(buttonIncrease)
      {
        countDown.secs++;
        if(countDown.secs > 59)
        {countDown.secs = 0;}
        buttonIncrease = false;
      }
    if(buttonDecrease)
      {
        countDown.secs--;
        if(countDown.secs < 0)
        {countDown.secs = 59;}
        buttonDecrease = false;
      }
    }
    print_tiny_char( 2, 1, countDown.hours / 10 + '0',1); 
    print_tiny_char( 6, 1, countDown.hours % 10 + '0',1);
    print_tiny_char(12, 1, countDown.mins / 10 + '0',1); 
    print_tiny_char(16, 1, countDown.mins % 10 + '0',1);
    print_tiny_char(22, 1, countDown.secs / 10 + '0',1); 
    print_tiny_char(26, 1, countDown.secs % 10 + '0',1);
    drawPoint(10, 2, 1);
    drawPoint(10, 4, 1);
    drawPoint(20, 2, 1);
    drawPoint(20, 4, 1);
 }
