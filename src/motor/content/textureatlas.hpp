/* Jayson Hartmann 08/05/2022 */
#pragma once

#include <fuchslib/shorter.hpp>
#include <sdl/SDL.h>

namespace Motor {
	
	class TextureAtlas;

	struct AtlasEntry {
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		string id = "";
		TextureAtlas* atlas = nullptr;
	};

	class TextureAtlas {
		vector<AtlasEntry*> entries_{};
		SDL_Surface* surface_ = nullptr;

	public:
		vector<AtlasEntry*>& getEntries() { return this->entries_; }

		void create(uint32 size) {
			surface_ = SDL_CreateRGBSurface(0, size, size, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		}

		AtlasEntry* add(SDL_Surface* surface, SDL_Rect* rect, string id) {
			if (surface && surface_) return new AtlasEntry{};
			AtlasEntry* entry{};
			if (entries_.size() > 0) entry = entries_.back();
			SDL_Rect* destination = new SDL_Rect{entry->x + entry->w, entry->y + entry->h, surface->w, surface->h};
			SDL_BlitSurface(surface, rect, surface_, destination);
			entries_.push_back(new AtlasEntry{destination->x, destination->y, surface->w, surface->h, id, this});
			return entries_.back();
		}
	};

}