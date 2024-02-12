// This is the one that is in production
#include <WiFiS3.h>

#define RELAY1_PIN 4
#define RELAY2_PIN 7
#define RELAY3_PIN 8
#define RELAY4_PIN 12

const char ssid[] = "xenadoo";
const char pass[] = "G33L0ngR0ck5Y";

int status = WL_IDLE_STATUS;
bool daylightMode = true; // Variable to track the current mode

WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  printWifiStatus();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String HTTP_req = "";
    while (client.connected()) {
      if (client.available()) {
        Serial.println("New HTTP Request");
        HTTP_req = client.readStringUntil('\n');
        Serial.print("<< ");
        Serial.println(HTTP_req);
        break;
      }
    }

    while (client.connected()) {
      if (client.available()) {
        String HTTP_header = client.readStringUntil('\n');
        if (HTTP_header.equals("\r"))
          break;
        Serial.print("<< ");
        Serial.println(HTTP_header);
      }
    }

    if (HTTP_req.indexOf("GET") == 0) {
      if (HTTP_req.indexOf("relay1") > -1) {
        digitalWrite(RELAY1_PIN, !digitalRead(RELAY1_PIN));
        Serial.println("Toggled relay 1");
      } else if (HTTP_req.indexOf("relay2") > -1) {
        digitalWrite(RELAY2_PIN, !digitalRead(RELAY2_PIN));
        Serial.println("Toggled relay 2");
      } else if (HTTP_req.indexOf("relay3") > -1) {
        digitalWrite(RELAY3_PIN, !digitalRead(RELAY3_PIN));
        Serial.println("Toggled relay 3");
      } else if (HTTP_req.indexOf("relay4") > -1) {
        digitalWrite(RELAY4_PIN, !digitalRead(RELAY4_PIN));
        Serial.println("Toggled relay 4");
      } else if (HTTP_req.indexOf("focus") > -1) {
        digitalWrite(RELAY1_PIN, HIGH);
        delay(500);
        digitalWrite(RELAY1_PIN, LOW);
        Serial.println("Triggered Auto Focus");
      } else if (HTTP_req.indexOf("trigger") > -1) {
        digitalWrite(RELAY2_PIN, HIGH);
        delay(100);
        digitalWrite(RELAY2_PIN, LOW);
        Serial.println("Took an Image!");
      } else if (HTTP_req.indexOf("longexp") > -1) {
        int start_index = HTTP_req.indexOf("longexp/") + strlen("longexp/");
        int end_index = HTTP_req.indexOf(" HTTP");
        String param = HTTP_req.substring(start_index, end_index);

        int delay_time = param.substring(0, param.indexOf("/")).toInt();
        int repeat_times = 1; // Default value to 1

        if (param.indexOf("/") != -1) {
          repeat_times = param.substring(param.indexOf("/") + 1).toInt();
        }

        if (delay_time > 0 && repeat_times > 0) {
          for (int i = 0; i < repeat_times; i++) {
            digitalWrite(RELAY2_PIN, HIGH);
            delay(delay_time * 1000);
            digitalWrite(RELAY2_PIN, LOW);
            delay(500);
            Serial.println("Delayed for " + String(delay_time) + " seconds");
          }
        } else {
          Serial.println("Invalid parameters");
        }
      } else if (HTTP_req.indexOf("mode") > -1) { // Handle mode switching
        daylightMode = !daylightMode; // Toggle mode
        Serial.println("Switched to " + String(daylightMode ? "daylight mode" : "night vision mode"));
      }
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head>");
    client.println("<title>Arduino Relay Control</title>");
    client.println("<style>");
    client.println(".timer {");
    client.println("  display: inline-block;");
    client.println("  font-family: monospace;");
    client.println("  font-size: 24px;");
    client.println("  color: red;");
    client.println("}");
    client.println("</style>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>Arduino Relay Control</h1>");
    client.println("<h2>Individual Relay Control:</h2>");
    client.println("<ul>");
    client.println("<li><button id=\"relay1\">Toggle Relay 1</button></li>");
    client.println("<li><button id=\"relay2\">Toggle Relay 2</button></li>");
    client.println("<li><button id=\"relay3\">Toggle Relay 3</button></li>");
    client.println("<li><button id=\"relay4\">Toggle Relay 4</button></li>");
    client.println("</ul>");
    client.println("<h2>All Relays Control:</h2>");
    client.println("<ul>");
    client.println("<li><button id=\"focus\">Auto Focus</button></li>");
    client.println("<li><button id=\"trigger\">Take an Image</button></li>");
    client.println("<li><button id=\"mode\">Switch Mode</button></li>"); // Added mode switch button
    client.println("<li><label for=\"exposuretime\">Choose Exposure Time:</label></li>");
    client.println("<li><button id=\"longExposureButton\">Long Exposure</button></li>");
    client.println("<select name=\"exposuretime\" id=\"exposuretime\">");
    client.println("<option value=\"5\">5 seconds</option>");
    client.println("<option value=\"10\">10 seconds</option>");
    client.println("<option value=\"15\">15 seconds</option>");
    client.println("<option value=\"20\">20 seconds</option>");
    client.println("<option value=\"25\">25 seconds</option>");
    client.println("<option value=\"30\">30 seconds</option>");
    client.println("<option value=\"60\">1 minute</option>");
    client.println("<option value=\"90\">1 minute 30 seconds</option>");
    client.println("</select>");
    client.println("<li><label for=\"repeatTimes\">How Many Repeats:</label></li>");
    client.println("<input type=\"number\" id=\"repeatTimes\" placeholder=\"Repeat Times\" value=\"1\">");
    client.println("<li><label for=\"countdown\">Countdown Timer:</label></li>");
    client.println("<p id=\"countdown\" class=\"timer\"></p>");
    client.println("</ul>");
    client.println("<script>");
    client.println("document.addEventListener('DOMContentLoaded', function() {");
    client.println("  document.getElementById('relay1').addEventListener('click', function() {");
    client.println("    sendRequest('/relay1');");
    client.println("  });");
    client.println("  document.getElementById('relay2').addEventListener('click', function() {");
    client.println("    sendRequest('/relay2');");
    client.println("  });");
    client.println("  document.getElementById('relay3').addEventListener('click', function() {");
    client.println("    sendRequest('/relay3');");
    client.println("  });");
    client.println("  document.getElementById('relay4').addEventListener('click', function() {");
    client.println("    sendRequest('/relay4');");
    client.println("  });");
    client.println("  document.getElementById('focus').addEventListener('click', function() {");
    client.println("    sendRequest('/focus');");
    client.println("  });");
    client.println("  document.getElementById('trigger').addEventListener('click', function() {");
    client.println("    sendRequest('/trigger');");
    client.println("  });");
    client.println("  document.getElementById('longExposureButton').addEventListener('click', function() {");
    client.println("    var delayValue = document.getElementById('exposuretime').value;");
    client.println("    var repeatTimes = document.getElementById('repeatTimes').value;");
    client.println("    if (repeatTimes === '') repeatTimes = '1';"); // Set default to 1 if empty
    client.println("    sendRequest('/longexp/' + delayValue + '/' + repeatTimes);");
    client.println("    startCountdownTimer(delayValue * repeatTimes);");
    client.println("  });");
    client.println("  document.getElementById('mode').addEventListener('click', function() {");
    client.println("    sendRequest('/mode');");
    client.println("  });"); // Added mode switch event listener
    client.println("  function sendRequest(path) {");
    client.println("    var xhr = new XMLHttpRequest();");
    client.println("    xhr.open('GET', path);");
    client.println("    xhr.send();");
    client.println("  }");
    client.println("  function startCountdownTimer(seconds) {");
    client.println("    var countdownElement = document.getElementById('countdown');");
    client.println("    var timer = seconds;");
    client.println("    var interval = setInterval(function() {");
    client.println("      countdownElement.innerHTML = timer;");
    client.println("      if (timer === 0) {");
    client.println("        clearInterval(interval);");
    client.println("        countdownElement.innerHTML = '';");
    client.println("      }");
    client.println("      timer--;");
    client.println("    }, 1000);");
    client.println("  }");
    client.println("});");
    client.println("</script>");
    client.println("</body>");
    client.println("</html>");
    delay(10);
    client.stop();
  }
}

void printWifiStatus() {
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Signal Strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
