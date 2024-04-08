/* Jayson Hartmann  27/03/2022 */
#pragma once

#include "motor/interfaces/component.hpp"
#include "motor/content/texture.hpp"
#include "motor/components/transform.hpp"
#include "motor/motor.hpp"

namespace Motor {

    /* Sprite Component
    * Used to render Sprites
    */
    class SpriteComponent : public IComponent {
    protected:
        Texture* texture_ = nullptr;

    public:
        bool ignoreCamera_ = false;
        //bool ui_ = false;
        bool translatePosition_ = true;
        bool translateScale_ = true;
        SDL_FRect destRectangle_{};
        TransformComponent* transform_ = nullptr;
        Motor::Event<std::function<void(bool&)>, bool&> preRenderEvent{};
        SDL_BlendMode blendMode_ = SDL_BLENDMODE_NONE;
        SDL_FPoint center_{};
        SDL_RendererFlip flip_ = SDL_RendererFlip::SDL_FLIP_NONE;
        Vector2D translate_{ 0, 0 };
        Color32 customColor_{ 0, 0, 0, 0 };
        bool useCustomColor_ = false;
        bool translateExtraPostion_ = true;
        Scale customScale_{};
        bool useCustomScale_ = false;
        float customAngle_ = 500;
        bool loadAtInit_ = true;

    protected:
        SpriteComponent() = default;

    public:
        /* Textures should never be owned by a Component, better create a list with textures, this is just for quick testing */
        explicit SpriteComponent(const string& path) : texture_(new Texture{ path }) {};
        explicit SpriteComponent(ResourceLocation path) : texture_(new Texture{ path }) {};
        explicit SpriteComponent(const string& path, uint32 pixelFormat) : texture_(new Texture{ path, pixelFormat }) {};
        /* Use This */
        explicit SpriteComponent(Texture* texture) : texture_(texture) {};

        //explicit SpriteComponent(const string& path, bool loadAtInit) : texture_(new Texture{ path }), loadAtInit_(loadAtInit) {};
        //explicit SpriteComponent(ResourceLocation path, bool loadAtInit) : texture_(new Texture{ path }), loadAtInit_(loadAtInit) {};
        //explicit SpriteComponent(const string& path, uint32 pixelFormat, bool loadAtInit) : texture_(new Texture{ path, pixelFormat }), loadAtInit_(loadAtInit) {};
        /* Use This */
        //explicit SpriteComponent(Texture* texture, bool loadAtInit) : texture_(texture), loadAtInit_(loadAtInit) {};

    public:

        void useSizeCenter() {
            if(!useCustomScale_) {
                center_.x = transform_->scale.x / 2;
                center_.y = transform_->scale.y / 2;
            } else {
                center_.x = customScale_.x / 2;
                center_.y = customScale_.y / 2;
            }
        }

        void createRuid() override {
            ruid_ = Fuchs::generateRUID("COMP", "SPR", { 8, 4, 15 });
        }

        void setTexture(Texture* texture) {
            this->texture_ = texture;
        }

        Texture* getTexture() {
            return this->texture_;
        }

        void changeTexture(const string& path, bool unload = false) {
            if (this->texture_ && unload) this->texture_->unload();
            this->texture_ = new Texture{ path };
            this->texture_->load();
        }

        void init() override {
            IComponent::init();
            if (loadAtInit_) texture_->safeLoad();
            transform_ = owner_->getComponent<TransformComponent>();
            if (!transform_) owner_->addComponent<TransformComponent>();
            transform_ = owner_->getComponent<TransformComponent>();
        }

