/* Jayson Hartmann */
#pragma once

#include "motor/object/gameobject.hpp"
#include "motor/components/collider.hpp"
#include "motor/components/text.hpp"
#include "motor/components/phy.hpp"
#include "motor/components/text.hpp"
#include "motor/util.hpp"

class KeyListener : pObject {
public:

    void onCreate() override {
        Motor::GameObject::onCreate();
        registerEvent(Motor::Events::keyDown.attach(&onKeyDown));
    }

	std::function<void(SDL_Event&)> onKeyDown = [this](SDL_Event& event) {
        const uint8* key = SDL_GetKeyboardState(NULL);
        if(key[SDL_Scancode::SDL_SCANCODE_LSHIFT] && key[SDL_Scancode::SDL_SCANCODE_KP_ENTER]) {
            getriebe.getGame()->toggleFullscreen();
        }
	};

};