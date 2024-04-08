/* Jayson Hartmann  27/03/2022 */
#pragma once

#include "motor/interfaces/component.hpp"
#include "motor/types/vector.hpp"
#include "motor/types/scale.hpp"
#include "motor/types/color.hpp"
#include "motor/types/rotation.hpp"

namespace Motor {

	class TransformComponent : public IComponent {
	public:
		Vector2D position{};
		Scale scale{};
		Color32 color{};
		Rotation rotation{};
		//bool moveWithParent = true;
		bool onScreen = true;
		int16 depth = 0;
		bool followCamera = true;

		float movingToTargetSpeed = 0.1;
		const TransformComponent* movingToTarget = nullptr;

		constexpr bool removeable() override { return false; }

		void moveTo(const TransformComponent* target, float speed) {
			movingToTarget = target;
			movingToTargetSpeed = speed;
		}

		bool isMovingToObject() {
			return movingToTarget;
		}

		void update() override;

	};

}
