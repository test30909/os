#include <stdio.h>
#include <stdbool.h>

#define MAX 100

struct Process {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int remaining;  // Remaining time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
};

void findWaitingTime(struct Process proc[], int n) {
    int complete = 0, time = 0, min_index;
    bool check = false;

    while (complete != n) {
        // Find the process with the smallest remaining time
        int min_remaining = 1000; // Initialize to a large value
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= time && proc[i].remaining > 0 && proc[i].remaining < min_remaining) {
                min_remaining = proc[i].remaining;
                min_index = i;
                check = true;
            }
        }

        if (check) {
            // Execute the selected process for 1 time unit
            proc[min_index].remaining--;
            time++;
            // If the process is complete
            if (proc[min_index].remaining == 0) {
                complete++;
                proc[min_index].turnaround = time - proc[min_index].arrival;
                proc[min_index].waiting = proc[min_index].turnaround - proc[min_index].burst;
            }
        } else {
            // If no process is ready to execute, move time forward
            time++;
        }
    }
}

void findAvgTime(struct Process proc[], int n) {
    findWaitingTime(proc, n);

    float total_waiting = 0, total_turnaround = 0;

    printf("Process ID\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
                proc[i].id,
                proc[i].arrival,
                proc[i].burst,
                proc[i].waiting,
                proc[i].turnaround);
        total_waiting += proc[i].waiting;
        total_turnaround += proc[i].turnaround;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct Process proc[MAX];

    // Input process details
    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Enter arrival time for Process %d: ", proc[i].id);
        scanf("%d", &proc[i].arrival);
        printf("Enter burst time for Process %d: ", proc[i].id);
        scanf("%d", &proc[i].burst);
        proc[i].remaining = proc[i].burst; // Initialize remaining time
    }

    // Calculate and display average waiting and turnaround times
    findAvgTime(proc, n);

    return 0;
}
