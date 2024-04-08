#include "motor/console/console.hpp"

#include "motor/motor.hpp"

namespace Motor {
    bool created_ = false;
    class HelpCommand : public ConsoleCommand {
    public:
        constexpr cstring getId() override { return "help"; }
        constexpr cstring getHelpText() override { return "Displays all Commands; Usages: help, help <page>"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            if (arguments.size() == 1) {
                for (ConsoleCommand* command : console->getHelpPages().at(0)) {
                    console->addLogEntry(string{command->getId()} + " : " + command->getHelpText());
                }
                int pages = console->getHelpPages().size();
                console->addLogEntry("Page [1] of [" + std::to_string(console->getHelpPages().size()) + "]");
            }
            else {
                int arg = std::stoi(arguments.at(1)) - 1;
                if (arg <= console->getHelpPages().size()) {
                    for (ConsoleCommand* command : console->getHelpPages().at(arg)) {
                        console->addLogEntry(string{command->getId()} + " : " + command->getHelpText());
                    }
                    console->addLogEntry("Page [" + std::to_string(arg + 1) + "] of [" + std::to_string(console->getHelpPages().size()) + "]");
                }
                else {
                    console->addLogEntry("Wrong Input!");
                    return false;
                }
            }
            return true;
        }
    };

    class ClearCommand : public ConsoleCommand {
    public:
        constexpr cstring getId() override { return "clear"; }
        constexpr cstring getHelpText() override { return "Clears the Console; Usages: clear, clear <position>, clear <start> <end>"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            if (arguments.size() == 2) {
                int clearance = stoi(arguments.at(1));
                if (clearance > console->getLog().size()) {
                    console->addLogEntry("Value is bigger than Log Size!");
                }
                else {
                    console->getLog().erase(console->getLog().begin() + clearance);
                }
            }
            else if (arguments.size() == 3) {
                int start = stoi(arguments.at(1));
                int end = stoi(arguments.at(2));
                if (end > console->getLog().size() || start < 0) {
                    console->addLogEntry("Wrong Input!");
                    return false;
                }
                console->getLog().erase(console->getLog().begin() + start, console->getLog().begin() + end);
            }
            else {
                console->getLog().clear();
            }
            return true;
        }
    };

    class ResolutionCommand : public ConsoleCommand {
    public:
        constexpr cstring getId() override { return "resolution"; }
        constexpr cstring getHelpText() override { return "Changes the Resolution; Usage: resolution <width> <height>"; }
        int neededArguments() override { return 2; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            int width = stoi(arguments.at(1));
            int height = stoi(arguments.at(2));
            game->changeResolution(width, height);
            return true;
        }
    };

    class ExitCommand : public ConsoleCommand {
    public:
        constexpr cstring getId() override { return "exit"; }
        constexpr cstring getHelpText() override { return "Stops the GameTick (Exit, basically)"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            bool running = false;
            Events::quit.dispatch(running);
            game->running_ = running;
            return true;
        }
    };

    class FullscreenCommand : public ConsoleCommand {
    public:
        constexpr cstring getId() override { return "fullscreen"; }
        constexpr cstring getHelpText() override { return "Toggles Fullscreen"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            //game->getDisplay()->toggleFullscreen();
            return true;
        }
    };

