/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <sdl/SDL.h>
#include <fuchslib/shorter.hpp>
#include "display.hpp"

namespace Motor {
	class Renderer {
	public:
		SDL_Renderer* sdlrenderer_;
		static constexpr uint16 logicalHeight = 1080;
		static constexpr uint16 logicalWidth = 1920;
		static constexpr uint16 logicalAspectRatioWidth = 16;
		static constexpr uint16 logicalAspectRatioHeight = 9;
		float heightDifference_ = 0;
		float widthDifference_ = 0;
		float aspectRatioWidthDifference_ = 0;
		float aspectRadioHeightDifference_ = 0;

		/* Functions */
	public:
		bool create(Window* window, const char* name, uint32 flags, bool logic);
        void update(Window* window);
	};
}