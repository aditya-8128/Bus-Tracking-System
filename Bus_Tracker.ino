// =================================================================
//         IIT Bhubaneswar Live Bus Tracking System Firmware
// =================================================================
// Project:     IITBBS Live Bus Tracker (Backend & IoT Hardware)
// Author:      [Your Name]
// Date:        October 2025
// Description: This firmware is designed for an ESP32-based IoT unit
//              (specifically the TTGO T-Call board) to track the
//              real-time location of campus buses. It reads GPS data,
//              connects to the cellular network, and transmits the
//              location to the Blynk IoT cloud for backend processing
//              and monitoring.
// =================================================================

// --- LIBRARIES ---
// Define necessary libraries for Blynk, the GSM modem, and the GPS module.
#define BLYNK_PRINT Serial
#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <BlynkSimpleEsp32_SIM800.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

// =================================================================
// ---               START OF USER CONFIGURATION                 ---
// =================================================================

// STEP 1: BLYNK AUTHENTICATION TOKEN
// Paste the unique Auth Token for each device from the Blynk Console.
// Each of the 6 buses will have a DIFFERENT token.
char auth[] = "PASTE_YOUR_BLYNK_AUTH_TOKEN_HERE";

// STEP 2: UNIQUE DEVICE IDENTIFIER
// Set a unique name for this specific hardware unit.
const char* BUS_ID = "Bus_1"; // e.g., "Bus_2", "Bus_3", etc.

// STEP 3: GPRS/CELLULAR NETWORK CREDENTIALS
// Update the APN (Access Point Name) for your SIM card provider.
const char apn[]  = "jionet"; // Common APNs: "jionet" (Jio), "airtelgprs.com" (Airtel)
const char user[] = "";       // Leave blank if not required by your provider.
const char pass[] = "";       // Leave blank if not required by your provider.

// =================================================================
// ---                END OF USER CONFIGURATION                  ---
// =================================================================


// --- HARDWARE PIN DEFINITIONS ---
// These are specific to the TTGO T-Call v1.4 board.
// Do not change these unless you are using a different board.

// For the SIM800L Modem
#define SerialAT Serial1
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

// For the NEO-6M/7M GPS Module
// We will use the ESP32's second hardware serial port (Serial2).
// Connect GPS TX to ESP32 RX2 and GPS RX to ESP32 TX2.
#define GPS_RX_PIN 2
#define GPS_TX_PIN 15

// --- GLOBAL OBJECTS ---
TinyGsm modem(SerialAT);
TinyGPSPlus gps;
BlynkTimer timer;
HardwareSerial gpsSerial(2); // Use hardware serial port 2

// Function to send validated GPS data to the Blynk cloud.
void sendSensorData() {
    // Check if the GPS has a valid location fix and the data is recent.
    if (gps.location.isValid() && gps.location.age() < 2000) {
        float latitude = gps.location.lat();
        float longitude = gps.location.lng();

        Serial.print("Sending Data to Blynk Cloud -> Lat: ");
        Serial.print(latitude, 6);
        Serial.print(", Lon: ");
        Serial.println(longitude, 6);

        // Send GPS coordinates to Blynk Virtual Pin V0 (Location Widget data type).
        Blynk.virtualWrite(V0, latitude, longitude);

        // Send the unique bus identifier to Blynk Virtual Pin V1.
        Blynk.virtualWrite(V1, BUS_ID);

    } else {
        Serial.println("Waiting for valid GPS fix...");
    }
}

void setup() {
    // Start the serial monitor for debugging.
    Serial.begin(115200);
    delay(100);

    // Start serial communication with the GPS module.
    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX_PIN, GPS_TX_PIN);

    // Power on and configure the SIM800L modem.
    Serial.println("Initializing modem...");
    pinMode(MODEM_PWKEY, OUTPUT);
    pinMode(MODEM_RST, OUTPUT);
    pinMode(MODEM_POWER_ON, OUTPUT);
    digitalWrite(MODEM_PWKEY, LOW);
    digitalWrite(MODEM_RST, HIGH);
    digitalWrite(MODEM_POWER_ON, HIGH);
    delay(1000);
    digitalWrite(MODEM_PWKEY, HIGH);

    // Start serial communication with the modem.
    SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
    delay(3000);
    
    // Restart the modem to ensure a clean state.
    modem.restart();

    // Establish connection to the cellular network.
    Serial.println("Waiting for network...");
    if (!modem.waitForNetwork()) {
        Serial.println(" Network connection failed.");
        delay(10000);
        return;
    }
    Serial.println(" Network connected.");

    // Establish GPRS connection.
    if (!modem.gprsConnect(apn, user, pass)) {
        Serial.println(" GPRS connection failed.");
        delay(10000);
        return;
    }
    Serial.println(" GPRS connected.");

    // Initialize the connection to the Blynk cloud.
    Blynk.begin(auth, modem, apn, user, pass);
    Serial.println("Blynk connection established.");

    // Set up a recurring timer to call the sendSensorData function every 10 seconds.
    timer.setInterval(10000L, sendSensorData);
}

void loop() {
    // Continuously process incoming GPS data.
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    // Keep the Blynk connection alive and run timers.
    Blynk.run();
    timer.run();
}
