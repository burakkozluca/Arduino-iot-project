
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define FIREBASE_HOST "fir-10bef-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDKL_C621WLXNjbB5RGwttmM4jluPs2Vxo"
#define WIFI_SSID "iphoneburak"
#define WIFI_PASSWORD "123456789"

FirebaseData veritabanim;

int a = 7;
int GazSensoru = A0;
int data = 0;
#define YesilLed 14 //D5
#define KirmiziLed 12 //D6
int buzzer = 0;
int flame_sensor = 2; //D4
int flame_sensor_read;

void setup()
{
  Serial.begin(115200);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Ağ Bağlantısı Oluşturuluyor");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("IP adresine bağlanıldı: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  pinMode(YesilLed, OUTPUT);
  pinMode(KirmiziLed, OUTPUT);
  pinMode(flame_sensor, INPUT);
}

void loop() 
{
  data = analogRead(GazSensoru);
  flame_sensor_read = digitalRead(flame_sensor);
  if(Firebase.setInt(veritabanim, "/firebase/Gaz", data) && 
  Firebase.setFloat(veritabanim, "/firebase/flame", flame_sensor_read))
  {
    Serial.println("veri gönderimi başarılı");
  }
  else{
    Serial.print("veri gönderilemedi, ");
    Serial.println(veritabanim.errorReason());
  }
  if(data > 120 || flame_sensor_read == 0)
  {
    digitalWrite(YesilLed, LOW);
    tone(buzzer, 2000, 200);
    digitalWrite(KirmiziLed, HIGH);
    delay(100);
    digitalWrite(KirmiziLed, LOW);
  }
  else
  {
    digitalWrite(YesilLed, HIGH);
    digitalWrite(KirmiziLed, LOW);
    noTone(buzzer);  
  }
}
