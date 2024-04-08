/* Jayson Hartmann */
#pragma once

#include "motor/interfaces/component.hpp"
#include "motor/components/transform.hpp"
#include "motor/motor.hpp"

namespace Motor {

	class RevolutionComponent : public IComponent {
	public:
		TransformComponent* transformComponent = nullptr;
		float speed_ = 0.01;
	public:

		void init() override {
			transformComponent = getOwner()->getComponent<TransformComponent>();
		}

		void update() override {
			if (transformComponent) {
				transformComponent->rotation.increaseAngle(speed_ * getriebe.getGame()->getDelta().deltaTime);
			}
		}
	};
}