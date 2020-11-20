#include <Arduino.h>

#define IN1 A2
#define IN2 A3
#define IN3 A6
#define IN4 A7

#define J1 PB0
#define J2 PB1

int mid = 0;
int mn = 0;
int mx = 0;


int sen0 = 0;
int sen1 = 0;
int sen2 = 0;
int sen3 = 0;
int sen4 = 0;
int sen5 = 0;

const int LEDPin = PC13;
char inputData = 0;
// int mulai = 0;
// char mode ='s';

void maju();
void mundur();
void kiri();
void kanan();
void stop();
void otomatis(); 

void setup()
{
    Serial.begin(9600);
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(J1, OUTPUT);
    pinMode(J2, OUTPUT);

    digitalWrite(PC13, LOW);

    Serial.println("IROSTECH");
    Serial.println("Dump Truck");

    for(int i=0; i<5000; i++)
    {
        digitalWrite(PC13, HIGH);

        int val = 0;
        for(int j=0; j<=5; j++)//Calibrating the sensor, finding max and 
        {                      //min reflectance values.
            val = analogRead(j);
            if(val >= mx)
            mx = val;
            if(val <= mn)
            mn = val;
        }
        delay(1);
    }    

    mid = (((mx + mn)/2)+250);;

    pinMode(LEDPin, OUTPUT);
}

void loop()
{
    if(Serial.available() > 0){
    inputData = Serial.read();
        if(inputData == 'f'){
                //maju
                analogWrite(J1, 180);
                analogWrite(J2, 0);
                digitalWrite(LEDPin, HIGH);
                Serial.println("Bak Naik");
            }

        else if(inputData == 'g')
            {
                analogWrite(J1, 0);
                analogWrite(J2, 0);   
                stop();
                digitalWrite(LEDPin, LOW);
                Serial.println("Stop");
            }

        else if(inputData == 'h')
            {
                maju();
                digitalWrite(LEDPin, HIGH);
                Serial.println("Maju!");
            }
            
        else  if(inputData == 'i')
            { // mundur
                mundur();
                digitalWrite(LEDPin, LOW);
                Serial.println("Mundur!");
            }

        else  if(inputData == 'j')
            { // kanan
                kanan();
                Serial.println("kanan!");
            }

        else  if(inputData == 'k')
            { // kanan
                kiri();
                Serial.println("kiri!");
            }

        else  if(inputData == 'a')
            { // auto
                otomatis();
            }
        }
}

void maju(){
    analogWrite(IN1, 180);
    analogWrite(IN2, 0);
    analogWrite(IN3, 180);
    analogWrite(IN4, 0);    
}

void mundur(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 180);
    analogWrite(IN3, 0);
    analogWrite(IN4, 180);    
}

void kiri(){
    analogWrite(IN1, 180);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);    
}

void kanan(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 180);
    analogWrite(IN4, 0);
}

void stop(){
    analogWrite(IN1, 0);
    analogWrite(IN2, 0);
    analogWrite(IN3, 0);
    analogWrite(IN4, 0);
}

void otomatis(){
 
    while(1){
        analogWrite(IN1, 200);
        analogWrite(IN2, 0);
        analogWrite(IN3, 200);
        analogWrite(IN4, 0);
        digitalWrite(LEDPin, HIGH);


        delay(1);

        int sen0 = analogRead(A0);
        int sen1 = analogRead(A1);
        int sen2 = analogRead(PB4);
        int sen3 = analogRead(A4);
        int sen4 = analogRead(A5);

        Serial.print("Mid");
        Serial.print(mid); 
        Serial.print(" ");

        Serial.print("Sens0 :");
        Serial.print(sen0);
        Serial.print(" ");

        Serial.print("Sens1 :");
        Serial.print(sen1);
        Serial.print(" ");

        Serial.print("Sens2 :");
        Serial.print(sen2);
        Serial.print(" ");

        Serial.print("Sens3 :");
        Serial.print(sen3);
        Serial.print(" ");

        Serial.print("Sens4 :");
        Serial.print(sen4);
        Serial.print(" ");
        Serial.println();

        Serial.print("Sens5 :");
        Serial.print(sen5);
        Serial.print(" ");
        Serial.println();

        // jalan maju

        if(((sen0 + sen1 + sen2)/3) < (((sen2 + sen3 + sen4)/3)-200))//Move LEFT
        {
            analogWrite(IN1, 200);
            analogWrite(IN2, 0);
            analogWrite(IN3, 0);
            analogWrite(IN4, 0);
            Serial.print("KIRI");
            delay(abs((((sen0 + sen1 +sen2)/3)-((sen2 + sen3 + sen4)/3))/2));
        }
        
        if((((sen0 + sen1 + sen3)/3) > (((sen2 + sen3 + sen4)/3)+200)))//Move RIGHT
        {
            analogWrite(IN1, 0);
            analogWrite(IN2, 0);
            analogWrite(IN3, 200);
            analogWrite(IN4, 0);
            Serial.print("KANAN");
            delay(abs((((sen0 + sen1 + sen2)/3)-((sen2 + sen3 + sen4)/3))/2));
        }

        //Kondisi Line Follower berhenti ketika sensor 1 dan sensor 4 lebih tinggi dari nilai 1023

        if((sen0 > mid)&&(sen1 > mid)&&(sen3 > mid)&&(sen4 > mid)) 
        {                         
            analogWrite(IN1, 0);
            analogWrite(IN2, 0);
            analogWrite(IN3, 0);
            analogWrite(IN4, 0);
            Serial.print(" STOP");


            for(int k=0; k<50; k++)
            {
                digitalWrite(PC13, HIGH);
                delay(100);
                digitalWrite(PC13, LOW);
                delay(100);
            }
        
        
            delay(1000);

        }

        if(Serial.available())
        {
            char newBT = (char)Serial.read();
            if(newBT=='t'){stop();break;}
        }
    }
}
