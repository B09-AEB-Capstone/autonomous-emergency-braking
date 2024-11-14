#include "fuzzyBrakeController.h"

FuzzyBrakeController::FuzzyBrakeController()
{
    fuzzy = new Fuzzy();

    initializeFuzzySets();
    initializeFuzzyRules();
}

void FuzzyBrakeController::initializeFuzzySets()
{
    /* distance */
    distanceSets[0] = new FuzzySet(0, 0, 0, 80);        /* very short */
    distanceSets[1] = new FuzzySet(60, 110, 110, 160);  /* short */
    distanceSets[2] = new FuzzySet(140, 170, 250, 280); /* medium */
    distanceSets[3] = new FuzzySet(260, 300, 300, 340); /* long */
    distanceSets[4] = new FuzzySet(320, 360, 400, 400); /* very long */

    /* speed */
    // speedSets[0] = new FuzzySet(0, 0, 0, 30);        /* very slow */
    // speedSets[1] = new FuzzySet(20, 40, 40, 60);     /* slow */
    // speedSets[2] = new FuzzySet(50, 70, 90, 100);    /* moderate */
    // speedSets[3] = new FuzzySet(80, 100, 100, 120);  /* fast */
    // speedSets[4] = new FuzzySet(110, 130, 150, 150); /* very fast */

    speedSets[0] = new FuzzySet(50, 50, 50, 80);     /* very slow */
    speedSets[1] = new FuzzySet(70, 100, 100, 130);  /* slow */
    speedSets[2] = new FuzzySet(110, 140, 180, 200); /* moderate */
    speedSets[3] = new FuzzySet(160, 200, 200, 220); /* fast */
    speedSets[4] = new FuzzySet(210, 230, 255, 255); /* very fast*/

    /* brake */
    brakeSets[0] = new FuzzySet(0, 15, 15, 33);   /* tiga */
    brakeSets[1] = new FuzzySet(30, 45, 45, 66);  /* empat */
    brakeSets[2] = new FuzzySet(60, 75, 75, 100); /* lima */

    FuzzyInput *distance = new FuzzyInput(1);
    FuzzyInput *speed = new FuzzyInput(2);
    for (auto &set : distanceSets)
        distance->addFuzzySet(set);
    for (auto &set : speedSets)
        speed->addFuzzySet(set);

    FuzzyOutput *brake = new FuzzyOutput(1);
    for (auto &set : brakeSets)
        brake->addFuzzySet(set);

    fuzzy->addFuzzyInput(distance);
    fuzzy->addFuzzyInput(speed);
    fuzzy->addFuzzyOutput(brake);
}

void FuzzyBrakeController::initializeFuzzyRules()
{
    /* rule mapping */
    addRule(speedSets[0], distanceSets[0], brakeSets[1]); /* verySlow & veryShort -> empat */
    addRule(speedSets[0], distanceSets[1], brakeSets[0]); /* verySlow & short -> tiga */
    addRule(speedSets[0], distanceSets[2], brakeSets[0]); /* verySlow & medium -> tiga */
    addRule(speedSets[0], distanceSets[3], brakeSets[0]); /* verySlow & long -> tiga */
    addRule(speedSets[0], distanceSets[4], brakeSets[0]); /* verySlow & veryLong -> tiga */

    addRule(speedSets[1], distanceSets[0], brakeSets[1]); /* slow & veryShort -> empat */
    addRule(speedSets[1], distanceSets[1], brakeSets[1]); /* slow & short -> empat */
    addRule(speedSets[1], distanceSets[2], brakeSets[0]); /* slow & medium -> tiga */
    addRule(speedSets[1], distanceSets[3], brakeSets[0]); /* slow & long -> tiga */
    addRule(speedSets[1], distanceSets[4], brakeSets[0]); /* slow & veryLong -> tiga */

    addRule(speedSets[2], distanceSets[0], brakeSets[2]); /* moderate & veryShort -> lima */
    addRule(speedSets[2], distanceSets[1], brakeSets[1]); /* moderate & short -> empat */
    addRule(speedSets[2], distanceSets[2], brakeSets[1]); /* moderate & medium -> empat */
    addRule(speedSets[2], distanceSets[3], brakeSets[0]); /* moderate & long -> tiga */
    addRule(speedSets[2], distanceSets[4], brakeSets[0]); /* moderate & veryLong -> tiga */

    addRule(speedSets[3], distanceSets[0], brakeSets[2]); /* fast & veryShort -> lima */
    addRule(speedSets[3], distanceSets[1], brakeSets[2]); /* fast & short -> lima */
    addRule(speedSets[3], distanceSets[2], brakeSets[1]); /* fast & medium -> empat */
    addRule(speedSets[3], distanceSets[3], brakeSets[1]); /* fast & long -> empat */
    addRule(speedSets[3], distanceSets[4], brakeSets[0]); /* fast & veryLong -> tiga */

    addRule(speedSets[4], distanceSets[0], brakeSets[2]); /* veryFast & veryShort -> lima */
    addRule(speedSets[4], distanceSets[1], brakeSets[2]); /* veryFast & short -> lima */
    addRule(speedSets[4], distanceSets[2], brakeSets[2]); /* veryFast & medium -> lima */
    addRule(speedSets[4], distanceSets[3], brakeSets[2]); /* veryFast & long -> lima */
    addRule(speedSets[4], distanceSets[4], brakeSets[0]); /* veryFast & veryLong -> tiga */
}

void FuzzyBrakeController::addRule(FuzzySet *speedSet, FuzzySet *distanceSet, FuzzySet *brakeSet)
{
    static int ruleIndex = 1;
    FuzzyRuleAntecedent *antecedent = new FuzzyRuleAntecedent();
    antecedent->joinWithAND(speedSet, distanceSet);

    FuzzyRuleConsequent *consequent = new FuzzyRuleConsequent();
    consequent->addOutput(brakeSet);

    FuzzyRule *rule = new FuzzyRule(ruleIndex++, antecedent, consequent);
    fuzzy->addFuzzyRule(rule);
}

int FuzzyBrakeController::getBrakeLevel(float inputDistance, float inputSpeed)
{
    fuzzy->setInput(1, inputSpeed);
    fuzzy->setInput(2, inputDistance);
    fuzzy->fuzzify();

    return fuzzy->defuzzify(1);
}

String FuzzyBrakeController::getBrakeLevelName(int output)
{
    if (output <= 33)
        return "tiga";
    if (output <= 66)
        return "empat";
    return "lima";
}