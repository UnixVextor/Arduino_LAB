#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#define MAX_DEVICES 4
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define CLK_PIN   13   // SPI SCK pin on UNO
#define DATA_PIN  11   // SPI MOSI pin on UNO
#define CS_PIN    10   // connected to pin 10 on UNO
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

int scrollSpeed = 75;
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_LEFT;
int scrollPause = 0;
int i = 0;
int NewStateOfbuttonMode;
int StateOfbuttonMode = 0;
boolean buttonMode = false;
int Mode = 0;
int Status = 0;
int Status_1 = 0;
char curMessage[75] = { "Mini-clock" }; 
char Name[4][75] = {"Time", "Alarm","stop watch","count down"};
void setup() {
   P.begin();
   Serial.begin(9600);
   pinMode(2, INPUT);
   P.displayScroll("MINI-CLOCK          TIME", PA_LEFT,PA_SCROLL_LEFT,75); 
   
}


void loop() {
    NewStateOfbuttonMode = digitalRead(2);
    buttonModeisPressed();

    if(P.displayAnimate()){
      if (P.displayAnimate() && buttonMode){
        Mode++;
    if(Mode > 3){
      Mode = 0;  
    }
    //Status = 1;
    buttonMode = false;
  }
  Status_1 = 1;
 }
 if(Mode == 0 && !buttonMode && Status_1 == 1) {
    P.print("Clear");
    P.displayScroll(Name[1], PA_LEFT,PA_SCROLL_LEFT,75); 
 }
 if(Mode == 1 && !buttonMode  ) {
    P.print("One");
    P.displayScroll(Name[2], PA_LEFT,PA_SCROLL_LEFT,75);
 }
 if(Mode == 2 && !buttonMode) {
    P.print("Two");
    P.displayScroll(Name[3], PA_LEFT,PA_SCROLL_LEFT,75);
  }
 if(Mode == 3 && !buttonMode) {
   P.print("Three");
   P.displayScroll(Name[0], PA_LEFT,PA_SCROLL_LEFT,75);
 }
}

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
