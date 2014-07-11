#include <iostream>
#include <string>

using std::cout;	using std::cin;
using std::string;	using std::endl;

int main()
{

    // ask for the person's name
    cout << "Please enter your first name: ";

    // read the name
    string name;
    cin >> name;

    //build the message we intend to write
    const string greeting = "Hello, " + name + "!";

    // the number of blanks surrounding the greeting
    cout << "Number of padded spaces: ";
    int pad;
    cin >> pad;

    // the total of number of rows to write
    const int rows = greeting.size() + pad * 2 + 1;
    const string::size_type cols = greeting.size() + pad * 2 + 2;

    // separate the output from the input
    cout << endl;

    // write rows rows of output
    // invariant: we have written r rows so far
    for (int r = 0; r != rows; ++r) {
        // write a row of output
        string::size_type c = 0;
        if (r == 0) {
            const string start_spaces((2 * rows - 1) / 2, ' ');
            cout << start_spaces + "*" << endl;
        } else {
            const int len(2 * r - 1);
            const string spaces(len, ' ');
            if (r == rows - 1) {
                const string border(len, '*');
                cout << "*" + border + "*" << endl;
            } else {
                const string start_spaces((2 * rows - 1) / 2 - (len + 2) / 2, ' ');
                if (r == rows / 2 + 1) {
                    const string pad_spaces((len - greeting.size()) / 2, ' ');
                    cout << start_spaces + "*" + pad_spaces + greeting + pad_spaces + "*" << endl;
                } else {
                    cout << start_spaces + "*" + spaces + "*" << endl;
                }
            }
        }
        cout << endl;
    }

    return 0;
}
