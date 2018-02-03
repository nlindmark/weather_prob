#include <Arduino.h>
#include <Ticker.h>
#include "Weather.h"



/* Null, because instance will be initialized on demand. */
Weather* Weather::instance = 0;

Weather* Weather::getInstance() {
  if (instance == 0)
  {
    instance = new Weather();
    instance->pDht->begin();
    instance->pTimer->start();
    instance->call = NULL;

  }
  return instance;
}



void Weather::setCallback(fptr callback){
  call = callback;
}


void Weather::Wrapper_To_Call_updateSensors() {
  // call member
  instance->updateSensors();
}


void Weather::update() {
  pTimer->update();
}

char* Weather::getTemp(char* t) {
  snprintf(t, 50, "%f", temp);
  return t;
}

char* Weather::getHumid(char* h) {
  snprintf(h, 50, "%f", humid);
  return h;
}

char* Weather::getEvent(char* e) {

  snprintf(e, 50, "%s", "Not in use");
  return e;
}

// PRIVATE
Weather::Weather() {
}



void Weather::updateSensors() {
  // Check temp and humidity
  temp = avg(rgTemp, TEMPELEMENTS, pDht->readTemperature(), lastTempIndex);
  humid = avg(rgHumid, TEMPELEMENTS, pDht->readHumidity(), lastTempIndex);
  lastTempIndex = ++lastTempIndex%TEMPELEMENTS;

  if(call != NULL) call();

  delay(1000);
  ESP.deepSleep(60*1000000, WAKE_RF_DEFAULT);
}


float Weather::avg(float *rgFloat, int length,  float newVal, uint8 index) {

  float avg = 0.0f;

  if(!isnan(newVal)) {
    rgFloat[index] = newVal;
  }

  for(int i = 0; i < length; i++) {
    avg += rgFloat[i];
  }

  avg /= length;

  return avg;
}
