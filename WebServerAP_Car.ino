/*------ Access Point Stuff ------*/
#include "WiFiEsp.h"
// Emulate Serial1 on pins 11/4 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(11, 4);  // RX, TX
#endif

/*------ Car Stuff ------*/
#include <PWMServo.h>
#define IN1 7
#define IN2 8
#define ENA 5  //  ENA pin


#define FRONT 70  // steering to front
int SHARP_RIGHT = FRONT + 28;
int SHARP_LEFT = FRONT - 30;

#define DELAY_TIME 1500
int L_SPEED = 130;
#define SERVO_PIN 9  //servo connect to D9
#define SERVO_SENSOR 10

#define Echo_PIN    2   // Ultrasonic Echo pin connect to D11
#define Trig_PIN    3  // Ultrasonic Trig pin connect to D12

PWMServo head;
PWMServo head_sensor;

/*motor control*/
void backward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);
}

void forward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);
}

void turn(int angle) {
  head.write(angle);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

char ssid[] = "LP_InesTania";      // your network SSID (name)
char pass[] = "projeto_carro123";  // your network password
int status = WL_IDLE_STATUS;       // the Wifi radio's status
int reqCount = 0;                  // number of requests received

WiFiEspServer server(80);

void setup() {
  Serial.begin(115200);  // initialize serial for debugging
  Serial1.begin(9600);   // initialize serial for ESP module
  WiFi.init(&Serial1);   // initialize ESP module
  pinMode(Trig_PIN, OUTPUT); 
  pinMode(Echo_PIN,INPUT); 

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true)
      ;  // don't continue
  }

  Serial.print(F("Attempting to start AP "));
  Serial.println(ssid);

  // uncomment these two lines if you want to set the IP address of the AP
  //IPAddress localIp(192, 168, 111, 111);
  //WiFi.configAP(localIp);

  // start access point
  status = WiFi.beginAP(ssid, 10, pass, ENC_TYPE_WPA2_PSK);

  Serial.println(F("Access point started"));
  printWifiStatus();

  // start the web server on port 80
  server.begin();
  Serial.println(F("Server started"));

  head.attach(SERVO_PIN);
  head_sensor.attach(SERVO_SENSOR);
  head.write(FRONT);
  head_sensor.write(FRONT);
}



void loop() {
  WiFiEspClient client = server.available();  // listen for incoming clients

  watch();

  if (client) {                                 //if a client is connected
    String req = client.readStringUntil('\r');  //read the request
    Serial.println(F("New request"));

    while (client.available()) {
      //read bytes left in request
      client.read();
    }
    watch();
    //Make the action indicated by the request and send a response
    if (req.indexOf("GET /FORWARD") >= 0) {
      sendEmptyResponse(client);
        turn(FRONT);
        head_sensor.write(FRONT);
        forward(L_SPEED);
    } else if (req.indexOf("GET /BACKWARDS") >= 0) {
      sendEmptyResponse(client);
      turn(FRONT);
      backward(L_SPEED);
      delay(DELAY_TIME);
      stop();
    } else if (req.indexOf("GET /FL") >= 0) {
      sendEmptyResponse(client);
      turn(SHARP_LEFT);
      head_sensor.write(90);
      forward(L_SPEED);
      delay(DELAY_TIME);
    } else if (req.indexOf("GET /FR") >= 0) {
      sendEmptyResponse(client);
      turn(SHARP_RIGHT);
      head_sensor.write(50);
      forward(L_SPEED);
      delay(DELAY_TIME);
    } else if (req.indexOf("GET /BR") >= 0) {
      sendEmptyResponse(client);
      turn(SHARP_LEFT);
      backward(L_SPEED);
      delay(DELAY_TIME);
      stop();
    } else if (req.indexOf("GET /BL") >= 0) {
      sendEmptyResponse(client);
      turn(SHARP_RIGHT);
      backward(L_SPEED);
      delay(DELAY_TIME);
      stop();
    } else if (req.indexOf("GET /STOP") >= 0) {
      sendEmptyResponse(client);
      stop();
    } else if(req.indexOf("GET /SPEED") >= 0) {
      switch(req[10]) {
        case '1': L_SPEED = 130; break;
        case '2': L_SPEED = 145; break;
        case '3': L_SPEED = 160; break;
        case '4': L_SPEED = 175; break;
        case '5': L_SPEED = 190; break;
        default: L_SPEED = 130; break;
      }
      }else if (req.indexOf("GET /") >= 0) {
      sendHttpResponse(client);
    }


    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
}


int watch(){
  long echo_distance;
  digitalWrite(Trig_PIN,LOW);
  delayMicroseconds(5);                                                                              
  digitalWrite(Trig_PIN,HIGH);
  delayMicroseconds(15);
  digitalWrite(Trig_PIN,LOW);
  echo_distance=pulseIn(Echo_PIN,HIGH);
  echo_distance=echo_distance*0.01657; //how far away is the object in cm
  //Serial.println((int)echo_distance);
  if(echo_distance < 30) {
    Serial.println(echo_distance);
    stop();
    delay(10);
    return true;
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}