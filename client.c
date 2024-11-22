#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "math.h"  


int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <server_name> <num1> <num2>\n", argv[0]);
        exit(1);
    }

    char *server_name = argv[1];
    int num1 = atoi(argv[2]);
    int num2 = atoi(argv[3]);

    // RPC-Client-Handle erstellen
    CLIENT *cl = clnt_create(server_name, MATHPROG, MATHVERS, "tcp");
    
    // Zahlen in die Struktur intpair packen
    intpair numbers;
    numbers.a = num1;
    numbers.b = num2;

    int *add_result = add_1(&numbers, cl);
    printf("Sum: %d + %d = %d\n", num1, num2, *add_result);

    int *multiply_result = multiply_1(&numbers, cl);
    printf("Product: %d * %d = %d\n", num1, num2, *multiply_result);

    int *cube_result = cube_1(&num1, cl);
    printf("Cube: %d^3 = %d\n", num1, *cube_result);

    return 0;
}
