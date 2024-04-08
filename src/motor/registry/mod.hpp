#pragma once
#include "string"

namespace Motor {
	class Mod {
		std::string id_;
	public:
		Mod(std::string id) : id_(id) {};

		std::string getId() {
			return id_;
		}
	};
}