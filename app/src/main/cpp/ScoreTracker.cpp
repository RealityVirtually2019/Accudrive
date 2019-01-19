

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ScoreTracker", __VA_ARGS__))

ScoreTracker::AddScore(doube score)
{
    totalScore += score;
    LOGI("Score added! New score: " + totalScore);
}

ScoreTracker::SubtractScore(double score)
{
    
}