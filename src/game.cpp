 /* Jayson Hartmann 27/03/2022 */
#include "motor/game.hpp"
#include <fuchslib/shorter.hpp>
#include <sdl/SDL.h>
#include "motor/content/texture.hpp"
#include "motor/log.hpp"
#include <algorithm>
#include <numeric>
#include <future>
#include <mutex>
#include <execution>
#include "motor/console/console.hpp"

namespace Motor {
	
	void Game::create(const string& title, uint16 width, uint16 height, uint32 windowFlags, uint32 rendererFlags, uint16 textFPS) {
		window_->height_ = height;
		window_->width_ = width;
		window_->create(title, windowFlags);
		if (window_->sdlwindow_) MOTOR_LOG("Window Created");
		renderer_->create(window_, nullptr, rendererFlags, false);
        //nonLogicRenderer_->create(window_, -1, rendererFlags, false);
		if (renderer_->sdlrenderer_) MOTOR_LOG("Renderer Created");
		createCache_.rendererFlags = rendererFlags;
		createCache_.title = title;
		createCache_.windowFlags = windowFlags;
		setTextFPS(textFPS);
	}
	
	void Game::loop() {
			fps_.currentStep = SDL_GetTicks();
			delta_.last = delta_.current;
			delta_.current = SDL_GetPerformanceCounter();

			delta_.deltaTime = ((delta_.current - delta_.last) * 1000.0 / (double)SDL_GetPerformanceFrequency());
			if (fixedFPS_.nextStep <= fps_.currentStep) {
				int limit = 10;
				while ((fixedFPS_.nextStep <= fps_.currentStep) && (--limit)) {
					fixedFPS_.nextStep += fixedFPS_.step;
				}

				fixedUpdate();
			}
			if (textFPS_.nextStep <= fps_.currentStep) {
				int limit = 15;
				while ((textFPS_.nextStep <= fps_.currentStep) && (--limit)) {
					textFPS_.nextStep += textFPS_.step;
				}
				textUpdate();
			}
			++fps_.frames;
			if (fps_.nextStep <= fps_.currentStep) { // || VSYNC

				int limit = 2;

				while ((fps_.nextStep <= fps_.currentStep) && (--limit)) {
					fps_.nextStep += fps_.step;
				}
				events();
				/*if (updateThreadState.load() == 1) {
					if(updateThread->joinable()) updateThread->join();
					updateThreadState.store(0);
					delete updateThread;
				}
				else {
					updateThread = new std::thread(&Game::update, this);
				}*/
				update();
				render();

				cleanSceneGarbage();
				getCurrentScene()->cleanObjects();
				for (ObjectHolder* obj : scene_->getObjects()) {
					obj->get()->postUpdate();
				}
				getCurrentScene()->addCachedObjects();
			}

			if (fps_.lastFrame < SDL_GetTicks() - 1.0 * 1000) {
				fps_.currentFrames = fps_.frames;
				fps_.frames = 0;
				fps_.lastFrame = SDL_GetTicks();
			}

			if (!(fps_.nextStep <= fps_.currentStep)) {
				SDL_Delay((fps_.nextStep - fps_.currentStep));
			}
	}
	
