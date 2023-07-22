
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVALID_PRIORITY -1

#define MAX_TASK_LENGTH 200
#define MAX_CATEGORY_LENGTH 40
#define MAX_STRING_LENGTH 1024
#define MAX_PATTERN_LENGTH 200

// You *should* #define each command
#define COMMAND_ADD_TASK 'a'
#define COMMAND_PRINT_TASKS 'p'
#define COMMAND_INCREASE_PRIORITY 'i'
#define COMMAND_PRINT_TASKS_COUNT 'n'
#define COMMAND_COMPLETE_TASK 'c'
#define COMMAND_PRINT_COMPLETE_TASK 'P'
#define COMMAND_ESTIMATE_TIME 'e'
#define COMMAND_DELETE_TASK 'd'
#define COMMAND_FINISH_DAY 'f'
#define COMMAND_REPEATABLE_TASK 'r'
#define COMMAND_MATCH_TASK 'm'
#define COMMAND_DELETE_MATCHED_TASK '^'
#define COMMAND_SORT 's'
#define COMMAND_END 'D'

enum priority { LOW,
                MEDIUM,
                HIGH };

struct task {
    char task_name[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    enum priority priority;
    int is_repeatable;
    struct task *next;
};

struct completed_task {
    struct task *task;
    int start_time;
    int finish_time;
    struct completed_task *next;
};

struct todo_list {
    struct task *tasks;
    struct completed_task *completed_tasks;
};

struct todo_list *todo; // Declare todo_list as a global variable

////////////////////////////////////////////////////////////////////////////////
///////////////////// YOUR FUNCTION PROTOTYPES GO HERE /////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Function prototypes

// Stage 1.1 - Adding Tasks
void add_task(const char *name_str, const char *category_str, enum priority *prio, int is_repeatable);

// Stage 1.3 - Printing Tasks
void print_tasks();

// Stage 1.4 - Update Priority
void update_task_priority(const char *name_str, const char *category_str);

// Stage 1.5 - Counting Tasks
void print_tasks_count();

// Stage 2.1 - Task Completion
void completed_task(const char *name_str, const char *category_str, int start_time, int finish_time);

// Stage 2.2 - Print Completed Tasks
void print_completed_tasks();

// Stage 2.3 - Expected Completion Time
void estimate_completion_time();

// Stage 3.1 - Delete Task
void delete_task(const char *name_str, const char *category_str);

// Stage 3.2 - Finish Day
void finish_day();

// Stage 3.3 - Repeat Task
void repeat_task(const char *name_str, const char *category_str);

// Stage 4.1 - Match Tasks
int task_name_matches(struct task *task, const char *pattern);
void match_tasks(const char *pattern);

// Stage 4.2 - Delete Matched Tasks
void delete_matched_tasks(const char *pattern);

// Stage 4.3 - Sort Tasks
void insert_sorted(struct task **sorted_list, struct task *new_task);
void sort_tasks();

void command_loop(struct todo_list *todo);

////////////////////////////////////////////////////////////////////////////////
//////////////////////// PROVIDED HELPER PROTOTYPES ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void parse_add_task_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH], enum priority *prio);
void parse_task_category_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH]);
void parse_pattern(
    char buffer[MAX_STRING_LENGTH], char pattern[MAX_PATTERN_LENGTH]);
void parse_complete_task_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH], int *start_time, int *finish_time);

enum priority string_to_priority(char priority[MAX_STRING_LENGTH]);
void remove_newline(char input[MAX_STRING_LENGTH]);
void trim_whitespace(char input[MAX_STRING_LENGTH]);
void print_one_task(int task_num, struct task *task);
void print_completed_task(struct completed_task *completed_task);

