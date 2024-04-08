#pragma once
#include "content.hpp"
#include "sdl/SDL_mixer.h"

namespace Motor {

    class SoundFile : public Content<Mix_Chunk> {
    public:
        uint16 size_ = 24;
        uintmax outline_ = 0;

        explicit SoundFile(string path): Content(path) {};

        explicit SoundFile(ResourceLocation path): Content(path) {};

    private:
        void inner_load() override;

        void inner_unload() override;
    };

    inline void SoundFile::inner_load() {
        this->content_ = Mix_LoadWAV(path_.c_str());
        //std::cout << TTF_GetError() << std::endl;
    }

    inline void SoundFile::inner_unload() {
        if (exists()) {
            delete get();
        }
        this->content_ = nullptr;
    }
}