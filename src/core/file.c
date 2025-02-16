#include "file.h"
#include <stdio.h>
#include <malloc.h>
#include "core.h"
#include "result.h"
#include "log.h"

#define READ_CHUNK_SIZE 1024

struct Result fileRead(const char *path, char **buffer)
{
    if(!path || !buffer)
    {
        return mErr("Invalid arguments");
    }

	*buffer = NULL;
	

	FILE *fp = fopen(path, "rb");
	if (!fp || ferror(fp)) {
        return mErr(format("Error opening file: %s\n", path));
	}

	char *tmp;
	size_t used = 0;
	size_t size = 0;
	size_t n;

	while (TRUE) {
		if (used + READ_CHUNK_SIZE + 1 > size) {
			size = used + READ_CHUNK_SIZE + 1;

			if (size <= used) {
				free(*buffer);
				return mErr(format("File too large: %s\n", path));
			}

			tmp = realloc(  *buffer, size);
			if (!tmp) {
				free(*buffer);
				return mErr(format("Memory allocation failed: %s\n", path));
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
		return mErr(format("Error reading file: %s\n", path));
	}

	tmp = realloc(*buffer, used + 1);
	if (!tmp) {
		free(*buffer);
        return mErr(format("Memory allocation failed: %s\n", path));
	}
    *buffer = tmp;
    (*buffer)[used] = 0;

	DEBUG("Read file: %s\n", path);

    fclose(fp);
    return ok();
}

struct Result fileWrite(const char *path, char *buffer){
	if(!path || !buffer)
	{
		return mErr("Invalid arguments");
	}
	
    FILE *fp = fopen(path, "wb");
    if (!fp || ferror(fp)) {
        return mErr(format("Error opening file: %s\n", path));
    }

    size n = fwrite(buffer, 1, strlen(buffer), fp);
    if (n != strlen(buffer)) {
        return mErr(format("Error writing file: %s\n", path));
    }

	DEBUG("Wrote file: %s\n", path);

    fclose(fp);
    return ok();
}