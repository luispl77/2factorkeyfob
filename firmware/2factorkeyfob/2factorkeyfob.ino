#include <ESP8266WiFi.h>
#include <RFM69LPL.h>
#include <SPI.h>
#include <RFM69LPLregisters.h>
#include <EEPROM.h>

#define CS_R 15
#define DIO2_R 4

RFM69LPL radio(CS_R, DIO2_R);

WiFiServer server(80);
String header;
String output4State = "off";
const int output4 = 4;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  radio.init();
  radio.updateSettings();
  radio.txBegin();
  radio.setTransmitPower(20, PA_MODE_PA1_PA2_20dbm, OCP_OFF);
  radio.setFrequencyMHz(433.92); //change jamming frequency here
  radio.readAllRegs();
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  EEPROM.begin(1);  //Initialize EEPROM address 0x00 for remembering signal state
  if (retrieveState() == 1){
    Serial.println("remebered active state");
    activateJammer();
  }
  else{ //give 60 s to lock the vehicle
    Serial.println("delaying for vehicle lock ...");
    for(int i; i < 60; i++){
      Serial.print('.');
      delay(1000);
    }
    Serial.println(" delay finished.");
    activateJammer();
  }

  IPAddress apIP(69, 69, 69, 69);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("iPhone de Maria", "12345678");

  Serial.print("AP started on IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    currentTime = millis();
    previousTime = currentTime;
    
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();

      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            if (header.indexOf("GET /4/on") >= 0) {
              activateJammer();
              
            } else if (header.indexOf("GET /4/off") >= 0) {
              deactivateJammer();
            }
            
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            
            if (output4State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

void activateJammer(){
  output4State = "on";
  digitalWrite(output4, HIGH);
  digitalWrite(2, LOW);

  EEPROM.write(0, 1);
  EEPROM.commit();

  Serial.println("<JAMMER ACTIVATED>");
}

void deactivateJammer(){
  output4State = "off";
  digitalWrite(output4, LOW);
  digitalWrite(2, HIGH);

  EEPROM.write(0, 0);
  EEPROM.commit();

  Serial.println("<JAMMER DEACTIVATED>");
}

byte retrieveState(){
  return EEPROM.read(0);
}
