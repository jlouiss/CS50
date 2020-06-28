#include <cs50.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    long card_number = get_long("Number: ");
    string card_type = "INVALID";

    // Calculate the pre-check type
    if ((card_number >= 340000000000000 && card_number < 350000000000000)
        || (card_number >= 370000000000000 && card_number < 380000000000000))
    {
        card_type = "AMEX";
    }
    else if ((card_number >= 4000000000000000 && card_number < 5000000000000000)
             || (card_number >= 4000000000000 && card_number < 5000000000000))
    {
        card_type = "VISA";
    }
    else if (card_number >= 5100000000000000 && card_number < 5600000000000000)
    {
        card_type = "MASTERCARD";
    }

    // Validate number
    if (strcmp(card_type, "INVALID") != 0)
    {
        int primary_digits_sum = 0;
        int secondary_digits_sum = 0;
        long card_number_copy = card_number;
        int index = 1;
        /**
         * Loop through the digits by dividing by 10 at each step, then
         * add the digit to the respective sum
         */
        while (card_number_copy > 1)
        {
            int current_digit = card_number_copy % 10;

            if (index % 2)
            {
                secondary_digits_sum += current_digit;
            }
            else
            {
                int number = current_digit * 2;

                if (number < 10)
                {
                    primary_digits_sum += number;
                }
                else // sum double digit number's digits
                {
                    int remainder = number % 10;
                    primary_digits_sum += remainder;
                    primary_digits_sum += (number - remainder) / 10;
                }
            }

            card_number_copy = (card_number_copy - current_digit) / 10;
            index++;
        }

        if ((primary_digits_sum + secondary_digits_sum) % 10)
        {
            card_type = "INVALID";
        }
    }

    printf("%s\n", card_type);
}
