#include "motor/motor.hpp"

MOTOR_DLL_API const char* MOTOR_BUILD_DATE = string{__DATE__ + string{" ;; "} + __TIME__}.c_str();