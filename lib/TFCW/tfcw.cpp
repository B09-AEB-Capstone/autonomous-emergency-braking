#include "TFCW.h"

TFCW::TFCW(float reactionTime, float maxDeceleration) {
    this->reactionTime = reactionTime;
    this->maxDeceleration = maxDeceleration;
}

float TFCW::calculateTFCW(float velocity) {
    if (maxDeceleration <= 0) {
        return -1; 
    }
    return reactionTime + (velocity / maxDeceleration);
}

bool TFCW::checkWarning(float ttc, float tfcw) {
    return (ttc > 0 && ttc <= tfcw);
}
