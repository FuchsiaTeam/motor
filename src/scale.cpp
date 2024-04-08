/* Jayson Hartmann 28/03/2022 */
#include "motor/types/scale.hpp"
#include "motor/motor.hpp"
namespace Motor {
	Scale Scale::cameraAdjusted() const {
#ifdef STRETCH_ASPECT
		Scale scale{};
		Motor::Renderer* renderer = getriebe.getGame()->getRenderer();
		float difference_ = max(renderer->heightDifference_, renderer->widthDifference_);
		scale.x = x * difference_ * getriebe.getGame()->getCurrentScene()->getCamera().zoom;
		scale.y = x * difference_ * getriebe.getGame()->getCurrentScene()->getCamera().zoom;
		return scale;
#else
		return Scale{
			(x * getriebe.getGame()->getRenderer()->widthDifference_ * getriebe.getGame()->getRenderer()->aspectRatioWidthDifference_ * getriebe.getGame()->getCurrentScene()->getCamera().zoom),
			(y * getriebe.getGame()->getRenderer()->heightDifference_ * getriebe.getGame()->getRenderer()->aspectRadioHeightDifference_ *  getriebe.getGame()->getCurrentScene()->getCamera().zoom)
		};
#endif
	}
}
