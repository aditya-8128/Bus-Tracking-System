IIT Bhubaneswar Live Bus Tracking System
Overview
This project provides a real-time, live tracking system for the campus buses at IIT Bhubaneswar. In a large campus environment, bus schedules can be subject to delays due to traffic, weather, or other unforeseen circumstances. This system aims to solve the problem of uncertainty by providing students and faculty with a simple web-based map showing the exact, live location of every bus in the fleet.

By retrofitting each of the 6 campus buses with a low-cost IoT tracker unit, this system transmits GPS data over a cellular network to the Blynk IoT cloud. A publicly accessible web dashboard then visualizes the location of all buses simultaneously, eliminating guesswork and allowing users to perfectly time their arrival at the bus stop.

The core of this project is simplicity and reliability, leveraging the powerful Blynk platform to handle all the backend and frontend complexity, so we can focus on building robust hardware units.

Features
Real-Time Tracking: View the live location of all 6 campus buses with a refresh rate of every 5-10 seconds.

Web-Based Access: No app installation required. The tracking map is accessible via a simple URL on any device with a web browser (desktop or mobile).

Multi-Vehicle Support: The dashboard is configured to display multiple buses, each with a unique icon and identifier.

Low-Cost & Scalable: Each tracker unit is built with affordable, off-the-shelf components.

Reliable Connectivity: Uses a cellular network (GPRS/4G) to ensure consistent data transmission, independent of campus Wi-Fi.

Robust Power System: Powered directly by the bus's electrical system for set-and-forget operation.

System Architecture
The system operates on a simple yet powerful architecture. Each component is chosen to ensure reliability and ease of setup.

Tracker Unit (On Bus): An ESP32 microcontroller paired with a GPS module acquires satellite location data. A SIM module transmits this data over the cellular network.

Cellular Network: A standard mobile network (like Jio, Airtel, etc.) acts as the bridge to the internet.

Blynk IoT Cloud: This is the core of our system. It receives the data from all 6 tracker units, processes it, and serves it to the web dashboard. It handles all the server management, data storage, and user interface.

Web Dashboard: A simple web page configured within the Blynk platform that displays a map widget. This map fetches and displays the coordinates from all tracker units in real-time.

Hardware Requirements (per bus unit)
You will need to build 6 identical hardware units.

Component

Model Example

Quantity

Purpose

Microcontroller + Cellular

TTGO T-Call ESP32 SIM800L

1

The "brains" of the tracker with built-in cellular capability.

GPS Module

NEO-6M or NEO-7M

1

Acquires location coordinates from GPS satellites.

DC-DC Buck Converter

LM2596

1

Safely steps down the bus's 12V/24V power to a stable 5V for the ESP32.

SIM Card

Any provider (e.g., Jio, Airtel)

1

Provides internet connectivity. An IoT/M2M data plan is ideal.

Cellular Antenna

SMA/u.FL Stick Antenna

1

For the SIM800L module.

GPS Antenna

Active Ceramic Patch Antenna

1

For the NEO-6M module.

Project Enclosure

Non-metallic, IP65 rated

1

Protects electronics from dust, vibration, and moisture.

Connecting Wires

Jumper Wires / Soldering Kit

-

To connect all the components together.

Software & Services
Arduino IDE: To program the ESP32 microcontroller.

Blynk Account: The free plan is sufficient to get started and supports enough devices for this project.

Arduino Libraries:

BlynkSimpleEsp32_SIM800.h

TinyGPS++.h

SoftwareSerial.h

Step-by-Step Setup Guide
Phase 1: Setting up the Blynk Cloud
This is the first step, as you will need credentials from Blynk to program the hardware.

Create a Blynk Account: Sign up on the Blynk website.

Create a New Template:

Go to the "Templates" section and click "+ New Template".

Name: IITBBS Bus Tracker

Hardware: ESP32

Connection Type: WiFi (This setting doesn't matter much for our code, but it's a required field).

Click "Done".

Define Datastreams: Datastreams are the data channels from your hardware to the cloud.

Inside your new template, go to the "Datastreams" tab.

Create a New Datastream:

Name: GPS Coordinates

Virtual Pin: V0

Data Type: Location (This is a special data type in Blynk).

Click "Create".

Create another Datastream:

Name: Bus ID

Virtual Pin: V1

Data Type: String

Click "Create".

Create Devices: Now, create a device for each of the 6 buses.

Go to the "Search" icon (magnifying glass) on the left panel, then click "Devices".

Click "+ New Device".

Select "From template".

Choose your IITBBS Bus Tracker template.

Give it a name, for example, Bus 1.

Click "Create".

A window will pop up with your BLYNK_AUTH_TOKEN. COPY THIS AND SAVE IT! This is the unique key for this device.

Repeat this process 5 more times for Bus 2, Bus 3, etc., saving the unique Auth Token for each one.

Configure the Web Dashboard: This is the map students will see.

In your template, go to the "Web Dashboard" tab.

Drag a Map widget from the right-hand panel onto the dashboard.

Click the settings icon (wrench) on the Map widget.

Title: IITBBS Live Bus Map

Datastream: Select GPS Coordinates (V0).

Enable "Show all devices on map".

Enable "Show device name on hover".

Save the dashboard.

Phase 2: Hardware Assembly
⚠️ SAFETY WARNING: Before connecting to the bus, do all your testing on a workbench using a 12V power adapter. Incorrectly wiring the buck converter can damage your components.

Set the Buck Converter:

Connect a 12V power source to the IN+ and IN- terminals of the LM2596.

Use a multimeter on the OUT+ and OUT- terminals.

Turn the small screw on the blue potentiometer until the multimeter reads exactly 5.0V. This is crucial.

Wire the Components:

Solder connections for a reliable, vibration-proof setup.

BUCK CONVERTER   ->   TTGO T-CALL (ESP32)
-------------------------------------------
OUT+             ->   5V Pin
OUT-             ->   GND Pin

NEO-6M GPS       ->   TTGO T-CALL (ESP32)
-------------------------------------------
VCC              ->   3.3V Pin
GND              ->   GND Pin
TX Pin           ->   GPIO 2 (RX2)
RX Pin           ->   GPIO 15 (TX2)

SIM800L Module (on T-Call board)
-------------------------------------------
- Insert SIM card into the slot.
- Connect the cellular antenna to its u.FL connector.

GPS Module
-------------------------------------------
- Connect the ceramic GPS antenna to its u.FL connector.
