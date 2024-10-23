#include <ESP8266WiFi.h>
#include <RFM69LPL.h>
#include <SPI.h>
#include <RFM69LPLregisters.h>
#include <EEPROM.h>

#define CS_R 15
#define DIO2_R 4  // This pin will go HIGH when RF energy is detected

RFM69LPL radio(CS_R, DIO2_R);

WiFiServer server(80);
String header;
String output4State = "off";
const int output4 = 4;
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

// Configuration variables
int RF_THRESHOLD = 100;  // RF detection threshold (0-255)
int DETECTION_CYCLES = 50;  // Number of consecutive HIGH readings needed
int detectionCount = 0;  // Counter for consecutive detections

// Add new timing variables at the top
unsigned long JAMMING_DURATION = 3000;  // Jam for 3 seconds
unsigned long jammingStartTime = 0;

// Add device state variable
bool deviceEnabled = false;  // Still starts false but will become true after delay
unsigned long startupTime;   // To track startup delay
const unsigned long STARTUP_DELAY = 60000;  // 60 seconds in milliseconds
bool startupComplete = false;

// Modify WiFi configuration
const char* ssid     = "RF_Detector";
const char* password = "12345678";

// Use standard IP address
IPAddress local_ip(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

// EEPROM address for storing delay
#define DELAY_ADDR 0
unsigned long startupDelay = 60000;  // Default 60 seconds, but will load from EEPROM

void setup() {
  Serial.begin(115200);
  
  // Initialize EEPROM
  EEPROM.begin(512);
  
  // Load startup delay from EEPROM
  unsigned long savedDelay = 0;
  EEPROM.get(DELAY_ADDR, savedDelay);
  
  // Validate and use saved delay, or use default if invalid
  if (savedDelay >= 0 && savedDelay <= 300000) {  // Max 5 minutes
  }
  
  // Configure radio for receiving/detection mode
  radio.init();
  radio.updateSettings();
  radio.rxBegin();
  radio.setFixedThreshold(RF_THRESHOLD);  // Using configurable threshold
  radio.setBandwidth(OOK_BW_20_8);
  radio.setFrequencyMHz(433.92);
  radio.readAllRegs();

  // Setup LED indicator
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Setup DIO2 as input for detection
  pinMode(DIO2_R, INPUT);

  // Configure soft AP
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  Serial.println("WiFi AP Started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.begin();

  Serial.println("Starting up - " + String(startupDelay/1000) + " second delay...");
  digitalWrite(2, HIGH);  // LED on during delay
  delay(startupDelay);
  digitalWrite(2, LOW);   // LED off after delay
  Serial.println("Delay complete - Device active!");
  
  deviceEnabled = true;  // Start enabled after delay
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    header = "";  // Clear header at the start of new connection
    currentTime = millis();
    previousTime = currentTime;

    // Add proper response headers and content type
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // Tell the client we'll close after sending
            client.println();
            
            // Consolidate all form handling
            if (header.indexOf("GET /update") >= 0) {
              // Extract threshold value
              int threshIndex = header.indexOf("threshold=");
              if (threshIndex >= 0) {
                String threshStr = header.substring(threshIndex + 9);
                threshStr = threshStr.substring(0, threshStr.indexOf("&"));
                RF_THRESHOLD = threshStr.toInt();
                RF_THRESHOLD = constrain(RF_THRESHOLD, 0, 255);
                
                // Immediately apply new threshold to radio
                radio.setFixedThreshold(RF_THRESHOLD);
                radio.readAllRegs();
                Serial.println("New threshold set: " + String(RF_THRESHOLD));
              }
              
              // Extract cycles value
              int cyclesIndex = header.indexOf("cycles=");
              if (cyclesIndex >= 0) {
                String cyclesStr = header.substring(cyclesIndex + 7);
                cyclesStr = cyclesStr.substring(0, cyclesStr.indexOf("&"));
                DETECTION_CYCLES = cyclesStr.toInt();
                DETECTION_CYCLES = constrain(DETECTION_CYCLES, 1, 100);
                Serial.println("New cycles set: " + String(DETECTION_CYCLES));
              }
              
              // Extract duration value
              int durationIndex = header.indexOf("duration=");
              if (durationIndex >= 0) {
                String durStr = header.substring(durationIndex + 9);
                durStr = durStr.substring(0, durStr.indexOf("&"));
                JAMMING_DURATION = durStr.toInt();
                JAMMING_DURATION = constrain(JAMMING_DURATION, 1000, 10000);
                Serial.println("New duration set: " + String(JAMMING_DURATION));
              }
              
              // Add handler for delay update
              int delayIndex = header.indexOf("startup_delay=");
              if (delayIndex >= 0) {
                String delayStr = header.substring(delayIndex + 13);
                delayStr = delayStr.substring(0, delayStr.indexOf("&"));
                unsigned long newDelay = delayStr.toInt() * 1000; // Convert seconds to milliseconds
                
                // Validate and save new delay
                if (newDelay >= 0 && newDelay <= 300000) {  // Max 5 minutes
                  startupDelay = newDelay;
                  EEPROM.put(DELAY_ADDR, startupDelay);
                  EEPROM.commit();
                  Serial.println("New startup delay saved: " + String(startupDelay/1000) + " seconds");
                }
              }
            }

            // Handle enable/disable
            if (header.indexOf("GET /4/on") >= 0) {
              deviceEnabled = true;
              digitalWrite(2, LOW);
              Serial.println("Device enabled");
            } 
            else if (header.indexOf("GET /4/off") >= 0) {
              deviceEnabled = false;
              deactivateJammer();
              digitalWrite(2, HIGH);
              Serial.println("Device disabled");
            }

            // Device control buttons
            client.println("<body><h1>RF Detector Control Panel</h1>");
            client.println("<div class='controls'>");
            client.println("<p><a href=\"/4/on\"><button class=\"button button-on\">ENABLE DEVICE</button></a>");
            client.println("<a href=\"/4/off\"><button class=\"button button-off\">DISABLE DEVICE</button></a></p>");
            client.println("</div>");

            // Settings form
            client.println("<div class='controls'>");
            client.println("<form action='/update' method='get'>");
            client.println("<p>RF Threshold (0-255): <input type='number' name='threshold' min='0' max='255' value='" + String(RF_THRESHOLD) + "'></p>");
            client.println("<p>Detection Cycles (1-100): <input type='number' name='cycles' min='1' max='100' value='" + String(DETECTION_CYCLES) + "'></p>");
            client.println("<p>Jamming Duration (ms): <input type='number' name='duration' min='1000' max='10000' step='100' value='" + String(JAMMING_DURATION) + "'></p>");
            client.println("<p>Startup Delay (0-300 seconds): <input type='number' name='startup_delay' min='0' max='300' value='" + String(startupDelay/1000) + "'></p>");
            client.println("<input type='submit' class='button' value='Update Settings'>");
            client.println("</form>");
            client.println("</div>");
            
            // Status display
            client.println("<div class='controls'>");
            client.println("<p>Device Status: <strong>" + String(deviceEnabled ? "ENABLED" : "DISABLED") + "</strong></p>");
            client.println("<p>Current Settings:</p>");
            client.println("<p>Threshold: " + String(RF_THRESHOLD) + "</p>");
            client.println("<p>Detection Cycles: " + String(DETECTION_CYCLES) + "</p>");
            client.println("<p>Jamming Duration: " + String(JAMMING_DURATION) + "ms</p>");
            client.println("<p>Startup Delay: " + String(startupDelay/1000) + " seconds</p>");
            client.println("</div>");
            
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
    
    // Clean up
    delay(1);      // Give the web browser time to receive the data
    client.stop(); // Close the connection
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  // Only run detection/jamming if enabled
  if (deviceEnabled) {
    if (output4State == "off") {
      // Detection mode
      if (digitalRead(DIO2_R) == HIGH) {
        detectionCount++;
        if (detectionCount >= DETECTION_CYCLES) {
          Serial.println("RF Signal detected! Activating jammer...");
          activateJammer();
          jammingStartTime = millis();
          detectionCount = 0;
        }
      } else {
        detectionCount = 0;
      }
    } else {
      // Jamming mode - check if it's time to stop
      if (millis() - jammingStartTime >= JAMMING_DURATION) {
        Serial.println("Jamming duration complete, returning to detection mode...");
        deactivateJammer();
      }
    }
  }
}

void activateJammer() {
  output4State = "on";
  digitalWrite(output4, HIGH);
  digitalWrite(2, LOW);

  // Reconfigure radio for jamming
  radio.init();
  radio.updateSettings();
  radio.txBegin();
  radio.setTransmitPower(20, PA_MODE_PA1_PA2_20dbm, OCP_OFF);
  radio.setFrequencyMHz(433.92);
  radio.readAllRegs();

  Serial.println("<JAMMER ACTIVATED>");
}

void deactivateJammer() {
  output4State = "off";
  digitalWrite(output4, LOW);
  
  // Only reconfigure radio if device is still enabled
  if (deviceEnabled) {
    // Reconfigure radio back to detection mode
    radio.init();
    radio.updateSettings();
    radio.rxBegin();
    radio.setFixedThreshold(RF_THRESHOLD);
    radio.setBandwidth(OOK_BW_20_8);
    radio.setFrequencyMHz(433.92);
    radio.readAllRegs();
  }

  Serial.println("<JAMMER DEACTIVATED>");
}
