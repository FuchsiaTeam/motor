#pragma once
#include "sdl/SDL_mixer.h"
#include "motor/content/music.hpp"
#include "motor/content/sound.hpp"
#include "fuchslib/stack.hpp"

namespace Motor {

    class ISound {
    public:
        virtual void play() = 0;
    };

    class Sound : public ISound {
    public:
        SoundFile* file_ = nullptr;

        void play() override {
            play(-1, 0);
        }

        void play(int channel, int loops) const {
            if(file_ && file_->exists()) {
                Mix_PlayChannel(channel, file_->get(), loops);
            }
        }

        void changeVolume(Fuchs::Stack<int, 128, 0> stack) const {
            if(file_ && file_->exists()) {
                Mix_VolumeChunk(file_->get(), stack.getAmount());
            }
        }

        void changeVolume(int stack) const {
            changeVolume(Fuchs::Stack<int, 128, 0>{stack});
        }
    };

    class Music : public ISound {
    public:

        Music() = default;
        explicit Music(MusicFile* file): file_(file) {}

        MusicFile* file_ = nullptr;
        void play() override {
            play(0);
        }

        void playFade(int ms) {
            playFade(0, ms);
        }

        void play(int loops) {
            if(file_ && file_->exists()) {
                Mix_PlayMusic(file_->get(), loops);
            }
        }

        void playFade(int loops, int ms) {
            if(file_ && file_->exists()) {
                Mix_FadeInMusic(file_->get(), loops, ms);
            }
        }

        static void changeChannelVolume(Fuchs::Stack<int, 128, 0> stack) {
            Mix_VolumeMusic(stack.getAmount());
        }

        static void changeChannelVolume(int stack) {
            changeChannelVolume(Fuchs::Stack<int, 128, 0>{stack});
        }

        static void fadeOut(int ms) {
            Mix_FadeOutMusic(ms);
        }

        static bool playing() {
            return Mix_PlayingMusic();
        }

        static void halt() {
            Mix_HaltMusic();
        }

        double position() const {
            return Mix_GetMusicPosition(file_->get());

        }

        double duration() const {
            return Mix_MusicDuration(file_->get());
        }
    };

    class SoundChannel {
    public:
        int id_ = 0;
        int volume_ = 64;

        void changeVolume(Fuchs::Stack<int, 128, 0> stack) {
            volume_ = stack.getAmount();
            Mix_Volume(id_, volume_);
        }

        void changeVolume(int stack) {
            changeVolume(Fuchs::Stack<int, 128, 0>{stack});
        }
    };
}