        void update(TransformComponent* transform) {
            if (transform) {
                Vector2D pos = Vector2D{ transform->position };
                pos += translate_;
                Vector2D screenPos = translatePosition_ ? pos.screen() : pos;
                Scale adjustedScale = translateScale_ ? transform->scale.cameraAdjusted() : transform->scale;
                if (useCustomScale_) {
                    adjustedScale = translateScale_ ? customScale_.cameraAdjusted() : customScale_;
                }
                if (ignoreCamera_) {
                    screenPos = pos.fixedScreen();
                    if (getriebe.getGame()->getCurrentScene()->getCamera().target != transform) {
                        adjustedScale.x /= getriebe.getGame()->getCurrentScene()->getCamera().zoom;
                        adjustedScale.y /= getriebe.getGame()->getCurrentScene()->getCamera().zoom;
                    }
                }

                destRectangle_.x = ((screenPos.getX() - (adjustedScale.x / 2)));
                destRectangle_.y = ((screenPos.getY() - (adjustedScale.y / 2)));
                destRectangle_.w = adjustedScale.x;
                destRectangle_.h = adjustedScale.y;

                TransformComponent* cameraTarget = getriebe.getGame()->getCurrentScene()->getCamera().getTarget();
                if (cameraTarget) {
                    if (cameraTarget == transform) {
                        destRectangle_.x += getriebe.getGame()->getCurrentScene()->getCamera().position.getX();
                        destRectangle_.y += getriebe.getGame()->getCurrentScene()->getCamera().position.getY();
                    }
                }
            }
        }

        void update() override {
            update(transform_);
        };

        virtual void render(TransformComponent* transform) {
            if (texture_ && texture_->get()) {
                bool shouldRender = true;
                preRenderEvent.dispatch(shouldRender);
                if (!shouldRender) return;
                float angle = 0;
                if (transform) {
                    if (!transform->onScreen) return;
                    SDL_SetTextureColorMod(texture_->get(), transform->color.getRed(), transform->color.getGreen(), transform->color.getBlue());
                    SDL_SetTextureAlphaMod(texture_->get(), transform->color.getAlpha());
                    angle = transform->rotation.getAngle();
                }
                if (customAngle_ != 500) {
                    angle = customAngle_;
                }
                if (useCustomColor_) {
                    SDL_SetTextureColorMod(texture_->get(), customColor_.getRed(), customColor_.getGreen(), customColor_.getBlue());
                    SDL_SetTextureAlphaMod(texture_->get(), customColor_.getAlpha());
                }
                SDL_SetTextureBlendMode(texture_->get(), blendMode_);
                SDL_SetRenderTarget(getriebe.sdl_renderer(), texture_->get());
                SDL_FPoint centerAdjusted = center_;
                centerAdjusted.x = center_.x * getriebe.getGame()->getRenderer()->widthDifference_;
                centerAdjusted.y = center_.y * getriebe.getGame()->getRenderer()->heightDifference_;
                SDL_RenderTextureRotated(getriebe.sdl_renderer(), texture_->get(), NULL, &destRectangle_, angle, &centerAdjusted, flip_);
                SDL_SetTextureColorMod(texture_->get(), 255, 255, 255);
                SDL_SetTextureAlphaMod(texture_->get(), 255);
                SDL_SetTextureBlendMode(texture_->get(), SDL_BLENDMODE_NONE);
            }
        }

        void render() override {
            render(transform_);
        }

        constexpr bool onlySingleInstance() override { return false; }
    };

    class DynamicSpriteComponent : public SpriteComponent {
    public:
        explicit DynamicSpriteComponent(const string& path) : SpriteComponent(path) {};
        explicit DynamicSpriteComponent(const string& path, uint32 pixelFormat) : SpriteComponent(path, pixelFormat) {};
        explicit DynamicSpriteComponent(DynamicTexture* texture) { texture_ = texture; };
        explicit DynamicSpriteComponent(ResourceLocation path) { texture_ = new DynamicTexture(path); };


