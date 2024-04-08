/* Jayson Hartmann 27/03/2022 */
#include "motor/object/display.hpp"
#include "motor/log.hpp"
#include "motor/motor.hpp"

namespace Motor {
	bool Window::create(const string& title, uint32 flags) {
		sdlwindow_ = SDL_CreateWindow(title.c_str(), width_, height_, flags);
		MOTOR_LOG("Window Created TITLE{" + title + "}, FLAGS{" + to_string(flags) + "}");
		return sdlwindow_;
	}

    void Window::update() {
		int newWidth = 0;
		int newHeight = 0;
        SDL_GetWindowSize(sdlwindow_, &newWidth, &newHeight);

		if (newWidth != width_ || newHeight != height_) {
			width_ = newWidth;
			height_ = newHeight;
			getriebe.getGame()->changeResolution(width_, height_);
		}
    }

}