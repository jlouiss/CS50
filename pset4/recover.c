#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

bool has_jpeg_header(BYTE *buffer);

// recover jpegs from the specified file and extract them into separate files
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    // initial assumption is that the images are stored in a FAT system
    // so we can safely read 512 bytes at a time since the images will be
    // aligned to a block
    FILE *input_file = fopen(argv[1], "r");

    if (!input_file)
    {
        return 1;
    }

    int image_count = 0;
    int item_count;
    BYTE buffer[BLOCK_SIZE];
    FILE *image = NULL;

    // whenever a block starts with the JPEG header a new file will be created
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, input_file))
    {
        if (has_jpeg_header(buffer))
        {
            // an image was open
            if (image != NULL)
            {
                fclose(image);
            }

            // create nnn.jpg\0
            char *filename = malloc(sizeof(char) * 8);
            sprintf(filename, "%03i.jpg", image_count);
            image = fopen(filename, "w");
            free(filename);
            image_count++;
        }

        // write current buffer to nnn.jpg
        if (image != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, image);
        }

        // reached EOF
        if (feof(input_file))
        {
            // close the last image
            if (image != NULL)
            {
                fclose(image);
            }
            break;
        }
    }

    fclose(input_file);

    return 0;
}

bool has_jpeg_header(BYTE *buffer)
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
}
