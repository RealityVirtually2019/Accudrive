//
// Created by Niveditha G on 19/01/19.
//

#ifndef ACCUDRIVE_SCORETRACKER_H
#define ACCUDRIVE_SCORETRACKER_H

#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdio>

enum ScoreFactor
{
    SPEEDING,
    PARKING,
    LANE_CHANGE,
    TRAFFIC_SIGNAL,

};

class ScoreTracker {
public:
    ScoreTracker();
    void AddScore(ScoreFactor factor, double score);
    void SubtractScore(ScoreFactor factor, double score);
    void SaveReport();
    double CalcTotalScore();
private:
    std::map<ScoreFactor, double> scoreMap;
    std::map<ScoreFactor, double> scoreWeightMap;
};


#endif //ACCUDRIVE_SCORETRACKER_H
