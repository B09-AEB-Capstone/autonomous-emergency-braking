#ifndef TFCW_H
#define TFCW_H

class TFCW {
  public:
    TFCW(float reactionTime, float maxDeceleration);  
    float calculateTFCW(float velocity);  
    bool checkWarning(float ttc, float tfcw); 
  private:
    float reactionTime;    
    float maxDeceleration; // Maximum deceleration (braking capability) in m/s^2
};

#endif
