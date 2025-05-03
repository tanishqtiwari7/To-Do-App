#include "../include/SDL.h"  // Ensure SDL.h is included for SDL_main redirection
#include "../include/gui.h"
#include "../include/main.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <limits> // Required for numeric_limits

MainApp::MainApp() : gui(new GUI("Task Manager", SCREEN_WIDTH, SCREEN_HEIGHT)), // Use constants from gui.h
                     running(false),
                     currentState(AppState::WELCOME),
                     mouseX(0),
                     mouseY(0),
                     mousePressed(false),
                     mouseReleased(false)
{
    loadTasks();
}

MainApp::~MainApp()
{
    saveTasks();
    delete gui;
}

void MainApp::handleEvents()
{
    SDL_Event event;
    mousePressed = false;
    mouseReleased = false;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running = false;
            break;
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mousePressed = true;
                mouseX = event.button.x;
                mouseY = event.button.y;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseReleased = true;
                mouseX = event.button.x;
                mouseY = event.button.y;
            }
        }
        else if (event.type == SDL_KEYDOWN)
        {
            if (currentState == AppState::ADD_TASK || currentState == AppState::EDIT_TASK)
            {
                if (currentInput.titleActive)
                {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !currentInput.title.empty())
                    {
                        currentInput.title.pop_back();
                    }
                }
                else if (currentInput.descriptionActive)
                {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && !currentInput.description.empty())
                    {
                        currentInput.description.pop_back();
                    }
                }

                if (event.key.keysym.sym == SDLK_TAB)
                {
                    currentInput.titleActive = !currentInput.titleActive;
                    currentInput.descriptionActive = !currentInput.descriptionActive;
                }

                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    if (currentState == AppState::ADD_TASK && !currentInput.title.empty())
                    {
                        addTask(currentInput.title, currentInput.description);
                        currentState = AppState::VIEW_TASKS;
                        currentInput = TaskInput();
                    }
                    else if (currentState == AppState::EDIT_TASK && !currentInput.title.empty() &&
                             currentInput.selectedTaskIndex >= 0 &&
                             currentInput.selectedTaskIndex < static_cast<int>(tasks.size()))
                    {
                        editTask(currentInput.selectedTaskIndex, currentInput.title, currentInput.description);
                        tasks[currentInput.selectedTaskIndex].setCompleted(currentInput.completed);
                        currentState = AppState::VIEW_TASKS;
                        currentInput = TaskInput();
                    }
                }

                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    currentState = AppState::VIEW_TASKS;
                    currentInput = TaskInput();
                }
            }
            else // VIEW_TASKS state
            {
                if (event.key.keysym.sym == SDLK_n && (event.key.keysym.mod & KMOD_CTRL))
                {
                    currentState = AppState::ADD_TASK;
                    currentInput = TaskInput();
                    currentInput.titleActive = true;
                }
            }
        }
        else if (event.type == SDL_TEXTINPUT)
        {
            if (currentState == AppState::ADD_TASK || currentState == AppState::EDIT_TASK)
            {
                if (currentInput.titleActive)
                {
                    currentInput.title += event.text.text;
                }
                else if (currentInput.descriptionActive)
                {
                    currentInput.description += event.text.text;
                }
            }
        }
    }
}

void MainApp::update()
{
    // Logic that happens every frame
}

