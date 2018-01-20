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
  /**
   * Get the Garage instance
   * @return Pointer to Garage instance
   */
  static Garage* getInstance();


  /**
   * Opens the Garage
   */
  void open();

  /**
   * Closes the Garage
   */
  void close();

  /**
   * Static wrapper function used in Ticker callback
   * http://www.newty.de/fpt/callback.html#static
   */
  static void Wrapper_To_Call_relayOff();

  /**
   * Static wrapper function used in Ticker callback
   * http://www.newty.de/fpt/callback.html#static
   */
  static void Wrapper_To_Call_updateStatus();

  /**
   * Detects door status (open/close) and updates lastEvent
   */
  void detect();

  /**
   * Called periodically to update the timers
   */
  void update();

  /**
   * Get current temperatur
   * @param  t Char array where temperature is stored
   * @return Pointer to temperture array
   */
  char* getTemp(char*);

  /**
  * Get current hunidity
  * @param  h CHar array where humitidy is stored
  * @return Pointer to humidity array
   */
  char* getHumid(char*);

  /**
   * Get last door event
   * @param  e Char array where event is stored
   * @return Pointer to event array
   */
  char* getEvent(char*);

  /**
   * Set a callback function.
   * The callback will be called when a new door event is detected
   * @param callback [description]
   */
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
  Ticker *pTimer2 = new Ticker(Wrapper_To_Call_updateStatus, 60000 , 0, MILLIS);
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
