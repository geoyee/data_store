#ifndef __MM_SQLITEF_H__
#define __MM_SQLITEF_H__

#include "sqlite3.h"

#include <iostream>
#include <string>
#include <cstring>
#include <exception>
#include <filesystem>

namespace mm
{
namespace sqlitef
{
class SqliteFile
{
public:
    SqliteFile(const std::string& path) : _fileName(path)
    {
        open();
        // close();
    }

    ~SqliteFile()
    {
        close();
    }

    SqliteFile(const SqliteFile& other) = delete;
    SqliteFile(SqliteFile&& other) = delete;
    SqliteFile& operator=(const SqliteFile& other) = delete;

    bool put(size_t key, const std::string& value)
    {
        char *zErrMsg = NULL;
        char *sql = sqlite3_mprintf(
            "INSERT OR REPLACE INTO Datas VALUES(%s, '%q')", std::to_string(key).c_str(), value.c_str());
        open();
        if (sqlite3_exec(_sdb, sql, NULL, NULL, &zErrMsg) != SQLITE_OK)
        {
            std::cerr << "[Error] sql failed in put: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_free(sql);
            close();
            return false;
        }
        sqlite3_free(sql);
        // close();
        return true;
    }

    bool del(size_t key)
    {
        char *zErrMsg = NULL;
        char *sql = sqlite3_mprintf("DELETE FROM Datas WHERE Key=%s", std::to_string(key).c_str());
        open();
        if (sqlite3_exec(_sdb, sql, NULL, NULL, &zErrMsg) != SQLITE_OK)
        {
            std::cerr << "[Error] sql failed in del: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_free(sql);
            close();
            return false;
        }
        sqlite3_free(sql);
        // close();
        return true;
    }

    std::string get(size_t key)
    {
        auto callback = [](void *data, int argc, char **argv, char **azColName) -> int
        {
            if (argc == 0)
            {
                return 1;
            }
            else
            {
                for (int i = 0; i < argc; ++i)
                {
                    if (strcmp(azColName[i], "Value") == 0)
                    {
                        memcpy(data, argv[i], strlen(argv[i]));
                        return 0;
                    }
                }
            }
            return 1;
        };

        char *zErrMsg = NULL;
        std::string valMsg;
        char *sql = sqlite3_mprintf("SELECT * FROM Datas WHERE Key=%s", std::to_string(key).c_str());
        open();
        if (sqlite3_exec(_sdb, sql, callback, valMsg.data(), &zErrMsg) != SQLITE_OK)
        {
            std::cerr << "[Error] sql failed in get: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
            sqlite3_free(sql);
            close();
            return "\0";
        }
        sqlite3_free(sql);
        // close();
        std::string result(valMsg.data());
        return result;
    }

private:
    void open()
    {
        bool init = !std::filesystem::exists(_fileName);
        if (sqlite3_open(_fileName.c_str(), &_sdb) != SQLITE_OK)
        {
            sqlite3_close(_sdb);
            throw std::runtime_error("[Error] sql failed in open: Cannt open or create database");
        }
        if (init)
        {
            char *zErrMsg = NULL;
            const char *sql = "CREATE TABLE IF NOT EXISTS Datas(Key BIGINT PRIMARY KEY NOT NULL, Value BLOB NOT NULL)";
            if (sqlite3_exec(_sdb, sql, NULL, NULL, &zErrMsg) != SQLITE_OK)
            {
                std::string errStr(zErrMsg);
                sqlite3_free(zErrMsg);
                // sqlite3_close(_sdb);
                throw std::runtime_error("[Error] sql failed in create: " + errStr);
            }
        }
    }

    void close()
    {
        sqlite3_close(_sdb);
    }

    std::string _fileName;
    sqlite3 *_sdb = NULL;
};
} // namespace sqlitef
} // namespace mm

#endif // __MM_SQLITEF_H__