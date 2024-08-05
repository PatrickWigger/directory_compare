#include <stddef.h>

// Struct to store file metadata
typedef struct FileData {
    char *path;
    char *hash;
} FileData;

char* getFileHash(const char *filePath);
FileData* readFiles(const char *directory, size_t *file_count);
void compareDirectories(const char *dirA, const char *dirB, const char *outputDir);
void freeFileData(FileData *files, size_t file_count);