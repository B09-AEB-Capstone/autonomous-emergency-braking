#pragma once

#include <WString.h>

#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>

class FuzzyBrakeController
{
private:
    Fuzzy *fuzzy;

    FuzzySet *distanceSets[5];
    FuzzySet *speedSets[5];
    FuzzySet *brakeSets[3];

    void addRule(FuzzySet *speedSet, FuzzySet *distanceSet, FuzzySet *brakeSet);
    void initializeFuzzySets();
    void initializeFuzzyRules();

public:
    FuzzyBrakeController();
    int getBrakeLevel(float inputDistance, float inputSpeed);
    String getBrakeLevelName(int output);
};