	void Game::cleanSceneGarbage() {
		int amount = 0;
		for (ObjectHolder* obj : sceneGarbage_) {
			if (obj->get()->isAwaitingDestroy()) {
				for (IComponent* component : obj->get()->getComponentVector()) {
					component->inner_destroy();
					component->onObjectDestroy();
					delete component;
				}
				obj->get()->getComponentVector().clear();
				for (Fuchs::RUID& ruid : obj->get()->getEventRUIDS()) {
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
				//MOTOR_LOG(string{ "Cleaning 1 object" });
			}
		}
		sceneGarbage_.clear();
	}

	void Game::events() {
		SDL_Event sdlEvent;
		//SDL_WaitEvent(&sdlEvent);
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_EVENT_QUIT) {
				running_ = false;
				Events::quit.dispatch(running_);
			}
			else if (sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				int x = sdlEvent.motion.x;
				int y = sdlEvent.motion.y;
				Events::mouseClick.dispatch(sdlEvent, std::move(x), std::move(y));
			} else if(sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                int x = sdlEvent.motion.x;
                int y = sdlEvent.motion.y;
                Events::mouseClickUp.dispatch(sdlEvent, std::move(x), std::move(y));
            }
			else if (sdlEvent.type == SDL_EVENT_MOUSE_MOTION) {
				Events::mouseMotion.dispatch(sdlEvent);
			}
			else if (sdlEvent.type == SDL_EVENT_KEY_DOWN) {
				Events::keyDown.dispatch(sdlEvent);
			}
			else if (sdlEvent.type == SDL_EVENT_KEY_UP) {
				Events::keyUp.dispatch(sdlEvent);
			}
			else if (sdlEvent.type == SDL_EVENT_WINDOW_MOVED) {
				window_->update();
				renderer_->update(window_);
				Events::windowEvent.dispatch(sdlEvent);
			}
			else if (sdlEvent.type == SDL_EVENT_TEXT_INPUT) {
				Events::textInput.dispatch(sdlEvent);
			}
		}
	}

	void Game::update() {
		Events::preUpdate.dispatch();
		for (ObjectHolder* obj : scene_->getObjects()) {
            obj->get()->update();
		}
		Events::update.dispatch();
		scene_->getCamera().update();
        scene_->update();
		Events::postUpdate.dispatch();
		updateThreadState.store(1);
	}

	void Game::fixedUpdate() {
		Events::preFixedUpdate.dispatch();
		for (ObjectHolder* obj : scene_->getObjects()) { //if ((((uintptr_t)obj != UINTPTR_MAX)) && (((uintptr_t)obj->get() != UINTPTR_MAX))) 
			obj->get()->fixedUpdate();
		}
		Events::fixedUpdate.dispatch();
		Events::postFixedUpdate.dispatch();	
	}

	void Game::textUpdate() {
		for (ObjectHolder* obj : scene_->getObjects()) {
			obj->get()->textUpdate();
		}
		Events::textUpdate.dispatch();
		/*
		auto updateTask = std::async(std::launch::async,
			[this]() {
				std::for_each(std::execution::par,
					scene_->getObjects().begin(),
					scene_->getObjects().end(),
					[](ObjectHolder* obj) {
						obj->get()->textUpdate();
					});
			});
		updateTask.wait();*/
	}
	
	void Game::render() {
			SDL_RenderClear(renderer_->sdlrenderer_);
			for (ObjectHolder* obj : scene_->getObjects()) {
                 obj->get()->render();
			}

			/*SDL_Texture* target = SDL_CreateTexture(getriebe.sdl_renderer(), 0, SDL_TEXTUREACCESS_TARGET, 1920, 1080);
			SDL_SetRenderTarget(getriebe.sdl_renderer(), target);
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
			SDL_RenderClear(getriebe.sdl_renderer());
			for (ObjectHolder* obj : scene_->getObjects()) {
				obj->get()->renderInterpolated();
			}
			SDL_SetRenderTarget(getriebe.sdl_renderer(), nullptr);
			SDL_Rect src{ 0, 0 };
			SDL_Rect dst{ 1920, 1080 };
			SDL_RenderCopy(renderer_->sdlrenderer_, target, &src, &dst);*/

			SDL_RenderPresent(renderer_->sdlrenderer_);
			getriebe.getConsole()->render();
	}


	void Game::changeResolution(uint16 width, uint16 height) {
		SDL_SetWindowSize(this->window_->sdlwindow_, width, height);
		//SDL_DestroyWindow(this->window_->sdlwindow_);
		//SDL_DestroyRenderer(this->renderer_->sdlrenderer_);
		//create(createCache_.title, width, height, createCache_.windowFlags, createCache_.rendererFlags);
		
		this->window_->height_ = height;
		this->window_->width_ = width;
        Events::resolutionChange.dispatch(std::move(width), std::move(height));
        this->window_->update();
        this->renderer_->update(this->window_);
	}

	void Game::forceChangePixelFormat(uint32 pixelFormat) {
		Events::pixelFormatChange.dispatch(std::move(pixelFormat));
		MOTOR_LOG("Pixelformat Changed PIXELFORMAT{" + to_string(pixelFormat) + "}");
	}

	void Game::toggleFullscreen() {
		fullscreen_ = !fullscreen_;
		Events::fullScreenToggle.dispatch(std::move(&fullscreen_));
		SDL_SetWindowFullscreen(window_->sdlwindow_, fullscreen_);
	}

}