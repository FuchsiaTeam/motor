/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <sdl/SDL.h>
#include <fuchslib/shorter.hpp>

namespace Motor {
	class Window {
	public:
		SDL_Window* sdlwindow_;
		int height_ = 1920;
		int width_ = 1080;
		int aspect_ratio_height_ = 9;
		int aspect_ratio_width_ = 16; //21, 16
	/* Functions */
	public:
		bool create(const string& title, uint32 flags);
        void update();
	};
}