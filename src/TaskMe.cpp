#include "TaskMe.h"

TaskMe::TaskMe(uint8_t capacity) : _capacity(capacity), _count(0)
{
    _tasks = new TaskRef[_capacity];
    for (uint8_t index = 0; index < _capacity; index++)
    {
        _tasks[index] = nullptr;
    }
}

uint8_t TaskMe::Capacity() const
{
    return _capacity;
}

uint8_t TaskMe::Count() const
{
    return _count;
}

uint8_t TaskMe::AddDelay(int time, Action callback)
{
    return addTask(time, nullptr, nullptr, 1, callback);
}

uint8_t TaskMe::AddSingle(int time, Action action)
{
    return addTask(time, action, nullptr, 1, nullptr);
}

uint8_t TaskMe::AddSingle(int time, Action action, Action callback)
{
    return addTask(time, action, nullptr, 1, callback);
}

uint8_t TaskMe::AddRepetitive(int time, Action action, unsigned int repetitions)
{
    return addTask(time, action, nullptr, repetitions, nullptr);
}

uint8_t TaskMe::AddRepetitive(int time, Action action, unsigned int repetitions, Action callback)
{
    return addTask(time, action, nullptr, repetitions, callback);
}

uint8_t TaskMe::AddContinuous(int time, Action action)
{
    return addTask(time, action, nullptr, 0, nullptr);
}

uint8_t TaskMe::AddContinuous(int time, Action action, Action callback)
{
    return addTask(time, action, nullptr, 0, callback);
}

uint8_t TaskMe::AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions)
{
    return addTask(time, action, alternateAction, repetitions, nullptr);
}

uint8_t TaskMe::AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback)
{
    return addTask(time, action, alternateAction, repetitions, callback);
}

uint8_t TaskMe::AddAlternantContinuous(int time, Action action, Action alternateAction)
{
    return addTask(time, action, alternateAction, 0, nullptr);
}

uint8_t TaskMe::AddAlternantContinuous(int time, Action action, Action alternateAction, Action callback)
{
    return addTask(time, action, alternateAction, 0, callback);
}

void TaskMe::Update()
{
    for (uint8_t index = 0; index < _capacity; index++)
    {
        if (_tasks[index] != nullptr)
        {
            bool finished = updateTask(_tasks[index]);

            if (finished)
            {
                if (_tasks[index]->callback != nullptr) 
                    _tasks[index]->callback();
                
                removeTask(index);
            }
        }
    }
}

void TaskMe::CancelTask(uint8_t index)
{
    removeTask(index);
}

bool TaskMe::IsFull() const
{
    return _count >= _capacity;
}

bool TaskMe::IsEmpty() const
{
    return _count == 0;
}

uint8_t TaskMe::addTask(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback)
{
    if (IsFull()) return _count;

    for (uint8_t index = 0; index < _capacity; index++)
    {
        if (_tasks[index] == nullptr)
        {
            _tasks[index] = new Task();
            _tasks[index]->time = time;
            _tasks[index]->trigger = millis() + time;
            _tasks[index]->action = action;
            _tasks[index]->alternateAction = alternateAction;
            _tasks[index]->repetitions = repetitions;
            _tasks[index]->callback = callback;
            _count++;
            return index;
        }
    }
    return _count;
}

void TaskMe::rearmTask(TaskRef task, Action action, Action alternateAction, unsigned int repetitions)
{
    task->trigger = millis() + task->time;
    task->action = action;
    task->alternateAction = alternateAction;
    task->repetitions = repetitions;
}

void TaskMe::removeTask(uint8_t index)
{
    if (index >= _capacity) return;

    delete _tasks[index];
    _tasks[index] = nullptr;
    _count--;
}

bool TaskMe::updateTask(TaskRef task) const
{
    unsigned long currentMillis = millis();
    if (currentMillis >= task->trigger)
    {
        return processTask(task);
    }
    return false;
}

bool TaskMe::processTask(TaskRef task) const
{
    if (task->action != nullptr) task->action();

    if (task->repetitions == 1) return true;

    if (task->repetitions > 1)
    {
        rearmTask(task, task->alternateAction == nullptr ? task->action : task->alternateAction, task->alternateAction == nullptr ? nullptr : task->action, task->repetitions - 1);
    }
    else if (task->repetitions == 0)
    {
        rearmTask(task, task->alternateAction == nullptr ? task->action : task->alternateAction, task->alternateAction == nullptr ? nullptr : task->action, 0);
    }

    return false;
}