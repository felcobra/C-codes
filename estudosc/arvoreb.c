
#include <stddef.h>

typedef struct No {
    int m; //qt de chaves armazenadas no nó
    struct No *pont_pai; // ponteiro pro nó pai
    int *s; //array de chaves
    struct No **p;  //ponteiro para array de pt pros filhos
}Tno;
Tno *busca(Tno *no, int chave) {
    if (no != NULL) {
        int i = 0;
        while (i < no->m && chave  > no->s[i]) {
            i++;
        }
        if (i<no->m && chave == no->s[i]) {
            return no; //encontrou a chave
        }
        else if (no->p[i] != NULL) {
            return busca(no->p[i],chave);
        }
        else return no;
    } else return NULL;
}

int main() {
    return 0;
}
