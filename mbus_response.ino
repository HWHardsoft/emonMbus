/*
 * mbus communication functions for emonMbus
 * Based on OpenEnergyMonitor code by Trystan Lea, Glyn Hudson, and others
 * https://github.com/openenergymonitor/HeatpumpMonitor
 * Copyright 2018 Bryan McLellan <btm@loftninjas.org>
 * License: GPLv3 https://www.gnu.org/licenses/gpl.txt
 */
#include <HardwareSerial.h>
 

bool mbus_get_response(byte *pdata, unsigned char len_pdata) {
  byte bid = 0;             // current byte of response frame
  byte bid_end = 255;       // last byte of frame calculated from length byte sent
  byte bid_checksum = 255;  // checksum byte of frame (next to last)
  byte len = 0;
  byte checksum = 0;
  bool long_frame_found = false;
  bool complete_frame  = false;
  bool frame_error = false;
 
  unsigned long timer_start = millis();
  while (!frame_error && !complete_frame && (millis()-timer_start) < MBUS_TIMEOUT)
  {
    while (customSerial->available()) {
      byte received_byte = (byte) customSerial->read();

      // Try to skip noise
      if (bid == 0 && received_byte != 0xE5 && received_byte != 0x68) {
        if (DEBUG) Serial.print(F(">"));
        continue;
      }
      
      if (bid > len_pdata) {
        Serial.print("mbus: error: frame length exceeded variable size of ");
        Serial.println(len_pdata);
        return MBUS_BAD_FRAME;
      }
      pdata[bid] = received_byte;

      // Single Character (ACK)
      if (bid == 0 && received_byte == 0xE5) {
        if (DEBUG) Serial.println(F("mbus: single character (ack)"));
        return MBUS_GOOD_FRAME;
      }
      
      // Long frame start
      if (bid == 0 && received_byte == 0x68) {
        if (DEBUG) Serial.println(F("mbus: start long frame"));
        long_frame_found = true;
      }

      if (long_frame_found) {
        // 2nd byte is the frame length
        if (bid == 1) {
          len = received_byte;
          bid_end = len+4+2-1;
          bid_checksum = bid_end-1;
        }
            
        if (bid == 2 && received_byte != len) {                          // 3rd byte is also length, check that its the same as 2nd byte
          if (DEBUG) Serial.println(F("mbus: frame length byte mismatch"));
          frame_error = true;
        }
        if (bid == 3 && received_byte != 0x68) {        ;                // 4th byte is the start byte again
          if (DEBUG) Serial.println(F("mbus: missing second start byte in long frame"));
          frame_error = true;
        }
        if (bid > 3 && bid < bid_checksum) checksum += received_byte;    // Increment checksum during data portion of frame
        
        if (bid == bid_checksum && received_byte != checksum) {          // Validate checksum
          if (DEBUG) Serial.println(F("mbus: frame failed checksum"));
          frame_error = true;
        }
        if (bid == bid_end && received_byte == 0x16) {                   // Parse frame if still valid
          complete_frame  = true;
        }
      }
      bid++;
    }
  }

  if (complete_frame && !frame_error) {
    // good frame
    return MBUS_GOOD_FRAME;
  } else {
    // bad frame
    return MBUS_BAD_FRAME;
  }
}


// Prints a whole response as a string for debugging
void print_bytes(byte *bytes, unsigned char len_bytes) {
  for (int i = 0; i < len_bytes; i++) {
    Serial.print(String(bytes[i], HEX));
    Serial.print(F(" "));
  } 
  Serial.println();
}
