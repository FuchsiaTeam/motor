#pragma once

#include "string"
#include "motor/registry/mod.hpp"
#include <fstream>

extern Motor::Mod* fallbackMod;

namespace Motor {
	class ResourceLocation {
		std::string path_;
		Mod* mod = nullptr;
	public:

		ResourceLocation(Mod* mod, std::string path) {
			path_ = "assets/" + mod->getId() + "/" + path;
			this->mod = mod;
			std::ifstream f(path_.c_str());
			if (!f.good() && fallbackMod) {
				path_ = "assets/" + fallbackMod->getId() + "/" + path;
			}
		}

		std::string getPath() {
			return this->path_;
		}
	};
}