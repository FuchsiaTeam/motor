/* Jayson Hartmann 27/03/2022 */
#pragma once

#include "content.hpp"
#include "sdl/SDL_ttf.h"
#include "motor/events.hpp"
#include "motor/motor.hpp"

namespace Motor {
	class Font : public Content<TTF_Font> {
	public:
		uint16 size_ = 24;
		uintmax outline_ = 0;

		explicit Font(string path) : Content(path) { Motor::Events::resolutionChange.attach(&onResolutionChange);  };
		Font(string path, uint16 size) : Content(path), size_(size) { Motor::Events::resolutionChange.attach(&onResolutionChange); };
		explicit Font(ResourceLocation path) : Content(path) { Motor::Events::resolutionChange.attach(&onResolutionChange); };
		Font(ResourceLocation path, uint16 size) : Content(path), size_(size) { Motor::Events::resolutionChange.attach(&onResolutionChange); };

	private:
		void inner_load() override;
		void inner_unload() override;

	public:
		void setOutline(uintmax outline) {
			outline_ = outline;
			if(content_) TTF_SetFontOutline(content_, outline);
		}

		std::function<void(uint16, uint16)> onResolutionChange = [this](uint16 width, uint16 height) {
			if (exists()) {
				uint16 sizeCache = size_;
				size_ *= (getriebe.getGame()->getRenderer()->widthDifference_ + getriebe.getGame()->getRenderer()->heightDifference_) / 2;
				unload();
				load();
				size_ = sizeCache;
				if (outline_ > 0) TTF_SetFontOutline(content_, outline_);
			}
		};

	};

	inline void Font::inner_load() {
		this->content_ = TTF_OpenFont(path_.c_str(), size_);
	}

	inline void Font::inner_unload() {
		if (exists()) {
			TTF_CloseFont(get());
		}
		this->content_ = nullptr;
	}
}