    void Console::create() {
        Events::keyDown.attach(&onKeyDown);
        Events::mouseClick.attach(&onMouseClick);
        log_.reserve(Renderer::logicalWidth / logFontSize - 2);
        //textTransform_.getColor().set(35, 35, 35, 245);
        //textTransform_.getScale().set(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT / 32);
        //textTransform_.getPosition().setY(DEFAULT_WINDOW_HEIGHT - (textTransform_.getScale().getY()));
        registerCommand(new HelpCommand{});
        registerCommand(new ClearCommand{});
        registerCommand(new ResolutionCommand{});
        registerCommand(new ExitCommand{});
        registerCommand(new FullscreenCommand{});
        backGroundTransform->scale.set(1920, 1080);
        backGroundTransform->color.set(0, 0, 0, 233);
        infoText = new TextComponent(consoleFont_);
        inputText = new TextComponent(consoleFont_);
        for(int i = 0; i > 44; ++i) {
            outputTexts.emplace_back(new TextComponent(consoleFont_));
        }
        created_ = true;
    }
    void Console::keyDown(SDL_Event& sdlEvent) {
        int keycode = sdlEvent.key.keysym.sym;
        if (keycode == SDLK_F10) {
            visible_ = !visible_;
            //game->setPaused(isVisible());
            if (isVisible()) {
                getriebe.getGame()->showMouse();
            }
            else {
                getriebe.getGame()->hideMouse();
                //game->getScene()->validateObjects();
                //for (ObjectHolder* obj : game->getScene()->getObjects()) {
                //obj->get()->onConsoleClose(this, event, keycode, game);
                //}
            }
        }
        else if (keycode == SDLK_KP_ENTER && isVisible()) {
            inputHistory_.push_back(input_);
            vector<string> arguments{};
            size_t pos;

            string argumentInput = input_;
            argumentInput.append(" ");
            while ((pos = argumentInput.find(' ')) != string::npos) {
                arguments.push_back(argumentInput.substr(0, pos));
                argumentInput.erase(0, pos + string{ " " }.length());
            }

            string cmd = !arguments.empty() ? arguments.at(0) : input_;
            //Con::log("Issuing Command: " + input_, cout);
            bool execution = false;
            for (ConsoleCommand* command : getCommands()) {
                if (command->getId() == cmd) {
                    if (arguments.size() - 1 >= command->neededArguments()) {
                        addLogEntry("Executing Command: " + string{command->getId()});
                        try {
                            execution = command->execute(arguments, this, input_, getriebe.getGame());
                        }
                        catch (const exception& e) {
                            addLogEntry("Failed to Execute || " + string{command->getId()} + " || " + string{ e.what() });
                            addLogEntry("Full Input: " + input_);
                        }
                        break;
                    }
                    else {
                        addLogEntry("Wrong amount of Arguments; Entered: " + to_string(arguments.size() - 1) + ", Needed: " + to_string(command->neededArguments()));
                    }
                }
            }
            addLogEntry(execution ? "Execution successful" : "Execution failed", execution ? Color32{0, 255, 50, 255} : Color32{255, 50, 50, 255});
            input_ = "";
            inputText->setContent(input_);
            //update();
        }
        else if (keycode == SDLK_UP) {
            string lasin = input_;
            //input_ = lastInput_;
            //lastInput_ = lasin;
            inputText->setContent(input_);
            //update();
        }
    }

