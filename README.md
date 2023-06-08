# mini_projet_apr
On considère l’algorithme de multiplication d’une matrice avec un vecteur Y = A·X où A est une matrice de taille (n×n) et X et Y sont des vecteurs de n éléments (n×1).
Si on considère que la matrice A et le vecteur X sont distribués sur les processeurs, Écrire un programme MPI pour chaque scénario suivant :
    1. Chaque processeur a dans sa mémoire un bloc de lignes de A et la le vecteur x,
    Exemple : avec 4 processeurs, la matrice A est partitionnée en 4 blocs de lignes, A0,
    A1, A2, A3, où chaque bloc Ai est de dimensions (n/4)×n . Le bloc Ai sera stocké dans la
    mémoire du processeur Pi.
    2.Chaque processeur a dans sa mémoire un bloc de lignes de A et un bloc de lignes du vecteur X,
    Exemple : avec 4 processeurs, la matrice A est partitionnée en 4 blocs de lignes, A0,
    A1, A2, A3, où chaque bloc Ai est de dimensions (n/4)×n . Le bloc Ai et le vecteur Xi
    seront stockés dans la mémoire du processeur Pi.
    3.Chaque processeur a dans sa mémoire un bloc de lignes et de colonnes de A (Aij) le
    vecteur X est distribué sur √P processeur.
    Exemple : Avec 16 processeurs, la matrice A est partitionnée en 16 blocs de lignes et
    de colonnes, où chaque bloc Aij est de dimensions (n/4)×(n/4)
