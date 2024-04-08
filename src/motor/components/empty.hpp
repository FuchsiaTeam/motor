/* Jayson Hartmann 27/03/2022 */
#pragma once

#include "motor/interfaces/component.hpp"

namespace Motor {

	/* Empty Component
	* Just an Empty Component for testing
	*/
	class EmptyComponent : public IComponent {
	public:
		std::string test;
		EmptyComponent(string testf) : test(testf) {};
	};
}