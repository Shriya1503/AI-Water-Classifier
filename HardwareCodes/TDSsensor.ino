/*
 * Standalone TDS Sensor Reader (Powered by GPIO D7)
 * Board: Seeed Studio Xiao BLE
 * Power: Pin D7 (Controlled by code) -> Connected to Sensor VCC
 * Signal: Pin A0
 */

#include <Arduino.h>

// Pin Connection
#define TDS_PIN         A0
#define TDS_POWER_PIN   7  // We use this pin to power the sensor

// Constants
const float ADC_REF = 3.3f;      // Reference voltage (Xiao BLE GPIO is 3.3V)
const int ADC_MAX = 4095;        // 12-bit ADC resolution

// Calibration
// Start with 1.0. If readings are double what they should be, set to 0.5. Change accordingly.
float kValue = 0.55; 

// Temperature (Fixed at 25C)
float temperature = 25.0;

void setup() {
  Serial.begin(115200);
  
  // Set ADC resolution to 12-bit (0-4095)
  analogReadResolution(12);
  
  // Configure the power pin
  pinMode(TDS_POWER_PIN, OUTPUT);
  digitalWrite(TDS_POWER_PIN, LOW); // Start with sensor OFF

  delay(2000);
  Serial.println("--- TDS Sensor Test Started ---");
}

void loop() {
  // 1. Turn ON the Sensor
  digitalWrite(TDS_POWER_PIN, HIGH);
  delay(100); // Wait 100ms for the sensor to stabilize/warm up

  // 2. Take multiple samples for stability
  long analogSum = 0;
  int sampleCount = 30;
  
  for (int i = 0; i < sampleCount; i++) {
    analogSum += analogRead(TDS_PIN);
    delay(10); 
  }

  // 3. Turn OFF the Sensor (Prevents interference and corrosion)
  digitalWrite(TDS_POWER_PIN, LOW);
  
  // 4. Calculate Average Voltage
  float averageAdc = analogSum / (float)sampleCount;
  float voltage = averageAdc * (ADC_REF / ADC_MAX);
  
  // 5. Temperature Compensation Formula
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
  float compensationVolatge = voltage / compensationCoefficient;

  // 6. Convert Voltage to TDS (ppm)
  // Standard Gravity TDS Formula
  float tdsValue = (133.42 * pow(compensationVolatge, 3) - 255.86 * pow(compensationVolatge, 2) + 857.39 * compensationVolatge) * 0.5 * kValue;

  // 7. Print Results
  Serial.print("Raw ADC: ");
  Serial.print((int)averageAdc);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print("V | TDS Value: ");
  Serial.print(tdsValue, 0);
  Serial.println(" ppm");

  delay(2000); // Wait 2 seconds before next reading
}