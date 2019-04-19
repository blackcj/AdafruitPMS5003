// This example is a modified version of the code available for Arduino on
// the Adafruit website:
// https://learn.adafruit.com/pm25-air-quality-sensor/arduino-code

// TODO: Add support for putting sensor to sleep.

// WIRING
// VCC: The PMS5003 sensor requires 5v to power the fan. Plug it into VIN
// on the Particle Photon to get the 5v.
// TX: (transmit) on the PMS5003 should be connected to RX (receive) on the Photon
// GND: GND

// Includes
#include "DustSensor.h"

DustSensor::DustSensor(void) {

}

void DustSensor::begin() {
  Serial1.begin(BAUD_RATE);
  Serial1.setTimeout(SERIAL_TIMEOUT);
}

pms5003data DustSensor::readData() {
  return data;
}

boolean DustSensor::listen() {

  // check if serial has data
  if (!Serial1.available()) {
    return false;
  }

  if (Serial1.peek()  != 0x42) {
    Serial1.read();
    return false;
  }
  if (Serial1.available() < 32) {
    return false;
  }
  // read serial port into buffer
  // break if we haven't totally filled the buffer
  uint8_t buffer[32];
  uint16_t sum = 0;
  if (!Serial1.readBytes((char*)buffer, 32)) {
    return false;
  }

  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }

  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);

  if (sum != data.checksum) {
    return false;
  }

  // success!
  return true;
}
