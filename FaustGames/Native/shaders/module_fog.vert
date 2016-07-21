
float calcFogAmount(float dist)
{
    return clamp(1.0 - exp( (fogStart-dist)* fogDensity ), 0.0, 1.0)*fogScale;
}
