#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char *ssid     = "internet";
const char *password = "123456789";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
String weekDays[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
String months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
unsigned long period = 1000;
unsigned long last_time = 0;
unsigned long period2 = 100;
unsigned long last_time2 = 0;

LiquidCrystal_I2C lcd1(0x27, 16, 2);  // จอที่ 1
LiquidCrystal_I2C lcd2(0x26, 16, 2);  // จอที่ 2
LiquidCrystal_I2C lcd3(0x25, 16, 2);  // จอที่ 3
LiquidCrystal_I2C lcd4(0x24, 16, 2);  // จอที่ 4
LiquidCrystal_I2C lcd5(0x23, 16, 2);  // จอที่ 5

#define SwitchPin D5
#define PumpPin   D6
#define ON HIGH
#define OFF LOW
int SwitchValue;
bool check = false;
int Count = 0;
float receiveOil;
float totalOilInTank = 200;



void setup() {
  pinMode(SwitchPin, INPUT_PULLUP);
  pinMode(PumpPin, OUTPUT);
  digitalWrite(PumpPin, OFF);
  lcd1.begin();
  lcd1.backlight();
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("SSID:internet");
  lcd1.setCursor(0, 1);
  lcd1.print("PASS:123456789");

  lcd2.begin();
  lcd2.backlight();
  lcd2.clear();
  lcd2.setCursor(3, 0);
  lcd2.print("PAY : 35");
  lcd2.setCursor(0, 1);
  lcd2.print("----------------");

  lcd3.begin();
  lcd3.backlight();
  lcd3.clear();
  lcd3.setCursor(2, 0);
  lcd3.print("PRICE : 35");
  lcd3.setCursor(0, 1);
  lcd3.print("----------------");

  lcd4.begin();
  lcd4.backlight();
  lcd4.clear();
  lcd4.setCursor(2, 0);
  lcd4.print("Receive oil");
  lcd4.setCursor(0, 1);
  lcd4.print(receiveOil);


  lcd5.begin();
  lcd5.backlight();
  lcd5.clear();
  lcd5.setCursor(0, 0);
  lcd5.print("Oil in Tank");
  lcd5.setCursor(0, 1);
  lcd5.print(totalOilInTank, 2);

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(25200);
}

void loop() {

  if ( millis() - last_time > period) {
    last_time = millis();
    TimeUpdate();
    LCD1();
    /* LCD4();
      LCD5();*/
    if (check == true) {
      Count = Count + 1;
      receiveOil = receiveOil + 28.57;
      if (receiveOil >= 999) {
        receiveOil = 1000;
        totalOilInTank = totalOilInTank - 1;
      }
      digitalWrite(PumpPin, ON);
      if (Count == 35) {
        Count = 0;
        check = false;
        Serial.println("receiveOil : " + String(receiveOil));
        LCD1();
        LCD2();
        LCD3();
        LCD4();
        LCD5();
      }
    }
    if (check == false) {
      digitalWrite(PumpPin, OFF);
    }

    Serial.println("check          : " + String(check));
    Serial.println("Count          : " + String(Count));
    Serial.println("receiveOil     : " + String(receiveOil));
    Serial.println("totalOilInTank : " + String(totalOilInTank));
  }

  if ( millis() - last_time2 > period2) {
    last_time2 = millis();
    SwitchValue = digitalRead(SwitchPin);
    if (SwitchValue == 0) {
      check = !check;
      receiveOil = 0;
      Serial.println("Switch actived");
      delay(350);
    }
  }
}

void LCD1() {
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print("Time: ");
  lcd1.print(timeClient.getFormattedTime());
  lcd1.setCursor(0, 1);
  lcd1.print("Date: ");
  lcd1.print(getFormattedDate());
}
void LCD2() {
  lcd2.begin();
  lcd2.backlight();
  lcd2.clear();
  lcd2.setCursor(3, 0);
  lcd2.print("PAY : 35");
  lcd2.setCursor(0, 1);
  lcd2.print("----------------");

}
void LCD3() {
  lcd3.begin();
  lcd3.backlight();
  lcd3.clear();
  lcd3.setCursor(2, 0);
  lcd3.print("PRICE : 35");
  lcd3.setCursor(0, 1);
  lcd3.print("----------------");
}

void LCD4() {
  lcd4.begin();
  lcd4.backlight();
  lcd4.clear();
  lcd4.setCursor(2, 0);
  lcd4.print("Receive oil");
  lcd4.setCursor(0, 1);
  lcd4.print(receiveOil);
}
void LCD5() {
  lcd5.begin();
  lcd5.backlight();
  lcd5.clear();
  lcd5.setCursor(0, 0);
  lcd5.print("Oil in Tank");
  lcd5.setCursor(0, 1);
  lcd5.print(totalOilInTank, 2);
}





void TimeUpdate() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);
  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);
  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute);
  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);
  String weekDay = weekDays[timeClient.getDay()];
  Serial.print("Week Day: ");
  Serial.println(weekDay);
  struct tm *ptm = gmtime ((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  Serial.print("Month day: ");
  Serial.println(monthDay);
  int currentMonth = ptm->tm_mon + 1;
  Serial.print("Month: ");
  Serial.println(currentMonth);
  String currentMonthName = months[currentMonth - 1];
  Serial.print("Month name: ");
  Serial.println(currentMonthName);
  int currentYear = ptm->tm_year + 1900;
  Serial.print("Year: ");
  Serial.println(currentYear);
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
  Serial.print("Current date: ");
  Serial.println(currentDate);
  Serial.println("");
}
String getFormattedDate() {
  time_t rawtime = timeClient.getEpochTime();
  struct tm * ti;
  ti = localtime(&rawtime);

  String formattedDate = "";
  formattedDate += String(ti->tm_mday) + "/";
  formattedDate += String(ti->tm_mon + 1) + "/";
  formattedDate += String(ti->tm_year + 1900);

  return formattedDate;
}
