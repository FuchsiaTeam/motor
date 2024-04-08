#include "motor/log.hpp"
#include "motor/ansi.hpp"
#include <sdl/SDL.h>
#include <algorithm>
#include <numeric>
#include <future>
#include <mutex>
#include <execution>
/*#include "fuchsallegro/log.hpp"
#include "fuchsallegro/fuchsallegro.hpp"

namespace Fuchs::Con {

    void log(const string& input, ostream& out) {
        log(input, out, al_map_rgb(255, 255, 255));
    }

    void log(const string& input, ostream& out, ALLEGRO_COLOR color) {
        cout << input << "\n";
        if (game) {
            game->getConsole().addLogEntry(input, color);
        }
    }

    void error(const string& input, ostream& out) {
        log(ansi::red + input, out);
    }

    void success(const string& input, ostream& out) {
        log(ansi::green + input, out);
    }

}
*/

void Motor::Con::log(const string& input, ostream& out, const string& func, int line, const string& file) {
    //out << func << " -> " << input << " -> " << file << ":" << line << " [" << MOTOR_VERSION << "] \n";
#ifdef MAKE_LOGS
    auto renderTask = std::async(std::launch::async,
        [&out, &input, &func, &line, &file]() {
            uint32 ms = SDL_GetTicks();
            int seconds = ms / 1000;
            ms %= 1000;
            int minutes = seconds / 60;
            seconds %= 60;
            int hours = minutes / 60;
            minutes %= 60;
            out << "[" << hours << ":" << minutes << ":" << seconds << ":" << ms << "] " << func << ansi::reset << " -> " << ansi::cyan << input << ansi::reset << " -> " << file << ":" << line << " \n";
        });
#endif
}

void Motor::Con::error(const string& input, ostream& out, const string& func, int line, const string& file) {
    //out << func << " -> " << input << " -> " << file << ":" << line << " [" << MOTOR_VERSION << "] \n";
#ifdef MAKE_LOGS
    auto renderTask = std::async(std::launch::async,
                                 [&out, &input, &func, &line, &file]() {
                                     uint32 ms = SDL_GetTicks();
                                     int seconds = ms / 1000;
                                     ms %= 1000;
                                     int minutes = seconds / 60;
                                     seconds %= 60;
                                     int hours = minutes / 60;
                                     minutes %= 60;
                                     out << "[" << hours << ":" << minutes << ":" << seconds << ":" << ms << " : ERROR] " << func << ansi::reset << " -> " << ansi::red << input << ansi::reset << " -> " << file << ":" << line << " \n";
                                 });
#endif
}

void Motor::Con::warn(const string& input, ostream& out, const string& func, int line, const string& file) {
    //out << func << " -> " << input << " -> " << file << ":" << line << " [" << MOTOR_VERSION << "] \n";
#ifdef MAKE_LOGS
    auto renderTask = std::async(std::launch::async,
                                 [&out, &input, &func, &line, &file]() {
                                     uint32 ms = SDL_GetTicks();
                                     int seconds = ms / 1000;
                                     ms %= 1000;
                                     int minutes = seconds / 60;
                                     seconds %= 60;
                                     int hours = minutes / 60;
                                     minutes %= 60;
                                     out << "[" << hours << ":" << minutes << ":" << seconds << ":" << ms << " : WARN] " << func << ansi::reset << " -> " << ansi::cyan << input << ansi::reset << " -> " << file << ":" << line << " \n";
                                 });
#endif
}