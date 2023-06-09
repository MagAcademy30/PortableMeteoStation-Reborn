// 
//  PortableMeteoStation::Reborn();
//        By MG30 and Xp14Rs
//

// Библиотеки
#include <OneWire.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <DHT.h>

// Настройки
#define RESOLU 10 //  Разрешение ds18b20 от 9 до 12 бит
#define DHTTYPE DHT11 // Какой вы DHT используете? (подсказка DHT11 or DHT22)
#define LCDADDR 0x27 // Адрес I2C 2004 lcd дисплея
#define MINHUM 30 // Минимальная влажнось
#define MAXHUM 75 // Максимальная влажность
#define SKIPLOGO 0 // Пропуск лого
#define PERIOD_DISUP 500 // Период обновления экрана, меньше 500 мс не ставить
#define BACKPERIOD 5 // Период выключения подцветки, указано 5 секунд

// Пины
#define DHTPIN 17 // Пин DHT
#define DALPIN 4 // Пин ds18b20
#define BUTPIN 25 // Пин кнопки

// Инит библиотек
OneWire ow(DALPIN);
DallasTemperature dallas(&ow);
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(LCDADDR, 20, 4);

// Временные переменные для работы
float mainTemp, hum, secondTemp;
bool buttonState, backState;
unsigned long displayMillis, backMillis;

void setup() {
  Serial.begin(115200);
  pinMode(BUTPIN, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  dallas.begin();
  dallas.setResolution(RESOLU);
  dht.begin();
  if (!SKIPLOGO) {
    lcd.setCursor(0, 1);
    lcd.print("       METEOS");
    lcd.setCursor(0, 2);
    lcd.print("       Reborn");
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print(" By MG30 and Xp14Rs");
    lcd.setCursor(0, 2);
    lcd.print("         :D  ");
    delay(1000);
  }
}

void temperatureRequest() {
  dallas.requestTemperatures();
  mainTemp = dht.readTemperature();
  hum = dht.readHumidity();
  secondTemp = dallas.getTempCByIndex(0);
  buttonState = digitalRead(BUTPIN);
}

void displayUpdate() {
  if (millis() - displayMillis > PERIOD_DISUP) {
    displayMillis = millis();
    temperatureRequest();
    int curr1, curr2, curr3, need1, need2, need3;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("M [");
    for (int i = 0; i < map(mainTemp, 0, 100, 0, 10); i++) {
      if (mainTemp > 100) {
        lcd.print(" OVERFLOW ");
        need1 = 0;
        curr1 = 0;
        break;
      } else {
        need1 = 1;
        curr1 = i;
        lcd.print("#");
      }
    }
    if (need1) {
      for (int i = 0; i < 9-curr1; i++) {
        lcd.print(" ");
      }
    }
    lcd.print("]");
    lcd.print(mainTemp);
    lcd.setCursor(0, 1);
    lcd.print("O [");
    for (int i = 0; i < map(secondTemp, 0, 100, 0, 10); i++) {
      if (secondTemp > 100) {
        lcd.print(" OVERFLOW ");
        need2 = 0;
        curr2 = 0;
        break;
      } else {
        need2 = 1;
        curr2 = i;
        lcd.print("#");
      }
    }
    if (need2) {
      for (int i = 0; i < 9-curr2; i++) {
        lcd.print(" ");
      }
    }
    lcd.print("]");
    lcd.print(secondTemp);
    lcd.setCursor(0, 2);
    lcd.print("H [");
    for (int i = 0; i < map(hum, 0, 100, 0, 10); i++) {
      if (hum > 100) {
        lcd.print(" OVERFLOW ");
        need3 = 0;
        curr3 = 0;
        break;
      } else {
        need3 = 1;
        curr3 = i;
        lcd.print("#");
      }
    }
    if (need3) {
      for (int i = 0; i < 9-curr3; i++) {
        lcd.print(" ");
      }
    }
    lcd.print("]");
    lcd.print(hum);
    lcd.setCursor(0, 3);
    lcd.print("Temp diff: ");
    lcd.print(secondTemp-mainTemp);
  }
}

void loop() {
  displayUpdate();
}
