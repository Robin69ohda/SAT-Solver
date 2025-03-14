#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_VARS 1000
#define MAX_CLAUSES 1000
#define MAX_LITERALS 1000

typedef struct {
    int literals[MAX_LITERALS];
    int size;
} Clause;

typedef struct {
    Clause clauses[MAX_CLAUSES];
    int num_vars;
    int num_clauses;
} CNF;

int model[MAX_VARS]; // Valori atribuite variabilelor

void read_cnf(FILE *file, CNF *cnf) {
    char line[256];
    int clause_count = 0;

    while (fgets(line, sizeof(line), file)) {
        // Ignoră liniile de comentarii
        if (line[0] == 'c') continue; 

        // Citirea liniei "p cnf num_vars num_clauses"
        if (line[0] == 'p') {
            if (sscanf(line, "p cnf %d %d", &cnf->num_vars, &cnf->num_clauses) != 2) {
                exit(1);
            }
        } else {
            // Curăță linia de orice caractere suplimentare
            line[strcspn(line, "\n")] = 0;  // Elimină newline la finalul liniei

            // Citirea clauzelor
            Clause clause;
            clause.size = 0;

            int literal;
            char *ptr = line;

            // Procesăm fiecare număr din linie
            while (sscanf(ptr, "%d", &literal) == 1) {

                // Dacă ajungem la 0, înseamnă că este sfârșitul clauzei
                if (literal == 0) break;

                // Adaugă literalul în clauză
                clause.literals[clause.size++] = literal;
                
                // Avansăm pointer-ul ptr pentru a citi următorul număr
                while (*ptr != ' ' && *ptr != '\0') ptr++; // Sărim peste număr
                while (*ptr == ' ') ptr++; // Sărim peste spații
            }

            // Adăugăm clauza la lista de clauze
            cnf->clauses[clause_count++] = clause;
        }
    }
}

bool is_clause_satisfied(Clause *clause, int *model) {
    for (int i = 0; i < clause->size; i++) {
        int literal = clause->literals[i];
        int var = abs(literal);
        if ((literal > 0 && model[var] == 1) || (literal < 0 && model[var] == 0)) {
            return true;
        }
    }
    return false;
}

bool unit_propagation(CNF *cnf, int *model) {
    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < cnf->num_clauses; i++) {
            Clause *clause = &cnf->clauses[i];
            int unassigned_count = 0, last_unassigned_literal = 0;
            bool clause_satisfied = false;

            for (int j = 0; j < clause->size; j++) {
                int literal = clause->literals[j];
                int var = abs(literal);

                if (model[var] == -1) {
                    unassigned_count++;
                    last_unassigned_literal = literal;
                } else if ((literal > 0 && model[var] == 1) || (literal < 0 && model[var] == 0)) {
                    clause_satisfied = true;
                    break;
                }
            }

            if (clause_satisfied) continue;

            if (unassigned_count == 0) return false; // Clauză nesatisfăcută

            if (unassigned_count == 1) {
                int literal = last_unassigned_literal;
                int var = abs(literal);
                model[var] = (literal > 0) ? 1 : 0;
                changed = true;
            }
        }
    }
    return true;
}

bool dpll(CNF *cnf, int *model) {
    // Propagare unitară
    if (!unit_propagation(cnf, model)) return false;

    // Verificăm dacă toate variabilele sunt atribuite
    bool all_assigned = true;
    for (int i = 1; i <= cnf->num_vars; i++) {
        if (model[i] == -1) {
            all_assigned = false;
            break;
        }
    }

    if (all_assigned) {
        // Validăm dacă soluția satisfice toate clauzele
        for (int i = 0; i < cnf->num_clauses; i++) {
            if (!is_clause_satisfied(&cnf->clauses[i], model)) {
                return false; // Soluția nu este validă
            }
        }
        return true; // Toate clauzele sunt satisfăcute
    }

    // Alegem o variabilă neatribuită și încercăm să o atribuim
    int var = 1;
    while (model[var] != -1) var++;

    // Backtracking
    int backup_model[MAX_VARS];
    memcpy(backup_model, model, sizeof(int) * (cnf->num_vars + 1));

    model[var] = 1;
    if (dpll(cnf, model)) return true;

    memcpy(model, backup_model, sizeof(int) * (cnf->num_vars + 1));
    model[var] = 0;
    return dpll(cnf, model);
}

void print_result(CNF *cnf, bool satisfiable) {
    if (satisfiable) {
        // Atribuire implicită pentru variabile neatribuite
        for (int i = 1; i <= cnf->num_vars; i++) {
            if (model[i] == -1) {
                model[i] = 0; // Implicit FALSE
            }
        }

        printf("s SATISFIABLE\n");
        printf("v ");
        for (int i = 1; i <= cnf->num_vars; i++) {
            printf("%d ", model[i] == 1 ? i : -i);
        }
        printf("0\n");
    } else {
        printf("s UNSATISFIABLE\n");
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    CNF cnf;
    read_cnf(file, &cnf);
    fclose(file);

    for (int i = 0; i < MAX_VARS; i++) model[i] = -1; // Toate variabilele neatribuite

    bool satisfiable = dpll(&cnf, model);
    print_result(&cnf, satisfiable);

    return 0;
}
