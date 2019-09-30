# Willkommen auf dem NodeAQ-Controller WiKi

Hier wird Ihnen geholfen einen NodeAQ-Controller aufzusetzen.



***

## Installation
Die Installation setzt Kenntnisse mit dem Umgang der Ardoino IDE und dem NodeMCU voraus

### Installiere die ArduinoIDE
Lade Sie die Installationsdateien von folgendem Seite herunter und installieren diese.

[Arduino Software Seite](https://www.arduino.cc/en/Main/Software)



### Libaris installieren
Um die benötigten Libaris installieren zu können, müssen Sie die ArduinoIDE zuerst öffnen.
Sobald das Programm läuft, wählen Sie den Menupunkt Werkzeuge - Bibliotheken verwalten... aus.

Folgende Libaris werden vom NodeAQ-Controller benötigt 
- ESP8266WiFi
- ESP8266HTTPClient
- WiFiUdp.h
- NTPClient
- time.h
- DallasTemperature
- OneWire
- Wire

### NodeAQ-Controller installieren
Laden Sie die Files von der NodeAQ-Controller GitHub Page herunter und speichern diese auf Ihrem Computer ab.
Öffnen Sie die Heruntergeladenen Files mit der ArduinoIDE und passen folgende Werte an:

`//-Einstellungen für Aquarien (Bitte Anpassen)
// W-Lan Konfiguration
const char* ssid = "SSID";
const char* password = "PASSWORT!";

// AQ-Controller Werte
String aq_1_main_light_on = "12:00";
String aq_1_main_light_off = "21:00";
String aq_1_main_co2_on = "11:30";
String aq_1_main_co2_off = "20:30";
String aq_1_temp = "24";`

Sobald Sie diese angepasst haben können Sie die Software auf den NodeMCU hochladen.
Bitte beachte Sie, dass Sie die Einstellungen für das passende Board gewählt haben.
Sobald die Software hochgeladen wurde startet diese automatisch.
Zur Kontrolle können Sie den SerialMonitor öffnen, hier wird Ihnen beim Start des NodeMCUs auch die IP Adresse angezeigt.
Mit dieser können Sie die Temperatur und den Status der Steuerausgänge prüfen.

### Relays und Temperatur Sensor anschliessen
Auf folgenden Pins können Relays angesteuert werden:
* D0 Licht
* D5 Heizung
* D6 CO2 Anlage
* D7 nicht angesteuert

Der Temperatursensor(DS18B20) wird an Pin D4 angeschlossen.

### Let it Run
Nun ist der NodeAQ-Controller bereit für seinen Einsatz.


# Disclaimer
Die Ersteller der Software NodeAQ-Controller übernehmen keine Haftung für allfällige Schäden oder Verletzungen.
Beim Umgang mit Netzspannungen raten wir dazu einen Fachmann zu kontaktieren.
