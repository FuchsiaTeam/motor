/* Jayson */
#pragma once

//TODO: TIDY
#define FUCHSLIB_BUILD_DISCORD
#ifdef FUCHSLIB_BUILD_DISCORD
#define CURL_STATICLIB

#include <fuchslib/shorter.hpp>
#include <curl/curl.h>
#include <discord/discord.h>
#include <memory>
#include <csignal>
#include <thread>
#include <chrono>
#include <fstream>
#include "log.hpp"
#include "json.hpp"

inline size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

inline size_t callbackfunction(void* ptr, size_t size, size_t nmemb, void* userdata) {
    FILE* stream = (FILE*)userdata;
    if (!stream) {
        //Fuchs::Con::log("Could not create filestream", cout);
        MOTOR_LOG("Could not create filestream");
        return 0;
    }

    size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
    return written;
}


namespace Fuchs::Discord {

#ifdef WINDOWS
#pragma pack(push, 1)
    struct BitmapImageHeader {
        uint32_t const structSize{ sizeof(BitmapImageHeader) };
        int32_t width{ 0 };
        int32_t height{ 0 };
        uint16_t const planes{ 1 };
        uint16_t const bpp{ 32 };
        uint32_t const pad0{ 0 };
        uint32_t const pad1{ 0 };
        uint32_t const hres{ 2835 };
        uint32_t const vres{ 2835 };
        uint32_t const pad4{ 0 };
        uint32_t const pad5{ 0 };

        BitmapImageHeader& operator=(BitmapImageHeader const&) = delete;
    };

    struct BitmapFileHeader {
        uint8_t const magic0{ 'B' };
        uint8_t const magic1{ 'M' };
        uint32_t size{ 0 };
        uint32_t const pad{ 0 };
        uint32_t const offset{ sizeof(BitmapFileHeader) + sizeof(BitmapImageHeader) };

        BitmapFileHeader& operator=(BitmapFileHeader const&) = delete;
    };
using namespace nlohmann;
#pragma pack(pop)
#endif

    namespace {
        volatile bool interrupted = false;
    }

    class Discord {
        discord::User currentUser_;
        discord::Core* core_;
        discord::Activity activity_{};
        bool created_ = false;
        bool refreshActivity_ = false;
        std::thread dThread;
    public:

        discord::Core* getCore() { return this->core_; }
        discord::User& getCurrentUser() { return this->currentUser_; }
        discord::Activity& getActivity() { return this->activity_; }
        bool isCreated() { return this->created_; }
        void interrupt() { interrupted = true; }

        void refreshActivity() { this->refreshActivity_ = true; }

        void start(discord::ClientId clientId, uintmax flags) {
            if (!dThread.joinable()) dThread = std::thread{ [this, clientId, flags] {
                this->create(clientId, flags);
            } };
        }

        static json getUserRawData(uintmax id, const string& token) {
            auto curl = curl_easy_init();
            if (curl) {
                string urlStr = string{ "https://discord.com/api/users/" + std::to_string(id) };
                cstring url = urlStr.c_str();
                curl_easy_setopt(curl, CURLOPT_URL, url);
                string response_string;
                int response_code = 1;
                curl_slist* headers = NULL;
                headers = curl_slist_append(headers, string{ "Authorization: Bot " + token }.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                /* Segment Fault in Linux with those LMAO */
                //curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                //curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                if (response_string.empty() || !json::accept(response_string)) {
                    MOTOR_LOG("reponse empty or json not valid");
                    return json{ "{}" };
                }
                return json::parse(response_string);
            }
            MOTOR_LOG("could not create curl");
            return json{ "{}" };
        }

