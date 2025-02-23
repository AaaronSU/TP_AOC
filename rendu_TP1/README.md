Nous avons obtenu environ 8x d'accélération de scalaire baseline à scalaire opti, il n'y a pas la version parallèle

Initialement, nous avons le baseline suivant:

```c
void kernel(unsigned n, double **const m, double *x, const double y[16])
{
   unsigned i, j, k;
   for (i=0; i<n; i++){
      x[i] = 0;
   }
   for (k=0; k<16; k++){
      for (j=0; j<n; j++){
         for (i=0; i<n; i++){
            x[i] += m[i][j] * y[k];
         }
      }
   }
}
```

En observant, j'ai observé que nous pouvons inverser les boucles, ainsi nous obtenons 

```c
void kernel(unsigned n, double **const m, double *x, const double y[16])
{
    unsigned i, j, k;
    for (i=0; i<n; i++){
      x[i] = 0;
    }
    for (i=0; i<n; i++){
    for (j=0; j<n; j++){
    for (k=0; k<16; k++){
            x[i] += m[i][j] * y[k];
         }
      }
   }
}
```
j'ai changé le struct de matrice m en matrice en 1D pour mieux vectoriser après.

par la suite, d'après le rapport maqao, j'ai observé que le boucle vectorise mal et que entre les boucle i et j, il n'y a pas forcément une dépendance, j'ai ainsi inverser les boucles i et j et changer de x[i] à x[j].

cela permet de bien vectoriser mais l'accès à array a apparement diminuer jusqu'à 33% et le pourcentage de vectorisation est à 75%

