from influxdb_client import InfluxDBClient, Point
from influxdb_client.client.write_api import SYNCHRONOUS
import time

# Konfigurera din InfluxDB-instans
url = "http://localhost:8086"  # URL till din InfluxDB
token = "din_token"            # Ditt autentiseringstoken
org = "din_organisation"       # Din organisation
bucket = "din_bucket"          # Ditt bucket-namn

# Skapa en klient för att ansluta till InfluxDB
client = InfluxDBClient(url=url, token=token, org=org)

# Skapa en write API för att skriva data
write_api = client.write_api(write_options=SYNCHRONOUS)

# Skicka data i ett loop-exempel
for i in range(10):
    # Skapa en datapunkt
    point = Point("sensor_data") \
        .tag("sensor_id", "sensor_1") \
        .field("temperature", 20 + i) \
        .field("humidity", 60 - i) \
        .time(time.time_ns())  # Tid i nanosekunder

    # Skriv datapunkten till InfluxDB
    write_api.write(bucket=bucket, org=org, record=point)

    # Vänta 1 sekund innan nästa datapunkt skickas
    time.sleep(1)

print("Data skickad till InfluxDB!")

# Stäng anslutningen
client.close()