#include <Arduino.h>
#include <DHT.h>

// ==========================================
// PIN DEFINITIONS
// ==========================================

// Stepper Motors (A4988 Drivers)
#define X_STEP_PIN     2
#define X_DIR_PIN      3
#define Y_STEP_PIN     4
#define Y_DIR_PIN      5

// Multiplexer CD74HC4067 Control Pins (Shared Address Lines)
#define MUX_S0         22
#define MUX_S1         23
#define MUX_S2         24
#define MUX_S3         25

// MUX Analog Signal Pins
#define MUX1_SIG_PIN   A0 // Sensors 1 to 16
#define MUX2_SIG_PIN   A1 // Sensors 17 to 25

// Actuators & Sensors
#define RELAY_PUMP_PIN 6
#define DHT_PIN        7
#define ESP32_CAM_PIN  8

#define DHTTYPE        DHT22

// ==========================================
// CONFIGURATION & CONSTANTS
// ==========================================

#define MATRIX_SIZE       5   // 5x5 Grid (25 Pots)
#define TOTAL_POTS        25
#define MOISTURE_THRESHOLD 400 // Calibrated Analog Value (Adjust based on sensor dry threshold)
#define WATERING_TIME_MS  10000 // 10 Seconds watering duration

// Stepper Motor Calibration (Steps per Grid Spacing)
const int STEPS_PER_GRID_X = 2000; // Adjust steps distance between adjacent pots on X-axis
const int STEPS_PER_GRID_Y = 2000; // Adjust steps distance between adjacent pots on Y-axis

// System State
int currentX = 0;
int currentY = 0;

DHT dht(DHT_PIN, DHTTYPE);

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================
void setMuxChannel(byte channel);
int readMoistureSensor(int potIndex);
void moveGantryTo(int targetX, int targetY);
void stepMotor(int stepPin, int dirPin, int steps);
void irrigateAndCapture(int potIndex);
void returnToHome();

// ==========================================
// SETUP
// ==========================================
void setup() {
  Serial.begin(9600);
  
  // Pin Modes - Steppers
  pinMode(X_STEP_PIN, OUTPUT);
  pinMode(X_DIR_PIN, OUTPUT);
  pinMode(Y_STEP_PIN, OUTPUT);
  pinMode(Y_DIR_PIN, OUTPUT);

  // Pin Modes - MUX
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  // Pin Modes - Actuators
  pinMode(RELAY_PUMP_PIN, OUTPUT);
  digitalWrite(RELAY_PUMP_PIN, LOW); // Pump off initially

  pinMode(ESP32_CAM_PIN, OUTPUT);
  digitalWrite(ESP32_CAM_PIN, LOW);

  // Initialize DHT Sensor
  dht.begin();

  Serial.println("==========================================");
  Serial.println("Smart Gantry System Initialized");
  Serial.println("==========================================");
}

// ==========================================
// MAIN LOOP
// ==========================================
void loop() {
  Serial.println("\n--- Starting Soil Moisture Scan Pass ---");

  // Read Ambient Environmental Data
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  Serial.print("Ambient Temp: "); Serial.print(temp); Serial.print(" C | ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");

  // Scan all 25 Pots
  for (int pot = 1; pot <= TOTAL_POTS; pot++) {
    int moistureValue = readMoistureSensor(pot);
    
    Serial.print("Pot "); Serial.print(pot);
    Serial.print(" Moisture: "); Serial.println(moistureValue);

    // Check if watering is needed
    if (moistureValue < MOISTURE_THRESHOLD) {
      Serial.print("-> Dry soil detected at Pot "); Serial.println(pot);

      // Convert 1D pot index (1..25) to 2D matrix coordinates (0..4, 0..4)
      int targetX = (pot - 1) % MATRIX_SIZE;
      int targetY = (pot - 1) / MATRIX_SIZE;

      // Move Gantry to Target Pot
      moveGantryTo(targetX, targetY);

      // Trigger Pump and ESP32-CAM Snapshot
      irrigateAndCapture(pot);
    }
  }

  // Return Gantry to Home Position (0,0) after scan cycle
  returnToHome();

  // Delay before the next complete scan cycle (e.g., 1 Hour)
  Serial.println("Scan complete. Sleeping for next routine...");
  delay(3600000); 
}

// ==========================================
// HELPER FUNCTIONS
// ==========================================

// Sets the 4-bit channel address for CD74HC4067 MUX
void setMuxChannel(byte channel) {
  digitalWrite(MUX_S0, bitRead(channel, 0));
  digitalWrite(MUX_S1, bitRead(channel, 1));
  digitalWrite(MUX_S2, bitRead(channel, 2));
  digitalWrite(MUX_S3, bitRead(channel, 3));
  delayMicroseconds(10); // Short stabilization delay
}

// Reads soil moisture value for a specific pot index (1 to 25)
int readMoistureSensor(int potIndex) {
  if (potIndex <= 16) {
    // Channel 0 to 15 on MUX 1
    setMuxChannel(potIndex - 1);
    return analogRead(MUX1_SIG_PIN);
  } else {
    // Channel 0 to 8 on MUX 2
    setMuxChannel(potIndex - 17);
    return analogRead(MUX2_SIG_PIN);
  }
}

// Controls stepper motor steps and direction
void stepMotor(int stepPin, int dirPin, int steps) {
  if (steps < 0) {
    digitalWrite(dirPin, LOW); // Move Backward/Left
    steps = -steps;
  } else {
    digitalWrite(dirPin, HIGH); // Move Forward/Right
  }

  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800); // Speed control delay
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}

// Navigates the gantry head to target (X, Y) coordinates
void moveGantryTo(int targetX, int targetY) {
  int deltaX = targetX - currentX;
  int deltaY = targetY - currentY;

  if (deltaX != 0 || deltaY != 0) {
    Serial.print("Moving Gantry to X:"); Serial.print(targetX);
    Serial.print(" Y:"); Serial.println(targetY);

    // Step X Axis
    stepMotor(X_STEP_PIN, X_DIR_PIN, deltaX * STEPS_PER_GRID_X);
    
    // Step Y Axis
    stepMotor(Y_STEP_PIN, Y_DIR_PIN, deltaY * STEPS_PER_GRID_Y);

    // Update current position trackers
    currentX = targetX;
    currentY = targetY;
  }
}

// Activates relay pump and pulses ESP32-CAM trigger line
void irrigateAndCapture(int potIndex) {
  Serial.print("Irrigating Pot "); Serial.println(potIndex);
  
  // Turn ON Pump
  digitalWrite(RELAY_PUMP_PIN, HIGH);
  delay(WATERING_TIME_MS);
  digitalWrite(RELAY_PUMP_PIN, LOW); // Turn OFF Pump

  // Pulse signal to ESP32-CAM to take picture
  Serial.println("Triggering ESP32-CAM image capture...");
  digitalWrite(ESP32_CAM_PIN, HIGH);
  delay(200); // 200ms trigger pulse
  digitalWrite(ESP32_CAM_PIN, LOW);
  
  delay(1000); // Wait for water to settle
}

// Moves Gantry back to (0, 0)
void returnToHome() {
  if (currentX != 0 || currentY != 0) {
    Serial.println("Returning to Home Position (0, 0)...");
    moveGantryTo(0, 0);
  }
}
