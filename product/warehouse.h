#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "product.h"
#include "warehouse_exception.h"
#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>
#include "log/logg.h"

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
        Logger::getInstance().log(LogLevel::INFO, std::string("添加产品: ") + products_[id]->getName() + std::string(" 数量: ") + std::to_string(products_[id]->getQuantity()));
    }

    // 删除产品
    void removeProduct(size_t id)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = products_.find(id);
        if (it != products_.end())
        {
            Logger::getInstance().log(LogLevel::INFO, std::string("移除产品: ") + it->second->getName());
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
            Logger::getInstance().log(LogLevel::INFO, std::string("更新产品: ") + it->second->getName() + std::string(" 新价格: ") + std::to_string(price) + std::string(" 新数量: ") + std::to_string(quantity));
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
            Logger::getInstance().log(LogLevel::INFO, std::string("仓库中没有产品。"));
        }
        else
        {
            Logger::getInstance().log(LogLevel::INFO, std::string("产品列表:"));
            for (const auto &product : products_)
            {
                Logger::getInstance().log(LogLevel::INFO, "ID: " + std::to_string(product.first) + " 名称: " + product.second->getName() + " 数量: " + std::to_string(product.second->getQuantity()));
            }
        }
    }

    void inventoryCheckThread(int checkInterval, int lowStockThreshold)
    {
        
        Logger::getInstance().log(LogLevel::INFO, std::string("开始库存检查..."));
        try
        {
            for (const auto &productPair : products_)
            {
                if (productPair.second->getQuantity() < lowStockThreshold)
                {
                    Logger::getInstance().log(LogLevel::WARNING, std::string("库存不足 - ID: ") + std::to_string(productPair.first) + std::string(" 名称: ") + productPair.second->getName() + std::string(" 数量: ") + std::to_string(productPair.second->getQuantity()));
                }else {
                    Logger::getInstance().log(LogLevel::INFO, std::string("库存正常 - ID: ") + std::to_string(productPair.first) + std::string(" 名称: ") + productPair.second->getName() + std::string(" 数量: ") + std::to_string(productPair.second->getQuantity()));
                }
            }
        }
        catch (const WarehouseException &e)
        {
            Logger::getInstance().log(LogLevel::ERROR, std::string("库存检查线程错误: ") + e.what());
        }
    }

private:
    std::unordered_map<size_t, std::unique_ptr<Product<T>>> products_;
    mutable std::mutex mutex_; // 用于保护产品数据的互斥锁
};

#endif
