#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Matrix {
    int size;
    double **element;
} Matrix;

void print_matrix (Matrix *matrix) {
    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++)
            printf("%lf ", matrix->element[i][j]);
        printf("\n");
    }
}

void fprint_matrix (FILE *f, Matrix *matrix) {
    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++)
            fprintf(f, "%lf ", matrix->element[i][j]);
        fprintf(f, "\n");
    }
}

void matrix_alloc (Matrix *matrix, int size) {
    matrix->element = malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++)
        matrix->element[i] = malloc(size * sizeof(double));
    
    matrix->size = size;
}

void matrix_fill (Matrix *matrix, double number) {
    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++) {
            matrix->element[i][j] = number;
        }
    }
}

void number_to_matrix (double number, Matrix *matrix) {
    matrix_fill(matrix, 0);
    for (int i = 0; i < matrix->size; i++)
        matrix->element[i][i] = number;
}

void matrix_sum (Matrix *a, Matrix *b) {
    for (int i = 0; i < a->size; i++)
        for (int j = 0; j < a->size; j++)
            a->element[i][j] += b->element[i][j];
}

void matrix_mult_number (double number, Matrix *matrix) {
    for (int i = 0; i < matrix->size; i++)
        for (int j = 0; j < matrix->size; j++)
            matrix->element[i][j] *= number;
}

void matrix_div_number (Matrix *matrix, double number) {
    for (int i = 0; i < matrix->size; i++)
        for (int j = 0; j < matrix->size; j++)
            matrix->element[i][j] /= number;
}

void matrix_copy (Matrix *dest, Matrix *src) {
    for (int i = 0; i < dest->size; i++)
        memcpy(dest->element[i], src->element[i], sizeof(double) * dest->size);
}

void matrix_mult_matrix (Matrix *a, Matrix *b){
    Matrix buff;
    matrix_alloc(&buff, a->size);
    matrix_fill(&buff, 0);
    
    for (int i = 0; i < a->size; i++)
        for (int j = 0; j < a->size; j++)
            for (int k = 0; k < a->size; k++)
                buff.element[i][j] += a->element[i][k] * b->element[k][j];
    
    matrix_copy(a, &buff);
    free(buff.element);
}

double matrix_compare (Matrix *a, Matrix *b) {
    double max_difference = 0;
    
    for (int i = 0; i < a->size; i++)
        for (int j = 0; j < a->size; j++) {
            double difference = fabs(a->element[i][j] - b->element[i][j]);
            if (difference > max_difference)
                max_difference = difference;
        }
    
    return max_difference;
}

void pow_of_matrix (double base, Matrix *power, Matrix *result, double precision, unsigned int calculations_limit) {
    Matrix curr_el;
    Matrix buff;
    
    matrix_mult_number(log(base), power);
    
    number_to_matrix(1, result);
    matrix_sum(result, power);
    
    matrix_alloc(&curr_el, power->size);
    matrix_copy(&curr_el, power);
    
    matrix_alloc(&buff, result->size);
    matrix_copy(&buff, result);
    
    int i;
    for (i = 2; i < calculations_limit; i++) {
        matrix_mult_matrix(&curr_el, power);
        matrix_div_number(&curr_el, i);
        matrix_sum(result, &curr_el);
        if (precision > matrix_compare(result, &buff)) break;
        matrix_copy(&buff, result);
    }
    
    free(buff.element);
    free(curr_el.element);
    
    if (i == calculations_limit)
        printf("[Reached calculations limit!] ");
    else
        printf("[Reached desired precision! (%lf)] ", precision);
    printf("[%d / %d calculations performed]\n", i, calculations_limit);
}

int main (int argc, char **argv) {
    
    double base;
    int matrix_size;
    Matrix power;
    Matrix result;
    double precision;
    unsigned int calculations_limit;
    
    FILE *fin;
    char *fin_name;
    
    if (argc > 1) {
        fin_name = malloc(strlen(argv[1]) * sizeof(char));
        strcpy(fin_name, argv[1]);
    } else {
        printf("[!] Please provide input file name upon running the prograrm! [!]");
        return -1;
    }
    
    fin = fopen(fin_name, "r");
    if (fin == NULL) {
        printf("[!] Couldn't open '%s' (the input file)! [!]", fin_name);
        return -1;
    }
    
    fscanf(fin, "%lf %d %lf %d", &base, &matrix_size, &precision, &calculations_limit);
    
    if (base == 0) {
        printf("[!] Not defined for the base of '0'! [!]");
        fclose(fin);
        return -1;
    }
    
    if (base < 0) {
        printf("[!] Not defined for the base less then 0! [!]");
        fclose(fin);
        return -1;
    }
    
    matrix_alloc(&power, matrix_size);
    for (int i = 0; i < matrix_size; i++) {
        for (int j = 0; j < matrix_size; j++) {
            if (feof(fin)) {
                printf("[!] Matrix lacks %d elements! [!]", matrix_size * (matrix_size - i) - j);
                fclose(fin);
                return -1;
            }
            fscanf(fin, "%lf", &(power.element[i][j]));
        }
        printf("\n");
    }
    
    fclose(fin);
    free(fin_name);
    
    printf("Base: %lf; Matrix:\n", base);
    print_matrix(&power);
    
    matrix_alloc(&result, matrix_size);
    
    pow_of_matrix(base, &power, &result, precision, calculations_limit);
    
    free(power.element);
    
    printf("The result:\n");
    print_matrix(&result);
    
    FILE *fout;
    fout = fopen("output.txt", "w");
    if (fout != NULL) {
        printf("The result is saved to 'output.txt'!\n");
        fprint_matrix(fout, &result);
        fclose(fout);
    } else
        printf("Couldn't save the result to 'output.txt!\n'");
    
    free(result.element);
    
    return 0;
    
}
