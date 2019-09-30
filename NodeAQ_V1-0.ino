/*----------Aquarium-Controller
 *Programmiert durch Pascal Rusca
 *Link zu Github
 *Für den Privaten Gebrauch frei Nutzbar!
 *Programmiert für NodeMCU V1.0/ESP8266MOD
 *--Chanchelog
  - V1-0 -Freigabe

*/

//-Einstellungen für Aquarien (Bitte Anpassen)
// W-Lan Konfiguration
const char* ssid = "SSID";
const char* password = "PASSWORT!";

// AQ-Controller Werte
String aq_1_main_light_on = "12:00:00";
String aq_1_main_light_off = "21:00:00";
String aq_2_main_light_on = "12:00:00";
String aq_2_main_light_off = "21:00:00";
String aq_1_main_co2_on = "11:30:00";
String aq_1_main_co2_off = "20:30:00";
String aq_2_main_co2_on = "11:30:00";
String aq_2_main_co2_off = "20:30:00";
String aq_1_temp = "24";
String aq_2_temp = "25";

// Definition der Pins für die Relays
int relay1 = 16; // D0 - Licht Aquarium 1
int relay2 = 14; // D5 - Licht Aquarium 2
int relay3 = 12; // D6 - Heizung Aquarium 1
int relay4 = 13; // D7 - Heizung Aquarium 2
int relay5 = 12; // SD2 - CO2 Aquarium 1
int relay6 = 13; // SD3 - CO2 Aquarium 2
// Temperatursensoren DS18B20 werden an Pin D4 angeschlossen 



//----------Start des Programms

// Definiert die zu importierenden Libarys
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Inizialisieren des OneWireBus 
#define ONE_WIRE_BUS 2 //Definiert den D4 Pin als OneWire Bus 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); // èbergibt die OneWire Referenz zur Dallas Libary

// Inizialisieren des NTP Clients
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "0.ch.pool.ntp.org", 3600, 60000); //Einstellungen für die Zeitabfrage

// Inizialisieren des Web-Servers
WiFiServer server(80); // Netzwerkport des Wevservers

// Allgemeine Variabeln
String aq_light1_status = "";
String aq_light2_status = "";
String aq_heater1_status = "";
String aq_heater2_status = "";
String aq_co21_status = "";
String aq_co22_status = "";
String aq_sen1_temp = "";
String aq_sen2_temp = "";

 
void setup() {
  Serial.begin(115200); // Startet die Serielle Ausgabe mit der Baudrate 115200
  sensors.begin(); // Startet das Auslesen der Temp Sensoren
  timeClient.begin(); // Startet den NTP Client
  Network(); // Startet die WiFi Verbindung und Web-Server
  delay(10);

  // Definition der Ausgangspins
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  // Schaltet die Relays Aus
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);  
}
 
void loop() {

  
  
  // Aqtualisiert die Zeit
  timeClient.update();
  String loctime = timeClient.getFormattedTime();

  // Aqtualisiert die Temperatur
  sensors.requestTemperatures();                // Startet die Temperatur Abfrage
  String aq_sen1_temp = String(sensors.getTempCByIndex(0)); // Liest die Temperatur vom ersten Sensor aus
  String aq_sen2_temp = String(sensors.getTempCByIndex(1)); // Liest die Temperatur vom zweiten Sensor aus

  // AQ_Mainlight Steuerung
  if(aq_1_main_light_on < loctime && aq_1_main_light_off > loctime){
    aq_light1_status = "An";
    digitalWrite(relay1, LOW);
  }
  else{
    aq_light1_status = "Aus";
    digitalWrite(relay1, HIGH);
  }
  if(aq_2_main_light_on < loctime && aq_2_main_light_off > loctime){
    aq_light2_status = "An";
    digitalWrite(relay2, LOW);
  }
  else{
    aq_light2_status = "Aus";
    digitalWrite(relay2, HIGH);
  }

  // CO2 Steuerung
  if(aq_1_main_co2_on < loctime && aq_1_main_co2_off > loctime){
    aq_co21_status = "An";
    digitalWrite(relay1, LOW);
  }
  else{
    aq_co21_status = "Aus";
    digitalWrite(relay1, HIGH);
  }
  if(aq_2_main_co2_on < loctime && aq_2_main_co2_off > loctime){
    aq_co22_status = "An";
    digitalWrite(relay2, LOW);
  }
  else{
    aq_co22_status = "Aus";
    digitalWrite(relay2, HIGH);
  }

  // AQ_Temp Steuerung
  if(aq_1_temp > aq_sen1_temp){
    aq_heater1_status = "An";
    digitalWrite(relay3, LOW);
  }
  else{
    aq_heater1_status = "Aus";
    digitalWrite(relay3, HIGH);
  }
  
  if(aq_2_temp > aq_sen2_temp){
    aq_heater2_status = "An";
    digitalWrite(relay4, LOW);
  }
  else{
    aq_heater2_status = "Aus";
    digitalWrite(relay4, HIGH);
  }

  //Informationsübergabe an den Web-Server
  HTML(aq_sen1_temp, aq_sen2_temp,aq_light1_status,aq_light2_status,aq_heater1_status,aq_heater2_status,aq_co21_status,aq_co22_status); // Verlinkung auf HTML Tab und Void Function
  
  //Serial Print Out
  Serial.println("-------------------");
  Serial.println("---AQ-Controller---");
  Serial.println(loctime);
  Serial.println("---Aquarium 1------");
  SerialPrint("Temperatur: ",aq_sen1_temp);
  SerialPrint("Beleuchtung: ",aq_light1_status);
  SerialPrint("Heizung: ",aq_heater1_status);
  SerialPrint("CO2 Zufuhr: ",aq_co21_status);
  Serial.println("---Aquarium 2------");
  SerialPrint("Temperatur: ",aq_sen2_temp);
  SerialPrint("Beleuchtung: ",aq_light2_status);
  SerialPrint("Heizung: ",aq_heater1_status);
  SerialPrint("CO2 Zufuhr: ",aq_co22_status);
  Serial.println("-------------------");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  
  
  delay(5000); //Allgemeines Delay damit der NodeMCU nicht durchdreht ;)
 
}



 
