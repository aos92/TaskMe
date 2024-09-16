#ifndef _TASKME_h
#define _TASKME_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

typedef void(*Action)();

typedef struct 
{
    unsigned long time;
    unsigned long trigger;
    unsigned int repetitions;
    Action action;
    Action alternateAction;
    Action callback;
} Task, *TaskRef;

class TaskMe
{
public:
    TaskMe(const uint8_t capacity);

    uint8_t Capacity() const;
    uint8_t Count() const;

    uint8_t AddDelay(int time, Action callback);
    uint8_t AddSingle(int time, Action action);
    uint8_t AddSingle(int time, Action action, Action callback);
    
    uint8_t AddRepetitive(int time, Action action, unsigned int repetitions);
    uint8_t AddRepetitive(int time, Action action, unsigned int repetitions, Action callback);
    uint8_t AddContinuous(int time, Action action);
    uint8_t AddContinuous(int time, Action action, Action callback);
    
    uint8_t AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions);
    uint8_t AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback);
    uint8_t AddAlternantContinuous(int time, Action action, Action alternateAction);
    uint8_t AddAlternantContinuous(int time, Action action, Action alternateAction, Action callback);

    void Update();
    void CancelTask(uint8_t index);

    bool IsFull() const;
    bool IsEmpty() const;

private:
    TaskRef* _tasks;
    uint8_t _capacity;
    uint8_t _count;

    uint8_t addTask(int time, Action action, Action alternateAction, unsigned int repetitions, Action callback);
    static void rearmTask(TaskRef task, Action action, Action alternateAction, unsigned int repetitions);
    bool updateTask(TaskRef task) const;
    void removeTask(uint8_t index);
    bool processTask(TaskRef task) const;
};

#endif
