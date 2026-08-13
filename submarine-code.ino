
#include <Wire.h> 
#include <OneWire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#define J1 2
#define J2 3
#define J3 4
#define J4 5
#define R1 6
#define R2 7
#define R3 8
#define R4 9
int JS1 = 0, JS2 = 0, JS3=0, JS4=0;
const int tmp = 13;
float V, KP, DP;
float tmc; 

OneWire oneWire(tmp);         
DallasTemperature tempSensor(&oneWire); 
LiquidCrystal_I2C lcd(0x3F,20,4);  


void setup()

{
  
  Serial.begin(9600);
  //pinMode
  pinMode(J1,INPUT);
  pinMode(J2,INPUT);
  pinMode(J3,INPUT);
  pinMode(J4,INPUT);
  pinMode(R1,OUTPUT);
  pinMode(R2,OUTPUT);
  pinMode(R3,OUTPUT);
  pinMode(R4,OUTPUT);
  //LCD code
  //scl= A5 pin   rcl= A4 pin
  lcd.init();                      
 lcd.init();
  lcd.backlight();
  lcd.setCursor(5,0);
  lcd.print("Submarine");
  lcd.setCursor(0,1);
  lcd.print("Depth:");
  lcd.setCursor(13,1);
  lcd.print("M");
  lcd.setCursor(0,2);
  lcd.print("Temperature:");
  lcd.setCursor(0,3);
  lcd.print("WPressure:");
  lcd.setCursor(17,3);
lcd.print("KPA");

tempSensor.begin();

}

void loop()

{
  loop1();
  loop2();
}

void loop1()

{ 
  
  //water pressuer senor 
  //water pressuer senor = Pin A0
  
   V = analogRead(0) * 5.00 / 1024; 
  KP = (V - 0.4882) * 250;                //μετατροπή τάσης σε KPA 
  DP = KP * 0.101972;                    //μετατροπή από KPA σε μέτρα νερού         
  lcd.setCursor(11,3);
lcd.print(KP);
lcd.setCursor(7,1);
lcd.print(DP);

//temperature senosor 

tempSensor.requestTemperatures();             // send the command to get temperatures
tmc = tempSensor.getTempCByIndex(0);  // read temperature in Celsius
lcd.setCursor(13,2);
lcd.print(tmc);
lcd.setCursor(19,2);
lcd.print("C");
  delay(500);

}
void loop2()
//εντολές joystick
{
    JS1 = digitalRead(J1);
    JS2 = digitalRead(J2);
     JS3 = digitalRead(J3);
     JS4 = digitalRead(J4);
  if (JS1 == HIGH) 
  {
  digitalWrite(R2,LOW);
  digitalWrite(R4,LOW);
  digitalWrite(R1,HIGH);
  digitalWrite(R3,HIGH);
    
    }
  else if (JS2 == HIGH) 
  {
  digitalWrite(R1,LOW);
  digitalWrite(R3,LOW);
  digitalWrite(R2,HIGH);
  digitalWrite(R4,HIGH);
    
    }
  else if (JS3 == HIGH) 
  {
    digitalWrite(R2,LOW);
    digitalWrite(R3,LOW);
  digitalWrite(R1,HIGH);
  digitalWrite(R4,LOW);
    
    }
  else if (JS4 == HIGH) 
  {
    digitalWrite(R1,LOW);
    digitalWrite(R4,LOW);
  digitalWrite(R2,LOW);
  digitalWrite(R3,HIGH);
  } 
}