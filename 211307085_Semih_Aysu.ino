
/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6ZrouHPyR"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "PEJ5z6MSfrWSKw5hjK5Kf5UsD0TvkD6J"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "SUPERONLINE-WiFi_6964";
char pass[] = "LUKJVKLC3TJM";


int c;


#define DHTPIN 14      // What digital pin we're connected to


// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN  21  /*Slave Select Pin*/
#define RST_PIN 22  /*Reset Pin for RC522*/
#define LED_G   12   /*Pin 8 for LED*/
MFRC522 mfrc522(SS_PIN, RST_PIN);



#define FAN 5
float f;
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
   f=t;
float k = analogRead(34);
float m = analogRead(35);

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, m);
  Blynk.virtualWrite(V1, k);
  Blynk.virtualWrite(V2, h);
  Blynk.virtualWrite(V4, t);
  Blynk.virtualWrite(V3, c);
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  dht.begin();

  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);

SPI.begin();          /*SPI communication initialized*/
  mfrc522.PCD_Init();   /*RFID sensor initialized*/
  pinMode(LED_G, OUTPUT);  /*LED Pin set as output*/
  Serial.println("Put your card to the reader...");
  Serial.println();

  pinMode(FAN, OUTPUT);
}

void loop()
{
  /*Look for the RFID Card*/
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  /*Select Card*/
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  /*Show UID for Card/Tag on serial monitor*/
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase(); 
  /* 23 F5 72 33   73 65 DF A5   03 0F 57 A8*/
  if (content.substring(1) == "73 65 DF A5" || content.substring(1) == "23 F5 72 33" || content.substring(1) == "03 0F 57 A8" ) /*UID for the Card/Tag we want to give access Replace with your card UID*/
  {
    if (content.substring(1) == "73 65 DF A5"){  c=122;}
  if (content.substring(1) == "23 F5 72 33"){  c=46;}
  if (content.substring(1) == "03 0F 57 A8"){  c=205;}
    Serial.println("Authorized access");  /*Print message if UID match with the database*/
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);  /*Led Turn ON*/
    delay(2500);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Access denied"); /*If UID do not match print message*/
  }
        /*digitalWrite(FAN, 250);*/

if(f >= 18 ){

 /*digitalWrite(FAN, HIGH); 
    delay(10000);
    digitalWrite(FAN, LOW);*/
    digitalWrite(FAN, 250);
}

    Blynk.run();
   timer.run();
}