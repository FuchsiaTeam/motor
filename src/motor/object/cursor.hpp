/* Jayson Hartmann */
#pragma once

namespace Motor {

    class GameObject;

    class Cursor : public GameObject {
    protected:
        void hide_os();
        void show_os();

    public:
        void hide();
        void show();

        void setTexture();
    };
}