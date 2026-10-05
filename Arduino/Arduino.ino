#include <Wire.h>
#include <DS1307.h>
#include <DHT11.h>
#include <LiquidCrystal.h>
#include <Adafruit_NeoPixel.h>

// LCD
LiquidCrystal lcd(6,7,8,9,10,11);

// DHT
DHT11 dht11(13);

// RTC
DS1307 rtc(A4,A5);
Time t;

// Sensors
#define SOIL A1
#define LDR A0

// RGB
#define LAMP A3
#define NUMPIXELS 3
Adafruit_NeoPixel pixels(NUMPIXELS, LAMP, NEO_GRB + NEO_KHZ800);

// Relays
#define PUMP 5
#define MIST 4
#define FAN 2

// Buttons
#define BTN_NEXT A2
#define BTN_PREV 12

// Plants
const char* plants[]={"Tomato","Lettuce","Chilli"};

const char* stages[]={
  "Sprout","Seedling","Vegetative",
  "Budding","Flowering","Ripening"
};

int plant=0;
int stage=0;

struct Preset{
  int tempmin,tempmax;
  int humiditymin,humiditymax;
  int submin,submax;
  int lmin;
  int lhrs;
};

Preset plantPreset[3][6]={

  {
    {20,25,60,70,50,70,200,8},
    {22,26,60,70,50,70,200,8},
    {22,28,60,70,50,70,300,10},
    {22,28,60,70,50,70,300,10},
    {20,26,55,65,50,70,300,10},
    {18,25,50,60,50,70,300,8}
  },

  {
    {18,22,60,70,50,70,200,8},
    {18,22,60,70,50,70,200,8},
    {18,24,60,70,50,70,300,10},
    {18,24,60,70,50,70,300,10},
    {18,24,60,70,50,70,300,10},
    {16,22,55,65,50,70,200,8}
  },

  {
    {22,28,60,70,50,70,200,8},
    {22,28,60,70,50,70,200,8},
    {22,30,60,70,50,70,300,10},
    {22,30,60,70,50,70,300,10},
    {22,30,60,70,50,70,300,10},
    {20,28,55,65,50,70,300,8}
  }

};

unsigned long lastLCD=0;
unsigned long lastSense=0;
unsigned long lastBtn=0;
unsigned long lastSerial=0;

byte lcdMode=0;

float temp=0,h=0;
int soilRaw=0,soilNew=0;
int lightRaw=0,lightPercent=0;

bool isDay=true;

void setup(){

  lcd.begin(16,2);
  pixels.begin();

  Serial.begin(9600);

  rtc.halt(false);

  pinMode(PUMP,OUTPUT);
  pinMode(MIST,OUTPUT);
  pinMode(FAN,OUTPUT);

  pinMode(BTN_NEXT,INPUT);
  pinMode(BTN_PREV,INPUT);

  digitalWrite(PUMP,HIGH);
  digitalWrite(MIST,HIGH);
  digitalWrite(FAN,HIGH);

  lcd.print("Plant System");
  delay(2000);
  lcd.clear();
}

void nextStage(){

  stage++;

  if(stage>5){
    stage=0;
    plant=(plant+1)%3;
  }

  lcd.clear();
}

void prevStage(){

  if(stage==0){
    stage=5;
    plant=(plant+2)%3;
  }
  else stage--;

  lcd.clear();
}

