#include <product.h>

// Example usage
int main() {
    Product<double> product1(1, "Laptop", 999.99, 10);
    std::cout << "Product ID: " << product1.getID() << "\n";
    std::cout << "Product Name: " << product1.getName() << "\n";
    std::cout << "Product Price: " << product1.getPrice() << "\n";
    std::cout << "Product Quantity: " << product1.getQuantityInStock() << "\n";

    product1.addStock(5);
    std::cout << "Updated Quantity: " << product1.getQuantityInStock() << "\n";

    if (product1.removeStock(3)) {
        std::cout << "Stock removed. New Quantity: " << product1.getQuantityInStock() << "\n";
    } else {
        std::cout << "Not enough stock to remove.\n";
    }

    return 0;
}