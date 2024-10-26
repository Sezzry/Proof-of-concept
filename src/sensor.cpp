#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFiClientSecure.h>

// Definiera DHT11-sensorns pinne
#define DHTPIN 0      // Pin som är ansluten till DHT11-data-pinnen
#define DHTTYPE DHT11 // DHT 11 sensor

DHT dht(DHTPIN, DHTTYPE);

//const char* apiToken = "GYaBnFXg4AwJtyu8k5yRZruN4O5Ssxe3yuxB1lyE3oKnyibaHxpljWz2FQn-h4ZPxpKwPT1e0zc1UD0DJ4FXRQ=="; API token för influxDB men används ej då vi skickar till nginx först

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

// Klientens certifikat (fyll i ditt egna certifikat)
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
"/FL0AoFNW6JcCyUCAwEAAaNTMFEwHQYDVR0OBBYEFNePoYAo9rdckZxqUHW+EP5N\n" \
"I02DMB8GA1UdIwQYMBaAFNePoYAo9rdckZxqUHW+EP5NI02DMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wDQYJKoZIhvcNAQELBQADggEBADPgLkhVKBwg2CovLuvs7v2Z507yczvS\n" \
"QjSTixheUH4AGg2S0ww9ozmACFvwtARq0Z0vRxfqtIJBY/IIA/is4xtFpdonqxeD\n" \
"OoPikizA3n7BiM0CF4Uo3RAlk4bByS/fpWfgw5EAqt01GOuWccdt8m0+EIFP1bsg\n" \
"TKcvc02IVSIYBdUkvoElG0F8mBRb/XYvyWgxYe0HhXsIFNNDzlc4CdCF9F6caPoK\n" \
"yVCuyf4eiX/I9egl9xWF+CiWxVEQd7AJmfM2gr0TDDGDa+4EEd1D5g/pMO2pyvCL\n" \
"0IJy5dOzRH51+pATjogonBMt5c/m9huTwVPwP4bhNiUufQtUBteQMok=\n" \
"-----END CERTIFICATE-----\n";

