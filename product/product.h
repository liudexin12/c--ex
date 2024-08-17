#include <iostream>
#include <string>
#include <atomic>
#include <utility>

namespace prodsale
{
    template <typename T>
    class Product
    {
    public:
        // Constructors
        Product(size_t id, const std::string &name, T price, int quantity)
            : ID(id), Name(name), Price(price), QuantityInStock(quantity) {}

        Product(size_t id, std::string &&name, T price, int quantity)
            : ID(id), Name(std::move(name)), Price(price), QuantityInStock(quantity) {}

        // Move constructor
        Product(Product &&other) noexcept
            : ID(other.ID), Name(std::move(other.Name)), Price(other.Price), QuantityInStock(other.QuantityInStock.load()) {}

        // Move assignment operator
        Product &operator=(Product &&other) noexcept
        {
            if (this != &other)
            {
                ID = other.ID;
                Name = std::move(other.Name);
                Price = other.Price;
                QuantityInStock.store(other.QuantityInStock.load());
            }
            return *this;
        }

        // Destructor
        ~Product() = default;

        // Getters
        size_t getID() const { return ID; }
        const std::string &getName() const { return Name; }
        T getPrice() const { return Price; }
        int getQuantityInStock() const { return QuantityInStock.load(); }

        // Setters
        void setName(const std::string &name) { Name = name; }
        void setName(std::string &&name) { Name = std::move(name); }
        void setPrice(T price) { Price = price; }
        void setQuantityInStock(int quantity) { QuantityInStock.store(quantity); }

        // Other member functions
        void addStock(int amount) { QuantityInStock.fetch_add(amount); }
        bool removeStock(int amount)
        {
            int current = QuantityInStock.load();
            if (current >= amount)
            {
                QuantityInStock.fetch_sub(amount);
                return true;
            }
            return false;
        }

    private:
        size_t ID;
        std::string Name;
        T Price;
        std::atomic<int> QuantityInStock;
    };

}
