#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[100];
    int priority;
    int completed;
} Task;

typedef struct {
    Task *tasks;
    int count;
    int capacity;
} todoList;

/**
 * @brief Initializes the todoList members and dynamically allocates memory
 * for the array of tasks stored. If allocation fails, the program exits.
 *
 * @param td Pointer to the todoList struct instance to be initialized.
 */
void initializeList(todoList *td) {
    td->count = 0;
    td->capacity = 10;

    td->tasks = (Task *)malloc(sizeof(Task) * td->capacity);
    if (td->tasks == NULL) {
        printf("Failed to allocate memory. Exiting program.\n");
        exit(1);
    }
}

/**
 * @brief Initializes members of the Task struct, storing the given
 * task name, priority status, and completion status in the struct.
 * 
 * @param t Pointer to the Task struct instance to be initialized.
 * @param name Pointer to the name of the task.
 * @param priority Integer for the task's priority status
 * @param completion Integer for the task's completion status
 */
void initializeTask(Task *t, const char *name, int priority, int completion) {
    strncpy(t->name, name, sizeof(t->name) - 1);
    t->name[sizeof(t->name) - 1] = 0;
    t->priority = priority;
    t->completed = completion;
}

/**
 * @brief Checks the todoList count. If count is greater than or equal to the
 * todoList instance's capacity, the capacity is doubled and memory for the tasks
 * is doubled.
 * 
 * @param td Pointer to the todoList struct whose memory is being reallocated.
 * 
 * @return 0 on success, 1 on failure
 */
int capacityCheck(todoList *td) {
    if (td->count >= td->capacity) {
        int newCapacity = td->capacity * 2;
        Task *tmp = (Task *)realloc(td->tasks, sizeof(Task) * newCapacity);
        if (tmp == NULL) {
            printf("Failed to allocate memory. Exiting program.\n");
            return 1;
        }
        td->tasks = tmp;
        td->capacity = newCapacity;
    }
    return 0;
}

/**
 * @brief Adds a Task to the todoList member tasks. Dynamically reallocates memory
 * to expand size of the list if current capacity has been reached.
 * 
 * @param td Pointer to the todoList struct that the task is being added to.
 * @param name Pointer to the name of the task to add.
 * @param priority Integer for the priority status of the task to be added.
 */
void addTask(todoList *td, const char *name, int priority) {
    if (capacityCheck(td)) {
        return;
    }
    initializeTask(&td->tasks[td->count], name, priority, 0);
    td->count++;
}

/**
 * @brief swaps two Task struct instances via a temporary Task copy.
 *
 * @param a Pointer to the Task struct instance to be swapped.
 * @param b Pointer to the Task struct instance to be swapped.
 */
