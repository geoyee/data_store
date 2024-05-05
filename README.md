# data_store

一个基于 msgpack 和 sqlite3 的数据仓库类，可以对数据进行持久化存储

## 示例

```c++
DataStore ds;

int a = 543;
std::string c = "Hello World!";
std::vector<double> e = {84.65, 74.23};

// 存取数据
ds.put("a", a);
ds.put("c", c);
ds.put("e", e);

// 读取数据
int a2 = ds.get<int>("a");
int c2 = ds.get<std::string>("c");
int e2 = ds.get<std::vector<double>>("e");
```

由于数据存储在数据库中，因此当程序崩溃或其他情况下，重启程序仍然可以取到之前保存的数据。同时存储 msgpack 序列化的数据，能够更好的节约内存

```c++
DataStore ds;

// 读取数据
int a2 = ds.get<int>("a");  // ok
int c2 = ds.get<std::string>("c");  // ok
int e2 = ds.get<std::vector<double>>("e");  // ok
```

程序正常退出前再清理数据库，下次重启程序后拥有全新的数据仓库

```c++
...

ds.finish();
```
