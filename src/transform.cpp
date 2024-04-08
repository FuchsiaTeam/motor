#include "motor/components/transform.hpp"
#include "motor/motor.hpp"

void Motor::TransformComponent::update() {
	if (isMovingToObject()) {
		Vector2D difference{movingToTarget->position};
		difference.subtract(position);
		difference.normalize();
		difference.multiply(movingToTargetSpeed * getriebe.getGame()->getDelta().deltaTime);
		position.add(difference);

		if (movingToTarget->position.distance(position) < 3) {
			movingToTarget = nullptr;
		}
	}
}