int task_compare(struct task *t1, struct task *t2);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    // Stage 1.1
    // You should initialize the `todo` variable below. You will need
    // to use the malloc() function to allocate memory for it!
    todo = (struct todo_list *)malloc(sizeof(struct todo_list));

    if (todo == NULL) {
        printf("Failed to allocate memory for the todo list.\n");
        return 1; // Return an error code indicating failure
    }

    // Initialize the tasks and completed_tasks pointers to NULL
    todo->tasks = NULL;
    todo->completed_tasks = NULL;

    printf("Welcome to CS ToDo\n");

    // Free the allocated memory before exiting
    command_loop(todo);

    // Freeing Memory
    free(todo);

    return 0;
}

/**
 * The central loop that executes commands until the program is completed.
 *
 * Parameters:
 *     todo - The todo list to execute commands on.
 *
 * Returns:
 *     Nothing
 */
void command_loop(struct todo_list *todo) {
    printf("Enter Command: ");
    char command;
    while (scanf(" %c", &command) == 1) {
        // TODO: Add to this loop as you complete the assignment
        if (command == COMMAND_ADD_TASK) {
            // Create a string to scan the entire command input into.
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);

            // Create variables for each part of the command being scanned in
            // (name of task, category of task and priority of task)
            char task_name[MAX_TASK_LENGTH];
            char task_category[MAX_CATEGORY_LENGTH];
            enum priority task_priority;
            parse_add_task_line(buffer, task_name, task_category, &task_priority);
            // Adding the Task
            add_task(task_name, task_category, &task_priority, 0); // Initially 0 means not repeatable task
        } else if (command == COMMAND_PRINT_TASKS) {
            print_tasks();
        } else if (command == COMMAND_INCREASE_PRIORITY) {
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);
            char task[MAX_TASK_LENGTH];
            char category[MAX_CATEGORY_LENGTH];
            parse_task_category_line(buffer, task, category);
            update_task_priority(task, category);
        } else if (command == COMMAND_PRINT_TASKS_COUNT) {
            print_tasks_count();
        } else if (command == COMMAND_COMPLETE_TASK) {
            // Fetch `[task] [category] [start_time] [finish_time]` from stdin
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);
            // Create strings for `task`/`category` and ints for times, then populate
            // them using the contents of `buffer`.
            char task[MAX_TASK_LENGTH];
            char category[MAX_CATEGORY_LENGTH];
            int start_time;
            int finish_time;
            parse_complete_task_line(buffer, task, category, &start_time, &finish_time);
            // Adding Completed Task
            completed_task(task, category, start_time, finish_time);
        } else if (command == COMMAND_PRINT_COMPLETE_TASK) {
            print_completed_tasks();
        } else if (command == COMMAND_ESTIMATE_TIME) {
            estimate_completion_time();
        } else if (command == COMMAND_DELETE_TASK) {
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);
            char task[MAX_TASK_LENGTH];
            char category[MAX_CATEGORY_LENGTH];
            parse_task_category_line(buffer, task, category);
            // Deleting Task
            delete_task(task, category);
        } else if (command == COMMAND_FINISH_DAY) {
            finish_day();
        } else if (command == COMMAND_REPEATABLE_TASK) {
            // Fetch `[task] [category]` from stdin
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);
            char task[MAX_TASK_LENGTH];
            char category[MAX_CATEGORY_LENGTH];
            parse_task_category_line(buffer, task, category);
            // Making task repeatable
            repeat_task(task, category);
        } else if (command == COMMAND_MATCH_TASK) {
            // Extracting Pattern
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);
            char pattern[MAX_TASK_LENGTH];
            parse_pattern(buffer, pattern);
            match_tasks(pattern);
        } else if (command == COMMAND_DELETE_MATCHED_TASK) {
            // Extracting Pattern
            char buffer[MAX_STRING_LENGTH];
            fgets(buffer, MAX_STRING_LENGTH, stdin);
            char pattern[MAX_TASK_LENGTH];
            parse_pattern(buffer, pattern);
            // Deleting matched task
            delete_matched_tasks(pattern);
        } else if (command == COMMAND_SORT) {
            sort_tasks();
        } else if (command == COMMAND_END) {
            free(todo);
            printf("[CTRL+D]All Done!\n");
            return;
        }

        printf("Enter Command: ");
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////// YOUR HELPER FUNCTIONS ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// You should add any helper functions you create here

