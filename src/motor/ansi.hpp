/* Jayson 9.10.21 */
#pragma once

#include <fuchslib/shorter.hpp>

namespace ansi {
#define FUCHS_ANSI_ENTRY constexpr cstring

    FUCHS_ANSI_ENTRY reset = "\033[0m";
    FUCHS_ANSI_ENTRY green = "\033[0;32m";
    FUCHS_ANSI_ENTRY red = "\033[0;31m";
    FUCHS_ANSI_ENTRY cyan = "\033[0;36m";
    FUCHS_ANSI_ENTRY blue = "\033[0;34m";
    FUCHS_ANSI_ENTRY black = "\033[0;30m";
    FUCHS_ANSI_ENTRY purple = "\033[0;35m";
    FUCHS_ANSI_ENTRY white = "\033[0;37m";

    namespace bold {
        FUCHS_ANSI_ENTRY green = "\033[1;32m";
        FUCHS_ANSI_ENTRY red = "\033[1;31m";
        FUCHS_ANSI_ENTRY cyan = "\033[1;36m";
        FUCHS_ANSI_ENTRY blue = "\033[1;34m";
        FUCHS_ANSI_ENTRY black = "\033[1;30m";
        FUCHS_ANSI_ENTRY purple = "\033[1;35m";
        FUCHS_ANSI_ENTRY white = "\033[1;37m";
    }

#undef FUCHS_ANSI_ENTRY
}
