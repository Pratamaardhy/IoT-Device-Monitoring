//Projek Fiks ditaruh di file ini

#define TINY_GSM_MODEM_SIM7600  // Pastikan ini didefinisikan sebelum mengimpor library

#include <TinyGsmClient.h>
#include <ThingSpeak.h>
#include <Wire.h>

#define TINY_GSM_MODEM_SIM7600
#define SerialMon Serial
#define MODEM_RST 5
#define MODEM_PWRKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26

// Your GPRS credentials
const char apn[]  = "telkomsel"; // Telkomsel APN
const char user[] = "";
const char pass[] = "";

const char* server = "api.thingspeak.com";
const char* api_key = "DJOGXUUY0XUDPJ70"; // Your Write API Key
unsigned long channel_id = 2635960; // Your Channel ID

TinyGsm modem(Serial1);
TinyGsmClient client(modem);

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Start the modem reset and turn on
  pinMode(MODEM_PWRKEY, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);

  digitalWrite(MODEM_PWRKEY, LOW);
  delay(100);
  digitalWrite(MODEM_PWRKEY, HIGH);
  delay(1000);
  digitalWrite(MODEM_PWRKEY, LOW);

  // Set GSM module baud rate
Serial1.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart and initialize modem
  SerialMon.println("Initializing modem...");
  modem.restart();

  // Connect to GPRS
  SerialMon.print("Connecting to ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println(" fail");
    while (true);
  }
  SerialMon.println(" success");

  int signalQuality = modem.getSignalQuality();
  SerialMon.print("Signal quality: ");
  SerialMon.println(signalQuality);


  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  if (!modem.isGprsConnected()) {
    SerialMon.println("GPRS disconnected! Reconnecting...");
    modem.gprsConnect(apn, user, pass);
  }

  // Simulasi status ON/OFF dari Arduino
  int status = random(0, 2); // Menghasilkan 0 (OFF) atau 1 (ON)
  int number = (status == 1) ? random(20, 80) : 0; // Jika ON: antara 20-80, jika OFF: 0

  int x = ThingSpeak.writeField(channel_id, 1, number, api_key);
  if (x == 200) {
    SerialMon.println("Channel update successful.");
  } else {
    SerialMon.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(5000); // Wait 5 seconds before next update
}