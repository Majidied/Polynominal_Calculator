#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

int main() {
    char buffer[MAX_LENGTH];
    char *cmd, *key, *poly;

    printf("Enter a string: ");
    while (1)
    {
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Allocate memory for cmd, key, and poly
        cmd = malloc(MAX_LENGTH);
        key = malloc(MAX_LENGTH);
        poly = malloc(MAX_LENGTH);

        // Check if memory allocation is successful
        if (cmd == NULL || key == NULL || poly == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }

        // Parse the input
        if (sscanf(buffer, "LET %[^=] = %[^\n]", key, poly) == 2) {
            // Print the parsed values
            printf("Key: %s\n", key);
            printf("Poly: %s\n", poly);
        } else {
            printf("Invalid input format\n");
        }

        // Free allocated memory
        free(cmd);
        free(key);
        free(poly);

    } else {
        perror("Failed to read input");
    }
    }
    return 0;
}
