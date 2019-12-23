/*----------NodeAQ-Aquarium-Controller
 *Programmiert durch Pascal
 *Link zu Github https://github.com/puchtuning/NodeAQ-Controller
 *Für den Privaten Gebrauch frei Nutzbar!
 *Programmiert für NodeMCU V1.0/ESP8266MOD
 *--Chanchelog
 
  - V1-0 -Freigabe
  - V1-3 - Integration MySQL API
  - V1-3-1 - Bug fixes
  - V1-4 - RTC Sync mit NTP
  - V1-6 - MQTT Support, Anpassung Webinterface

*/

//-Einstellungen für Aquarien (Bitte Anpassen)
// W-Lan Konfiguration
const char* ssid = "SSID";
const char* password = "PASSWD";

// AQ-Controller Werte
String aq_1_main_light_on = "12:00";
String aq_1_main_light_off = "21:00";
String aq_1_main_co2_on = "10:30";
String aq_1_main_co2_off = "21:00";
String aq_1_temp = "24";

// Webinterface
String AQNAME = "NODEAQ";

// MYSQL API Infos
String USE_API = "FALSE"; //TRUE -> nutzt die API Funktion / FALSE -> nutzt die API Funktion nicht
String Controller_ID = "NODEAQ";
int api_call_freq = 180;  //Default Value = 180 -> 15 Minuten

// MQTT Client Infos
String USE_MQTT = "FALSE"; //TRUE -> nutzt die MQTT Funktion / FALSE -> nutzt die MQTT Funktion nicht
int mqtt_call_freq = 12;  //Default Value = 12 -> 1 Minute

const char broker[] = "MQTTBrokerAdresse";
int        port     = 1883;
const char topic[]  = "TOPIC";

const long interval = 1000;
unsigned long previousMillis = 0;



//----------Start des Programms
// Definition der Pins für die Relays
int relay1 = 16; // D0 - Licht Aquarium 1
int relay2 = 14; // D5 - Heizung Aquarium 1
int relay3 = 12; // D6 - CO2 Aquarium 1
int relay4 = 13; // D7 - Frei
// Temperatursensoren DS18B20 werden an Pin D4 angeschlossen 



int setRTC = 0;


// Definiert die zu importierenden Libarys
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>



#define DS3231_I2C_ADDRESS 0x68 //Adresse des RTC Boards



// Inizialisieren des OneWireBus 
#define ONE_WIRE_BUS 2 //Definiert den D4 Pin als OneWire Bus 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); // èbergibt die OneWire Referenz zur Dallas Libary

// Inizialisieren des NTP Clients
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.ch.pool.ntp.org", 3600, 60000); //Einstellungen für die Zeitabfrage

//initialisiert das LCD Display
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);


// Inizialisieren des Web-Servers
WiFiServer server(80); // Netzwerkport des Wevservers
WiFiClient client;


// Inizalisiert den MQTT Client
MqttClient mqttClient(client);


// Allgemeine Variabeln
String aq_light1_status = "";
String aq_heater1_status = "";
String aq_co21_status = "";
String aq_sen1_temp = "";
String aq_rtc_status = "";

String loctime = "";
String locdate = "";
String timestamp = "";

int loopcounter1 = api_call_freq;
int loopcounter2 = mqtt_call_freq;
 
void setup() {

  lcd.begin(16,2); //Initalisiert das Display 16x2 Pixel
  DisplayFunction("Welcome to ", 0, "NodeAQ", 0);
  delay(5000);
  
  Serial.begin(115200); // Startet die Serielle Ausgabe mit der Baudrate 115200
  
  Network(); // Startet die WiFi Verbindung und Web-Server
  
  Wire.begin(); //Startet den I2C Bus
  sensors.begin(); // Startet das Auslesen der Temp Sensoren
  


  DisplayFunction("IP-Adresse", 0, WiFi.localIP().toString(), 0);

  if(USE_MQTT == "TRUE"){    
      ConnectMQTT(); //Verbindet den MQTT Client
  }

  timeClient.begin(); // Startet den NTP Client
  timeClient.setTimeOffset(3600); // Zeit Offset auf GMT+2
  setRTCfromNPT(); // Synchronisiert die RTC Zeit mit der NTP Zeit
  
  delay(5000);
  lcd.clear();

  // Definition der Ausgangspins
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);


  // Schaltet die Relays Aus
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  

}
 
