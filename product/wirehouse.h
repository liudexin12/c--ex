#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <stdexcept>
#include <atomic>
#include <utility>
#include <product.h>

namespace prodsale
{
    // 自定义异常类
    class WarehouseException : public std::runtime_error
    {
    public:
        explicit WarehouseException(const std::string &message) : std::runtime_error(message) {}
    };

    // 仓库类模板
    template <typename T>
    class Warehouse
    {
    public:
        // 使用 std::unique_ptr 来管理产品的独占所有权
        void addProduct(std::unique_ptr<Product> product) {
            std::lock_guard<std::mutex> lock(mutex_);
            auto result = products_.emplace(product.getID(), std::move(product));
            if (!result.second)
            {
                throw WarehouseException("产品已存在");
            }
        }

        // 移除产品
        void removeProduct(size_t id)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (products_.erase(id) == 0)
            {
                throw WarehouseException("产品未找到");
            }
        }

        // 更新产品详情
        void updateProduct(size_t id, const std::string &name, T price, int quantity)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = products_.find(id);
            if (it == products_.end())
            {
                throw WarehouseException("产品未找到");
            }
            it->second.setName(name);
            it->second.setPrice(price);
            it->second.setQuantityInStock(quantity);
        }

        // 获取产品信息
        Product<T> getProduct(size_t id)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = products_.find(id);
            if (it == products_.end())
            {
                throw WarehouseException("产品未找到");
            }
            return it->second;
        }

    private:
        std::unordered_map<size_t, std::unique_ptr<Product<T>> products_;
        std::mutex mutex_;
    };
}
