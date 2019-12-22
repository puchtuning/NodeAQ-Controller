void ConnectMQTT(){
  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
}

void SendMQTT(float aq_sen1_temp,String aq_light1_status,String aq_heater1_status,String aq_co21_status){
    // call poll() regularly to allow the library to send MQTT keep alives which
  // avoids being disconnected by the broker
  mqttClient.poll();

  // avoid having delays in loop, we'll use the strategy from BlinkWithoutDelay
  // see: File -> Examples -> 02.Digital -> BlinkWithoutDelay for more info
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    

    String aq_mainlight_status = "An";



    const size_t capacity = JSON_OBJECT_SIZE(7);
    DynamicJsonDocument doc(capacity);

    doc["aqmainlight_status"] = aq_light1_status;
    doc["aqheater_status"] = aq_heater1_status;
    doc["aqco2_status"] = aq_co21_status;
    doc["timestamp"] = timestamp;
    doc["temp"] = aq_sen1_temp;
    

    
    



    Serial.print("Sending message to topic: ");
    Serial.println(topic);
    
    serializeJsonPretty(doc, Serial);
    
    String MQTTMSG = doc.as<String>();
    

    // send message, the Print interface can be used to set the message contents
    mqttClient.beginMessage(topic);
    mqttClient.print(MQTTMSG);
    mqttClient.endMessage();

    Serial.println();

  }
}
