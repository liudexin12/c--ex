#include "product/product.h"
// #include <iostream>
// #include <string>
// #include <atomic>
// #include <utility>
#include "product/warehouse.h"
#include "product/warehouse_test.cpp"
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <sstream>
#include "log/logg.h"
#include "test/test.h"
// #include "log/logg.h"

// 供应商线程函数
template <typename T>
void supplierThread(Warehouse<T> &warehouse, int supplierId, size_t productId, int minUpdate, int maxUpdate)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minUpdate, maxUpdate);

    while (true)
    {
        int updateAmount = dis(gen);
        try
        {   
            auto product = warehouse.getProduct(productId);
            warehouse.updateProduct(productId, product->getName(), product->getPrice(), product->getQuantity() + updateAmount);
        }
        catch (const WarehouseException &e)
        {
            Logger::getInstance().log(LogLevel::ERROR,std::string("供应商线程错误: ") + e.what());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟供应商更新频率
    }
}

// 库存检查线程函数
template <typename T>
void inventoryCheckThread(Warehouse<T> &warehouse, int checkInterval, int lowStockThreshold)
{   
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(checkInterval));
        warehouse.inventoryCheckThread(checkInterval, lowStockThreshold);
    }
}

// 命令解析器类
class CommandParser
{
public:
    CommandParser(Warehouse<double> &warehouse) : warehouser_(warehouse)
    {
        commands_["add"] = [this](const std::string &args)
        { this->addCommand(args); };
        commands_["delete"] = [this](const std::string &args)
        { this->deleteCommand(args); };
        commands_["update"] = [this](const std::string &args)
        { this->updateCommand(args); };
    }

    void parseCommand(const std::string &input)
    {
        std::istringstream iss(input);
        std::string command;
        iss >> command;

        auto it = commands_.find(command);
        if (it != commands_.end())
        {
            std::string args;
            std::getline(iss, args);
            it->second(args);
        }
        else
        {
            std::cout << "Invalid command: " << command << std::endl;
        }
    }

private:
    void addCommand(const std::string &args)
    {
        std::istringstream iss(args);

        size_t id;        // 产品ID
        std::string name; // 产品名称
        double price;      // 产品价格
        int quantity;     // 库存
        iss >> id >> name >> price >> quantity;
        Product<double>* product = new Product<double>(id, name, price, quantity);
        std::unique_ptr < Product<double>> p(product);
        warehouser_.addProduct(std::move(p));
    }

    void deleteCommand(const std::string &args)
    {
        std::istringstream iss(args);
        size_t id; // 产品ID
        iss >> id;
        warehouser_.removeProduct(id);
    }

    void updateCommand(const std::string &args)
    {
        std::istringstream iss(args);
        size_t id;        // 产品ID
        std::string name; // 产品名称
        double price;      // 产品价格
        int quantity;     // 库存
        iss >> id >> name >> price >> quantity;
        warehouser_.updateProduct(id, name, price, quantity);
    }

    Warehouse<double> &warehouser_;
    std::map<std::string, std::function<void(const std::string &)>> commands_;
};

// int main()
// {
//     Warehouse<double> warehouse;
//     CommandParser parser(warehouse);

//     std::string input;
//     while (true)
//     {
//         std::cout << "Enter command: ";
//         std::getline(std::cin, input);
//         if (input == "exit")
//         {
//             break;
//         }
//         parser.parseCommand(input);
//     }

//     return 0;
// }

// int main()
// {
//     Warehouse<double> warehouse;

//     // 添加一些产品
//     try
//     {
//         warehouse.addProduct(std::make_unique<Product<double>>(1, "Product A", 99.99, 100));
//         warehouse.addProduct(std::make_unique<Product<double>>(2, "Product B", 49.99, 200));
//     }
//     catch (const WarehouseException &e)
//     {
//         Logger::getInstance().log(LogLevel::ERROR,std::string("添加产品错误: ") + e.what());
//     }

//     // 创建供应商线程
//     std::vector<std::thread> supplierThreads;
//     for (int i = 0; i < 3; ++i)
//     {
//         supplierThreads.emplace_back(supplierThread<double>, std::ref(warehouse), i, 1, -10, 10);
//     }

//     // 创建库存检查线程
//     std::thread inventoryThread(inventoryCheckThread<double>, std::ref(warehouse), 5, 100);

//     // 等待所有供应商线程完成（在这个示例中，它们将永远运行）
//     for (auto &thread : supplierThreads)
//     {
//         thread.join();
//     }

//     // 等待库存检查线程完成（在这个示例中，它将永远运行）
//     inventoryThread.join();

//     return 0;
// }

// int main()
// {
//     Product<double> product1(1, "Product A", 99.99, 100);
//     std::cout << "ID: " << product1.getId() << ", Name: " << product1.getName() << ", Price: " << product1.getPrice() << ", Quantity: " << product1.getQuantity() << std::endl;

//     product1.setPrice(89.99);
//     product1.increaseQuantity(10);
//     std::cout << "Updated Price: " << product1.getPrice() << ", Updated Quantity: " << product1.getQuantity() << std::endl;

//     // 使用移动构造函数
//     Product<double> product2(std::move(product1));
//     std::cout << "Moved Product - ID: " << product2.getId() << ", Name: " << product2.getName() << ", Price: " << product2.getPrice() << ", Quantity: " << product2.getQuantity() << std::endl;

//     return 0;
// }

// int main()
// {
//     logger.log(LogLevel::INFO,std::string("开始运行"));
//     return 0;
// }

int main() {
    TestRunner runner;
    runner.addTest("testAddProduct", testAddProduct);
    runner.addTest("testUpdateQuantity", testUpdateQuantity);
    runner.run();
    return 0;
}
