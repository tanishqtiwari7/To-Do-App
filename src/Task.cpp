#include "../include/Task.h"
#include <sstream>
#include <iomanip>
#include <vector>

Task::Task(const std::string &title)
    : title(title), description(""), completed(false),
      creationDate(std::chrono::system_clock::now())
{
}

Task::Task(const std::string &title, const std::string &description, bool completed)
    : title(title), description(description), completed(completed),
      creationDate(std::chrono::system_clock::now())
{
}

Task::~Task()
{
}

std::string Task::getTitle() const
{
    return title;
}

void Task::setTitle(const std::string &title)
{
    this->title = title;
}

std::string Task::getDescription() const
{
    return description;
}

void Task::setDescription(const std::string &description)
{
    this->description = description;
}

bool Task::isCompleted() const
{
    return completed;
}

void Task::setCompleted(bool completed)
{
    this->completed = completed;
}

std::chrono::system_clock::time_point Task::getCreationDate() const
{
    return creationDate;
}

std::chrono::system_clock::time_point Task::getDueDate() const
{
    return dueDate;
}

void Task::setDueDate(const std::chrono::system_clock::time_point &dueDate)
{
    this->dueDate = dueDate;
}

void Task::toggleCompletion()
{
    completed = !completed;
}

// For display purposes
std::string Task::toString() const
{
    std::stringstream ss;
    ss << "[" << (completed ? "X" : " ") << "] " << title;

    if (!description.empty())
    {
        ss << "\n    " << description;
    }

    // Format creation date
    auto creationTime = std::chrono::system_clock::to_time_t(creationDate);
    std::tm tmCreation = *std::localtime(&creationTime);

    ss << "\n    Created: " << std::put_time(&tmCreation, "%Y-%m-%d %H:%M:%S");

    // If due date is set, add it to the string
    if (dueDate != std::chrono::system_clock::time_point{})
    {
        auto dueTime = std::chrono::system_clock::to_time_t(dueDate);
        std::tm tmDue = *std::localtime(&dueTime);
        ss << "\n    Due: " << std::put_time(&tmDue, "%Y-%m-%d %H:%M:%S");
    }

    return ss.str();
}

// For saving to file format
std::string Task::toFileString() const
{
    std::stringstream ss;

    // Format: title|description|completed|creationDate|dueDate
    ss << title << "|";
    ss << description << "|";
    ss << (completed ? "1" : "0") << "|";

    // Format dates as timestamps for reliable storage/retrieval
    auto creationTimestamp = std::chrono::system_clock::to_time_t(creationDate);
    ss << creationTimestamp << "|";

    // Only add dueDate if it is set
    if (dueDate != std::chrono::system_clock::time_point{})
    {
        auto dueTimestamp = std::chrono::system_clock::to_time_t(dueDate);
        ss << dueTimestamp;
    }

    return ss.str();
}

// Static method to create a Task from a file string
Task Task::fromFileString(const std::string &str)
{
    std::stringstream ss(str);
    std::string titleStr, descStr, completedStr, creationStr, dueStr;

    // Parse the pipe-delimited string
    std::getline(ss, titleStr, '|');
    std::getline(ss, descStr, '|');
    std::getline(ss, completedStr, '|');
    std::getline(ss, creationStr, '|');
    std::getline(ss, dueStr, '|');

    // Create task with basic info
    Task task(titleStr, descStr, completedStr == "1");

    // Set creation date if available
    if (!creationStr.empty())
    {
        try
        {
            std::time_t creationTime = std::stoll(creationStr);
            task.creationDate = std::chrono::system_clock::from_time_t(creationTime);
        }
        catch (const std::exception &e)
        {
            // Default to current time if parsing fails
        }
    }

    // Set due date if available
    if (!dueStr.empty())
    {
        try
        {
            std::time_t dueTime = std::stoll(dueStr);
            task.setDueDate(std::chrono::system_clock::from_time_t(dueTime));
        }
        catch (const std::exception &e)
        {
            // Leave due date unset if parsing fails
        }
    }

    return task;
}