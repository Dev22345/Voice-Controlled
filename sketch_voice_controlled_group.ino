#include <Wire.h>

#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <DHT_U.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define echoPin 9
#define trigPin 10
#define rxPin 0
#define txPin 1
SoftwareSerial BT(rxPin, txPin);
String readvoice;
#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht (DHTPIN, DHTTYPE);

long duration;
int distance; 
int motor = 3;
int motorl = 4;
int motor2 = 5;
int motor3 = 6;
int speedmotor = 255;
float hum;
float temp;
int buzzer = 2;

void setup()
{
  pinMode (motor, OUTPUT);
  pinMode (motorl, OUTPUT);
  pinMode (motor2, OUTPUT);
  pinMode (motor3, OUTPUT);
  //lcd.init();
  lcd.backlight(); 
  lcd.setCursor (0, 0); 
  lcd.print ("VOICE CONTROLLED");
  lcd.setCursor (5, 1);
  lcd.print ("ROBOT");
  pinMode (trigPin, OUTPUT); pinMode (echoPin, INPUT);
  Serial.begin(9600); dht.begin();
  BT.begin(9600);
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds (2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds (10);
  digitalWrite (trigPin, LOW);
  duration = pulseIn (echoPin, HIGH);
  distance = duration * 0.034 / 2;

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  if (temp > 70)
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print ("HIGH TEMPERATURE");
    lcd.setCursor (3, 1);
    lcd.print (temp);
    digitalWrite (buzzer, HIGH);
  }
  if (distance > 70)
  {
    digitalWrite(buzzer, LOW);
    while (BT.available())
    {
      delay(10);
      char c = BT.read();
      readvoice += c;
    }

    if (readvoice == "forward")
    {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print ("MOVING FORWARD");
      digitalWrite(motor2, HIGH);
      digitalWrite (motor3, HIGH);
      delay(100);
    }

    if (readvoice == "backward")
    {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print ("MOVING BACKWARD");
      digitalWrite(motor, HIGH);
      digitalWrite (motor, HIGH);
      delay(100);
    }

    else if (readvoice == "left")
    {
      lcd.clear();
      lcd.setCursor (1, 0);
      lcd.print ("MOVING LEFT");
      digitalWrite (motor, LOW);
      digitalWrite (motorl, LOW);
      digitalWrite (motor2, HIGH);
      digitalWrite(motor3, LOW);
      delay(100);
    }

    else if (readvoice == "right")
    {
      lcd.clear();
      lcd.setCursor (1, 0);
      lcd.print ("MOVING RIGHT");
      digitalWrite (motor, LOW);
      digitalWrite (motorl, HIGH);
      digitalWrite (motor2, LOW);
      digitalWrite(motor3, LOW);
      delay(100);
    }

    else if (readvoice == "stop")
    {
      lcd.setCursor (1, 0);
      lcd.print ("STOP"); 
      digitalWrite (motor, LOW);
      digitalWrite (motorl, LOW);
      digitalWrite (motor2, LOW);
      digitalWrite (motor3, LOW);
      delay (100);
    }

    else if (readvoice == "display temperature")
    {
      lcd.clear();
      lcd.setCursor (1, 0);
      lcd.print ("TEMPERATURE -");
      lcd.print (3, 1);
      lcd.print (temp);
    }

    else if (readvoice == "display humidity")
    {
      lcd.clear();
      lcd.setCursor (1, 0);
      lcd.print ("HUMIDITY -");
      lcd.print (3, 1);
      lcd.print (hum);
    }

    else if (readvoice == "keep watching all directions")
    {
      lcd.clear();
      lcd.setCursor (1, 0);
      lcd.print ("KEEP WATCH IN");
      lcd.print ("ALL DIRECTION");
      digitalWrite (motor, HIGH); 
      digitalWrite (motorl, LOW);
      digitalWrite (motor2, LOW);
      digitalWrite (motor3, HIGH);
      delay (500);
    }
    readvoice = "";
  }

  else
  {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print ("OBSTACLE  IS");
    lcd.setCursor (4, 1);
    lcd.print ("DETECTED");
    delay(100);
    digitalWrite(motor, LOW);
    digitalWrite(motorl, LOW);
    digitalWrite(motor2, LOW);
    digitalWrite(motor3, LOW);
    digitalWrite(buzzer, HIGH);
    lcd.clear();
  }
}
