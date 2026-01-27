#ifndef MOVIE_H
#define MOVIE_H

#include "types.h"

/* Database initialisation and management */
void initDatabase(MovieDatabase *db);
void clearAllMovies(MovieDatabase *db);

/* Movie lookup functions */
int findMovieByCode(const MovieDatabase *db, int code);
int movieCodeExists(const MovieDatabase *db, int code);

/* Movie CRUD operations */
int addMovie(MovieDatabase *db, const Movie *movie);
int addMovieInteractive(MovieDatabase *db);
int deleteMovie(MovieDatabase *db, int code);
int editMovie(MovieDatabase *db, int code);

/* Movie data manipulation */
void copyMovie(Movie *dest, const Movie *src);
int getNextAvailableCode(const MovieDatabase *db);

/* Search functions - returns array of indices */
int searchByTitle(const MovieDatabase *db, const char *searchTerm, int *results,
                  int maxResults);
int searchByGenre(const MovieDatabase *db, Genre genre, int *results,
                  int maxResults);
int searchByDirector(const MovieDatabase *db, const char *director,
                     int *results, int maxResults);
int searchByActor(const MovieDatabase *db, const char *actor, int *results,
                  int maxResults);

/* Sorting helpers */
void sortMoviesByCode(MovieDatabase *db, SortOrder order);
void sortMoviesByTitle(int *indices, int count, const MovieDatabase *db);

/* Validation helpers */
int validateMovieData(const Movie *movie);

#endif /* MOVIE_H */
