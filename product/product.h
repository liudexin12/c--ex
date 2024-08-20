#ifndef PRODECT_H
#define PRODECT_H

#include <iostream>
#include <string>
#include <atomic>
#include <utility>
#include "log/logg.h"

template <typename T>
class Product
{
public:
    // 构造函数
    Product(size_t id, const std::string &name, T price, int quantity)
        : id_(id), name_(name), price_(price), quantity_(quantity)
    {
    }

    // 移动构造函数
    Product(Product &&other) noexcept
        : id_(other.id_), name_(std::move(other.name_)), price_(std::move(other.price_)), quantity_(other.quantity_.load())
    {
        other.quantity_ = 0;
    }

    //拷贝构造函数
    Product(const Product& other)
        : id_(other.id_), name_(std::move(other.name_)), price_(other.price_), quantity_(other.quantity_.load())
    {
    }

    // 移动赋值运算符
    Product &operator=(Product &&other) noexcept
    {
        if (this != &other)
        {
            id_ = other.id_;
            name_ = std::move(other.name_);
            price_ = std::move(other.price_);
            quantity_ = other.quantity_.load();
            other.quantity_ = 0;
        }
        return *this;
    }

    // 禁用拷贝构造函数和拷贝赋值运算符
    // Product(const Product &) = delete;
    // Product &operator=(const Product &) = delete;

    // 析构函数
    ~Product() = default;

    // 获取产品ID
    size_t getId() const { return id_; }

    // 获取产品名称
    std::string getName() const { return name_; }

    // 获取产品价格
    T getPrice() const { return price_; }

    // 设置产品价格
    void setPrice(T price) { price_ = price; }

    // 获取库存数量
    int getQuantity() const { return quantity_.load(); }

    // 设置库存数量
    void setQuantity(int quantity) { quantity_ = quantity; }

    // 增加库存数量
    void increaseQuantity(int amount) { quantity_ += amount; }

    // 减少库存数量
    void decreaseQuantity(int amount) { quantity_ -= amount; }

private:
    size_t id_;                 // 产品ID
    std::string name_;          // 产品名称
    T price_;                   // 产品价格
    std::atomic<int> quantity_; // 库存数量
};

#endif
