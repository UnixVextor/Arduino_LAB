// C++ code
//
int R1=2;
int R2 =3;
int R3 =4;
unsigned long Speed = 1000;
void setup()
{
    
}

void loop()
{  
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, HIGH);
    pinMode(R3, INPUT);
    delay(Speed);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R3, INPUT);
    delay(Speed);
    //------------------------ L1
    pinMode(R1, OUTPUT);
    digitalWrite(R1, HIGH);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R3, INPUT);
    delay(Speed);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R3, INPUT);
    delay(Speed);
    //------------------------- L2
    pinMode(R3, OUTPUT);
    digitalWrite(R3, HIGH);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R1, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R1, INPUT);
    delay(Speed);
    //-------------------------- L3
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, HIGH);
    pinMode(R1, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R1, INPUT);
    delay(Speed);
    //-------------------------- L4
    pinMode(R3, OUTPUT);
    digitalWrite(R3, HIGH);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, INPUT);
    delay(Speed);
    //--------------------------- L5
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, HIGH);
    pinMode(R2, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, INPUT);
    delay(Speed);
    //---------------------------- L6
    pinMode(R3, OUTPUT);
    digitalWrite(R3, HIGH);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, INPUT);
    delay(Speed);
    //--------------------------- L5
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, HIGH);
    pinMode(R1, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R1, INPUT);
    delay(Speed);
    //-------------------------- L4
    pinMode(R3, OUTPUT);
    digitalWrite(R3, HIGH);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R1, INPUT);
    delay(Speed);
    pinMode(R3, OUTPUT);
    digitalWrite(R3, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R1, INPUT);
    delay(Speed);
    //-------------------------- L3
    pinMode(R1, OUTPUT);
    digitalWrite(R1, HIGH);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R3, INPUT);
    delay(Speed);
    pinMode(R1, OUTPUT);
    digitalWrite(R1, LOW);
    pinMode(R2, OUTPUT);
    digitalWrite(R2, LOW);
    pinMode(R3, INPUT);
    delay(Speed);
    
    //------------------------- L2
} 
