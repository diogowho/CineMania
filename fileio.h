#ifndef FILEIO_H
#define FILEIO_H

#include "types.h"

/* CSV Import/Export functions */
int importMoviesFromCSV(MovieDatabase *db, const char *filename);
int exportMoviesToCSV(const MovieDatabase *db, const char *filename);

/* Helper functions for CSV parsing */
char *parseCSVField(char **linePtr, char *buffer, int bufferSize);
void replaceCommaWithDot(char *str);
void replaceDotWithComma(char *str);

#endif /* FILEIO_H */
