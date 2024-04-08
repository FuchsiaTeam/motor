/* Jayson Hartmann 27/03/2022 */
#pragma once

#include "game.hpp"

namespace Motor {
	class Console;
	class Getriebe {
	public:
		Game* game_ = new Game();
		class Content;
		class Texture;
		std::vector<Content*> contents_{};
		Console* console_;
		Getriebe();
	public:
		std::vector<Motor::Texture*> textures_{};

		/* Functions */
		public:
			Game* getGame() { return this->game_; }

			/* Initializes SDL */
			void init(const std::string& title, uint16 width, uint16 height, uint32 windowFlags, uint32 rendererFlags);
			void stop();
			void start();

			SDL_Renderer* sdl_renderer() { return game_->getRenderer()->sdlrenderer_; }
			SDL_Window* sdl_window() { return game_->getWindow()->sdlwindow_; }
			void addContent(Content* content);

			std::vector<Content*>& contents() { return contents_; }
			Console* getConsole() {
				return console_;
			}
	};
}