// Stage 1.2 - Adding Tasks
void add_task(const char *name_str, const char *category_str, enum priority *prio, int is_repeatable) {
    // Create a new task
    struct task *new_task = (struct task *)malloc(sizeof(struct task));

    // Set the task properties
    strncpy(new_task->task_name, name_str, MAX_TASK_LENGTH - 1);
    strncpy(new_task->category, category_str, MAX_CATEGORY_LENGTH - 1);
    new_task->priority = *prio;
    new_task->is_repeatable = is_repeatable;

    if (new_task->priority == INVALID_PRIORITY) {
        printf("Invalid priority. Task not added.\n");
        free(new_task);
        return;
    }

    // Set the next pointer to NULL
    new_task->next = NULL;

    // Check if the list is empty
    if (todo->tasks == NULL) {
        todo->tasks = new_task;
    } else {
        // Find the last task in the list
        struct task *current_task = todo->tasks;
        while (current_task->next != NULL) {
            current_task = current_task->next;
        }

        // Append the new task to the end
        current_task->next = new_task;
    }
    // printf("Task %s added successfully.\n", name_str);
}

// Stage 1.3 - Printing Tasks
void print_tasks() {
    if (todo->tasks == NULL) {
        printf("==== Your ToDo List ====\n");
        printf("All tasks completed, you smashed it!\n");
        printf("====   That's it!   ====\n");
        return;
    }

    struct task *current_task = todo->tasks;
    int task_num = 1;
    printf("==== Your ToDo List ====\n");
    while (current_task != NULL) {
        print_one_task(task_num, current_task);
        current_task = current_task->next;
        task_num++;
    }
    printf("====   That's it!   ====\n");
}
// Stage 1.4 - Update Priority
void update_task_priority(const char *name_str, const char *category_str) {
    struct task *current_task = todo->tasks;

    while (current_task != NULL) {
        // Check if the task matches the name and category
        if (strcmp(current_task->task_name, name_str) == 0 && strcmp(current_task->category, category_str) == 0) {
            // Increase the priority
            switch (current_task->priority) {
            case LOW:
                current_task->priority = MEDIUM;
                break;
            case MEDIUM:
                current_task->priority = HIGH;
                break;
            case HIGH:
                current_task->priority = LOW;
                break;
            default:
                printf("Invalid priority. Task not found.\n");
                return;
            }

            // printf("Priority of task '%s' in category '%s' updated successfully.\n", name_str, category_str);
            return;
        }

        current_task = current_task->next;
    }

    printf("Could not find task '%s' in category '%s'.\n", name_str, category_str);
}

// Stage 1.5 - Counting Tasks
void print_tasks_count() {
    int count = 0;
    struct task *current_task = todo->tasks;

    while (current_task != NULL) {
        count++;
        current_task = current_task->next;
    }
    printf("There are %d items on your list!\n", count);
}
// Stage 2.1 - Task Completion
void completed_task(const char *name_str, const char *category_str, int start_time, int finish_time) {
    struct task *current_task = todo->tasks;
    struct task *prev_task = NULL;
    struct task *task_to_move = NULL;

    // Search for the task with name_str and category_str
    while (current_task != NULL) {
        if (strcmp(current_task->task_name, name_str) == 0 && strcmp(current_task->category, category_str) == 0) {
            task_to_move = current_task;
            break;
        }
        prev_task = current_task;
        current_task = current_task->next;
    }

    // If the task is found, remove it from the tasks list
    if (task_to_move != NULL) {
        // Remove the task from the tasks list
        if (prev_task == NULL) {
            todo->tasks = task_to_move->next; // The task to move is the first in the list
        } else {
            prev_task->next = task_to_move->next;
        }

        // Create a new completed_task and wrap the task to move into it
        struct completed_task *new_completed_task = (struct completed_task *)malloc(sizeof(struct completed_task));
        new_completed_task->task = task_to_move;
        new_completed_task->start_time = start_time;
        new_completed_task->finish_time = finish_time;

        // Add the completed_task to the head of the completed_tasks list
        new_completed_task->next = todo->completed_tasks;
        todo->completed_tasks = new_completed_task;

        // printf("Task '%s' in category '%s' marked as completed and added to completed tasks.\n", name_str, category_str);
    } else {
        printf("Could not find task '%s' in category '%s'.\n", name_str, category_str);
    }
}

