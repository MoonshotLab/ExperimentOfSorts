#include "CandyReader.h"
#include "Candy.h"


// setup the color definitions
Candy candies[] = {
  {
    1, "Red", {220, 121, 103}
  },
  {
    2, "Green", {267, 351, 207}
  },
  {
    3, "Blue", {136, 143, 196}
  },
  {
    4, "Yellow", {582, 512, 216}
  },
  {
    5, "White", {565, 685, 596}
  },
  {
    6, "black", {118, 94, 81}
  }
};


// reference the color reader class
CandyReader candyReader;


void setup() {
  // init the color reader with log options
  candyReader.init(false);

  Serial.begin(9600);
}


void loop() {
  Candy candy = candyReader.getType(candies);
  if(!candy.inValid){
    Serial.print("Color Sort:\t"); Serial.print(candy.name); Serial.println();
    delay(100);
  }
}
