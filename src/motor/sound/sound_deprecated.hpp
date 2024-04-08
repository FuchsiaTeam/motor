#pragma once

#include "sdl/SDL_audio.h"
#include "motor/content/resourcelocation.hpp"
#include "motor/log.hpp"
#include "fuchslib/stack.hpp"

namespace Motor {

    class ISound {
    public:
        virtual ~ISound() = default;
        virtual void play() = 0;
        virtual void stop() = 0;
    };

    class Sound : public ISound {
    private:
        SDL_AudioDeviceID deviceId;
        SDL_AudioSpec audioSpec;
        uint8_t* waveStart;
        uint32_t waveLength;

    public:
        //TODO TRY TO INTEGRATE INTO CONTENT
        Sound(ResourceLocation resourceLocation) {
            if(!SDL_LoadWAV(resourceLocation.getPath().c_str(), &audioSpec, &waveStart, &waveLength)) {
                MOTOR_LOG("Could not load sound " + resourceLocation.getPath());
            } else {
                MOTOR_LOG("Loaded sound " + resourceLocation.getPath())
            }
        }
        ~Sound() {
            SDL_FreeWAV(waveStart);
            SDL_CloseAudioDevice(deviceId);
        };

        void setup() {
            deviceId = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE);
            if(deviceId == 0) {
                MOTOR_LOG("Couldnt create Audio Device ID")
            }
        }

        void changeVolume(Fuchs::Stack<int, 128, 0> stack) {
            SDL_MixAudioFormat(nullptr, waveStart, SDL_AUDIO_ALLOW_FORMAT_CHANGE, waveLength, 0);
        }

        void play() override {
            int status = SDL_QueueAudio(deviceId, waveStart, waveLength);
            SDL_PauseAudioDevice(deviceId, 0);
        }

        void stop() override {
            SDL_PauseAudioDevice(deviceId, 1);
        }
    };

}