void loop() {

  
  
  // Aqtualisiert die Zeit
  SyncTime();
  if(loctime == "00:00" && setRTC == 0){
    setRTCfromNPT();
    aq_rtc_status = "RTC in sync with NTP";
    setRTC = setRTC +1;   
  }
  else if(loctime == "00:00" && setRTC == 1){
    aq_rtc_status = "RTC already synced";
  }
  else{
    aq_rtc_status = "Next sync at midnight";
    setRTC = 0;
  }
  





  // Aqtualisiert die Temperatur
  sensors.requestTemperatures();                // Startet die Temperatur Abfrage
  String aq_sen1_temp = String(sensors.getTempCByIndex(0)+1); // Liest die Temperatur vom ersten Sensor aus
  float aq_sen1_temp_float = sensors.getTempCByIndex(0)+1;


  // AQ_Mainlight Steuerung
  if(aq_1_main_light_on < loctime && aq_1_main_light_off > loctime){
    aq_light1_status = "An";
    digitalWrite(relay1, LOW);
  }
  else{
    aq_light1_status = "Aus";
    digitalWrite(relay1, HIGH);
  }


  // CO2 Steuerung
  if(aq_1_main_co2_on < loctime && aq_1_main_co2_off > loctime){
    aq_co21_status = "An";
    digitalWrite(relay3, LOW);
  }
  else{
    aq_co21_status = "Aus";
    digitalWrite(relay3, HIGH);
  }


  // AQ_Temp Steuerung
  if(aq_1_temp > aq_sen1_temp && aq_sen1_temp > "1"){
    aq_heater1_status = "An";
    digitalWrite(relay2, LOW);
  }
  else if(aq_sen1_temp < "0"){
    aq_heater1_status = "Sensor Error!";
    digitalWrite(relay2, HIGH);
  }
  else{
    aq_heater1_status = "Aus";
    digitalWrite(relay2, HIGH);
  }
  



  
  //Serial Print Out
  Serial.println("-------------------");
  Serial.println("---AQ-Controller---");
  Serial.println(timestamp);
  SerialPrint("Temperatur: ",aq_sen1_temp);
  SerialPrint("Beleuchtung: ",aq_light1_status);
  SerialPrint("Heizung: ",aq_heater1_status);
  SerialPrint("CO2 Zufuhr: ",aq_co21_status);
  SerialPrint("RTC Status: ",  aq_rtc_status);
  
  
  //Informationsübergabe an den Web-Server
  HTML(aq_sen1_temp, aq_light1_status,aq_heater1_status,aq_co21_status,AQNAME); // Verlinkung auf HTML Tab und Void Function

  //Informationsübergabe an das LCD Display
  DisplayFunction("" + timestamp, 0, "Temp: " + aq_sen1_temp, 0);

  //Informationsübergabe an den MYSQL API
  if(USE_API == "TRUE"){    
    if(api_call_freq == loopcounter1){
      mysql_api(Controller_ID,loctime,locdate,aq_sen1_temp, aq_light1_status,aq_heater1_status,aq_co21_status); // Verlinkung auf HTML Tab und Void Function
      loopcounter1 = 0;
    }
    loopcounter1 = loopcounter1 + 1;
    SerialPrint("API Loop: ",String(loopcounter1));
  }

  //Informationsübergabe an den MQTTClient
  if(USE_MQTT == "TRUE"){    
    if(mqtt_call_freq == loopcounter2){
      SendMQTT(aq_sen1_temp_float, aq_light1_status,aq_heater1_status,aq_co21_status); // Verlinkung auf HTML Tab und Void Function
      loopcounter2 = 0;
    }
    loopcounter2 = loopcounter2 + 1;
    SerialPrint("MQTT Loop: ",String(loopcounter2));
  }
  Serial.println("-------------------");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

 
  


  
  
  delay(5000); //Allgemeines Delay damit der NodeMCU nicht durchdreht ;)
 
}



 
