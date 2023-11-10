#include "math.h"

void save_value_to_file(float result)
{
    FILE *fp = fopen("last_ans.txt", "w");
    fprintf(fp, "%f", result);
    fprintf(fp, "\n");
    fclose(fp);
}

float get_value_from_file()
{
    FILE *fp;
  
    // retrieve the value from the file.
    fp = fopen("last_ans.txt", "r");
    if (fp == NULL) {
      fp = fopen("last_ans.txt", "w");
      fprintf(fp, "%f", 0.0f);
      fclose(fp);
    }
    else {  
        float value = 0;
        fscanf(fp, "%f", &value);
        fclose(fp);
        
        return value;
    }
}

bool is_a_valid_operations(char operator[])
{
    char valid_operator[] = {'+', '-', 'x', '/', '%'}; 

    if(strlen(operator) > 1)
        return false;
    else {
        for(int i=0; i < strlen(valid_operator); ++i) {
            if(operator[0] == valid_operator[i])
                return true;
        }
    }

    return false;
}

bool is_a_number_or_ANS(char s[]) {
    if (!(strcmp(s, "ANS")))
        return true;
    else {
        for(int i=0; i < strlen(s); ++i) {
            if(!(isdigit(s[i]))) 
                return false;
        }
    }

    return true;
}

void wait_for_any_key() {
    // Clear the input buffer
    int c;
    while ( (c = getchar()) != '\n' && c != EOF ) { }

    // Wait for new input to continue
    printf("\n--------------------------------\n");
    char wait;

    printf("Press any key to continue!\n ");
    //wait = getchar();
    getchar();

    // Clear the terminal screen
    system("clear");
}

float get_number_from_string(char s[], float pre_result) {
    float x;

    if (!(strcmp(s, "ANS")))
        return pre_result;
    else
        sscanf(s, "%f", &x);

    return x;
}

float get_result(float a, float b, char operator, int *print)
{
    float res = 0.0;

    switch(operator) {
        case '+': {
            res = a + b;
            break;
        }
        case '-': {
            res = a - b;
            break;
        }
        case 'x': {
            res = a * b;
            break;
        }
        case '/': {
            if (b == 0) {
                printf("MATH ERROR\n");
                *print = 0;
            }
            else 
                res = a / b;
            break;
        }
        case '%': {
            res = (int) a % (int) b;
            break;
        }
        default: {
            printf("SYNTAX ERROR\n");
            *print = 0;
            break;
        }
    }

    return res;
}
