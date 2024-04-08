#include "motor/registry/registries.hpp"

namespace Motor::Reg {
    Mod* MOTOR_MOD = new Mod("motor");
    Registry<GameObject>* GAME_OBJECT = new Registry<GameObject>(MOTOR_MOD);
}