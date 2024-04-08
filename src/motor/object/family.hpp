/* Jayson Hartmann */
#pragma once

#include <fuchslib/shorter.hpp>
#include <fuchslib/ruid.hpp>

namespace Motor {
	
	class Family {
	protected:
		std::vector<Family*> children_{};
		Family* parent_ = nullptr;
		Fuchs::RUID ruid_ = Fuchs::generateRUID("FAM");

	public:
		std::vector<Family*>& getChildren() { return children_; }

		void addChild(Family* child) {
			child->parent_ = this;
			getChildren().push_back(child);
		}

		void setParent(Family* parent) {
			if (parent_ == parent) return;
			int i = -1;
			for (Family* child : getParent()->getChildren()) {
				++i;
				if (child->ruid_ == ruid_) break;
			}
			if(i > -1) getParent()->getChildren().erase(getParent()->getChildren().begin() + i);
			parent->addChild(this);
		}

		Family* getParent() {
			return parent_;
		}

		bool hasParent() {
			return parent_;
		}

		Fuchs::RUID getFamilyRUID() {
			return this->ruid_;
		}

		Family* getKingParent() {
			Family* parent = getParent();
			Family* lastParent = getParent();
			int i = 0;
			constexpr int limit = 1250;
			while (parent) {
				if (parent->hasParent()) lastParent = getParent();
				parent = getParent()->parent_;
				++i;
				if (i > (limit - 1)) break;
			}
			return lastParent;
		}
		
	};

}