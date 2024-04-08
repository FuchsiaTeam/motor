/* Jayson Hartmann */
#pragma once

#include <fuchslib/shorter.hpp>
#include "motor/types/color.hpp"
#include "motor/components/text.hpp"
#include "motor/components/sprite.hpp"
#include "motor/object/gameobject.hpp"

namespace Motor {
    class Console;
    class Game;

    class ConsoleCommand {
    /* (De-)Constructors */
    public:
        virtual ~ConsoleCommand() = default;

    /* Functions */
    public:
        virtual constexpr cstring getId() { return ""; }
        virtual constexpr cstring getHelpText() { return ""; }
        virtual constexpr int neededArguments() { return 0; }
        virtual bool execute(const vector<string>& arguments, Console* console, string input, Game* game) {
            return false;
        }
    };
    
    struct LogEntry {
        string content_;
        Color32 color_;
        Color32 cacheColor_;
        bool clickAble_ = false;
        void (*clickAction)() = nullptr;

    };

    constexpr int16 consoleFontSize = 28;
    constexpr int16 logFontSize = 22;

    class Console {
    public:
        vector<ConsoleCommand*> commands_{};
        vector<LogEntry> log_{};
        vector<vector<ConsoleCommand*>> helpPages_{};
        bool visible_ = false;
        string input_;
        vector<string> inputHistory_;
        int textLength_;
        Font* consoleFont_;
        SpriteComponent* backGround_;
        TransformComponent* backGroundTransform = new TransformComponent{};
        TransformComponent* textTransform = new TransformComponent{};
        vector<TextComponent*> outputTexts{};
        TextComponent* infoText;
        TextComponent* inputText;
    public:

        Console() {
        }


        void keyDown(SDL_Event& event);
        void mouseEvent(SDL_Event& event, int x, int y);

        std::function<void(SDL_Event&, int, int)> onMouseClick = [this](SDL_Event& event, int x, int y) {
            mouseEvent(event, x, y);
        };

        std::function<void(SDL_Event&)> onKeyDown = [this](SDL_Event& event) {
            keyDown(event);
        };

        vector<vector<ConsoleCommand*>>& getHelpPages() { return helpPages_; }
        vector<ConsoleCommand*>& getCommands() { return this->commands_; }
        vector<LogEntry>& getLog() { return this->log_; }
        string getInput() { return this->input_; }
        nd bool isVisible() const { return this->visible_; }
        void create();

        void addLogEntry(string text, Color32 color) {
            if (getLog().capacity() < getLog().size() + 2 && !getLog().empty()) {
                getLog().erase(getLog().begin());
            }
            LogEntry entry{};
            entry.content_ = text + "\n";
            entry.color_ = color;
            getLog().push_back(entry);
        }

        void addLogEntry(string text) {
            addLogEntry(text, {});
        }

        void registerCommand(ConsoleCommand* command) {
            getCommands().push_back(command);
            addLogEntry("Registered Command " + string{command->getId()});
            if (helpPages_.empty()) {
                helpPages_.push_back({ command });
            }
            else {
                if (helpPages_.back().size() >= logFontSize / 2) {
                    helpPages_.push_back({ command });
                }
                else {
                    helpPages_.back().push_back(command);
                }
            }
        }

        void update() {
           // textLength_ = al_get_text_width(textCache_.getFont()->get(), textCache_.getContent().c_str());
            textLength_ = 55;
        }
        void render();
    };
}


/*
#include "shorter.hpp"
#include "transform.hpp"
#include "sprite.hpp"
#include <allegro5/allegro_primitives.h>
#include "text.hpp"

namespace Fuchs {

    class Console;
    class Game;

    class ConsoleCommand {

    public:
        virtual ~ConsoleCommand() = default;

    public:
        virtual string getId() { return ""; }
        virtual string getHelpText() { return ""; }
        virtual int neededArguments() { return 0; }


        virtual bool execute(const vector<string>& arguments, Console* console, string input, Game* game) {
            return false;
        }
    };

    constexpr int16 consoleFontSize = 28;
    constexpr int16 logFontSize = 22;

    struct LogEntry {
        string content_;
        ALLEGRO_COLOR color_;
        ALLEGRO_COLOR cacheColor_;
        bool clickAble = false;
        void (*clickAction)() = nullptr;
    };

    class Console {
        vector<ConsoleCommand*> commands_{};
        vector<LogEntry> log_{};
        vector<vector<ConsoleCommand*>> helpPages_{};
        bool visible_ = false;
        Transform backgroundTransform_{};
        Transform textTransform_{};
        Sprite background_{ "texture/console/background.png", &fuchsAllegroMod, "console_background" };
        Font logFont_{ "font/moshita_mono.ttf", &fuchsAllegroMod, "console_log_font", logFontSize };
        Text textCache_{ "font/moshita_mono.ttf", &fuchsAllegroMod, consoleFontSize };
        Text logCache_{ &logFont_ };
        Text infoText_{ &logFont_, "FuchsAllegro Console; Press F10 to Open/Close; Type \"help\" for commands (Hover & Clickable)" };
        string input_;
        string lastInput_;
        int textLength_;

    public:
        Console() = default;
        ~Console() {
            for (ConsoleCommand* command : getCommands()) { delete command; }
            background_.unload();
        }
    public:

        vector<ConsoleCommand*>& getCommands() { return this->commands_; }
        vector<LogEntry>& getLog() { return this->log_; }
        Text& getTextCache() { return this->textCache_; }
        Text& getLogCache() { return this->logCache_; }
        Text& getInfoText() { return this->infoText_; }
        string getInput() { return this->input_; }
        nd bool isVisible() const { return this->visible_; }
        vector<vector<ConsoleCommand*>>& getHelpPages() { return helpPages_; }

        void setInput(Text input) { this->textCache_ = input; }

        void create();

        void addLogEntry(string text, ALLEGRO_COLOR color) {
            if (getLog().capacity() < getLog().size() + 2 && !getLog().empty()) {
                getLog().erase(getLog().begin());
            }
            getLog().push_back({ text + "\n", color, color });
        }

        void addLogEntry(string text) {
            addLogEntry(text, al_map_rgb(255, 255, 255));
        }

        void registerCommand(ConsoleCommand* command) {
            getCommands().push_back(command);
            addLogEntry("Registered Command " + command->getId());
            if (helpPages_.empty()) {
                helpPages_.push_back({ command });
            }
            else {
                if (helpPages_.back().size() >= logFontSize / 2) {
                    helpPages_.push_back({ command });
                }
                else {
                    helpPages_.back().push_back(command);
                }
            }
        }

        void update() {
            textLength_ = al_get_text_width(textCache_.getFont()->get(), textCache_.getContent().c_str());
        }

        void keyDown(ALLEGRO_EVENT& event, int keycode, Game* game);
        void charDown(ALLEGRO_EVENT& event, int keycode, int unicode, Game* game);

        void draw(ALLEGRO_EVENT& event, bool& redraw, Game* game);
        void tick(ALLEGRO_EVENT& event, bool& redraw, Game* game);
        void mouseEvent(int button, int x, int y, ALLEGRO_EVENT& event, Game* game);
    };
}
*/