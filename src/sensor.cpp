#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFiClientSecure.h>

// Definiera DHT11-sensorns pinne
#define DHTPIN 0      // Pin som är ansluten till DHT11-data-pinnen
#define DHTTYPE DHT11 // DHT 11 sensor

DHT dht(DHTPIN, DHTTYPE);

const char* apiToken = "GYaBnFXg4AwJtyu8k5yRZruN4O5Ssxe3yuxB1lyE3oKnyibaHxpljWz2FQn-h4ZPxpKwPT1e0zc1UD0DJ4FXRQ==";
// Definiera dina WiFi-inloggningsuppgifter
const char* ssid = "Tele2_9cae3d";       // Din WiFi SSID
const char* password = "cjmzjywy"; // Ditt WiFi-lösenord

// CA-certifikat (konverterat från den givna texten)
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEFTCCAv2gAwIBAgIURJ5uNfY7nI8IGE38opN/JBZcK74wDQYJKoZIhvcNAQEL\n" \
"BQAwgZkxCzAJBgNVBAYTAlNFMRIwEAYDVQQIDAlTdG9ja2hvbG0xEjAQBgNVBAcM\n" \
"CVN0b2NraG9sbTESMBAGA1UECgwJbG9jYWxob3N0MRIwEAYDVQQLDAlsb2NhbGhv\n" \
"c3QxEjAQBgNVBAMMCWxvY2FsaG9zdDEmMCQGCSqGSIb3DQEJARYXam9lLmtvbmd0\n" \
"aG9uZ0BnbWFpbC5jb20wHhcNMjQxMDI2MTk1NDM4WhcNMjcwODE2MTk1NDM4WjCB\n" \
"mTELMAkGA1UEBhMCU0UxEjAQBgNVBAgMCVN0b2NraG9sbTESMBAGA1UEBwwJU3Rv\n" \
"Y2tob2xtMRIwEAYDVQQKDAlsb2NhbGhvc3QxEjAQBgNVBAsMCWxvY2FsaG9zdDES\n" \
"MBAGA1UEAwwJbG9jYWxob3N0MSYwJAYJKoZIhvcNAQkBFhdqb2Uua29uZ3Rob25n\n" \
"QGdtYWlsLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANuZQQ7n\n" \
"bLfV8oyOoNa8V77N8qwowEvvoEdbtSRL5B5Bx5m2/BF3ah7BH+dtJ+x5FuCZ0kAY\n" \
"6WfIZPTsn4vS3q74A3vWWw65/O86ZjHHstxwJoBdA/jR9ANmw7HZ39uYU5dq0Cl/\n" \
"+uQHRghX7nwVA9o9eRjTiX2EfYETlzlULfLNy4A+w11pfGxR8WOH4NzN1MbCfWQY\n" \
"alVk38J+gZJ25XtbPMpmHCD0uSg+2AXO8I9TIe+jRW3o5OQWrZhAmH5k9ctJKRw/\n" \
"y0l3RMpXFtazuiW/2rIXn67HHEQpwMWmVua4TFjfZsDw8vu579ZwaDmMgSC7XvBN\n" \
"UA1Ua3stcXqjh6kCAwEAAaNTMFEwHQYDVR0OBBYEFNePoYAo9rdckZxqUHW+EP5N\n" \
"I02DMB8GA1UdIwQYMBaAFNePoYAo9rdckZxqUHW+EP5NI02DMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wDQYJKoZIhvcNAQELBQADggEBADPgLkhVKBwg2CovLuvs7v2Z507yczvS\n" \
"QjSTixheUH4AGg2S0ww9ozmACFvwtARq0Z0vRxfqtIJBY/IIA/is4xtFpdonqxeD\n" \
"OoPikizA3n7BiM0CF4Uo3RAlk4bByS/fpWfgw5EAqt01GOuWccdt8m0+EIFP1bsg\n" \
"TKcvc02IVSIYBdUkvoElG0F8mBRb/XYvyWgxYe0HhXsIFNNDzlc4CdCF9F6caPoK\n" \
"yVCuyf4eiX/I9egl9xWF+CiWxVEQd7AJmfM2gr0TDDGDa+4EEd1D5g/pMO2pyvCL\n" \
"0IJy5dOzRH51+pATjogonBMt5c/m9huTwVPwP4bhNiUufQtUBteQMok=\n" \
"-----END CERTIFICATE-----\n";

