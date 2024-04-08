/* Jayson Hartmann 27/03/2022 */
#pragma once

#include "object/renderer.hpp"
#include "object/gameobject.hpp"
#include "object/scene.hpp"
#include "events.hpp"

namespace Motor {
    class Texture;
    struct CreateCache {
        std::string title;
        uint32 windowFlags;
        uint32 rendererFlags;
    };

    struct FPS {
        uint16 fps = 60;
        uint32 step = 1000 / fps;
        uint32 nextStep = SDL_GetTicks();
        uint32 currentStep;
        uint32 frames = 0;
        uint32 currentFrames = 0;
        uint32 lastFrame = SDL_GetTicks();
    };

    struct DELTA {
        uintmax current = SDL_GetPerformanceCounter();
        uintmax last = 0;
        double deltaTime = 0;
    };

    class Game {
        Scene* scene_ = nullptr;
        FPS fps_;
        FPS fixedFPS_;
        FPS textFPS_;
        DELTA delta_;
        Renderer* renderer_ = new Renderer();
        //Renderer* nonLogicRenderer_ = new Renderer();
        Window* window_ = new Window();
        CreateCache createCache_{};
        bool mouseHidden_ = false;
        bool fullscreen_ = false;
        bool renderTick = false;
        std::thread* updateThread = nullptr;
        std::atomic_int updateThreadState{ 0 };
        std::thread* renderThread = nullptr;
        std::atomic_int renderThreadState{ 0 };

    public:
        std::vector<ObjectHolder*> sceneGarbage_{};
        std::vector<GameObject*> persistent_{};
        bool running_ = false;

    public:
        Game() = default;

        friend class Getriebe;

        /* Functions */
    public:

        Scene* getCurrentScene() { return this->scene_; }
        /* Use switchScene() instead */
        void forceSetScene(Scene* scene) {
            this->scene_ = scene;
            this->scene_->init(this);
        }
        FPS& getFPS() { return this->fps_; }
        FPS& getFixedFPS() { return this->fixedFPS_; }
        FPS& getTextFPS() { return this->textFPS_; }
        DELTA& getDelta() { return this->delta_; }
        void setFPS(int fps) {
            fps_.fps = fps;
            fps_.step = 1000 / fps;
        }

        void setTextFPS(int fps) {
            textFPS_.fps = fps;
            textFPS_.step = 1000 / fps;
        }

        Renderer* getRenderer() { return this->renderer_; }
        Window* getWindow() { return this->window_; }
        void forceSetWindow(Window* window) { this->window_ = window; }
        void forceSetRenderer(Renderer* renderer) { this->renderer_ = renderer; }

        void create(const std::string& title, uint16 width, uint16 height, uint32 windowFlags, uint32 rendererFlags, uint16 textFPS = 18);

        void loop();
        void events();
        void update();
        void fixedUpdate();
        void textUpdate();
        void render();

        void switchScene(Scene* newScene) {
            if (newScene) {
                if (scene_) {
                    for (ObjectHolder* obj : scene_->getObjects()) {
                        if (obj->exists()) {
                            bool shouldDelete = true;
                            GameObject* gameObject = obj->get();
                            gameObject->onSceneSwitch(scene_, newScene, shouldDelete);
                            if (gameObject->isPersistent()) {
                                //(*obj)->addToScene(newScene);
                                newScene->addObject(gameObject);
                                persistent_.emplace_back(gameObject);
                                continue;
                            }
                            if (shouldDelete) {
                                sceneGarbage_.push_back(obj);
                                gameObject->setActive(false);
                                gameObject->destroy();
                            }
                        }
                    }
                }
                scene_->onSwitch(newScene, this);
                scene_->getObjects().clear();
                //delete getScene();
                scene_ = newScene;
                scene_->init(this);
                //scene_->readdPersistent(persistent_);
                scene_->sortObjects();
            }
        }

        void cleanSceneGarbage(); /* {
            for (ObjectHolder* sceneGarbe : sceneGarbage_) {
                sceneGarbe->remove();
                sceneGarbe = nullptr;
            }
            sceneGarbage_.clear();
        }*/

        void changeResolution(uint16 width, uint16 height);
        void forceChangePixelFormat(uint32 pixelFormat);
        void toggleFullscreen();

        void showMouse() {
            mouseHidden_ = false;
            SDL_ShowCursor();
        }

        void hideMouse() {
            mouseHidden_ = true;
            SDL_HideCursor();
        }

    };
}