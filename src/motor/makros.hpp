#pragma once
#define MOTOR_VERSION "DEV_MAJOR:0_BUILD:0.1"
#define MOTOR_VERSION_HEX "D_0x1"


/*
	Concept to check if T is derived from I but isn't I
	Useful for (abstract) Interfaces
*/
//template <typename T, class I>
//concept CInterface = std::derived_from<T, I> && !std::is_same<T, I>();