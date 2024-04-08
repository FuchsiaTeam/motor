#include "motor/object/scene.hpp"
namespace Motor {

    void Scene::sortObjectsThread() {
        MOTOR_LOG("Objects: " + std::to_string(getObjects().size()));
        vector<pair<int, ObjectHolder*>> sortedObjects;
        for (ObjectHolder* object : getObjects()) if (object->get()) sortedObjects.emplace_back(object->get()->transform()->depth, object);
        sort(sortedObjects.begin(), sortedObjects.end(), compareDepths);
        auto size = getObjects().size();
        getObjects().clear();
        getObjects().reserve(size);
        for (auto& object : sortedObjects) getObjects().emplace_back(object.second);
        MOTOR_LOG("Sorted " + std::to_string(size) + " objects");
    }

    void Scene::update() {
        
    }

    void Scene::cleanObjects() {
        int amount = 0;
        for (ObjectHolder* obj : getObjects()) {
            if (obj->get()->isAwaitingDestroy()) {
                if (camera_.getTarget() == obj->get()->transform())  camera_.target = nullptr;
                for (IComponent* component : obj->get()->components_) {
                    component->inner_destroy();
                    component->onObjectDestroy();
                    delete component;
                }
                obj->get()->components_.clear();
                for (Fuchs::RUID& ruid : obj->get()->eventRuids_) {
                    Motor::eventObjsToRemove_.push_back(ruid);
                }
                obj->remove();
                ++amount;
            }
        }
        if (amount == 0) {
            return;
        }
        else {
            if (amount > 1) {
                //MOTOR_LOG(string{ "Cleaning " + to_string(amount) + " objects" });
            }
            else {
                //MOTOR_LOG(string{ "Cleaning 1 object"});
            }
        }
        //getObjects().erase(std::remove_if(getObjects().begin(), getObjects().end(),
           // [&](ObjectHolder* obj) { return !obj->get(); }), getObjects().end());
        std::erase_if(objects_, [](ObjectHolder* obj) {
            bool objget = obj->get();
            if (!objget) {
                delete obj;
            }
            return !objget;
            });
    }

}