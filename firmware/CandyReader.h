#ifndef CandyReader_h
#define CandyReader_h

#include "Candy.h"

// log level params
// 1: all sensor data, useful for default calibration
// 2: non empty sensor data, useful for color calibration
// 3: color scores

class CandyReader
{
  public:
    CandyReader();
    void init(int s0, int s1, int s2, int s3, int out, int logLevel);
    Candy getType(struct Candy candies[]);
  private:
    void detect(int colorReading[3]);
    int _s0;
    int _s1;
    int _s2;
    int _s3;
    int _out;
    int _logLevel;
};

#endif
