//Includes
#include "Particle.h"

// Defines
#define BAUD_RATE           9600  //bps
#define SERIAL_TIMEOUT      500 //ms
#define BUFFER_SIZE         60 //bytes

struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};

class DustSensor {

public:
  DustSensor(void);

  void begin();
  bool listen();
  pms5003data readData();

private:
  struct pms5003data data;
};
