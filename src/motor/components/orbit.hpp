/* Jayson Hartmann */
#pragma once

#include "motor/interfaces/component.hpp"
#include "motor/components/transform.hpp"
#include "motor/motor.hpp"

namespace Motor {

	class OrbitComponent : public IComponent {
	public:
		TransformComponent* transformComponent = nullptr;
		TransformComponent* targetComponent = nullptr;
		Vector2D offset_{};
		float angle_ = 0;
		float speed_ = 0.01;
	public:

		void init() override {
			transformComponent = getOwner()->getComponent<TransformComponent>();
		}

		void update() override {
			if (transformComponent && targetComponent) {
				angle_ += speed_ * getriebe.getGame()->getDelta().deltaTime;
				transformComponent->position.setX(targetComponent->position.getX()
					+ (cos(angle_) * offset_.getX()) + (-sin(angle_) * offset_.getY()));

				transformComponent->position.setY(targetComponent->position.getY()
					+ (sin(angle_) * offset_.getX()) + (cos(angle_) * offset_.getY()));
				if (angle_ > 359) {
					angle_ = 0;
				}
			}
		}
	};
}