// Stage 2.2 - Print Completed Tasks
void print_completed_tasks() {
    if (todo->completed_tasks == NULL) {
        printf("==== Completed Tasks ====\n");
        printf("No tasks have been completed today!\n");
        printf("=========================\n");
        return;
    }

    struct completed_task *current_completed_task = todo->completed_tasks;

    printf("==== Completed Tasks ====\n");
    while (current_completed_task != NULL) {
        print_completed_task(current_completed_task);
        current_completed_task = current_completed_task->next;
    }
    printf("=========================\n");
}

// Stage 2.3 - Expected Completion Time
void estimate_completion_time() {
    printf("Expected completion time for remaining tasks:\n");
    struct task *current_task = todo->tasks;

    int task_count = 1;

    while (current_task != NULL) {
        // Initialize variables to calculate average completion time
        int count = 0;
        int total_time = 0;

        struct completed_task *current_completed_task = todo->completed_tasks;

        while (current_completed_task != NULL) {
            // Check if the completed task belongs to the same category as the current task
            if (strcmp(current_completed_task->task->category, current_task->category) == 0) {
                count++;
                total_time += current_completed_task->finish_time - current_completed_task->start_time;
                // We can Cache the total_time for each cayegory to reduce execution time
            }
            current_completed_task = current_completed_task->next;
        }

        // If no tasks in the same category are completed, set the priority to 100 by default
        int average_time = 100;

        // Calculate the average completion time and update the current task's priority
        if (count > 0) {
            average_time = total_time / count;
        }

        print_one_task(task_count, current_task);
        printf("Expected completion time: %d minutes\n", average_time);

        current_task = current_task->next;
        task_count++;
    }
}

// Function to delete a task from the todo list
// Stage 3.1 - Delete Task
void delete_task(const char *name_str, const char *category_str) {
    // Check if the list is empty
    if (todo->tasks == NULL) {
        printf("No tasks in the list. Cannot delete.\n");
        return;
    }

    struct task *prev_task = NULL;
    struct task *current_task = todo->tasks;

    // Search for the task with the given name_str and category_str
    while (current_task != NULL) {
        if (strcmp(current_task->task_name, name_str) == 0 && strcmp(current_task->category, category_str) == 0) {
            // Task found, remove it from the list

            // Update the previous task's next pointer to skip the current task
            if (prev_task != NULL) {
                prev_task->next = current_task->next;
            } else {
                // If the current task is the first node, update the list head
                todo->tasks = current_task->next;
            }

            // Free the memory allocated for the deleted task
            free(current_task);

            // printf("Task '%s' in category '%s' deleted successfully.\n", name_str, category_str);
            return;
        }

        prev_task = current_task;
        current_task = current_task->next;
    }
    printf("Could not find task '%s' in category '%s'.\n", name_str, category_str);
}
// Stage 3.2 - Finish Day
void finish_day() {
    struct completed_task *current_completed_task = todo->completed_tasks;
    while (current_completed_task != NULL) {
        struct completed_task *next_completed_task = current_completed_task->next;

        // printf("current_completed_task->task->is_repeatable - %d\n", current_completed_task->task->is_repeatable);

        if (current_completed_task->task->is_repeatable == 1) {
            add_task(current_completed_task->task->task_name, current_completed_task->task->category, &current_completed_task->task->priority, 1);
        }
        free(current_completed_task);
        current_completed_task = next_completed_task;
    }
    // Set the completed_tasks pointer to NULL to indicate an empty list
    todo->completed_tasks = NULL;
}
// Stage 3.3 - Repeat Task
void repeat_task(const char *name_str, const char *category_str) {
    // Check if the list is empty
    if (todo->tasks == NULL) {
        printf("No tasks in the list. Cannot repeat.\n");
        return;
    }

    struct task *current_task = todo->tasks;

    // Search for the task with the given name_str and category_str
    while (current_task != NULL) {
        if (strcmp(current_task->task_name, name_str) == 0 && strcmp(current_task->category, category_str) == 0) {
            // Task found, changhe its repeatable
            // printf("current_task->is_repeatable - %d\n", current_task->is_repeatable);

            if (current_task->is_repeatable == 1) {
                current_task->is_repeatable = 0;
            } else {
                current_task->is_repeatable = 1;
            }
            // printf("Task '%s' in category '%s' marked as repeatable.\n", name_str, category_str);
            return;
        }
        current_task = current_task->next;
    }

    printf("Could not find task '%s' in category '%s'.\n", name_str, category_str);
}

