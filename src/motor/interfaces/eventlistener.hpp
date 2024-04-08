/* Jayson Hartmann */
#pragma once

#include <fuchslib/shorter.hpp>
#include <fuchslib/ruid.hpp>
#include "events.hpp"
#include <event.h>

namespace Motor {
	class IEventListener {
		vector<Fuchs::RUID> eventRuids_;
	public:
		~IEventListener() {
			for (Fuchs::RUID& ruid : eventRuids_) {
				Motor::eventObjsToRemove_.push_back(ruid);
			}
		}

		void notifyDestroy() {
			for (Fuchs::RUID& ruid : eventRuids_) {
				Motor::eventObjsToRemove_.push_back(ruid);
			}
			eventRuids_.clear();
		}

		void registerEvent(Fuchs::RUID ruid) {
			eventRuids_.push_back(ruid);
		}
	};
}