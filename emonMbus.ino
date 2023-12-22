/*
 * MBUS master for Arduino MBUS Master Shield 
 * https://www.hwhardsoft.de/english/projects/m-bus-mkr-shield/
 * 
 * Version 2.2
 * Copyright (C) 2023  Hartmut Wendt  www.zihatec.de
 * 
 * 
 * External library "MBusinoLib" needed: https://github.com/Zeppelin500/MBusinoLib
 * 
 * Based on OpenEnergyMonitor code by Trystan Lea, Glyn Hudson, and others
 * https://github.com/openenergymonitor/HeatpumpMonitor
 * Copyright 2018, Bryan McLellan <btm@loftninjas.org>
 * 
 * License: GPLv3 https://www.gnu.org/licenses/gpl.txt
 */

#define DEBUG true

#include "MBusinoLib.h"
#include "ArduinoJson.h"


// Serial interface used for mbus to allow use of 8E1 encoding
#include <HardwareSerial.h>
HardwareSerial *customSerial;


#define MBUS_BAUD_RATE 2400   // slave baudrate
#define MBUS_ADDRESS 2        // slave address
#define MBUS_TIMEOUT 1000     // milliseconds
#define MBUS_DATA_SIZE 255
#define MBUS_GOOD_FRAME true
#define MBUS_BAD_FRAME false
#define JSON_DATA_SIZE 2000


unsigned long loop_start = 0;
unsigned long last_loop = 0;
bool firstrun = true;
int Startadd = 0x13;      // Start address for decoding


void setup() {
  Serial.begin(115200);
  delay(1500);
  Serial.println(F("emonMbus startup"));
  Serial.println(F("mbus:"));
  Serial.print(F("  slave address: "));
  Serial.println(MBUS_ADDRESS);
  Serial.print(F("  baud rate: "));
  Serial.println(MBUS_BAUD_RATE);
  customSerial = &Serial1; 
  customSerial->begin(MBUS_BAUD_RATE, SERIAL_8E1); // mbus uses 8E1 encoding
  delay(1000); // let the serial initialize, or we get a bad first frame
}

void loop() {
  loop_start = millis();
  
  /************************
   * DATA COLLECTION LOOP *
   ************************/
  if ((loop_start-last_loop)>=9800 || firstrun) { // 9800 = ~10 seconds
    last_loop = loop_start; firstrun = false;

    /*************
     * MBUS DATA *
     *************/
    bool mbus_good_frame = false;
    byte mbus_data[MBUS_DATA_SIZE] = { 0 };
    
    mbus_normalize(0xFD);
    delay(500);
    mbus_normalize(0xFD);
    delay(200);
    if (DEBUG) Serial.print(F("mbus: requesting data from address: "));
    if (DEBUG) Serial.println(MBUS_ADDRESS);
    mbus_request_data(MBUS_ADDRESS);
    mbus_good_frame = mbus_get_response(mbus_data, sizeof(mbus_data));
    if (mbus_good_frame) {
      if (DEBUG) Serial.println(F("mbus: good frame: "));
      if (DEBUG) print_bytes(mbus_data, sizeof(mbus_data));

      int packet_size = mbus_data[1] + 6; 
      Serial.println(F("Creating payload buffer..."));
      MBusinoLib payload(JSON_DATA_SIZE);
      Serial.print(F("Packet size: ")); Serial.println(packet_size);

      Serial.print(F("Start Address: ")); Serial.println(Startadd);
      
      Serial.println(F("Decoding..."));
      DynamicJsonDocument jsonBuffer(2048);
      JsonArray root = jsonBuffer.createNestedArray();  
      uint8_t fields = payload.decode(&mbus_data[Startadd], packet_size - Startadd - 2, root); 
      serializeJsonPretty(root, Serial);

      Serial.println();
      Serial.print("Detected data fields: ");
      Serial.println(fields);
      
      Serial.print("Detected error: "); 
      Serial.println(payload.getError());

      Serial.println();
      for (uint8_t i=0; i<fields; i++) {
          float value = root[i]["value_scaled"].as<float>();
          uint8_t code = root[i]["code"].as<int>();
          const char* name = root[i]["name"];
          Serial.print("Field "); Serial.print(i+1); 
          //Serial.print(" ("); Serial.print((char *) payload.getCodeName(code)); 
          Serial.print(" ("); Serial.print(name); 
          Serial.print("): ");
          Serial.print(value); Serial.print(" "); Serial.print((char *) payload.getCodeUnits(code));
          Serial.println();
      }

      Serial.println();
    } else {
      Serial.print(F("mbus: bad frame: "));
      print_bytes(mbus_data, sizeof(mbus_data));
    }
  }
}
