void HTML(String aq_sen1_temp,String aq_light1_status,String aq_heater1_status,String aq_co21_status){

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");

 
  // Read the first line of the request
  String request = client.readStringUntil('\r');

  Serial.println(request.substring(6,27));
  client.flush();
 
  


 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<title>Aquarium Controller</title>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<meta content=\"text/html; charset=utf-8\" http-equiv=\"content-type\" />");
  //client.println("<meta http-equiv=\"refresh\" content=\"60\" />");
  client.println("<style>");
  client.println("body {background-color: #666666;}");
  client.println("h1 {color: white; font-size: 30px;}");
  client.println("p {color: white; font-size: 16px;}");
  client.println(".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px;}");
  client.println("</style>");
  

  client.println("</head>");
 
  

  client.println("<br><br>");

  client.print("<h1> Aquarium </h1>");
  client.print("<p>Temperatur: ");
  client.print(aq_sen1_temp);
  client.print(" °C</p>");
  client.print("<p>Licht: ");
  client.print(aq_light1_status);
  client.print("</p>");
  client.print("<p>Heizung: ");
  client.print(aq_heater1_status);
  client.print("</p>");
  client.print("<p>CO2 Anlage: ");
  client.print(aq_co21_status);
  client.print("</p>");

  client.println("<br><br>");




  
  client.println("</html>");
 
  delay(10);
  
}
