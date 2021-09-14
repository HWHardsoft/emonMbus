/*
 * MBUS master for Arduino MKR MBUS Shield 
 * https://www.hwhardsoft.de/english/projects/m-bus-mkr-shield/
 * 
 * Version 1.0
 * Copyright (C) 2021  Hartmut Wendt  www.zihatec.de
 * 
 * 
 * Based on OpenEnergyMonitor code by Trystan Lea, Glyn Hudson, and others
 * https://github.com/openenergymonitor/HeatpumpMonitor
 * 
 * and Bryan McLellan  <btm@loftninjas.org>
 * https://github.com/btm/emonMbus
 * 
 * License: GPLv3 https://www.gnu.org/licenses/gpl.txt
 */

#define DEBUG true


// Serial interface used for mbus to allow use of 8E1 encoding
// MKR Shield is using UART Serial1
#include <HardwareSerial.h>
HardwareSerial *customSerial;


#define MBUS_BAUD_RATE 2400
#define MBUS_ADDRESS 2
#define MBUS_TIMEOUT 1000 // milliseconds
#define MBUS_DATA_SIZE 255
#define MBUS_GOOD_FRAME true
#define MBUS_BAD_FRAME false

unsigned long loop_start = 0;
unsigned long last_loop = 0;
bool firstrun = true;
int Startadd = 17;

/* emonhub.conf configuration
  [[12]]
    nodename = emonmbus
    [[[rx]]]
       names = energy, flow, power, flowrate, supplyT, returnT
       scales = 1,1,1,1,0.01,0.01
       units = kW,kW,m,mm,C,C
 */
typedef struct {
  int energy, flow, power, flowrate, supplyT, returnT ;
} PayloadTX;

PayloadTX emon_data_tx;

void setup() {
  Serial.begin(115200);
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

    if (DEBUG) Serial.print(F("mbus: requesting data from address: "));
    if (DEBUG) Serial.println(MBUS_ADDRESS);
    mbus_request_data(MBUS_ADDRESS);
    mbus_good_frame = mbus_get_response(mbus_data, sizeof(mbus_data));
    if (mbus_good_frame) {
      if (DEBUG) Serial.println(F("mbus: good frame: "));
      if (DEBUG) print_bytes(mbus_data, sizeof(mbus_data));


      // change this section for your M-Bus device:
      //
      // SMTenergy 6h 27b, SMTflowrate 3Bh 44b, SMTflowT 59h 49b, SMTreturnT 5Dh 53b, SMTflow 14h 33b, SMTheat 2Ch 39b   
      emon_data_tx.energy = get_spire_value(mbus_data, 27, 4);
      emon_data_tx.flow = get_spire_value(mbus_data, 33, 4);
      emon_data_tx.power = get_spire_value(mbus_data, 39, 3);
      emon_data_tx.flowrate = get_spire_value(mbus_data, 44, 3);
      emon_data_tx.supplyT = get_spire_value(mbus_data, 49, 2);
      emon_data_tx.returnT = get_spire_value(mbus_data, 53, 2);
      Serial.print(F("SMTenergy kWh: ")); Serial.println(emon_data_tx.energy);
      Serial.print(F("SMTflow: ")); Serial.println(emon_data_tx.flow);
      Serial.print(F("SMTpower: ")); Serial.println(emon_data_tx.power);
      Serial.print(F("SMTflowrate: ")); Serial.println(emon_data_tx.flowrate);
      Serial.print(F("SMTsupplyT: ")); Serial.println(emon_data_tx.supplyT);
      Serial.print(F("SMTreturnT: ")); Serial.println(emon_data_tx.returnT);
    
    } else {
      Serial.print(F("mbus: bad frame: "));
      print_bytes(mbus_data, sizeof(mbus_data));
    }
  }
}
