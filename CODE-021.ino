//#VTM https://www.youtube.com/c/VTMVlogVoThongMinh
#include <Wire.h>
#include <LiquidCrystal_I2C.h>       //https://drive.google.com/file/d/1lpJyuODmZXfi7wwBM2B79BqBIBmBfanS/view?usp=sharing
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Sim800l.h>                 //https://github.com/cristiansteib/Sim800l.git
#include <SoftwareSerial.h>
SoftwareSerial sim(10, 11);
int _timeout;
String _buffer;
#define buzzer 8
#define ir 9
Sim800l Sim800l;
String textSms;
String number = "+84992837465";      //Các bạn thay số điện thoại để module gửi SMS và call về
uint8_t index1;
int demSms = 0;
//...........................................................
void setup() {
  Serial.begin(9600);
  _buffer.reserve(50);
  sim.begin(9600);
  Sim800l.begin();
  lcd.init();
  lcd.backlight();
  pinMode(buzzer, OUTPUT);
  pinMode(ir, INPUT);
  lcd.setCursor(5, 0);
  lcd.print("WELCOME");
  chaychu();
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   ANTI THEFT   ");
  lcd.setCursor(0, 1);
  lcd.print("Status:");
  Sim800l.delAllSms();
  Serial.println("<<Connected>>");
}
//...........................................................
void loop() {
  if (digitalRead(ir) == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Status: ");
    lcd.setCursor(8, 1);
    lcd.print("NORMAL  ");
    Serial.println("normal");
  }
  if (digitalRead(ir) == 0)
  {
    lcd.setCursor(8, 1);
    lcd.print("WARNING ");
    digitalWrite(buzzer, HIGH);
    Serial.println("WARNING");
    Serial.println(number);
    Serial.println("send meggase");
    lcd.setCursor(0, 1);
    Serial.println("Calling to phone");
    lcd.print("Calling to phone");
    callNumberr();
    lcd.setCursor(0, 1);
    lcd.print("Sending message ");
    SendMessage();
    lcd.setCursor(0, 1);
    lcd.print("Send message    ");
    while (1) {
      textSms = Sim800l.readSms(1);
      Serial.println("Ready");
      if (textSms.indexOf("OFF") != -1)
      {
        lcd.setCursor(0, 1);
        lcd.print("Turn OFF BUZZER ");
        digitalWrite(buzzer, LOW);
        Serial.println("off");
        Sim800l.delAllSms();
        Serial.println("delete");
        goto endd;
      }
    }
  }
endd:;
}
//...........................................................
void SendMessage()
{
  Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(1000);
  String SMS = "WARNING";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(5000);
}
//......................................................
void callNumberr() {
  sim.println("AT");
  updateSerial();
  sim.println("ATD+ " + number + ';');
  updateSerial();
  Serial.println("calling");
  delay(20000); // wait for 20 seconds...
  sim.println("ATH");
  updateSerial();
}
void updateSerial()
{
  delay(500);
  while (Serial.available())
  {
    sim.write(Serial.read());
  }
  while (sim.available())
  {
    Serial.write(sim.read());
  }
}
//...........................................................
void chaychu()
{
  for (int positionCounter = 0; positionCounter < 13; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(150);
  }
  for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    lcd.scrollDisplayRight();
    delay(150);
  }
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(150);
  }
}
