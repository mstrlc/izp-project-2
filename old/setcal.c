#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Nepouzite funkce

/*
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

/*
 *DALA 15.11. +-13:40 -pridana funkce na spocitani velikosti mnoziny a implementace z textoveho souboru "testik.txt"

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
    ✓ Implementujte funkci pro načtení množiny ze souboru do paměti.
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

void set_const(set_t *set, int size) //Constructor for sets
{
    set->elements = malloc(set->size * sizeof(char *));
    set->size = size;
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
        set->elements[i] = "AAA";
    }
}

void set_print(set_t *set, char type) // Print a given set
{
    printf("%c %s", type, set->elements[0]); // Print the first element
    for (int i = 1; i < set->size; i++)      // Print second through last elements
    {
        printf(" %s", set->elements[i]);
    }
    printf("\n");
}

char *get_line_in_file(char *path, int line)
{
    FILE *input_file;
    input_file = fopen(path, "r");

    if (input_file == NULL)
    {
        fprintf(stderr, "Input file could not be opened. Does the file exist and do you have permissions?\nTerminating program.\n");
        exit(1);
    }

    char c = 0;
    char *string_to_return = (char *)malloc(1000);
    int line_index = 0;

    if (line > 0)
    {
        for (int i = 0; line_index < line; i++)
        {
            c = fgetc(input_file);
            if (c == '\n')
            {
                line_index++;
            }
        }
    }

    int i = 0;
    while ((c = fgetc(input_file)) != '\n')
    {
        string_to_return[i] = c;
        i++;
    }
    string_to_return[i] = '\n';

    fclose(input_file);
    return (string_to_return);
    free(string_to_return);
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

int get_set_size(char *string)
{
    int size = 0;
    for (int i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == ' ')
            size++;
    }

    return size;
}

void set_from_line(set_t *set, char *string)
{
    char *substring = (char *)malloc(31);
    int index = 0;

    for (int i = 1; string[i] != '\0'; i++)
    {
        memset(substring, '\0', 31);
        if (string[i] != ' ')
        {
            int j = 0;
            while (string[i] != ' ' && string[i] != '\n')
            {
                substring[j] = string[i];
                i++;
                j++;
            }
            set->elements[index] = (char *)malloc(31); // max length of substring is 30 characters
                                                       // nie som si ista, ci ked destroyneme celu mnozinu, tak sa uvolni aj tato pamat co bola alokovana
            memcpy((set->elements[index]), substring, 31);
            index++;
        }
    }

    free(substring);
}

void command_empty(set_t *set)
{
    if (set->size == 0)
    {
        printf("true\n");
    }
    else
        printf("false\n");
}

void command_card(set_t *set)
{
    printf("%d\n", set->size);
}

void command_complement(set_t *set, set_t *universe)
{
    set_t complement;
    set_const(&complement, (universe->size - set->size));

    int comp_index = 0;

    for (int i = 0; i < universe->size; i++)
    {
        bool found = false;

        for (int j = 0; j < set->size; j++)
        {
            if (strcmp(universe->elements[i], set->elements[j]) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found)
        {
            complement.elements[comp_index] = universe->elements[i];
            comp_index++;
        }
    }

    set_print(&complement, 'S');
    set_dest(&complement);
}

int main(int argc, char **argv)
{
    char *path;

    if (argc == 2)
    {
        path = argv[1];
    }
    else
    {
        fprintf(stderr, "Input file not supplied.\nTerminating program.\n");
        exit(1);
    }
    printf("---\n");

    for (int i = 0; i < 5; i++) // ! Testovaci tisk jak funguji funkce
    {
        printf("radek %d souboru %s, pocet prvku: %d, text: %s", i, path, get_set_size(get_line_in_file(path, i)), get_line_in_file(path, i));
    }

    printf("---\n");

    set_t universe; // ! Vezme univezum a naplni ho ze souboru, vytiskne ho
    set_const(&universe, get_set_size(get_line_in_file(path, 0)));
    set_from_line(&universe, get_line_in_file(path, 0));
    printf("univerzum: ");
    set_print(&universe, 'U');

    set_t mnozina;
    set_const(&mnozina, get_set_size(get_line_in_file(path, 3)));
    set_from_line(&mnozina, get_line_in_file(path, 3));
    printf("mnozina na radku 1: ");
    set_print(&mnozina, 'S');

    command_complement(&mnozina, &universe);
    command_empty(&mnozina);
    command_card(&mnozina);

    set_dest(&mnozina);
    set_dest(&universe);

    printf("program path: %s, number of arguments: %d\n", argv[0], argc); // ! jen aby se to zkompilovalo, kdyz to tu neni tak to krici

    return 0;
}
