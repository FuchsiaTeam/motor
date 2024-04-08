/* Jayson Hartmann 27/03/2022 */
#pragma once

#include "motor/components/transform.hpp"

namespace Motor {

    class GameObject;
	class ObjectHolder;
	/* Game Camera
	* 
	*/
	class Camera {
	public:
		TransformComponent* target = nullptr;
		Vector2D position{};
		Scale scale{1920, 1080};
		float zoom = 1;
		bool enableX = true;
		bool enableY = true;
		Vector2D translate{};
	public:
		void update();
		void setTarget(GameObject* object);

		TransformComponent* getTarget() {
			return this->target;
		}

		void reset() {
			translate.set(0, 0);
			enableX = true;
			enableY = true;
			target = nullptr;
		}
	};
}