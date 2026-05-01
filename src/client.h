#ifndef CLIENT_H
#define CLIENT_H

#include "parson.h"
#include "requests.h"
#include "helper.h"

/* 
    number of times times the client can provide 
    invalid input untill the application stops
*/
#define MAX_INPUT_TRIES 3

// reads a word (no spaces), repeats the request in case of an invalid word
char *read_word(char *field);

// reads a sentence (spaces included)
char *read_sentence(char *field);

// reads a natural number, repeats the request in case of an invalid number
char *read_nat_number(char *field);

// reads a float number, repeats the request in case of an invalid number
char *read_float_number(char *field);

#endif