        //explicit DynamicSpriteComponent(const string& path, bool loadAtInit) : SpriteComponent(path, loadAtInit) {};
        //explicit DynamicSpriteComponent(const string& path, uint32 pixelFormat, bool loadAtInit) : SpriteComponent(path, pixelFormat, loadAtInit) {};
        //explicit DynamicSpriteComponent(DynamicTexture* texture, bool loadAtInit) { texture_ = texture; loadAtInit_ = loadAtInit; };
        //explicit DynamicSpriteComponent(ResourceLocation path, bool loadAtInit) { texture_ = new DynamicTexture(path); loadAtInit_ = loadAtInit; };

    public:
        void init() override {
            SpriteComponent::init();
            DynamicTexture* dynTexture = dynamic_cast<DynamicTexture*>(texture_);
            if (dynTexture) {
                if (dynTexture->getHolders().empty() && !dynTexture->exists()) dynTexture->load();
                dynTexture->getHolders().emplace_back(this->ruid_);
            }
        }

        void onObjectDestroy() override {
            DynamicTexture* dynTexture = dynamic_cast<DynamicTexture*>(texture_);
            if (dynTexture) {
                std::erase_if(dynTexture->getHolders(), [this](Fuchs::RUID& objRuid) { return objRuid == this->ruid_; });
                dynTexture->validate();
            }
        }

        void onRemove() override {
            onObjectDestroy();
        }
    };

    class SpriteSheetComponent : public SpriteComponent {
    protected:
        SDL_FRect spriteRect_{};
        int rows = 0;
        int columns = 0;
    public:

        explicit SpriteSheetComponent(const string& path) : SpriteComponent(path) {};
        explicit SpriteSheetComponent(const string& path, uint32 pixelFormat) : SpriteComponent(path, pixelFormat) {};
        explicit SpriteSheetComponent(Texture* texture) : SpriteComponent(texture) {};
        explicit SpriteSheetComponent(ResourceLocation path) : SpriteComponent(path) {};

        explicit SpriteSheetComponent(const string& path, int row, int column) : SpriteComponent(path) { setRowColumn(row, column); };
        explicit SpriteSheetComponent(const string& path, uint32 pixelFormat, int row, int column) : SpriteComponent(path, pixelFormat) { setRowColumn(row, column); };
        explicit SpriteSheetComponent(Texture* texture, int row, int column) : SpriteComponent(texture) { setRowColumn(row, column); };
        explicit SpriteSheetComponent(ResourceLocation path, int row, int column) : SpriteComponent(path) { setRowColumn(row, column); };


        SDL_FRect getSpriteRect() {
            return spriteRect_;
        }

        void setRowColumn(int row, int column) {
            spriteRect_.w = getTexture()->getWidth() / column;
            spriteRect_.h = getTexture()->getHeight() / row;
            this->rows = row;
            this->columns = column;
        }

        void selectSprite(int x, int y) {
            spriteRect_.x = x * spriteRect_.w;
            spriteRect_.y = y * spriteRect_.h;
        }

        void render(TransformComponent* transform) override {
            if (texture_ && texture_->get()) {
                bool shouldRender = true;
                preRenderEvent.dispatch(shouldRender);
                if (!shouldRender) return;
                float angle = 0;
                if (transform) {
                    SDL_SetTextureColorMod(texture_->get(), transform->color.getRed(), transform->color.getGreen(), transform->color.getBlue());
                    SDL_SetTextureAlphaMod(texture_->get(), transform->color.getAlpha());
                    angle = transform->rotation.getAngle();
                    //if (!transform->onScreen) return;
                }
                if (customAngle_ != 500) {
                    angle = customAngle_;
                }
                if (useCustomColor_) {
                    SDL_SetTextureColorMod(texture_->get(), customColor_.getRed(), customColor_.getGreen(), customColor_.getBlue());
                    SDL_SetTextureAlphaMod(texture_->get(), customColor_.getAlpha());
                }
                SDL_SetTextureBlendMode(texture_->get(), blendMode_);
                SDL_SetRenderTarget(getriebe.sdl_renderer(), texture_->get());
                SDL_FPoint centerAdjusted = center_;
                centerAdjusted.x = center_.x * getriebe.getGame()->getRenderer()->widthDifference_;
                centerAdjusted.y = center_.y * getriebe.getGame()->getRenderer()->heightDifference_;
                SDL_RenderTextureRotated(getriebe.sdl_renderer(), texture_->get(), &spriteRect_, &destRectangle_, angle, &centerAdjusted, flip_);
                SDL_SetTextureColorMod(texture_->get(), 255, 255, 255);
                SDL_SetTextureAlphaMod(texture_->get(), 255);
                SDL_SetTextureBlendMode(texture_->get(), SDL_BLENDMODE_NONE);
            }
        }
    };

