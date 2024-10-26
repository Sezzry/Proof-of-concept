#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFiClientSecure.h>

// Definiera DHT11-sensorns pinne
#define DHTPIN 0      // Pin som är ansluten till DHT11-data-pinnen
#define DHTTYPE DHT11 // DHT 11 sensor

DHT dht(DHTPIN, DHTTYPE);

// Definiera dina WiFi-inloggningsuppgifter
const char* ssid = "Tele2_9cae3d";       // Din WiFi SSID
const char* password = "cjmzjywy"; // Ditt WiFi-lösenord

// CA-certifikat (konverterat från den givna texten)
const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEKzCCAxOgAwIBAgIUQTSOEFs401w1erBfSrPkYuOa/YgwDQYJKoZIhvcNAQEL\n" \
"BQAwgaQxCzAJBgNVBAYTAlNFMRIwEAYDVQQIDAlTdG9ja2hvbG0xEjAQBgNVBAcM\n" \
"CVN0b2NraG9sbTETMBEGA1UECgwKTmFja2FkZW1pbjETMBEGA1UECwwKTmFja2Fk\n" \
"ZW1pbjESMBAGA1UEAwwJbG9jYWxob3N0MS8wLQYJKoZIhvcNAQkBFiBzYXJhbi5r\n" \
"b25ndGhvbmdAeWgubmFja2FkZW1pbi5zZTAeFw0yNDEwMjQyMDU5MTJaFw0yNzA4\n" \
"MTQyMDU5MTJaMIGkMQswCQYDVQQGEwJTRTESMBAGA1UECAwJU3RvY2tob2xtMRIw\n" \
"EAYDVQQHDAlTdG9ja2hvbG0xEzARBgNVBAoMCk5hY2thZGVtaW4xEzARBgNVBAsM\n" \
"Ck5hY2thZGVtaW4xEjAQBgNVBAMMCWxvY2FsaG9zdDEvMC0GCSqGSIb3DQEJARYg\n" \
"c2FyYW4ua29uZ3Rob25nQHloLm5hY2thZGVtaW4uc2UwggEiMA0GCSqGSIb3DQEB\n" \
"AQUAA4IBDwAwggEKAoIBAQD1FZP+L300GQq6eD1z8qtfDj3F6YU2ebqJepy5Zc4W\n" \
"nB6Q/LaZ8N4vYk7Jo3bnVYgaQLnxz6ZVkrF46PNks1Pcb2vy7ySlEA0xf1JXzPK+\n" \
"e20efhqZQUCcnRQfA03oGokFKn/0I21r+te3yK0zAePWbXAgKkkJ3HiQc/RHQdyE\n" \
"g6IFTtk6V6IqNernWyhKvXIrdHCs1Z7NxoxiGr6fyUmz7XSGvboE2ko6+1AoG+r4\n" \
"7jZouyohuW8j6Q1z8ZW3OmyVC7iQiVq5Zf6RTdInAXtLr/4O5Spr+uk/glbOgAh9\n" \
"DWYz4iQK8MvGl3bnDfBGPNwZiiI/q/JdLfP4uWjJgp4NAgMBAAGjUzBRMB0GA1Ud\n" \
"DgQWBBTRLyII97sSH9w0E0jYYY+QapMh9zAfBgNVHSMEGDAWgBTRLyII97sSH9w0\n" \
"E0jYYY+QapMh9zAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4IBAQCz\n" \
"QomztatOg/Whi7TQin5cAekHTZmRUE9tpO577Nl3DkfiQqDduAO5O2KEkmpbL017\n" \
"4AuwYgs4h7PDT8V4uEB0NlSA+qXcZ5Vj4r1BGm7UfoSP1Hhb/6rtkIdmGixQ/wAu\n" \
"y1cFbZfJyhr8sDP1yKww9J+LVlefs/ofTLMdtxLk4rSBs7beQ+YPe9EgMN03udE+\n" \
"dUni1jBrCQ4vhk1w6mVfXedChp2hmtrCp/r78laJKzdpTLbtYBlgMhmcmNEaJWhC\n" \
"bjYb2E78/wpHYxP2p38JKDFFO3scwGKTD1De/WFZi9ZcHH+gMRMJbBcgcrBzmnNg\n" \
"FEZfu7Jxl3GOBfqTOWPc\n" \
"-----END CERTIFICATE-----\n";

