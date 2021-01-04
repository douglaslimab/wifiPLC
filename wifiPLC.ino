//-----------------------------------------------------------------
//  wifiPLC
//-----------------------------------------------------------------
//-----------------------------------------------------------------

#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>

#define relay1 4
#define relay2 7
#define relay3 8
#define relay4 12

char ssid[] = "Douglas";
char pass[] = "hzzs03322";
int keyIndex = 0;

float x, y, z;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

String readString;

void setup(){
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  Serial.begin(9600);

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
    Serial.println("new client");

    while (client.connected()){
      if (client.available()){
        char c = client.read();

        accelRead();
        
        if (readString.length() < 100){
          readString += c;
          Serial.write(c);
          
          if (c == '\n'){
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
              } else{
                Serial.print("Envia dado");
                delay(1);
              }
//-----------------------------------------------------------------
//  HTML part
//-----------------------------------------------------------------              
            client.println("<html>");
            client.println("<head>");
//            client.println("<meta charset="UTF-8">");
            client.println("<title> Douglas Iot </title>");
            client.println("</head>");
            client.println("<body>");

            client.println("Relay 1: ");
            client.println(digitalRead(relay1));
            client.println("<br />");
            client.println("Relay 2: ");
            client.println(digitalRead(relay2));
            client.println("<br />");
            client.println("Relay 3: ");
            client.println(digitalRead(relay3));
            client.println("<br />");
            client.println("Relay 4: ");
            client.println(digitalRead(relay4));
            client.println("<br />");
            
            client.println("Accel x: ");
            client.println(x);
            client.println("<br />");
            client.println("Accel y: ");
            client.println(y);
            client.println("<br />");
            client.println("Accel z: ");
            client.println(z);
            client.println("<br />");
            
            client.println("</body>");
            client.println("</html>");
//-----------------------------------------------------------------
            readString="";

            delay(10);
            client.stop();
            Serial.println();
          }
        }
      }
    }
  }
}

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
