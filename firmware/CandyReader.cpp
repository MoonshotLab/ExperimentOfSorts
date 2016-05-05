#include "CandyReader.h"
#include "Candy.h"
#include "Arduino.h"
#include "Adafruit_TCS34725.h"


// configuration
int threshold = 25;
uint16_t emptyState[] = {106, 79, 68};


CandyReader::CandyReader(){}


// setup the pins and the log level
void CandyReader::init(bool logger){
  _tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
  _tcs.begin();
  _logger = logger;
}


// find the matched candy
Candy CandyReader::getType(struct Candy candies[]){
  // setup
  uint16_t clear, red, green, blue;
  Candy matchedCandy;
  matchedCandy.inValid = true;

  _tcs.setInterrupt(false);
  _tcs.getRawData(&red, &green, &blue, &clear);
  _tcs.setInterrupt(true);

  // does the reading appear to be not empty
  uint16_t colorReading[3] = {red, green, blue};
  bool testForCandy = false;
  for(int x=0; x<3; x++){
    if(colorReading[x] > emptyState[x] + threshold
    || colorReading[x] < emptyState[x] - threshold){
      testForCandy = true;
    }
  }

  // test the color for each candy and determine which has the least variation from its ideal color
  if(testForCandy){

     if(_logger){
      Serial.print("R:\t"); Serial.print(red);
      Serial.print("\tG:\t"); Serial.print(green);
      Serial.print("\tB:\t"); Serial.print(blue);
      Serial.println("--------------------");
    }

    int leastPoints = -1;
    for(int i=0; i<5; i++){

      int points = 0;
      for(int j=0; j<3; j++){
        int diff = colorReading[j] - candies[i].idealColor[j];
        if(diff < 0) diff = -1*diff;
        points += diff;
      }

      if(points < leastPoints || leastPoints == -1){
        matchedCandy = candies[i];
        leastPoints = points;
      }

      if(_logger){
        Serial.print(candies[i].name); Serial.print(":\t"); Serial.print(points); Serial.println();
      }
    }
  }

  return matchedCandy;
}