// Klientens certifikat (exempel, fyll i ditt egna certifikat)
const char* client_cert = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIGjCCAwKgAwIBAgIUfwfQ7RKGVcN+w5WP2pLMaNHE+LowDQYJKoZIhvcNAQEL\n" \
"BQAwgaQxCzAJBgNVBAYTAlNFMRIwEAYDVQQIDAlTdG9ja2hvbG0xEjAQBgNVBAcM\n" \
"CVN0b2NraG9sbTETMBEGA1UECgwKTmFja2FkZW1pbjETMBEGA1UECwwKTmFja2Fk\n" \
"ZW1pbjESMBAGA1UEAwwJbG9jYWxob3N0MS8wLQYJKoZIhvcNAQkBFiBzYXJhbi5r\n" \
"b25ndGhvbmdAeWgubmFja2FkZW1pbi5zZTAeFw0yNDEwMjQyMTAxNDdaFw0yNjAz\n" \
"MDgyMTAxNDdaMIGkMQswCQYDVQQGEwJTRTESMBAGA1UECAwJU3RvY2tob2xtMRIw\n" \
"EAYDVQQHDAlTdG9ja2hvbG0xEzARBgNVBAoMCk5hY2thZGVtaW4xEzARBgNVBAsM\n" \
"Ck5hY2thZGVtaW4xEjAQBgNVBAMMCWxvY2FsaG9zdDEvMC0GCSqGSIb3DQEJARYg\n" \
"c2FyYW4ua29uZ3Rob25nQHloLm5hY2thZGVtaW4uc2UwggEiMA0GCSqGSIb3DQEB\n" \
"AQUAA4IBDwAwggEKAoIBAQC5wM2ROUMO1rPOxNwaHnzTJ8/MTNaKqzVaZQUBboAp\n" \
"2X0MzHdAPiCXWNARiT7ln95D03+DXIgwnWLYLB2vKNYlqQ6OYjV4juR2yP7ykP2j\n" \
"RF26nrabx07xc3CrWjolP7bupfRS4oSss+wuuLMvpHFMPMAEzIXIs+aYIMQavxHM\n" \
"V1EMU8Wmd9B4cwQXOFCE33Rgn82ypctOoX9AswPyBZsWAyhfwLW+O+p6UGLb7Rjt\n" \
"WZr8uPPknn/Q7uqv8liESUh1r2ClPlcE/kun5SSKYYTXRdu2ArjNfcF15z3BVCdR\n" \
"1r2bz7grk1D0P4aGx412V+NFHeEIXePje+K6O5IJQSNJAgMBAAGjQjBAMB0GA1Ud\n" \
"DgQWBBRp7tdS5kNjWUPlKmWKxuyVIyfpPzAfBgNVHSMEGDAWgBTRLyII97sSH9w0\n" \
"E0jYYY+QapMh9zANBgkqhkiG9w0BAQsFAAOCAQEAwXDM56YefwnVzg4HVb5be4i7\n" \
"lcwTQO/1gGHdbEIHP5VdMMYXm40DGBWgHExLxHI8mfND+q0uwvwu/kc2l8cLE5Bv\n" \
"4+a6oYwi3HvMtHKTrnR6MH1WUXU+UREviy+8PLUhgyer6yF8Twm20Em1vVcbWIXW\n" \
"iRGAQHXJwGej04jtL6gHaODKElPblCSBf8GLjAB+VUINJ2I/vzgcc/gdE55D2I6e\n" \
"AKAujsQhPx37gjWoadXB4LdMhqTY3tAmLaV1DoLjSRHtyy2Z1TYQXwnLxNM56L4D\n" \
"/1+xjD6Hr3XOfc4ew+u1INCOMEuB6QJGZ6yCyhSprXOZPyIhS++V88eHIDEEXw==\n" \
"-----END CERTIFICATE-----\n";

