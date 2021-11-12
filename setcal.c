#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
! Prekladat s argumenty: gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal

Link na wiki:
https://wis.fit.vutbr.cz/FIT/st/cwk.php.cs?title=Projekt2&csid=779718&id=14723

Vstupni soubor:
1.  Definice univerza - jeden řádek začínající "U " a pokračující mezerou oddělenými prvky,
2.  Definice množin a relací - jeden nebo více řádků začínající "S " nebo "R " a pokračující mezerou oddělenými prvky
    (řádek začínající "S" indikuje definici množiny, "R" slouží pro definici relace),
3.  Příkazy nad množinami a relacemi - jeden nebo více řádků začínající "C " a pokračující identifikátorem příkazu.

Slovnicek:
mnozina = set
relace = relation
prvek = element
*/

typedef struct // Definiton of the data type used to store sets
{
    char *elements[31]; // Maximal length of one element is 30 characters, 31 is used to account for null character
    int size;           // Number of elements in set
} set_t;

int main(int argc, char *argv[])
{

    return 0;
}