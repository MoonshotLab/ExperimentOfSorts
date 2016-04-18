#include "CandyReader.h"
#include "Candy.h"
#include "Arduino.h"


// configuration
int requiredConsecutivePositives = 1;
int threshold = 15;
int emptyState[] = {475, 312, 416};


// class vars
int consecutivePositives = 0;
Candy lastCandy;


CandyReader::CandyReader(){}


// setup the pins and the log level
void CandyReader::init(int s0, int s1, int s2, int s3, int out, int logLevel){
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);

  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);

  _s0 = s0; _s1 = s1; _s2 = s2; _s3 = s3; _out = out; _logLevel = logLevel;
}


// modifies an integer array assiging red, blue and green values
void CandyReader::detect(int colorReading[3]){
  // turn off the sensors
  digitalWrite(_s2, LOW);
  digitalWrite(_s3, LOW);

  // get red
  colorReading[0] = pulseIn(_out, digitalRead(_out) == HIGH ? LOW : HIGH);
  digitalWrite(_s3, HIGH);
  // get blue
  colorReading[1] = pulseIn(_out, digitalRead(_out) == HIGH ? LOW : HIGH);
  digitalWrite(_s2, HIGH);
  // get green
  colorReading[2] = pulseIn(_out, digitalRead(_out) == HIGH ? LOW : HIGH);
}


// find the matched candy
Candy CandyReader::getType(struct Candy candies[]){
  // setup
  Candy matchedCandy;
  matchedCandy.inValid = true;

  // get the reading
  int colorReading[3];
  detect(colorReading);

  if(_logLevel == 1){
    Serial.print(colorReading[0]);
    Serial.print(',');
    Serial.print(colorReading[1]);
    Serial.print(',');
    Serial.print(colorReading[2]);
    Serial.println();
  }

  // does the reading appear to be not empty
  bool testForCandy = false;
  for(int x=0; x<3; x++){
    if(colorReading[x] > emptyState[x] + threshold
    || colorReading[x] < emptyState[x] - threshold){
      testForCandy = true;
    }
  }

  // test the color for each candy and determine which has the least variation from its ideal color
  if(testForCandy){

    if(_logLevel == 2){
      Serial.print(colorReading[0]);
      Serial.print(',');
      Serial.print(colorReading[1]);
      Serial.print(',');
      Serial.print(colorReading[2]);
      Serial.println();
    }

    Candy predictedCandy;

    int leastPoints = -1;
    for(int i=0; i<5; i++){

      int points = 0;
      for(int j=0; j<3; j++){
        points += abs(colorReading[j] - candies[i].idealColor[j]);
      }

      if(points < leastPoints || leastPoints == -1){
        predictedCandy = candies[i];
        leastPoints = points;
      }

      if(_logLevel == 3){
        Serial.print(candies[i].name);
        Serial.print(':');
        Serial.print(points);
        Serial.println();
      }
    }

    if(_logLevel == 3){ Serial.println("-----------"); }

    // add or reset the positive count dependent on matching to last candy
    if(predictedCandy.id == lastCandy.id){
      consecutivePositives++;
    } else{
      consecutivePositives = 0;
    }
    lastCandy = predictedCandy;


    // are number of consecutive positives sufficient?
    if(consecutivePositives >= requiredConsecutivePositives){
      matchedCandy = lastCandy;
    }
  }

  return matchedCandy;
}
