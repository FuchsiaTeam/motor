/* Jayson Hartmann 27/03/2022 */
#pragma once

#include <fuchslib/shorter.hpp>
#include "motor/motor.hpp"
#include <motor/log.hpp>
#include "resourcelocation.hpp"
#include <fstream>

namespace Motor {

    template <typename T>
    class Content {
    protected:
        T* content_ = nullptr;
        string path_;
        uintmax size_ = 0;

    public:
        Content(string path) : path_(path) {
            //getriebe.addContent(this);
        };

        Content(ResourceLocation rsloc): path_(rsloc.getPath()) {}

        /* Functions */
    private:
        virtual void inner_load() = 0;
        virtual void inner_unload() {};

        /* Functions */
    public:
        void load();
        void unload();
        void safeLoad() { if (!exists()) load(); }

        void loadAsThread();
        void unloadAsThread();

        T* get() { return this->content_; }
        bool exists() { return get(); }
        uintmax getFileSize() { return size_; }
    };

    template<typename T>
    inline void Content<T>::load() {
        std::ifstream file{path_};
        if (file.good()) {
            inner_load();
            std::filesystem::path p{path_};
            if (this->content_) {
                MOTOR_LOG(string{ "Content " + path_ + " loaded (" + to_string(std::filesystem::file_size(p)) + " bytes)" });
                size_ = std::filesystem::file_size(p);
            }
            else {
                MOTOR_ERROR(string{ "Content " + path_ + " found but could not load!" });
                MOTOR_ERROR(string{ SDL_GetError() });
            }
        } else {
            MOTOR_ERROR(string{ "Content " + path_ + " not found!" });
        }
        file.close();
    }

    template<typename T>
    inline void Content<T>::unload() {
        if (exists()) {
            inner_unload();
            MOTOR_LOG(string{ "Content " + path_ + " unloaded!" });
            size_ = 0;
        }
    }

    template<typename T>
    inline void Content<T>::loadAsThread() {
        std::thread thread = std::thread(&Content<T>::safeLoad, this);
        thread.detach();
    }

    template<typename T>
    inline void Content<T>::unloadAsThread() {
        std::thread thread = std::thread(&Content<T>::unload, this);
        thread.detach();
    }

}