#pragma once
#include "keyable.hpp"

namespace Motor {
	template <typename T>
    class RegistryObject : public KeyAble {
		T* reference_;
	public:

		RegistryObject() = default;

		RegistryObject(T* ref, Fuchs::FuchsKey key) {
			this->reference_ = ref;
			forceSetKey(key);
		}

		T* original() {
			return reference_;
		}

		T* clone() {
			return new T(*reference_);
		}
	};
}