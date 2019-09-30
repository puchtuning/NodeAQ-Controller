void Network(){
  
  // Connect to WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Server IP Adress: ");
  Serial.print(WiFi.localIP());

  String ipaddress = WiFi.localIP().toString();

  delay(1000);
  
}

void SerialPrint(String text1, String text2){

  Serial.print(text1);
  Serial.println(text2);
  
}

void SyncTime() {

   timeClient.update();
   
  
   time_t rawtime = timeClient.getEpochTime();
   struct tm * ti;
   ti = localtime (&rawtime);

   uint16_t year = ti->tm_year + 1900;
   String yearStr = String(year);

   uint8_t month = ti->tm_mon + 1;
   String monthStr = month < 10 ? "0" + String(month) : String(month);

   uint8_t day = ti->tm_mday;
   String dayStr = day < 10 ? "0" + String(day) : String(day);

   uint8_t hours = ti->tm_hour;
   String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

   uint8_t minutes = ti->tm_min;
   String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

   uint8_t seconds = ti->tm_sec;
   String secondStr = seconds < 10 ? "0" + String(seconds) : String(seconds);

   timestamp = hoursStr + ":" + minuteStr + " " + dayStr + "." + monthStr + "." + yearStr;
   locdate = dayStr + "." + monthStr + "." + yearStr;
   loctime = hoursStr + ":" + minuteStr;
}
