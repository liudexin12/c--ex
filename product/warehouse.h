#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "product.h"
#include "warehouse_exception.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>

template <typename T>
class Warehouse
{
public:
    // 添加新产品
    void addProduct(std::unique_ptr<Product<T>> product)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t id = product->getId();
        if (products_.find(id) != products_.end())
        {
            throw WarehouseException("产品ID已存在: " + std::to_string(id));
        }
        products_[id] = std::move(product);
        std::cout << "添加产品: " << products_[id]->getName() << " 数量: " << products_[id]->getQuantity() << "\n";
    }

    // 删除产品
    void removeProduct(size_t id)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = products_.find(id);
        if (it != products_.end())
        {
            std::cout << "移除产品: " << it->second->getName() << "\n";
            products_.erase(it);
        }
        else
        {
            throw WarehouseException("产品ID不存在: " + std::to_string(id));
        }
    }

    // 更新产品详细信息
    void updateProduct(size_t id, const std::string &name, T price, int quantity)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = products_.find(id);
        if (it != products_.end())
        {
            it->second->setPrice(price);
            it->second->setQuantity(quantity);
            std::cout << "更新产品: " << it->second->getName() << " 新名称: " << name << " 新价格: " << price << " 新数量: " << quantity << "\n";
        }
        else
        {
            throw WarehouseException("产品ID不存在: " + std::to_string(id));
        }
    }

    // 检索产品信息
    std::shared_ptr<Product<T>> getProduct(size_t id)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = products_.find(id);
        if (it != products_.end())
        {
            return std::make_shared<Product<T>>(*it->second);
        }
        else
        {
            throw WarehouseException("产品ID不存在: " + std::to_string(id));
        }
    }

    // 列出所有产品
    void listProducts() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (products_.empty())
        {
            std::cout << "仓库中没有产品。\n";
        }
        else
        {
            std::cout << "产品列表:\n";
            for (const auto &product : products_)
            {
                std::cout << "ID: " << product.first << " 名称: " << product.second->getName() << " 数量: " << product.second->getQuantity() << "\n";
            }
        }
    }

private:
    std::unordered_map<size_t, std::unique_ptr<Product<T>>> products_;
    mutable std::mutex mutex_; // 用于保护产品数据的互斥锁
};

#endif