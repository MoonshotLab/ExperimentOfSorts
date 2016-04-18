#include "CandyReader.h"
#include "Candy.h"


// setup the color definitions
Candy candies[] = {
  {
    1, "red", {356, 301, 402}
  },
  {
    2, "green", {376, 293, 341}
  },
  {
    3, "yellow", {295, 284, 300}
  },
  {
    4, "purple", {415, 302, 401}
  },
  {
    5, "orange", {294, 298, 366}
  }
};


// reference the color reader class
CandyReader candyReader;


void setup() {
  // init the color reader with pins
  int logLevel = 2;
  candyReader.init(6, 5, 4, 3, 2, logLevel);

  Serial.begin(9600);
}


void loop() {
  Candy candy = candyReader.getType(candies);

  if(!candy.inValid){
    Serial.print(candy.name);
    Serial.println();
    delay(500);
  }
}
