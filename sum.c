/* sum.c (Roland Teodorowitsch)
 * Compilation: gcc sum.c -o sum -fopenmp
 * Adapted from: http://lspd.mackenzie.br/marathon/06/sum.zip
 */

#include <stdio.h>
#include <string.h>
#include <omp.h>

#define START  100000
#define STEP   100000
#define END   1000000

#define DIGITS 100

void sum(char* output, const long unsigned int d, const long unsigned int n) {
    long unsigned int digit, i, remainder, div, mod;
    long unsigned int digits[d + 11];
    for (digit = 0; digit < d + 11; ++digit) {
        digits[digit] = 0;
    }
    
    #pragma omp parallel for private(i, remainder, div, mod) shared(digits)
    for (i = 1; i <= n; ++i) {
        remainder = 1;
        for (digit = 0; digit < d + 11 && remainder; ++digit) {
            div = remainder / i;
            mod = remainder % i;
            #pragma omp atomic
            digits[digit] += div;
            remainder = mod * 10;
        }
    }

    for (i = d + 11 - 1; i > 0; --i) {
        digits[i - 1] += digits[i] / 10;
        digits[i] %= 10;
    }

    if (digits[d + 1] >= 5) {
        ++digits[d];
    }

    sprintf(output, "%lu.", digits[0]);
    unsigned long int t = strlen(output);
    for (i = 1; i <= d; ++i)
        output[t++] = digits[i] + '0';
    output[t] = '\0';
}

int main() {
    long unsigned int n;
    double start, finish;
    char output[DIGITS + 10]; // extra chars to avoid error
    start = omp_get_wtime();
    for (n=START; n<=END; n+=STEP) {
        sum(output, DIGITS, n);
          
        fprintf(stdout,"%s\n",output);
        
    }
    finish = omp_get_wtime();
    fprintf(stderr,"Tempo utilizado: %lu %lf\n",n,finish-start);
    return 0;
}