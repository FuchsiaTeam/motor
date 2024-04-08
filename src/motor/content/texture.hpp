/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <sdl/SDL.h>
#include <sdl/SDL_image.h>
#include "content.hpp"
#include "motor/motor.hpp"
#include <fuchslib/ruid.hpp>
#include <utility>

namespace Motor {
	class Texture : public Content<SDL_Texture> {
		uint16 width_ = 0;
		uint16 height_ = 0;
		//https://wiki.libsdl.org/SDL_PixelFormatEnum
		uint32 pixelFormat_ = 0;
	public:
		bool interpolated_ = false;
		/* Probably NULL */

	private:
		void inner_load() override;
		void inner_unload() override;

	public:
		explicit Texture(string path) : Content(std::move(path)) {
			Events::pixelFormatChange.attach(&onPixelFormatChange);
			getriebe.textures_.push_back(this);
		};

		explicit Texture(ResourceLocation path) : Content(std::move(path)) {
			Events::pixelFormatChange.attach(&onPixelFormatChange);
			getriebe.textures_.push_back(this);
		};

		Texture(string path, uint32 pixelFormat) : Content(std::move(path)), pixelFormat_(pixelFormat) {
			Events::pixelFormatChange.attach(&onPixelFormatChange);
			getriebe.textures_.push_back(this);
		}

		Texture();;

	public:

		nd uint16 getWidth() const { return this->width_; }
		nd uint16 getHeight() const { return this->height_; }

		std::function<void(uint32)> onPixelFormatChange = [this](uint32 pixelFormat) {
			pixelFormat_ = pixelFormat;
			if (exists()) {
				unload();
				load();
			}
		};

		//SDL2 MIGRATION https://github.com/libsdl-org/SDL/blob/main/docs/README-migration.md
		SDL_Surface* SDL_CreateRGBSurfaceFrom(void *pixels, int width, int height, int depth, int pitch, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
    		return SDL_CreateSurfaceFrom(pixels, width, height, pitch,
            	SDL_GetPixelFormatEnumForMasks(depth, Rmask, Gmask, Bmask, Amask));
		}		

		void fromPixelData(uintmax* pixels, int width, int height, int depth, int pitch, uint32 rMask = 0x0f00, uint32 gMask = 0x00f0, uint32 bMask = 0x000f, uint32 aMask = 0xf000) {
			if (exists()) unload();
			SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, rMask, gMask, bMask, aMask);
			content_ = SDL_CreateTextureFromSurface(getriebe.sdl_renderer(), surface);
			SDL_DestroySurface(surface);
		}

		std::byte* rawData() {
			std::byte* data = nullptr;
			return data;
		}
	};

	inline void Texture::inner_load() {
		if (interpolated_) {
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		}
		SDL_Surface* surface = IMG_Load(path_.c_str());
		if (pixelFormat_ != 0) {
			SDL_Surface* pixelFormattedSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat_);
			SDL_DestroySurface(surface);
			surface = pixelFormattedSurface;
		}
		if (!surface) return;
		width_ = surface->w;
		height_ = surface->h;
		content_ = SDL_CreateTextureFromSurface(getriebe.sdl_renderer(), surface);
		SDL_SetTextureBlendMode(content_, SDL_BlendMode::SDL_BLENDMODE_BLEND);
		SDL_DestroySurface(surface);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	}

	inline void Texture::inner_unload() {
		SDL_DestroyTexture(get());
		this->content_ = nullptr;
	}

	class DynamicTexture : public Texture {
		vector<Fuchs::RUID> holders_{};
	public:
		explicit DynamicTexture(string path) : Texture(std::move(path)) {};
		explicit DynamicTexture(ResourceLocation path) : Texture(std::move(path)) {};

	public:
		vector<Fuchs::RUID>& getHolders() { return this->holders_; }
		void validate() {
			if (getHolders().empty() && exists()) {
				unload();
				MOTOR_LOG("Unloaded DynamicTexture (" + path_ + ")");
			}
		}
	};
}