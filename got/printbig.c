/*
 *  A function illustrating how to link C code to code generated from LLVM 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// extern void free(void *value);
/*
 * Font information: one byte per row, 8 rows per character
 * In order, space, 0-9, A-Z
 */
static const char font[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x1c, 0x3e, 0x61, 0x41, 0x43, 0x3e, 0x1c, 0x00,
  0x00, 0x40, 0x42, 0x7f, 0x7f, 0x40, 0x40, 0x00,
  0x62, 0x73, 0x79, 0x59, 0x5d, 0x4f, 0x46, 0x00,
  0x20, 0x61, 0x49, 0x4d, 0x4f, 0x7b, 0x31, 0x00,
  0x18, 0x1c, 0x16, 0x13, 0x7f, 0x7f, 0x10, 0x00,
  0x27, 0x67, 0x45, 0x45, 0x45, 0x7d, 0x38, 0x00,
  0x3c, 0x7e, 0x4b, 0x49, 0x49, 0x79, 0x30, 0x00,
  0x03, 0x03, 0x71, 0x79, 0x0d, 0x07, 0x03, 0x00,
  0x36, 0x4f, 0x4d, 0x59, 0x59, 0x76, 0x30, 0x00,
  0x06, 0x4f, 0x49, 0x49, 0x69, 0x3f, 0x1e, 0x00,
  0x7c, 0x7e, 0x13, 0x11, 0x13, 0x7e, 0x7c, 0x00,
  0x7f, 0x7f, 0x49, 0x49, 0x49, 0x7f, 0x36, 0x00,
  0x1c, 0x3e, 0x63, 0x41, 0x41, 0x63, 0x22, 0x00,
  0x7f, 0x7f, 0x41, 0x41, 0x63, 0x3e, 0x1c, 0x00,
  0x00, 0x7f, 0x7f, 0x49, 0x49, 0x49, 0x41, 0x00,
  0x7f, 0x7f, 0x09, 0x09, 0x09, 0x09, 0x01, 0x00,
  0x1c, 0x3e, 0x63, 0x41, 0x49, 0x79, 0x79, 0x00,
  0x7f, 0x7f, 0x08, 0x08, 0x08, 0x7f, 0x7f, 0x00,
  0x00, 0x41, 0x41, 0x7f, 0x7f, 0x41, 0x41, 0x00,
  0x20, 0x60, 0x40, 0x40, 0x40, 0x7f, 0x3f, 0x00,
  0x7f, 0x7f, 0x18, 0x3c, 0x76, 0x63, 0x41, 0x00,
  0x00, 0x7f, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00,
  0x7f, 0x7f, 0x0e, 0x1c, 0x0e, 0x7f, 0x7f, 0x00,
  0x7f, 0x7f, 0x0e, 0x1c, 0x38, 0x7f, 0x7f, 0x00,
  0x3e, 0x7f, 0x41, 0x41, 0x41, 0x7f, 0x3e, 0x00,
  0x7f, 0x7f, 0x11, 0x11, 0x11, 0x1f, 0x0e, 0x00,
  0x3e, 0x7f, 0x41, 0x51, 0x71, 0x3f, 0x5e, 0x00,
  0x7f, 0x7f, 0x11, 0x31, 0x79, 0x6f, 0x4e, 0x00,
  0x26, 0x6f, 0x49, 0x49, 0x4b, 0x7a, 0x30, 0x00,
  0x00, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 0x00,
  0x3f, 0x7f, 0x40, 0x40, 0x40, 0x7f, 0x3f, 0x00,
  0x0f, 0x1f, 0x38, 0x70, 0x38, 0x1f, 0x0f, 0x00,
  0x1f, 0x7f, 0x38, 0x1c, 0x38, 0x7f, 0x1f, 0x00,
  0x63, 0x77, 0x3e, 0x1c, 0x3e, 0x77, 0x63, 0x00,
  0x00, 0x03, 0x0f, 0x78, 0x78, 0x0f, 0x03, 0x00,
  0x61, 0x71, 0x79, 0x5d, 0x4f, 0x47, 0x43, 0x00
};

void printbig(int c)
{
  int index = 0;
  int col, data;
  if (c >= '0' && c <= '9') index = 8 + (c - '0') * 8;
  else if (c >= 'A' && c <= 'Z') index = 88 + (c - 'A') * 8;
  do {
    data = font[index++];
    for (col = 0 ; col < 8 ; data <<= 1, col++) {
      char d = data & 0x80 ? 'X' : ' ';
      putchar(d); putchar(d);
    }
    putchar('\n');
  } while (index & 0x7); 
}


double pow_func(double a, double b)
{return pow(a,b);}

/* grid functions */ 

typedef struct Grid {
   int x;
   int y; 
} grid; 

void *grid_init(int x, int y){
  struct Grid *g = NULL;
  g = malloc(sizeof(struct Grid));
  g->x = x;
  g->y = y; 
  return g;
};

void grid_end(grid *g) {
  free(g);
};

int get_grid_x(grid *g){
  return g->x; 
}; 

int get_grid_y(grid *g){
  return g->y; 
};

/* knight functions */ 

typedef struct Knight {
   int health;
   int x;
   int y;
} knight; 


void *new_knight(int x, int y){
  struct Knight *k = NULL;
  k = malloc(sizeof(struct Knight));
  k->health = 100;
  k->x = x; 
  k->y = y; 
  return k;
};

int get_knight_health(knight *k) {
  return k->health; 
};

int get_knight_x_pos(knight *k) {
  return k->x; 
};

int get_knight_y_pos(knight *k) {
  return k->y; 
};


void knight_die(knight *k) {
  free(k);
}; 

/* knave functions */ 

typedef struct Knave {
   int health;
   int x; 
   int y; 
   int stealth;
   int attack_strength;  
} knave; 

void *new_knave(int x, int y){
  struct Knave *k = NULL;
  k = malloc(sizeof(struct Knave));
  k->health = 100;
  k->x = x;
  k->y = y;
  k-> stealth = 1; 
  k-> attack_strength = 25;
  return k;
};

void knave_die(knave *k) {
  free(k);
}; 



/** getters for knave **/ 
int get_stealth(knave *k) {
  return k->stealth; 
}; 

int get_knave_x_pos(knave *k) {
  return k->x; 
}; 

int get_knave_y_pos(knave *k) {
  return k->y; 
};

int get_knave_health(knave *k) {
  return k->health; 
};


/** setters for knave **/ 
void *set_stealth(knave *k, int val) {
  k->stealth = val; 
  return k; 
}; 

void *move_knave(knave *k, int x, int y){
  k->x = k->x+x;
  k->y = k->y+y;
  return k;
}

/** interactors for knave **/
knight *attack_knight(knave *k, knight *a){
  a->health = a->health - k->attack_strength;
  return a;
}


#ifdef BUILD_TEST
int main()
{
  char s[] = "HELLO WORLD09AZ";
  char *c;
  for ( c = s ; *c ; c++) printbig(*c);
}
#endif
