#pragma once
#include "registry.hpp"
#include "motor/object/gameobject.hpp"

namespace Motor::Reg {
    extern Mod* MOTOR_MOD;
    extern Registry<Motor::GameObject>* GAME_OBJECT;
}
