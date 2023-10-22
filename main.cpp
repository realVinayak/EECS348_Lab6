#include <iostream>

bool isDigit(char digit);
int *get_matrix(const int size_of_matrix, FILE *fptr);
int **allocate_matrix_space(const int size_of_matrix);
void deallocate_matrix_space(int **matrix, const int size_of_matrix);
int **convert_stream_into_matrix(const int *stream, int size_of_matrix);
void print_matrix(const char *msg, int **matrix, const int size);
int **add_or_sub(int **matrix_a, int **matrix_b, const int size_of_matrix, bool add_or_sub);
int **multiply_matrix(int **matrix_a, int **matrix_b, const int size_of_matrix);

int main() {
    FILE *fptr;
    int size_of_matrix = 0;
    fptr = fopen("input.txt", "r");
    if (fptr == NULL){
        printf("Cannot read file");
        exit(0);
    }
    char buff[5000];
    fgets(buff, 5000, fptr);
    int buff_ptr = 0;
    while (true){
        if (buff[buff_ptr] == '-'){
            buff_ptr++;
            continue;
        }
        if (!isDigit(buff[buff_ptr])) break;
        size_of_matrix += 10*size_of_matrix + (buff[buff_ptr++]-'0');
    }
    if (size_of_matrix < 0){
        printf("Cannot have negative size of matrix");
        exit(0);
    }

    printf("Vinayak Jha\n"
           "Lab#6: Matrix manipulation\n\n");
    int *matrix = get_matrix(size_of_matrix, fptr);

    fclose(fptr);

    int **matrix_a = convert_stream_into_matrix(matrix, size_of_matrix);
    int **matrix_b = convert_stream_into_matrix(matrix + (size_of_matrix*size_of_matrix), size_of_matrix);

    free(matrix);

    print_matrix("Matrix A\n", matrix_a, size_of_matrix);
    printf("\n");
    print_matrix("Matrix B\n", matrix_b, size_of_matrix);
    printf("\n");

    int **sum_matrix = add_or_sub(matrix_a, matrix_b, size_of_matrix, true);
    int **diff_matrix = add_or_sub(matrix_a, matrix_b, size_of_matrix, false);
    int **prod_matrix = multiply_matrix(matrix_a, matrix_b, size_of_matrix);

    print_matrix("Matrix Sum (A+B)\n", sum_matrix, size_of_matrix);
    print_matrix("\nMatrix Product (A*B) \n", prod_matrix, size_of_matrix);
    print_matrix("\nMatrix Difference (A-B)\n", diff_matrix, size_of_matrix);

    deallocate_matrix_space(matrix_a, size_of_matrix);
    deallocate_matrix_space(matrix_b, size_of_matrix);
    deallocate_matrix_space(sum_matrix, size_of_matrix);
    deallocate_matrix_space(diff_matrix, size_of_matrix);
    deallocate_matrix_space(prod_matrix, size_of_matrix);

    return 0;
}

bool isDigit(char digit){
    return digit == '0' || digit == '1' || digit == '2' || digit == '3'
           || digit == '4' || digit == '5' || digit == '6'
           || digit == '7' || digit == '8' || digit == '9';
}

int *get_matrix(const int size_of_matrix, FILE *fptr){
    int *return_matrix = (int*)malloc(2*size_of_matrix*size_of_matrix*sizeof(int));
    int max_size_of_buffer = size_of_matrix*10 // Characters + Negative Sign
                             + (size_of_matrix -1); // Spaces
    char buff[max_size_of_buffer];
    int write_ptr = 0; // Pointer to write the value at
    for(int line = 0; line < size_of_matrix*2; line++){
        for (int clean_buf_idx = 0; clean_buf_idx < max_size_of_buffer; clean_buf_idx++){
            // Clean the buffer
            buff[clean_buf_idx] = '\0';
        }
        fgets(buff, max_size_of_buffer, fptr);
        //printf("Read: %s", buff);
        int ptr = 0;

        while (true){
            bool is_negative = false;
            int number = 0;
            int stops = 0;
            if (buff[ptr] == '\0' || buff[ptr] == '\n' || buff + ptr  == nullptr) break;
            if (buff[ptr] != '-' && !isDigit(buff[ptr])){
                ptr++;
                continue;
            }
            if (buff[ptr] == '-') {
                is_negative = true;
                ptr++;
            }
            while (isDigit(buff[ptr++])){
                int _number = buff[ptr-1] - '0';
                number = number*10*(stops++) + _number;
            }
            fflush(stdout);
            *(return_matrix + (write_ptr++)) = number * (is_negative ? -1 : 1);
        }
    }
    return return_matrix;
}

int **allocate_matrix_space(const int size_of_matrix){
    int **matrix = (int**)malloc(size_of_matrix * sizeof(int*));
    for (int _idx = 0; _idx < size_of_matrix; _idx++)
        matrix[_idx] = (int*)malloc(size_of_matrix*sizeof(int));
    return matrix;
}

void deallocate_matrix_space(int **matrix, const int size_of_matrix){
    for (int _idx = 0; _idx < size_of_matrix; _idx++)
        free(matrix[_idx]);
    free(matrix);
}

int **convert_stream_into_matrix(const int *stream, int size_of_matrix){
    int **matrix = allocate_matrix_space(size_of_matrix);
    for (int stream_idx = 0; stream_idx < size_of_matrix*size_of_matrix; stream_idx++){
        int row_idx = (int) stream_idx / size_of_matrix;
        int col_idx = stream_idx % size_of_matrix;
        matrix[row_idx][col_idx] = stream[stream_idx];
    }
    return matrix;
}

void print_matrix(const char *msg, int **matrix, const int size){
    printf("%s", msg);
    for (int row_idx = 0; row_idx < size; row_idx++){
        for (int col_idx = 0; col_idx < size; col_idx++){
            if (col_idx != 0) printf("\t");
            printf("%d", matrix[row_idx][col_idx]);
        }
        printf("\n");
    }
}

int **add_or_sub(int **matrix_a, int **matrix_b, const int size_of_matrix, bool add_or_sub){
    int **final_matrix = allocate_matrix_space(size_of_matrix);
    for (int row_idx = 0; row_idx < size_of_matrix; row_idx++)
        for (int col_idx = 0; col_idx < size_of_matrix; col_idx++)
            final_matrix[row_idx][col_idx] = matrix_a[row_idx][col_idx] + matrix_b[row_idx][col_idx]*(add_or_sub ? 1 : -1);
    return final_matrix;
}

int **multiply_matrix(int **matrix_a, int **matrix_b, const int size_of_matrix){
    int **final_matrix = allocate_matrix_space(size_of_matrix);
    for (int row_idx = 0; row_idx < size_of_matrix; row_idx++)
        for (int col_idx = 0; col_idx < size_of_matrix; col_idx++){
            int *rows = matrix_a[row_idx];
            final_matrix[row_idx][col_idx] = 0;
            for (int _other_size_col=0; _other_size_col < size_of_matrix; _other_size_col++)
                final_matrix[row_idx][col_idx] += rows[_other_size_col]*matrix_b[_other_size_col][col_idx];
        }
    return final_matrix;
}