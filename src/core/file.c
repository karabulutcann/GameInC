#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "core/types.h"

#define READ_CHUNK_SIZE 1024

void fileRead(const char *path, char **buffer)
{
    ASSERT(path && buffer, "Invalid arguments");

	*buffer = NULL;

	FILE *fp = fopen(path, "rb");
	ASSERT(fp && !ferror(fp), "Error opening file: %s\n", path);

	char *tmp;
	size_t used = 0;
	size_t size = 0;
	size_t n;

	while (true) {
		if (used + READ_CHUNK_SIZE + 1 > size) {
			size = used + READ_CHUNK_SIZE + 1;

			if (size <= used) {
				free(*buffer);
				PANIC("File too large: %s\n", path);
			}

			tmp = realloc(  *buffer, size);
			if (!tmp) {
				free(*buffer);
				PANIC("Memory allocation failed: %s\n", path);
			}
            *buffer = tmp;
		}

		n = fread(*buffer + used, 1, READ_CHUNK_SIZE, fp);
		if (n == 0)
			break;

		used += n;
	}

	if (ferror(fp)) {
		free(*buffer);
		PANIC("Error reading file: %s\n", path);
	}

	tmp = realloc(*buffer, used + 1);
	if (!tmp) {
		free(*buffer);
		PANIC("Memory allocation failed: %s\n", path);
	}
    *buffer = tmp;
    (*buffer)[used] = 0;

    fclose(fp);
}


// struct Result fileWrite(const char *path, char *buffer)
// {
// 	if(!path || !buffer)
// 	{
// 		return mErr("Invalid arguments");
// 	}

//     FILE *fp = fopen(path, "wb");
//     if (!fp || ferror(fp)) {
//         return mErr(format("Error opening file: %s\n", path));
//     }

// 	mDebug("Wrote file: %s\n", path);

//     size_t n = fwrite(buffer, 1, strlen(buffer), fp);
//     if (n != strlen(buffer)) {
//         return mErr(format("Error writing file: %s\n", path));
//     }

//     fclose(fp);
//     return ok();
// }
