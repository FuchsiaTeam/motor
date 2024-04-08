/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <fuchslib/shorter.hpp>
#include <fuchslib/utility.hpp>

namespace Motor {

	struct ColorF { float r, g, b, a; };

	inline void to_json(json& j, const ColorF& p) {
		j = json{
			{"r", p.r},
			{"g", p.g},
			{"b", p.b},
			{"a", p.a}
		};
	}

	inline void from_json(const json& j, ColorF& p) {
		Fuchs::Utility::js::getIfHas(j, "r", p.r);
		Fuchs::Utility::js::getIfHas(j, "g", p.g);
		Fuchs::Utility::js::getIfHas(j, "b", p.b);
		Fuchs::Utility::js::getIfHas(j, "a", p.a);
	}

	/* Uses 2 bytes, Colors are ranged from 0-15 */
	class Color16 {
		uint8 r_ : 4;
		uint8 g_ : 4;
		uint8 b_ : 4;
		uint8 a_ : 4;

	public:
		nd ColorF toFloatColor() const { return ColorF{ (float)r_ / 15, (float)g_ / 15, (float)b_ / 15, (float)a_ / 15 }; }
	};

	class Color32;

	class Color24 {
	protected:
		uint8 r_ = 255, g_ = 255, b_ = 255;

	public:
		Color24() = default;
		constexpr Color24(uint8 r, uint8 g, uint8 b) : r_(r), g_(g), b_(b) {}

	public:
		nd uint8 getRed() const { return this->r_; }
		nd uint8 getGreen() const { return this->g_; }
		nd uint8 getBlue() const { return this->b_; }

		void setRed(uint8 red) { this->r_ = red; }
		void setGreen(uint8 green) { this->g_ = green; }
		void setBlue(uint8 blue) { this->b_ = blue; }

		void set(uint8 redIn, uint8 greenIn, uint8 blueIn) {
			setRed(redIn);
			setGreen(greenIn);
			setBlue(blueIn);
		}

		//operator Color32() const {
			//return Color32(r_, g_, b_, 255);
		//}
	};

	inline void to_json(json& j, const Color24& p) {
		j = json{
			{"r", p.getRed()},
			{"g", p.getGreen()},
			{"b", p.getBlue()}
		};
	}

	inline void from_json(const json& j, Color24& p) {
		if (Fuchs::Utility::js::hasKey(j, "r")) p.setRed(j.at("r"));
		if (Fuchs::Utility::js::hasKey(j, "g")) p.setGreen(j.at("g"));
		if (Fuchs::Utility::js::hasKey(j, "b")) p.setBlue(j.at("b"));
	}


	class Color32 : public Color24 {
	protected:
		uint8 a_ = 255;

	public:
		Color32() = default;
		constexpr Color32(uint8 r, uint8 g, uint8 b, uint8 a) : Color24(r, g, b), a_(a) {}
	public:
		nd uint8 getAlpha() const { return this->a_; }
		void setAlpha(uint8 alpha) { this->a_ = alpha; }

		void set(uint8 redIn, uint8 greenIn, uint8 blueIn, uint8 alphaIn) {
			Color24::set(redIn, greenIn, blueIn);
			setAlpha(alphaIn);
		}
	};

	inline void to_json(json& j, const Color32& p) {
		j = json{
			{"r", p.getRed()},
			{"g", p.getGreen()},
			{"b", p.getBlue()},
			{"a", p.getAlpha()}
		};
	}

	inline void from_json(const json& j, Color32& p) {
		if (Fuchs::Utility::js::hasKey(j, "r")) p.setRed(j.at("r"));
		if (Fuchs::Utility::js::hasKey(j, "g")) p.setGreen(j.at("g"));
		if (Fuchs::Utility::js::hasKey(j, "b")) p.setBlue(j.at("b"));
		if (Fuchs::Utility::js::hasKey(j, "a")) p.setAlpha(j.at("a"));
	}


	namespace Color {
		nd inline uint8 convertFloat(float in, float max) { return (max / 100) * (in * 100); }
		nd inline uint8 convertFloat(float in) { return convertFloat(in, 255); }
		nd inline ColorF toFloatColor(const Color32& color) { return ColorF{ (float)color.getRed() / 255, (float)color.getGreen() / 255, (float)color.getBlue() / 255, (float)color.getAlpha() / 255 }; }
		nd inline Color32 toColor32(float r, float g, float b, float a) { return Color32{ convertFloat(r), convertFloat(g), convertFloat(b), convertFloat(a) }; }
	}


	enum ColorBlindnessType {
		REGULAR, DEUTERANOPIA, PROTANOPIA, TRITANOPIA
	};

	struct ColorBlindness {
		Color32 regular;
		Color32 deuteranopia;
		Color32 protanopia;
		Color32 tritanopia;

		Color32& getColor(const ColorBlindnessType& type) {
			switch (type) {
			case REGULAR: return regular;
			case DEUTERANOPIA: return deuteranopia;
			case PROTANOPIA: return protanopia;
			case TRITANOPIA: return tritanopia;
			}
			return regular;
		}

		nd Color32 getColor(const ColorBlindnessType& type) const {
			switch (type) {
			case REGULAR: return regular;
			case DEUTERANOPIA: return deuteranopia;
			case PROTANOPIA: return protanopia;
			case TRITANOPIA: return tritanopia;
			}
			return regular;
		}
	};

	inline void to_json(json& j, const ColorBlindness& p) {
		j = json{
				{"regular", p.regular},
				{"deuteranopia", p.deuteranopia},
				{"protanopia", p.protanopia},
				{"tritanopia", p.tritanopia}
		};
	}

	inline void from_json(const json& j, ColorBlindness& p) {
		Fuchs::Utility::js::getIfHas(j, "regular", p.regular);
		Fuchs::Utility::js::getIfHas(j, "deuteranopia", p.deuteranopia);
		Fuchs::Utility::js::getIfHas(j, "protanopia", p.protanopia);
		Fuchs::Utility::js::getIfHas(j, "tritanopia", p.tritanopia);
	}


}