        static json getGuildRawData(uintmax id, const string& token) {
            auto curl = curl_easy_init();
            if (curl) {
                string urlStr = string{ "https://discord.com/api/guilds/" + std::to_string(id) };
                cstring url = urlStr.c_str();
                curl_easy_setopt(curl, CURLOPT_URL, url);
                string response_string;
                int response_code = 1;
                curl_slist* headers = NULL;
                headers = curl_slist_append(headers, string{ "Authorization: Bot " + token }.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                /* Segment Fault in Linux with those LMAO */
                //curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
                //curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20);
                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                if (response_string.empty() || !json::accept(response_string)) return json{ "{}" };
                return json::parse(response_string);
            }
            return json{ "{}" };
        }

        static json getData(const string& apiUrl, const string& token) {
            auto curl = curl_easy_init();
            if (curl) {
                cstring url = apiUrl.c_str();
                curl_easy_setopt(curl, CURLOPT_URL, url);
                string response_string;
                curl_slist* headers = NULL;
                headers = curl_slist_append(headers, string{ "Authorization: Bot " + token }.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                return json::parse(response_string);
            }
            return json{ "{}" };
        }

        static string getAvatarId(const json& rawData) {
            if (!rawData.contains("avatar")) {
                MOTOR_LOG("json did not contain avatar key");
                return "";
            }
            return rawData.at("avatar");
        }

        static string getAvatarId(uintmax id, const string& token) {
            json js = getUserRawData(id, token);
            if (!js.contains("avatar")) {
                MOTOR_LOG("json did not contain avatar key");
                return "";
            }
            return js.at("avatar");
        }

        static string getAvatarURL(const json& rawData, uint16 size) {
            string avatar = getAvatarId(rawData);
            if (avatar.empty()) {
                MOTOR_LOG("avatar id empty");
                return "";
            }
            string id = rawData.at("id");
            return string{ "https://cdn.discordapp.com/avatars/" + id + "/" + avatar + ".webp?size=" + to_string(size)};
        }

        static string getAvatarURL(uintmax id, uint16 size, const string& token) {
            string avatar = getAvatarId(id, token);
            if (avatar.empty()) {
                MOTOR_LOG("avatar id empty");
                return "";
            }
            return string{ "https://cdn.discordapp.com/avatars/" + to_string(id) + "/" + avatar + ".webp?size=" + to_string(size) };
        }

        static bool saveAvatar(uintmax id, const string& file, uint16 size, const string& token) {
            return saveAvatar(file, getAvatarURL(id, size, token));
        }

        static bool saveAvatar(const json& rawUserData, const string& file, uint16 size) {
            return saveAvatar(file, getAvatarURL(rawUserData, size));
        }


        static bool saveAvatar(const string& file, const string& urlStr) {
            FILE* fp = fopen(file.c_str(), "wb");
            if (!fp) {
                //Con::log("Could not create file " + file, cout);
                MOTOR_LOG("Could not create file " + file);
                return false;
            }
            if (urlStr.empty()) {
                MOTOR_LOG("urlstr empty");
                return false;
            }
            const char* url = urlStr.c_str();
            auto curl = curl_easy_init();
            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackfunction);
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

                CURLcode rc = curl_easy_perform(curl);
                if (rc) {
                    //Con::log("Could not download " + urlStr, cout);
                    MOTOR_LOG("Could not download " + urlStr);
                    return false;
                }

                long res_code = 0;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
                if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK)) {
                    //Con::log("Wrong response code : " + res_code, cout);
                    MOTOR_LOG("Wrong response code : " + res_code);
                    return false;
                }

                curl_easy_cleanup(curl);
            }
            else {
                MOTOR_LOG("could not create curl");
            }
            fflush(fp);
            fclose(fp);
            return true;
        }


        void saveAvatarDAP(const string& file) {
            saveAvatarDAP(getCurrentUser().GetId(), file);
        }

        void saveAvatarDAP(uintmax id, const string& file) {
            discord::ImageHandle handle{};
            handle.SetId(id);
            handle.SetType(discord::ImageType::User);
            handle.SetSize(256);
            getCore()->ImageManager().Fetch(handle, true, [this, &file](discord::Result res, discord::ImageHandle handle) {
                if (res == discord::Result::Ok) {
                    discord::ImageDimensions dims{};
                    getCore()->ImageManager().GetDimensions(handle, &dims);
                    std::cout << "Fetched " << dims.GetWidth() << "x" << dims.GetHeight()
                        << " avatar!\n";

                    std::vector<uint8_t> data;
                    data.reserve(dims.GetWidth() * dims.GetHeight() * 4);
                    uint8_t* d = data.data();
                    getCore()->ImageManager().GetData(handle, d, data.size());

#ifdef WINDOWS
                    auto fileSize = data.size() + sizeof(BitmapImageHeader) + sizeof(BitmapFileHeader);

                    BitmapImageHeader imageHeader;
                    imageHeader.width = static_cast<int32_t>(dims.GetWidth());
                    imageHeader.height = static_cast<int32_t>(dims.GetHeight());

                    BitmapFileHeader fileHeader;
                    fileHeader.size = static_cast<uint32_t>(fileSize);

                    FILE* fp = fopen(file.c_str(), "wb");
                    fwrite(&fileHeader, sizeof(BitmapFileHeader), 1, fp);
                    fwrite(&imageHeader, sizeof(BitmapImageHeader), 1, fp);

                    for (auto y = 0u; y < dims.GetHeight(); ++y) {
                        auto pixels = reinterpret_cast<uint32_t const*>(data.data());
                        auto invY = dims.GetHeight() - y - 1;
                        fwrite(
                            &pixels[invY * dims.GetWidth()], sizeof(uint32_t) * dims.GetWidth(), 1, fp);
                    }

                    fflush(fp);
                    fclose(fp);
#endif
                }
                else {
                    std::cout << "Failed fetching avatar. (err " << static_cast<int>(res) << ")\n";
                }
                });
        }


    private:
        discord::Result updateActivity() { discord::Result res; getCore()->ActivityManager().UpdateActivity(getActivity(), [&res](discord::Result result) { res = result; }); return res; }

        discord::Result createCore(discord::ClientId clientId, uintmax flags) {
            discord::Result result = discord::Core::Create(clientId, flags, &core_);
            if (!getCore()) {
                //Con::log("Could not instantiate discord core -> " + static_cast<int>(result), cout);
                MOTOR_LOG("Could not instantiate discord core -> " + static_cast<int>(result));
            }
            return result;
        }

        void connectUser() { getCore()->UserManager().OnCurrentUserUpdate.Connect([this]() { getCore()->UserManager().GetCurrentUser(&currentUser_); }); }

        void create(discord::ClientId clientId, uintmax flags) {
            if (createCore(clientId, flags) == discord::Result::Ok) {
                connectUser();
                this->created_ = true;
                signal(SIGINT, [](int) { interrupted = true; });
                do {
                    getCore()->RunCallbacks();
                    if (refreshActivity_) updateActivity(); refreshActivity_ = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(120));
                } while (!interrupted);
            }
        }
    };
}

#endif