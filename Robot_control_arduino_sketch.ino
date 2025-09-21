// ESP8266 NodeMCU sketch - Robot Control + Sensors
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

// --- WiFi credentials ---
const char* ssid     = "YourSSID";      // Enter your wifi name 
const char* password = "YourPassword";  // Enter yourt WiFi Password

ESP8266WebServer server(80);

// --- Pin definitions ---
const int IN1 = D5;  // motor A
const int IN2 = D6;
const int IN3 = D7;  // motor B
const int IN4 = D8;
const int ENA = D1;  // enable A (PWM)
const int ENB = D2;  // enable B (PWM)

const int LED_PIN    = D0;
const int BUZZER_PIN = D4;   // buzzer pin
const int MOTION_PIN = D3;   // PIR motion sensor
const int TRIG_PIN   = D5;   // ultrasonic trig
const int ECHO_PIN   = D6;   // ultrasonic echo

// --- Motor functions ---
void motors_stop() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}
void motors_forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 700); analogWrite(ENB, 700);
}
void motors_back() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 700); analogWrite(ENB, 700);
}
void motors_left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, 700); analogWrite(ENB, 700);
}
void motors_right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, 700); analogWrite(ENB, 700);
}

// --- Setup pins ---
void setup_pins() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MOTION_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  motors_stop();
}

// --- Ultrasonic ---
float measure_distance_cm() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 9999.0;
  return duration * 0.0343 / 2.0;
}

// --- Routes ---
void handle_ping() {
  server.send(200, "text/plain", "pong");
}

void handle_cmd() {
  if (!server.hasArg("act")) {
    server.send(400, "text/plain", "no act");
    return;
  }
  String act = server.arg("act"); act.toLowerCase();

  if (act == "forward") motors_forward();
  else if (act == "back") motors_back();
  else if (act == "left") motors_left();
  else if (act == "right") motors_right();
  else if (act == "stop") motors_stop();
  else if (act == "led_on") digitalWrite(LED_PIN, HIGH);
  else if (act == "led_off") digitalWrite(LED_PIN, LOW);
  else if (act == "buzzer_on") digitalWrite(BUZZER_PIN, HIGH);
  else if (act == "buzzer_off") digitalWrite(BUZZER_PIN, LOW);

  server.send(200, "text/plain", "ok");
}

void handle_sensor() {
  float d = measure_distance_cm();
  int motion = digitalRead(MOTION_PIN);

  String json = "{";
  json += "\"distance\":" + String(d, 2) + ",";
  json += "\"motion\":" + String(motion);
  json += "}";
  server.send(200, "application/json", json);
}

// --- Setup ---
void setup() {
  Serial.begin(115200);
  setup_pins();

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500); Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connected!");
    Serial.print("IP: "); Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi failed, starting AP mode...");
    WiFi.softAP("RobotAP", "12345678");
    Serial.print("AP IP: "); Serial.println(WiFi.softAPIP());
  }

  server.on("/ping", handle_ping);
  server.on("/cmd", handle_cmd);
  server.on("/sensor", handle_sensor);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}