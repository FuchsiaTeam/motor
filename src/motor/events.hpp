/* Jayson Hartmann */
#pragma once

#include "event.hpp"
#include "classdef.hpp"
#include <sdl/SDL.h>

namespace Motor {
	class ObjectHolder;
}

namespace Motor::Events {

	#ifdef __WIN32
		#define MOTOR_DLL_API _declspec(dllexport)
	#else
		#define MOTOR_DLL_API 
	#endif

    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&, int, int)>, SDL_Event&, int, int> mouseClick;
    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&, int, int)>, SDL_Event&, int, int> mouseClickUp;
    /* example function:
    * std::function<void(SDL_Event&, int, int)> onMouseClick = [](SDL_Event&, int x, int y) { x + y; };
    */


	/* Change bool 'shouldQuit' if the game SHOULD really quit */
	MOTOR_DLL_API extern Motor::Event<std::function<void(bool&)>, bool&> quit;
	/* example function:
	* std::function<void(bool&)> onQuit = [](bool& shouldQuit) { shouldQuit = false; };
	*/

    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&)>, SDL_Event&> keyDown;
    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&)>, SDL_Event&> keyUp;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> preUpdate;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> update;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> textUpdate;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> postUpdate;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> preFixedUpdate;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> fixedUpdate;
    MOTOR_DLL_API extern Motor::Event<std::function<void()>> postFixedUpdate;
    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&)>, SDL_Event&> mouseMotion;
    MOTOR_DLL_API extern Motor::Event<std::function<void(uint16 width, uint16 height)>, uint16, uint16> resolutionChange;
    MOTOR_DLL_API extern Motor::Event<std::function<void(ObjectHolder** gameObject)>, ObjectHolder**> gameObjectSceneAdd;
    MOTOR_DLL_API extern Motor::Event<std::function<void(uint32 pixelFormat)>, uint32> pixelFormatChange;
    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&)>, SDL_Event> sdlEvent;
    MOTOR_DLL_API extern Motor::Event<std::function<void(bool*)>, bool*> fullScreenToggle;
    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&)>, SDL_Event&> textInput;
    MOTOR_DLL_API extern Motor::Event<std::function<void(SDL_Event&)>, SDL_Event&> windowEvent;
}