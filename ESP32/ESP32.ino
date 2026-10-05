#define BLYNK_TEMPLATE_ID "xxxxxxxxxxxxx"             //Insert your Blynk Template ID
#define BLYNK_TEMPLATE_NAME "XXXXX"                   //Insert your Blynk Template Name
#define BLYNK_AUTH_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxx" //Insert your Blynk Authentication Token

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "WIFI_NAME";        //Change to your WiFi Name and Password
char pass[] = "WIFI_PASSWORD";

HardwareSerial ArduinoSerial(2);

// Sensor values
float temp = 0;
float hum = 0;
float soil = 0;
float light = 0;

int dayState = 0;
int plant = 0;
int stage = 0;

// Preset values
int tempmin = 0;
int tempmax = 0;
int hummin = 0;
int hummax = 0;
int soilmin = 0;
int soilmax = 0;
int lmin = 0;
int lhrs = 0;

// Prevent preset overwrite
int lastPlant = -1;
int lastStage = -1;

// Names
const char* plantNames[]={
"Tomato",
"Lettuce",
"Chilli"
};

const char* stageNames[]={
"Sprout",
"Seedling",
"Vegetative",
"Budding",
"Flowering",
"Ripening"
};

void setup(){
  Serial.begin(115200);

  ArduinoSerial.begin(9600, SERIAL_8N1, 16, 17);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop(){
  Blynk.run();

  if(ArduinoSerial.available()){
    String data = ArduinoSerial.readStringUntil('\n');

    if(data.startsWith("DATA")){
      sscanf(data.c_str(),
      "DATA,%f,%f,%f,%f,%d,%d,%d",
      &temp,&hum,&soil,&light,
      &dayState,&plant,&stage);

      plant = constrain(plant,0,2);
      stage = constrain(stage,0,5);

      Blynk.virtualWrite(V0,temp);
      Blynk.virtualWrite(V1,hum);
      Blynk.virtualWrite(V2,soil);
      Blynk.virtualWrite(V3,light);

      Blynk.virtualWrite(V4,plantNames[plant]);
      Blynk.virtualWrite(V5,stageNames[stage]);

      Blynk.virtualWrite(V6, dayState ? "DAY" : "NIGHT");
    }

    if(data.startsWith("PRESET") && (plant!=lastPlant || stage!=lastStage)){
      sscanf(data.c_str(),
      "PRESET,%d,%d,%d,%d,%d,%d,%d,%d",
      &tempmin,&tempmax,
      &hummin,&hummax,
      &soilmin,&soilmax,
      &lmin,&lhrs);

      Blynk.virtualWrite(V10,tempmin);
      Blynk.virtualWrite(V11,tempmax);
      Blynk.virtualWrite(V12,hummin);
      Blynk.virtualWrite(V13,hummax);
      Blynk.virtualWrite(V14,soilmin);
      Blynk.virtualWrite(V15,soilmax);
      Blynk.virtualWrite(V16,lmin);
      Blynk.virtualWrite(V17,lhrs);

      lastPlant = plant;
      lastStage = stage;
    }

  }

}

// RECEIVE PRESET FROM DASHBOARD

BLYNK_WRITE(V10){ tempmin = param.asInt(); }
BLYNK_WRITE(V11){ tempmax = param.asInt(); }
BLYNK_WRITE(V12){ hummin = param.asInt(); }
BLYNK_WRITE(V13){ hummax = param.asInt(); }
BLYNK_WRITE(V14){ soilmin = param.asInt(); }
BLYNK_WRITE(V15){ soilmax = param.asInt(); }
BLYNK_WRITE(V16){ lmin = param.asInt(); }
BLYNK_WRITE(V17){ lhrs = param.asInt(); }

// SEND PRESET TO ARDUINO

BLYNK_WRITE(V18){
  if(param.asInt()){

    ArduinoSerial.print("SET,");
    ArduinoSerial.print(plant); ArduinoSerial.print(",");
    ArduinoSerial.print(stage); ArduinoSerial.print(",");

    ArduinoSerial.print(tempmin); ArduinoSerial.print(",");
    ArduinoSerial.print(tempmax); ArduinoSerial.print(",");
    ArduinoSerial.print(hummin); ArduinoSerial.print(",");
    ArduinoSerial.print(hummax); ArduinoSerial.print(",");
    ArduinoSerial.print(soilmin); ArduinoSerial.print(",");
    ArduinoSerial.print(soilmax); ArduinoSerial.print(",");
    ArduinoSerial.print(lmin); ArduinoSerial.print(",");
    ArduinoSerial.println(lhrs);

    Blynk.virtualWrite(V18,0);
  }
}

// STAGE CONTROL

BLYNK_WRITE(V20){
  if(param.asInt()){
    ArduinoSerial.println("NEXT");
    Blynk.virtualWrite(V20,0);
  }
}

BLYNK_WRITE(V21){
  if(param.asInt()){
    ArduinoSerial.println("PREV");
    Blynk.virtualWrite(V21,0);
  }
}