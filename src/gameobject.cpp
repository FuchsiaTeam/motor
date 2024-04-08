#include "motor/object/gameobject.hpp"
#include "motor/object/scene.hpp"
#include "motor/motor.hpp"

namespace Motor {
	
	void GameObject::addToScene(Scene* scene, bool sortObj) {
		scene->addObject(this, sortObj);
	}

	void GameObject::addToCurrentScene(bool sortObj) {
		addToScene(getriebe.getGame()->getCurrentScene(), sortObj);
	}

	GameObject* GameObject::find(const string& name) {
        return GameObject::find(name, getriebe.getGame()->getCurrentScene());
    }

	GameObject* GameObject::findByRUID(const Fuchs::RUID& ruid) {
		return GameObject::findByRUID(ruid, getriebe.getGame()->getCurrentScene());
	}

	vector<GameObject*> GameObject::findByTag(const string& tag) {
		return GameObject::findByTag(tag, getriebe.getGame()->getCurrentScene());
	}

    GameObject* GameObject::find(const string& name, Scene* scene) {
        for (const auto& obj: scene->getObjects()) {
            if(obj->get()->name_ == name) return obj->get();
        }
        MOTOR_LOG("GameObject with NAME \"" + name + "\" not found in Scene " + scene->getRUID());
		return nullptr;
    }

    GameObject* GameObject::findByRUID(const Fuchs::RUID& ruid, Scene* scene) {
        for (const auto& obj: scene->getObjects()) {
            if(obj->get()->ruid_ == ruid) return obj->get();
        }
        MOTOR_LOG("GameObject with RUID \"" + ruid + "\" not found in Scene " + scene->getRUID());
        return nullptr;
    }

    vector<GameObject*> GameObject::findByTag(const string& tag, Scene* scene) {
        vector<GameObject*> vec{};
        for (const auto& obj: scene->getObjects()) {
            if(obj->get()->tag_ == tag) vec.push_back(obj->get());
        }
        if (vec.empty()) {
            MOTOR_LOG("No GameObjects with TAG \"" + tag + "\" found in Scene " + scene->getRUID());
        }
        return vec;
    }
}