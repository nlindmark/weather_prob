#ifndef WEATHER_H
#define WEATHER_H

#include <Ticker.h>
#include <DHT.h>

#define DHTTYPE           DHT22     // DHT 22 (AM2302)
#define TEMPELEMENTS              1
#define DHTPIN 4 // D2

typedef void (*fptr)();

enum event_t {INIT, DOOR_OPEN, DOOR_CLOSED_CAR_IN, DOOR_CLOSED_CAR_OUT};

class Weather
{

public:
  /**
   * Get the Weather instance
   * @return Pointer to Weather instance
   */
  static Weather* getInstance();




  /**
   * Static wrapper function used in Ticker callback
   * http://www.newty.de/fpt/callback.html#static
   */
  static void Wrapper_To_Call_updateSensors();


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
  * Get current event
  * @param  char array where the event is stored
  * @return Pointer to event string
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
  Weather();
  /* Here will be the instance stored. */
  static Weather* instance;
  void weatherProb(bool up);
  void updateSensors();
  float avg(float *rgFloat, int length, float newVal, uint8 index);
  int median(int* values, int length);


  // Variables
  Ticker *pTimer2 = new Ticker(Wrapper_To_Call_updateSensors, 60000 , 0, MILLIS);
  DHT *pDht = new DHT(DHTPIN, DHTTYPE);
  event_t lastEvent = INIT;
  float temp;
  float humid;

  float rgTemp[TEMPELEMENTS];
  float rgHumid[TEMPELEMENTS];
  int lastTempIndex = 0;


  fptr call;

};

#endif
