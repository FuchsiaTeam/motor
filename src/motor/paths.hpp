#pragma once
#include <fuchslib/shorter.hpp>

namespace Motor::Path {
    constexpr cstring sprite{"texture/"};
    constexpr cstring font{"font/"};

    extern string docs;
    extern string profiles;
    extern string worlds;
    extern string mods;
    extern string scripts;

    /*namespace mods {
        extern string root;
        extern string libraries;
        namespace scripts {
            extern string root;
            extern string lua;
            extern string python;
        }
    }*/

    void set_save_game_folder(string gameName);
}