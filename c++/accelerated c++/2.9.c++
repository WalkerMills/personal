#include <iostream>
#include <string>
using std::cout;	using std::cin;
using std::endl;	using std::string;

int main()
{
    int num_1;
    int num_2;
    cout << "Enter a number: ";
    cin >> num_1;
    cout << "Enter another number: ";
    cin >> num_2;
    if (num_1 > num_2) {
        cout << num_1 << " is greater than " << num_2 << endl;
    } else {
        if (num_2 > num_1) {
            cout << num_2 << " is greater than " << num_1 << endl;
        } else {
            cout << num_1 << " and " << num_2 << " are equal" << endl;
        }
    }
}
