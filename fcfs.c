#include <stdio.h>

struct Process {
    int id;         // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
};

void findWaitingTime(struct Process proc[], int n) {
    proc[0].waiting = 0; // First process has 0 waiting time

    // Calculate waiting time for each process
    for (int i = 1; i < n; i++) {
        proc[i].waiting = proc[i - 1].waiting + proc[i - 1].burst;
    }
}

void findTurnaroundTime(struct Process proc[], int n) {
    // Calculate turnaround time for each process
    for (int i = 0; i < n; i++) {
        proc[i].turnaround = proc[i].waiting + proc[i].burst;
    }
}

void findavgTime(struct Process proc[], int n) {
    findWaitingTime(proc, n);
    findTurnaroundTime(proc, n);

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

void sortProcessesByArrival(struct Process proc[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (proc[j].arrival > proc[j + 1].arrival) {
                // Swap processes
                struct Process temp = proc[j];
                proc[j] = proc[j + 1];
                proc[j + 1] = temp;
            }
        }
    }
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

    // Sort processes by arrival time
    sortProcessesByArrival(proc, n);

    // Calculate and display average waiting and turnaround times
    findavgTime(proc, n);

    return 0;
}
