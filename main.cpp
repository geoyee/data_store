#include "datastore.hpp"

#include <vector>
#include <atomic>
#include <functional>

using namespace mm;

struct MyData
{
    int d1, d2;
    MSGPACK_DEFINE(d1, d2);
};

int main(int argc, char **argv)
{
    DataStore ds;

    if (argc == 1)
    {
        int a = 543;
        float b = 3.14;
        std::string c = "Hello World!";
        MyData d = {1, 2};
        std::vector<double> e = {84.65, 74.23};

        ds.put("a", a);
        ds.put("b", b);
        ds.put("c", c);
        ds.put("d", d);
        ds.put("e", e);
    }
    if (argc == 1 || (argc == 2 && std::string(argv[1]) == "load"))
    {
        std::cout << ds.get<int>("a") << std::endl;
        std::cout << ds.get<float>("b") << std::endl;
        std::cout << ds.get<std::string>("c") << std::endl;
        MyData tmpD = ds.get<MyData>("d");
        std::cout << tmpD.d1 << ", " << tmpD.d2 << std::endl;
        std::vector<double> tmpE = ds.get<std::vector<double>>("e");
        for (auto i : tmpE)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
    if (argc == 2)
    {
        if (std::string(argv[1]) == "remove")
        {
            ds.remove("a");
        }
        else if (std::string(argv[1]) == "clear")
        {
            ds.finish();
        }
        else if (std::string(argv[1]) == "invalid")
        {
            ds.get<std::string>("a");
            ds.get<int>("v");
        }
    }

    return 0;
}