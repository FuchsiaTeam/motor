/* Jayson Hartmann */
#pragma once

#include "motor/interfaces/component.hpp"
#include "motor/components/transform.hpp"
#include "motor/motor.hpp"

namespace Motor {

	class PhyComponent : public IComponent {
	public:
		TransformComponent* transformComponent = nullptr;
		Vector2D force{};
		Vector2D velocity{};
		float friction = 0.01;
		bool stopped = false;
		float speed = 1;
	private:
		
		void __handleForce() {
			float delta = getriebe.getGame()->getDelta().deltaTime;
			float deltaFriction = friction * delta;
			transformComponent->position += {force.getX() * delta, force.getY() * delta};
			if (force.getX() > 0 && !stopped) {
				force -= {deltaFriction, 0};
			}
			else if (force.getX() < 0 && !stopped) {
				force += {deltaFriction, 0};
			}
			if (force.getY() > 0 && !stopped) {
				force -= {0, deltaFriction};
			}
			else if (force.getY() < 0 && !stopped) {
				force += {0, deltaFriction};
			}
			if (abs(force.getX()) > deltaFriction || abs(force.getY()) > deltaFriction) {
				stopped = false;
			}
			else {
				stopped = true;
				force.setX(0);
				force.setY(0);
			};
		}

		void __handleVelocity() {
			transformComponent->position += {velocity.getX() * speed, velocity.getY() * speed};
		}

	public:

		void init() override {
			transformComponent = getOwner()->getComponent<TransformComponent>();
		}

		void update() override {
			if (transformComponent) {
				__handleVelocity();
				__handleForce();
			}
		}
	};
}