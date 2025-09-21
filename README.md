# Security-Robot-Arduino-sketch-
🤖 ESP8266 NodeMCU Robot Control + Sensors

Yeh project ek WiFi controlled robot hai jo ESP8266 NodeMCU par based hai.
Is robot ko aap mobile app, web browser ya kisi bhi HTTP client ke through control kar sakte ho.

🔹 Features

WiFi Control

NodeMCU WiFi se connect hota hai aur ek web server run karta hai.

Robot ko HTTP links (routes) ke through control kiya ja sakta hai.


Motor Control (via L298N / L293D driver)

Forward, Backward, Left, Right aur Stop commands support karta hai.

PWM ka use karke speed control bhi possible hai.


Peripheral Control

LED ON/OFF

Buzzer ON/OFF


Sensors

Ultrasonic sensor (HC-SR04) → distance measure karne ke liye (cm).

PIR motion sensor → motion detect karne ke liye.


API Endpoints (HTTP Routes)

/ping → server test (returns "pong")

/cmd?act=forward|back|left|right|stop|led_on|led_off|buzzer_on|buzzer_off

/sensor → JSON data deta hai (distance + motion).


Modes

Station Mode → WiFi router se connect hota hai aur local IP milta hai.

Access Point Mode (fallback) → Agar WiFi connect nahi hua to NodeMCU apna hotspot (RobotAP, password 12345678) start kar deta hai.
