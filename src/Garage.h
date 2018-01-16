#ifndef GARAGE_H
#define GARAGE_H

#include <Ticker.h>
#include <Ultrasonic.h>
#include <DHT.h>

#define DHTTYPE           DHT22     // DHT 22 (AM2302)
#define SMAX              1
#define ULTRAECHO 0 // D3
#define ULTRATRIG 2 // D4
#define MOTOR 5 // D1
#define DHTPIN 4 // D2

typedef void (*fptr)();

enum event_t {INIT, DOOR_OPEN, DOOR_CLOSED_CAR_IN, DOOR_CLOSED_CAR_OUT};

class Garage
{

public:
  /* Static access method. */
  static Garage* getInstance();
  void open();
  void close();
  static void Wrapper_To_Call_relayOff(); //http://www.newty.de/fpt/callback.html#static
  static void Wrapper_To_Call_updateStatus();
  void detect();
  void update();
  char* getTemp(char*);
  char* getHumid(char*);
  char* getEvent(char*);
  void setCallback(fptr callback);


private:
  /* Private constructor to prevent instancing. */
  Garage();
  /* Here will be the instance stored. */
  static Garage* instance;
  void garageCntrl(bool up);
  void relayOn();
  void relayOff();
  void updateStatus();
  float avg(float *rgFloat, float newVal, uint8 index);


  // Variables
  Ticker *pTimer = new Ticker(Wrapper_To_Call_relayOff, 200, 1, MILLIS);
  Ticker *pTimer2 = new Ticker(Wrapper_To_Call_updateStatus, 3000 , 0, MILLIS);
  Ultrasonic *pSonic = new Ultrasonic(ULTRATRIG, ULTRAECHO);
  DHT *pDht = new DHT(DHTPIN, DHTTYPE);
  event_t lastEvent = INIT;
  float temp;
  float humid;
  float rgTemp[SMAX];
  float rgHumid[SMAX];
  int rgIndex = 0;
  fptr call;

};

#endif
