# Använd en officiell Python-bild som bas
FROM python:3.9-slim

# Installera nödvändiga verktyg för att kompilera C++
RUN apt-get update && \
    apt-get install -y g++ make && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Sätt arbetskatalogen
WORKDIR /app

# Kopiera Python-koden och installera beroenden
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Kopiera C++-koden
COPY sensor.cpp .

# Kopiera Python-koden
COPY client.py .

# Exponera porten för Flask
EXPOSE 5000

# Kompilera C++-koden (justera efter behov)
# Kompilera DHTSensor.ino om det behövs, beroende på din C++-miljö
# RUN g++ DHTSensor.ino -o DHTSensor  # Exempel, om DHTSensor är en ren C++-fil

# Starta Python-applikationen
CMD ["python", "client.py"]
