#define BLYNK_TEMPLATE_ID "TMPL3tJzgdoX0"
#define BLYNK_TEMPLATE_NAME "ESDProject"
#define BLYNK_AUTH_TOKEN "MNRaRbC9v2dRyx2LnMwTFLCh1GFRlsdj"

// Include the library files
#include <ESP8266WiFi.h>    // NodeMCU Library
#include <BlynkSimpleEsp8266.h>     // Blynk App Library
#include <DHT.h>      // DHT Sensor Library
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

// DHT Sensor Configrations
#define DHTPIN D2        
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

// Other Sensor Configrations
#define WATER D1  
#define SOILMOISTURE A0
#define BUZZER D6
#define LED D0

// 1.8 TFT LCD Configrations
#define TFT_CS  D8
#define TFT_RST  D4                                            
#define TFT_DC  D3  
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);  

// Variables
float h;
float t;
int s;
int w;
int score = 1000;
int rank = 3132;
int streak = 1;
String status;

// WiFi Credentials
char auth[] = BLYNK_AUTH_TOKEN;   //Enter your Blynk auth token
char ssid[] = "Abhishek";  //Enter your WIFI name
char pass[] = "#abhishek1803";   //Enter your WIFI passowrd

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
}

void loop() {
  Blynk.run();    // Run the blynk function
  digitalWrite(LED,LOW);

  // Read Sensor Data
  h = dht.readHumidity();
  t = dht.readTemperature();
  s = map(analogRead(SOILMOISTURE),1024,0,0,100);
  w = digitalRead(WATER);
  if (w == 0){
    status = "Yes";
    Blynk.logEvent("tank_alert");
    score -= 50;
    digitalWrite(LED,HIGH);
    digitalWrite(BUZZER,HIGH);
    tft.setCursor(0, 30);
    tft.fillScreen(ST77XX_RED);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(4);
    tft.println(": (");
    tft.setTextSize(2);
    tft.println(" ");
    tft.println("Tank");
    tft.println("is");
    tft.println("Full!");
    delay(3000);
    digitalWrite(LED,LOW);
    digitalWrite(BUZZER,LOW);
  }
  else {
    status = "No";
  }

  if (s <= 20 || s >= 80){
    Blynk.logEvent("humidity_alert");
    score -= 10;
    digitalWrite(LED,HIGH);
    digitalWrite(BUZZER,HIGH);
    delay(200);
    digitalWrite(BUZZER,LOW);
    tft.setCursor(0, 30);
    tft.fillScreen(ST77XX_RED);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(4);
    tft.println(": (");
    tft.setTextSize(2);
    tft.println(" ");
    tft.println("Something");
    tft.println("is");
    tft.println("Wrong!");
    delay(1500);
  }
  else{
    score += 1;
    tft.setCursor(0, 30);
    tft.fillScreen(ST77XX_GREEN);
    tft.setTextColor(ST77XX_BLACK);
    tft.setTextSize(4);
    tft.println(": )");
    tft.setTextSize(2);
    tft.println(" ");
    tft.println("All");
    tft.println("Good!");
    delay(1500);
  }
  
  // Write data to cloud
  Blynk.virtualWrite(0,s);
  Blynk.virtualWrite(1,h);
  Blynk.virtualWrite(2,t);
  Blynk.virtualWrite(3,status);
  Blynk.virtualWrite(4,score);
  Blynk.virtualWrite(5,rank);
  Blynk.virtualWrite(6,streak);

  // Serial Communication 
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.print(" | Humidity : ");
  Serial.print(h);
  Serial.print(" | Tank Full? : ");
  Serial.print(w);
  Serial.print(" | Soil Moisture Level : ");
  Serial.println(s);

  if(t > 25){
    Blynk.logEvent("temp_alert");
  }
  delay(500);
  tftPrintTest();
}

void tftPrintTest() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_YELLOW);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.println("Plant: ");
  tft.println("Atmosphere: ");
  tft.println("Temperature: ");
  tft.println(" ");
  tft.println(t);
  delay(1000);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.println("Plant: ");
  tft.println("Atmosphere: ");
  tft.println("Humidity : ");
  tft.println(" ");
  tft.println(h);
  delay(1000);
  tft.setCursor(0, 30);
  tft.fillScreen(ST77XX_WHITE);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Plant");
  tft.println("Soil");
  tft.println("Moisture");
  tft.println("Level : ");
  tft.println(" ");
  tft.println(s);
  delay(1000);
  tft.setCursor(0, 30);
  tft.fillScreen(ST77XX_RED);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.println("Is");
  tft.println("Water");
  tft.println("Tank");
  tft.println("Full? ");
  tft.println(" ");
  tft.println(status);
  delay(1000);
  tft.setCursor(0, 30);
  tft.fillScreen(ST77XX_GREEN);
  tft.setTextColor(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.println("Current");
  tft.println("Score :");
  tft.println(" ");
  tft.println(score);
  delay(1500);
  tft.setCursor(0, 30);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_BLUE);
  tft.setTextSize(2);
  tft.println("Current");
  tft.println("Rank :");
  tft.println(" ");
  tft.println(rank);
  delay(1500);
  tft.setCursor(0, 30);
  tft.fillScreen(ST77XX_YELLOW);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(2);
  tft.println("Streak");
  tft.println(streak);
  tft.println("Days ; )");
  delay(1000);
}