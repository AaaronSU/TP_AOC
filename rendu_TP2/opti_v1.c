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


// Pseudo-randomly generates 'n' values and writes them to a text file
int generate_random_values (const char *file_name, unsigned nx, unsigned ny)
{
   printf ("Generate %u x %u values and dump them to %s...\n", nx, ny, file_name);

   // Open/create output file
   FILE *fp = fopen (file_name, "w");
   if (!fp) {
      fprintf (stderr, "Cannot write %s\n", file_name);
      return -1;
   }

   // Save nx and ny on the first line
   if (fprintf (fp, "%u %u\n", nx, ny) <= 0)
      return -2;

   // Generate values (one per line)
   unsigned i, j;
   for (i=0; i<nx; i++) {
      for (j=0; j<ny; j++) {
         const float v1 = (float) rand() / RAND_MAX;
         const float v2 = (float) rand() / RAND_MAX;

         if (fprintf (fp, "%lf %lf\n", v1, v2) <= 0)
            return -2;
      }
   }

   // Close output file
   fclose (fp);

   return 0;
}

// Loads values from a file written by generate_random_values() to the grid
int load_values (const char *file_name, val_grid_t *val_grid)
{
   printf ("Load values from %s...\n", file_name);

   // Open input file (containing one coordinate per line)
   FILE *fp = fopen (file_name, "r");
   if (!fp) {
      fprintf (stderr, "Cannot read %s\n", file_name);
      return -1;
   }

   char buf [100];

   // Load grid size from input file (first line)
   unsigned nx, ny;
   if (fgets (buf, sizeof buf, fp) != NULL &&
       sscanf (buf, "%u %u", &nx, &ny) != 2) {
      fprintf (stderr, "Failed to parse the first line from the input file\n");
      fclose (fp);
      return 1;
   }

   // Update output array length
   val_grid->nx = nx;
   val_grid->ny = ny;
   val_grid->v1_list = (float *)malloc(val_grid->nx * val_grid->ny * sizeof(float));
   val_grid->v2_list = (float *)malloc(val_grid->nx * val_grid->ny * sizeof(float));

   // Load pairs from input file (one per line)
   unsigned nb_inserted_values = 0;
   while (fgets (buf, sizeof buf, fp) != NULL) {
      // Parse current line (v1, v2)
      float v1, v2;
      if (sscanf (buf, "%f %f", &v1, &v2) != 2) {
         fprintf (stderr, "Failed to parse a line from the input file\n");
         fclose (fp);
         return 1;
      }

      val_grid->v1_list[nb_inserted_values] = v1;
      val_grid->v2_list[nb_inserted_values] = v2;
      nb_inserted_values++;
   }
   // Close input file
   fclose (fp);
   

   if (nb_inserted_values != (nx * ny)) {
      fprintf (stderr, "Mismatch between the number of parsed values (%u) and the grid size (%u x %u)\n",
               nb_inserted_values, nx, ny);
      return 1;
   }

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
