#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <vector>

using std::cin;		using std::setprecision;
using std::cout;	using std::string;
using std::endl;	using std::streamsize;
using std::sort;	using std::vector;

int main()
{

    // ask for and read the student's name
    cout << "Name: ";
    string name;
    cin >> name;
    // ask for and read the midterm and final grades
    cout << "Enter your midterm and final exam grades: ";
    double midterm, final;
    cin >> midterm >> final;

    // ask for the homework grades
    cout << "Enter all your homework grades, "
            "followed by end-of-file: ";

    // a variable into which to read
    double x;
    vector<double> homework;
    vector<double> upper;
    vector<double> lower;

    // invariant: homework contains all the homework grades read so far
    while (cin >> x) {
        homework.push_back(x);
    }

    typedef vector<double>::size_type vec_sz;
    vec_sz size = homework.size();

    if (size == 0) {
        cout << endl << "You must enter your grades. "
                        "Please try again" << endl;
        return 1;
    }
    sort(homework.begin(), homework.end());
    vec_sz mid = size / 2;
    double median;
    if (size % 2 == 0) {
        median = (homework[mid] + homework[mid - 1]) / 2;
        // invariant: lower and upper each contain n elements
        for (int n = 0; n != mid; ++n) {
            lower.push_back(homework[n]);
            upper.push_back(homework[n + mid]);
        }
    } else {
        median = homework[mid];
        // invariant: lower and upper each contain n elements
        for (int n = 0; n != mid; ++n) {
            lower.push_back(homework[n]);
            upper.push_back(homework[n + mid + 1]);
        }
    }
    vec_sz upper_size = upper.size();
    vec_sz lower_size = lower.size();
    vec_sz upper_mid = upper_size / 2;
    vec_sz lower_mid = lower_size / 2;
    double upper_q, lower_q;
    upper_q = upper_size % 2 == 0 ? (upper[upper_mid] - upper[upper_mid - 1]) / 2
                                  : upper[upper_mid];
    lower_q = lower_size % 2 == 0 ? (lower[lower_mid] - lower[lower_mid - 1]) / 2
                                  : lower[lower_mid];
   
 
    // write the result
    streamsize prec = cout.precision();
    cout << "Your final grade is " << setprecision(3)
         << 0.2 * midterm + 0.4 * final + 0.4 * median
         << "\nUpper quartile: " << upper_q
         << "\nMedian: " << median
         << "\nLower quartile: " << lower_q
         << setprecision(prec) << endl;
}
