<<<<<<< HEAD
#include <iostream>

// #include "menu.cpp"
#include <string>
#include <fstream>
using namespace std;

class New
{
public:
    string task[10];
    int r = 1;
    int i = 0;
    int o = 0;
    char t;

    int write()
    {

        ofstream info_in;
        info_in.open("datacenter.txt");
        cout << "Write:-\n\n";
        do
        {

            cout << r << ". ";

            // std::cin >> task[i];
            cin.ignore();

            getline(cin, task[i]);

            info_in << task[i] << endl;

            cout << "\nWant to write another ? ";
            cout << "\nY or N:\n";
            std::cin >> t;

            r++;
            i++;
            o++;

        } while ((t == 'y') || (t == 'Y'));
        info_in.close();
    }
=======
#include <iostream>

// #include "menu.cpp"
#include <string>
#include <fstream>
using namespace std;

class New
{
public:
    string task[10];
    int r = 1;
    int i = 0;
    int o = 0;
    char t;

    int write()
    {

        ofstream info_in;
        info_in.open("datacenter.txt");
        cout << "Write:-\n\n";
        do
        {

            cout << r << ". ";

            // std::cin >> task[i];
            cin.ignore();

            getline(cin, task[i]);

            info_in << task[i] << endl;

            cout << "\nWant to write another ? ";
            cout << "\nY or N:\n";
            std::cin >> t;

            r++;
            i++;
            o++;

        } while ((t == 'y') || (t == 'Y'));
        info_in.close();
    }
>>>>>>> 48719e305426b5ba73b85a15ba736465372c200a
};