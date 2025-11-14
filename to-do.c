#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

// --- Constants and Global Data Structure ---
// These define the  to-do list structure for the entire program

#define MAX_TASKS 50   // Maximum number of tasks we can store
#define MAX_LENGTH 150 // Maximum length of a task description

struct Task {
    char description[MAX_LENGTH];
    int isComplete; // 0 = incomplete, 1 = complete
};

// --- Global Variables ---
struct Task taskList[MAX_TASKS]; // The array that holds all our tasks
int taskCount = 0;               // The current number of tasks in the list

// --- Function Prototypes ---
void printMenu();
void addTask();
void viewTasks();
void markTaskComplete();
void clearInputBuffer(); // A helper function to fix scanf/fgets issues
void clearScreen();      // A helper function to clear the terminal
void pauseScreen();      // A helper function to pause before clearing

/*
==================================================================
  MAIN FUNCTION 
==================================================================
*/
int main() {
    int choice = 0;

    // This is the main "do-while" loop that keeps the program running.
    do {
        clearScreen(); // Clear the screen at the start of each loop
        printMenu();
        // Get the user's choice
        if (scanf("%d", &choice) != 1) {
            // This 'if' is input validation.
            // It handles cases where the user types letters instead of numbers.
            printf("Error: Invalid input. Please enter a number (1-4).\n");
            clearInputBuffer(); // Clear the bad input
            choice = 0;         // Reset choice to prevent errors
            continue;           // Skip the rest of the loop and start over
        }

        // This removes the "Enter" keypress, so it doesn't break addTask's fgets.
        clearInputBuffer();

        // This "switch" statement is the main conditional logic
        switch (choice) {
            case 1:
                clearScreen(); // Clear screen before showing the function
                addTask(); // Call the addTask function
                break;
            case 2:
                clearScreen(); // Clear screen before showing the function
                viewTasks(); // Call the viewTasks function
                break;
            case 3:
                clearScreen(); // Clear screen before showing the function
                markTaskComplete(); // Call the markTaskComplete function
                break;
            case 4:
                printf("\nGoodbye!\n"); // Exit message
                break;
            default:
                printf("\nInvalid choice. Please pick a number from 1 to 4.\n");
        }

        // After the switch, pause the screen so the user can see the output
        // But don't pause if the user is exiting
        if (choice != 4) {
            pauseScreen();
        }

    } while (choice != 4); // The loop continues as long as the choice is NOT 4

    return 0; // Program finished successfully
}

/*
==================================================================
  FUNCTION: printMenu
  Displays the main menu to the user.
==================================================================
*/
void printMenu() {
    printf("\n--- C To-Do List ---\n");
    printf("1. Add a new task\n");
    printf("2. View all tasks\n");
    printf("3. Mark a task as complete\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

/*
==================================================================
  FUNCTION: addTask
  (Adapted from 'void addTask();.c')
  Adds a new, incomplete task to the global taskList.
==================================================================
*/
void addTask() {
    // It validates if the list is full.
    if (taskCount >= MAX_TASKS) {
        printf("Sorry, the to-do list is full.\n");
        return; // Exit the function
    }

    // Prompt for user input
    printf("Enter task description: ");

    // Use fgets to safely read the task description
    if (fgets(taskList[taskCount].description, MAX_LENGTH, stdin) != NULL) {
        
        // Remove the newline character (\n) that fgets automatically adds
        taskList[taskCount].description[strcspn(taskList[taskCount].description, "\n")] = 0;

        // Set the new task's status to incomplete (0)
        taskList[taskCount].isComplete = 0;

        // Increment the global task counter
        taskCount++;

        printf("Task added!\n");
    } else {
        printf("Error reading input. Task not added.\n");
    }
}

/*
==================================================================
  FUNCTION: viewTasks
  (Adapted from 'ViewTask.c')
  Uses a 'for' loop to display all tasks in the list.
==================================================================
*/
void viewTasks() {
    printf("\n--- Your Tasks ---\n");

    // This 'if' is a conditional check for an empty list.
    if (taskCount == 0) {
        printf("You have no tasks.\n");
        return; // Exit the function
    }

    // This 'for' loop iterates through the taskList array.
    // It stops at 'taskCount'
    for (int i = 0; i < taskCount; i++) {

        // This 'if' (as a ternary operator) is a conditional check
        // to display [X] for complete or [ ] for incomplete.
        printf("%d. [%s] %s\n",
               i + 1, // Display 1-based number for the user
               taskList[i].isComplete == 1 ? "X" : " ", // Use "X" if 1 (true), " " if 0 (false)
               taskList[i].description
        );
    }
}

/*
==================================================================
  FUNCTION: markTaskComplete
  (Adapted from 'void markTaskComplete();.c')
  Marks an existing task as complete.
==================================================================
*/
void markTaskComplete() {
    int taskNumber;

    // It checks if the list is empty BEFORE asking for input.
    if (taskCount == 0) {
        printf("You have no tasks to mark.\n");
        return; // Exit the function
    }

    // Show the user the list so they can pick a number
    viewTasks();

    printf("\nWhich task number do you want to mark as complete? ");
    
    // This 'if' validates the user's input
    if (scanf("%d", &taskNumber) != 1) {
        printf("Error: Invalid input. Please enter a number.\n");
        clearInputBuffer(); // Clear the bad input
        return;
    }
    clearInputBuffer(); // Clear the newline from scanf

    // This 'if' validates if the number is in the correct range
    if (taskNumber < 1 || taskNumber > taskCount) {
        printf("Invalid task number.\n");
    } else {
        int index = taskNumber - 1; // Convert user's 1-based number to 0-based array index

        // This 'if' checks if the task is already done
        if (taskList[index].isComplete == 1) {
            printf("That task is already complete.\n");
        } else {
            // Marks the task as complete
            taskList[index].isComplete = 1;
            printf("Task marked as complete!\n");
        }
    }
}


/*
==================================================================
  FUNCTION: clearInputBuffer
  A helper function to fix a common C problem.
  It reads and throws away all characters until it finds the
  "Enter" key (\n), clearing the input for the next command.
==================================================================
*/
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*
==================================================================
  FUNCTION: clearScreen
  A helper function to clear the terminal screen.
  Uses preprocessor directives for cross-platform compatibility.
==================================================================
*/
void clearScreen() {
    #ifdef _WIN32 // This checks if the OS is Windows (for Dev-C++)
        system("cls");
    #else // For other systems like Linux or macOS
        system("clear");
    #endif
}

/*
==================================================================
  FUNCTION: pauseScreen
  A helper function to pause the program and wait for user input.
  This lets the user read the output before the screen is cleared.
==================================================================
*/
void pauseScreen() {
    printf("\n\nPress Enter to continue...");
    clearInputBuffer(); // Wait for the "Enter" key
}