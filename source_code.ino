#include <DHT.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT11
#define DHTPIN 8
#define R1 3 //pompa
#define R2 5 //kipas

int d,h,t,KT;
char buf1[20];
LiquidCrystal_I2C lcd(0x27, 16, 2); //Alamat LCD
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  Wire.begin();
  dht.begin();
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  pinMode (R1, OUTPUT);
  pinMode (R2, OUTPUT);
  digitalWrite(R1,HIGH);
  digitalWrite(R2,HIGH);
}

void loop() {
  sensorTanah();
  pompaAir();
  kipasAngin();
  tampil();
  delay(1000);
}

void sensorTanah() {
  d = analogRead(A0);
  KT = (100 - (d * 0.0977));
}

void pompaAir() {
  Serial.println();
  DateTime now = rtc.now();
  sprintf(buf1, "%02d/%02d/%02d %02d:%02d:%02d", now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  Serial.print(F("Tanggal/Jam  :  "));
  Serial.println(buf1);

  if (now.hour() == 21 && now.minute() == 49){
    if (KT <= 55) {
      digitalWrite(R1, LOW);//pompa aktif
      delay(10000);
    }
    else if (KT >= 65) {
      digitalWrite(R1, HIGH);
    }
    else{
     digitalWrite(R1, HIGH);
     delay(59000); 
    }
  }


  if (now.hour() == 16 && now.minute() == 30) {
    if (KT <= 55) {
      digitalWrite(R1, LOW);//pompa aktif
      delay(10000);
    }
    else if (KT >= 65) {
      digitalWrite(R1, HIGH);
    }
    else{
     digitalWrite(R1, HIGH);
     delay(59000); 
     }
  }
}

void kipasAngin() {
  h = dht.readHumidity();
  t = dht.readTemperature();

  if (h >= 65 || t >= 28) {
    digitalWrite(R2, LOW);  //kipas on
  }
  else if (h <= 55 || t <= 25) {
    digitalWrite(R2, HIGH);
  }
  else{
    digitalWrite(R1, HIGH); 
  }
}

void tampil() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("K. Tanah : ");
  lcd.print(KT);
  lcd.print("%");
  Serial.print("K. Tanah     :  ");
  Serial.print(KT); 
  Serial.println(" %");
  lcd.setCursor(0, 1);
  lcd.print("K. Udara : ");
  lcd.print(h);
  lcd.print("%");
  Serial.print("K. Udara     :  ");
  Serial.print(h);
  Serial.println(" %");
  delay(1500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Suhu     : ");
  lcd.print(t);
  lcd.print("C");
  Serial.print("Suhu         :  ");
  Serial.print(t);
  Serial.print(" C");
  Serial.println();
  delay(1500);
}
