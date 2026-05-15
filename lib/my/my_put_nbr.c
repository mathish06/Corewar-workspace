/*
** EPITECH PROJECT, 2025
** my_put_nbr
** File description:
** display number given as parameter
*/

#include "../../include/corewar.h"

void nbextremum(int nb)
{
    my_putchar(45);
    my_putchar('2');
    my_putchar('1');
    my_putchar('4');
    my_putchar('7');
    my_putchar('4');
    my_putchar('8');
    my_putchar('3');
    my_putchar('6');
    my_putchar('4');
    my_putchar('8');
}

void isextrem(int nb)
{
    int p = 1;
    int tmp;
    int r;

    if (nb < 0) {
        my_putchar(45);
        nb = -nb;
    }
    tmp = nb;
    while (tmp >= 10) {
        tmp /= 10;
        p *= 10;
    }
    while (p > 0) {
        r = nb / p;
        my_putchar(r + 48);
        nb %= p;
        p /= 10;
    }
}

int my_put_nbr(int nb)
{
    if (!(nb == -2147483648)) {
        isextrem(nb);
    } else {
        nbextremum(nb);
    }
    return 0;
}
