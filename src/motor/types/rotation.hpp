/* Jayson */
#pragma once

#include <fuchslib/shorter.hpp>
#include <fuchslib/utility.hpp>

namespace Motor {

    class Rotation {
        float angle_;

    public:
        nd float getAngle() const { return this->angle_; }
        void setAngle(float angle) { 
            if (angle <= 360) {
                this->angle_ = angle;
            }
            else {
                this->angle_ = 0;
            }
        }

        nd float asRadians() const { return getAngle() * (2 * acos(0.0)) / 180; }

        void increaseAngle(float increment) { setAngle(getAngle() + increment); }
        void increaseAngle() { increaseAngle(1); }
        void decreaseAngle(float decrement) { setAngle(getAngle() - decrement); }
        void decreaseAngle() { decreaseAngle(1); }
    };

    inline void to_json(json& j, const Rotation& p) {
        j = json{
                {"angle", p.getAngle()}
        };
    }

    inline void from_json(const json& j, Rotation& p) {
        if (Fuchs::Utility::js::hasKey(j, "angle")) p.setAngle(j.at("angle"));
    }

}
