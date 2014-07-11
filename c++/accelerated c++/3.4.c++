#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cout;	using std::cin;
using std::vector;	using std::string;
using std::endl;	using std::max;

int main()
{
    vector<string> vec;
    string word;
    cout << "Enter a list of words: ";
    while (cin >> word) {
        vec.push_back(word);
    }
    typedef vector<string>::size_type vec_size;
    vec_size size = vec.size();
    string longest;
    for (int n = 0; n != size; ++n) {
        if (n == 0 || max(vec[n].size(), longest.size()) == vec[n].size()) {
            longest = vec[n];
        }
    }
    cout << longest << endl;
}
