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

#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

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

char ssid[] = "Douglas";
char pass[] = "hzzs03322";
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
  
  Serial.begin(9600); // Rx = 0, Tx = 1

  while (status != WL_CONNECTED){
    Serial.print("Attempting to connect to Ne twork named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  
  server.begin();

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

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
}

void loop(){
    WiFiClient client = server.available();
    
    if (client){
      Serial.println(client);

      while (client.connected()){
        if (client.available()){
          char c = client.read();

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
                delay(1);
              } else if(readString.indexOf("?r1off") > 0){
                digitalWrite(relay1, LOW);    
                delay(1);
              } else if(readString.indexOf("?r2on") > 0){
                digitalWrite(relay2, HIGH);    
                delay(1);
              } else if(readString.indexOf("?r2off") > 0){
                digitalWrite(relay2, LOW);    
                delay(1);
              } else if(readString.indexOf("?r3on") > 0){
                digitalWrite(relay3, HIGH);    
                delay(1);
              } else if(readString.indexOf("?r3off") > 0){
                digitalWrite(relay3, LOW);    
                delay(1);
              } else if(readString.indexOf("?r4on") > 0){
                digitalWrite(relay4, HIGH);    
                delay(1);
              } else if(readString.indexOf("?r4off") > 0){
                digitalWrite(relay4, LOW);    
                delay(1);
              } else if(readString.indexOf("?a1") > 0){
                aReader = readString.substring(8, 11);
                
//                analogWrite(a_out01, aReader);
                Serial.print("Analog: ");
                Serial.println(aReader);
                delay(1);
              } else{
                Serial.println("Envia dado");
                delay(1);
              }
                
              //-----------------------------------------------------------------
              //  HTML code
              //-----------------------------------------------------------------
              client.println("<html>");
              client.println("<head>");
//              client.println("<meta charset="UTF-8">");
              client.println("<title> Douglas Iot </title>");
              client.println("</head>");

              client.println("<table>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital output 01: ");
              client.print(digitalRead(relay1));
              client.println("</td>");
              client.print("<td>");
              client.print(" ");
              client.println("</td>");
              client.print("<td>");
              client.print("Analog output 01: ");
              client.print(analogRead(a_out01));
              client.println("</td>");
              client.println("</tr>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital output 02: ");
              client.print(digitalRead(relay2));
              client.println("</td>");
              client.print("<td>");
              client.print(" ");
              client.println("</td>");
              client.print("<td>");
              client.print("Analog output 02: ");
              client.print(analogRead(a_out02));
              client.println("</td>");
              client.println("</tr>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital output 03: ");
              client.print(digitalRead(relay3));
              client.println("</td>");
              client.print("<td>");
              client.print(" ");
              client.println("</td>");
              client.print("<td>");
              client.print("Analog output 03: ");
              client.print(analogRead(a_out03));
              client.println("</td>");
              client.println("</tr>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital output 04: ");
              client.print(digitalRead(relay4));
              client.println("</td>");
              client.print("<td>");
              client.print(" ");
              client.println("</td>");
              client.print("<td>");
              client.print("Analog output 04: ");
              client.print(analogRead(a_out04));
              client.println("</td>");
              client.println("</tr>");
              client.println("</table>");

              client.println("<table>");
              client.print("<table>");
              client.print("<td>");
              client.print(" ");
              client.println("</td>");
              client.println("</rd>");
              client.println("</table>");
              
              client.println("<table>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital input 01: ");
              client.print(digitalRead(din01));
              client.println("</td>");
              client.print("<td>");
              client.print("Analog input 01: ");
              client.print(analogRead(a_in01));
              client.println("</td>");
              client.println("</tr>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital input 02: ");
              client.print(digitalRead(din02));
              client.println("</td>");
              client.print("<td>");
              client.print("Analog input 02: ");
              client.print(analogRead(a_in02));
              client.println("</td>");
              client.println("</tr>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital input 03: ");
              client.print(digitalRead(din03));
              client.println("</td>");
              client.print("<td>");
              client.print("Analog input 03: ");
              client.print(analogRead(a_in03));
              client.println("</td>");
              client.println("</tr>");
              client.println("<tr>");
              client.print("<td>");
              client.print("Digital input 04: ");
              client.print(digitalRead(din04));
              client.println("</td>");
              client.print("<td>");
              client.print("Analog input 04: ");
              client.print(analogRead(a_in04));
              client.println("</td>");
              client.println("</tr>");
              client.println("</table>");

              client.println("</body>");
              client.println("</html>");
              //-----------------------------------------------------------------
              
              readString = "";
              delay(10);
              client.stop();
              Serial.println();
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
