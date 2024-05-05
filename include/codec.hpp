#ifndef __MM_CODEC_H__
#define __MM_CODEC_H__

#include "msgpack.hpp"

#include <iostream>
#include <string>
#include <typeinfo>

namespace mm
{
    namespace codec
    {
        using bufferType = msgpack::sbuffer;

        template <typename T>
        static std::string pack(T &&t)
        {
            bufferType buffer;
            msgpack::pack(buffer, std::forward<T>(t));
            return std::string(buffer.data(), buffer.size());
        }

        template <typename T>
        static T unpack(char const *data, size_t length)
        {
            try
            {
                msgpack::unpacked msg;
                msgpack::unpack(msg, data, length);
                return msg.get().as<T>();
            }
            catch (...)
            {
                throw std::invalid_argument("[Error] unpack failed: type \"" + std::string(typeid(T).name()) + "\" not match");
            }
        }
    } // namespace codec
} // namespace mm

#endif // __MM_CODEC_H__