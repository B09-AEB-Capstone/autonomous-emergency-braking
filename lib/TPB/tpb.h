#ifndef TPB_H
#define TPB_H
#include <vector>

class TPB {
  public:
    TPB();
    float calculate(float ttc, const std::vector<float>& apb_section, float relative_speed);  
};

#endif