    enum AnimationSheetType {
        ROW,
        COLUMN
    };

    class SpriteAnimationFinishEvent : public IComponentEvent {
    public:
        SpriteAnimationFinishEvent() = default;
        bool* run_ = nullptr;
        static constexpr cstring id_ = "spriteanimation";
        constexpr cstring id() override {
            return id_;
        };
    };

    class SpriteAnimationComponent : public SpriteSheetComponent {
        int frameTime_ = 0;
        int currentSprite_;
        AnimationSheetType sheetType_ = AnimationSheetType::COLUMN;
        int rowColumnSelection_ = 0;
    public:
        bool run_ = true;

        explicit SpriteAnimationComponent(const string& path) : SpriteSheetComponent(path) {};
        explicit SpriteAnimationComponent(const string& path, uint32 pixelFormat) : SpriteSheetComponent(path, pixelFormat) {};
        explicit SpriteAnimationComponent(Texture* texture) : SpriteSheetComponent(texture) {};
        explicit SpriteAnimationComponent(ResourceLocation path) : SpriteSheetComponent(path) {};

        explicit SpriteAnimationComponent(const string& path, int row, int column) : SpriteSheetComponent(path, row, column) {};
        explicit SpriteAnimationComponent(const string& path, uint32 pixelFormat, int row, int column) : SpriteSheetComponent(path, pixelFormat, row, column) {};
        explicit SpriteAnimationComponent(Texture* texture, int row, int column) : SpriteSheetComponent(texture, row, column) {};
        explicit SpriteAnimationComponent(ResourceLocation path, int row, int column) : SpriteSheetComponent(path, row, column) {};

        AnimationSheetType getSheetType() {
            return sheetType_;
        }

        void setRowColumnSelection(int selection) {
            this->rowColumnSelection_ = selection;
        }

        int getRowColumnSelection() {
            return rowColumnSelection_;
        }

        void setSheetType(AnimationSheetType sheetType) {
            this->sheetType_ = sheetType;
        }

        void setFrameTime(int frameTime) {
            this->frameTime_ = frameTime;
        }

        int getFrameTime() {
            return this->frameTime_;
        }

        float frameTimeWaiter_ = 0;
        void render() override {
            SpriteSheetComponent::render(transform_);
            if(!run_) return;
            float delta = getriebe.getGame()->getDelta().deltaTime;
            frameTimeWaiter_ += delta;
            if(frameTimeWaiter_ > (float)frameTime_) {
                frameTimeWaiter_ = 0;
                switch(sheetType_) {
                    case ROW: {
                        ++currentSprite_;
                        if(currentSprite_ >= rows) {
                            currentSprite_ = 0;
                            auto* event = new SpriteAnimationFinishEvent();
                            event->run_ = &run_;
                            callComponentEvent(event);
                        }
                        selectSprite(rowColumnSelection_, currentSprite_);
                        break;
                    }

                    case COLUMN: {
                        ++currentSprite_;
                        if(currentSprite_ >= columns) {
                            currentSprite_ = 0;
                            auto* event = new SpriteAnimationFinishEvent();
                            event->run_ = &run_;
                            callComponentEvent(event);
                        }
                        selectSprite(currentSprite_, rowColumnSelection_);
                        break;
                    }
                }
            }
        }
    };
}
