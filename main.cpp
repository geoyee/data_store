#include "datastore.hpp"

#include <vector>

using namespace mm;

struct MyData
{
    int d1, d2;
    MSGPACK_DEFINE(d1, d2);
};

int main(int argc, char **argv)
{
    DataStore ds;

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

    return 0;
}