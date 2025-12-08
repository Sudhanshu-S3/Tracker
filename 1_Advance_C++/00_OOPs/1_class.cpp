#include <iostream>

using namespace std;

class Strategy
{
    public:
        int order_qty;
        void PrintOrderQty()
        {
            cout << "Order Qty: \t" << order_qty <<'\n';
        }
};

int main()
{
    // Automatic Storage Allocation : Auto Delete after getting out of the function scope

    Strategy obj;
    obj.order_qty = 101;
    obj.PrintOrderQty();

    // Dynamic Storage Allocation
    Strategy* obj1 = new Strategy();
    obj1->order_qty = 1;
    obj1->PrintOrderQty() ;
    delete obj1;

    //New return Pointer to the object and to access the member of the object we have to use the ->

    return 0;
}