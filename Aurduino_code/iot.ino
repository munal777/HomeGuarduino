#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Kopila@Wifi";         
const char* password = "kopila2050@";
const char* serverUrl = "http://192.168.16.105:8000/alert/";


const int reedSwitchPin[] = {12, 13}; // GPIO12 and GPIO13 for reed switches
const int buzzerPin = 4;              // GPIO4 for buzzer (shared buzzer for all alerts)
const int MQ2_PIN = 36;               // Analog pin for MQ-2 sensor
const int RISK_THRESHOLD = 2600;
const int PIR_PIN = 27;               // GPIO27 for PIR sensor

bool reedSwitchState[] = {false, false}; // Track reed switch states
bool pirState = false;                   // Track PIR state
bool gasAlertSent = false;               // Track gas alert state
bool motionAlertSent = false;            // Track motion alert state

void setup() {
  for (int i = 0; i < 2; i++) {
    pinMode(reedSwitchPin[i], INPUT_PULLUP); // using the internal pull-up resistor
  }
  pinMode(MQ2_PIN, INPUT);              // Configure MQ-2 sensor analog pin
  pinMode(PIR_PIN, INPUT);              // Configure PIR sensor as input
  pinMode(buzzerPin, OUTPUT);           // Buzzer pin as output
  digitalWrite(buzzerPin, LOW);         // Ensure buzzer is off initially

  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  Serial.println("System initialized!");
}

void loop() {
  // Check reed switches
  for (int i = 0; i < 2; i++) {
    int switchState = digitalRead(reedSwitchPin[i]); // Read reed switch state
    if (switchState == HIGH && !reedSwitchState[i]) {
      // Reed switch triggered for the first time
      reedSwitchState[i] = true;
      Serial.print("Door is OPEN: ");
      Serial.println(i + 1);
      // Send data to backend
      postRequestToBackend("door_open", "1");
    }
    if (switchState == HIGH) {
      digitalWrite(buzzerPin, HIGH); // Keep buzzer on while door is open
    } else {
      reedSwitchState[i] = false; // Reset state when door is closed
    }
  }

  // Check gas sensor
  int gasLevel = analogRead(MQ2_PIN); // Read the analog gas level
  if (gasLevel > RISK_THRESHOLD && !gasAlertSent) {
    // Gas detected for the first time
    gasAlertSent = true;
    Serial.println("WARNING! High gas level detected. Risk signal triggered!");
    // Send data to backend
    postRequestToBackend("gas_detected", String(gasLevel));
  }
  if (gasLevel > RISK_THRESHOLD) {
    digitalWrite(buzzerPin, HIGH); // Keep buzzer on while gas level is high
  } else {
    gasAlertSent = false; // Reset state when gas level is safe
  }

  // Check PIR sensor
  int motionState = digitalRead(PIR_PIN); // Read the state of the PIR sensor
  if (motionState == HIGH && !motionAlertSent) {
    // Motion detected for the first time
    motionAlertSent = true;
    Serial.println("Motion detected inside the area!");
    // Send data to backend
    postRequestToBackend("motion_detected", "true");
  }
  if (motionState == HIGH) {
    digitalWrite(buzzerPin, HIGH); // Keep buzzer on while motion is detected
  } else {
    motionAlertSent = false; // Reset state when no motion is detected
  }


  if (gasLevel <= RISK_THRESHOLD && digitalRead(PIR_PIN) == LOW && digitalRead(reedSwitchPin[0]) == LOW && digitalRead(reedSwitchPin[1]) == LOW) {
    digitalWrite(buzzerPin, LOW);
  }

  // Serial.println(gasLevel);
  delay(100); 
}

void postRequestToBackend(String alertType, String alertData) {
  HTTPClient http;
  String url = serverUrl; // Construct URL based on alert type
  
  http.begin(url);                    // Begin HTTP request to the server
  http.addHeader("Content-Type", "application/json"); // Set Content-Type header for JSON
  
  String jsonData = "{";

    // Check alert type and add corresponding data
  if (alertType == "door_open") {
    jsonData += "\"door_open\":" + alertData;
  } 
  if (alertType == "motion_detected") {
    if (jsonData.length() > 1) jsonData += ",";
    jsonData += "\"motion_detected\":" + alertData;
  }
  if (alertType == "gas_detected") {
    if (jsonData.length() > 1) jsonData += ",";
    jsonData += "\"gas_detected\":" + alertData;
  }

  jsonData += "}"; 

  // Send POST request
  int httpCode = http.POST(jsonData);
  
  // Handle response
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.print("POST Response Code: ");
    Serial.println(httpCode);
    Serial.print("Response from server: ");
    Serial.println(payload);
  } else {
    Serial.print("Error in sending POST request: ");
    Serial.println(httpCode);
  }
  
  http.end();  // Close HTTP connection
}