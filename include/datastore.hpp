#pragma once

#ifndef __MM_DATASTORE_HPP__
#define __MM_DATASTORE_HPP__

#include "codec.hpp"
#include "sqlitef.hpp"

namespace mm
{
    const std::string MMF_FILE_PATH = std::filesystem::current_path().string() + "/mmf.db";

    class DataStore
    {
    public:
        DataStore() : _sqlFile(sqlitef::SqliteFile(MMF_FILE_PATH))
        {
            //_sqlFile.open(MMF_FILE_PATH);
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
            _sqlFile.put(hashKey, strValue);
        }

        template <typename T>
        T get(const std::string &key)
        {
            size_t hashKey = calcHash(key);
            std::string result = _sqlFile.get(hashKey);
            return codec::unpack<T>(result.data(), result.size());
        }

        bool find(const std::string &key)
        {
            size_t hashKey = calcHash(key);
            return _sqlFile.get(hashKey) != "\0";
        }

        void remove(const std::string &key)
        {
            size_t hashKey = calcHash(key);
            _sqlFile.del(hashKey);
        }

        void finish()
        {
            _sqlFile.~SqliteFile();
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

        sqlitef::SqliteFile _sqlFile;
    };
} // namespace mm

#endif // __MM_DATASTORE_HPP__