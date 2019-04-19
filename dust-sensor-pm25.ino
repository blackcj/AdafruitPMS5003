// This #include statement was automatically added by the Particle IDE.
#include "DustSensor.h"

char resultstr[128]; // This is where we will put the data
DustSensor dustSensor;

void setup() {
    Particle.publish("log", "Air Quality Start");
    Particle.variable("result", resultstr, STRING);
    dustSensor.begin();
}

// Start out at 90 so our first reading is logged quickly
int i = 90;
// Have we found data this iteration
bool foundData = false;
// Where the found data is stored
pms5003data data;
// Used to compare against the last capture, the higher number is kept
pms5003data tempData;

/**
 * I'm only interested in the highest value found. Something like solder
 * will only trigger poor air quality for a short period of time.
 */
void loop() {
    i = i + 1;
    // .listen() returns true if data was captured
    if(dustSensor.listen()) {
        if(foundData == true) {
            // If we've already found data, keep the higher number.
            // TODO: Keep the higher number for each size.
            tempData = dustSensor.readData();
            if(tempData.pm10_standard > data.pm10_standard || tempData.pm25_standard > data.pm25_standard) {
                data = tempData;
            }
        } else {
            data = dustSensor.readData();
        }

        foundData = true;
    } // else do nothing, no data was captured
    // Every 10 seconds, publish the data
    if(i >= 100) {
        i = 0;
        if(foundData) {
            // Reset found data to false to allow for the reading.
            foundData = false;
            Particle.publish("log", "Found data!");
            // TODO: Send all data from struct
            sprintf(resultstr, "{\"pm10\":%i,\"pm25\":%i,\"pm100\":%i,\"checksum\":%i}", data.pm10_standard, data.pm25_standard, data.pm100_standard, data.checksum);
            Particle.publish("reading", resultstr);
        } else {
            // In 100 tries, no data was captured. There is likely a problem
            // with the wiring or too long of a delay between readings.
            Particle.publish("log", "No data available.");
        }
    }
    // Short delay before next reading. When this is too high, no data was
    // being captured. Keep it low or update the PMS5003 code to support
    // running in the background.
    delay(100);
}
