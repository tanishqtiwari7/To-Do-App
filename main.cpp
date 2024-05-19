<<<<<<< HEAD

#include <iostream>
#include "menu.cpp"
#include "new_todo.cpp"
#include "saved_todo.cpp"

using namespace std;

class MAIN
{
public:
    char input;

    void intro()
    {
        std::cout << "\t\t\t********************welcome********************\n";
        cout << "\t\t\t*************To the ToDo application***********\n";
    }

    void home()
    {
        Menu obj;
        obj.menu();

        do
        {
            std::cout << "Choose the letter: ";
            std::cin >> input;

            if (input != 'a' && input != 'b' && input != 'c' &&
                input != 'A' && input != 'B' && input != 'C')
            {
                std::cout << "Enter the correct letter!\n";
            }
        } while (input != 'a' && input != 'b' && input != 'c' &&
                 input != 'A' && input != 'B' && input != 'C');

        if (input == 'a' || input == 'A')
        {
            Saves obj;
            obj.saved();
        }
        else if (input == 'b' || input == 'B')
        {
            // Handle 'b' case (if needed)
        }
        else
        {
            New obj;
            obj.write();
        }
    }
};

int main()
{
    char k;
    int l;
    int o;
    MAIN obj;
    obj.intro();

    do
    {
        obj.home();
        cout << "Want to go back to the menu? (Y/N): ";
        cin >> k;
    } while (k == 'y' || k == 'Y');

    return 0;
}
=======

#include <iostream>
#include "menu.cpp"
#include "new_todo.cpp"
#include "saved_todo.cpp"

using namespace std;

class MAIN
{
public:
    char input;

    void intro()
    {
        std::cout << "\t\t\t********************welcome********************\n";
        cout << "\t\t\t*************To the ToDo application***********\n";
    }

    void home()
    {
        Menu obj;
        obj.menu();

        do
        {
            std::cout << "Choose the letter: ";
            std::cin >> input;

            if (input != 'a' && input != 'b' && input != 'c' &&
                input != 'A' && input != 'B' && input != 'C')
            {
                std::cout << "Enter the correct letter!\n";
            }
        } while (input != 'a' && input != 'b' && input != 'c' &&
                 input != 'A' && input != 'B' && input != 'C');

        if (input == 'a' || input == 'A')
        {
            Saves obj;
            obj.saved();
        }
        else if (input == 'b' || input == 'B')
        {
            // Handle 'b' case (if needed)
        }
        else
        {
            New obj;
            obj.write();
        }
    }
};

int main()
{
    char k;
    MAIN obj;
    obj.intro();

    do
    {
        obj.home();
        cout << "Want to go back to the menu? (Y/N): ";
        cin >> k;
    } while (k == 'y' || k == 'Y');

    return 0;
}
>>>>>>> 48719e305426b5ba73b85a15ba736465372c200a