void loop(){

  unsigned long now=millis();

  if(digitalRead(BTN_NEXT)==HIGH && now-lastBtn>200){
    lastBtn=now;
    nextStage();
  }

  if(digitalRead(BTN_PREV)==HIGH && now-lastBtn>200){
    lastBtn=now;
    prevStage();
  }

  if(now-lastSense>500){

    lastSense=now;

    temp=dht11.readTemperature();
    h=dht11.readHumidity();

    soilRaw=analogRead(SOIL);
    soilNew=constrain(map(soilRaw,225,500,100,0),0,100);

    lightRaw=analogRead(LDR);
    lightPercent = map(lightRaw,0,1023,0,100);

    t=rtc.getTime();

    Preset p=plantPreset[plant][stage];

    int dayStart=6;
    int dayEnd=dayStart+p.lhrs;

    isDay=(t.hour>=dayStart && t.hour<dayEnd);
    isDay = true;

    if(isDay && lightRaw<p.lmin){

      pixels.setPixelColor(0,pixels.Color(255,255,255));
      pixels.setPixelColor(1,pixels.Color(255,255,255));
      pixels.setPixelColor(2,pixels.Color(255,255,255));
      pixels.show();

    }
    else{

      pixels.clear();
      pixels.show();

    }

    if(soilNew<p.submin) digitalWrite(PUMP,LOW);
    else if(soilNew>p.submax) digitalWrite(PUMP,HIGH);

    if(h<p.humiditymin) digitalWrite(MIST,LOW);
    else if(h>p.humiditymax) digitalWrite(MIST,HIGH);

    if(temp>p.tempmax || h>p.humiditymax) digitalWrite(FAN,LOW);
    else digitalWrite(FAN,HIGH);

  }

  if(now-lastSerial>2000){

    lastSerial=now;

    Preset p=plantPreset[plant][stage];

    Serial.print("DATA,");
    Serial.print(temp);Serial.print(",");
    Serial.print(h);Serial.print(",");
    Serial.print(soilNew);Serial.print(",");
    Serial.print(lightPercent);Serial.print(",");
    Serial.print(isDay);Serial.print(",");
    Serial.print(plant);Serial.print(",");
    Serial.println(stage);

    Serial.print("PRESET,");
    Serial.print(p.tempmin);Serial.print(",");
    Serial.print(p.tempmax);Serial.print(",");
    Serial.print(p.humiditymin);Serial.print(",");
    Serial.print(p.humiditymax);Serial.print(",");
    Serial.print(p.submin);Serial.print(",");
    Serial.print(p.submax);Serial.print(",");
    Serial.print(p.lmin);Serial.print(",");
    Serial.println(p.lhrs);

  }

  static String serialBuffer="";

  while(Serial.available()){
    char c=Serial.read();

    if(c=='\n'){
      String msg=serialBuffer;
      serialBuffer="";

      msg.trim();

      if(msg=="NEXT") nextStage();

      else if(msg=="PREV") prevStage();

      else if(msg.startsWith("SET")){
        int pIndex,sIndex;
        Preset newPreset;

        int parsed = sscanf(msg.c_str(),
          "SET,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
          &pIndex,&sIndex,
          &newPreset.tempmin,
          &newPreset.tempmax,
          &newPreset.humiditymin,
          &newPreset.humiditymax,
          &newPreset.submin,
          &newPreset.submax,
          &newPreset.lmin,
          &newPreset.lhrs
        );

        if(parsed==10){
          if(pIndex>=0 && pIndex<3 && sIndex>=0 && sIndex<6){
              plantPreset[pIndex][sIndex] = newPreset;
          }
        }
      }
    }
    else{
      serialBuffer+=c;
    }
  }

  if(now-lastLCD>2000){

    lastLCD=now;

    lcd.setCursor(0,0);
    lcd.print("                ");
    lcd.setCursor(0,0);

    lcd.print(plants[plant]);
    lcd.print(" ");
    lcd.print(stages[stage]);

    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(0,1);

    if(lcdMode==0){

      lcd.print("T:");
      lcd.print((int)temp);
      lcd.write(223);
      lcd.print("C ");

      lcd.print("H:");
      lcd.print((int)h);
      lcd.print("%");

    }

    else if(lcdMode==1){

      lcd.print("Soil:");
      lcd.print(soilNew);
      lcd.print("%");

    }

    else if(lcdMode==2){

      lcd.print("Light:");
      lcd.print(lightPercent);
      lcd.print("%");

    }

    else{

      lcd.print(isDay?"DAY":"NIGHT");

    }

    lcdMode=(lcdMode+1)%4;

  }

}