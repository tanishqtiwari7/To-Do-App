<<<<<<< HEAD
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Saves
{
public:
    int y = 1;
    std::string saves;
    int saved()
    {
        // cout<<" hii";
        ifstream info_get;
        info_get.open("datacenter.txt");
        if (info_get.peek()==EOF)
        {
            // std::cerr << "Error opening file for reading!" << std::endl;
            // std::cerr << "\t\tOR" << endl;
            std::cerr << "You does not have any saved todoss" << endl;
            return 1; // Exit with an error code
        }
        else
        {
            cout << "Your saved todoss are:\n";

            while (getline(info_get, saves))
            {
                std::cout << y << ". " << saves << std::endl;
                y++;
            }

            info_get.close(); // Close the file

            return 0;
        }
    }
=======
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Saves
{
public:
    int y = 1;
    std::string saves;
    int saved()
    {
        // cout<<" hii";
        ifstream info_get;
        info_get.open("datacenter.txt");
        if (info_get.peek()==EOF)
        {
            // std::cerr << "Error opening file for reading!" << std::endl;
            // std::cerr << "\t\tOR" << endl;
            std::cerr << "You does not have any saved todoss" << endl;
            return 1; // Exit with an error code
        }
        else
        {
            cout << "Your saved todoss are:\n";

            while (getline(info_get, saves))
            {
                std::cout << y << ". " << saves << std::endl;
                y++;
            }

            info_get.close(); // Close the file

            return 0;
        }
    }
>>>>>>> 48719e305426b5ba73b85a15ba736465372c200a
};