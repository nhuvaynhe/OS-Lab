#include "math.h"

int main()
{
    float res = 0;
    char a[16];
    char b[16];
    char operator[16];

    float ans = get_value_from_file();
    float lval, rval;

    bool wait_for_new_calculation = true;
    bool input_is_valid = true;
    
    // If math error, not print
    int print = 1;
    int *p = &print;

    while (wait_for_new_calculation) 
    {
        print = 1;
        input_is_valid = true;
        printf(">> ");
        scanf("%s", a);

        // Check if input is first number, ANS or EXIT
        if(!(strcmp(a, "EXIT"))) {
            wait_for_new_calculation = false;
            input_is_valid = false;
        }
        else if (!(is_a_number_or_ANS(a))) {
            printf("SYNTAX ERROR\n");
            input_is_valid = false;
    
            wait_for_any_key();
        }

        if (input_is_valid) {
            scanf("%s %s", operator, b);
            // Handle the case when operator not valid
            if (!(is_a_valid_operations(operator)))
                printf("SYNTAX ERROR\n");
            // Handle the case when b not a number or ANS
            else if (!(is_a_number_or_ANS(b))) {
                printf("SYNTAX ERROR\n");
            } else {
                lval = get_number_from_string(a, ans);
                rval = get_number_from_string(b, ans);
        
                res = get_result(lval, rval, operator[0], p);

                if(print) {
                    ans = res;
                    save_value_to_file(res);

                    if(res == (int)res) 
                        printf("%d\n", (int)res);
                    else
                        printf("%.2f\n", res);
                }
            }
            wait_for_any_key();
        }
    }
    return 0;
}

