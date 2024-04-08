/* Jayson 9.10.21 */
#pragma once
/*
#include "ansi.hpp"
#include "shorter.hpp"
#include <allegro5/allegro_color.h>

namespace Motor::Con {
    void log(const string& input, ostream& out);
    void log(const string& input, ostream& out, ALLEGRO_COLOR color);
    void error(const string& input, ostream& out);
    void success(const string& input, ostream& out);
}
*/

#define MAKE_LOGS

#include "motor/makros.hpp"
#include <fuchslib/shorter.hpp>

#ifdef LINUX
#define MOTOR_LOG(input) Motor::Con::log(input, std::cout, __PRETTY_FUNCTION__, __LINE__, __FILENAME__);
#define MOTOR_ERROR(input) Motor::Con::error(input, std::cout, __PRETTY_FUNCTION__, __LINE__, __FILENAME__);
#endif

#ifdef MACOS
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define MOTOR_LOG(input) Motor::Con::log(input, std::cout, __PRETTY_FUNCTION__, __LINE__, __FILENAME__);
#endif

#ifdef WINDOWS
#define MOTOR_LOG(input) MAKE_LOGS Motor::Con::log(input, std::cout, __FUNCTION__, __LINE__, __FILENAME__);
#define MOTOR_ERROR(input) MAKE_LOGS Motor::Con::error(input, std::cout, __FUNCTION__, __LINE__, __FILENAME__);
#define MOTOR_WARN(input) MAKE_LOGS Motor::Con::warn(input, std::cout, __FUNCTION__, __LINE__, __FILENAME__);
#endif

namespace Motor::Con {
    void log(const string& input, ostream& out, const string& func, int line, const string& file);
    void error(const string& input, ostream& out, const string& func, int line, const string& file);
    void warn(const string& input, ostream& out, const string& func, int line, const string& file);
}
