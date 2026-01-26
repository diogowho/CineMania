#ifndef UTILS_H
#define UTILS_H

#include "types.h"

/* Genre utility functions */
const char* getGenreName(Genre genre);
Genre getGenreFromString(const char* genreStr);
void printGenreList(void);

/* String utility functions */
void trimString(char* str);
void toLowerString(char* dest, const char* src);
int containsSubstring(const char* haystack, const char* needle);

/* Input utility functions */
int readInteger(const char* prompt, int min, int max);
float readFloat(const char* prompt, float min, float max);
void readString(const char* prompt, char* buffer, int maxLength);
int readConfirmation(const char* prompt);

/* Validation functions */
int isValidYear(int year);
int isValidDuration(int duration);
int isValidRating(float rating);
int isValidRevenue(float revenue);

/* Display utility functions */
void clearScreen(void);
void pauseScreen(void);
void printLine(int length);
void printHeader(const char* title);

#endif /* UTILS_H */
