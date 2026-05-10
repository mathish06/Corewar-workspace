/*
** EPITECH PROJECT, 2025
** my_getnbr
** File description:
** my_getnbr
*/

int my_getnbr(char const *str)
{
    int sign = 1;
    long result = 0;

    while (*str == '-' || *str == '+') {
        if (*str == '-')
            sign = -sign;
        str++;
    }
    while (*str <= '9' && *str >= '0') {
        result = result * 10 + (*str - '0');
        if ((result * sign > 2147483647) || (result * sign < -2147483648))
            return 0;
        str++;
    }
    result *= sign;
    return (int)result;
}
