/* Jayson Hartmann 27/03/2022 */
#pragma once
#include <fuchslib/ruid.hpp>
#include <sdl/SDL.h>
#include "motor/event.hpp"

namespace Motor {
	class ComponentHolder;
	class Scene;
    fuchs_interface IComponentEvent;
	/* /Abstract Class/
	* Interface for Components
	*/
	fuchs_interface IComponent {
	public:
		Fuchs::RUID ruid_ = Fuchs::generateRUID("COMP", "INF", {8, 4, 15});
		vector<Fuchs::RUID> eventRuids_;
		ComponentHolder* owner_ = nullptr;
		bool awaitingRemove_ = false;
		std::string name_ = "UNNAMED";

		/* De-/Constructors */
	public:
		IComponent() = default;
		virtual ~IComponent() = default;
		friend class Scene;
		friend class ComponentHolder;
	public:
		void inner_destroy() {
			for (Fuchs::RUID& ruid : eventRuids_) {
				Motor::eventObjsToRemove_.push_back(ruid);
			}
		}

		/* Functions */
	public:

		void registerEvent(Fuchs::RUID ruid) {
			eventRuids_.push_back(ruid);
		}

		bool hasOwner() { return this->owner_; }
		ComponentHolder* getOwner() { return this->owner_; }
		void setOwner(ComponentHolder* owner) { this->owner_ = owner; }
		virtual constexpr bool removeable() { return true; }
		/* Should multiple Instances be allowed to add to an Object?*/
		virtual constexpr bool onlySingleInstance() { return true; }

		/* Override if you want a custom RUID */
		virtual void createRuid() {
			ruid_ = Fuchs::generateRUID("COMP", "INF", {8, 4, 15});
		}

		virtual void init() {
			createRuid();
		}

		virtual void update() {}
		virtual void fixedUpdate() {}
		virtual void render() {}
		virtual void renderInterpolated() {}
		virtual void onRemove() {}
		virtual void onObjectDestroy() {}

		void callComponentEvent(IComponentEvent* eventData, bool _delete = true);
	};

    class IComponentEvent {
        //T* component_ = nullptr;
        IComponent* component_ = nullptr;
    public:

        virtual ~IComponentEvent() = default;

        IComponent* getComponent() { return component_; }
		template <typename T, typename... TArgs> 
		static T* create(TArgs&&... mArgs) { return new T(std::forward<TArgs>(mArgs)...); }
        virtual constexpr cstring id() = 0;
    };

	template <typename T>
	concept CIComponent = std::derived_from<T, IComponent> && !std::is_same<T, IComponent>();
}