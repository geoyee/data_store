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
        // std::atomic<bool> f = true;
        // std::function<int(int, int)> g = [](int x1, int x2)
        // { return x1 + x2; };

        ds.put("a", a);
        ds.put("b", b);
        ds.put("c", c);
        ds.put("d", d);
        ds.put("e", e);
        // ds.put("f", f);
        // ds.put("g", g);
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
        // std::atomic<bool> tmpF = ds.get<std::atomic<bool>>("f");
        // std::cout << (tmpF.load() ? "true" : "false") << std::endl;
        // std::function<int(int, int)> tmpG = ds.get<std::function<int(int, int)>>("g");
        // std::cout << "7 + 8 = " << tmpG(7, 8) << std::endl;
    }
    if (argc == 2 && std::string(argv[1]) == "clear")
    {
        ds.finish();
    }

    return 0;
}