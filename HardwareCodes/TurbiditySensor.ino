
/*
 * Turbidity Caliberation
 * Hardware: XIAO nRF52840, pH (A0), Turbidity (A1)
 * Wiring: Boost Converter (5V) -> Capacitor Bank -> Sensor
 * Turbidity Protection: 2.2k resistor (top) +  4.7k resistor (bottom)
 */
#include <Adafruit_TinyUSB.h>
const int TURB_PIN = A1; // XIAO analog pin
const float ADC_REF = 3.3f;   // ADC reference (XIAO)
const int ADC_MAX = 4095;     // 12-bit ADC on nRF52840

// Voltage divider: R1 = 2.2k (top), R2 = 4.7k (bottom)
const float R1 = 2200.0f;
const float R2 = 4700.0f;
const float DIV_RATIO = R2 / (R1 + R2); // ~0.681159

// Calibration (use your measured values if they change)
const float V_CLEAN = 1.160f; // measured probe voltage for "clean"
const float V_DIRTY = 0.002f; // measured probe voltage for "dirty"
const float K = 1000.0f / (V_CLEAN - V_DIRTY); // ~953

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("Turbidity Sensor readings...");
}

void loop() {
  int raw = analogRead(TURB_PIN);
  float v_adc = raw * (ADC_REF / (float)ADC_MAX);     // voltage at ADC node (after divider)
  float probeV = v_adc / DIV_RATIO;                   // actual probe voltage (before divider)

  // lower probeV -> higher NTU
  float ntu = K * (V_CLEAN - probeV);

  // beyond limit cases
  if (ntu < 0.0f) ntu = 0.0f;
  if (ntu > 1000.0f) ntu = 1000.0f;

  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print("  V_adc: ");
  Serial.print(v_adc, 3);
  Serial.print(" V  ProbeV: ");
  Serial.print(probeV, 3);
  Serial.print(" V  NTU: ");
  Serial.println(ntu, 1);

  delay(500);
}
