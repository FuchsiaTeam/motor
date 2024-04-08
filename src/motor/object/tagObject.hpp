#pragma once

#include "string"
#include "vector"

namespace Motor {
	class TagObject {
		std::vector<std::string> tags_{};
	public:
		virtual ~TagObject() = default;
	public:
		std::vector<std::string>& getTags() {
			return tags_;
		}

		bool hasTag(const std::string& tag) {
			for (std::string str : tags_) {
				if (str == tag) {
					return true;
				}
			}
			return false;
		}

		void addTag(std::string tag) {
			tags_.push_back(tag);
		}

	};
}