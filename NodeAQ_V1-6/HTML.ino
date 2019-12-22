void HTML(String aq_sen1_temp,String aq_light1_status,String aq_heater1_status,String aq_co21_status,String AQNAME){

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
  client.println("<title>NodeAQ</title>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<meta content=\"text/html; charset=utf-8\" http-equiv=\"content-type\" />");
  //client.println("<meta http-equiv=\"refresh\" content=\"60\" />");


  client.println("<style>");
  client.println("body {background-color: rgb(201, 201, 201); margin-top: 0%; margin-left: 0%; margin-right: 0%; padding-top: 0%; padding-left: 0%;}");
  client.println("div.header {background-color: rgb(100, 100, 100); width: 100%; height: 10%; display: table; position: relative; top: 0; margin: 0 auto; text-align: left; padding: 2%;}");
  client.println("div.main {background-color: rgb(201, 201, 201); width: 100%; height: auto; display: table; margin: 0 auto; padding-bottom: 3%;}");
  client.println("div.aquarium {background-color: rgb(104, 113, 236); width: 20%; height: 20; margin-top: 3%; margin-left: 3%; padding: 2%; float: left; text-align: center;}");
  client.println("h1 {color: white; font-family: 'Roboto', sans-serif; margin: 0%;} ");
  client.println("h2 {color: white; font-family: 'Roboto', sans-serif; margin: 0%;} ");
  client.println("h3 {color: white; font-family: 'Roboto', sans-serif; margin: 0%;} ");
  client.println("p {color: white; font-size: 16px;}");
  client.println(".button {background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px;}");
  client.println("</style>");


  client.println("</head>");
  client.println("<body>");
    client.println("<div class=\"header\">");
      client.println("<h1>" + AQNAME + " - Web Interface</h1>");
    client.println("</div>");
    
    client.println("<div class=\"main\">");
      client.println("<div class=\"aquarium\">");
        client.println("<h2>Temperatur</h2>");
        client.print("<h3>");
        client.print(aq_sen1_temp);
        client.print(" °C</h3>");
        client.print("<h3> Heizung: ");
        client.print(aq_heater1_status);
        client.print("</h3>");
      client.println("</div>");

      client.println("<div class=\"aquarium\">");
        client.println("<h2>Licht</h2>");
        client.print("<h3>");
        client.print(aq_light1_status);
        client.print("</h3>");
      client.println("</div>");

      client.println("<div class=\"aquarium\">");
        client.println("<h2>CO2 Anlage</h2>");
        client.print("<h3>");
        client.print(aq_co21_status);
        client.print("</h3>");
      client.println("</div>");

      client.println("<div class=\"aquarium\">");
        client.println("<h2>Letzter Zugriff</h2>");
        client.print("<h3>");
        client.print(timestamp);
        client.print("</h3>");
      client.println("</div>");
    client.println("</div>");
  
  
  client.println("</html>");
 
  delay(10);
  
}
