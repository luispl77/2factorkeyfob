#include <RFM69LPL.h>
#include <SPI.h>
#include <RFM69LPLregisters.h>

#define CS_R 15
#define DIO2_R 4  // This pin will go HIGH when RF energy is detected
#define LED_PIN 2
#define JAM_PIN 4

RFM69LPL radio(CS_R, DIO2_R);

// Fixed configuration
const int RF_THRESHOLD = 70;        // RF detection threshold (0-255)
const int DETECTION_CYCLES = 50;     // Number of consecutive HIGH readings needed
const unsigned long JAM_DURATION = 3000;  // Jam for 3 seconds

// Runtime variables
int detectionCount = 0;
unsigned long jammingStartTime = 0;
bool isJamming = false;

void setup() {
  Serial.begin(115200);
  
  // Configure radio for receiving/detection mode
  radio.init();
  radio.updateSettings();
  radio.rxBegin();
  radio.setFixedThreshold(RF_THRESHOLD);
  radio.setBandwidth(OOK_BW_20_8);
  radio.setFrequencyMHz(433.92);
  radio.readAllRegs();

  // Setup pins
  pinMode(LED_PIN, OUTPUT);
  pinMode(JAM_PIN, OUTPUT);
  pinMode(DIO2_R, INPUT);

  // Initial state
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(JAM_PIN, LOW);

  Serial.println("Starting up - 60 second delay...");
  delay(60000);  // 60 second startup delay
  digitalWrite(LED_PIN, LOW);
  Serial.println("Device active!");
}

void loop() {
  if (!isJamming) {
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
    // Check if jamming duration is complete
    if (millis() - jammingStartTime >= JAM_DURATION) {
      Serial.println("Jamming duration complete, returning to detection mode...");
      deactivateJammer();
    }
  }
}

void activateJammer() {
  isJamming = true;
  digitalWrite(JAM_PIN, HIGH);
  digitalWrite(LED_PIN, LOW);

  // Configure for jamming
  radio.init();
  radio.updateSettings();
  radio.txBegin();
  radio.setTransmitPower(20, PA_MODE_PA1_PA2_20dbm, OCP_OFF);
  radio.setFrequencyMHz(433.92);
  radio.readAllRegs();

  Serial.println("<JAMMER ACTIVATED>");
}

void deactivateJammer() {
  isJamming = false;
  digitalWrite(JAM_PIN, LOW);
  
  // Return to detection mode
  radio.init();
  radio.updateSettings();
  radio.rxBegin();
  radio.setFixedThreshold(RF_THRESHOLD);
  radio.setBandwidth(OOK_BW_20_8);
  radio.setFrequencyMHz(433.92);
  radio.readAllRegs();

  Serial.println("<JAMMER DEACTIVATED>");
}
