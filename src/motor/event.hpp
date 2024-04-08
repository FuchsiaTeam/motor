/* Jayson Hartmann */
#pragma once

#include <functional>
#include <unordered_map>
#include <fuchslib/shorter.hpp>
#include <fuchslib/ruid.hpp>
#include <motor/log.hpp>
#include <algorithm>
#include <numeric>
#include <future>
#include <mutex>
#include <execution>

namespace Motor {
	#ifdef __WIN32
		#define MOTOR_DLL_API _declspec(dllexport)
	#else
		#define MOTOR_DLL_API 
	#endif

	MOTOR_DLL_API extern vector<Fuchs::RUID> eventObjsToRemove_;

	template<typename TFunction, typename... TArgs>
	class Event {
		std::unordered_map<Fuchs::RUID, TFunction*> listeners_{};

	public:

		void validate() {
			for (Fuchs::RUID& ruid : eventObjsToRemove_) {
				auto it = listeners_.begin();
				for (auto& entry : listeners_) {
					if (entry.first == ruid) {
						listeners_.erase(it);
						ruid = "";
						break;
					}
					it++;
				}
			}
			std::erase_if(eventObjsToRemove_, [](Fuchs::RUID& ruid) { return ruid == ""; });
		}

		void dispatch(TArgs&&... args) {
			validate();
			for (auto& entry : listeners_) {
				if (entry.second) {
					TFunction func = *entry.second;
					func(args...);
				}
			}
		}

		/*void disptach_async(TArgs&&... args) {
			validate();
			auto dispatchTask = std::async(std::launch::async,
				[this, args...]() {
					std::for_each(std::execution::par,
						listeners_.begin(),
						listeners_.end(),
						[](auto& entry) {
							TFunction func = *entry.second;
							func(args...);
						});
				});
		}*/

		void dispatch_copy(TArgs... args) {
			validate();
			for (auto& entry : listeners_) {
				if (entry.second) {
					TFunction func = *entry.second;
					func(args...);
				}
			}
		}

		Fuchs::RUID attach(TFunction* function) {
			Fuchs::RUID ruid = Fuchs::generateRUID();
			listeners_.emplace(std::pair(ruid, function));
			return ruid;
		}

		void detach(const Fuchs::RUID& ruid) {
			int i = 0;
			for (auto& entry : listeners_) {
				if (entry.first == ruid) {
					break;
				}
				i++;
			}
			listeners_.erase(listeners_.begin() + i);
		}
	};
}