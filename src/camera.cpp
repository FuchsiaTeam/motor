/* Jayson Hartmann 27/03/2022 */
#include "motor/object/camera.hpp"
#include "motor/motor.hpp"
#include "motor/components/sprite.hpp"
#include "motor/object/scene.hpp"
#include "motor/object/gameobject.hpp"

namespace Motor {
	void Camera::update() {
		if (target) {
            Vector2D pos = target->position.fixedScreen();
            Scale adjScale = scale.cameraAdjusted();
            //position.set((-pos.getX() + (adjScale.x / 2 / zoom / getriebe.getGame()->getRenderer()->aspectRatioWidthDifference_)), (-pos.getY() + (adjScale.y / 2 / zoom / getriebe.getGame()->getRenderer()->aspectRadioHeightDifference_)));
            Vector2D ve = translate.fixedScreen();
            if (enableX) position.setX((-pos.getX() + (adjScale.x / 2 / zoom / getriebe.getGame()->getRenderer()->aspectRatioWidthDifference_)) + ve.getX());
            if (enableY) position.setY((-pos.getY() + (adjScale.y / 2 / zoom / getriebe.getGame()->getRenderer()->aspectRadioHeightDifference_)) + ve.getY());
		}
	}
    void Camera::setTarget(GameObject* object) {
        target = object->getComponent<TransformComponent>();
    }
}