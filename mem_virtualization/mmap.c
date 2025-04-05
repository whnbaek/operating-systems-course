#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // For open()
#include <sys/mman.h>   // For mmap()
#include <sys/stat.h>   // For fstat()
#include <unistd.h>     // For close()
#include <string.h>     // For memcpy()
#include <errno.h>      // For error handling

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *filepath = argv[1];

    // Open the file for reading and writing
    // O_RDWR means we want both read and write access
    int fd = open(filepath, O_RDWR);
    if (fd == -1) {
        perror("open");  // Print error if file open fails
        exit(EXIT_FAILURE);
    }

    // Get information about the file, including its size
    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("fstat");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t filesize = sb.st_size;

    // Map the file into memory
    // NULL lets the kernel choose the memory address
    // PROT_READ | PROT_WRITE sets memory protection (read + write)
    // MAP_SHARED means changes in memory will be written to the file
    // fd is the file descriptor we are mapping
    // 0 is the offset into the file (start at beginning)
    char *mapped = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");  // If mapping fails
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Now we can access the file like a normal char array in memory
    // Print the first 64 bytes (or less if file is smaller)
    printf("First few bytes in the file:\n");
    for (size_t i = 0; i < (filesize < 64 ? filesize : 64); i++) {
        putchar(mapped[i]);
    }
    putchar('\n');

    // Let's modify the contents directly in memory
    // This will also update the file on disk (because of MAP_SHARED)
    if (filesize >= 5) {
        memcpy(mapped, "HELLO", 5);  // Overwrite the first 5 bytes
        printf("Modified first 5 bytes to 'HELLO'\n");
    }

    // Unmap the memory before exiting to clean up
    if (munmap(mapped, filesize) == -1) {
        perror("munmap");
    }

    // Close the file descriptor
    close(fd);
    return 0;
}

