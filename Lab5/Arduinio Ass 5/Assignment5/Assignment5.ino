#define Red_s 3
#define Yellow_s 4
#define Green_s 5
#define button_s 2

#define Red_w 6
#define Yellow_w 7
#define Green_w 8
#define button_w 9

#define Red_walk 10
#define Green_walk 11
#define button_walk 12

#define goW   0
#define waitW 1
#define goS   2
#define waitS 3


struct State{
  unsigned long ST_OUT;
  unsigned long Time;
  unsigned long Next[4];  
};
typedef const struct State SType;
SType FSM[4]{
  {B01100001,1000,{goS,waitS,goS,waitS}},
  {B01100010,300,{goW,goW,goW,goW}},  
  {B01001100,1000,{goW,goW,waitW,waitW}},
  {B01010100,300,{goS,goS,goS,goS}} 
};
unsigned long s=0;

void setup(){
  Serial.begin(9600);
  pinMode(Red_s,OUTPUT);
  pinMode(Yellow_s,OUTPUT);
  pinMode(Green_s,OUTPUT);
  pinMode(button_s,INPUT);

  pinMode(Red_w,OUTPUT);
  pinMode(Yellow_w,OUTPUT);
  pinMode(Green_w,OUTPUT);
  pinMode(button_w,INPUT);

  pinMode(Red_walk,OUTPUT);
  pinMode(Green_walk,OUTPUT);
  pinMode(button_walk,INPUT);
  
 }
  int input,input1,input2,input3;
 
 void loop(){
  digitalWrite(Red_s,FSM[s].ST_OUT &    B00000001);
  digitalWrite(Yellow_s,FSM[s].ST_OUT & B00000010);
  digitalWrite(Green_s,FSM[s].ST_OUT &  B00000100);

  digitalWrite(Red_w,FSM[s].ST_OUT &    B00001000);
  digitalWrite(Yellow_w,FSM[s].ST_OUT & B00010000);
  digitalWrite(Green_w,FSM[s].ST_OUT &  B00100000);

  digitalWrite(Red_walk,FSM[s].ST_OUT & B01000000);
  digitalWrite(Green_walk,FSM[s].ST_OUT & B10000000);

  delay(FSM[s].Time);
  input1 = digitalRead(button_w);
  input2 = digitalRead(button_s);
  input3 = digitalRead(button_walk);
  input =(input2*2)+input1;
  Serial.println(B00000111 & B00000001);
  s = FSM[s].Next[input];
  }
