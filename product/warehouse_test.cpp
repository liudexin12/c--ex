#include "product/warehouse.h"
#include "test/test.h"

// 测试用例
bool testAddProduct() {
    Warehouse<double> warehouse;
    warehouse.addProduct(std::make_unique<Product<double>>(1, "Product A", 99.99, 100));
    ASSERT_EQUAL("Product A", warehouse.getProduct(1)->getName());
    ASSERT_EQUAL(100, warehouse.getProduct(1)->getQuantity());
    ASSERT_EQUAL(double(99.99),warehouse.getProduct(1)->getPrice());
    try
    {
        warehouse.addProduct(std::make_unique<Product<double>>(1, "Product A", 99.99, 100));
    }
    catch(const std::exception& e)
    {
        std::cout<<"测试用例: 添加产品失败"<<std::endl;
        ASSERT_EQUAL(std::string("产品ID已存在: ") + std::to_string(1),e.what());
    }
    
    return true;
}

bool testUpdateQuantity() {
    Warehouse<double> warehouse;
    warehouse.addProduct(std::make_unique<Product<double>>(1, "Product A", 99.99, 100));
    ASSERT_EQUAL("Product A", warehouse.getProduct(1)->getName());
    ASSERT_EQUAL(100, warehouse.getProduct(1)->getQuantity());
    ASSERT_EQUAL(double(99.99),warehouse.getProduct(1)->getPrice());

    warehouse.updateProduct(1,"Product A", 9.99, 0 );
    ASSERT_EQUAL("Product A", warehouse.getProduct(1)->getName());
    ASSERT_EQUAL(0, warehouse.getProduct(1)->getQuantity());
    ASSERT_EQUAL(double(9.99),warehouse.getProduct(1)->getPrice());

    try
    {
        warehouse.updateProduct(2, "Product A", 99.99, 100);
    }
    catch(const std::exception& e)
    {
        std::cout<<"测试用例: 更新产品失败"<<std::endl;
        ASSERT_EQUAL(std::string("产品ID不存在: ") + std::to_string(2),e.what());
    }
    return true;
}
