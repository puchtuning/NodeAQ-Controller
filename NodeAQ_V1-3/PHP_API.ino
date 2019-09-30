void mysql_api(String Controller_ID,String loctime,String locdate,String aq_sen1_temp,String aq_light1_status,String aq_heater1_status,String aq_co21_status){

    HTTPClient http;

    //String mysql_api_url = "http://192.168.0.103/_api/mysql-api.php?controller_id=NM01&time=23:40&date=29.09.2019&aq_temp=25&aq_mainlight=On&aq_heater=On";
    String mysql_api_url = "http://192.168.0.103/_api/mysql-api.php?controller_id=" + Controller_ID + "&timestamp=" + loctime + "%20" + locdate + "&aq_temp=" + aq_sen1_temp + "&aq_mainlight=" + aq_light1_status + "&aq_heater=" + aq_heater1_status;

    http.begin(mysql_api_url); //HTTP
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // API wurde angesprochen
      Serial.println("MYSQL API erreicht"); 


    } else {
      Serial.println("MYSQL API nicht erreichbar");
    }

    http.end();
  
}
