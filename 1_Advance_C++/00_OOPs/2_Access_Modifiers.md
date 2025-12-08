## Access Modifiers

1. Public : Used Outside the class also
2. Private (members): Only used with in the class
3. Protected (members) : Can be Accessed by the class child


Example:-

```cpp

#include <iostream>
using namespace std;

class Strategy
{
    private:
        double userID; // Store Garbage Valueby default.

    public:
        double qty;
        double price;
        void printPriceQty()
        {
            cout<< "Quantity : " << qty <<endl;
            cout<< "Prince : " <<price <<endl;
        }
};

int main()
{
    Strategy obj;
    obj.qty = 100;
    obj.price = 550;

    obj.printPriceQty();

    return 0;
}


```


### Questions

1. When should we use class instead of struct and vice versa?
2. Why do we delete default constructor while designing any system?