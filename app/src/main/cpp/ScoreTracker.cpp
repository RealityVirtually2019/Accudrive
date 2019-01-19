
#include "ScoreTracker.h"
#include "SpeedCheck.h"

ScoreTracker::ScoreTracker() {
    scoreMap[SPEEDING] = 0.0;
    scoreMap[PARKING] = 0.0;
    scoreMap[LANE_CHANGE] = 0.0;
    scoreMap[TRAFFIC_SIGNAL] = 0.0;

    scoreWeightMap[SPEEDING] = 1.0;
    scoreWeightMap[PARKING] = 1.0;
    scoreWeightMap[LANE_CHANGE] = 1.0;
    scoreWeightMap[TRAFFIC_SIGNAL] = 1.0;
}

void ScoreTracker::AddScore(ScoreFactor factor, double score) {
    scoreMap[factor] += score;
    LOGI("Score Increased! New score: %f", scoreMap[factor]);
}

void ScoreTracker::SubtractScore(ScoreFactor factor, double score) {
    scoreMap[factor] -= score;
    LOGI("Score Lost... New score: %f", scoreMap[factor]);
}

double ScoreTracker::CalcTotalScore() {
    double totalScore = 0.0;
    totalScore += scoreMap[SPEEDING] * scoreWeightMap[SPEEDING];
    totalScore += scoreMap[PARKING] * scoreWeightMap[PARKING];
    totalScore += scoreMap[LANE_CHANGE] * scoreWeightMap[LANE_CHANGE];
    totalScore += scoreMap[TRAFFIC_SIGNAL] * scoreWeightMap[TRAFFIC_SIGNAL];
    return totalScore;
}