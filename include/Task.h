#ifndef TASK_H
#define TASK_H

#include <string>
#include <chrono>

class Task
{
public:
    Task(const std::string &title);
    Task(const std::string &title, const std::string &description, bool completed = false);
    ~Task();

    // Getters and setters
    std::string getTitle() const;
    void setTitle(const std::string &title);

    std::string getDescription() const;
    void setDescription(const std::string &description);

    bool isCompleted() const;
    void setCompleted(bool completed);

    std::chrono::system_clock::time_point getCreationDate() const;
    std::chrono::system_clock::time_point getDueDate() const;
    void setDueDate(const std::chrono::system_clock::time_point &dueDate);

    // Task operations
    void toggleCompletion();

    // Serialization methods for saving/loading
    std::string toString() const;
    std::string toFileString() const;                   // New method for saving to file
    static Task fromFileString(const std::string &str); // New static method for loading from file

private:
    std::string title;
    std::string description;
    bool completed;
    std::chrono::system_clock::time_point creationDate;
    std::chrono::system_clock::time_point dueDate;
};

#endif // TASK_H