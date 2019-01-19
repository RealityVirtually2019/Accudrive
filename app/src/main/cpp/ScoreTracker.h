#ifndef SCORE_TRACKER_H
#define SCORE_TRACKER_H

#include <context.h>
#include <application.h>
#include <android/sensor.h>
#include <android/log.h>
#include <utilities.h>
#include <detectionobject.h>
#include <color.h>

class ScoreTracker
{
    public:
        double totalScore = 0.0;
        ScoreTracker();
        void AddScore(double score);
        
        void SubtractScore(double score)
        {
            totalScore -= score;
            LOGI("Score lost... New score: " + totalScore);
        }

        
    private:

}


#endif