int task_name_matches(struct task *task, const char *pattern) {
    if (*pattern == '\0')
        return !task->task_name[0];

    if (*pattern == '*') {
        if (task_name_matches(task, pattern + 1))
            return 1;
        if (task->task_name[0] && task_name_matches((struct task *)((char *)task + 1), pattern))
            return 1;
        return 0;
    } else if (*pattern == '?') {
        if (task->task_name[0])
            return task_name_matches((struct task *)((char *)task + 1), pattern + 1);
        return 0;
    } else if (*pattern == '[') {
        const char *end = strchr(pattern + 1, ']');
        if (!end)
            return 0; /* No close bracket */

        int matched = 0, negate = pattern[1] == '^';
        char c;
        const char *p;

        for (p = pattern + 1 + negate; !matched && p < end; p++) {
            c = task->task_name[0];
            if (*p == '-' && *(p - 1) < *(p + 1)) {
                matched |= (*(p - 1) <= c && c <= *(p + 1));
            } else {
                matched |= (*p == c);
            }
        }
        if ((matched && !negate) || (!matched && negate))
            return task_name_matches((struct task *)((char *)task + 1), end + 1);
    } else if (*pattern == task->task_name[0]) {
        return task_name_matches((struct task *)((char *)task + 1), pattern + 1);
    }

    return 0;
}
// Stage 4.1 - Match Tasks
void match_tasks(const char *pattern) {
    if (todo->tasks == NULL) {
        printf("No task found\n");
        return;
    }

    printf("pattern - %s\n", pattern);

    struct task *current_task = todo->tasks;

    int count = 1;
    while (current_task != NULL) {
        if (task_name_matches(current_task, pattern)) {
            print_one_task(count, current_task);
            count++;
        }
        current_task = current_task->next;
    }
}

// Stage 4.2 - Delete Matched Tasks
void delete_matched_tasks(const char *pattern) {
    if (todo->tasks == NULL) {
        printf("No task found\n");
        return;
    }

    printf("pattern - %s\n", pattern);

    struct task *current_task = todo->tasks;

    while (current_task != NULL) {
        if (task_name_matches(current_task, pattern)) {
            delete_task(current_task->task_name, current_task->category);
        }
        current_task = current_task->next;
    }
}

