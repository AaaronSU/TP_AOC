#include <stdio.h>  // printf, fopen etc.
#include <stdlib.h> // atoi, qsort, malloc etc.

typedef struct {
   float *v1_list;
   float *v2_list;
   unsigned nx, ny;
} val_grid_t;

typedef struct {
   unsigned x, y; // position in the 2D grid
   float val;
} pos_val_t;

size_t sum_bytes;       // Cumulated sum of allocated bytes (malloc, realloc)


// Génère pseudo-aléatoirement 'nx x ny' valeurs et les écrit dans un fichier binaire
int generate_random_values(const char *file_name, unsigned nx, unsigned ny)
{
    printf ("Generate %u x %u values and dump them to %s...\n", nx, ny, file_name);


    size_t count = (size_t) nx * ny;
    // Allocation des deux tableaux pour v1 et v2
    float *v1_list = (float *)malloc(count * sizeof(float));
    float *v2_list = (float *)malloc(count * sizeof(float));
    if (!v1_list || !v2_list) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        free(v1_list);
        free(v2_list);
        return -1;
    }

    // Remplissage des tableaux avec des valeurs aléatoires
    for (size_t k = 0; k < count; k++) {
        v1_list[k] = (float)rand() / RAND_MAX;
        v2_list[k] = (float)rand() / RAND_MAX;
    }

    // Ouverture/création du fichier en mode binaire
    FILE *fp = fopen(file_name, "wb");
    if (!fp) {
        fprintf(stderr, "Impossible d'écrire dans %s\n", file_name);
        free(v1_list);
        free(v2_list);
        return -1;
    }

    // Écriture des dimensions (nx et ny) dans le fichier
    if (fwrite(&nx, sizeof(nx), 1, fp) != 1 ||
        fwrite(&ny, sizeof(ny), 1, fp) != 1)
    {
        fclose(fp);
        free(v1_list);
        free(v2_list);
        return -2;
    }

    // Écriture en binaire de l'intégralité de v1_list, puis de v2_list
    if (fwrite(v1_list, sizeof(float), count, fp) != count ||
        fwrite(v2_list, sizeof(float), count, fp) != count)
    {
        fclose(fp);
        free(v1_list);
        free(v2_list);
        return -2;
    }

    fclose(fp);
    free(v1_list);
    free(v2_list);
    return 0;
}

// Charge les valeurs depuis un fichier binaire écrit par generate_random_values() dans la structure de la grille
int load_values(const char *file_name, val_grid_t *val_grid)
{
    printf ("Load values from %s...\n", file_name);

    // Ouverture du fichier en mode binaire
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        fprintf(stderr, "Impossible de lire %s\n", file_name);
        return -1;
    }

    // Lecture des dimensions de la grille
    unsigned nx, ny;
    if (fread(&nx, sizeof(nx), 1, fp) != 1 ||
        fread(&ny, sizeof(ny), 1, fp) != 1)
    {
        fprintf(stderr, "Échec de la lecture des dimensions\n");
        fclose(fp);
        return -1;
    }

    val_grid->nx = nx;
    val_grid->ny = ny;
    size_t count = (size_t)nx * ny;

    // Allocation des tableaux pour stocker les valeurs
    val_grid->v1_list = (float *)malloc(count * sizeof(float));
    val_grid->v2_list = (float *)malloc(count * sizeof(float));
    if (!val_grid->v1_list || !val_grid->v2_list) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        fclose(fp);
        return -1;
    }

    // Lecture de l'intégralité de v1_list, puis de v2_list
    if (fread(val_grid->v1_list, sizeof(float), count, fp) != count ||
        fread(val_grid->v2_list, sizeof(float), count, fp) != count)
    {
        fprintf(stderr, "Erreur lors de la lecture des données\n");
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}


void free_value_grid(val_grid_t *val_grid){
   free(val_grid->v1_list);
   free(val_grid->v2_list);
   printf ("Free memory allocated for positions+values (%u x %u entries)...\n",
      val_grid->nx, val_grid->ny);
   printf ("Free memory allocated for coordinates (%u x %u entries)...\n",
      val_grid->nx, val_grid->ny);

}

void compute_max(unsigned nx, unsigned ny, float* val_grid, pos_val_t* pos_val){
   int pos = 0;
   float value = 0;
   for (int i=0; i<nx*ny; i++){
      if (value < val_grid[i]){
         value = val_grid[i];
         pos = i;
      }
   }

   pos_val->x = pos/ny;
   pos_val->y = pos%ny;
   pos_val->val = value;
}

// Program entry point: CF comments on top of this file + README
int main (int argc, char *argv[])
{
   // Check arguments number
   if (argc < 4) {
      fprintf (stderr, "Usage: %s <nb repetitions> <nb points X> <nb points Y>\n", argv[0]);
      return EXIT_FAILURE;
   }

   // Read arguments from command line
   unsigned nrep = (unsigned) atoi (argv[1]);
   unsigned nx   = (unsigned) atoi (argv[2]);
   unsigned ny   = (unsigned) atoi (argv[3]);

   // Generate points and save them to a text file named "values.txt"
   const char *input_file_name = "values.txt";
   if (generate_random_values (input_file_name, nx, ny) != 0) {
      fprintf (stderr, "Failed to write %u x %u coordinates to %s\n",
               nx, ny, input_file_name);
      return EXIT_FAILURE;
   }

   sum_bytes = 0;

   // Main part: repeated nrep times
   unsigned r;
   for (r=0; r<nrep; r++) {
      val_grid_t value_grid;
      pos_val_t *pos_v1_max = malloc(sizeof(*pos_v1_max));
      pos_val_t *pos_v2_max = malloc(sizeof(*pos_v2_max));

      // Load coordinates from disk to memory
      if (load_values (input_file_name, &value_grid) != 0) {
         fprintf (stderr, "Failed to load coordinates\n");
         return EXIT_FAILURE;
      }

      // Compute maximum
      printf ("Compute maximum v1...\n");
      compute_max(value_grid.nx, value_grid.ny, value_grid.v1_list, pos_v1_max);
      printf ("Compute maximum v2...\n");
      compute_max(value_grid.nx, value_grid.ny, value_grid.v2_list, pos_v2_max);

      // Print maximum (v1 and v2)
      printf ("Max v1: x=%u, y=%u, v1=%f\n",
              pos_v1_max->x, pos_v1_max->y, pos_v1_max->val);
      printf ("Max v2: x=%u, y=%u, v2=%f\n",
              pos_v2_max->x, pos_v2_max->y, pos_v2_max->val);

      // Free allocated memory
      free_value_grid(&value_grid);
      free(pos_v1_max);
      free(pos_v2_max);
   }

   // Delete text file
   remove (input_file_name);

   return EXIT_SUCCESS;
}
