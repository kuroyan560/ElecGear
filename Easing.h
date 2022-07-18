#pragma once
namespace Easing
{
    float Lerp(float start, float end, float t);

    float QuadIn(float t, float totaltime, float min, float max);
    float QuadOut(float t, float totaltime, float min, float max);
    float QuadInOut(float t, float totaltime, float min, float max);

    float CubicIn(float t, float totaltime, float min, float max);
    float CubicOut(float t, float totaltime, float min, float max);
    float CubicInOut(float t, float totaltime, float min, float max);

    float QuartIn(float t, float totaltime, float min, float max);
    float QuartOut(float t, float totaltime, float min, float max);
    float QuartInOut(float t, float totaltime, float min, float max);

    float QuintIn(float t, float totaltime, float min, float max);
    float QuintOut(float t, float totaltime, float min, float max);
    float QuintInOut(float t, float totaltime, float min, float max);

    float SineIn(float t, float totaltime, float min, float max);
    float SineOut(float t, float totaltime, float min, float max);
    float SineInOut(float t, float totaltime, float min, float max);

    float ExpIn(float t, float totaltime, float min, float max);
    float ExpOut(float t, float totaltime, float min, float max);
    float ExpInOut(float t, float totaltime, float min, float max);

    float CircIn(float t, float totaltime, float min, float max);
    float CircOut(float t, float totaltime, float min, float max);
    float CircInOut(float t, float totaltime, float min, float max);

    float ElasticIn(float t, float totaltime, float min, float max);
    float ElasticOut(float t, float totaltime, float min, float max);
    float ElasticInOut(float t, float totaltime, float min, float max);

    float BackIn(float t, float totaltime, float min, float max);
    float BackOut(float t, float totaltime, float min, float max);
    float BackInOut(float t, float totaltime, float min, float max);

    float BounceIn(float t, float totaltime, float min, float max);
    float BounceOut(float t, float totaltime, float min, float max);
    float BounceInOut(float t, float totaltime, float min, float max);

    float Linear(float t, float totaltime, float min, float max);
};