#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <vector>
#include <memory> // For smart pointers
#include "gui.h" // Updated to use our new GUI implementation
#include "Task.h"

// Enum to manage different application views/states
enum class AppState
{
    WELCOME, // New state for the welcome screen
    VIEW_TASKS,
    ADD_TASK,
    EDIT_TASK
};

// Structure to hold user input for task creation/editing
struct TaskInput
{
    std::string title;
    std::string description;
    bool completed = false;
    int selectedTaskIndex = -1; // Used for editing
    bool titleActive = false;
    bool descriptionActive = false;
};

/**
 * @brief Main application controller class
 */
class MainApp
{
public:
    /**
     * @brief Constructor for MainApp
     */
    MainApp();

    /**
     * @brief Destructor for MainApp
     */
    ~MainApp();

    /**
     * @brief Main application flow
     * @return 0 on success
     */
    int run();

private:
    void handleEvents();
    void update();
    void renderUI();

    // Task management methods
    void loadTasks();
    void saveTasks();
    void addTask(const std::string &title, const std::string &description = "");
    void deleteTask(size_t index);
    void toggleTaskCompletion(size_t index);
    void editTask(size_t index, const std::string &newTitle, const std::string &newDescription);

    // State variables
    GUI *gui; // Updated to use our new GUI implementation
    bool running;
    AppState currentState;
    std::vector<Task> tasks;
    TaskInput currentInput;
    SDL_Texture* logoTexture; // Added for the logo

    // UI related state
    int mouseX, mouseY;
    bool mousePressed;
    bool mouseReleased;
};

#endif // MAIN_H