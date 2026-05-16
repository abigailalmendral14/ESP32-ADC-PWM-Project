#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ================= LCD =================
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= DHT22 =================
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ================= BUTTON =================
#define BUTTON_PIN 18

int page = 0;

// Button control
bool lastButtonState = HIGH;
unsigned long pressTime = 0;
bool longPressDone = false;

// ================= MEMBERS =================
String members[] = {
  "Prince Aldem",
  "Mary Joyce",
  "Lea Penolio",
  "Abigail Almendral",
  "John Patrick"
};

int memberIndex = 0;
unsigned long lastSwitch = 0;

// ================= SETUP =================
void setup() {
  Wire.begin(21, 22);

  lcd.init();
  lcd.backlight();

  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Startup message
  lcd.setCursor(0, 0);
  lcd.print("MAct3");
  lcd.setCursor(0, 1);
  lcd.print("Embedded Systems");
  delay(3000);

  lcd.clear();
}

// ================= LOOP =================
void loop() {
  handleButton();
  showPage();
}

// ================= BUTTON LOGIC =================
void handleButton() {
  bool state = digitalRead(BUTTON_PIN);

  // button pressed
  if (state == LOW && lastButtonState == HIGH) {
    pressTime = millis();
    longPressDone = false;
  }

  // long press (2 seconds)
  if (state == LOW && (millis() - pressTime >= 2000) && !longPressDone) {
    page = 0;  // reset to Temperature page
    longPressDone = true;
    lcd.clear();
  }

  // button released
  if (state == HIGH && lastButtonState == LOW) {
    if (!longPressDone) {
      page++;
      if (page > 2) page = 0;  // 3 pages only
      lcd.clear();
    }
  }

  lastButtonState = state;
}

// ================= DISPLAY =================
void showPage() {

  // PAGE 0: TEMPERATURE
  if (page == 0) {
    float temp = dht.readTemperature();

    lcd.setCursor(0, 0);
    lcd.print("Temperature:");

    lcd.setCursor(0, 1);

    if (isnan(temp)) {
      lcd.print("Sensor Error");
    } else {
      lcd.print(temp);
      lcd.print(" C");
    }
  }

  // PAGE 1: HUMIDITY
  else if (page == 1) {
    float hum = dht.readHumidity();

    lcd.setCursor(0, 0);
    lcd.print("Humidity:");

    lcd.setCursor(0, 1);

    if (isnan(hum)) {
      lcd.print("Sensor Error");
    } else {
      lcd.print(hum);
      lcd.print(" %");
    }
  }

  // PAGE 2: MEMBERS
  else if (page == 2) {

    if (millis() - lastSwitch >= 2000) {
      lastSwitch = millis();
      memberIndex++;
      if (memberIndex >= 5) memberIndex = 0;
    }

    lcd.setCursor(0, 0);
    lcd.print("Group Members:");

    lcd.setCursor(0, 1);
    lcd.print("                "); // clear line
    lcd.setCursor(0, 1);
    lcd.print(members[memberIndex]);
  }
}
