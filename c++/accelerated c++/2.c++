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
    const int rows = pad * 2 + 3;
    const string::size_type cols = greeting.size() + pad * 2 + 2;
    const string pad_spaces(pad, ' ');
    const string spaces(cols - 2, ' ');
    const string border(cols - 2, '*');

    // separate the output from the input
    cout << endl;

    // write rows rows of output
    // invariant: we have written r rows so far
    for (int r = 0; r != cols; ++r) {
        // write a row of output
        string::size_type c = 0;
 
        // invariant: we have written c characters so far in the given row
        while (c != cols) {
            if (r == (cols / 2) + 1 && c == 1){
                cout << pad_spaces + greeting + pad_spaces;
                c += greeting.size() + pad * 2;
            } else {
                if (c == 0 || c == cols - 1) {
                    cout << "*";
                    ++c;
                } else {
                    if (r == 0 || r == cols - 1){
                        cout << border;
                        c += border.size();
                    } else {
                        cout << spaces;
                        c += spaces.size();
                    }
                }
            }
        }
        cout << endl;
    }

    return 0;
}
