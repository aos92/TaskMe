# taskMe 

**taskMe** is an Arduino library designed to simplify the execution of multiple non-blocking tasks, providing asynchronous behavior for projects that require scheduled task management. With **taskMe**, you can instantiate objects to handle several tasks simultaneously, create, modify, and monitor these tasks dynamically as needed.

Each task is stored in an array, and **taskMe** allows you to add, remove, or update the status of tasks regularly using the `Update()` method. If a task has expired, the associated action will be automatically executed.

### Key Features

- **Delay**: Wait for a specified time without executing any action.
- **Single**: Execute an action one time.
- **Repetitive**: Execute an action repeatedly for a specified number of times.
- **Continuous**: Execute an action continuously without limits.
- **Alternant**: Execute two actions alternately for a specified number of times.
- **AlternantContinuous**: Execute two actions alternately without limits.

### Using taskMe

You can create a **taskMe** object by specifying its capacity, which is the maximum number of tasks that can be stored.

```cpp
TaskMe myTaskManager(10);  // Maximum 10 tasks
```

#### Adding Tasks

Use one of the following methods to add tasks to **taskMe**:

```cpp
uint8_t AddDelay(int time, Action callback);
uint8_t AddSingle(int time, Action action);
uint8_t AddRepetitive(int time, Action action, unsigned int repetitions);
uint8_t AddContinuous(int time, Action action);
uint8_t AddAlternant(int time, Action action, Action alternateAction, unsigned int repetitions);
uint8_t AddAlternantContinuous(int time, Action action, Action alternateAction);
```

#### Updating Task Status

To ensure all tasks are executed at their scheduled time, regularly call the `Update()` method.

```cpp
myTaskManager.Update();
```

#### Removing Tasks

If you want to cancel a task, use the `CancelTask()` method.

```cpp
myTaskManager.CancelTask(taskIndex);
```

### Example Implementations

#### Example 1: Basic Delayed Task

This example demonstrates adding a task that is triggered once after a delay.

```cpp
#include <TaskMe.h>

TaskMe taskManager(5);  // Create a TaskMe object with capacity for 5 tasks.

void setup() {
    Serial.begin(115200);

    // Add a single delay task with a 3-second delay and a callback function.
    taskManager.AddDelay(3000, []() {
        Serial.println("Task executed after 3 seconds.");
    });
}

void loop() {
    taskManager.Update();  // Update the task manager in the main loop.
}
```

#### Example 2: Single Action Task

In this example, a task is scheduled to run once after a specified time.

```cpp
#include <TaskMe.h>

TaskMe taskManager(5);

void ledToggle() {
    Serial.println("LED toggled!");
}

void setup() {
    Serial.begin(115200);

    // Add a task that toggles the LED once after 2 seconds.
    taskManager.AddSingle(2000, ledToggle);
}

void loop() {
    taskManager.Update();
}
```

#### Example 3: Repetitive Task with a Callback

This example shows how to create a repetitive task that runs multiple times and executes a callback when completed.

```cpp
#include <TaskMe.h>

TaskMe taskManager(5);

void blink() {
    Serial.println("Blinking...");
}

void onBlinkComplete() {
    Serial.println("Blink completed!");
}

void setup() {
    Serial.begin(115200);

    // Add a task that repeats 5 times every 1 second, with a callback when done.
    taskManager.AddRepetitive(1000, blink, 5, onBlinkComplete);
}

void loop() {
    taskManager.Update();
}
```

#### Example 4: Continuous Alternating Task

This example adds a task that alternates between two actions continuously.

```cpp
#include <TaskMe.h>

TaskMe taskManager(5);

void toggleLEDOn() {
    Serial.println("LED ON");
}

void toggleLEDOff() {
    Serial.println("LED OFF");
}

void setup() {
    Serial.begin(115200);

    // Add an alternating task that switches between two actions every 1 second.
    taskManager.AddAlternantContinuous(1000, toggleLEDOn, toggleLEDOff);
}

void loop() {
    taskManager.Update();
}
```

#### Example 5: Canceling a Task

In this example, a task is added and canceled after a certain condition.

```cpp
#include <TaskMe.h>

TaskMe taskManager(5);
uint8_t taskID;

void myTask() {
    Serial.println("Task is running...");
}

void setup() {
    Serial.begin(115200);

    // Add a continuous task and save its ID.
    taskID = taskManager.AddContinuous(2000, myTask);
}

void loop() {
    taskManager.Update();

    // Cancel the task after 10 seconds.
    if (millis() > 10000) {
        taskManager.CancelTask(taskID);
        Serial.println("Task canceled.");
    }
}
```

#### Example 6: Blinking LED with Alternate Actions and Callback

In this example, the LED blinks on and off alternately every 500 milliseconds, with a total of 10 repetitions. After the task completes, a callback function is executed to turn off the LED permanently.