// Klientens nyckel (exempel, fyll i din egen nyckel)
const char* client_key = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQC5wM2ROUMO1rPO\n" \
"xNwaHnzTJ8/MTNaKqzVaZQUBboAp2X0MzHdAPiCXWNARiT7ln95D03+DXIgwnWLY\n" \
"LB2vKNYlqQ6OYjV4juR2yP7ykP2jRF26nrabx07xc3CrWjolP7bupfRS4oSss+wu\n" \
"uLMvpHFMPMAEzIXIs+aYIMQavxHMV1EMU8Wmd9B4cwQXOFCE33Rgn82ypctOoX9A\n" \
"swPyBZsWAyhfwLW+O+p6UGLb7RjtWZr8uPPknn/Q7uqv8liESUh1r2ClPlcE/kun\n" \
"5SSKYYTXRdu2ArjNfcF15z3BVCdR1r2bz7grk1D0P4aGx412V+NFHeEIXePje+K6\n" \
"O5IJQSNJAgMBAAECggEAAkoXzocp5iwrxavj+ZdwGusl8sLsrwNasj1GAUVDI6Do\n" \
"W2jGvH43iR4BXIH4Sjy88NGRxAT3McAy2G68w+ypLQgnI4ae2ZhLRRKHn+Y9Fg7Q\n" \
"5+yuW/+qH6w4UohXaJ+fKdRWTVI45+cEzXOd08O/WLzz8esyZ+2dtTjMKfc6aS4R\n" \
"455VnIKcXT9WusXLAG+Gv9ZN5qK3Bn8jKWLi9aaoBpKmuroXwsvWtxNb4nqw2rZi\n" \
"8z1pzautcjCnymvFm/XgMxzCOYKOITygzLPaGn39yUJvYhDau29xE225BA/ggvL9\n" \
"ZUHDAsKGeuKn4p66Q9/OGzy2Rtq9dG7irIKBoeXwoQKBgQD+KWG+tlywrk9qZSox\n" \
"amLU6NB8bjwG5i/tZH1fTOcwM0bNz73mHe7i1GmKJaG48JpTzrwjK7Zj3CMBSaVv\n" \
"EYP+mPDrL48iPd+CfhsvVAQ1qUtAdaExcaPUKRQPObENxWJOj1lyPim+dBBwwXis\n" \
"Urr7p8hpFzz7Zs9njd1HOk5CeQKBgQC7GMCr8veWruXitmfiVsYXjGSspCsfYQYA\n" \
"TX8j9alV0QLGW3T9JAhpNdf2ok3N/hjWoSfTJuMo6cBFSPbvZXbgfKXyzKwSYKuR\n" \
"/ezhsPx6cLOl8pQkn2GmCDG5QTvB5Lglwn0Al5ecCXNgeo5hiujlENVEGlL22jf5\n" \
"vZ+dKUAzUQKBgFzhppTZc/olWWRaYBb69+8yva4bYthqmq04ghCnu/0DYCaqeo1z\n" \
"9u7yDTetFPLPCrcB4ANCYD8SHcDEty5lykTqg3CPStGKpvm7OBgNrhQmMBf4ZNX6\n" \
"35ITCP+fZ7LOpWRsQpXbXcn6BGAQdBcVjLuWz0iIc4K0SatgTrNO/XaRAoGAYinF\n" \
"hTIQRyXyhXWwoadnfZlyhMYJvgf4zxXZ/DL7KoRkEVCQ2/QY2aKfcc6MReTogUvC\n" \
"il2f16p076UtskMpEPU1WGOccHP58Loqm+txPIh/KTVSFrjs0eEAvbMFOGDZYS5e\n" \
"yX2/1OV8fIUwyOrJ8M1ZJ8cr+kpaDhU4dovbHFECgYBcLeYvy8ppd3u9u6k2kwGc\n" \
"NE6dATUCuScN/7YsuKQeGYELmtql++I7JQf0kUmyQy7AHfF8xp/yFf1daDtAOech\n" \
"v6A+6kWa4Z/ZmC8WHP8ZjrKn6AsEnvZaZ4Lr6GBKqrsB8hFMiwgaKhE94DQ2As+c\n" \
"NXTdAV9veTzIR8kBEVf4Pw==\n" \
"-----END PRIVATE KEY-----\n";

void setup() {
  // Starta seriell kommunikation
  Serial.begin(115200);
  
  // Initiera DHT-sensorn
  dht.begin();

  // Anslut till WiFi-nätverket
  Serial.print("Försöker ansluta till WiFi-nätverket: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  // Vänta tills en anslutning har etablerats
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // När anslutningen är framgångsrik
  Serial.println("");
  Serial.println("WiFi anslutet!");
  Serial.print("IP-adress: ");
  Serial.println(WiFi.localIP());

  // Initiera WiFiClientSecure
  WiFiClientSecure client;
  client.setCACert(ca_cert);        // Sätt CA-certifikat
  client.setCertificate(client_cert); // Sätt klientcertifikat
  client.setPrivateKey(client_key);   // Sätt klientnyckel

  // Här kan du göra en HTTPS-förfrågan, exempelvis:
  if (!client.connect("your.server.com", 443)) { // Ersätt med din servers domän
    Serial.println("Anslutning misslyckades!");
    return;
  }

  // Gör en GET-förfrågan
  client.println("GET /path/to/resource HTTP/1.1");
  client.println("Host: your.server.com");
  client.println("Connection: close");
  client.println();

  // Vänta på svar
  while (client.connected() || client.available()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }
}

void loop() {
  // Vänta 2 sekunder mellan mätningar
  delay(2000);

  // Läs temperaturen och luftfuktigheten från sensorn
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Kontrollera om något gick fel vid avläsningen
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Misslyckades att läsa från DHT-sensorn!"));
    return;
  }

  // Skriv ut värdena till seriella monitorn
  Serial.print(F("Luftfuktighet: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperatur: "));
  Serial.print(temperature);
  Serial.println(F("°C"));
}
