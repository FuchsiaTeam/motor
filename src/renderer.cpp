/* Jayson Hartmann 27/03/2022 */
#include "motor/object/renderer.hpp"
#include "motor/log.hpp"
#include "motor/motor.hpp"

namespace Motor {
	
	bool Renderer::create(Window* window, const char* name, uint32 flags, bool logic) {
		sdlrenderer_ = SDL_CreateRenderer(window->sdlwindow_, name, flags);
        if(!sdlrenderer_) {
            MOTOR_LOG(SDL_GetError());
        }
		if(logic) SDL_SetRenderLogicalPresentation(sdlrenderer_, Renderer::logicalWidth, Renderer::logicalHeight, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE, SDL_SCALEMODE_NEAREST);
		SDL_SetRenderDrawColor(sdlrenderer_, 55, 50, 47, 255);
		//heightDifference_ = (float)window->height_ / (float)Renderer::logicalHeight;
		//widthDifference_ = (float)window->width_ / (float)Renderer::logicalWidth;
		update(window);
		MOTOR_LOG("Renderer Created VULKAN{0}, OPENGL{1}, LOGIC{" + to_string(logic) + "}, HEIGHTDIF{" + to_string(heightDifference_) + "}, WIDTHDIF{" + to_string(widthDifference_) + "}");
		return sdlrenderer_;
	}

    void Renderer::update(Window* window) {
        heightDifference_ = (float)window->height_ / (float)Renderer::logicalHeight;
        widthDifference_ = (float)window->width_ / (float)Renderer::logicalWidth;
		aspectRadioHeightDifference_ = (float)logicalAspectRatioHeight / (float)window->aspect_ratio_height_;
		aspectRatioWidthDifference_ = (float)logicalAspectRatioWidth / (float)window->aspect_ratio_width_;
		//MOTOR_LOG(std::to_string(aspectRatioWidthDifference_));
    }

}