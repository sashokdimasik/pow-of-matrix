#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct Complex {
    double real;
    double imag;
} Complex;

typedef struct Matrix {
    unsigned int size;
    Complex **element;
} Matrix;

typedef struct Configuration {
    Complex base;
    unsigned int matrix_size;
    double precision;
    unsigned int iterations_limit;
    Matrix power;
} Configuration;

Complex complex_sum (Complex a, Complex b) {
    Complex result = {a.real + b.real, a.imag + b.imag};
    return result;
}

Complex complex_mult (Complex a, Complex b) {
    Complex result = {a.real * b.real - a.imag * b.imag, a.real * b.imag + a.imag * b.real};
    return result;
}

Complex complex_div (Complex a, Complex b) {
    Complex result;
    
    if (a.imag == 0 && b.imag == 0) {
	result.real = a.real / b.real;
	result.imag = 0;
    } else {
	double denominator = b.real * b.real + b.imag * b.imag;
	result.real = (a.real * b.real + a.imag * b.imag) / denominator;
	result.imag = (a.imag * b.real - a.real * b.imag) / denominator;
    }
    
    return result;
}

double complex_magnitude (Complex number) {
    return sqrt(number.real*number.real + number.imag*number.imag);
}

Complex complex_log (Complex number) {
    Complex result;

    if (number.imag == 0 && number.real > 0) {
	result.real = log(number.real);
	result.imag = 0;
    } else {
	result.real = log(complex_magnitude(number));
	result.imag = atan2(number.imag, number.real);
    }

    return result;
}

void matrix_alloc (Matrix *matrix, int size) {
    matrix->element = malloc(size * sizeof(Complex*));
    for (int i = 0; i < size; i++)
        matrix->element[i] = malloc(size * sizeof(Complex));
    
    matrix->size = size;
}

void matrix_free (Matrix *matrix) {
    for (int i = 0; i < matrix->size; i++)
	free(matrix->element[i]);

    free(matrix->element);
}

int input_from_file (Configuration *config, FILE *fin) {
    if (fscanf(fin, "%lf,%lf",
	       &(config->base.real),
	       &(config->base.imag)) != 2) {
	printf("[ERROR] Base must contain real and imaginary parts separated by comma!");
	return -1;
    }
    
    if (config->base.real == 0 && config->base.imag == 0) {
        printf("[ERROR] Not defined for the base of zero!");
        return -1;
    }
    
    if (fscanf(fin, " - %lf - %u - %u",
	       &(config->precision),
	       &(config->iterations_limit),
	       &(config->matrix_size)) != 3) {
	printf("[ERROR] Please provide precision, iterations limit, power matrix' size and power matrix!\n[...] Before each parameter write minus sign \"-\".");
	return -1;
    }

    if (config->iterations_limit < 3) {
	printf("[ERROR] Iterations limit must be greater than \"3\"!");
	return -1;
    }

    if (config->matrix_size < 1) {
	printf("[ERROR] Matrix size can't be less than \"1\"!");
	return -1;
    }

    matrix_alloc(&(config->power), config->matrix_size);

    for (int i = 0; i < config->matrix_size; i++) {
        for (int j = 0; j < config->matrix_size; j++) {
	    int read_elements_count = fscanf(fin, "%lf,%lf",
					     &(config->power.element[i][j].real),
					     &(config->power.element[i][j].imag));
	    
	    if (read_elements_count < 2) {
		if (read_elements_count == EOF) {
		    printf("[ERROR] Provided matrix lacks %d elements!",
			   config->matrix_size * (config->matrix_size - i) - j);
		} else {
		    printf("[ERROR] Error occured while reading power matrix at [row = %d, column =  %d]\n", i + 1, j + 1);
		    
		    if (read_elements_count == 1) {
			printf("[...] Imaginary part is not provided!\n");
		    } else {
			printf("[...] Either the current element [%d, %d] lacks real part, or the previous one [%d, %d] lacks imaginary part!\n", i + 1, j + 1, i + 1 - (j - 1 == -1), (j - 1 == -1) ? config->matrix_size : j);
		    }
		    
		    printf("[...] Each element must contain real and imaginary parts separated by comma!\nElements can be separated by space or new line.\n");
		}
		matrix_free(&(config->power));
		return -1;
	    }
        }
    }

    return 0;
}

void matrix_fill (Matrix *matrix, Complex number) {
    for (int i = 0; i < matrix->size; i++)
        for (int j = 0; j < matrix->size; j++) {
            matrix->element[i][j].real = number.real;
	    matrix->element[i][j].imag = number.imag;
	}
}

void matrix_copy (Matrix *dest, Matrix *src) {
    for (int i = 0; i < dest->size; i++)
        memcpy(dest->element[i], src->element[i], sizeof(Complex) * dest->size);
}

void number_to_matrix (Complex number, Matrix *matrix) {
    Complex zero = {0, 0};
    matrix_fill(matrix, zero);
    for (int i = 0; i < matrix->size; i++)
        matrix->element[i][i] = number;
}

void matrix_sum (Matrix *a, Matrix *b) {
    for (int i = 0; i < a->size; i++)
        for (int j = 0; j < a->size; j++)
            a->element[i][j] = complex_sum(a->element[i][j], b->element[i][j]);
}

void matrix_mult_number (Matrix *matrix, Complex number) {
    for (int i = 0; i < matrix->size; i++)
        for (int j = 0; j < matrix->size; j++)
            matrix->element[i][j] = complex_mult(matrix->element[i][j], number);
}

void matrix_div_number (Matrix *matrix, Complex number) {
    for (int i = 0; i < matrix->size; i++)
        for (int j = 0; j < matrix->size; j++)
            matrix->element[i][j] = complex_div(matrix->element[i][j], number);
}