void MainApp::renderUI()
{
    gui->clear();

    // Use constants from gui.h
    int contentWidth = SCREEN_WIDTH - 2 * PADDING;
    int headerHeight = 60; // Adjusted header height
    int taskItemHeight = FONT_SIZE + PADDING; // From gui.cpp

    switch (currentState)
    {
    case AppState::WELCOME:
    {
        // Header panel (Light Blue-Green)
        gui->renderPanel(0, 0, SCREEN_WIDTH, headerHeight, COLOR_PANEL);

        // Render title text (centered now)
        gui->renderTextCentered("TASK MANAGER", 0, 0, SCREEN_WIDTH, headerHeight, COLOR_TEXT, FONT_SIZE, true);

        // Main welcome panel (Cream background, Panel border)
        int mainY = headerHeight + PADDING;
        int mainH = SCREEN_HEIGHT - mainY - PADDING;
        gui->renderPanel(PADDING, mainY, contentWidth, mainH, COLOR_BACKGROUND, false, BORDER_SIZE);

        // Welcome message
        int textY = mainY + PADDING * 2;
        gui->renderTextCentered("Welcome to Your Task Manager", PADDING, textY, contentWidth, 40, COLOR_TEXT, FONT_SIZE + 4, true);
        textY += 40;
        gui->renderTextCentered("Organize your tasks efficiently", PADDING, textY, contentWidth, 30, COLOR_TEXT);
        textY += 40;

        // Task stats if available
        if (!tasks.empty())
        {
            int completed = 0;
            for (const auto &task : tasks)
            {
                if (task.isCompleted())
                {
                    completed++;
                }
            }
            float progress = tasks.empty() ? 0.0f : static_cast<float>(completed) / tasks.size();
            int statsX = PADDING * 3;
            int statsW = contentWidth - PADDING * 4;
            gui->renderText("Current Status:", statsX, textY, COLOR_TEXT, FONT_SIZE, true);
            textY += FONT_SIZE + PADDING;
            gui->renderText(std::to_string(tasks.size()) + " Total Tasks", statsX, textY, COLOR_PRIMARY);
            gui->renderText(std::to_string(completed) + " Completed", statsX + statsW / 2, textY, COLOR_ACCENT);
            textY += FONT_SIZE + PADDING / 2;
            gui->renderProgressBar(statsX, textY, statsW, 15, progress);
            textY += 15 + PADDING * 2;
        }

        // Start button (Teal)
        int btnWidth = 180;
        int btnX = (SCREEN_WIDTH - btnWidth) / 2;
        int btnY = mainY + mainH - BUTTON_HEIGHT - PADDING * 2;

        bool startBtnHovered = gui->isMouseOver(btnX, btnY, btnWidth, BUTTON_HEIGHT, mouseX, mouseY);
        gui->renderButton("Get Started", btnX, btnY, btnWidth, BUTTON_HEIGHT, startBtnHovered);

        if (startBtnHovered && mouseReleased)
        {
            currentState = AppState::VIEW_TASKS;
        }

        break;
    }
    case AppState::VIEW_TASKS:
    {
        // Header panel (Light Blue-Green)
        gui->renderPanel(0, 0, SCREEN_WIDTH, headerHeight, COLOR_PANEL);

        // Render title text (aligned left now)
        gui->renderText("TASK MANAGER", PADDING, (headerHeight - FONT_CHAR_HEIGHT) / 2, COLOR_TEXT, FONT_SIZE, true);

        // Add Task button (Teal)
        int addBtnW = 150;
        int addBtnX = SCREEN_WIDTH - addBtnW - PADDING;
        int addBtnY = (headerHeight - BUTTON_HEIGHT) / 2;

        bool addBtnHovered = gui->isMouseOver(addBtnX, addBtnY, addBtnW, BUTTON_HEIGHT, mouseX, mouseY);
        gui->renderButton("+ Add Task", addBtnX, addBtnY, addBtnW, BUTTON_HEIGHT, addBtnHovered);

        if (addBtnHovered && mouseReleased)
        {
            currentState = AppState::ADD_TASK;
            currentInput = TaskInput();
            currentInput.titleActive = true;
        }

        // Main content area (Cream background)
        int mainY = headerHeight + PADDING;
        int mainH = SCREEN_HEIGHT - mainY - PADDING;
        // No explicit panel needed here, just use background color

        // Task list header and stats
        int listHeaderY = mainY;
        gui->renderText("Your Tasks", PADDING, listHeaderY + (PADDING / 2), COLOR_TEXT, FONT_SIZE, true);

        if (!tasks.empty())
        {
            int completed = 0;
            for (const auto &task : tasks)
            {
                if (task.isCompleted())
                {
                    completed++;
                }
            }
            float progress = tasks.empty() ? 0.0f : static_cast<float>(completed) / tasks.size();
            std::string statsText = std::to_string(completed) + "/" + std::to_string(tasks.size()) + " Done";
            int statsTextWidth = statsText.length() * FONT_CHAR_WIDTH;
            int statsX = SCREEN_WIDTH - PADDING - statsTextWidth - 120; // Position stats text
            int progressW = 100;
            int progressX = SCREEN_WIDTH - PADDING - progressW;

            gui->renderText(statsText, statsX, listHeaderY + (PADDING / 2), COLOR_PRIMARY);
            gui->renderProgressBar(progressX, listHeaderY + PADDING / 2, progressW, FONT_SIZE, progress);
        }

        // Render task list
        int listY = listHeaderY + FONT_SIZE + PADDING;
        int listH = mainH - (listY - mainY);
        int taskListWidth = contentWidth;
        int listAreaX = PADDING;

        if (tasks.empty())
        {
            int emptyY = listY + listH / 2 - 30;
            gui->renderTextCentered("No tasks yet.", listAreaX, emptyY, taskListWidth, 30, COLOR_DISABLED);
            gui->renderTextCentered("Click '+ Add Task' to start!", listAreaX, emptyY + 30, taskListWidth, 30, COLOR_DISABLED);
        }
        else
        {
            // Calculate max visible tasks based on smaller height
            int maxVisibleTasks = listH / (taskItemHeight + PADDING / 2); // Add small gap between items
            size_t numTasksToDisplay = std::min(tasks.size(), static_cast<size_t>(maxVisibleTasks));

            // Define button sizes once
            int btnWidth = 60;
            int btnHeight = taskItemHeight - PADDING / 2; // Make buttons fit nicely in the task item
            int btnGap = PADDING / 4;
            int actionButtonsTotalWidth = btnWidth * 2 + btnGap;
            int taskTextWidth = taskListWidth - actionButtonsTotalWidth - PADDING * 2; // Space for text

            for (size_t i = 0; i < numTasksToDisplay; ++i)
            {
                int taskY = listY + i * (taskItemHeight + PADDING / 2);

                // Render task item (adjust width for buttons)
                gui->renderTaskItem(tasks[i].getTitle(), tasks[i].isCompleted(), listAreaX, taskY, taskTextWidth);

                // Checkbox click detection (uses dimensions from renderTaskItem implicitly)
                int checkboxX = listAreaX + PADDING / 2;
                int checkboxSize = FONT_SIZE;
                int checkboxClickY = taskY + (taskItemHeight - checkboxSize) / 2;
                bool checkboxHovered = gui->isMouseOver(checkboxX, checkboxClickY, checkboxSize, checkboxSize, mouseX, mouseY);

                if (checkboxHovered && mouseReleased)
                {
                    toggleTaskCompletion(i);
                }

                // Edit and Delete buttons (place them to the right of the task item text area)
                int btnAreaX = listAreaX + taskTextWidth + PADDING / 2;
                int btnY = taskY + (taskItemHeight - btnHeight) / 2;

                // Edit button (Secondary - Light Blue)
                int editBtnX = btnAreaX;
                bool editBtnHovered = gui->isMouseOver(editBtnX, btnY, btnWidth, btnHeight, mouseX, mouseY);
                gui->renderSecondaryButton("Edit", editBtnX, btnY, btnWidth, btnHeight, editBtnHovered);

                if (editBtnHovered && mouseReleased)
                {
                    currentState = AppState::EDIT_TASK;
                    currentInput.title = tasks[i].getTitle();
                    currentInput.description = tasks[i].getDescription();
                    currentInput.completed = tasks[i].isCompleted();
                    currentInput.selectedTaskIndex = static_cast<int>(i);
                    currentInput.titleActive = true;
                }

                // Delete button (Danger - Red)
                int delBtnX = editBtnX + btnWidth + btnGap;
                bool delBtnHovered = gui->isMouseOver(delBtnX, btnY, btnWidth, btnHeight, mouseX, mouseY);
                gui->renderDangerButton("Delete", delBtnX, btnY, btnWidth, btnHeight, delBtnHovered);

                if (delBtnHovered && mouseReleased)
                {
                    deleteTask(i);
                    // Important: Break or adjust loop index if deleting to avoid accessing invalid memory
                    break;
                }
            }

            // Show 'more tasks' message if needed
            if (tasks.size() > numTasksToDisplay)
            {
                int moreY = listY + numTasksToDisplay * (taskItemHeight + PADDING / 2);
                gui->renderTextCentered("... " + std::to_string(tasks.size() - numTasksToDisplay) + " more",
                                        listAreaX, moreY, taskListWidth, 30, COLOR_DISABLED);
            }
        }
        break;
    }

    case AppState::ADD_TASK:
    case AppState::EDIT_TASK: // Combine Add and Edit rendering logic
    {
        bool isEditMode = (currentState == AppState::EDIT_TASK);
        std::string headerText = isEditMode ? "EDIT TASK" : "ADD NEW TASK";

        // Header panel (Light Blue-Green)
        gui->renderPanel(0, 0, SCREEN_WIDTH, headerHeight, COLOR_PANEL);

        // Render title text (aligned left now)
        gui->renderText(headerText, PADDING, (headerHeight - FONT_CHAR_HEIGHT) / 2, COLOR_TEXT, FONT_SIZE, true);

        // Main content panel (Cream background, Panel border)
        int mainY = headerHeight + PADDING;
        int mainH = SCREEN_HEIGHT - mainY - PADDING;
        gui->renderPanel(PADDING, mainY, contentWidth, mainH, COLOR_BACKGROUND, false, BORDER_SIZE);

        // Form layout
        int formX = PADDING * 2;
        int formWidth = contentWidth - PADDING * 2;
        int fieldY = mainY + PADDING * 2;

        // Title field
        gui->renderText("Task Title:", formX, fieldY, COLOR_TEXT, FONT_SIZE, true);
        fieldY += FONT_SIZE + PADDING / 2;
        int titleY = fieldY;
        bool titleFieldHovered = gui->isMouseOver(formX, titleY, formWidth, INPUT_HEIGHT, mouseX, mouseY);
        if (titleFieldHovered && mousePressed)
        {
            currentInput.titleActive = true;
            currentInput.descriptionActive = false;
        }
        gui->renderInputField(currentInput.title, formX, titleY, formWidth, INPUT_HEIGHT,
                              currentInput.titleActive, "Enter task title...");
        fieldY += INPUT_HEIGHT + PADDING;

        // Description field
        gui->renderText("Description (Optional):", formX, fieldY, COLOR_TEXT, FONT_SIZE, true);
        fieldY += FONT_SIZE + PADDING / 2;
        int descY = fieldY;
        int descHeight = INPUT_HEIGHT * 3; // Allow more space for description
        bool descFieldHovered = gui->isMouseOver(formX, descY, formWidth, descHeight, mouseX, mouseY);
        if (descFieldHovered && mousePressed)
        {
            currentInput.titleActive = false;
            currentInput.descriptionActive = true;
        }
        gui->renderInputField(currentInput.description, formX, descY, formWidth, descHeight,
                              currentInput.descriptionActive, "Enter details...");
        fieldY += descHeight + PADDING;

        // Completed checkbox (only in Edit mode)
        if (isEditMode)
        {
            int checkboxX = formX;
            int checkboxY = fieldY;
            int checkboxSize = FONT_SIZE;
            bool checkboxHovered = gui->isMouseOver(checkboxX, checkboxY, checkboxSize, checkboxSize, mouseX, mouseY);

            gui->renderCheckbox(checkboxX, checkboxY, checkboxSize, currentInput.completed, checkboxHovered);
            gui->renderText("Mark as completed", checkboxX + checkboxSize + PADDING / 2, checkboxY + (checkboxSize - FONT_CHAR_HEIGHT) / 2, COLOR_TEXT);

            if (checkboxHovered && mouseReleased)
            {
                currentInput.completed = !currentInput.completed;
            }
            fieldY += checkboxSize + PADDING * 2;
        }

        // Action buttons
        int btnY = mainY + mainH - BUTTON_HEIGHT - PADDING * 2;
        int btnWidth = 120;
        int cancelBtnX = formX + formWidth - btnWidth;
        int saveBtnX = cancelBtnX - btnWidth - PADDING;

        // Save button (Teal)
        bool saveBtnEnabled = !currentInput.title.empty();
        bool saveBtnHovered = saveBtnEnabled && gui->isMouseOver(saveBtnX, btnY, btnWidth, BUTTON_HEIGHT, mouseX, mouseY);
        gui->renderButton(isEditMode ? "Save Changes" : "Save Task", saveBtnX, btnY, btnWidth, BUTTON_HEIGHT, saveBtnHovered, saveBtnEnabled);

        if (saveBtnEnabled && saveBtnHovered && mouseReleased)
        {
            if (isEditMode)
            {
                if (currentInput.selectedTaskIndex >= 0 && currentInput.selectedTaskIndex < static_cast<int>(tasks.size()))
                {
                    editTask(currentInput.selectedTaskIndex, currentInput.title, currentInput.description);
                    tasks[currentInput.selectedTaskIndex].setCompleted(currentInput.completed);
                }
            }
            else
            {
                addTask(currentInput.title, currentInput.description);
            }
            currentState = AppState::VIEW_TASKS;
            currentInput = TaskInput();
        }

        // Cancel button (Secondary - Light Blue)
        bool cancelBtnHovered = gui->isMouseOver(cancelBtnX, btnY, btnWidth, BUTTON_HEIGHT, mouseX, mouseY);
        gui->renderSecondaryButton("Cancel", cancelBtnX, btnY, btnWidth, BUTTON_HEIGHT, cancelBtnHovered);

        if (cancelBtnHovered && mouseReleased)
        {
            currentState = AppState::VIEW_TASKS;
            currentInput = TaskInput();
        }

        // Keyboard shortcuts info
        gui->renderText("Tab: Switch | Enter: Save | Esc: Cancel",
                        formX, btnY - PADDING - FONT_SIZE, COLOR_DISABLED, FONT_SIZE - 2);

        break;
    }
    }

    gui->present();
}

