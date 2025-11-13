#include <stdlib.h>
#include <stdio.h>

// Formatul intern: prima line contine numarul de linii(m) respectiv numarul de coloane(n)
// exemplu:
//       2 3 10 -20 -50 0 1 40890
//       m n |         ||        |

struct tuple{
    int m;
    int n;
};

void new_matrix(FILE *f, int m, int n);
float get(FILE *f, int i, int j);
void set(FILE *f, int i, int j, float x);
struct tuple* get_size(FILE *f);