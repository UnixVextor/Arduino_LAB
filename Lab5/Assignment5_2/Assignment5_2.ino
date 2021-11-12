#define LED_R_N 3
#define LED_Y_N 4
#define LED_G_N 5
#define Button_N 2

#define LED_R_E 6
#define LED_Y_E 7
#define LED_G_E 8
#define Button_E 9

#define LED_R_w 10
#define LED_G_w 11
#define Button_w 12

#define goN   0
#define waitN 1
#define goE   2
#define waitE 3
#define waitNw 4
#define gow   5
#define waitEw 6
#define waitwN1 7
#define waitwN2 8
#define waitwN3 9
#define waitwN4 10
#define waitwN5 11
#define waitwN6 12
#define waitwE1 13
#define waitwE2 14
#define waitwE3 15
#define waitwE4 16
#define waitwE5 17
#define waitwE6 18

struct State{
  unsigned long ST_OUT;
  unsigned long Time;
  unsigned long Next[8];  
};
typedef const struct State SType;
SType FSM[19]{
  {B00110001,1000,{goN,waitN,goN,waitN,waitNw,waitN,waitNw,waitN}},
  {B00101001,300,{goE,goE,goE,goE,goE,goE,goE,goE}},
  {B10000101,1000,{goE,goE,waitE,waitE,waitEw,waitEw,waitE,waitEw}},
  {B01000101,300,{goN,goN,goN,goN,goN,goN,goN,goN}},
  {B00101001,300,{gow,gow,gow,gow,gow,gow,gow,gow}},
  {B00100110,1000,{gow,waitwE1,waitwN1,waitwN1,gow,waitwE1,waitwN1,waitwN1}},
  {B01000101,300,{gow,gow,gow,gow,gow,gow,gow,gow}},
  {B00100100,300,{waitwN2,waitwN2,waitwN2,waitwN2,waitwN2,waitwN2,waitwN2,waitwN2}},
  {B00100110,300,{waitwN3,waitwN3,waitwN3,waitwN3,waitwN3,waitwN3,waitwN3,waitwN3}},
  {B00100100,300,{waitwN4,waitwN4,waitwN4,waitwN4,waitwN4,waitwN4,waitwN4,waitwN4}},
  {B00100110,300,{waitwN5,waitwN5,waitwN5,waitwN5,waitwN5,waitwN5,waitwN5,waitwN5}},  
  {B00100100,300,{waitwN6,waitwN6,waitwN6,waitwN6,waitwN6,waitwN6,waitwN6,waitwN6}},
  {B00100110,300,{goN,goN,goN,goN,goN,goN,goN,goN}},
  {B00100100,300,{waitwE2,waitwE2,waitwE2,waitwE2,waitwE2,waitwE2,waitwE2,waitwE2}},
  {B00100110,300,{waitwE3,waitwE3,waitwE3,waitwE3,waitwE3,waitwE3,waitwE3,waitwE3}},
  {B00100100,300,{waitwE4,waitwE4,waitwE4,waitwE4,waitwE4,waitwE4,waitwE4,waitwE4}},
  {B00100110,300,{waitwE5,waitwE5,waitwE5,waitwE5,waitwE5,waitwE5,waitwE5,waitwE5}},
  {B00100100,300,{waitwE6,waitwE6,waitwE6,waitwE6,waitwE6,waitwE6,waitwE6,waitwE6}},
  {B00100110,300,{goE,goE,goE,goE,goE,goE,goE,goE}}
};

unsigned long s=0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_R_N,OUTPUT);
  pinMode(LED_Y_N,OUTPUT);
  pinMode(LED_G_N,OUTPUT);
  pinMode(Button_N,INPUT);

  pinMode(LED_R_E,OUTPUT);
  pinMode(LED_Y_E,OUTPUT);
  pinMode(LED_G_E,OUTPUT);
  pinMode(Button_E,INPUT);

  pinMode(LED_R_w,OUTPUT);
  pinMode(LED_G_w,OUTPUT);
  pinMode(Button_w,INPUT);
}
  int input,input1,input2,input3;
void loop() {
  digitalWrite(LED_R_w,FSM[s].ST_OUT & B00000001);
  digitalWrite(LED_G_w,FSM[s].ST_OUT & B00000010);
  
  digitalWrite(LED_R_N,FSM[s].ST_OUT & B00000100);
  digitalWrite(LED_Y_N,FSM[s].ST_OUT & B00001000);
  digitalWrite(LED_G_N,FSM[s].ST_OUT & B00010000);

  digitalWrite(LED_R_E,FSM[s].ST_OUT & B00100000);
  digitalWrite(LED_Y_E,FSM[s].ST_OUT & B01000000);
  digitalWrite(LED_G_E,FSM[s].ST_OUT & B10000000);

  delay(FSM[s].Time);
  input1 = digitalRead(Button_E);
  input2 = digitalRead(Button_N);
  input3 = digitalRead(Button_w);
  input = (input3*4)+(input2*2)+input1;
  s = FSM[s].Next[input];
}
