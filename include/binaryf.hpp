#ifndef __MM_BINARYF_H__
#define __MM_BINARYF_H__

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <unordered_map>

namespace mm
{
    namespace binaryf
    {
        namespace fs = std::filesystem;

        class BinaryFile
        {
        public:
            BinaryFile() : _isInit(true) {}

            BinaryFile(const std::string &path)
            {
                open(path);
            }

            ~BinaryFile()
            {
                _bfs.close();
            }

            BinaryFile(const BinaryFile &other) = delete;
            BinaryFile(BinaryFile &&other) = delete;
            BinaryFile &operator=(const BinaryFile &other) = delete;

            void open(const std::string &path)
            {
                _isInit = fs::exists(path);
                _bfs.open(path, std::ios::in | std::ios::out | std::ios::binary);
                if (!_bfs.is_open())
                {
                    throw std::runtime_error("[Error] DataStore: Cannt open file");
                }
                fs::perms permissions = fs::perms::owner_read | fs::perms::owner_write | fs::perms::group_read | fs::perms::others_read;
                fs::permissions(path, permissions);
            }

            bool isInit()
            {
                return _isInit;
            }

            size_t size()
            {
                _bfs.seekg(0, std::ios::end);
                std::streamsize size = _bfs.tellg();
                _bfs.seekg(0, std::ios::beg);
                return size;
            }

            size_t offset()
            {
                return _bfs.tellg();
            }

            size_t put(size_t key, const std::string &value)
            {
                _bfs << key << value.size() << value.data();
                return _bfs.tellp();
            }

            std::string get(size_t offset)
            {
                char *temp = new char[sizeof(size_t)];
                std::streamsize curOff = _bfs.tellp();
                _bfs.seekg(offset + sizeof(size_t), std::ios::beg);
                size_t size = 0;
                _bfs.read(temp, sizeof(size_t));
                std::memcpy(&size, temp, sizeof(size_t));
                std::string value(size, '\0');
                _bfs.read(value.data(), size);
                _bfs.seekg(curOff, std::ios::beg);
                delete[] temp;
                return value;
            }

            void swap(size_t offset, const std::string &value)
            {
                char *temp = new char[sizeof(size_t)];
                std::streamsize curOff = _bfs.tellp();
                _bfs.seekp(offset + sizeof(size_t), std::ios::beg);
                size_t size = 0;
                _bfs.read(temp, sizeof(size_t));
                std::memcpy(&size, temp, sizeof(size_t));
                if (size != value.size())
                {
                    delete[] temp;
                    throw std::runtime_error("[Error] BinaryFile: Cannot swap, size not match");
                }
                _bfs.write(value.data(), value.size());
                _bfs.seekp(curOff, std::ios::beg);
                delete[] temp;
            }

            void mmap(std::unordered_map<size_t, size_t> &indexMap)
            {
                char *temp = new char[sizeof(size_t)];
                _bfs.seekg(0, std::ios::beg);
                size_t len = size();
                while (_bfs.tellg() < len)
                {
                    size_t key = 0;
                    size_t size = 0;
                    _bfs.read(temp, sizeof(size_t));
                    std::memcpy(&key, temp, sizeof(size_t));
                    _bfs.read(temp, sizeof(size_t));
                    std::memcpy(&size, temp, sizeof(size_t));
                    indexMap[key] = _bfs.tellg();
                    _bfs.seekg(size, std::ios::cur);
                }
                _bfs.seekg(0, std::ios::beg);
                delete[] temp;
            }

        private:
            bool _isInit = true;
            std::fstream _bfs;
        };
    } // namespace binaryf
} // namespace mm

#endif // __MM_BINARYF_H__