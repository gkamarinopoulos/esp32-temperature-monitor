#include <WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WebServer.h>
#include <time.h>

// Replace with your Wi-Fi network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

#define ONE_WIRE_BUS 18
#define BUZZER_PIN 19
#define TEMP_THRESHOLD 30.0

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float currentTemp = -127.0;

WebServer server(80);
String tempLog = "";
String timeLabels[20];
float tempValues[20];
int entryCount = 0;

String generatePage() {
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta http-equiv='refresh' content='5'>";
  html += "<title>ESP32 Temp Monitor</title>";
  html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
  html += "<style>";
  html += "body{font-family:sans-serif;background:#1e1e1e;color:#fff;text-align:center;padding:30px;}";
  html += "h1{color:#f66;}";
  html += ".container{display:flex;justify-content:center;gap:30px;flex-wrap:wrap;align-items:flex-start;margin-top:30px;}";
  html += "table{border-collapse:collapse;width:300px;}th,td{border:1px solid #ccc;padding:6px;}th{background:#333;}td{background:#2b2b2b;}";
  html += "canvas{max-width:400px;}";
  html += "</style></head><body>";

  html += "<h1>🌡️ Temperature: ";
  html += String(currentTemp, 2);
  html += " &deg;C</h1>";

  if (currentTemp >= TEMP_THRESHOLD) {
    html += "<p style='color:red;font-size:20px;'>🚨 Overheating detected. Buzzer is ON.</p>";
  } else {
    html += "<p style='color:lightgreen;font-size:18px;'>✅ Within safe range.</p>";
  }

  html += "<div class='container'>";
  html += "<div><canvas id='tempChart'></canvas></div>";
  html += "<div><h2>Live Temperature Log</h2>";
  html += "<table><tr><th>Time (UTC)</th><th>Temperature (&deg;C)</th></tr>";
  html += tempLog;
  html += "</table></div></div>";

  html += "<script>";
  html += "const ctx = document.getElementById('tempChart').getContext('2d');";
  html += "const chart = new Chart(ctx, {type: 'line',data: {labels: [";

  for (int i = entryCount - 1; i >= 0; i--) {
    html += "'";
    html += timeLabels[i];
    html += "'";
    if (i > 0) html += ",";
  }

  html += "],datasets: [{label: 'Temperature (°C)',data: [";

  for (int i = entryCount - 1; i >= 0; i--) {
    html += String(tempValues[i], 2);
    if (i > 0) html += ",";
  }

  html += "],borderColor: 'rgba(255,99,132,1)',borderWidth: 2,tension:0.3,fill:false}]}";
  html += ",options: {scales: {y: {beginAtZero: false},x:{ticks:{align:'center'}}}}});";
  html += "</script>";

  html += "</body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", generatePage());
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  configTime(0, 0, "pool.ntp.org");

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  sensors.requestTemperatures();
  currentTemp = sensors.getTempCByIndex(0);
  Serial.print("Current Temp: ");
  Serial.println(currentTemp);

  if (currentTemp >= TEMP_THRESHOLD) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    configTime(0, 0, "pool.ntp.org");
    delay(100);
  }

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeinfo);

  String row = "<tr><td>";
  row += timeString;
  row += "</td><td>";
  row += String(currentTemp, 2);
  row += "</td></tr>";

  tempLog = row + tempLog;

  if (entryCount < 20) {
    for (int i = entryCount; i > 0; i--) {
      timeLabels[i] = timeLabels[i - 1];
      tempValues[i] = tempValues[i - 1];
    }
    timeLabels[0] = timeString;
    tempValues[0] = currentTemp;
    entryCount++;
  } else {
    for (int i = 19; i > 0; i--) {
      timeLabels[i] = timeLabels[i - 1];
      tempValues[i] = tempValues[i - 1];
    }
    timeLabels[0] = timeString;
    tempValues[0] = currentTemp;
  }

  server.handleClient();
  delay(5000);
}
