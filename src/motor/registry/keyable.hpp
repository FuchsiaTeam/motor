#pragma once
#include "fuchslib/key.hpp"

namespace Motor {
    class KeyAble {
        Fuchs::FuchsKey key_;
    protected:
        void forceSetKey(Fuchs::FuchsKey& key) {
            this->key_ = key;
        }
    public:
        ~KeyAble() = default;

        Fuchs::FuchsKey& getKey() {
            return this->key_;
        }
    };
}