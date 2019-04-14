// include library, include base class, make path known
#include <GxEPD.h>
#include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <WiFi.h>
#include <ArduinoJson.h>
#include <SPI.h>

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

//#include GxEPD_BitmapExamples

// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\espressif\esp32\variants\lolin32\pins_arduino.h
GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

#define DEMO_DELAY 20

const unsigned char ST12Logo[350] = {
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X7F,0X00,0X00,0X00,0X00,0XF0,0X03,0XEF,0X80,0X00,0X00,0X00,0X3E,
0X06,0X03,0X80,0X00,0X00,0X00,0X0F,0XF0,0X01,0X80,0X00,0X00,0X00,0X0F,0XF0,0X01,
0X80,0X00,0X00,0X00,0X03,0XF8,0X01,0X80,0X00,0X00,0X00,0X03,0XFC,0X01,0X00,0X00,
0X01,0XF8,0X01,0XFE,0X01,0X00,0X00,0X07,0XFF,0X00,0XFF,0X02,0X00,0X00,0X0F,0XFF,
0XC0,0XFF,0X82,0X00,0X00,0X0F,0XFF,0XE1,0XFF,0XC4,0X00,0X00,0X1F,0XFF,0XF3,0X1E,
0X04,0X00,0X00,0X1F,0XFF,0XFE,0X1C,0X08,0X00,0X00,0X1F,0XFF,0XFC,0X18,0XE8,0X00,
0X00,0X1F,0XFF,0XFC,0X1F,0XF0,0X00,0X00,0X0F,0XFF,0XFE,0X9F,0XE0,0X00,0X00,0X07,
0XFF,0XFF,0X9F,0XC0,0X00,0X10,0X01,0XFF,0XFF,0X9F,0XC0,0X00,0X30,0X00,0X1F,0XFF,
0X9F,0X88,0X00,0X38,0X00,0X03,0XFF,0X9F,0X18,0X00,0X38,0X00,0X00,0X7F,0X9E,0X3C,
0X00,0X3C,0X00,0X00,0X1F,0X9C,0X7C,0X00,0X3E,0X00,0X00,0X0F,0X98,0X00,0X00,0X3F,
0X80,0X00,0X07,0X98,0X00,0X00,0X3F,0XC0,0X00,0X03,0XFF,0XFC,0X00,0X3F,0XF0,0X00,
0X01,0XFF,0XFC,0X00,0X3F,0XFE,0X00,0X01,0XFF,0XFC,0X00,0X3F,0XFF,0XC0,0X00,0XFF,
0XF8,0X00,0X1F,0XFF,0XFC,0X01,0XFF,0XF8,0X00,0X1F,0XFF,0XFE,0X02,0XFF,0XF8,0X00,
0X1F,0XFF,0XFF,0X08,0XFF,0XF8,0X00,0X1F,0XFF,0XFF,0X90,0XFF,0XF0,0X00,0X0C,0X7F,
0XFF,0XA0,0XFF,0XF0,0X00,0X00,0X3F,0XFF,0X40,0XFF,0XE0,0X00,0X10,0X1F,0XFF,0X80,
0XFF,0XE0,0X00,0X20,0X0F,0XFE,0X00,0XFF,0XC0,0X00,0X20,0X03,0XFC,0X01,0XFF,0X80,
0X00,0X20,0X00,0X18,0X01,0XFF,0X00,0X00,0X60,0X00,0X20,0X03,0XFE,0X00,0X00,0X60,
0X00,0X80,0X07,0XFC,0X00,0X00,0X60,0X03,0X00,0X0F,0XF8,0X00,0X00,0X60,0X0C,0X00,
0X1F,0XF0,0X00,0X00,0X70,0X70,0X00,0XFF,0XC0,0X00,0X00,0X3F,0XC0,0X0F,0XFF,0X00,
0X00,0X00,0X1E,0X00,0X3F,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};

const char* ssid     = "***";
const char* password = "***";

const char* host = "192.168.1.107";
const int httpPort = 1880;

String projectid;
String customer;
String pm;
String owner;
String cstep;
String eta;
String late;

void setup(void)
{
    Serial.begin(115200);
    display.init(115200);
    delay(10);

    // We start by connecting to a WiFi network
    
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  
}

void loop()
{

    delay(10000);

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/get/project";
    
    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

    
    // Read all the lines of the reply from server and print them to Serial
    String line;
     while(client.available()) {
        line = client.readStringUntil('\r');
        Serial.println(line);
    }
    
    
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 254;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, line);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
  
    // Extract values
    Serial.println(F("Response:"));

    projectid = String(doc["id"].as<char*>());
    customer = String(doc["customer"].as<char*>());
    pm = String(doc["pm"].as<char*>());
    owner = String(doc["Owner"].as<char*>());
    cstep = String(doc["current_step"].as<char*>());
    eta = String(doc["ETA"].as<char*>());
    late = String(doc["Late level"].as<char*>());
    
    Serial.println("MAC: " + WiFi.macAddress());
    Serial.println("Project: " +projectid);
    Serial.println("Customer: " + customer);
    Serial.println("Project Mgr: " + pm);
    Serial.println("Task Owner: " + owner);
    Serial.println("Task: " + cstep);
    Serial.println("ETA: " + eta);
    Serial.println("Late Level: " + late);
    
    Serial.println();
    Serial.println("closing connection");
   
  Serial.println("Coucou");
  display.drawPaged(showFontCallback);
  delay(DEMO_DELAY * 1000);
  client.stop();
}

void showFontCallback()
{
  //const char* name = "FreeSans9pt7b";
  const GFXfont* f = &FreeSans9pt7b;
  const GFXfont* bold = &FreeSansBold9pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  
  display.setRotation(3);
  display.setCursor(0, 0);
  display.drawBitmap(0, 0, ST12Logo, 50 , 50, GxEPD_BLACK);
  
  display.setCursor(70, 40);
  display.setFont(bold);
  display.setTextSize(2);
  display.print("PR ");
  display.println(projectid);
  
  display.setCursor(50, 70);
  display.setTextSize(1);
  display.setFont(f);
  display.print("Owner:"); 
  display.setCursor(115, 70);
  display.setFont(bold);
  display.println(owner);
  
  display.setCursor(50, 90);
  display.setTextSize(1);
  display.setFont(f);
  display.print("Step: "); 
  display.setCursor(115, 90);
  display.setFont(bold);
  display.println(cstep);
  
  display.setCursor(50, 110);
  display.setTextSize(1);
  display.setFont(f);
  display.print("ETA: "); 
  display.setCursor(115, 110);
  display.setFont(bold);
  display.println(eta);
  
}
