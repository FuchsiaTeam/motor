#pragma once
#include "vector"
#include "fuchslib/key.hpp"
#include "registryobject.hpp"
#include "mod.hpp"
namespace Motor {

    template <typename T>
    class Registry {
        std::vector<RegistryObject<T>*> objects_{};
        Mod* mod_;
    public:

        Registry(Mod* mod): mod_(mod) {}

        template<typename J>
        RegistryObject<J>* registerExternObject(J* t, Fuchs::FuchsKey key) {
            RegistryObject<J>* reg = new RegistryObject{t, key};
            objects_.push_back((RegistryObject<T>*) reg);
            return reg;
        }

        template<typename J>
        RegistryObject<J>* registerObject(J* t, string key) {
            RegistryObject<J>* reg = new RegistryObject{t, Fuchs::FuchsKey{key, mod_->getId()}};
            objects_.push_back((RegistryObject<T>*) reg);
            return reg;
        }

        bool exists(const Fuchs::FuchsKey& key) {
            for (const auto& item: getObjects()) {
                KeyAble* keyAble = item;
                if(keyAble) {
                    if(keyAble->getKey().asString() == key.asString()) {
                        return true;
                    }
                }
            }
            return false;
        }

        std::vector<RegistryObject<T>*> getObjects() {
            return objects_;
        }
    };
}