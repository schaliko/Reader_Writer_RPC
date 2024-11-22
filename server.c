#include <stdio.h>
#include <rpc/rpc.h>
#include "math.h"

int *add_1_svc(intpair *pair, struct svc_req *rqstp) {
    static int result;
    result = pair->a + pair->b;
    printf("Addition: %d + %d = %d\n", pair->a, pair->b, result);
    return &result;
}

int *multiply_1_svc(intpair *pair, struct svc_req *rqstp) {
    static int result;
    result = pair->a * pair->b;
    printf("Multiplikation: %d * %d = %d\n", pair->a, pair->b, result);
    return &result;
}

int *cube_1_svc(int *num, struct svc_req *rqstp) {
    static int result;
    result = (*num) * (*num) * (*num);
    printf("Kubierung: %d ^ 3 = %d\n", *num, result);
    return &result;
}