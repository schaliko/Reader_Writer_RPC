#include "math.h"
#include <rpc/rpc.h>
#include <stdio.h>

int main(int argc, char *argv[]){
    if(argc != 4){
        fprintf(stderr, "Usage: %s <server_name> <num1> <num2>\n", argv[0]);
        exit(1);
    }

    char *server_name = argv[1];
    intpair nums;
    int *result;
    nums.a = atoi(argv[2]);
    nums.b = atoi(argv[3]);

    CLIENT *c1 = clnt_create(server_name, MATHPROG, MATHVERS, "tcp");
    result = add_1(&nums, c1);
    printf ("Addition: %d + %d = %d\n", nums.a, nums.b,
	   *result);
    result = multiply_1(&nums, c1);
    printf("Multiplikation: %d * %d = %d\n", nums.a, nums.b, *result);
    result = cube_1(&nums.a, c1);
    printf("Kubierung: %d ^ 3 = %d\n", nums.a, *result);
    
}