void insert_sorted(struct task **sorted_list, struct task *new_task) {
    struct task *current;
    if (*sorted_list == NULL || task_compare(*sorted_list, new_task) > 0) {
        new_task->next = *sorted_list;
        *sorted_list = new_task;
    } else {
        current = *sorted_list;
        while (current->next != NULL && task_compare(current->next, new_task) <= 0) {
            current = current->next;
        }
        new_task->next = current->next;
        current->next = new_task;
    }
}
// Stage 4.3 - Sort Tasks
void sort_tasks() {
    if (todo->tasks == NULL) {
        printf("No tasks in the list. Nothing to sort.\n");
        return;
    }

    struct task *sorted_list = NULL;
    struct task *current_task = todo->tasks;

    while (current_task != NULL) {
        struct task *next_task = current_task->next;
        current_task->next = NULL;
        insert_sorted(&sorted_list, current_task);
        current_task = next_task;
    }

    todo->tasks = sorted_list;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////// PROVIDED HELPER FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * Given a raw string in the format: [string1] [string2] [enum priority]
 * This function will extract the relevant values into the given variables.
 * The function will also remove any newline characters.
 *
 * For example, if given: "finish_assignment_2 assignment2 high"
 * The function will copy the string:
 *     "finish_assignment_2" into the `task_name` array
 * Then copy the string:
 *     "assignment2" into the `task_category` array
 * And finally, copy over the enum:
 *     "high" into the memory that `prio` is pointing at.
 *
 * Parameters:
 *     buffer        - A null terminated string in the following format
 *                     [string1] [string2] [enum priority]
 *     task_name     - A character array for the [string1] to be copied into
 *     task_category - A character array for the [string2] to be copied into
 *     prio          - A pointer to where [enum priority] should be stored
 *
 * Returns:
 *     None
 */
void parse_add_task_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH],
    enum priority *prio) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }

    // Extract value 3 as string
    char *prio_str = strtok(NULL, " ");
    if (prio_str != NULL) {
        *prio = string_to_priority(prio_str);
    }

    if (
        name_str == NULL ||
        category_str == NULL ||
        prio_str == NULL ||
        *prio == INVALID_PRIORITY) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/*
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * See `parse_add_task_line` for explanation - This function is very similar,
 * with only the exclusion of an `enum priority`.
 */
