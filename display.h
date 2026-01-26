#ifndef DISPLAY_H
#define DISPLAY_H

#include "types.h"

/* Display a single movie with all details (including description) */
void displayMovieDetails(const Movie* movie);

/* Display movies in table format (without description) */
void displayMoviesTable(const MovieDatabase* db, const int* indices, int count, int usePagination);

/* Display all movies in table format with sorting options */
void listAllMovies(MovieDatabase* db);

/* Display search results in table format (sorted by title) */
void displaySearchResults(const MovieDatabase* db, const int* indices, int count);

/* Helper function to format and print table row */
void printMovieRow(const Movie* movie);

/* Print table header */
void printTableHeader(void);

#endif /* DISPLAY_H */
