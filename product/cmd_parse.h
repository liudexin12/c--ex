#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <sstream>

namespace prodsale
{
    // 命令解析器类
    class CommandParser
    {
    public:
        CommandParser(Warehouse &warehouse) : warehouse_(warehouse)
        {
            commands_["add"] = [this](const std::string &args)
            { this->addProduct(args); };
            commands_["remove"] = [this](const std::string &args)
            { this->removeProduct(args); };
            commands_["update"] = [this](const std::string &args)
            { this->updateQuantity(args); };
            commands_["list"] = [this](const std::string &)
            { this->listProducts(); };
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
                std::cout << "无效的命令: " << command << "\n";
            }
        }

    private:
        void addProduct(const std::string &args)
        {
            std::istringstream iss(args);
            int id;
            std::string name;
            int quantity;
            if (iss >> id >> std::ws && std::getline(iss, name, ' ') && iss >> quantity)
            {
                warehouse_.addProduct(id, name, quantity);
            }
            else
            {
                std::cout << "无效的参数。用法: add <id> <name> <quantity>\n";
            }
        }

        void removeProduct(const std::string &args)
        {
            std::istringstream iss(args);
            int id;
            if (iss >> id)
            {
                warehouse_.removeProduct(id);
            }
            else
            {
                std::cout << "无效的参数。用法: remove <id>\n";
            }
        }

        void updateQuantity(const std::string &args)
        {
            std::istringstream iss(args);
            int id, quantity;
            if (iss >> id >> quantity)
            {
                warehouse_.updateQuantity(id, quantity);
            }
            else
            {
                std::cout << "无效的参数。用法: update <id> <quantity>\n";
            }
        }

        void listProducts()
        {
            warehouse_.listProducts();
        }

        Warehouse &warehouse_;
        std::map<std::string, std::function<void(const std::string &)>> commands_;
    };

}
