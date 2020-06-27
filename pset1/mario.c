#include <cs50.h>
#include <string.h>
#include <stdio.h>

void build_pyramid(int height);

int main(void)
{
    int height = 0;
    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }

    build_pyramid(height);
}

void build_pyramid(int height)
{
    for (int i = 0; i < height; i++)
    {
        // left part
        for (int j = 0; j < height; j++)
        {
            if (j >= height - i - 1)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }

        // gap
        printf("  ");

        // right part
        for (int j = 0; j < height; j++)
        {
            if (j <= i)
            {
                printf("#");
            }
        }

        printf("\n");
    }
}
