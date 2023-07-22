This assignment will test your ability to create, use, manipulate and solve problems using linked lists. To do this, the todo list is implemented as a linked list of tasks, and another separate linked list is used to keep track of your “completed tasks”.

## Compile and Run
```gcc cs_todo.c -o cs_todo```

```./cs_todo```

## Stage 1
For stage 1 of this assignment, you will be creating a basic structure for CS ToDo. This will include:

  - Setting up an empty todo list
  - Adding tasks to the todo list:
  - Enter Command: a finish_assignment_2 assignments high
  - Printing tasks in the todo list
  - Updating priority of tasks in the todo list
  - Counting the number of tasks in the todo list


## Stage 2
For stage 2 of this assignment, you will apply more advanced linked list knowledge by using the second separate list of completed tasks. This stage will involve:

  - Completion of tasks
  - Printing of completed tasks
  - Estimated completion time of current tasks based on completed tasks

## Stage 3
For stage 3 of this assignment, you will add the ability to delete tasks from the list (free task-associated memory to avoid memory leaks), as well as allow repetition of tasks day-to-day.

Specific tasks involve:

  - Deleting from the tasks list
  - Finishing the day by deleting all completed tasks
  - Adding functionality to repeat tasks each day
  - Deleting the entire todo list

## Stage 4
For stage 4 of this assignment, you will be doing even more advanced things with your todo list, including:

  - Pattern matching on task names
  - Deleting all tasks matching this pattern
  - Sorting the the tasks list


## Commund Example
| Stage       | Command                        | Example                                      | Meaning                                                      |
|-------------|--------------------------------|----------------------------------------------|--------------------------------------------------------------|
| 1.1         | n/a                            | n/a                                          | The setup stage of the assignment                            |
| 1.2         | a [task_name] [category] [priority] | a clean_house home medium                     | Adds a task to the todo list                                 |
| 1.3         | p                              | p                                            | Prints the todo list                                         |
| 1.4         | i [task] [category]            | i clean_house home                           | Increases the priority of a task                             |
| 1.5         | n                              | n                                            | Counts the number of tasks in the todo list                  |
| End stage 1 |                                |                                              |                                                              |
| 2.1         | c [task] [category] [start_time (minutes)] [finish_time (minutes)] | c assignment 1511 600 1000                  | Completes given task in the start-end time provided           |
| 2.2         | P                              | P                                            | Prints out the completed tasks                               |
| 2.3         | e                              | e                                            | Calculates the expected completion time of all incomplete tasks |
| End stage 2 |                                |                                              |                                                              |
| 3.1         | d [task] [category]            | d study comp1511                             | Deletes a given task from the todo list                       |
| 3.2         | f                              | f                                            | Finishes the day, deleting all completed tasks                |
| 3.3         | r [task] [category]            | r walk_dog pets                              | Marks a task as repeatable. When the day finishes, it is added back to the todo list |
| 3.4         | n/a                            | n/a                                          | Frees all memory associated with program upon completion      |
| End stage 3 |                                |                                              |                                                              |
| 4.1         | m [pattern]                    | m [abc]*end?                                 | Matches all tasks with name matching the pattern "[abc]*end?" |
| 4.2         | ^ [pattern]                    | ^ [abc]*end?                                 | Deletes all incomplete tasks matching the pattern "[abc]*end?" |
| 4.3         | s                              | s                                            | Sorts the incomplete tasks list by category->priority->name   |
| End stage 4 |                                |                                              |                                                              |