// Task management methods
void MainApp::loadTasks()
{
    // Ensure dataCenter directory exists
    try
    {
        if (!std::filesystem::exists("dataCenter"))
        {
            std::filesystem::create_directories("dataCenter");
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
    }

    std::ifstream inFile("dataCenter/tasks.txt");
    if (!inFile)
    {
        // Don't treat this as an error, just means no tasks saved yet.
        // std::cerr << "Warning: Could not open tasks.txt for loading." << std::endl;
        return;
    }

    tasks.clear();
    std::string line;

    while (std::getline(inFile, line))
    {
        if (!line.empty())
        {
            try
            {
                tasks.push_back(Task::fromFileString(line));
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error parsing task line: " << line << " - " << e.what() << std::endl;
            }
        }
    }

    inFile.close();
}

void MainApp::saveTasks()
{
    // Ensure dataCenter directory exists
    try
    {
        if (!std::filesystem::exists("dataCenter"))
        {
            std::filesystem::create_directories("dataCenter");
        }
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error creating directory before saving: " << e.what() << std::endl;
        // Optionally, decide if you want to proceed without saving or return
    }

    std::ofstream outFile("dataCenter/tasks.txt");
    if (!outFile)
    {
        std::cerr << "Error: Could not open tasks.txt for saving." << std::endl;
        return;
    }

    for (const auto &task : tasks)
    {
        try
        {
            outFile << task.toFileString() << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error writing task to file: " << e.what() << std::endl;
        }
    }

    outFile.close();
}

void MainApp::addTask(const std::string &title, const std::string &description)
{
    if (!title.empty())
    { // Basic validation
        tasks.emplace_back(title, description);
        saveTasks();
    }
}

void MainApp::deleteTask(size_t index)
{
    if (index < tasks.size())
    {
        tasks.erase(tasks.begin() + index);
        saveTasks();
    }
}

void MainApp::toggleTaskCompletion(size_t index)
{
    if (index < tasks.size())
    {
        tasks[index].toggleCompletion();
        saveTasks();
    }
}

void MainApp::editTask(size_t index, const std::string &newTitle, const std::string &newDescription)
{
    if (index < tasks.size() && !newTitle.empty()) // Basic validation
    {
        tasks[index].setTitle(newTitle);
        tasks[index].setDescription(newDescription);
        // Completion status is handled separately via currentInput.completed
        saveTasks();
    }
}

int MainApp::run()
{
    if (!gui->initialize())
    {
        std::cerr << "Failed to initialize the GUI!" << std::endl;
        return 1;
    }

    SDL_StartTextInput();
    running = true;

    Uint32 frameStart;
    int frameTime;
    const int FRAME_DELAY = 16; // Target ~60 FPS

    while (running)
    {
        frameStart = SDL_GetTicks();

        handleEvents();
        update();
        renderUI();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime)
        {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_StopTextInput();
    // Save tasks one last time on exit
    saveTasks();
    return 0;
}

// Keep SDL_main redirection
#ifdef main
#undef main
#endif

int main(int argc, char *argv[])
{
    // Directory creation is handled in load/save now
    MainApp app;
    return app.run();
}