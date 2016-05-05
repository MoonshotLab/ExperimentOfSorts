#ifndef CandyReader_h
#define CandyReader_h

#include "Candy.h"
#include "Adafruit_TCS34725.h"

class CandyReader
{
  public:
    CandyReader();
    void init(bool logger);
    Candy getType(struct Candy candies[]);
  private:
    Adafruit_TCS34725 _tcs;
    bool _logger;
};

#endif
