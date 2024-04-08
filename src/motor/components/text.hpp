/* Jayson Hartmann */
#pragma once

#include "motor/interfaces/component.hpp"
#include "motor/content/font.hpp"
#include "transform.hpp"
#include "motor/motor.hpp"
#include <sdl/SDL_image.h>

namespace Motor {

	enum TextAlignment {
		LEFT, MID, RIGHT
	};

	/* Solid */
	class TextComponent : public IComponent {
	public:
		Font* font_ = nullptr;
		Font* outlineFont_ = nullptr;
		string text_;
		SDL_Texture* texture_ = nullptr;
		SDL_Texture* outlineTexture_ = nullptr;

		SDL_FRect destRectangle_{};
		bool ignoreCamera_ = false;
		TransformComponent* transform_ = nullptr;
		uint32 pixelFormat_ = 0;
		TextAlignment alignment_ = TextAlignment::MID;
		Vector2D translate_{};
		Scale customScale_{};
		Color32 color_{};
		Color32 outlineColor_{};
		bool useTransformColor_ = true;
		bool useTransformScale_ = true;
		bool translatePosition_ = true;
		bool useCustomScale_ = false;
        bool interpolated = true;

		/* (De-)Constructors */
	public:
		TextComponent(const string& path) : font_(new Font{ path }) {};
		TextComponent(const string& path, uint16 size) : font_(new Font{ path, size }) {};
		TextComponent(Font* font) : font_(font) {};
		TextComponent(ResourceLocation path) : font_(new Font{ path }) {};
		TextComponent(ResourceLocation path, uint16 size) : font_(new Font{ path, size }) {};

		TextComponent(const string& path, TextAlignment alignment) : alignment_(alignment), font_(new Font{ path }) {}
		TextComponent(const string& path, uint16 size, TextAlignment alignment) : alignment_(alignment), font_(new Font(path, size)) {}
		TextComponent(Font* font, TextAlignment alignment) : font_(font), alignment_(alignment) {}

	protected:

		virtual SDL_Surface* createSurface(const string& content, Font* font) {
			return TTF_RenderText_Solid(font->get(), content.c_str(), { 255, 255, 255 });;
		}


		virtual constexpr bool pixelFormatChangeable() { return true; }

	public:
		constexpr bool onlySingleInstance() override { return false; }

		void setOutlineFont(Font* font) {
			this->outlineFont_ = font;
		}

		void init() override {
			IComponent::init();
			if (!font_->exists()) font_->load();
			//font_->safeLoad();
			transform_ = owner_->getComponent<TransformComponent>();
			if (!transform_) owner_->addComponent<TransformComponent>();
			transform_ = owner_->getComponent<TransformComponent>();
			registerEvent(Events::resolutionChange.attach(&onResolutionChange));
			if (pixelFormatChangeable()) registerEvent(Events::pixelFormatChange.attach(&onPixelFormatChange));
		}

		void refresh() {
			setContent(getContent());
		}

		std::function<void(uint16, uint16)> onResolutionChange = [this](uint16 width, uint16 height) {
			refresh();
			};

		std::function<void(uint32)> onPixelFormatChange = [this](uint32 pixelFormat) {
			pixelFormat_ = pixelFormat;
			refresh();
			};

		void update() override {
			if (transform_) {
				Vector2D screenPos = translatePosition_ ? transform_->position.screen() : transform_->position;
				Scale adjustedScale = transform_->scale.cameraAdjusted();
				screenPos += translate_.screen();
				if (useCustomScale_) {
					Scale customAdjustedScale = customScale_.cameraAdjusted();
					adjustedScale.x = customAdjustedScale.x;
					adjustedScale.y = customAdjustedScale.y;
				}
				if (ignoreCamera_) {
					screenPos = transform_->position.fixedScreen();
					screenPos += translate_.fixedScreen();
					adjustedScale.x /= getriebe.getGame()->getCurrentScene()->getCamera().zoom;
					adjustedScale.y /= getriebe.getGame()->getCurrentScene()->getCamera().zoom;
				}
				adjustedScale.x *= getContent().size() / 2.0;
				if (!useTransformScale_) {
					int scaleX = 0;
					int scaleY = 0;
					SDL_QueryTexture(texture_, NULL, NULL, &scaleX, &scaleY);
					adjustedScale.x = scaleX;
					adjustedScale.y = scaleY;
				}


				destRectangle_.w = adjustedScale.x;
				destRectangle_.h = adjustedScale.y;
				switch (alignment_) {
				case LEFT: {
					destRectangle_.x = screenPos.getX();
					destRectangle_.y = screenPos.getY() - (adjustedScale.y / 2.0);
					break;
				}

				case RIGHT: {
					//int size = 0;
					//TTF_SizeText(font_->get(), getContent().c_str(), &size, NULL);
					destRectangle_.x = screenPos.getX() - (adjustedScale.x);
					destRectangle_.y = screenPos.getY() - (adjustedScale.y / 2.0);
					break;
				}

				case MID: {
					destRectangle_.x = screenPos.getX() - (adjustedScale.x / 2.0);
					destRectangle_.y = screenPos.getY() - (adjustedScale.y / 2.0);
					break;
				}
				}
			}
		};

