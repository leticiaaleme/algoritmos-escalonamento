#include <stdio.h>

typedef struct {
    int id;
    int tempo_execucao;
    int tempo_espera;
    int turnaround;
    int tempo_restante;
} Processo;


int coletaDados(Processo proc[]) {
    int n, i;
    printf("Digite o numero de processos: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        proc[i].id = i + 1;
        printf("Digite o tempo de execucao do processo %d: ", i + 1);
        scanf("%d", &proc[i].tempo_execucao);
    }

    return n;
}

// SJF (Shortest Job First)
void sjf(Processo proc[], int n) {
    int i, j;
    int total_tempo_espera = 0, total_turnaround = 0, tempo_total = 0;

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (proc[i].tempo_execucao > proc[j].tempo_execucao) {
                Processo temp = proc[i];
                proc[i] = proc[j];
                proc[j] = temp;
            }
        }
    }

    proc[0].tempo_espera = 0;

    for (i = 1; i < n; i++) {
        proc[i].tempo_espera = proc[i - 1].tempo_espera + proc[i - 1].tempo_execucao;
        total_tempo_espera += proc[i].tempo_espera;
    }

    for (i = 0; i < n; i++) {
        proc[i].turnaround = proc[i].tempo_espera + proc[i].tempo_execucao;
        total_turnaround += proc[i].turnaround;
        tempo_total += proc[i].tempo_execucao;
    }

    printf("\nSJF (Nao Preemptivo):\n");
    for (i = 0; i < n; i++) {
        printf("Processo %d - Tempo de espera: %d, Turnaround: %d\n", proc[i].id, proc[i].tempo_espera, proc[i].turnaround);
    }
    printf("Tempo medio de espera: %.2f\n", (float)total_tempo_espera / n);
    printf("Tempo medio de turnaround: %.2f\n", (float)total_turnaround / n);
    printf("Tempo total do processador: %.2f\n", (float)tempo_total);
}


// Round Robin
void roundRobin(Processo proc[], int n, int quantum) {
    int i, total_tempo_espera = 0, total_turnaround = 0, tempo_total = 0, completos = 0;

    for (i = 0; i < n; i++) {
        proc[i].tempo_restante = proc[i].tempo_execucao;
        proc[i].tempo_espera = 0;
    }

    while (completos < n) {
        for (i = 0; i < n; i++) {
            if (proc[i].tempo_restante > 0) {
                if (proc[i].tempo_restante > quantum) {
                    proc[i].tempo_restante -= quantum;
                    tempo_total += quantum;
                } else {
                    tempo_total += proc[i].tempo_restante;
                    proc[i].tempo_restante = 0;
                    completos++;
                    proc[i].turnaround = tempo_total;
                    proc[i].tempo_espera = proc[i].turnaround - proc[i].tempo_execucao;
                    total_tempo_espera += proc[i].tempo_espera;
                    total_turnaround += proc[i].turnaround;
                }
            }
        }
    }

    printf("\nRound Robin (Quantum %d):\n", quantum);
    for (i = 0; i < n; i++) {
        printf("Processo %d - Tempo de espera: %d, Turnaround: %d\n", proc[i].id, proc[i].tempo_espera, proc[i].turnaround);
    }
    printf("Tempo medio de espera: %.2f\n", (float)total_tempo_espera / n);
    printf("Tempo medio de turnaround: %.2f\n", (float)total_turnaround / n);
    printf("Tempo total do processador: %.2f\n", (float)tempo_total);
}


int main() {
    int choice, quantum;
    Processo proc[20]; 

    do {
        printf("\nEscolha o algoritmo de escalonamento:\n");
        printf("1. SJF (Nao Preemptivo)\n");
        printf("2. Round Robin (Preemptivo)\n");
        printf("3. Sair\n");
        printf("Opcao: ");
        scanf("%d", &choice);

        if (choice == 1 || choice == 2) {
            int n = coletaDados(proc); 

            if (choice == 1) {
                sjf(proc, n);
            } else if (choice == 2) {
                printf("Insira o Quantum: ");
                scanf("%d", &quantum);
                roundRobin(proc, n, quantum);
            }
        } else if (choice != 3) {
            printf("Opcao invalida.\n");
        }

    } while (choice != 3);

    return 0;
}
