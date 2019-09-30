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
