#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

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

Poznamky:
    Zatim nevim, jak udelat mnozinu stringu, ted to funguje jako mnozina znaku, ale funguje to

*/

typedef struct // Definiton of the data type used to store sets
{
    char *elements; //
    int size;       // Number of elements in set
} set_t;

void set_const(set_t *set, int size) //Construct a set
{
    set->elements = (char *)malloc(size * sizeof(char));
    set->size = size;
}

void vector_dest(set_t *set) // Destruct a set
{
    if (set->elements != NULL)
    {
        free(set->elements);
    }
    set->elements = NULL;
    set->size = 0;
}

void set_init(set_t *set)
{
    for (int i = 0; i < set->size; i++)
    {
        set->elements[i] = 'A' + i;
    }
}

void set_print(set_t *set) // Print a given set using printf
{
    printf("[%c, ", set->elements[0]);      // Print the first element
    for (int i = 1; i < set->size - 1; i++) // Print second through penultimate elements
    {
        printf("%c, ", set->elements[i]);
    }
    printf("%c]\n", set->elements[set->size - 1]); // Print the last element
}

int main()
{
    set_t universe;

    set_const(&universe, 4);
    set_init(&universe);
    set_print(&universe);

    return 0;
}