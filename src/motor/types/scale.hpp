/* Jayson Hartmann 27/03/2022 */
#pragma once

namespace Motor {
	struct Scale {
		float x = 32;
		float y = 32;

		Scale& set(float xIn, float yIn) {
			this->x = xIn;
			this->y = yIn;
			return *this;
		}

		Scale& set(float xyIn) {
			return set(xyIn, xyIn);
		}

		Scale cameraAdjusted() const;
	};
}