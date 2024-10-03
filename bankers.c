#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

int allocation[MAX_PROCESSES][MAX_RESOURCES];  // Allocation matrix
int max[MAX_PROCESSES][MAX_RESOURCES];          // Maximum demand matrix
int need[MAX_PROCESSES][MAX_RESOURCES];         // Need matrix
int available[MAX_RESOURCES];                    // Available resources

void calculateNeed() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

bool isSafe() {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};

    // Step 1: Initialize work and finish
    for (int i = 0; i < MAX_RESOURCES; i++) {
        work[i] = available[i];
    }

    int count = 0;
    while (count < MAX_PROCESSES) {
        bool found = false;

        for (int p = 0; p < MAX_PROCESSES; p++) {
            if (!finish[p]) {
                // Check if need <= work
                bool canAllocate = true;
                for (int j = 0; j < MAX_RESOURCES; j++) {
                    if (need[p][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    // Add allocation to work
                    for (int j = 0; j < MAX_RESOURCES; j++) {
                        work[j] += allocation[p][j];
                    }
                    finish[p] = true;
                    found = true;
                    count++;
                }
            }
        }

        if (!found) {
            // If no process was found that could be allocated resources
            return false;
        }
    }
    return true;  // System is in a safe state
}

bool requestResources(int processID, int request[]) {
    // Check if request is less than need
    for (int j = 0; j < MAX_RESOURCES; j++) {
        if (request[j] > need[processID][j]) {
            printf("Error: Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // Check if request is less than available
    for (int j = 0; j < MAX_RESOURCES; j++) {
        if (request[j] > available[j]) {
            printf("Process %d must wait for resources.\n", processID);
            return false;
        }
    }

    // Pretend to allocate requested resources
    for (int j = 0; j < MAX_RESOURCES; j++) {
        available[j] -= request[j];
        allocation[processID][j] += request[j];
        need[processID][j] -= request[j];
    }

    // Check if the system is still in a safe state
    if (isSafe()) {
        printf("Resources allocated to Process %d.\n", processID);
        return true;
    } else {
        // Rollback if not safe
        for (int j = 0; j < MAX_RESOURCES; j++) {
            available[j] += request[j];
            allocation[processID][j] -= request[j];
            need[processID][j] += request[j];
        }
        printf("Resources cannot be allocated to Process %d (unsafe state).\n", processID);
        return false;
    }
}

int main() {
    // Initialize available resources
    printf("Enter available resources (total):\n");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        printf("Resource %d: ", i + 1);
        scanf("%d", &available[i]);
    }

    // Initialize maximum resources and allocation for each process
    for (int i = 0; i < MAX_PROCESSES; i++) {
        printf("Enter maximum resources for Process %d:\n", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            printf("Max resource %d: ", j + 1);
            scanf("%d", &max[i][j]);
        }
        printf("Enter allocation for Process %d:\n", i);
        for (int j = 0; j < MAX_RESOURCES; j++) {
            printf("Allocated resource %d: ", j + 1);
            scanf("%d", &allocation[i][j]);
        }
    }

    // Calculate need matrix
    calculateNeed();

    // Display initial state
    printf("\nInitial State:\n");
    printf("Available Resources: ");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    printf("\nAllocation Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            printf("%d ", allocation[i][j]);
        }
        printf("\n");
    }
    printf("Maximum Demand Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            printf("%d ", max[i][j]);
        }
        printf("\n");
    }
    printf("Need Matrix:\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }

    // Request resources for a process
    int processID;
    int request[MAX_RESOURCES];

    printf("\nEnter process ID (0-%d) to request resources: ", MAX_PROCESSES - 1);
    scanf("%d", &processID);

    printf("Enter request for resources:\n");
    for (int i = 0; i < MAX_RESOURCES; i++) {
        printf("Request for Resource %d: ", i + 1);
        scanf("%d", &request[i]);
    }

    // Try to allocate resources
    requestResources(processID, request);

    // Check system safety
    if (isSafe()) {
        printf("\nThe system is in a safe state.\n");
    } else {
        printf("\nThe system is not in a safe state.\n");
    }

    return 0;
}
