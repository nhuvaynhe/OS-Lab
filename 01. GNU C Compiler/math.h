#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>

void save_value_to_file(float result);

float get_value_from_file();

bool is_a_valid_operations(char operator[]);

bool is_a_number_or_ANS(char s[]);

void wait_for_any_key();

float get_number_from_string(char s[], float pre_result);

float get_result(float a, float b, char operator,int *print);