void swap(Task *a, Task *b) {
    Task tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * @brief Partitions the array around pivot set to priority of Task at
 * index [high] in array.
 *
 * @param arr Array of tasks to partition
 * @param low The low index for the list (0)
 * @param high The highest index for the list (todoList->count - 1)
 * 
 * @return Returns the final sorted index of the pivot element.
 */
int partition(Task arr[], int low, int high) {
    int piv = arr[high].priority;
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (arr[j].priority < piv) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return (i + 1);
}

/**
 * @brief Recursively applies quickSort to the sub-arrays defined by
 * partition index.
 *
 * @param arr Array of tasks to recursively quick sort
 * @param low The lowest index to access the array from
 * @param high The highest index to access array from (todoList->count-1)
 */
void quickSortRecursion(Task arr[], int low, int high) {
    if (low < high) {
        int piv = partition(arr, low, high);
        quickSortRecursion(arr, low, piv - 1);
        quickSortRecursion(arr, piv + 1, high);
    }
}

/**
 * @brief Function calls quickSortRecursion if there are at least 2 elements
 * in todoList to sort tasks by priority status.
 *
 * @param td todoList struct instance to be quick sorted.
 */
void sortTasks(todoList *td) {
    if (td->count > 1) {
        quickSortRecursion(td->tasks, 0, td->count-1);
    }
}

/**
 * @brief Sorts todoList by priority status, then iterates through
 * todoList tasks and prints them.
 *
 * @param td todoList struct instance to be listed.
 */
void listTasks(todoList *td) {
    sortTasks(td);
    printf("\nTotal Tasks: %d\n", td->count);
    for (int i = 0; i < td->count; i++) {
        printf("\tTask %d: %s; priority: %d\n", i+1, td->tasks[i].name, td->tasks[i].priority);
    }
}

/**
 * @brief Removes a Task from tasks member of todoList given the
 * task name and decrements the count.
 *
 * @param td todoList struct instance to be quick sorted.
 * 
 * @return 0 on success, 1 on failure
 */
int deleteTask(todoList *td, char *name) {
    for (int i = 0; i < td->count; i++) {
        if (strcmp(td->tasks[i].name, name) == 0) {
            for (int j = i; j < td->count-1; j++) {
                td->tasks[j] = td->tasks[j+1];
            }
            td->count--;
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Opens a .txt file and iterates through todoList, writing
 * task name, task priority, and task completion status to file as 
 * comma separated values.
 * 
 * @param td todoList struct that we write data to file from
 * @param filename Name of the file that is being written to
 * 
 * @return 0 on success, 1 on failure
 */
int saveTasks(todoList *td, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening or creating file to write to. Exit program.\n");
        return 1;
    }
    for (int i = 0; i < td->count; i++) {
        fprintf(fp, "%s,%d,%d\n", 
            td->tasks[i].name, 
            td->tasks[i].priority,
            td->tasks[i].completed);
    }
    fclose(fp);
    return 0;
}

/**
 * @brief Frees memory that was dynamically allocated for todoList struct.
 *
 * @param td todoList struct instance to free memory from.
 */
void freeList(todoList *td) {
    free(td->tasks);
    td->tasks = NULL;
    td->count = 0;
    td->capacity = 0;
}

/**
 * @brief Opens a file and writes data to a todoList struct
 * to obtain the last saved version of the to-do list.
 *
 * @param td todoList struct instance that is being written to
 * @param filename Name of file to open and read from
 * 
 * @return 0 on success, 1 on failure
 */
int loadTasks(todoList *td, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file. Exiting program.\n");
        freeList(td);
        return 1;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (capacityCheck(td)) {
            fclose(fp);
            return 1;
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        char *taskName = strtok(buffer, ",");
        char *priority = strtok(NULL, ",");
        char *completion = strtok(NULL, ",");
        if (taskName != NULL && priority != NULL && completion != NULL) {
            int taskPriority = atoi(priority);
            int taskCompletion = atoi(completion);
            Task newTask;
            initializeTask(&newTask, taskName, taskPriority, taskCompletion);
            td->tasks[td->count] = newTask;
            td->count++;
        } else {
            printf("Error, file's formatting is off or strtok failed.\n");
            break;
        }
    }
    fclose(fp);
    return 0;
}

/**
 * @brief Controls program UI for to-do list management, parsing 
 * arguments from CLI to run functions accordingly.
 *
 * @param td todoList struct instance being worked with.
 * @param filename Name of file to save tasks to.
 */
void runMainLoop(todoList *td, char *filename) {
    while (1) {
        char input[100];
        printf("\nEnter command to execute: ");
        scanf("%99s", input);
        if (strcmp(input, "list") == 0) {
            listTasks(td);
        }
        else if (strcmp(input, "add") == 0) {
            char name[100];
            int priority;
            printf("\tEnter task name: ");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(name, sizeof(name),stdin);
            name[strcspn(name, "\n")] = 0;
            for (int i = 0; name[i]; i++) {
                if (name[i] == ',') {
                    name[i] = ';';
                }
            }
            printf("\nEnter priority status\n\t1: high priority\n\t2: medium priority\n\t3:low priority.\n");
            if (scanf("%d", &priority) != 1 || priority < 1 || priority > 3) {
                printf("\tInvalid input. Priority must be an integer between 1 and 3.\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF);
                continue;
            }
            addTask(td, name, priority);
        }
        else if (strcmp(input, "delete") == 0) {
            char name[100];
            printf("\tEnter task name to delete: ");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(name, sizeof(name),stdin);
            name[strcspn(name, "\n")] = 0;
            if (deleteTask(td, name) == 0) {
                printf("\tTask deleted.\n");
            } else {
                printf("\tTask not found.\n");
            }
        }
        else if (strcmp(input, "save") == 0) {
            saveTasks(td, filename);
        }
        else if (strcmp(input, "exit") == 0) {
            saveTasks(td, filename);
            return;
        } else {
            printf("\tInvalid command entered.\n");
        }
    }
}

/**
 * @brief Controls main logic; initializes todoList, gets user input to open/create
 * to-do list file before entering runMainLoop.
 *
 * @param argc Argument count
 * @param argv[] CLI arguments
 * 
 * @return 0 on success, 1 on failure
 */
int main(int argc, char *argv[]) {
    todoList tdl;
    initializeList(&tdl);
    printf("Hello, welcome to Leo's to-do list manager!\n");
    while (1) {
        char input[100];
        printf("\nTo navigate further into the program, type one of the following commands:\n\topen: view existing to-do list.\n\tcreate: create new to-do list.\n\texit: exit program.\n");
        scanf("%99s", input);
        if (strcmp(input, "open") == 0) {
            char filename[100];
            printf("\nEnter the to-do file name to open: ");
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            tdl.count = 0;
            if (loadTasks(&tdl, filename)) {
                printf("Exiting program. to-do list failed to load.\n");
                return 1;
            }
            printf("\nWelcome to the task manager. Listed below are commands to navigate it.\nlist: prints the formatted to-do list.\nadd: add task to the list.\ndelete: delete task from the list.\nsave: save current to-do list state.\nexit: exit the task manager.\n");
            runMainLoop(&tdl, filename);
        }
        else if (strcmp(input, "create") == 0) {
            printf("\nEnter name for to-do list file to be named: ");
            char filename[100];
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = 0;
            if (filename[0] != '\0') {
                tdl.count = 0;
                printf("\nWelcome to the task manager. Enter one of the following commands.\nlist: prints the formatted to-do list.\nadd: add task to the list.\ndelete: delete task from the list.\nsave: save current to-do list state.\nexit: exit the task manager.\n");
                runMainLoop(&tdl, filename);
            } else {
                printf("\nInvalid filename entered.\n");
            }
        }
        else if (strcmp(input, "exit") == 0) {
            break;
        } else {
            printf("\nInvalid command entered.\n");
        }
    }
    freeList(&tdl);
    return 0;
}
