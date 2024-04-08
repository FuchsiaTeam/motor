/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <fuchslib/shorter.hpp>
#include <fuchslib/ruid.hpp>
#include <motor/interfaces/component.hpp>
#include <motor/components/transform.hpp>
#include <sdl/SDL.h>
#include <motor/events.hpp>
#include "componentholder.hpp"
#include "motor/classdef.hpp"
#include "family.hpp"
#include <algorithm>
#include <numeric>
#include <future>
#include <mutex>
#include "dataObject.hpp"

namespace Motor {
	class Scene;
	class Game;
	class ObjectHolder;

	/* Gameobject always has a TransformComponent */
	class GameObject : public ComponentHolder, public TagObject, public DataObject, public Family {
	protected:
		bool awaitingDestroy_ = false;
		bool active_ = true;
		bool persistent_ = false;
		bool created_ = false;
		bool hidden_ = false;
		/* ID of Object, used by mechanics such as .findByRUID() */
		Fuchs::RUID ruid_;
		vector<GameObject*> children_;
		vector<Fuchs::RUID> eventRuids_;
		ObjectHolder** holder_ = nullptr;
		Family family_{};
		string tag_ = "untagged";
		/* Name used in .find() */
		string name_ = "Unnamed GameObject";
	public:
		GameObject() {
			addComponent<TransformComponent>();
		}

		GameObject(bool addToScene) {
			addComponent<TransformComponent>();
			if (addToScene) addToCurrentScene();
		}

		GameObject(string name) : name_(name) {
			addComponent<TransformComponent>();
		}

		GameObject(bool addToScene, string name) : name_(name) {
			addComponent<TransformComponent>();
			if (addToScene) addToCurrentScene();
		}

		virtual ~GameObject() = default;

		friend class Scene;
	private:
		void inner_create() {
			registerEvents();
		}
		/* Functions */
	public:

		Fuchs::RUID& getRUID() {
			return this->ruid_;
		}

		Family& getFamily() {
			return this->family_;
		}

		bool isHidden() {
			return this->hidden_;
		}

		std::vector<IComponent*>& getComponentVector() {
			return this->components_;
		}

		void registerEvent(Fuchs::RUID ruid) {
			eventRuids_.push_back(ruid);
		}

		virtual void onCreate() {}
		virtual void onSceneSwitch(Scene* currentScene, Scene* newScene, bool& shouldDelete) {}

		void startTextInput() {
			SDL_StartTextInput();
		}

		void stopTextInput() {
			SDL_StopTextInput();
		}

		/*
			Called when created
			example:
			void registerEvents() override {
				registerEvent(Motor::Events::mouseClick.attach(&onMouseClick));
				registerEvent(Motor::Events::keyUp.attach(&onUpDown));
			}
		*/
		virtual void registerEvents() {}

		ObjectHolder** getHolder() {
			return this->holder_;
		}

		uintmax getEstimatedByteSize() {
			uintmax comps = 0;
			for (IComponent* component : components_) comps += sizeof(*component);
			return sizeof(*this) + comps;
		}

		uintmax getEstimatedBitSize() {
			return getEstimatedByteSize() * 8;
		}

		void addToScene(Scene* scene, bool sortObj = true);
		void addToCurrentScene(bool sortObj = true);

		vector<Fuchs::RUID>& getEventRUIDS() {
			return this->eventRuids_;
		}


		virtual void update() {
			for (IComponent* component : components_) {
				//if (((uintptr_t)component == UINTPTR_MAX)) continue;
				component->update();
			}

		}

		virtual void postUpdate() {

		}

		virtual void fixedUpdate() {
			for (IComponent* component : components_) {
				//if (((uintptr_t)component == UINTPTR_MAX)) continue;
				component->fixedUpdate();
			}
		}

		/* Use this to update Text */
		virtual void textUpdate() {}

		virtual void render() {
			if (hidden_) { return; }
			if (awaitingDestroy_) { return; }
			for (IComponent* component : components_) {
				//if (((uintptr_t)component == UINTPTR_MAX)) continue;
				component->render();
			}
		}

		virtual void renderInterpolated() {
			if (hidden_) { return; }
			if (awaitingDestroy_) { return; }
			for (IComponent* component : components_) {
				//if (((uintptr_t)component == UINTPTR_MAX)) continue;
				component->renderInterpolated();
			}
		}

		void hide() {
			hidden_ = true;
		}

		void show() {
			hidden_ = false;
		}

		void toggleShow() {
			hidden_ = !hidden_;
		}

		TransformComponent* transform() { return this->getComponent<TransformComponent>(); }

		void setActive(bool active) { this->active_ = active; }
		void toggleActive() { setActive(!isActive()); }

		bool isActive() const { return active_; }
		bool isAwaitingDestroy() const { return awaitingDestroy_; }
		bool isPersistent() const { return persistent_; }

		void destroy() {
			setActive(false);
			awaitingDestroy_ = true;
			for (IComponent* component : components_) {
				component->owner_ = nullptr;
			}
		}

		static GameObject* find(const string& name);
		static GameObject* findByRUID(const Fuchs::RUID& ruid);
		static vector<GameObject*> findByTag(const string& tag);

		static GameObject* find(const string& name, Scene* scene);
		static GameObject* findByRUID(const Fuchs::RUID& ruid, Scene* scene);
		static vector<GameObject*> findByTag(const string& tag, Scene* scene);

	};

#define pObject public Motor::GameObject
}
inline void to_json(json& j, const Motor::GameObject& p) {
	/* j = json{
	 {"ruid", p.getRUID()}
	 };*/
}
inline void from_json(const json& j, Motor::GameObject& p) {
	if (j.contains("ruid")) {

	}
}