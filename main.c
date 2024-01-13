#include "Poly_project.h"

int main(){
    char *buffer;
    int i;
    int size;

    buffer =malloc(256);
    hash_table_t *table = poly_table_create(10);
    while(0)
    {
        size = read(0, buffer, 256);
        if (size >= 0)
            buffer[size] = '\0';
        if (strcmp(buffer,"EXIT\n")==0) break;
        else if (strncmp(buffer, "LET ", 4) ==0)
            poly_table_set(table, buffer + 4);
        poly_table_print(table, "P");
    }
    poly_node_t *poly = create_polynome("=X+4");
}