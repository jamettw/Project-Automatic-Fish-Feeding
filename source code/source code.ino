#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Servo.h>

char auth[] = "ใส่ Token ที่นี่";
char ssid[] = "ใส่ ชื่อ WIFI ที่นี่";
char pass[] = "ใส่ รหัส WIFI ที่นี่";

int check = 0;
int totalsec = 0;
int realtime = 0;
int morning = 21600;
int midday = 43200;
int evening = 64800;
int shour = 0;
int smin = 0;

BlynkTimer timer;
WidgetRTC rtc;
Servo servo;

void setup()
{ 
  pinMode(D1,OUTPUT);
  pinMode(D2,OUTPUT);
  pinMode(D3,OUTPUT);
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  rtc.begin();
  servo.attach(D4);
  servo.write(0);
  timer.setInterval(1000L, clockDisplay);
}
 
void loop()
{
  Blynk.run();
  timer.run();
}

void clockDisplay()
{
  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  realtime = (hour()*3600)+(minute()*60)+ second();
  if (totalsec==realtime && check==0){
    servo.write(180);
    delay(500);
    servo.write(0);
  }
  Blynk.virtualWrite(V1, currentTime);
  Serial.print("Time : ");
  Serial.println(currentTime);
}

BLYNK_WRITE(V2)
{
   servo.write(param.asInt());
}

BLYNK_WRITE(V3)
{
  servo.write(180);
  delay(500);
  servo.write(0);
}
BLYNK_WRITE(V4) {
  long startTimeInSecs = param[0].asLong();
  totalsec = startTimeInSecs;
  shour = totalsec/3600;
  smin = (totalsec%3600)/60;
  Serial.print("Time Set : ");
  Serial.print(shour);
  Serial.print(":");
  Serial.println(smin);
}
BLYNK_WRITE(V5) {
  switch (param.asInt())
  {
    case 1: // None
      check = 0;
      digitalWrite(D1,HIGH);
      digitalWrite(D2,LOW);
      digitalWrite(D3,LOW);
      Serial.println("Mode : None");
      break;
      
    case 2: // 2 Times
      check = 1;
      if ((morning==realtime||evening==realtime) && check==0){
        servo.write(180);
        delay(500);
        servo.write(0);
      }
      digitalWrite(D1,HIGH);
      digitalWrite(D2,HIGH);
      digitalWrite(D3,HIGH);
      Serial.println("Mode : 2 Times");
      break;
      
    case 3: // 3 Times
      check = 1;
      if ((morning==realtime||midday==realtime||evening==realtime) && check==0){
        servo.write(180);
        delay(500);
        servo.write(0);
      }
      digitalWrite(D1,LOW);
      digitalWrite(D2,LOW);
      digitalWrite(D3,HIGH);
      Serial.println("Mode : 3 Times");
      break;
  }
}