    void Console::mouseEvent(SDL_Event& event, int x, int y) {
     if (!isVisible()) return;
        int x1, y1, x2, y2;

        x1 = 0;
        x2 = getriebe.getGame()->getRenderer()->logicalWidth;

        int i = 0;
        for (auto& item : getLog()) {
            item.color_ = item.cacheColor_;
            y1 = (logFontSize * (i + 1));
            y2 = y1 + logFontSize;
            #undef max
            #undef min
            if (x <= std::max(x1, x2) && x >= std::min(x1, x2) && y <= std::max(y1, y2) && y >= std::min(y1, y2)) {
                string content = item.content_;
                content.pop_back();
                for (const auto& cmd : getCommands()) {
                    if (content.contains(cmd->getId())) {
                        //if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                            if (cmd->neededArguments() == 0) {
                                cmd->execute({}, this, cmd->getId(), getriebe.getGame());
                                return;
                            }
                            input_ = cmd->getId();
                            inputText->setContent(cmd->getId());
                            update();
                            return;
                       // }
                       // else {
                        //    item.color_ = al_map_rgb(200, 255, 200);
                        //}
                    }
                }

                if (item.clickAble_) {
                    if (item.clickAction) item.clickAction();
                }
            }
            i++;
        }
    }

    void Console::render() {
        if (!created_) return;
        infoText->render();
        inputText->render();
        for (TextComponent* text : outputTexts) {
            text->render();
        }

        if(isVisible()) {
            backGround_->render();
            backGround_->render();
            if(infoText) infoText->setContent("Motor Console; Press F10 to Open/Close; Type \"help\" for commands (Hover & Clickable)");
            
            for(int i = 0; i > 44; ++i) {
                if(outputTexts.at(i) && getLog().size() > i) {
                    outputTexts.at(i)->setContent(getLog()[i].content_);
                    outputTexts.at(i)->color_ = (Color32)getLog()[i].color_;
                }
            }
            
            /*int i = logFontSize * 2;
            for(LogEntry str : getLog()) {
                str.content_.pop_back();
                getLogCache().setContent(str.content_);
                i += logFontSize;
                getLogCache().drawJustified(str.color_, textTransform_.getPosition().getScreenX(), Position::toScreenX(DEFAULT_WINDOW_WIDTH), Position::toScreenY(i), 1, ALLEGRO_ALIGN_LEFT);
            }
            getTextCache().drawJustified(al_map_rgb(255, 255, 255), textTransform_.getPosition().getScreenX() + consoleFontSize / 8, Position::toScreenX(DEFAULT_WINDOW_WIDTH), textTransform_.getPosition().getScreenY() + textTransform_.getScale().getY() + consoleFontSize, 1, ALLEGRO_ALIGN_LEFT);
            if(showEndSymbol) {
                al_draw_justified_text(getTextCache().getFont()->get(), al_map_rgb(255, 255, 255),
                                       (textTransform_.getPosition().getScreenX() + consoleFontSize / 8) + textLength_,
                                       Position::toScreenX(DEFAULT_WINDOW_WIDTH),
                                       textTransform_.getPosition().getScreenY() + textTransform_.getScale().getY() +
                                       consoleFontSize, 1, ALLEGRO_ALIGN_LEFT, "|");
            }*/
        }
    }

    /*void Console::render() {
        if(isVisible()) {
            backGround_->render();
            backGround_->render();
            getInfoText().drawJustified(al_map_rgb(133, 75, 72), textTransform_.getPosition().getScreenX(), Position::toScreenX(DEFAULT_WINDOW_WIDTH), Position::toScreenY(logFontSize * 2), 1, ALLEGRO_ALIGN_LEFT);
            int i = logFontSize * 2;
            for(LogEntry str : getLog()) {
                str.content_.pop_back();
                getLogCache().setContent(str.content_);
                i += logFontSize;
                getLogCache().drawJustified(str.color_, textTransform_.getPosition().getScreenX(), Position::toScreenX(DEFAULT_WINDOW_WIDTH), Position::toScreenY(i), 1, ALLEGRO_ALIGN_LEFT);
            }
            getTextCache().drawJustified(al_map_rgb(255, 255, 255), textTransform_.getPosition().getScreenX() + consoleFontSize / 8, Position::toScreenX(DEFAULT_WINDOW_WIDTH), textTransform_.getPosition().getScreenY() + textTransform_.getScale().getY() + consoleFontSize, 1, ALLEGRO_ALIGN_LEFT);
            if(showEndSymbol) {
                al_draw_justified_text(getTextCache().getFont()->get(), al_map_rgb(255, 255, 255),
                                       (textTransform_.getPosition().getScreenX() + consoleFontSize / 8) + textLength_,
                                       Position::toScreenX(DEFAULT_WINDOW_WIDTH),
                                       textTransform_.getPosition().getScreenY() + textTransform_.getScale().getY() +
                                       consoleFontSize, 1, ALLEGRO_ALIGN_LEFT, "|");
            }
        }
    }*/
}


