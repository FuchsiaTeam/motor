/* Jayson Hartmann 31/03/2022 */
#pragma once

#include "scale.hpp"
#include <fuchslib/shorter.hpp>
#include <cmath>
namespace Motor {

    constexpr int VEC2D_X = 0;
    constexpr int VEC2D_Y = 1;
	constexpr int VEC2D_XY = 2;

	class Vector2D {
		float x_ = 0;
		float y_ = 0;

		/* Constructors */
	public:
		Vector2D() = default;
		Vector2D(float x, float y) : x_(x), y_(y) {};

		friend class TransformComponent;

		/* Functions */
	public:

		 float distance(const Vector2D& vector) const {
			float yD = getY() - vector.getY();
			yD = yD * yD;
			float xD = (vector.getX() - getX());
			xD = xD * xD;
			return sqrt(yD + xD);
		 }

		 float length() {
			 return std::sqrt(x_ * x_ + y_ * y_);
		 }

		 void normalize() {
			 float len = length();
			 x_ /= len;
			 y_ /= len;
		 }

		void setX(float x) { this->x_ = x; }
		float getX() { return this->x_; }
		float getX() const { return this->x_; }
		void setY(float y) { this->y_ = y; }
		float getY() { return this->y_; }
		float getY() const { return this->y_; }
		void set(float xIn, float yIn) {
			setX(xIn);
			setY(yIn);
		}

		float added() {
			return getX() + getY();
		}

		Vector2D& add(const Vector2D& vector) {
			this->x_ += vector.x_;
			this->y_ += vector.y_;
			return *this;
		}

		Vector2D& subtract(const Vector2D& vector) {
			this->x_ -= vector.x_;
			this->y_ -= vector.y_;
			return *this;
		}

		Vector2D& multiply(const Vector2D& vector) {
			this->x_ *= vector.x_;
			this->y_ *= vector.y_;
			return *this;
		}

		Vector2D& multiply(double vector) {
			this->x_ *= vector;
			this->y_ *= vector;
			return *this;
		}

		Vector2D& divide(const Vector2D& vector) {
			this->x_ /= vector.x_;
			this->y_ /= vector.y_;
			return *this;
		}


		/* Position inside the Center from an Object */
		Vector2D center(const Scale& scale);

		/*
		* Position of Object influenced by the Camera
		*/
		Vector2D screen();
		Vector2D screenCenter(const Scale& scale);
        Vector2D fixedScreen();
		static float screenX(float xIn);
		static float screenY(float yIn);

		/* Operator Overloading*/
	public:
		friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2) { return v1.add(v2); }
		friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2) { return v1.subtract(v2); }
		friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2) { return v1.multiply(v2); }
		friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2) { return v1.divide(v2); }

		friend ostream& operator<<(ostream& os, const Vector2D& vector) {
			os << "Vec2D{x=" << vector.x_ << "/y=" << vector.y_ << "}";
			return os;
		}

		Vector2D& operator+=(const Vector2D& vector) { return this->add(vector); }
		Vector2D& operator-=(const Vector2D& vector) { return this->subtract(vector); }
		Vector2D& operator*=(const Vector2D& vector) { return this->multiply(vector); }
		Vector2D& operator/=(const Vector2D& vector) { return this->divide(vector); }

        float operator [](int idx) const {
            if(idx == VEC2D_X) return x_;
            if(idx == VEC2D_Y) return y_;
			if(idx == VEC2D_XY) return x_ + y_;
            return 0;
        }
	};
}