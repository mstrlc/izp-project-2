#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 *DALA 15.11. +-13:40 -pridana funkce na spocitani velikosti mnoziny a implementace z textoveho souboru "testik.txt"
 *
! Prekladat s argumenty: gcc -std=c99 -Wall -Wextra -Werror setcal.c -o setcal
! Prosim, pokud budete delat upravy, nemazat nic co napsal nekdo jiny. Staci zakomentarovat a napsat, proc jste to udelali.
! Komentare s ! na zacatku jsou pracovni, pro nas at vime, muzou byt v cestine. Normalni komentare bych psal v anglictine, ale to staci udelat dodatecne nakonec.

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

Tipy pro implementaci:
    ✓ Vytvořte datový typ pro množinu. Uvažujte předem neznámý počet prvků. Každý prvek (řetězec) by měl být dynamicky alokován.
    ✓ Implementujte funkci pro tisk množiny.
    - Implementujte funkci pro načtení množiny ze souboru do paměti.
    - Vytvořte základní (unární operace) nad množinou.
    - Vytvořte datový typ pro načtení více množin. Implementujte další operace nad množinami.
    - Vytvořte datový typ pro relaci.
    - Vytvořte funkci pro načtení relace ze souboru do paměti.
    - Implementujte operace nad relacemi.

Poznamky:
    
*/

typedef struct // Definiton of the data type used to store sets
{
    char **elements; // Array of elements stored as strings
    int size;        // Number of elements in set
} set_t;

void elements_in_line(char *path, set_t *set, int line)
{ //counts elementsof concrete line of input file
    FILE *input_file;
    input_file = fopen(path, "r");
    char c;
    int count = 0;
    int i;

    c = fgetc(input_file);
    for (i = 1; i < line; i++)
    {
        for (int j = 0; j != '\n'; j++)
        {
            c = fgetc(input_file);
        }
    }

    c = fgetc(input_file);
    c = fgetc(input_file);
    if (i > 1)
        c = fgetc(input_file);
    while (c != EOF)
    {
        if (c == ' ' || c == '\n')
            count++;
        c = fgetc(input_file);
    }

    fclose(input_file);

    set->size = count;
}

void set_const(set_t *set) //Constructor for sets
{
    set->elements = (char **)malloc(set->size * sizeof(char *));
}

void set_dest(set_t *set) // Destructor for sets
{
    if (set->elements != NULL)
    {
        free(set->elements);
    }
    set->elements = NULL;
    set->size = 0;
}

void set_init(set_t *set) // ! Testovaci funkce na vyplneni mnoziny prvky (text. retezci)
{
    for (int i = 0; i < set->size; i++)
    {
        set->elements[i] = "AAi";
    }
}

void set_print(set_t *set) // Print a given set
{
    printf("S %s", set->elements[0]);   // Print the first element
    for (int i = 1; i < set->size; i++) // Print second through penultimate elements
    {
        printf(" %s", set->elements[i]);
    }
    printf("\n");
}

void print_line_in_file(char *path, int line)
{
    FILE *input_file;
    input_file = fopen(path, "r");
    char c = 0;

    for (int i = 0; i < line; i++)
    {
        for (int j = 0; c != '\n'; j++)
        {
            c = fgetc(input_file);
        }
    }

    c = fgetc(input_file);
    while (c != '\n')
    {
        printf("%c", c);
        c = fgetc(input_file);
    }
    printf("\n");

    fclose(input_file);
}

void print_file(char *path) // ! Nacte a vypise vstupni soubor
{
    FILE *input_file;
    input_file = fopen(path, "r");
    char c = 0;

    c = fgetc(input_file);
    while (c != -1)
    {
        printf("%c", c);
        c = fgetc(input_file);
    }

    fclose(input_file);
}

/*
 !nejspis nebude potreba, ale necham to tu
void chars_in_line(char *path, set_t *set, int line) // ! Nacte pocet znaku (bez mezer) daneho radku ze vstupniho souboru
{
    FILE *input_file;
    input_file = fopen(path, "r");
    char c;
    int count=0;

    c = fgetc(input_file);
    for(int i=1; i<line; i++){
        for(int j=0; j!='\n'; j++){
            c=fgetc(input_file);
        }
    }
    
    c=fgetc(input_file);
    c=fgetc(input_file);
    while (c != '\n')
    {
        if(c!=' ') count++;
        c = fgetc(input_file);
    }

    fclose(input_file);

    set->size=count;
}
*/

int main(int argc, char **argv)
{
    set_t universe;

    print_file("testik.txt");
    elements_in_line("testik.txt", &universe, 1);
    set_const(&universe); // ! druhy argument konstruktoru je velikost mnoziny univerza, musi se napsat funkce ktera ten radek z textoveho souboru prelozi
    set_init(&universe);

    printf("---\n");

    print_line_in_file("sets.txt", 1);

    printf("Program path: %s, number of arguments: %d\n", argv[0], argc); // ! jen aby se to zkompilovalo, kdyz to tu neni tak to krici
    // char *univ_line = "U a b c f d x y z\n"; // ! testovaci radek, stejny jako to co se dostane ze souboru

    // ! je potreba napsat funkci, ktera by vzala ten radek a ty informace z neho ulozila do mnoziny universe (pocet prvku a prvky)
    // ! - prozatim bych to testoval ze stringu univ_line, nacitani primo ze souboru bych udelal az pak

    return 0;
}
