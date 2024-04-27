#pragma once

#ifndef __MM_DATASTORE_HPP__
#define __MM_DATASTORE_HPP__

#include "codec.hpp"
#include "binaryf.hpp"

namespace mm
{
    namespace fs = std::filesystem;

    const std::string MMF_FILE_PATH = std::filesystem::current_path().string() + "/db.mmf";

    class DataStore
    {
    public:
        DataStore() : _offsetMap({})
        {
            _binFile.open(MMF_FILE_PATH);
            if (_binFile.isInit())
            {
                _binFile.mmap(_offsetMap);
            }
        }

        DataStore(const DataStore &other) = delete;
        DataStore(DataStore &&other) = delete;
        DataStore &operator=(const DataStore &other) = delete;
        ~DataStore() = default;

        template <typename T>
        void put(const std::string &key, T value)
        {
            size_t hashKey = calcHash(key);
            std::string strValue = codec::pack(value);
            if (find(key))
            {
                _binFile.swap(_offsetMap[hashKey], strValue);
            }
            else
            {
                _binFile.put(hashKey, strValue);
            }
        }

        template <typename T>
        T get(const std::string &key)
        {
            size_t offset = _offsetMap.at(calcHash(key));
            std::string result = _binFile.get(offset);
            return codec::unpack<T>(result.data(), result.size());
        }

        bool find(const std::string &key)
        {
            return _offsetMap.find(calcHash(key)) != _offsetMap.end();
        }

        void remove(const std::string &key)
        {
            _offsetMap.erase(calcHash(key));
        }

        void finish()
        {
            _offsetMap.clear();
            if (std::filesystem::exists(MMF_FILE_PATH))
            {
                std::filesystem::remove(MMF_FILE_PATH);
            }
        }

    private:
        size_t calcHash(const std::string &str)
        {
            return std::hash<std::string>{}(str);
        }

        binaryf::BinaryFile _binFile;
        std::unordered_map<size_t, size_t> _offsetMap;
    };
} // namespace mm

#endif // __MM_DATASTORE_HPP__