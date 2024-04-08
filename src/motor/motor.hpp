/* Jayson Hartmann 27/03/2022 */
#include "getriebe.hpp"

#pragma once

#define STRETCH_ASPECT
#undef STRETCH_ASPECT //Bei einem Widescreen stretch es weg; TODO
#define DELTA_FPS 60.0


MOTOR_DLL_API extern Motor::Getriebe getriebe;
MOTOR_DLL_API extern const char* MOTOR_BUILD_DATE;

//#define DELTA_INT (DELTA_FPS / (float) getriebe.getGame()->getFPS().fps)