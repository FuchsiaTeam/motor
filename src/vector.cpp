/* Jayson Hartmann 27/03/2022 */
#include "motor/types/vector.hpp"
#include "motor/motor.hpp"

namespace Motor {
	Vector2D Vector2D::screen() {
        Vector2D fixed = fixedScreen();
		return Vector2D{
                ((fixed.x_ + getriebe.getGame()->getCurrentScene()->getCamera().position.x_) * getriebe.getGame()->getCurrentScene()->getCamera().zoom),
                ((fixed.y_ + getriebe.getGame()->getCurrentScene()->getCamera().position.y_) * getriebe.getGame()->getCurrentScene()->getCamera().zoom)
        };
	}

    /*Vector2D Vector2D::screen() {
        Vector2D fixed = fixedScreen();
        return Vector2D{
                (((float)getriebe.getGame()->getWindow()->width_) * ((fixed.x_ / getriebe.getGame()->getRenderer()->logicalWidth)) + getriebe.getGame()->getCurrentScene()->getCamera().position.x_) / getriebe.getGame()->getRenderer()->widthDifference_,
                (((float)getriebe.getGame()->getWindow()->height_) * ((fixed.y_ / getriebe.getGame()->getRenderer()->logicalHeight)) + getriebe.getGame()->getCurrentScene()->getCamera().position.y_) / getriebe.getGame()->getRenderer()->heightDifference_,
                };
    }*/


	Vector2D Vector2D::screenCenter(const Scale& scale) {
		return Vector2D{ (this->x_ - getriebe.getGame()->getCurrentScene()->getCamera().position.x_) + (scale.x / 2), (this->y_ - getriebe.getGame()->getCurrentScene()->getCamera().position.y_) - (scale.y / 2)};
	}

	Vector2D Vector2D::center(const Scale& scale) {
		return Vector2D{ (this->x_) + (scale.x * getriebe.getGame()->getCurrentScene()->getCamera().zoom / 2), (this->y_) + (scale.y * getriebe.getGame()->getCurrentScene()->getCamera().zoom  / 2) };
	}

    Vector2D Vector2D::fixedScreen() {
        float width = getriebe.getGame()->getRenderer()->logicalWidth;
        float height = getriebe.getGame()->getRenderer()->logicalHeight;
        return Vector2D{
            getriebe.getGame()->getWindow()->width_ * (x_ / width),
            getriebe.getGame()->getWindow()->height_ * (y_ / height)
        };
    }

    float Vector2D::screenY(float yIn) {
        float difference = getriebe.getGame()->getRenderer()->logicalHeight;
        yIn = getriebe.getGame()->getWindow()->height_ * (yIn / difference);
        return (yIn + getriebe.getGame()->getCurrentScene()->getCamera().position.y_ ) * getriebe.getGame()->getCurrentScene()->getCamera().zoom;
    }

    float Vector2D::screenX(float xIn) {
        float difference = getriebe.getGame()->getRenderer()->logicalWidth;
        xIn = getriebe.getGame()->getWindow()->width_ * (xIn / difference);
        return (xIn + getriebe.getGame()->getCurrentScene()->getCamera().position.x_) * getriebe.getGame()->getCurrentScene()->getCamera().zoom;
    }
}