// Klientens privata nyckel (fyll i din egen privata nyckel)
const char* client_key = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDjyfDXQXKazUhh\n" \
"iUZ/JmUpjGlzpU+QwDYYsxaZi+j+9XMPeECKM6+DOv8h0x6wyMlB5zUpFEjHcLTW\n" \
"onJ0mUiBQrG+FUVcLVkSgC6H/I+i/WuMdPqrkjU5ZmGZ+fF3ycIZ1oCZENR/wTlx\n" \
"g/mA27E2d1fHN/R68Xmrk9j/ABTkrTPDB/7Qm8+15jW83NL7eCjgz0vcvZwSnFQFU\n" \
"SKpJOraQPhttuynBLxdcgzzRpab9+dppsmQdpIg17eHqm1pYZDCnzxoGWGZ4heNy\n" \
"p7NImgvj3Nl9Ct+geHuEnx8JbqsKSPmD99VpqYhOM1+sDHcK0c4ZQ5aM3vxS9AKB\n" \
"TVuiYGcHAgMBAAECggEAUOMcytNKxv2ufjQo/G297osVxyMkpdobW5n8pHmyOxcd\n" \
"zJh/EpRX9tm0T2aQt2YVUXmwvu2dCnVTQgRdcFoeDQiJ5V3YS2su/grWeyOMPeyP\n" \
"jEjd2p8Il2FrtymyPT8/1bz93VzEmLkclNofIGG5Dhsyylrf1zQP8U4gRSeVrXHx\n" \
"YnUk+NhZbrAWus53G41Y80rblXOa2xrPiyXU+22VyHjscKB1fjsssIF9pzd8Q2SN\n" \
"ds99gvuUxILCuLdY2wRXgcG4ASGg4OmLV3bAr2AR2zUpvm2ji0uuUJ8kFe2UQcsT\n" \
"DLWucq9gknNixElVMICtqYZJ+fNjWLy6U/IgI5DXgQKBgQD2iAD4MMwXBuHERZWZ\n" \
"CY+Veeh0Hq0tepdiqr5rhQa7VAWqTh2E3QcExkAJQwWO+/lJ9scNkrIEHgyeNaF8\n" \
"XpdRx9bz2X8DYZ0pyBvaoIi65FRtYTfM1D/0wkkuSpz8Sjxya1bauJd773Tm/3vx\n" \
"h/neulS4aEjG1ZjyFwRTtH5LO6QKBgQDsiadakIq6TDJXIiRc5cQ+knLtuKTkefVz\n" \
"TKNTFMkLcvjwGCtv7ZDDhR86gGaAb3f5fLiZ1nEREpre59Gh7pEWxw0TDBgcZTRy\n" \
"5qENp56MGZeuKMSIAdKwg1qNgrHjvBiea1fm+ZvTJfBPwaPdrPHcK2v0VhjCFUkC\n" \
"m/Htx4WUwbwKBgQDVAM1Vo1Oy0Y9fNX/7qfcU/y+IpLicAr+p0DyWSWl3kTET5Eo8\n" \
"8XuO9Ht/hRjYYX0SGtYBNC36wLvGNjBAdLnpPvDJ37GOCljwMKVgUDPSxlTr7IN5\n" \
"xik8IGF6TZ01ni577aIteC/qNQqcS0GbfMMLZHlgrTels5+3MQwTlKQTgQKBgQDV\n" \
"pPeNq2RCJLSLUwE5DycTxT8dTFtTKnzyKVSTpAB64NvMMGh7bj4E+2ZbdsIeW5An\n" \
"vekWukPhwcY0c3o7IrnVsvAyNghJDXg0F+9739wqeqsv2Q66MbtVRy8xBc0a/zfV\n" \
"TutUaju+mVPXFfWmMaE61dVqGaG/G26Yc5V8JLM67QKBgCBvuhYebe9x68Zne3jF\n" \
"VV/xhf307MqWWT7tAePg0KKpUX6iS7V7bqr4nUd/H7CTxe+uMxUUOEv+nGdMz8zr\n" \
"wQPdWyRdiwlGsVNjEhrxBqm6UbDIjph5GioPaB+0LLIMGnBBmqVfMhrADauZv/eM\n" \
"Miqp7eDMXZipLrOBniI1xudd\n" \
"-----END PRIVATE KEY-----\n";



// Definiera din serveradress
const char* serverName = "https://192.168.0.103"; // Din NGINX-serveradress

// Skapa en instans av WiFiClientSecure för mTLS
WiFiClientSecure client;

// Konfigurera sensorerna
void setup() {
    Serial.begin(115200);
    dht.begin();

    // Anslut till WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Ansluter till WiFi...");
    }
    Serial.println("Ansluten till WiFi!");

    // Installera certifikaten för mTLS
    client.setCACert(ca_cert);
    client.setCertificate(client_cert);
    client.setPrivateKey(client_key);
}

void loop() {
    // Läs temperatur och luftfuktighet
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    // Kontrollera om läsningarna misslyckades
    if (isnan(h) || isnan(t)) {
        Serial.println("Misslyckades med att läsa från DHT-sensorn!");
        return;
    }

    // Skapa en JSON-sträng för att skicka data
    String jsonData = String("{\"temperature\":") + t + ",\"humidity\":" + h + "}";

    // Skicka data till servern
    if (client.connect(serverName, 443)) {
        Serial.println("Ansluten till server!");

        // Skicka HTTP POST-förfrågan
        client.println("POST /data HTTP/1.1");
        client.println("Host: 192.168.0.103");
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(jsonData.length());
        client.println();
        client.println(jsonData);

        // Läs serverns svar
        while (client.connected() || client.available()) {
            if (client.available()) {
                String line = client.readStringUntil('\n');
                Serial.println(line);
            }
        }

        client.stop();
        Serial.println("Framgångsrikt skickad data!");
    } else {
        Serial.println("Anslutning till server misslyckades!");
    }

    delay(5000); // Vänta 5 sekunder innan nästa läsning
}
