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

    vector<string> name_vec;
    vector< vector<double> > test_vec;
    vector<double> median_vec;
    int students = 2;
    int assignments = 3;

    for (int n = 0; n != students; ++n) {

        // ask for and read the student's name
        cout << "Name: ";
        string name;
        cin >> name;
        name_vec.push_back(name);

        // ask for and read the midterm and final grades
        cout << name_vec[n] << ", enter your midterm and final exam grades: ";
        double midterm, final;
        cin >> midterm >> final;
        vector<double> test;
        test.push_back(midterm);
        test.push_back(final);
        test_vec.push_back(test);

        // ask for the homework grades
        cout << name_vec[n] << ", enter all your homework grades:\n";

        // a vatiable into which to read
        vector<double> homework;
        for (int n = 0; n != assignments; ++n) {
            double x;
            cout << "Assignment " << n + 1 << ": ";
            cin >> x;
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
        median = size % 2 == 0 ? (homework[mid] - homework[mid - 1]) / 2
                               : homework[mid];
        median_vec.push_back(median);   
    }
     
    // write the result
    streamsize prec = cout.precision();
    for (int n = 0; n != name_vec.size(); ++n) {
        cout << name_vec[n] << ", your final grade is " << setprecision(3)
             << 0.2 * test_vec[n][0] + 0.4 * test_vec[n][1] + 0.4 * median_vec[n]
             << setprecision(prec) << endl;
    }
}
