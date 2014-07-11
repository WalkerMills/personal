#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cout;	using std::cin;
using std::vector;	using std::string;
using std::endl;	using std::max;

int main()
{
    vector<int> count_vec;
    vector<string> string_vec;
    string word;
    cout << "Enter a list of words: ";
    while (cin >> word) {
        string_vec.push_back(word);
        count_vec.push_back(0);
    }
    typedef vector<string>::size_type vec_size;
    vec_size size = string_vec.size();
    for (int n = 0; n != size; ++n) {
        string string = string_vec[n];
        for (int i = 0; i != size; ++i) {
            if (string == string_vec[i]) {
                ++count_vec[n];
            }
        }
    }
    int num;
    int index;
    for (int n = 0; n != size; ++n) {
        if (n == 0 || max(count_vec[n], num) == count_vec[n]) {
            num = count_vec[n];
            index = n;
        }
    }
    cout << string_vec[index] << " was repeated " << num << " times" << endl;
}
