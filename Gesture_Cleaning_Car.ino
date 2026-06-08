#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ---- Network Credentials (Abstracted for Security Deployment) ----
const char* ssid     = "Your_WiFi_SSID";
const char* password = "Your_WiFi_Password";

// ---- Object Initialization ----
ESP8266WebServer server(80);

// ---- Motor Driver (L298N) Pin Allocations ----
// Left Motor Channels
const int Motor_L1 = 0;   // D3 (GPIO0)
const int Motor_L2 = 2;   // D4 (GPIO2)
const int LEnable  = 12;  // D6 (GPIO12) - PWM Speed control

// Right Motor Channels
const int Motor_R1 = 4;   // D2 (GPIO4)
const int Motor_R2 = 5;   // D1 (GPIO5) - PWM Speed control
const int REnable  = 14;  // D5 (GPIO14)

// ---- Global Variables ----
String direction = "S";    // S = Stop, F = Forward, B = Backward, L = Left, R = Right
int robospeed = 800;       // Set PWM speed threshold (Range: 0 - 1023)

// ---- Function Prototypes ----
void handleRequest();
void connectingToWiFi();
void move_forward();
void move_backward();
void turn_right();
void turn_left();
void move_stop();

void setup() {
  Serial.begin(115200);
  Serial.println("\n[!] Initializing NodeMCU Gesture Control System...");

  // Configure Motor Control GPIOs as Output
  pinMode(Motor_L1, OUTPUT);
  pinMode(Motor_L2, OUTPUT);
  pinMode(Motor_R1, OUTPUT);
  pinMode(Motor_R2, OUTPUT);
  pinMode(REnable, OUTPUT);
  pinMode(LEnable, OUTPUT);

  // Initialize WiFi Station Profile
  connectingToWiFi();

  // Route Incoming Web API Endpoints
  server.on("/", handleRequest);
  server.onNotFound(handleRequest);
  
  server.begin();
  Serial.println("[+] HTTP Web Server Started successfully.");
}

void loop() {
  server.handleClient();
}

// ---- Request Parsing & Core Logic Routing ----
void handleRequest() {
  if (server.hasArg("direction")) {
    direction = server.arg("direction");
    Serial.print("[->] Gesture Input Decoded: ");
    Serial.println(direction);

    // Apply Hardware PWM Drive Speeds
    analogWrite(REnable, robospeed);
    analogWrite(LEnable, robospeed);

    // Dynamic State Machine Routing
    if (direction == "F") {
      move_forward();
    } else if (direction == "B") {
      move_backward();
    } else if (direction == "R") {
      turn_right();
    } else if (direction == "L") {
      turn_left();
    } else if (direction == "S") {
      move_stop();
    }
  }
  server.send(200, "text/plain", "Command Processed");
}

// ---- Network Subsystem Interface ----
void connectingToWiFi() {
  delay(1000);
  WiFi.disconnect();
  delay(1000);
  
  Serial.println("[+] Establishing Connection to Access Point...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n[+] Network Auth Success!");
  Serial.print("[+] NodeMCU IP Address allocation: ");
  Serial.println(WiFi.localIP());
}

// ---- Low-Level H-Bridge Actuation Primitives ----
void move_forward() {
  digitalWrite(Motor_L1, LOW);
  digitalWrite(Motor_L2, HIGH);
  digitalWrite(Motor_R1, HIGH);
  digitalWrite(Motor_R2, LOW);
}

void move_backward() {
  digitalWrite(Motor_L1, HIGH);
  digitalWrite(Motor_L2, LOW);
  digitalWrite(Motor_R1, LOW);
  digitalWrite(Motor_R2, HIGH);
}

void turn_right() {
  digitalWrite(Motor_L1, LOW);
  digitalWrite(Motor_L2, HIGH);
  digitalWrite(Motor_R1, LOW);
  digitalWrite(Motor_R2, HIGH);
}

void turn_left() {
  digitalWrite(Motor_L1, HIGH);
  digitalWrite(Motor_L2, LOW);
  digitalWrite(Motor_R1, LOW);
  digitalWrite(Motor_R2, HIGH);
}

void move_stop() {
  digitalWrite(Motor_L1, LOW);
  digitalWrite(Motor_L2, LOW);
  digitalWrite(Motor_R1, LOW);
  digitalWrite(Motor_R2, LOW);
}