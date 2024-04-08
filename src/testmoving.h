//
// Created by jayso on 25/04/2022.
//

#ifndef MOTOR_TESTMOVING_H
#define MOTOR_TESTMOVING_H

#include "motor/object/gameobject.hpp"
#include "motor/components/collider.hpp"
#include "motor/components/text.hpp"
#include "motor/components/phy.hpp"
#include "motor/components/text.hpp"


class DiscordAvatar : pObject {
    void onComponentEvent(Motor::IComponentEvent* eventData) override {
        if(eventData->id() == "collider") {
            Motor::ColliderEvent* colliderEvent = dynamic_cast<Motor::ColliderEvent*>(eventData);
            if(colliderEvent->type == Motor::HOVER) {
                if(!colliderEvent->onObject) {
                    transform()->color.set(255, 255, 255, 255);
                } else {
                    transform()->color.set(125, 50, 90, 255);
                }
            }
        }
    };
};

class AutoMoverObject : pObject {

    float tick = 0;
    bool show = false;

    void onCreate() override {
        GameObject::onCreate();
        addComponent<Motor::SpriteComponent>("test.png", SDL_PIXELFORMAT_RGB332);
        addComponent<Motor::PhyComponent>();
        getComponent<Motor::TransformComponent>()->scale.set(512);
    }

    void update() override {
        GameObject::update();
        float delta = getriebe.getGame()->getDelta().deltaTime;
        if (!show) {
            //getComponent<Motor::PhyComponent>()->force.add({ 0.001f * delta, 0 });
            //getComponent<Motor::PhyComponent>()->friction = 0.01 / delta;
            getComponent<Motor::TransformComponent>()->position.setX(getComponent<Motor::TransformComponent>()->position.getX() + 0.05 * delta);
        }
        tick += 0.1;
        if (getComponent<Motor::TransformComponent>()->position.getX() > 300 && !show) {
           // std::cout << tick << std::endl;
            show = true;
        }
    }
};

class TestMovingObject : public Motor::GameObject {
    int x = 1;
public:

    AutoMoverObject* autoMover = nullptr;

    virtual void render() override {
        GameObject::render();
        //getComponent<Motor::TransformComponent>()->rotation.increaseAngle(0.1);
        Motor::Vector2D& pos = getComponent<Motor::TransformComponent>()->position;
        //std::cout << pos.getX() << "_" << pos.getY() << ";" << pos.screen().getX() << "_" << pos.screen().getY() << std::endl;
        //getComponent<TransformComponent>()->position.y += 1;

        const uint8* key = SDL_GetKeyboardState(NULL);
        float delta = getriebe.getGame()->getDelta().deltaTime;
        //if (key[SDL_Scancode::SDL_SCANCODE_D]) getComponent<Motor::PhyComponent>()->force.add({ 0.005f * delta, 0 });
        //if (key[SDL_Scancode::SDL_SCANCODE_A]) getComponent<Motor::PhyComponent>()->force.add({ -0.005f * delta, 0 });
        //if (key[SDL_Scancode::SDL_SCANCODE_W]) getComponent<Motor::PhyComponent>()->force.add({ 0, -0.005f * delta });
        //if (key[SDL_Scancode::SDL_SCANCODE_S]) getComponent<Motor::PhyComponent>()->force.add({ 0, 0.005f * delta });

        if (key[SDL_Scancode::SDL_SCANCODE_D]) getComponent<Motor::TransformComponent>()->position.add({ 0.05f * delta, 0 });
        if (key[SDL_Scancode::SDL_SCANCODE_A]) getComponent<Motor::TransformComponent>()->position.add({ -0.05f * delta, 0 });
        if (key[SDL_Scancode::SDL_SCANCODE_W]) getComponent<Motor::TransformComponent>()->position.add({ 0, -0.05f * delta });
        if (key[SDL_Scancode::SDL_SCANCODE_S]) getComponent<Motor::TransformComponent>()->position.add({ 0, 0.05f * delta });
        //getComponent<Motor::TextComponent>()->setContent(to_string(getComponent<Motor::PhyComponent>()->force.getX()).substr(0, 4) + "_" + to_string(getComponent<Motor::PhyComponent>()->force.getY()).substr(0, 4));
        getComponent<Motor::PhyComponent>()->friction = 0.01 / delta;
        if (key[SDL_Scancode::SDL_SCANCODE_Z]) getriebe.getGame()->getCurrentScene()->getCamera().zoom += 0.005 * delta;
        if (key[SDL_Scancode::SDL_SCANCODE_U]) {
            getriebe.getGame()->getCurrentScene()->getCamera().zoom -= 0.005 * delta;
            std::cout << getriebe.getGame()->getCurrentScene()->getCamera().zoom << std::endl;
        }

        if(key[SDL_Scancode::SDL_SCANCODE_L]) {
            transform()->position = autoMover->transform()->position;
        }
    }

