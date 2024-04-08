#pragma once
#include "content.hpp"
#include "sdl/SDL_mixer.h"

namespace Motor {

    class MusicFile : public Content<Mix_Music> {
    public:
        uint16 size_ = 24;
        uintmax outline_ = 0;

        explicit MusicFile(string path): Content(path) {};

        explicit MusicFile(ResourceLocation path): Content(path) {};

    private:
        void inner_load() override;

        void inner_unload() override;
    };

    inline void MusicFile::inner_load() {
        this->content_ = Mix_LoadMUS(path_.c_str());
    }

    inline void MusicFile::inner_unload() {
        if (exists()) {
            delete get();
        }
        this->content_ = nullptr;
    }
}