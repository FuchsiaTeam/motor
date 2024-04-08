/* Jayson Hartmann */
#pragma once

#include <fuchslib/shorter.hpp>
#include "motor/interfaces/component.hpp"
#include "motor/components/transform.hpp"
#include "motor/makros.hpp"
#include "motor/log.hpp"
#include "tagObject.hpp"

namespace Motor {
	
	class ComponentHolder {
	protected:
		std::vector<IComponent*> components_{};

	public:
		virtual ~ComponentHolder() = default;
        friend class IComponent;
    protected:
        /*dtp
            void onComponentEvent(IComponentEvent<T> eventData) {};*/
        virtual void onComponentEvent(IComponentEvent* eventData) {};

	public:
		template <typename T> requires CIComponent<T>
		int hasComponent() const {
			int i = 0;
			for (IComponent* component : components_) {
				if (!component) break;
				T* ptr = dynamic_cast<T*>(component);
				if (ptr) {
					++i;
					continue;
				}
			}
			return i;
		}

		template <typename T> requires CIComponent<T>
		bool canAddComponent() const {
			for (IComponent* component : components_) {
				T* ptr = dynamic_cast<T*>(component);
				if (ptr) {
					if (ptr->onlySingleInstance()) {
						return false;
					}
					return true;
				}
			}
			return true;
		}

		template <typename T, typename... TArgs> requires CIComponent<T>
		T* addComponent(TArgs&&... mArgs) {
			if (canAddComponent<T>()) {
				T* c(new T(std::forward<TArgs>(mArgs)...));
				c->owner_ = this;
				c->init();
				components_.emplace_back(c);
				return c;
			}
			else {
				MOTOR_LOG("Tried to add Component, but can't");
			}
			return nullptr;
		}

		template<typename T> requires CIComponent<T>
		T* getComponent() const {
			for (IComponent* component : components_) {
				//if (component->id_ == T.id_) return static_cast<T*>(component);
				T* ptr = dynamic_cast<T*>(component);
				if (ptr) return ptr;
			}
			return nullptr;
		}

		template<typename T> requires CIComponent<T>
		T* getComponent(const Fuchs::RUID& ruid) const {
			for (IComponent* component : components_) {
				T* ptr = dynamic_cast<T*>(component);
				if(ptr->ruid_ == ruid) return ptr;
			}
			return nullptr;
		}

		template<typename T> requires CIComponent<T>
		T* getComponent(uint16 index) const {
			vector<T*> comps = getMultipleComponent<T>();
			if (comps.size() < index) {
				MOTOR_LOG(std::string{ "INDEX{" + to_string(index) + "} bigger than SIZE{" + to_string(comps.size()) + "}"});
				return nullptr;
			}
			return comps[index];
		}

		template<typename T> requires CIComponent<T>
		T* getComponentByName(const std::string& name) const {
			for(IComponent* component : components_) {
				T* ptr = dynamic_cast<T*>(component);
				if(ptr->name_ == name) return ptr;
			}
			return nullptr;
		}

		/*
		auto getComponent(const Fuchs::RUID& ruid) {
			for (IComponent* component : components_) {
				if(component->ruid_ == ruid) return component;
			}
			return components_[0];
		}*/

		template<typename T> requires CIComponent<T>
		vector<T*> getMultipleComponent() const {
			vector<T*> vec{};
			for (IComponent* component : components_) {
				T* ptr = dynamic_cast<T*>(component);
				if (ptr) {
					vec.push_back(ptr);
				}
			}
			return vec;
		}

		void validate();

		template <typename T> requires CIComponent<T>
		void removeComponent() {
			for (IComponent* component : components_) {
				if (!component->removeable()) {
					MOTOR_LOG("Tried to remove unremoveable component");
					continue;
				}
				//if (component->id_ == T.id_) return static_cast<T*>(component);
				T* ptr = dynamic_cast<T*>(component);
				if (ptr) {
					component->onRemove();
					component->awaitingRemove_ = true;
				};
			}
			validate();
		}

		template <typename T> requires CIComponent<T>
		void removeComponent(uint8 index) {
			vector<IComponent> comps{};
			for (IComponent* component : components_) {
				if (!component->removeable()) continue;
				T* ptr = dynamic_cast<T*>(component);
				if (ptr) comps.push_back(ptr);
			}
			if (comps.size() > index) comps[index].awaitingRemove_ = true;
			validate();
		}

		void removeComponent(const Fuchs::RUID& ruid) {
			for (IComponent* component : components_) {
				if (!component->removeable()) continue;
				if (component->ruid_ == ruid) {
					component->onRemove();
					component->awaitingRemove_ = true;
				};
			}
			validate();
		}
	};

}