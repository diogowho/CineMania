#ifndef TYPES_H
#define TYPES_H

/* Maximum capacity constraints */
#define MAX_MOVIES 2000
#define MAX_STRING_LENGTH 256
#define MAX_DESCRIPTION_LENGTH 1024
#define MAX_GENRES_PER_MOVIE 20
#define MAX_ACTORS_PER_MOVIE 50
#define MAX_ACTOR_NAME_LENGTH 100

/* Pagination */
#define LINES_PER_PAGE 25

/* Genre enumeration - corresponds to the 20 genres specified in requirements */
typedef enum {
  GENRE_ACTION,
  GENRE_ADVENTURE,
  GENRE_ANIMATION,
  GENRE_BIOGRAPHY,
  GENRE_COMEDY,
  GENRE_CRIME,
  GENRE_DRAMA,
  GENRE_FAMILY,
  GENRE_FANTASY,
  GENRE_HISTORY,
  GENRE_HORROR,
  GENRE_MUSIC,
  GENRE_MUSICAL,
  GENRE_MYSTERY,
  GENRE_ROMANCE,
  GENRE_SCI_FI,
  GENRE_SPORT,
  GENRE_THRILLER,
  GENRE_WAR,
  GENRE_WESTERN,
  /* GENRE_NONE: Sentinel value for internal use only (array initialisation,
     marking empty slots, error handling in parsing). Not a valid movie genre -
     never shown to users or assigned by them. Functions like NULL for pointers
     or \0 for strings - a programming construct, not domain data. */
  GENRE_NONE
} Genre;

/* Movie structure */
typedef struct {
  int code;                                 /* Unique code (immutable) */
  char title[MAX_STRING_LENGTH];            /* Movie title */
  Genre genres[MAX_GENRES_PER_MOVIE];       /* Array of genres */
  int genreCount;                           /* Number of genres */
  char description[MAX_DESCRIPTION_LENGTH]; /* Movie description */
  char director[MAX_STRING_LENGTH];         /* Director name */
  char actors[MAX_ACTORS_PER_MOVIE][MAX_ACTOR_NAME_LENGTH]; /* Actor names */
  int actorCount; /* Number of actors */
  int year;       /* Release year */
  int duration;   /* Duration in minutes */
  float rating;   /* Rating [0, 10] */
  int favorite;   /* Favorite count */
  float revenue;  /* Revenue in millions */
} Movie;

/* Movie database structure */
typedef struct {
  Movie movies[MAX_MOVIES]; /* Array of movies */
  int count;                /* Current number of movies */
  int nextCode;             /* Next available code for new movies */
} MovieDatabase;

/* Sort order enumeration */
typedef enum { SORT_ASCENDING, SORT_DESCENDING } SortOrder;

/* Search type enumeration */
typedef enum {
  SEARCH_BY_TITLE,
  SEARCH_BY_GENRE,
  SEARCH_BY_DIRECTOR,
  SEARCH_BY_ACTOR
} SearchType;

#endif /* TYPES_H */
