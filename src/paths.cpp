#include "motor/paths.hpp"
#include "motor/log.hpp"
#include <fuchslib/utility.hpp>
#include <fuchslib/shorter.hpp>
#include <codecvt>

#ifdef WINDOWS
#include <Windows.h>
#include <ShlObj.h>
#endif

#ifdef UNIX
#include <unistd.h>
#include <pwd.h>
#endif

namespace Motor::Path {

    string docs;
    string profiles;
    string worlds;
    string mods;
    string scripts;
    string libraries;

    void set_save_game_folder(string gameName) {
#ifdef WINDOWS
        PWSTR   documentFoldWIND;
            HRESULT hr = SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &documentFoldWIND);
            std::wstring documentPath;
            if (SUCCEEDED(hr)) {
                documentPath = documentFoldWIND;
            }
            CoTaskMemFree(documentFoldWIND);
            docs = string{string{documentPath.begin(), documentPath.end()} + "\\My Games\\" + gameName + "\\"};
            if(docs.contains(gameName)) {
                CreateDirectory(docs.c_str(), nullptr);
                profiles = docs + "saves\\profiles\\";
                worlds = docs + "saves\\worlds\\";
                mods = docs + "mods\\";
                scripts = mods + "scripts\\";
                libraries = mods + "libraries\\";
            }
#endif

#ifdef UNIX
        passwd* pw = getpwuid(getuid());
            #ifdef LINUX
                docs = string{pw->pw_dir} + "/.local/share/" + gameName + "/";
            #endif
            #ifdef MACOS
                docs = string{homedir} + "/Library/Application Support/" + gameName + "/";
            #endif
            profiles = docs + "saves/profiles/";
            worlds = docs + "saves/worlds/";
            mods = docs + "mods/";
            scripts = mods + "scripts/";
            libraries = mods + "libraries/";
#endif

        //Fuchs::Utility::createDirectories(profiles);
        //Fuchs::Utility::createDirectories(worlds);
        //Fuchs::Utility::createDirectories(mods);
        //Fuchs::Utility::createDirectories(scripts);
        //Fuchs::Utility::createDirectories(libraries);
        Fuchs::Utility::createDirectories(docs);
        MOTOR_LOG("Using docs Directory: " + docs);
    }

}