1. read_cnf(FILE *file, CNF *cnf)
Această funcție citește un fișier care conține formula CNF în format DIMACS și o salvează într-o structură de date.

Intrare:

Fișierul file conținând o formulă în format DIMACS.
Structura cnf unde se va salva formula.

Pași:

Ignoră liniile care încep cu c (comentarii).
Identifică linia p cnf num_vars num_clauses și salvează numărul de variabile și clauze.
Parcurge restul liniilor pentru a citi clauzele. Fiecare linie conține literali (numere întregi), terminați cu 0.

Rezultat: Structura cnf conține toate clauzele și variabilele asociate.

2. is_clause_satisfied(Clause *clause, int *model)
Verifică dacă o clauză este satisfăcută, dată o atribuire curentă a variabilelor.

Intrare:

clause – o clauză (o listă de literali).
model – un array care indică valoarea fiecărei variabile (1 pentru true, 0 pentru false, -1 pentru neatribuit).

Pași:

Pentru fiecare literal din clauză:
    Dacă un literal este pozitiv și variabila sa este 1 în model, clauza este satisfăcută.
    Dacă un literal este negativ și variabila sa este 0, clauza este satisfăcută.

Rezultat: Returnează true dacă clauza este satisfăcută, altfel false.

3. unit_propagation(CNF *cnf, int *model)
Aplică regula de propagare unitară, adică atribuie valori variabilelor pentru a satisface clauze cu un singur literal rămas neatribuit.

Intrare:

cnf – structura care conține formula CNF.
model – atribuirile curente ale variabilelor.

Pași:

Parcurge fiecare clauză:
    Dacă o clauză nu este satisfăcută și are un singur literal neatribuit, atribuie variabila respectivă astfel încât clauza să fie satisfăcută.
Repetă procesul până când nu se mai fac schimbări sau se detectează o clauză imposibil de satisfăcut (returnează false).

Rezultat: Returnează true dacă propagarea a avut succes, altfel false.

4. dpll(CNF *cnf, int *model)
Implementarea principală a algoritmului DPLL. Acesta încearcă să satisfacă formula folosind propagare unitară și backtracking.

Intrare:

cnf – structura care conține formula CNF.
model – atribuirile curente ale variabilelor.
Pași:

Aplica propagare unitară:
    Dacă propagarea determină o contradicție, returnează false.
Verifică dacă toate variabilele sunt atribuite:
    Dacă da, verifică dacă soluția satisface toate clauzele.
Selectează o variabilă neatribuită și încearcă:
    Atribuie true și verifică recursiv.
    Dacă nu funcționează, atribuie false și verifică recursiv.

Rezultat: Returnează true dacă formula este satisfiabilă, altfel false.

5. print_result(CNF *cnf, bool satisfiable)
Afișează rezultatul algoritmului DPLL.

Intrare:

cnf – structura CNF.
satisfiable – un boolean care indică dacă formula este satisfăcută.

Pași:

Dacă formula este satisfăcută:
    Completează variabilele neatribuite cu false (implicit 0).
    Afișează soluția în format DIMACS (v ... 0).
Dacă nu este satisfăcută:
    Afișează s UNSATISFIABLE.

Rezultat: Mesajul corespunzător în consola utilizatorului.

6. main(int argc, char *argv[])
Punctul de intrare al programului.

Pași:
Verifică dacă fișierul de intrare este specificat.
Citește formula CNF din fișier.
Inițializează toate variabilele ca neatribuite (-1).
Apelează algoritmul DPLL pentru a determina satisfiabilitatea formulei.
Afișează rezultatul.

Rezumat Structuri:

Structura Clause:

Reprezintă o clauză în formula CNF.
Conține o listă de literali și numărul acestora (size).

Structura CNF:

Reprezintă formula completă în formă normală conjunctivă.
Include un array de clauze, numărul de variabile și numărul de clauze.