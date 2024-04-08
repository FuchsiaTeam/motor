/* Jayson Hartmann  27/03/2022 */
#pragma once

#include "motor/interfaces/component.hpp"
#include "sprite.hpp"
#include "motor/motor.hpp"

namespace Motor {

    /* Requires onMouseButtonDown event */
    class IColliderComponent : public IComponent {
    public:
        virtual ~IColliderComponent() = default;

        virtual void onScreen() { }
        virtual bool inCoordinate(const Vector2D& position) { return true; }
        virtual bool colliding(IColliderComponent* colliderComponent) { return true; }
        virtual vector<IColliderComponent*> colliders() { return {}; }
        virtual bool collided(IColliderComponent* collider) { return false; };
        constexpr bool onlySingleInstance() override { return false; }
    };

    enum ColliderEventType {
        UNKNOWN, CLICK, HOVER, CLICK_UP
    };

    class ColliderEvent : public IComponentEvent {
    public:
        bool onObject = false;
        ColliderEventType type = UNKNOWN;
        static constexpr cstring id_ = "collider";

        ColliderEvent() = default;

        constexpr cstring id() override {
            return id_;
        };
    };


    class RectangleColliderComponent : public IColliderComponent {
    public:
        Scale scale_{};
        Vector2D displacement_;
        RectangleColliderComponent(Scale scale, Vector2D displacement) : scale_(scale), displacement_(displacement) {};
    };

    class SpriteColliderComponent : public IColliderComponent {
    public:
        std::thread* onScreenThread = nullptr;
        void init() override {
            IColliderComponent::init();
        }

        void registerMouseClickEvent() {
            registerEvent(Events::mouseClick.attach(&onMouseClick));
            registerEvent(Events::mouseClickUp.attach(&onMouseClickUp));
        }

        void registerMouseHoverEvent() {
            registerEvent(Events::mouseMotion.attach(&onMouseMotion));
        }

        void update() override {
            onScreen();
        }

        std::function<void(SDL_Event&, int, int)> onMouseClick = [this](SDL_Event& event, int x, int y) {
            if (!owner_) return;
            GameObject* obj = dynamic_cast<GameObject*>(owner_);
            TransformComponent* transformComponent;
            if (obj) {
                transformComponent = obj->getComponent<TransformComponent>();
            }
            if (!transformComponent->onScreen) return;
            //auto f = new ColliderEvent{};
            auto f = IComponentEvent::create<ColliderEvent>();
            f->onObject = inCoordinate({ (float)event.button.x, (float)event.button.y });;
            f->type = CLICK;
            callComponentEvent(f);
        };

        std::function<void(SDL_Event&, int, int)> onMouseClickUp = [this](SDL_Event& event, int x, int y) {
            if (!owner_) return;
            GameObject* obj = dynamic_cast<GameObject*>(owner_);
            TransformComponent* transformComponent;
            if (obj) {
                transformComponent = obj->getComponent<TransformComponent>();
            }
            if (!transformComponent->onScreen) return;
            //auto f = new ColliderEvent{};
            auto f = IComponentEvent::create<ColliderEvent>();
            f->onObject = inCoordinate({ (float)event.button.x, (float)event.button.y });;
            f->type = CLICK_UP;
            callComponentEvent(f);
        };

        std::function<void(SDL_Event&)> onMouseMotion = [this](SDL_Event& event) {
            if (!owner_) return;
            GameObject* obj = dynamic_cast<GameObject*>(owner_);
            TransformComponent* transformComponent;
            if (obj) {
                transformComponent = obj->getComponent<TransformComponent>();
            }
            if (!transformComponent->onScreen) return;
            auto f = new ColliderEvent{};
            f->onObject = inCoordinate({ (float)event.button.x, (float)event.button.y });;
            f->type = HOVER;
            callComponentEvent(f);
        };

        //TODO
        void onScreen() override {
            if (!owner_) return;
            SpriteComponent* spriteComponent = owner_->getComponent<SpriteComponent>();
            if (spriteComponent) {
                TransformComponent* transformComponent = spriteComponent->transform_;
                if (transformComponent) {
                    if (spriteComponent->ignoreCamera_) {
                        transformComponent->onScreen = true;
                        return;
                    }
                    Scale scale = transformComponent->scale.cameraAdjusted();
                    Vector2D spritePos{ (float)spriteComponent->destRectangle_.x - 1, (float)spriteComponent->destRectangle_.y - 1 };
                    Vector2D endPos{ spritePos.getX() + scale.x, spritePos.getY() + scale.y };
#undef max
#undef min

                    transformComponent->onScreen = std::max(spritePos.getX(), endPos.getX()) >= 0 && std::min(spritePos.getX(), endPos.getX()) <= getriebe.getGame()->getWindow()->width_ && std::max(spritePos.getY(), endPos.getY()) >= 0 && std::min(spritePos.getY(), endPos.getY()) <= getriebe.getGame()->getWindow()->height_;
                }
            }
        }

