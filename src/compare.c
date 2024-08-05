#include "compare.h"
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/evp.h>

#define BUFFER_SIZE 8192


// Calculate the hash of a given file
char* getFileHash(const char *filePath) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned char buff[BUFFER_SIZE];
    unsigned int hash_length;
    const EVP_MD *md_type = EVP_md5();

    // Attempt to open the provided file
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) {
        printf("Cannot open file: %s\n", filePath);
        return NULL;
    }

    // Allocate a digest context
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        printf("Failed to create EVP_MD_CTX\n");
        return NULL;
    }

    // Set up context to use md5
    if (EVP_DigestInit_ex(mdctx, md_type, NULL) != 1) {
        printf("EVP_DigestInit_ex failed\n");
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        return NULL;
    }

    int bytes;
    // Read file - hash bytes of data into the context
    while ((bytes = fread(buff, 1, BUFFER_SIZE, file)) != 0) {
        if (EVP_DigestUpdate(mdctx, buff, bytes) != 1) {
            perror("EVP_DigestUpdate failed");
            EVP_MD_CTX_free(mdctx);
            fclose(file);
            return NULL;
        }
    }

    // Read final hash value
    if (EVP_DigestFinal_ex(mdctx, hash, &hash_length) != 1) {
        perror("EVP_DigestFinal_ex failed");
        EVP_MD_CTX_free(mdctx);
        fclose(file);
        return NULL;
    }

    // Free the EVP context
    EVP_MD_CTX_free(mdctx);
    fclose(file);

    // Convert each string byte into hexadecimal representation
    char *hash_string = malloc(2 * hash_length + 1);
    for (unsigned int i = 0; i < hash_length; i++) {
        sprintf(&hash_string[i * 2], "%02x", hash[i]);
    }
    return hash_string;
}


// Create hashes for all files in a given directory
FileData* readFiles(const char *directory, size_t *file_count) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    dir = opendir(directory);
    if (dir == NULL) {
        printf("Unable to open directory: %s\n", directory);
        exit(1);
    }

    // Create array to hold file metadata
    FileData *files = NULL;
    *file_count = 0;
    size_t capacity = 10;
    files = malloc(sizeof(FileData) * capacity);

    while ((entry = readdir(dir)) != NULL) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

        // Check if file exists and is an actual (REG) file
        if (stat(path, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            if (*file_count == capacity) {
                capacity *= 2;
                files = realloc(files, sizeof(FileData) * capacity);
            }

            // Store the file metadata
            files[*file_count].path = strdup(path);
            files[*file_count].hash = getFileHash(path);
            (*file_count)++;
        }
    }
    closedir(dir);
    return files;
}


// Compare file hashes of all files in 2 directories and write outputs
void compareDirectories(const char *dirA, const char *dirB, const char *outputDir) {
    size_t a_count, b_count;

    // Collect metadata for all files in each directory
    FileData *filesA = readFiles(dirA, &a_count);
    FileData *filesB = readFiles(dirB, &b_count);

    // Output files
    FILE *common = fopen(strcat(strdup(outputDir), "/common"), "w");
    FILE *a_only = fopen(strcat(strdup(outputDir), "/a_only"), "w");
    FILE *b_only = fopen(strcat(strdup(outputDir), "/b_only"), "w");

    // Iterate through files in directory A
    for(size_t i = 0; i < a_count; i++) {
        int found = 0;
        for (size_t j = 0; j < b_count; j++) {
            if (strcmp(filesA[i].hash, filesB[j].hash) == 0) {
                // Matching hash found in directory B
                fprintf(common, "%s : %s\n", filesA[i].path, filesB[j].path);
                found = 1;
                break;
            }
        }
        if (!found) {
            // No matching hash found in directory B
            fprintf(a_only, "%s\n", filesA[i].path);
        }
    }

    // Iterate through files in directory B
    for (size_t i = 0; i < b_count; i++) {
        int found = 0;
        for (size_t j = 0; j < a_count; j++) {
            if (strcmp(filesB[i].hash, filesA[j].hash) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(b_only, "%s\n", filesB[i].path);
        }
    }

    // Cleanup
    fclose(common);
    fclose(a_only);
    fclose(b_only);
    freeFileData(filesA, a_count);
    freeFileData(filesB, b_count);
}

// Free struct and associated metadata fields
void freeFileData(FileData *files, size_t file_count) {
    for (size_t i = 0; i < file_count; i++) {
        free(files[i].path);
        free(files[i].hash);
    }
    free(files);
}