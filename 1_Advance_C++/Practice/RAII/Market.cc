#include <iostream>

class MarketOrder 
{
    private:
        double price;
        int id;

    public:
    // Constructor: resource acquired
        MarketOrder(double p, int id) : price(p), id(id) 
        {
            std::cout << "[Constructed] Order " << id << " at price: " << price << "\n";
        }

        void execute() 
        {
            std::cout << "[Executing] Order " << id << " new price: "<< price*2 << "\n";
        }

    // Destructor: resource released
        ~MarketOrder() 
        {
            std::cout << "[Destroyed]   Order " << id << " cleanup done.\n";
        }
};


/*

With RAII (Stack Allocation): If an exception is thrown, C++ performs Stack Unwinding. 
It works backwards through the scope, calling destructors for every object it created. 
The cleanup is guaranteed.

*/
int main() {

    std::cout << "--- Main Start ---\n";

    { // THIS IS A SCOPE BLOCK.

        // This is the "Hot Path" simulation.
        // The variable 'order' ONLY exists inside these brackets.
        MarketOrder order(100, 1);
        order.execute();
        
        std::cout << "--- End of Scope Block ---\n";


    } // 'order' dies HERE automatically.

    std::cout << "--- Main End ---\n";
    return 0;
}