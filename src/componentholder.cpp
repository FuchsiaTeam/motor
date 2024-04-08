#include "motor/object/componentholder.hpp"

namespace Motor {
	void ComponentHolder::validate() {
		std::erase_if(components_, [](IComponent* obj) { 
			bool yes = obj->awaitingRemove_;
			if (yes) {
				obj->inner_destroy();
				delete obj;
			}
			return yes; 
		});
	}
}