void parse_task_category_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH]) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }

    if (name_str == NULL || category_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

void parse_pattern(
    char buffer[MAX_STRING_LENGTH],
    char pattern[MAX_TASK_LENGTH]) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *pattern_str = strtok(buffer, " ");
    if (pattern_str != NULL) {
        strcpy(pattern, pattern_str);
    }

    if (pattern_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/*
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * See `parse_add_task_line` for explanation - This function is very similar,
 * with only the exclusion of an `enum priority` and addition of start/end times
 */
void parse_complete_task_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH],
    int *start_time,
    int *finish_time) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }

    // Extract value 2 as string
    char *start_str = strtok(NULL, " ");
    if (start_str != NULL) {
        *start_time = atoi(start_str);
    }

    // Extract value 2 as string
    char *finish_str = strtok(NULL, " ");
    if (finish_str != NULL) {
        *finish_time = atoi(finish_str);
    }

    if (
        name_str == NULL ||
        category_str == NULL ||
        start_str == NULL ||
        finish_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/**
 * Helper Function
 * You should not need to change this function.
 *
 * Given a raw string, will return the corresponding `enum priority`,
 * or INVALID_PRIORITY if the string doesn't correspond with the enums.
 *
 * Parameters:
 *     priority - string representing the corresponding `enum priority` value
 * Returns:
 *     enum priority
 */
enum priority string_to_priority(char priority[MAX_STRING_LENGTH]) {
    if (strcmp(priority, "low") == 0) {
        return LOW;
    } else if (strcmp(priority, "medium") == 0) {
        return MEDIUM;
    } else if (strcmp(priority, "high") == 0) {
        return HIGH;
    }

    return INVALID_PRIORITY;
}

/**
 * Helper Function
 * You should not need to change this function.
 *
 * Given an priority and a character array, fills the array with the
 * corresponding string version of the priority.
 *
 * Parameters:
 *     prio - the `enum priority` to convert from
 *     out  - the array to populate with the string version of `prio`.
 * Returns:
 *     Nothing
 */
void priority_to_string(enum priority prio, char out[MAX_STRING_LENGTH]) {
    if (prio == LOW) {
        strcpy(out, "LOW");
    } else if (prio == MEDIUM) {
        strcpy(out, "MEDIUM");
    } else if (prio == HIGH) {
        strcpy(out, "HIGH");
    } else {
        strcpy(out, "Provided priority was invalid");
    }
}

/*
 * Helper Function
 * You should not need to change this function.
 *
 * Given a raw string will remove and first newline it sees.
 * The newline character wil be replaced with a null terminator ('\0')
 *
 * Parameters:
 *     input - The string to remove the newline from
 *
 * Returns:
 *     Nothing
 */
void remove_newline(char input[MAX_STRING_LENGTH]) {
    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}

/*
 * Helper Function
 * You likely do not need to change this function.
 *
 * Given a raw string, will remove any whitespace that appears at the start or
 * end of said string.
 *
 * Parameters:
 *     input - The string to trim whitespace from
 *
 * Returns:
 *     Nothing
 */
void trim_whitespace(char input[MAX_STRING_LENGTH]) {
    remove_newline(input);

    int lower;
    for (lower = 0; input[lower] == ' '; ++lower)
        ;

    int upper;
    for (upper = strlen(input) - 1; input[upper] == ' '; --upper)
        ;

    for (int base = lower; base <= upper; ++base) {
        input[base - lower] = input[base];
    }

    input[upper - lower + 1] = '\0';
}

/**
 * Helper Function
 * You SHOULD NOT change this function.
 *
 * Given a task, prints it out in the format specified in the assignment.
 *
 * Parameters:
 *     task_num - The position of the task within a todo list
 *     task     - The task in question to print
 *
 * Returns:
 *     Nothing
 */
void print_one_task(int task_num, struct task *task) {
    char prio_str[MAX_STRING_LENGTH];
    priority_to_string(task->priority, prio_str);

    printf(
        "  %02d. %-30.30s [ %s ] %s\n",
        task_num, task->task_name, task->category, prio_str);

    int i = 30;
    while (i < strlen(task->task_name)) {
        printf("      %.30s\n", task->task_name + i);
        i += 30;
    }
}

/**
 * Helper Function
 * You SHOULD NOT change this function.
 *
 * Given a completed task, prints it out in the format specified in the
 * assignment.
 *
 * Parameters:
 *     completed_task - The task in question to print
 *
 * Returns:
 *     Nothing
 */
void print_completed_task(struct completed_task *completed_task) {
    int start_hour = completed_task->start_time / 60;
    int start_minute = completed_task->start_time % 60;
    int finish_hour = completed_task->finish_time / 60;
    int finish_minute = completed_task->finish_time % 60;

    printf(
        "  %02d:%02d-%02d:%02d | %-30.30s [ %s ]\n",
        start_hour, start_minute, finish_hour, finish_minute,
        completed_task->task->task_name, completed_task->task->category);

    int i = 30;
    while (i < strlen(completed_task->task->task_name)) {
        printf("      %.30s\n", (completed_task->task->task_name + i));
        i += 30;
    }
}

/**
 * Compares two tasks by precedence of category -> priority -> name and returns
 * an integer referring to their relative ordering
 *
 * Parameters:
 *     t1 - The first task to compare
 *     t2 - The second task to compare
 *
 * Returns:
 *     a negative integer if t1 belongs before t2
 *     a positive integer if t1 belongs after t2
 *     0 if the tasks are identical (This should never happen in your program)
 */
int task_compare(struct task *t1, struct task *t2) {
    int category_diff = strcmp(t1->category, t2->category);
    if (category_diff != 0) {
        return category_diff;
    }

    int priority_diff = t2->priority - t1->priority;
    if (priority_diff != 0) {
        return priority_diff;
    }

    return strcmp(t1->task_name, t2->task_name);
}