		virtual void setContent(const string& content) {
			if (content == text_) return;
			if (transform_ && font_ && font_->get()) {
				if (texture_) SDL_DestroyTexture(texture_);
				text_ = content;
				SDL_Surface* surface = createSurface(content, font_);
				if (pixelFormat_ != 0) {
					SDL_Surface* pixelFormattedSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat_);
					SDL_DestroySurface(surface);
					surface = pixelFormattedSurface;
				}
				if (!surface) return;
                if(interpolated) SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
                texture_ = SDL_CreateTextureFromSurface(getriebe.sdl_renderer(), surface);
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
                SDL_DestroySurface(surface);
			}

			if (transform_ && outlineFont_ && outlineFont_->get()) {
				if (outlineTexture_) SDL_DestroyTexture(outlineTexture_);
				SDL_Surface* surface = createSurface(content, outlineFont_);
				if (pixelFormat_ != 0) {
					SDL_Surface* pixelFormattedSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat_);
					SDL_DestroySurface(surface);
					surface = pixelFormattedSurface;
				}
				if (!surface) return;
                if(interpolated) SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
                outlineTexture_ = SDL_CreateTextureFromSurface(getriebe.sdl_renderer(), surface);
                SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
                SDL_DestroySurface(surface);
			}

		}

		SDL_Texture* getTexture() const { return this->texture_; }
		Font* getFont() const { return this->font_; }

		string getContent() const {
			return this->text_;
		}

		void render() override {
			if (texture_) {
				float angle = 0;
				if (transform_) {
					if (useTransformColor_) {
						SDL_SetTextureColorMod(texture_, transform_->color.getRed(), transform_->color.getGreen(), transform_->color.getBlue());
					}
					else {
						SDL_SetTextureColorMod(texture_, color_.getRed(), color_.getGreen(), color_.getBlue());
					}
					SDL_SetTextureAlphaMod(texture_, transform_->color.getAlpha());
					angle = transform_->rotation.getAngle();

				}
				if (outlineFont_ && outlineTexture_) {
					SDL_SetTextureColorMod(outlineTexture_, outlineColor_.getRed(), outlineColor_.getGreen(), outlineColor_.getBlue());
					SDL_SetTextureAlphaMod(outlineTexture_, outlineColor_.getAlpha());
					angle = transform_->rotation.getAngle();
					SDL_RenderTextureRotated(getriebe.sdl_renderer(), outlineTexture_, NULL, &destRectangle_, angle, NULL, SDL_FLIP_NONE);
				}
				SDL_RenderTextureRotated(getriebe.sdl_renderer(), texture_, NULL, &destRectangle_, angle, NULL, SDL_FLIP_NONE);
			}
		}
	};

	class TextComponentBlended : public TextComponent {
	public:
		TextComponentBlended(const string& path) : TextComponent(path) {};
		TextComponentBlended(const string& path, uint16 size) : TextComponent(path, size) {};
		TextComponentBlended(const ResourceLocation& path) : TextComponent(path) {};
		TextComponentBlended(const ResourceLocation& path, uint16 size) : TextComponent(path, size) {};
		TextComponentBlended(Font* font) : TextComponent(font) {};

	protected:
		SDL_Surface* createSurface(const string& content, Font* font) override {
			return TTF_RenderText_Blended(font->get(), content.c_str(), { 255, 255, 255 });
		}

		constexpr bool pixelFormatChangeable() override { return false; }
	};

	class TextComponentBlendedWrapped : public TextComponent {
	protected:
		uint32 length = 32;
	public:
		TextComponentBlendedWrapped(const string& path) : TextComponent(path) {};
		TextComponentBlendedWrapped(const string& path, uint16 size) : TextComponent(path, size) {};

		TextComponentBlendedWrapped(Font* font) : TextComponent(font) {};

	protected:
		SDL_Surface* createSurface(const string& content, Font* font) override {
			return TTF_RenderText_Blended_Wrapped(font->get(), content.c_str(), { 255, 255, 255 }, length);
		}

		constexpr bool pixelFormatChangeable() override { return false; }
	};

	class TextComponentShaded : public TextComponent {
	protected:
		Color24 bg{ 0, 0, 0 };
	public:
		TextComponentShaded(const string& path) : TextComponent(path) {};
		TextComponentShaded(const string& path, uint16 size) : TextComponent(path, size) {};

		TextComponentShaded(Font* font) : TextComponent(font) {};
	protected:
		SDL_Surface* createSurface(const string& content, Font* font) override {
			return TTF_RenderText_Shaded(font->get(), content.c_str(), { 255, 255, 255 }, { bg.getRed(), bg.getGreen(), bg.getBlue() });
		}

		constexpr bool pixelFormatChangeable() override { return false; }
	};

	struct MultiColorTextSegment {
		uintmax colorLength = 0;
		Color32 color{};
	};

	class MultiColorText : public TextComponent {
	public:

		MultiColorText(const string& path) : TextComponent(path) {};
		MultiColorText(const string& path, uint16 size) : TextComponent(path, size) {};
		MultiColorText(const ResourceLocation& path) : TextComponent(path) {};
		MultiColorText(const ResourceLocation& path, uint16 size) : TextComponent(path, size) {};
		MultiColorText(Font* font) : TextComponent(font) {};


		void tokenize_input(std::string const& str, const char delim, std::vector<std::string>& out) {
			size_t start;
			size_t end = 0;
			while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
				end = str.find(delim, start);
				out.push_back(str.substr(start, end - start));
			}
		}

		void setContent_Delimeter(const string& content, const char delimeter = ' ') {
			if (content == text_) return;
			if (transform_ && font_ && font_->get()) {
				vector<MultiColorTextSegment> segments{};
				string updatedContent = "";
				vector<string> tokens{};
				tokenize_input(content, delimeter, tokens);
				for (string& token : tokens) {
					if (token.contains("COLOR{") && token.contains("}COLOR")) {
						string colorStr = token.substr(token.find("COLOR{") + 5, token.find("}COLOR") - 4);
						if (json::accept(colorStr)) {
							json js = json::parse(colorStr);
							if (js.contains("text")) {
								if (js.contains("color")) {
									segments.push_back({ to_string(js["text"]).length(), {js["color"][0], js["color"][1], js["color"][2], js["color"][3]} });
									updatedContent += js["text"];
									//segmentCahce += to_string(js["text"]).length();
								}
								else {
									MOTOR_LOG("MultiColorText JSON missing \"color\"");
									segments.push_back({ string{"INVALID COLORTAG"}.length() + 2, {255, 255, 255, 255} });
									updatedContent += " INVALID COLORTAG ";
								}
							}
							else {
								MOTOR_LOG("MultiColorText JSON missing \"text\"");
								segments.push_back({ string{"INVALID COLORTAG"}.length() + 2, {255, 255, 255, 255} });
								updatedContent += " INVALID COLORTAG ";
							}
						}
						else {
							MOTOR_LOG("JSON not accepted: " + colorStr);
							segments.push_back({ string{"INVALID COLORTAG"}.length() + 2, {255, 255, 255, 255} });
							updatedContent += " INVALID COLORTAG ";
						}
					}
					else {
						updatedContent += token + " ";
						segments.push_back({ token.length(), {255, 255, 255, 255} });
					}
				}
				setContent(updatedContent, segments);
			}
		}

		void setContent(const string& content) override {
			if (content == text_) return;
			if (transform_ && font_ && font_->get()) {
				//uintmax segmentSize = 0;
				string updatedContent = "";
				string contentCache = content;
				//int segmentCahce = 1;
				vector<MultiColorTextSegment> segments{};
				/* NEEDS REWORK */
				while (1) {
					if (!contentCache.contains("COLOR{")) {
						/* All White if it doesn't contain any COLOR tag*/
						segments.push_back({ updatedContent.length() - 1, {255, 255, 255, 255} });
						updatedContent += contentCache;
						break;
					}
					uintmax contentPos = contentCache.find("COLOR{");
					//segmentSize += contentPos;
					segments.push_back({ contentPos, {255, 255, 255, 255} });
					updatedContent += contentCache.substr(0, contentPos - 1);
					if (contentCache.contains("}COLOR")) {
						contentCache = contentCache.substr(contentPos + 5);
						string colorStr = contentCache.substr(0, contentCache.find("}COLOR") + 1);
						contentCache = contentCache.substr(contentCache.find("}COLOR") + 7);
						if (json::accept(colorStr)) {
							json js = json::parse(colorStr);
							if (js.contains("text")) {
								if (js.contains("color")) {
									segments.push_back({ to_string(js["text"]).length() - 1, {js["color"][0], js["color"][1], js["color"][2], js["color"][3]} });
									updatedContent += js["text"];
									//segmentCahce += to_string(js["text"]).length();
								}
								else {
									MOTOR_LOG("MultiColorText JSON missing \"color\"");
									segments.push_back({ string{"INVALID COLORTAG"}.length() + 2, {255, 255, 255, 255} });
									updatedContent += " INVALID COLORTAG ";
								}
							}
							else {
								MOTOR_LOG("MultiColorText JSON missing \"text\"");
								segments.push_back({ string{"INVALID COLORTAG"}.length() + 2, {255, 255, 255, 255} });
								updatedContent += " INVALID COLORTAG ";
							}
						}
						else {
							MOTOR_LOG("JSON not accepted: " + colorStr);
							segments.push_back({ string{"INVALID COLORTAG"}.length() + 2, {255, 255, 255, 255} });
							updatedContent += " INVALID COLORTAG ";
						}
						int minus = 0;
						for (char c : updatedContent) {
							if (c == ' ') {
								++minus;
							}
						}
						//segmentSize = updatedContent.length() - minus - segmentCahce;
					}
				}

				for (auto& segment : segments) {
					segment.colorLength -= 1;
				}

				setContent(updatedContent, segments);
			}
		}

		SDL_Surface *SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask) {
    		return SDL_CreateSurface(width, height, SDL_GetPixelFormatEnumForMasks(depth, Rmask, Gmask, Bmask, Amask));
		}

		void setContent(const string& content, const std::vector<MultiColorTextSegment>& segments) {
			text_ = content;
			uint16 loopIt = 0;
			int width = 0;
			int height = 0;
			int lastAdvance = 0;
			int advance = 0;
			int start = 0;
			TTF_SizeText(font_->get(), content.c_str(), &width, &height);

			Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
#endif

			SDL_Surface* masterSurface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
			if (!masterSurface) return;
			for (auto& segment : segments) {
				string str = "";
				int minX = 0;
				for (int i = loopIt; i < segment.colorLength + loopIt; ++i) {
					if (i <= content.length()) {
						char c = content[i];
						str += c;
						int advanceStep = 0;
						TTF_GlyphMetrics(font_->get(), c, &minX, NULL, NULL, NULL, &advanceStep);
						advance += advanceStep;
					}
					else {
						break;
					}
				}
				loopIt += segment.colorLength;
				TTF_SizeText(font_->get(), str.c_str(), &width, &height);
				SDL_Surface* surface = TTF_RenderText_Solid(font_->get(), str.c_str(), { segment.color.getRed(), segment.color.getGreen(), segment.color.getBlue() });
				if (surface) {
					if (pixelFormat_ != 0) {
						SDL_Surface* pixelFormattedSurface = SDL_ConvertSurfaceFormat(surface, pixelFormat_);
						SDL_DestroySurface(surface);
						surface = pixelFormattedSurface;
					}
					if (!surface) return;
					if (start == 0) start = minX;
					SDL_Rect srcRect{ start, 0, width + advance, height };
					SDL_Rect dstRect{ lastAdvance + minX, 0, width + advance, height };
					lastAdvance = advance;
					SDL_BlitSurface(surface, &srcRect, masterSurface, &dstRect);
					//IMG_SavePNG(surface, "sur.png");
				}
				SDL_DestroySurface(surface);
			}
			if (texture_) SDL_DestroyTexture(texture_);
			texture_ = SDL_CreateTextureFromSurface(getriebe.sdl_renderer(), masterSurface);
			//IMG_SavePNG(masterSurface, "master.png");
			SDL_DestroySurface(masterSurface);
		}
	};
}