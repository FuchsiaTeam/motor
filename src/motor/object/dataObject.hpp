#pragma once

#include "vector"
#include "map"
#include "string"

namespace Motor {
    class DataObject {
    public:
        std::map<std::string, void*> datas_{};

        virtual ~DataObject() = default;

        bool hasDataTag(const std::string& tag) {
            for (auto const& [key, val] : datas_) {
                if(key == tag) {
                    return true;
                }
            }
            return false;
        }

        template<typename T>
        T* getData(const std::string& tag) {
            for (auto const& [key, val] : datas_) {
                if(key == tag) {
                    return (T*)val;
                }
            }
            return nullptr;
        }

        template<typename T>
        void setData(const std::string& tag, void* data) {
            T* existingData = getData<T>(tag);
            if(existingData) {
                existingData = data;
            } else {
                datas_.insert(std::pair(tag, data));
            }
        }

        void deleteData() {

        }

        void removeData() {
            
        }
    };
}