// Klientens certifikat (exempel, fyll i ditt egna certifikat)
const char* client_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEBDCCAuygAwIBAgIUQI/k8GGGqVGbbtWeSbKR2JMwC9swDQYJKoZIhvcNAQEL\n" \
"BQAwgZkxCzAJBgNVBAYTAlNFMRIwEAYDVQQIDAlTdG9ja2hvbG0xEjAQBgNVBAcM\n" \
"CVN0b2NraG9sbTESMBAGA1UECgwJbG9jYWxob3N0MRIwEAYDVQQLDAlsb2NhbGhv\n" \
"c3QxEjAQBgNVBAMMCWxvY2FsaG9zdDEmMCQGCSqGSIb3DQEJARYXam9lLmtvbmd0\n" \
"aG9uZ0BnbWFpbC5jb20wHhcNMjQxMDI2MTk1NjE0WhcNMjYwMzEwMTk1NjE0WjCB\n" \
"mTELMAkGA1UEBhMCU0UxEjAQBgNVBAgMCVN0b2NraG9sbTESMBAGA1UEBwwJU3Rv\n" \
"Y2tob2xtMRIwEAYDVQQKDAlsb2NhbGhvc3QxEjAQBgNVBAsMCWxvY2FsaG9zdDES\n" \
"MBAGA1UEAwwJbG9jYWxob3N0MSYwJAYJKoZIhvcNAQkBFhdqb2Uua29uZ3Rob25n\n" \
"QGdtYWlsLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOPJ8NdB\n" \
"cprNSGGJRn8mZSmMaXOlT5DANhizFpmL6P71cw94QIozr4M6/yHTHrDIyUHnNSkU\n" \
"SMdwtNaicnSZSIFCsb4VRVwtWRKALof8j6L9a4x0+quSNTlmYZn58XfJwhnWgJkQ\n" \
"1H/BOXGCYDbsTZ3V8c39HrxeauT2P8AFOStM8MH/tCbz7XmNbzc0vt4KODPS9y9n\n" \
"BKcVAVRIqkk6tpA+G227KcEvF1yDPNGlpv352mmyZB2kiDXt4eqbWlhkMKfPGgZY\n" \
"ZniF43Kns0iaC+Pc2X0K36B4e4SfHwluqwpI+YP31WmpiE4zX6wMdwrRzhlDloze\n" \
"/FL0AoFNW6JgZwcCAwEAAaNCMEAwHQYDVR0OBBYEFDqLyJbXmI/nRadV+G41iDpD\n" \
"n///MB8GA1UdIwQYMBaAFNePoYAo9rdckZxqUHW+EP5NI02DMA0GCSqGSIb3DQEB\n" \
"CwUAA4IBAQAvWvif9J7Vt+cda3MLXMXR4jO9QE+H2+BZlQTsOKg1lMMA6/0kwBgd\n" \
"0IleFMFm+P7v/D1x6lYks7rhdp+nGOUrzPvt2YxE8TGNp+7lbDcyQ5TuFR23sBOD\n" \
"K6wFdk1yMuzm9XJ4lc2ymTACY+v3/BmKUY3Ml9rvGByUQaa5ZKMgqt0mwXKy6/7p\n" \
"UokPEbxsft1Xf23o9NqYOT1VZNWyMgsIcnJA8cdWROUAnDzRTWSfvNNoTVY3ilVA\n" \
"UnBbrdw9QrJziiM+QNSV/DkCxsT4qtIMUqZDUUesNjTfQZSd+5wq0ThWoTatwJWo\n" \
"H++AHfhe9pTq4lHrdP5Kg9vbEOlzKbLL\n" \
"-----END CERTIFICATE-----\n";

void sendToNGINX(float humidity, float temperature); // Deklarera funktionen här

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  // Anslut till WiFi
  WiFi.begin(ssid, password);
  Serial.print("Ansluter till WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Ansluten till WiFi!");
}

void loop() {
  // Läs temperatur och luftfuktighet från DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Kontrollera om läsningen misslyckades
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Misslyckades med att läsa från DHT11!");
    return;
  }
  
  Serial.print("Luftfuktighet: ");
  Serial.print(humidity);
  Serial.print(" %, Temperatur: ");
  Serial.print(temperature);
  Serial.println(" *C");
  
  // Skicka data till InfluxDB
  sendToNGINX(humidity, temperature);
  
  // Vänta 10 sekunder innan nästa mätning
  delay(10000);
}

void sendToNGINX(float humidity, float temperature) {
    WiFiClientSecure client; // Använd WiFiClient för HTTP
    String org = "my_org"; 
    String bucket = "sensor";

    if (client.connect("172.19.166.79", 443)) { // Ändra till din NGINX IP-adress
        String data = "environment,sensor=dht11 humidity=" + String(humidity) + ",temperature=" + String(temperature);
        String request = "POST /api/v2/write?org=" + org + "&bucket=" + bucket + "&precision=s HTTP/1.1\r\n" +
                         "Host: 172.19.166.79\r\n" + // Återigen, ändra till din NGINX IP-adress
                         "Authorization: Token " + String(apiToken) + "\r\n" +
                         "Content-Length: " + String(data.length()) + "\r\n" +
                         "Content-Type: application/x-www-form-urlencoded\r\n" +
                         "Connection: close\r\n\r\n" + data;

        client.print(request);

        while (client.connected() || client.available()) {
            if (client.available()) {
                String line = client.readStringUntil('\n');
                Serial.println(line); // Debug för att kontrollera serverns svar
            }
        }
    } else {
        Serial.println("Connection to NGINX failed!");
    }
}