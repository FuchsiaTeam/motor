#pragma once

#include <fuchslib/shorter.hpp>
#include <fstream>
#include <iostream>
#include <curl/curl.h>
#include "paths.hpp"
#include "sstream"

#ifdef WINDOWS
#include <windows.h>
#include <psapi.h>
#endif

#ifdef LINUX
#include <sys/sysinfo.h>
#endif

namespace Motor {
    /* THIS IS SHIT, BUT IT WORKS!*/
    //TODO: DOES NOT WORK IN LINUX
    inline string getActualURLOneDrive(const string& url) {
        auto curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            try { std::filesystem::create_directories(Path::docs + "/dumps/"); } catch (std::exception& e) { cout << e.what() << endl; }
            string path = string{Path::docs + "/dumps/od_0"};
            FILE* logfile = fopen(path.c_str(), "wb");
            if(logfile) {
                curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                curl_easy_setopt(curl, CURLOPT_STDERR, logfile);
                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                fflush(logfile);
                fclose(logfile);
                ifstream file = ifstream{path};
                if (file.good()) {
                    string content = string((std::istreambuf_iterator<char>(file)),
                                            std::istreambuf_iterator<char>());
                    file.close();
#ifdef WINDOWS
                    size_t locationStart = content.find("< Location: ");
                        size_t varyStart = content.find("< Vary:");
                        if (locationStart != string::npos && varyStart != string::npos) {
                            content = content.replace(0, locationStart + 12, "");
                            varyStart = content.find("< Vary:");
                            content = content.replace(varyStart - 1, content.size(), "");
                            remove(path.c_str());
                            return content;
                        }
#endif
#ifdef UNIX
                    size_t locationStart = content.find("< location: ");
                        size_t varyStart = content.find("< vary:");
                        if (locationStart != string::npos && varyStart != string::npos) {
                            content = content.replace(0, locationStart + 12, "");
                            varyStart = content.find("< vary:");
                            content = content.replace(varyStart - 2, content.size(), "");
                            remove(path.c_str());
                            return content;
                        }
#endif
                }
            }
            return "";
        }
        return "";
    }

    /*        void save() {
            std::filesystem::create_directory(PTH::docs);
            std::ofstream out(PTH::docs + "mas.json");
            out << setw(4) << asJson() << endl;
        }

        void load() {
            ifstream file(PTH::docs + "mas.json");
            if(file.good()) {
                string content{(istreambuf_iterator<char>(file)), (istreambuf_iterator<char>())};
                if(json::accept(content)) {
                    json j = json::parse(content);
                    js::getIfHas(j, "key_mapping", keyMapping_);
                    js::getIfHas(j, "settings", settings_);
                }
            } else {
                save();
            }
        }*/
}

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 6) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}