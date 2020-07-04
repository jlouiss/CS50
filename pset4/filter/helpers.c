#include <math.h>
#include "helpers.h"
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // transform each rgb value to the average of the three values
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int average = round((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0);
            image[h][w].rgbtBlue = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtRed = average;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // flip rows
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            RGBTRIPLE temp;
            temp.rgbtBlue = image[h][w].rgbtBlue;
            temp.rgbtGreen = image[h][w].rgbtGreen;
            temp.rgbtRed = image[h][w].rgbtRed;

            image[h][w].rgbtBlue = image[h][width - w - 1].rgbtBlue;
            image[h][w].rgbtGreen = image[h][width - w - 1].rgbtGreen;
            image[h][w].rgbtRed = image[h][width - w - 1].rgbtRed;

            image[h][width - w - 1].rgbtBlue = temp.rgbtBlue;
            image[h][width - w - 1].rgbtGreen = temp.rgbtGreen;
            image[h][width - w - 1].rgbtRed = temp.rgbtRed;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_copy[h][w].rgbtBlue = image[h][w].rgbtBlue;
            image_copy[h][w].rgbtGreen = image[h][w].rgbtGreen;
            image_copy[h][w].rgbtRed = image[h][w].rgbtRed;
        }
    }

    // transform each pixel to the average of surrounding pixels (3x3)
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int blue_sum = 0;
            int green_sum = 0;
            int red_sum = 0;
            int pixel_count = 0;

            for (int h1 = -1; h1 < 2; h1++)
            {
                for (int w1 = -1; w1 < 2; w1++)
                {
                    if ((h + h1 < 0 || h + h1 > height - 1) || (w + w1 < 0 || w + w1 > width - 1))
                    {
                        continue;
                    }

                    blue_sum += image_copy[h + h1][w + w1].rgbtBlue;
                    green_sum += image_copy[h + h1][w + w1].rgbtGreen;
                    red_sum += image_copy[h + h1][w + w1].rgbtRed;
                    pixel_count++;
                }
            }

            image[h][w].rgbtBlue = round((float)blue_sum / pixel_count);
            image[h][w].rgbtGreen = round((float)green_sum / pixel_count);
            image[h][w].rgbtRed = round((float)red_sum / pixel_count);
        }
    }

    return;
}

int calculate_final_pixel_value(int gx_value, int gy_value)
{
    return round(fmin(sqrt(pow(gx_value, 2) + pow(gy_value, 2)), 255.0));
}

/**
 * Detect edges by applying the Sobel operator
 * Transform pixels using 2 kernels (Gx, Gy) per RGB channel
 * The final value of each pixel will be the weighted sum
 * of the values around the pixel in a 3x3 grid,
 * calculated with sqrt(Gx^2 + Gy^2)
 */
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // gx weights, swap columns with rows for gy
    int weights[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    RGBTRIPLE image_copy[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_copy[h][w].rgbtBlue = image[h][w].rgbtBlue;
            image_copy[h][w].rgbtGreen = image[h][w].rgbtGreen;
            image_copy[h][w].rgbtRed = image[h][w].rgbtRed;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int bgx = 0;
            int ggx = 0;
            int rgx = 0;
            int bgy = 0;
            int ggy = 0;
            int rgy = 0;

            for (int h1 = -1, weight_i = 0; h1 < 2; h1++, weight_i++)
            {
                for (int w1 = -1, weight_j = 0; w1 < 2; w1++, weight_j++)
                {
                    // assume black pixel (0, 0, 0) when out of bounds
                    if ((h + h1 < 0 || h + h1 > height - 1) || (w + w1 < 0 || w + w1 > width - 1))
                    {
                        continue;
                    }

                    bgx += image_copy[h + h1][w + w1].rgbtBlue * weights[weight_i][weight_j];
                    ggx += image_copy[h + h1][w + w1].rgbtGreen * weights[weight_i][weight_j];
                    rgx += image_copy[h + h1][w + w1].rgbtRed * weights[weight_i][weight_j];

                    bgy += image_copy[h + h1][w + w1].rgbtBlue * weights[weight_j][weight_i];
                    ggy += image_copy[h + h1][w + w1].rgbtGreen * weights[weight_j][weight_i];
                    rgy += image_copy[h + h1][w + w1].rgbtRed * weights[weight_j][weight_i];
                }
            }

            image[h][w].rgbtBlue = calculate_final_pixel_value(bgx, bgy);
            image[h][w].rgbtGreen = calculate_final_pixel_value(ggx, ggy);
            image[h][w].rgbtRed = calculate_final_pixel_value(rgx, rgy);
        }
    }

    return;
}
