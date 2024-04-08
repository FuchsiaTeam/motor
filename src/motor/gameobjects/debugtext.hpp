/* Jayson Hartmann */
#pragma once

#include "motor/object/gameobject.hpp"
#include "motor/components/collider.hpp"
#include "motor/components/text.hpp"
#include "motor/components/phy.hpp"
#include "motor/components/text.hpp"
#include "motor/util.hpp"

class DebugText : pObject {
public:
    Motor::Font* font_;
    Motor::Font* outlineFont_;
    DebugText(Motor::Font* font, Motor::Font* outlineFont) {
        this->font_ = font;
        this->outlineFont_ = outlineFont;
    }

    Motor::GameObject* coordinateObject = nullptr;

    void addDebugTextEntry(const string& content, int& index) {
        addComponent<Motor::TextComponentBlended>(font_)->setContent(content);
        getComponent<Motor::TextComponentBlended>(index)->ignoreCamera_ = true;
        getComponent<Motor::TextComponentBlended>(index)->alignment_ = Motor::TextAlignment::LEFT;
        getComponent<Motor::TextComponentBlended>(index)->translate_.setY(30 * index);
        getComponent<Motor::TextComponentBlended>(index)->setOutlineFont(outlineFont_);
        getComponent<Motor::TextComponentBlended>(index)->outlineColor_ = { 37, 46, 61, 255 };
        ++index;
    }

    void registerEvents() override {
        registerEvent(Motor::Events::keyUp.attach(&onKeyUp));
    }

    void onCreate() override {
        toggleShow();
        transform()->position.set(24, 35);
        transform()->scale.set(30 / 2, 48 / 2);
        transform()->depth = 32767;
        //transform()->color.set(125, 15, 15, 190);
        //Anonymous.ttf
        persistent_ = true;
        int index = 0;
        addDebugTextEntry(Fuchs::Utility::getGPUName(0), index);
        addDebugTextEntry("FPS_TIME", index);
        addDebugTextEntry("TEXTFPS_TIME", index);
        addDebugTextEntry("COORDINATES", index);
        addDebugTextEntry("DELTA_TIME", index);
        addDebugTextEntry("RESOLUTION", index);
        addDebugTextEntry("RENDERER_LOGIC", index);
        addDebugTextEntry("RENDERER_DIFF", index);
        addDebugTextEntry("MEMORY", index);
        addDebugTextEntry("SCENEOBJECTS", index);
        addDebugTextEntry("TEXTURES", index);
        addDebugTextEntry("ASPECTRATIO", index);
        addDebugTextEntry("F3 To Show/Hide", index);
        addDebugTextEntry("BUILTDATE", index);
#ifdef STRETCH_ASPECT
        //addDebugTextEntry(debugFont, "DONT STRETCH ASPECT MAKRO DEFINED", index);
#endif
        //addDebugTextEntry(debugFont, "ZOOM", index);
#ifdef LINUX
        Fuchs::Data::LINUX_DESKTOP_ENVIRONMENT env = Fuchs::Utility::getDesktopEnvironment();
		switch (env) {
		case Fuchs::Data::LINUX_DESKTOP_ENVIRONMENT::UNKNOWN: {
			addDebugTextEntry("Desktop: UNKNOWN", index);
			break;
		}

		case Fuchs::Data::LINUX_DESKTOP_ENVIRONMENT::GNOME: {
			addDebugTextEntry("Desktop: GNOME", index);
			break;
		}

		case Fuchs::Data::LINUX_DESKTOP_ENVIRONMENT::PLASMA: {
			addDebugTextEntry("Desktop: KDE PLASMA", index);
			break;
		}

		}
#endif
    }

    std::function<void(SDL_Event&)> onKeyUp = [this](SDL_Event& event) {
        if (event.key.keysym.sym == SDL_KeyCode::SDLK_F3) {
            toggleShow();
            getriebe.getGame()->getCurrentScene()->sortObjects();
        }
    };

    void textUpdate() override {
        Motor::GameObject::textUpdate();
        getComponent<Motor::TextComponentBlended>(1)->setContent("deltaTime: " + to_string(getriebe.getGame()->getDelta().deltaTime));
        getComponent<Motor::TextComponentBlended>(2)->setContent("fps: " + to_string((getriebe.getGame()->getFPS().currentFrames)));
        getComponent<Motor::TextComponentBlended>(3)->setContent("textFps: " + to_string((getriebe.getGame()->getTextFPS().fps)));
        if (coordinateObject) getComponent<Motor::TextComponentBlended>(4)->setContent("X:" + to_string(coordinateObject->getComponent<Motor::TransformComponent>()->position.getX()) + " ; Y:" + to_string(coordinateObject->getComponent<Motor::TransformComponent>()->position.getY()));
        getComponent<Motor::TextComponentBlended>(5)->setContent("Resolution: " + to_string((getriebe.getGame()->getWindow()->width_)) + "x" + to_string((getriebe.getGame()->getWindow()->height_)));
        getComponent<Motor::TextComponentBlended>(6)->setContent("Renderer Logical: " + to_string((getriebe.getGame()->getRenderer()->logicalWidth)) + "x" + to_string((getriebe.getGame()->getRenderer()->logicalHeight)));
        getComponent<Motor::TextComponentBlended>(7)->setContent("Renderer Diff: " + to_string((getriebe.getGame()->getRenderer()->widthDifference_)) + "x" + to_string((getriebe.getGame()->getRenderer()->heightDifference_)));
        double usedMemory = (double)Fuchs::Utility::getCurrentMemoryUsage();
        double totalMemory = (double)Fuchs::Utility::getTotalMemory();
        getComponent<Motor::TextComponentBlended>(8)->setContent("Memory: " + to_string_with_precision(usedMemory / 1024.0 / 1000.0, 3) + "mb/" + to_string_with_precision((double)totalMemory / 1024.0 / 1000.0, 2) + "mb [" + to_string_with_precision(usedMemory / totalMemory * 100.0, 2) + "%]");
        getComponent<Motor::TextComponentBlended>(9)->setContent("Objs in Scene: " + to_string(getriebe.getGame()->getCurrentScene()->getObjects().size()));
        getComponent<Motor::TextComponentBlended>(13)->setContent(string{ __DATE__ } + string{ " ;; " } + __TIME__ + " GMT+1 BERLIN");
        uintmax_t textureSize_ = 0;
        for (Motor::Texture* texture : getriebe.textures_) {
            textureSize_ += texture->getFileSize();
        }
        getComponent<Motor::TextComponentBlended>(10)->setContent(string{ "TEXTURE FILE SIZE: " + to_string(textureSize_) + " BYTES" });
        getComponent<Motor::TextComponentBlended>(11)->setContent(string{ "ASPECT_RATIO: " + to_string(getriebe.getGame()->getWindow()->aspect_ratio_width_) + ":" + to_string(getriebe.getGame()->getWindow()->aspect_ratio_height_) + " (" + to_string(getriebe.getGame()->getRenderer()->aspectRatioWidthDifference_) + ":" + to_string(getriebe.getGame()->getRenderer()->aspectRadioHeightDifference_) + ")" });
        //getComponent<Motor::TextComponentBlended>(12)->setContent("Zoom: " + to_string(getriebe.getGame()->getCurrentScene()->getCamera().zoom));
    }
};