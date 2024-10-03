#include <stdio.h>

struct Process {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
};

void findWaitingTime(struct Process proc[], int n) {
    int complete = 0, time = 0, min_index, min_burst = 0;
    bool isCompleted[n];

    // Initialize the completion status of all processes to false
    for (int i = 0; i < n; i++) {
        isCompleted[i] = false;
    }

    while (complete < n) {
        min_burst = 1000; // Initialize to a large value
        min_index = -1;

        // Find the process with the minimum burst time that has arrived
        for (int i = 0; i < n; i++) {
            if (proc[i].arrival <= time && !isCompleted[i] && proc[i].burst < min_burst) {
                min_burst = proc[i].burst;
                min_index = i;
            }
        }

        if (min_index != -1) {
            // Process found, execute it
            time += proc[min_index].burst; // Increase current time
            proc[min_index].waiting = time - proc[min_index].arrival - proc[min_index].burst; // Calculate waiting time
            proc[min_index].turnaround = time - proc[min_index].arrival; // Calculate turnaround time
            isCompleted[min_index] = true; // Mark the process as completed
            complete++;
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

    struct Process proc[n];

    // Input process details
    for (int i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Enter arrival time for Process %d: ", proc[i].id);
        scanf("%d", &proc[i].arrival);
        printf("Enter burst time for Process %d: ", proc[i].id);
        scanf("%d", &proc[i].burst);
    }

    // Calculate and display average waiting and turnaround times
    findAvgTime(proc, n);

    return 0;
}
