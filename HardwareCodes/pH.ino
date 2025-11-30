/*
 * Caliberating pH sensor
 * Hardware: XIAO nRF52840
 * Sensor: pH Sensor on Pin A2
 * Power: 5V from Boost Converter (with Capacitor Bank for filtering)
 */

#include <Arduino.h>

// Pin Definitions
const int PH_PIN = A2;

// ADC Constants
const float ADC_REF = 3.3f;   
const int ADC_MAX = 4095;     // 12-bit resolution

// pH Calibration 
const float PH_V_NEUTRAL = 1.658f;     // Neutral Voltage (at pH 7.0)
const float PH_SENSITIVITY = 0.0736f; // Sensitivity (Volts per pH unit): (V_pH4 - V_pH7) / 3

//  Filtering Functions
float readMedianVoltage(int pin);

void setup() {
  Serial.begin(115200);
  
  // IMPORTANT: Set 12-bit resolution for Xiao nRF52840
  analogReadResolution(12);
  
  delay(1000);
  Serial.println("--- pH Sensor Test (With Median Filter) ---");
}

void loop() {
  // 1. Get Stable Voltage
  float voltage = readMedianVoltage(PH_PIN);

  // 2. Calculate pH
  // Formula: pH = 7.0 + (V_neutral - V_measured) / Sensitivity
  float phValue = 7.0 + ((PH_V_NEUTRAL - voltage) / PH_SENSITIVITY);

  // 3. Clamp logical limits
  if (phValue < 0.0) phValue = 0.0;
  if (phValue > 14.0) phValue = 14.0;

  // 4. Printing Values
  Serial.print("V: ");
  Serial.print(voltage, 3);
  Serial.print(" | pH: ");
  Serial.println(phValue, 2);

  delay(1000);
}

// --- Robust Median Filter ---
// Takes 20 samples, sorts them, and returns the average of the middle 6.
// This removes outliers (noise spikes) effectively.
float readMedianVoltage(int pin) {
  const int SAMPLES = 20;
  float readings[SAMPLES];
  
  // 1. Take Readings
  for (int i = 0; i < SAMPLES; i++) {
    readings[i] = analogRead(pin) * (ADC_REF / (float)ADC_MAX);
    delay(10); // 10ms delay to catch different parts of power ripple
  }

  // 2. Sort Readings (Bubble Sort)
  for (int i = 0; i < SAMPLES - 1; i++) {
    for (int j = 0; j < SAMPLES - i - 1; j++) {
      if (readings[j] > readings[j + 1]) {
        float temp = readings[j];
        readings[j] = readings[j + 1];
        readings[j + 1] = temp;
      }
    }
  }

  // 3. Average the Middle Values (Trimmed Mean)
  // Ignore the first 5 (lowest) and last 5 (highest) to remove spikes
  float sum = 0;
  int count = 0;
  for (int i = 5; i < 15; i++) {
    sum += readings[i];
    count++;
  }

  return sum / count;
}