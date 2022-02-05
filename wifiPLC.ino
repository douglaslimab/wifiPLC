//-----------------------------------------------------------------
//  Name: wifiPLC  Date Created: 13/05/2020
//  Author: Douglas Lima    Date Modified: 06/01/2021
//  Board: Arduino Uno WIFI Rev 02
//-----------------------------------------------------------------
//  Description: 
//
//-----------------------------------------------------------------
//  Devices:  WIFI library
//            Relay Shield
//-----------------------------------------------------------------

#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String r1 = "off";
String r2 = "off";
String r3 = "off";
String r4 = "off";

//  digital outputs
#define relay1 4
#define relay2 7
#define relay3 8
#define relay4 12

//  digital inputs
#define din01 2
#define din02 10
#define din03 11
#define din04 13

//  analog outputs
#define a_out01 3
#define a_out02 5
#define a_out03 6
#define a_out04 9

//  analog inputs
#define a_in01 A1
#define a_in02 A2
#define a_in03 A3
#define a_in04 A4

char ssid[] = "Douglas";//"VM47E4856";          //"Douglas";
char pass[] = "hzzs03322";//"7z4ycmhbRaMh";       //"hzzs03322";
int keyIndex = 0;

//Time Control------------------------------------------
unsigned long currentMillis;
long previousMillis = 0;
long previousMillis2 = 0;
long previousMillis3 = 0;
float loopTime = 10;

float x, y, z;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

String readString;

void setup(){
  digitalWrite(relay1, LOW);      
  digitalWrite(relay2, LOW);      
  digitalWrite(relay3, LOW);      
  digitalWrite(relay4, LOW);   
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display_init();

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  pinMode(din01, INPUT);
  pinMode(din02, INPUT);
  pinMode(din03, INPUT);
  pinMode(din04, INPUT);

  pinMode(a_out01, OUTPUT);
  pinMode(a_out02, OUTPUT);
  pinMode(a_out03, OUTPUT);
  pinMode(a_out04, OUTPUT);

  pinMode(a_in01, INPUT);
  pinMode(a_in02, INPUT);
  pinMode(a_in03, INPUT);
  pinMode(a_in04, INPUT);
  
  Serial.begin(9600);

  while (status != WL_CONNECTED){
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    display_init();
    delay(1000);
  }
  
  server.begin();

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  delay(1000);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  digitalWrite(relay1, LOW);      
  digitalWrite(relay2, LOW);      
  digitalWrite(relay3, LOW);      
  digitalWrite(relay4, LOW);      
}

void loop(){
    WiFiClient client = server.available();
    
    if (client){
      Serial.println(client);

      while (client.connected()){
        if (client.available()){
          char c = client.read();   //*

          accelRead();
        
          if (readString.length() < 100){
            String aReader;
            readString += c;
            Serial.write(c);
          
            if (c == '\n'){
              //-----------------------------------------------------------------
              //  relays switching
              //-----------------------------------------------------------------
              if(readString.indexOf("?r1on") > 0){
                digitalWrite(relay1, HIGH);
                if(digitalRead(relay1))
                  r1 = "on";
                delay(1);
              } else if(readString.indexOf("?r1off") > 0){
                digitalWrite(relay1, LOW);    
                if(!digitalRead(relay1))
                  r1 = "off";
                delay(1);
              } else if(readString.indexOf("?r2on") > 0){
                digitalWrite(relay2, HIGH);    
                if(digitalRead(relay2))
                  r2 = "on";
                delay(1);
              } else if(readString.indexOf("?r2off") > 0){
                digitalWrite(relay2, LOW);    
                if(!digitalRead(relay2))
                  r2 = "off";
                delay(1);
              } else if(readString.indexOf("?r3on") > 0){
                digitalWrite(relay3, HIGH);   
                if(digitalRead(relay3))
                  r3 = "on"; 
                delay(1);
              } else if(readString.indexOf("?r3off") > 0){
                digitalWrite(relay3, LOW);    
                if(!digitalRead(relay3))
                  r3 = "off";
                delay(1);
              } else if(readString.indexOf("?r4on") > 0){
                digitalWrite(relay4, HIGH);    
                if(digitalRead(relay4))
                  r4 = "on";
                delay(1);
              } else if(readString.indexOf("?r4off") > 0){
                digitalWrite(relay4, LOW);  
                if(!digitalRead(relay4))
                  r4 = "off";  
                delay(1);
              } else if(readString.indexOf("?a1") > 0){
//                aReader = readString.substring(8, 11);
                
//                analogWrite(a_out01, aReader);
//                Serial.print("Analog: ");
//                Serial.println(aReader);
//                delay(1);
              } else{
                Serial.println("Envia dado");
                delay(1);
              }

              relay_state_screen();
              htmlCode();
            }
          }
        }  
      }  
    }
}
//-----------------------------------------------------------------
//  acceloremeter reading
//-----------------------------------------------------------------
void  accelRead(){
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }
}
//-------------
//  html code
//-------------
void htmlCode(){
  WiFiClient client = server.available();

  client.println("<html><head><meta charset='UTF-8'><title> Douglas Iot </title>");
  client.println("<style>body{margin: 1rem; padding: 1rem; display: flex; flex-direction: column; justify-content: center; items-align: center; border: 1px solid #0f0;color: #0f0;background-color: #333;}");
  client.println("table{ }</style></head>");

    client.println("<body><table><tr><td>Digital output 01: ");
    client.print(digitalRead(relay1));
    client.println("</td><td></td><td>Analog output 01: ");
    client.print(analogRead(a_out01));
    client.println("</td></tr><tr><td>Digital output 02: ");
    client.print(digitalRead(relay2));
    client.println("</td><td></td><td>Analog output 02: ");
    client.print(analogRead(a_out02));
    client.println("</td></tr><tr><td>Digital output 03: ");
    client.print(digitalRead(relay3));
    client.println("</td><td></td><td>Analog output 03: ");
    client.print(analogRead(a_out03));
    client.println("</td></tr><tr><td>Digital output 04: ");
    client.print(digitalRead(relay4));
    client.println("</td><td></td><td>Analog output 04: ");
    client.print(analogRead(a_out04));
    client.println("</td></tr></table>");

    client.println("<table><tr><td>Digital input 01: ");
    client.print(digitalRead(din01));
    client.println("</td><td>Analog input 01: ");
    client.print(analogRead(a_in01));
    client.println("</td></tr><tr><td>Digital input 02: ");
    client.print(digitalRead(din02));
    client.println("</td><td>Analog input 02: ");
    client.print(analogRead(a_in02));
    client.println("</td></tr><tr><td>Digital input 03: ");
    client.print(digitalRead(din03));
    client.println("</td><td>Analog input 03: ");
    client.print(analogRead(a_in03));
    client.println("</td></tr><tr><td>Digital input 04: "); 
    client.print(digitalRead(din04));
    client.println("</td><td>Analog input 04: ");
    client.print(analogRead(a_in04));
    client.println("</td></tr></table></body></html>");
  
  readString = "";
  delay(10);
  client.stop();
  Serial.println();
}
//-------------
//  load oled display
//-------------
void relay_state_screen(void) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.print(F("Relay 1: "));
  display.println(r1);
  display.print(F("Relay 2: "));
  display.println(r2);
  display.print(F("Relay 3: "));
  display.println(r3);
  display.print(F("Relay 4: "));
  display.println(r4);

  display.display();
  delay(100);
}
void display_init(void){
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(F("wifiPLC.init().."));
  display.println(WiFi.SSID());
  display.println(WiFi.localIP());

  display.display();
  delay(100);
}