        //TODO
        std::vector<IColliderComponent*> collissionWithOther() {
            if (!owner_) return {};
            SpriteComponent* spriteComponent = owner_->getComponent<SpriteComponent>();
            if (spriteComponent) {
                TransformComponent* transformComponent = spriteComponent->transform_;
                if (transformComponent) {
                    Scale scale = transformComponent->scale.cameraAdjusted();
                    Vector2D spritePos{ (float)spriteComponent->destRectangle_.x - 1, (float)spriteComponent->destRectangle_.y - 1 };
                    Vector2D endPos{ spritePos.getX() + scale.x, spritePos.getY() + scale.y };
#undef max
#undef min

                    std::vector<IColliderComponent*> sprColliders{};
                    for (ObjectHolder* gameObj : getriebe.getGame()->getCurrentScene()->getObjects()) {
                        if (!gameObj->exists()) continue;
                        TransformComponent* trnsComp = gameObj->get()->transform();
                        if (trnsComp) {
                            if (gameObj->get()->hasComponent<SpriteColliderComponent>()) {
                                spriteComponent = gameObj->get()->getComponent<SpriteComponent>();
                                Scale scale2 = trnsComp->scale.cameraAdjusted();
                                Vector2D spritePos2{ (float)spriteComponent->destRectangle_.x - 1, (float)spriteComponent->destRectangle_.y - 1 };
                                Vector2D endPos2{ spritePos2.getX() + scale2.x, spritePos2.getY() + scale2.y };
                                transformComponent->onScreen = std::max(spritePos.getX(), endPos.getX()) >= 0 && std::min(spritePos.getX(), endPos.getX()) <= getriebe.getGame()->getWindow()->width_ && std::max(spritePos.getY(), endPos.getY()) >= 0 && std::min(spritePos.getY(), endPos.getY()) <= getriebe.getGame()->getWindow()->height_;
                                if (std::min(spritePos2.getX(), endPos2.getX()) <= std::max(spritePos.getX(), endPos.getX()) && std::max(spritePos2.getX(), endPos2.getX()) >= std::min(spritePos.getX(), endPos.getX()) && std::min(spritePos2.getY(), endPos2.getY()) <= std::max(spritePos.getY(), endPos.getY()) && std::max(spritePos2.getY(), endPos2.getY()) >= std::min(spritePos.getY(), endPos.getY())) {
                                    IColliderComponent* collComp = gameObj->get()->getComponent<IColliderComponent>();
                                    sprColliders.push_back(collComp);
                                }
                            }
                        }
                    }
                    return sprColliders;
                }
            }
            return {};
        }

        bool inCoordinate(const Vector2D& position) override {
            SpriteComponent* spriteComponent = owner_->getComponent<SpriteComponent>();
            //std::cout << "c0" << std::endl;
            if (spriteComponent) {
                //std::cout << "c1" << std::endl;
                TransformComponent* transformComponent = spriteComponent->transform_;
                if (transformComponent) {
                    //std::cout << "c2" << std::endl;
                    Scale scale = transformComponent->scale.cameraAdjusted();
                    Vector2D spritePos{ (float)spriteComponent->destRectangle_.x + 16, (float)spriteComponent->destRectangle_.y + 16 };
                    Vector2D endPos{ spritePos.getX() + scale.x, spritePos.getY() + scale.y };
#undef max
#undef min
                    if (position.getX() <= std::max(spritePos.getX(), endPos.getX()) && position.getX() >= std::min(spritePos.getX(), endPos.getX())
                        && position.getY() <= std::max(spritePos.getY(), endPos.getY()) && position.getY() >= std::min(spritePos.getY(), endPos.getY())) {
                        //std::cout << "click" << std::endl;
                        return true;
                    }
                }
            }
            return false;
        }

        bool collided(IColliderComponent* collider) {
            SpriteComponent* spriteComponent = owner_->getComponent<SpriteComponent>();
            if (spriteComponent) {
                TransformComponent* transformComponent = spriteComponent->transform_;
                if (transformComponent) {
                    Scale scale = transformComponent->scale.cameraAdjusted();
                    Vector2D spritePos{ (float)spriteComponent->destRectangle_.x - 1, (float)spriteComponent->destRectangle_.y - 1 };
                    Vector2D endPos{ spritePos.getX() + scale.x, spritePos.getY() + scale.y };
#undef max
#undef min
                    std::vector<SpriteColliderComponent*> sprColliders{};
                    for (ObjectHolder* gameObj : getriebe.getGame()->getCurrentScene()->getObjects()) {
                        if (!gameObj->exists()) continue;
                        TransformComponent* trnsComp = gameObj->get()->transform();
                        if (trnsComp) {
                            if (gameObj->get()->hasComponent<IColliderComponent>()) {
                                spriteComponent = owner_->getComponent<SpriteComponent>();
                                Scale scale2 = trnsComp->scale.cameraAdjusted();
                                Vector2D spritePos2{ (float)spriteComponent->destRectangle_.x - 1, (float)spriteComponent->destRectangle_.y - 1 };
                                Vector2D endPos2{ spritePos2.getX() + scale2.x, spritePos2.getY() + scale2.y };

                                return (std::min(spritePos2.getX(), endPos2.getX()) <= std::max(spritePos.getX(), endPos.getX()) && std::max(spritePos2.getX(), endPos2.getX()) >= std::min(spritePos.getX(), endPos.getX())) && std::min(spritePos2.getY(), endPos2.getY()) <= std::max(spritePos.getY(), endPos.getY()) && std::max(spritePos2.getY(), endPos2.getY()) >= std::min(spritePos.getY(), endPos.getY());
                            }
                        }
                    }
                    return false;
                }
            }
            return false;
        }
    };
}
