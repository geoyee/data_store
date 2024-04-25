#pragma once

#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include "msgpack.hpp"
#include "mio/mmap.hpp"

#include <iostream>
#include <unordered_map>
#include <string>
#include <type_traits>

class DataStore
{
public:
    DataStore() = default;
    DataStore(const DataStore& other) = delete;
    DataStore(DataStore&& other) = delete;
    ~DataStore() = default;
    DataStore& operator=(const DataStore& other) = delete;

    template<typename T,
             std::enable_if_t<std::is_pointer<std::decay<T>>::value == 1, int> = 1,
             std::enable_if_t<std::is_copy_constructible<std::decay<T>>::value == 1, int> = 1>
    void put(const std::string& key, T value)
    {
        _dataMap[key] = static_cast<void *>(new std::decay<T>(*value));
    }

    template<typename T,
             std::enable_if_t<std::is_pointer<std::decay<T>>::value == 1, int> = 1,
             std::enable_if_t<std::is_copy_constructible<std::decay<T>>::value == 0, int> = 1>
    void put(const std::string& key, T value)
    {
        _dataMap[key] = static_cast<void *>(std::move(value));
    }

    template<typename T,
             std::enable_if_t<std::is_pointer<std::decay<T>>::value == 0, int> = 1,
             std::enable_if_t<std::is_copy_constructible<std::decay<T>>::value == 1, int> = 1>
    void put(const std::string& key, T value)
    {
        _dataMap[key] = static_cast<void *>(new std::decay<T>(value));
    }

    template<typename T,
             std::enable_if_t<std::is_pointer<std::decay<T>>::value == 0, int> = 1,
             std::enable_if_t<std::is_copy_constructible<std::decay<T>>::value == 0, int> = 1>
    void put(const std::string& key, T value)
    {
        _dataMap[key] = static_cast<void *>(std::move(&value));
    }

    template<typename T, std::enable_if_t<std::is_pointer<std::decay<T>>::value == 1, int> = 1>
    T get(const std::string& key)
    {
        return static_cast<T>(_dataMap[key]);
    }

    template<typename T, std::enable_if_t<std::is_pointer<std::decay<T>>::value == 0, int> = 1>
    T get(const std::string& key)
    {
        return *(static_cast<T *>(_dataMap[key]));
    }

    bool find(const std::string& key)
    {
        return _dataMap.find(key) != _dataMap.end();
    }

    void remove(const std::string& key)
    {
        _dataMap.erase(key);
    }

private:
    std::unordered_map<std::string, void *> _dataMap = {};
};

#endif // DATASTORE_HPP