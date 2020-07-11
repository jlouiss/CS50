from sys import exit


def get_card_type(card_number):
    card_type = "INVALID"

    if ((card_number >= 340000000000000 and card_number < 350000000000000)
            or (card_number >= 370000000000000 and card_number < 380000000000000)):
        card_type = "AMEX"
    elif ((card_number >= 4000000000000000 and card_number < 5000000000000000)
          or (card_number >= 4000000000000 and card_number < 5000000000000)):
        card_type = "VISA"
    elif (card_number >= 5100000000000000 and card_number < 5600000000000000):
        card_type = "MASTERCARD"

    if card_type == "INVALID":
        print(card_type)
        exit(1)

    return card_type


def validate_card(card_number, card_type):
    card_number_copy = card_number
    primary_digits_sum = 0
    secondary_digits_sum = 0
    index = 1

    """
    Loop through the digits by dividing by 10 at each step, then add the digit
    to the respective sum
    """
    while card_number_copy > 1:
        current_digit = card_number_copy % 10

        if index % 2 != 0:
            secondary_digits_sum += current_digit
        else:
            number = current_digit * 2

            if number < 10:
                primary_digits_sum += number
            else:
                # sum double digit number's digits
                remainder = number % 10
                primary_digits_sum += remainder
                primary_digits_sum += (number - remainder) / 10

        card_number_copy = (card_number_copy - current_digit) / 10
        index += 1

    if (primary_digits_sum + secondary_digits_sum) % 10 != 0:
        card_type = "INVALID"


def main():
    # loop until input is integer
    while True:
        card_number = input("Number: ")
        if card_number.isdigit():
            card_number = int(card_number)
            break

    card_type = get_card_type(card_number)
    validate_card(card_number, card_type)

    print(card_type)

    exit(0 if card_type != "INVALID" else 1)


main()