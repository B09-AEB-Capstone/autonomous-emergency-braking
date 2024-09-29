#include "TTC.h"

TTC::TTC() {
}

float TTC::calculate(float distance, float relativeSpeed) {
    if (relativeSpeed <= 0) {
        return -1;
    }
    return distance / relativeSpeed;
}
