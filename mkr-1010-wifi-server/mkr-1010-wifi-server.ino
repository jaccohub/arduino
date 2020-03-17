#include <SPI.h>
#include <WiFiNINA.h>

char ssid[] = "xxxx"; // wifi name
char pass[] = "xxxx"; // wifi pw

int status = WL_IDLE_STATUS;
WiFiServer server(80);

// define pins
int redLED = 7;

void setup() {
  pinMode(redLED, OUTPUT);
  Serial.begin(9600);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin(); // start the web server on port 80
  printWiFiStatus();
}


void loop() {
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<body>");
            client.println("<center>");
            client.println("<button type='submit' onclick=location.href='/ON-7\'> ON 7 </button>");
            client.println("<button type='submit' onclick=location.href='/OFF-7\'> OFF 7 </button>");
            client.println("</center");
            client.println("</body>");
            client.println("</html>");
            client.println();
            break;
          }
          else {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }

        // handle client requests
        Serial.print("CURRENT: ");
        Serial.println(currentLine);
        if (currentLine.endsWith("GET /ON-7")) {
          digitalWrite(redLED, HIGH);
        }
        if (currentLine.endsWith("GET /OFF-7")) {
          digitalWrite(redLED, LOW);
        }
      }
    }

    // close the connection
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}