void matrix_mult_matrix (Matrix *a, Matrix *b) {
    Matrix buff;
    Complex zero = {0, 0};
    matrix_alloc(&buff, a->size);
    matrix_fill(&buff, zero);
    
    for (int i = 0; i < a->size; i++)
        for (int j = 0; j < a->size; j++)
            for (int k = 0; k < a->size; k++)
                buff.element[i][j] = complex_sum(buff.element[i][j], complex_mult(a->element[i][k], b->element[k][j]));
    
    matrix_copy(a, &buff);
    matrix_free(&buff);
}

double matrix_compare (Matrix *a, Matrix *b) {
    double max_difference = 0;
    
    for (int i = 0; i < a->size; i++)
        for (int j = 0; j < a->size; j++) {
            double difference_real = fabs(a->element[i][j].real - b->element[i][j].real);
	    double difference_imag = fabs(a->element[i][j].imag - b->element[i][j].imag);
            if (difference_real > max_difference)
                max_difference = difference_real;
	    if (difference_imag > max_difference)
                max_difference = difference_imag;
        }
    
    return max_difference;
}

void pow_of_matrix (Matrix *result, Configuration *config) {
    Matrix curr_el;
    Matrix buff;
    Complex one = {1, 0};
    
    matrix_mult_number(&(config->power), complex_log(config->base));
    
    number_to_matrix(one, result);
    matrix_sum(result, &(config->power));
    
    matrix_alloc(&curr_el, config->matrix_size);
    matrix_copy(&curr_el, &(config->power));
    
    matrix_alloc(&buff, result->size);
    matrix_copy(&buff, result);
    
    int i;
    for (i = 2; i <= config->iterations_limit; i++) {
	Complex curr_step = {i, 0};
        matrix_mult_matrix(&curr_el, &(config->power));
        matrix_div_number(&curr_el, curr_step);
        matrix_sum(result, &curr_el);
        if (config->precision > matrix_compare(result, &buff)) break;
        matrix_copy(&buff, result);
    }
    
    matrix_free(&buff);
    matrix_free(&curr_el);
    
    if (i == config->iterations_limit)
        printf("[Reached iterations limit!] ");
    else
        printf("[Reached desired precision! (%lf)] ", config->precision);
    
    printf("[%d / %d iterations performed]\n", i + 1, config->iterations_limit);
}

void print_matrix (Matrix *matrix) {
    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++) {
	    printf("(");
	    if (fabs(matrix->element[i][j].real) >= 10000) {
		printf("%+.6e", matrix->element[i][j].real);
	    } else {
		printf("%13lf", matrix->element[i][j].real);
	    }
	    printf(",");
	    if (fabs(matrix->element[i][j].imag) >= 10000) {
		printf("%+.6e", matrix->element[i][j].imag);
	    } else {
		printf("%13lf", matrix->element[i][j].imag);
	    }
	    printf(") ");
	}
	
        printf("\n");
    }
}

void fprint_matrix (FILE *fout, Matrix *matrix) {
    for (int i = 0; i < matrix->size; i++) {
        for (int j = 0; j < matrix->size; j++) {
	    fprintf(fout, "(");
	    if (fabs(matrix->element[i][j].real) >= 10000) {
		fprintf(fout, "%+.6e", matrix->element[i][j].real);
	    } else {
		fprintf(fout, "%13lf", matrix->element[i][j].real);
	    }
	    fprintf(fout, ",");
	    if (fabs(matrix->element[i][j].imag) >= 10000) {
		fprintf(fout, "%+.6e", matrix->element[i][j].imag);
	    } else {
		fprintf(fout, "%13lf", matrix->element[i][j].imag);
	    }
	    fprintf(fout, ") ");
	}
	
        fprintf(fout, "\n");
    }
}

int output_to_file_and_free (char *fout_name, Matrix *matrix) {
    FILE *fout;

    if (fout_name == NULL) {
	printf("Enter OUTPUT file name: ");
	fout_name = malloc(256 * sizeof(char));
	scanf("%s", fout_name);
    }
    
    fout = fopen(fout_name, "w");
    
    if (fout == NULL) {
	printf("[ERROR] Couldn't open \"%s\" (OUTPUT file)!", fout_name);
	matrix_free(matrix);
	free(fout_name);
	return -1;
    }

    fprint_matrix(fout, matrix);
    
    matrix_free(matrix);

    printf("\nThe result is saved to \"%s\"!", fout_name);

    free(fout_name);

    return 0;
}

int main (int argc, char **argv) {
    
    Configuration config;
    Matrix result;
    
    FILE *fin;
    char *fin_name;
    char *fout_name = NULL;
    
    if (argc > 1) {
	fin_name = argv[1];
	if (argc > 2) {
	    fout_name = argv[2];
	}
    } else {
	printf("Enter INPUT file name: ");
	fin_name = malloc(256 * sizeof(char));
	scanf("%s", fin_name);
    }

    fin = fopen(fin_name, "r");
    
    if (fin == NULL) {
	printf("[ERROR] Couldn't open \"%s\" (INPUT file)!", fin_name);
	return -1;
    }
    
    free(fin_name);
    
    if (input_from_file(&config, fin) != 0) {
	fclose(fin);
	return -1;
    }

    fclose(fin);
    
    printf("Base: %lf + %lf * i;\nDesired precision: %lf\nPower matrix:\n",
	           config.base.real,
	           config.base.imag,
	           config.precision);
    print_matrix(&(config.power));
    printf("\n");
    
    matrix_alloc(&result, config.matrix_size);

    pow_of_matrix(&result, &config);
    
    matrix_free(&(config.power));

    printf("\nThe result:\n");
    print_matrix(&result);
    
    return output_to_file_and_free(fout_name, &result);
    
}
