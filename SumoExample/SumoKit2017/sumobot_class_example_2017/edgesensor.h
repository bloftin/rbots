#ifndef SumoEdge_h
#define SumoEdge_h

#include <QTRSensors.h>

//edge sensor input pins
int EDGE_L = A0;
int EDGE_R = A1;

#define NUM_SENSORS   2    // number of sensors used
#define TIMEOUT       900  // waits for 2500 microseconds for sensor outputs to go low
#define EDGE_DETECT   500  // threshold where edge is detected

// sensors 0 through 7 are connected to digital pins 3 through 10, respectively
QTRSensorsRC qtrrc((unsigned char[]) {A0, A1}, NUM_SENSORS, TIMEOUT); 
unsigned int sensorValues[NUM_SENSORS];

void update_edge()
{
  // read raw sensor values
  qtrrc.read(sensorValues);
}

bool left_edge()
{   
  if(sensorValues[0] < EDGE_DETECT) return true;
  else return false;

  return false;
}

bool right_edge()
{ 
  if(sensorValues[1] < EDGE_DETECT) return true;
  else return false;

  return false;
}

#endif // SumoEdge_h
