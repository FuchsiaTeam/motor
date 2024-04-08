/* Jayson Hartmann 27/03/2022 */
#include "motor/getriebe.hpp"
#include <fuchslib/shorter.hpp>
#include <sdl/SDL.h>
#include <sdl/SDL_ttf.h>
#include "motor/console/console.hpp"
#include "sdl/SDL_mixer.h"

namespace Motor {

	Getriebe::Getriebe() {
		this->console_ = new Console();
	}

	/* Init a SDL Sub-System */
	void initSDLSystem(const string& systemName, uint32 flag) {
		MOTOR_LOG("Init SDL: " + systemName);
		int res = SDL_InitSubSystem(flag);
		if (res == -1) MOTOR_LOG("Init " + systemName + " failed!");
	}

	void Getriebe::init(const string& title, uint16 width, uint16 height, uint32 windowFlags, uint32 rendererFlags) {
		MOTOR_LOG("Init SDL: Video");
		SDL_Init(SDL_INIT_VIDEO);
		initSDLSystem("Timer", SDL_INIT_TIMER);
		initSDLSystem("Audio", SDL_INIT_AUDIO);
		initSDLSystem("Joystick", SDL_INIT_JOYSTICK);
		initSDLSystem("Haptic", SDL_INIT_HAPTIC);
		initSDLSystem("GameController", SDL_INIT_GAMEPAD);
		initSDLSystem("Events", SDL_INIT_EVENTS);
		initSDLSystem("Sensors", SDL_INIT_SENSOR);
        initSDLSystem("Video", SDL_INIT_VIDEO);
        //SDL_AudioInit("dsound");
        //SDL_AudioInit("directsound");
        //SDL_AudioInit("waveout");
        Mix_Init(MIX_INIT_MP3);
        MOTOR_LOG("Mixer init");
        MOTOR_LOG(SDL_GetError());
		TTF_Init();
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		game_->create(title, width, height, windowFlags, rendererFlags);
		game_->running_ = true;
		//getConsole()->create();
	}


	void Getriebe::start() {
		std::cout << "Built On " << __DATE__ << " ;; " << __TIME__ <<  " ;; STD " << __cplusplus << std::endl;
		game_->loop();
	}

	void Getriebe::addContent(Content* content) {
		contents_.push_back(content);
	}

	void Getriebe::stop() {
		MOTOR_LOG("Stopping");
		SDL_DestroyWindow(sdl_window());
		SDL_DestroyRenderer(sdl_renderer());
		SDL_Quit();
	}
}