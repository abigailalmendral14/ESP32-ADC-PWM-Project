/*
 * ESP32 ADC + PWM WITH LDR CONTROL 
 *
 * MEMBERS:
 * JOHN PATRICK MIRAM
 * PRINCE ALDEM A. FIGUEROA
 * MARY JOYCE ANN D. ISIP
 * LEA PENOLIO
 * ABIGAIL ALMENDRAL
 */

// PIN DEFINITIONS (kung saan nakakabit ang components)
#define LED_PIN 5      // LED connected sa GPIO 5
#define POT_PIN 34     // Potentiometer (analog input)
#define LDR_PIN 35     // LDR (light sensor)
#define NTC_PIN 32     // NTC (temperature sensor)

// PWM SETTINGS (for LED brightness control)
#define PWM_FREQ 5000  // frequency ng PWM signal
#define PWM_RES 8      // resolution (0–255 values)

// LDR THRESHOLD
// value kung saan natin masasabi kung dark or bright
int threshold = 2000;

void setup() {
  Serial.begin(115200); // start serial monitor

  // i-attach ang PWM sa LED pin
  ledcAttach(LED_PIN, PWM_FREQ, PWM_RES);
}

void loop() {

  // READ SENSORS (ADC)
  // kumukuha ng analog values (0–4095)
  int potValue = analogRead(POT_PIN); // basahin potentiometer
  int ldrValue = analogRead(LDR_PIN); // basahin light level
  int ntcValue = analogRead(NTC_PIN); // basahin temperature sensor

  // LDR → LED CONTROL
  // DARK → LED ON
  // BRIGHT → LED OFF

  if (ldrValue < threshold) {
    // kapag mababa ang value = madilim
    ledcWrite(LED_PIN, 255);  // full brightness (ON)
  } else {
    // kapag mataas ang value = maliwanag
    ledcWrite(LED_PIN, 0);    // OFF
  }

  // LDR → LUX (approximation lang ng light intensity)
  float ldrVoltage = ldrValue * (3.3 / 4095.0); // convert ADC to voltage
  float ldrLux = pow((3.3 - ldrVoltage) / ldrVoltage * 10000, 1.25);

  // NTC → TEMPERATURE (approximation)
  float ntcVoltage = ntcValue * (3.3 / 4095.0); // convert to voltage
  float temperature = (ntcVoltage - 0.5) * 100; // convert to °C

  // SERIAL OUTPUT (para makita sa Serial Monitor)
  Serial.print("Potentiometer: ");
  Serial.print(potValue); // show pot value

  Serial.print(" | LDR: ");
  Serial.print(ldrValue); // raw LDR value

  Serial.print(" | LDR Lux: ");
  Serial.print(ldrLux, 2); // computed light intensity

  Serial.print(" | NTC Temp: ");
  Serial.print(temperature, 2); // temperature value
  Serial.println(" °C");

  delay(300); // konting delay para readable output
}