    std::function<void(SDL_Event&, int, int)> onMouseClick = [this](SDL_Event&, int x, int y) {
        MOTOR_LOG(to_string(x));
    };

    std::function<void(SDL_Event&)> onUpDown = [this](SDL_Event& event) {
        //if (event.key.keysym.sym == SDL_KeyCode::SDLK_h) destroy();
    };

    void registerEvents() override {
        registerEvent(Motor::Events::mouseClick.attach(&onMouseClick));
        registerEvent(Motor::Events::keyUp.attach(&onUpDown));
    }


    void onComponentEvent(Motor::IComponentEvent* eventData) override {
        if(eventData->id() == Motor::ColliderEvent::id_) {
            Motor::ColliderEvent* colliderEvent = dynamic_cast<Motor::ColliderEvent*>(eventData);
            if(colliderEvent->type == Motor::HOVER) {
                if(!colliderEvent->onObject) {
                    transform()->color.set(255, 255, 255, 255);
                } else {
                    transform()->color.set(125, 50, 90, 255);
                }
            }
        }
    };

};

class TestHover : public Motor::GameObject {
public:

    void registerEvents() override {
        registerEvent(Motor::Events::keyUp.attach(&onUpDown));
    }

    std::function<void(SDL_Event&)> onUpDown = [this](SDL_Event& event) {
        if (event.key.keysym.sym == SDL_KeyCode::SDLK_g) destroy();
    };

    void onComponentEvent(Motor::IComponentEvent* eventData) override {
        if(eventData->id() == "collider") {
            Motor::ColliderEvent* colliderEvent = dynamic_cast<Motor::ColliderEvent*>(eventData);
            if(colliderEvent->type == Motor::HOVER) {
                if(!colliderEvent->onObject) {
                    transform()->color.set(255, 255, 255, 255);
                } else {
                    transform()->color.set(125, 50, 90, 255);
                }
            }
        }
    };

};

class DestroyObj1 : public Motor::GameObject {
       void registerEvents() override {
        registerEvent(Motor::Events::keyUp.attach(&onUpDown));
    }

    std::function<void(SDL_Event&)> onUpDown = [this](SDL_Event& event) {
        if (event.key.keysym.sym == SDL_KeyCode::SDLK_n) destroy();
    };
};

class DestroyObj2 : public Motor::GameObject {
       void registerEvents() override {
        registerEvent(Motor::Events::keyUp.attach(&onUpDown));
    }

    std::function<void(SDL_Event&)> onUpDown = [this](SDL_Event& event) {
        if (event.key.keysym.sym == SDL_KeyCode::SDLK_m) destroy();
    };

    void onComponentEvent(Motor::IComponentEvent* eventData) override {
        if (eventData->id() == "collider") {
            Motor::ColliderEvent* colliderEvent = dynamic_cast<Motor::ColliderEvent*>(eventData);
            if (colliderEvent->type == Motor::HOVER) {
                if (!colliderEvent->onObject) {
                    transform()->color.set(255, 255, 255, 255);
                }
                else {
                    transform()->color.set(125, 50, 90, 255);
                }
            }
        }
    };

};

class MouseRectObj : public Motor::GameObject {
public:

    void registerEvents() override {
        registerEvent(Motor::Events::mouseMotion.attach(&onMouse));
    }

    std::function<void(SDL_Event&)> onMouse = [this](SDL_Event& event) {
        getComponent<Motor::TransformComponent>()->position.set(event.button.x, event.button.y);
    };
};


#endif //MOTOR_TESTMOVING_H