/*
#include "fuchsallegro/console.hpp"
#include "fuchsallegro/game.hpp"
#include "fuchsallegro/log.hpp"

#ifdef UNIX
#include <dlfcn.h>
#endif

namespace Fuchs {

    class HelpCommand : public ConsoleCommand {
    public:
        string getId() override { return "help"; }
        string getHelpText() override { return "Displays all Commands; Usages: help, help <page>"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            if (arguments.size() == 1) {
                for (ConsoleCommand* command : console->getHelpPages().at(0)) {
                    console->addLogEntry(command->getId() + " : " + command->getHelpText());
                }
                int pages = console->getHelpPages().size();
                console->addLogEntry("Page [1] of [" + std::to_string(console->getHelpPages().size()) + "]");
            }
            else {
                int arg = std::stoi(arguments.at(1)) - 1;
                if (arg <= console->getHelpPages().size()) {
                    for (ConsoleCommand* command : console->getHelpPages().at(arg)) {
                        console->addLogEntry(command->getId() + " : " + command->getHelpText());
                    }
                    console->addLogEntry("Page [" + std::to_string(arg + 1) + "] of [" + std::to_string(console->getHelpPages().size()) + "]");
                }
                else {
                    console->addLogEntry("Wrong Input!");
                    return false;
                }
            }
            return true;
        }
    };

    class ClearCommand : public ConsoleCommand {
    public:
        string getId() override { return "clear"; }
        string getHelpText() override { return "Clears the Console; Usages: clear, clear <position>, clear <start> <end>"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            if (arguments.size() == 2) {
                int clearance = stoi(arguments.at(1));
                if (clearance > console->getLog().size()) {
                    console->addLogEntry("Value is bigger than Log Size!");
                }
                else {
                    console->getLog().erase(console->getLog().begin() + clearance);
                }
            }
            else if (arguments.size() == 3) {
                int start = stoi(arguments.at(1));
                int end = stoi(arguments.at(2));
                if (end > console->getLog().size() || start < 0) {
                    console->addLogEntry("Wrong Input!");
                    return false;
                }
                console->getLog().erase(console->getLog().begin() + start, console->getLog().begin() + end);
            }
            else {
                console->getLog().clear();
            }
            return true;
        }
    };

    class ResolutionCommand : public ConsoleCommand {
    public:
        string getId() override { return "resolution"; }
        string getHelpText() override { return "Changes the Resolution; Usage: resolution <width> <height>"; }
        int neededArguments() override { return 2; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            int width = stoi(arguments.at(1));
            int height = stoi(arguments.at(2));
            game->getDisplay()->resizeDisplay(width, height);
            return true;
        }
    };

    class ExitCommand : public ConsoleCommand {
    public:
        string getId() override { return "exit"; }
        string getHelpText() override { return "Stops the GameTick (Exit, basically)"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            game->setRunning(false);
            return true;
        }
    };

    class FullscreenCommand : public ConsoleCommand {
    public:
        string getId() override { return "fullscreen"; }
        string getHelpText() override { return "Toggles Fullscreen"; }
        bool execute(const vector<string>& arguments, Console* console, string input, Game* game) override {
            game->getDisplay()->toggleFullscreen();
            return true;
        }
    };


    void Console::keyDown(ALLEGRO_EVENT& event, int keycode, Game* game) {
        if (keycode == ALLEGRO_KEY_F10) {
            visible_ = !visible_;
            game->setPaused(isVisible());
            if (isVisible()) {
                game->showMouse();
            }
            else {
                game->getScene()->validateObjects();
                for (ObjectHolder* obj : game->getScene()->getObjects()) {
                    obj->get()->onConsoleClose(this, event, keycode, game);
                }
            }
        }
        else if (keycode == ALLEGRO_KEY_ENTER && isVisible()) {
            lastInput_ = input_;
            vector<string> arguments{};
            size_t pos;

            string argumentInput = input_;
            argumentInput.append(" ");
            while ((pos = argumentInput.find(' ')) != string::npos) {
                arguments.push_back(argumentInput.substr(0, pos));
                argumentInput.erase(0, pos + string{ " " }.length());
            }

            string cmd = !arguments.empty() ? arguments.at(0) : input_;
            Con::log("Issuing Command: " + input_, cout);
            bool execution = false;
            for (ConsoleCommand* command : getCommands()) {
                if (command->getId() == cmd) {
                    if (arguments.size() - 1 >= command->neededArguments()) {
                        addLogEntry("Executing Command: " + command->getId());
                        try {
                            execution = command->execute(arguments, this, input_, game);
                        }
                        catch (const exception& e) {
                            addLogEntry("Failed to Execute || " + command->getId() + " || " + string{ e.what() });
                            addLogEntry("Full Input: " + input_);
                        }
                        break;
                    }
                    else {
                        addLogEntry("Wrong amount of Arguments; Entered: " + to_string(arguments.size() - 1) + ", Needed: " + to_string(command->neededArguments()));
                    }
                }
            }
            addLogEntry(execution ? "Execution successful" : "Execution failed", execution ? al_map_rgb(50, 255, 50) : al_map_rgb(255, 50, 50));
            input_ = "";
            getTextCache().setContent(input_);
            update();
        }
        else if (keycode == ALLEGRO_KEY_BACKSPACE && isVisible()) {
            //if(!input_.empty()) input_.pop_back(); //pop back "|"
            //if(!input_.empty()) input_.pop_back(); //actual pop back
            //input_.append("|");
            //getTextCache().setContent(input_);
        }
        else if (keycode == ALLEGRO_KEY_UP) {
            string lasin = input_;
            input_ = lastInput_;
            lastInput_ = lasin;
            getTextCache().setContent(input_);
            update();
        }
    }

    int endSymbolTick_;
    bool showEndSymbol = true;
    void Console::draw(ALLEGRO_EVENT& event, bool& redraw, Game* game) {
        //if(isVisible()) {
            //al_draw_filled_rectangle(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, al_map_rgb(255, 255, 255));
         //}
        if (isVisible()) {
            background_.draw(backgroundTransform_, true, 0);
            background_.draw(textTransform_, true, 0);
            getInfoText().drawJustified(al_map_rgb(133, 75, 72), textTransform_.getPosition().getScreenX(), Position::toScreenX(DEFAULT_WINDOW_WIDTH), Position::toScreenY(logFontSize * 2), 1, ALLEGRO_ALIGN_LEFT);
            int i = logFontSize * 2;
            for (LogEntry str : getLog()) {
                str.content_.pop_back();
                getLogCache().setContent(str.content_);
                i += logFontSize;
                getLogCache().drawJustified(str.color_, textTransform_.getPosition().getScreenX(), Position::toScreenX(DEFAULT_WINDOW_WIDTH), Position::toScreenY(i), 1, ALLEGRO_ALIGN_LEFT);
            }
            getTextCache().drawJustified(al_map_rgb(255, 255, 255), textTransform_.getPosition().getScreenX() + consoleFontSize / 8, Position::toScreenX(DEFAULT_WINDOW_WIDTH), textTransform_.getPosition().getScreenY() + textTransform_.getScale().getY() + consoleFontSize, 1, ALLEGRO_ALIGN_LEFT);
            if (showEndSymbol) {
                al_draw_justified_text(getTextCache().getFont()->get(), al_map_rgb(255, 255, 255),
                    (textTransform_.getPosition().getScreenX() + consoleFontSize / 8) + textLength_,
                    Position::toScreenX(DEFAULT_WINDOW_WIDTH),
                    textTransform_.getPosition().getScreenY() + textTransform_.getScale().getY() +
                    consoleFontSize, 1, ALLEGRO_ALIGN_LEFT, "|");
            }
        }
    }

    void Console::charDown(ALLEGRO_EVENT& event, int keycode, int unicode, Game* game) {
        if (isVisible() && keycode != ALLEGRO_KEY_BACKSPACE && keycode != ALLEGRO_KEY_ENTER) {
            input_.append(string{ (char)unicode });
            input_.erase(remove(input_.begin(), input_.end(), '\0'), input_.end());
            input_.append("\0");
            getTextCache().setContent(input_);
            update();
        }
    }

    void Console::create() {
        log_.reserve(DEFAULT_WINDOW_HEIGHT / logFontSize - 2);
        background_.safeLoad();
        logFont_.safeLoad();
        textCache_.getFont()->safeLoad();
        backgroundTransform_.getScale().set(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
        backgroundTransform_.getColor().set(0, 0, 0, 230);
        textTransform_.getColor().set(35, 35, 35, 245);
        textTransform_.getScale().set(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT / 32);
        textTransform_.getPosition().setY(DEFAULT_WINDOW_HEIGHT - (textTransform_.getScale().getY()));
        registerCommand(new HelpCommand{});
        registerCommand(new ClearCommand{});
        registerCommand(new ResolutionCommand{});
        registerCommand(new ExitCommand{});
        registerCommand(new FullscreenCommand{});
    }

    int backSpaceTick = 0;

    void Console::tick(ALLEGRO_EVENT& event, bool& redraw, Game* game) {
        backSpaceTick++;
        if (game->isKeyPressed(ALLEGRO_KEY_BACKSPACE) && backSpaceTick > game->getTickSpeed() / 8) {
            if (!input_.empty()) input_.pop_back(); //actual pop back
            getTextCache().setContent(input_);
            update();
            backSpaceTick = 0;
        }

        endSymbolTick_++;
        if (endSymbolTick_ >= 15) {
            endSymbolTick_ = 0;
            showEndSymbol = !showEndSymbol;
        }
    }

    void Console::mouseEvent(int button, int x, int y, ALLEGRO_EVENT& event, Game* game) {
        if (!isVisible()) return;
        int x1, y1, x2, y2;

        x1 = 0;
        x2 = DEFAULT_WINDOW_WIDTH;

        int i = 0;
        for (auto& item : getLog()) {
            item.color_ = item.cacheColor_;
            y1 = (logFontSize * (i + 1));
            y2 = y1 + logFontSize;
            if (x <= std::max(x1, x2) && x >= std::min(x1, x2) && y <= std::max(y1, y2) && y >= std::min(y1, y2)) {
                string content = item.content_;
                content.pop_back();
                for (const auto& cmd : getCommands()) {
                    if (content.contains(cmd->getId())) {
                        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                            if (cmd->neededArguments() == 0) {
                                cmd->execute({}, this, cmd->getId(), game);
                                return;
                            }
                            input_ = cmd->getId();
                            getTextCache().setContent(cmd->getId());
                            update();
                            return;
                        }
                        else {
                            item.color_ = al_map_rgb(200, 255, 200);
                        }
                    }
                }

                if (item.clickAble) {
                    if (item.clickAction) item.clickAction();
                }
            }
            i++;
        }
    }
}
*/