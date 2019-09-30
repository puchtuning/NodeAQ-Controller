/*----------Aquarium-Controller
 *Programmiert durch Pascal
 *Link zu Github https://github.com/puchtuning/NodeAQ-Controller
 *Für den Privaten Gebrauch frei Nutzbar!
 *Programmiert für NodeMCU V1.0/ESP8266MOD
 *--Chanchelog
 
  - V1-0 -Freigabe
  - V1-3 - Integration MySQL API

*/

//-Einstellungen für Aquarien (Bitte Anpassen)
// W-Lan Konfiguration
const char* ssid = "SSID";
const char* password = "PASSWORT!";

// AQ-Controller Werte
String aq_1_main_light_on = "12:00";
String aq_1_main_light_off = "21:00";
String aq_1_main_co2_on = "11:30";
String aq_1_main_co2_off = "20:30";
String aq_1_temp = "24";


// Definition der Pins für die Relays
int relay1 = 16; // D0 - Licht Aquarium 1
int relay2 = 14; // D5 - Heizung Aquarium 1
int relay3 = 12; // D6 - CO2 Aquarium 1
int relay4 = 13; // D7 - Frei
// Temperatursensoren DS18B20 werden an Pin D4 angeschlossen 

// MYSQL API Infos
String USE_API = "FALSE"; //TRUE -> nutzt die API Funktion / FALSE -> nutzt die API Funktion nicht
String Controller_ID = "NM01"; // Controller ID wird zur Identifizierung in der Datenbank verwendet
int api_call_freq = 180;  //Default Value = 180 -> alle 15 Minuten


//----------Start des Programms

// Definiert die zu importierenden Libarys
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <time.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>



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
String aq_heater1_status = "";
String aq_co21_status = "";
String aq_sen1_temp = "";

String loctime = "";
String locdate = "";
String timestamp = "";

int loopcounter = api_call_freq;

 
void setup() {
  Serial.begin(115200); // Startet die Serielle Ausgabe mit der Baudrate 115200
  sensors.begin(); // Startet das Auslesen der Temp Sensoren
  timeClient.begin(); // Startet den NTP Client
  timeClient.setTimeOffset(7200); // Zeit Offset auf GMT+2 Zürich
  Network(); // Startet die WiFi Verbindung und Web-Server
  delay(10);

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
  





  // Aqtualisiert die Temperatur
  sensors.requestTemperatures();                // Startet die Temperatur Abfrage
  String aq_sen1_temp = String(sensors.getTempCByIndex(0)); // Liest die Temperatur vom ersten Sensor aus


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
    digitalWrite(relay1, LOW);
  }
  else{
    aq_co21_status = "Aus";
    digitalWrite(relay1, HIGH);
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
  



  
  //Serial Print Out
  Serial.println("-------------------");
  Serial.println("---AQ-Controller---");
  Serial.println(timestamp);
  SerialPrint("Temperatur: ",aq_sen1_temp);
  SerialPrint("Beleuchtung: ",aq_light1_status);
  SerialPrint("Heizung: ",aq_heater1_status);
  SerialPrint("CO2 Zufuhr: ",aq_co21_status);
  
  //Informationsübergabe an den Web-Server
  HTML(aq_sen1_temp, aq_light1_status,aq_heater1_status,aq_co21_status); // Verlinkung auf HTML Tab und Void Function

  //Informationsübergabe an den MYSQL API
  if(USE_API == "TRUE"){    
    if(api_call_freq == loopcounter){
      mysql_api(Controller_ID,loctime,locdate,aq_sen1_temp, aq_light1_status,aq_heater1_status,aq_co21_status); // Verlinkung auf HTML Tab und Void Function
      loopcounter = 0;
    }
    loopcounter = loopcounter + 1;
    SerialPrint("API Loop: ",String(loopcounter));
  }
  Serial.println("-------------------");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

 
   


  
  
  delay(5000); //Allgemeines Delay damit der NodeMCU nicht durchdreht ;)
 
}



 
