#include "TPB.h"
#include <vector>

TPB::TPB() {
}

float TPB::calculate(float ttc, const std::vector<float>& apb_section, float relative_speed) {
    float apb = 0;

    for (float apb_value : apb_section) {
        if (relative_speed / apb_value > ttc) {
            apb = apb_value;
            break; 
        }
    }

    return apb;
}