```cpp
#include <TaskMe.h>

const int ledPin = 13;
TaskMe taskManager(5);

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);

    // Add an alternant task that toggles the LED every 500 ms, 10 times, and then runs a callback
    taskManager.AddAlternant(500, turnOnLED, turnOffLED, 10, taskCompleted);
}

void loop() {
    taskManager.Update();
}

void turnOnLED() {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
}

void turnOffLED() {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
}

void taskCompleted() {
    Serial.println("Task Completed: LED turned off permanently.");
    digitalWrite(ledPin, LOW);
}
```

#### Example 7: Continuous Task with Callback

This example shows how to execute a continuous task that prints a message every second. After stopping the task, a callback is executed to signal the task has been stopped.

```cpp
#include <TaskMe.h>

TaskMe taskManager(3);

void setup() {
    Serial.begin(9600);
    // Add a continuous task that prints "Running..." every second (1000 ms)
    taskManager.AddContinuous(1000, printRunning, taskCompleted);
}

void loop() {
    taskManager.Update();
    // Simulate stopping the task after 10 seconds
    if (millis() > 10000) {
        taskManager.CancelTask(0);
    }
}

void printRunning() {
    Serial.println("Running...");
}

void taskCompleted() {
    Serial.println("Task has been stopped.");
}
```

#### Example 8: Multiple Tasks with Repetitions and Alternate Actions

This example demonstrates managing multiple tasks. The first task alternates between two functions, while the second task is repetitive, counting down from 10 to 1.

```cpp
#include <TaskMe.h>

TaskMe taskManager(5); // Capacity for 5 tasks

int counter = 10;

void setup() {
    Serial.begin(9600);

    // Task 1: Alternates between two messages every 300ms, 8 repetitions
    taskManager.AddAlternant(300, printMessage1, printMessage2, 8, alternateTaskCompleted);

    // Task 2: Repeats the countdown from 10 to 1 every second
    taskManager.AddRepetitive(1000, countdown, 10, countdownCompleted);
}

void loop() {
    taskManager.Update();
}

void printMessage1() {
    Serial.println("Message 1");
}

void printMessage2() {
    Serial.println("Message 2");
}

void alternateTaskCompleted() {
    Serial.println("Alternate task completed!");
}

void countdown() {
    Serial.println(counter--);
}

void countdownCompleted() {
    Serial.println("Countdown task completed!");
}
```

#### Example 9: Delayed Task with Callback

This example demonstrates a simple delay task that waits for 5 seconds and then turns on an LED, followed by a callback to confirm completion.

```cpp
#include <TaskMe.h>

const int ledPin = 13;
TaskMe taskManager(2);

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);

    // Add a delay task of 5 seconds (5000 ms) to turn on the LED
    taskManager.AddDelay(5000, turnOnLED);
}

void loop() {
    taskManager.Update();
}

void turnOnLED() {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED is now ON after delay.");
}
```

#### Example 10: Continuous Alternating Task with Stop Mechanism

This example runs an alternating task that toggles between two states continuously every second, and then stops it manually after 10 seconds.

```cpp
#include <TaskMe.h>

TaskMe taskManager(4);
const int ledPin = 13;

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);

    // Add a continuous alternant task to blink the LED with alternate actions
    taskManager.AddAlternantContinuous(1000, turnOnLED, turnOffLED);
}

void loop() {
    taskManager.Update();

    // Stop the continuous task after 10 seconds
    if (millis() > 10000) {
        taskManager.CancelTask(0);  // Assuming task 0 is the alternant continuous task
        Serial.println("Alternant continuous task stopped.");
    }
}

void turnOnLED() {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
}

void turnOffLED() {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
}
```

#### Example 11: Combining Continuous, Repetitive, and Alternating Tasks

This more complex example shows how multiple types of tasks can work together. One task continuously blinks an LED, another task alternates between messages, and a third task counts down repeatedly.

```cpp
#include <TaskMe.h>

TaskMe taskManager(6);
const int ledPin = 13;
int counter = 5;

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);

    // Task 1: Continuous LED blink every 1 second
    taskManager.AddContinuous(1000, blinkLED);

    // Task 2: Alternates between two messages every 500ms, 6 times
    taskManager.AddAlternant(500, printHello, printWorld, 6, alternateCompleted);

    // Task 3: Repetitive countdown every 2 seconds
    taskManager.AddRepetitive(2000, countdown, 5, countdownCompleted);
}

void loop() {
    taskManager.Update();
}

void blinkLED() {
    static bool ledState = false;
    digitalWrite(ledPin, ledState ? HIGH : LOW);
    ledState = !ledState;
}

void printHello() {
    Serial.println("Hello");
}

void printWorld() {
    Serial.println("World");
}

void alternateCompleted() {
    Serial.println("Alternating messages task completed!");
}

void countdown() {
    Serial.println(counter--);
}

void countdownCompleted() {
    Serial.println("Countdown task completed!");
}
```
