/*
 *      setcal.c
 *  
 *      Set calculator
 *      IZP - 2nd project
 * 
 *      xkurci00    Kurcikova Julia 
 *      xmacec03    Macecek Patrik  
 *      xpenas02    Penas Dalibor   
 *      xstrel03    Strelec Matyas  
 *           
 *      11/2021
 */

#include <stdio.h>   // Header defining variables and functions for input / output
#include <stdlib.h>  // Header defining variables and functions for general functions
#include <string.h>  // Header defining functions for working with strings
#include <stdbool.h> // Header defining boolean types
/*
TODO LIST:
- kontrola spravnosti volani prikazu (prikaz nad mnozinou volany na radek s relaci / prikazem)
- spravna dealokace a uvolnovani pameti pri chybach
- check spravnosti commandu
*/

typedef struct // Custom data type definiton for a set
{
    bool empty;
    char **elements; // Elements are stored in an array of strings
    int size;        // The number of elements
} set_t;

void set_const(set_t *set, int size) // Constructor for sets
{
    set->empty = false;
    set->size = size; // Set the size of set => number of elements in set

    // if (size == 0)
    // {
    //     set->elements = malloc(sizeof(char *));
    // }
    // else
    // {

    set->elements = malloc(size * sizeof(char *)); // Allocate memory for the array of elements

    for (int i = 0; i < set->size; i++)
    {
        set->elements[i] = malloc(31 * sizeof(char)); // Allocate memory for each element
        if (set->elements[i] == NULL)                 // If malloc returns NULL, there was an error when accessing memory
        {
            fprintf(stderr, "Memory not accessible.\nTerminating program.\n");
            exit(1);
        }
    }
    // }
}

void set_dest(set_t *set) // Destructor for sets
{
    if (!set->empty)
        for (int i = 0; i < set->size; i++) // Go through all individual elements
        {
            if (set->elements[i] != NULL)
            {
                free(set->elements[i]);
            }
        }
    free(set->elements);
    set->size = 0;
}

void set_print(set_t *set, char type) // Print a given set, type argument is the letter printed at the beginning of line (either U for universe or S for set)
{
    printf("%c", type);
    if (!set->empty)
    {
        for (int i = 0; i < set->size; i++) // Go through all elements and print them to stdout
        {
            printf(" %s", set->elements[i]);
        }
    }
    printf("\n");
}

int set_from_line(set_t *set, char *line) // Read a given string and create a set from the string
{
    char *element = (char *)malloc(31 * sizeof(char)); // Allocate memory for the element currently being read

    int i = 0;
    int number_of_elements = 0;

    while (line[i] != '\n') // Count number of elements - number of '0' characters is the same
    {
        if (line[i] == ' ')
        {
            number_of_elements++;
        }
        if (!((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || (line[i] == ' ') || (line[i] >= '0' && line[i] <= '9')))
        {
            fprintf(stderr, "Given set doesn't match criteria.\nTerminating program.\n");
            free(element);
            return 1;
        }
        i++;
    }

    set_const(set, number_of_elements); // Run the constructor with the size of the set

    if (number_of_elements != 0) // Set can have no elements, therefore no need to add any elements in this case
    {
        int element_index = 0; // Index of element currently being read

        for (int i = 1; line[i] != '\0'; i++) // Read the given string until the end, starting at index 1 - index 0 defines either set (S) or universe (U)
        {
            memset(element, '\0', 31); // Set the whole element string to be '\0'
            if (line[i] != ' ')
            {
                int j = 0;
                while (line[i] != ' ' && line[i] != '\n')
                {
                    if (!((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z')))
                    {
                        fprintf(stderr, "Given set doesn't match criteria.\nTerminating program.\n");
                        free(element);
                        return 1;
                    }

                    element[j] = line[i];
                    i++;
                    j++;
                    if (j > 30) // If the length of the currently read element reaches 30, program is terminated
                    {
                        free(element);
                        fprintf(stderr, "Maximum length of element in set is 30 characters.\nTerminating program.\n");
                        return 1;
                    }
                }
                strcpy((set->elements[element_index]), element); // Copy the read element to the set array
                element_index++;
            }
        }
    }
    free(element);
    return 0;
}

typedef struct // Custom data type definiton for a pair of strings used in relations
{
    char a[31]; // First element in relation
    char b[31]; // Second element in relation
} double_t;

typedef struct // Custom data type definiton for a relation
{
    bool empty;
    double_t *elements; // Array of pairs of strings
    int size;           // Number of elements in the relation
} rel_t;

void rel_const(rel_t *rel, int size) // Constructor for relations
{
    rel->empty = false;
    rel->size = size;

    // if (size == 0)
    // {
    //     rel->elements = malloc(sizeof(double_t));
    // }
    // else
    // {
    rel->elements = malloc(size * sizeof(double_t)); // Allocate as much memory as needed for number of elements given
    // }
}

void rel_dest(rel_t *rel) // Destructor for relations
{
    free(rel->elements);
    rel->size = 0;
}

void rel_print(rel_t *rel) // Print a given relation
{
    printf("R");
    if ((strcmp(rel->elements[0].a, "") != 0) && (strcmp(rel->elements[0].b, "") != 0)) // If the first element is empty, there are no elements in the relation
    {
        for (int i = 0; i < rel->size; i++) // Go through all elements in relation
        {
            printf(" (%s %s)", rel->elements[i].a, rel->elements[i].b); // Print each element in the given format (first, second)
        }
    }
    printf("\n");
}

int rel_from_line(rel_t *rel, char *line) // Read a given string and create a relation from the string
{
    int i = 0;
    int number_of_pairs = 0;
    while (line[i] != '\n') // Count number of elements - number of '(' characters is the same
    {
        if (line[i] == '(')
        {
            number_of_pairs++;
        }
        i++;
    }

    rel_const(rel, number_of_pairs); // Run the constructor with the size of the relation

    if (number_of_pairs != 0) // Relation can have no elements, therefore no need to add any elements in this case
    {
        int pair_index = 0; // Index of pair currently being read

        int i = 1;
        while (line[i] != '\0') // Go through the given string until the end
        {
            if (line[i] == '(')
            {
                i++; // Read the first element
                int j = 0;
                while (line[i] != ' ')
                {
                    rel->elements[pair_index].a[j] = line[i]; // Copy the elements from the string to the relation
                    i++;
                    j++;
                    if (j > 30) // If the length of the currently read element reaches 30, program is terminated
                    {
                        fprintf(stderr, "Maximum length of element in set is 30 characters.\nTerminating program.\n");
                        return 1;
                    }
                }
                i++;
                j = 0; // Same for the second element
                while (line[i] != ')')
                {
                    rel->elements[pair_index].b[j] = line[i];
                    i++;
                    j++;
                    if (j > 30)
                    {
                        fprintf(stderr, "Maximum length of element in set is 30 characters.\nTerminating program.\n");
                        return 1;
                    }
                }
                pair_index++;
            }
            else
            {
                i++;
            }
        }
    }
    return 0;
}

int universe_check(set_t *universe) // Check if the universe matches criteria given (no duplicite elements, no true/false, no elements same as commands)
{
    for (int i = 0; i < universe->size; i++) // Check if elements are not duplicite or if they are not same as forbidden words
    {
        for (int j = 0; j < universe->size; j++)
        {
            if ((strcmp(universe->elements[i], universe->elements[j]) == 0) && (i != j))
            {
                fprintf(stderr, "Set doesn't match criteria.\nTerminating program.\n");
                return 1;
            }
        }

        if ((strcmp(universe->elements[i], "true") == 0) || (strcmp(universe->elements[i], "false") == 0) ||
            (strcmp(universe->elements[i], "empty") == 0) || (strcmp(universe->elements[i], "card") == 0) || (strcmp(universe->elements[i], "complement") == 0) ||
            (strcmp(universe->elements[i], "union") == 0) || (strcmp(universe->elements[i], "intersect") == 0) || (strcmp(universe->elements[i], "minus") == 0) ||
            (strcmp(universe->elements[i], "subseteq") == 0) || (strcmp(universe->elements[i], "subset") == 0) || (strcmp(universe->elements[i], "equals") == 0) ||
            (strcmp(universe->elements[i], "reflexive") == 0) || (strcmp(universe->elements[i], "symmetric") == 0) || (strcmp(universe->elements[i], "antisymmetric") == 0) ||
            (strcmp(universe->elements[i], "transitive") == 0) || (strcmp(universe->elements[i], "function") == 0) || (strcmp(universe->elements[i], "domain") == 0) ||
            (strcmp(universe->elements[i], "codomain") == 0) || (strcmp(universe->elements[i], "injective") == 0) || (strcmp(universe->elements[i], "surjective") == 0) ||
            (strcmp(universe->elements[i], "bijective") == 0))
        {
            fprintf(stderr, "Set doesn't match criteria.\nTerminating program.\n");
            return 1;
        }
    }
    return 0;
}

int set_check(set_t *set, set_t *univ) // Check if a set matches criteria given (all elements are from the universe and they are not duplicite)
{
    for (int i = 0; i < set->size; i++) // Check if the elements are only from the universe
    {
        bool found = false;

        for (int j = 0; j < univ->size; j++)
        {
            if (strcmp(set->elements[i], univ->elements[j]) == 0)
            {
                found = true;
                break;
            }
        }

        if (!found) // Print an error if an element not from the universe is found
        {
            fprintf(stderr, "Given set doesn't match criteria.\nTerminating program.\n");
            return 1;
        }
    }

    for (int i = 0; i < set->size; i++) // Check if elements are not duplicite
    {
        for (int j = 0; j < set->size; j++)
        {
            if ((strcmp(set->elements[i], set->elements[j]) == 0) && (i != j))
            {
                fprintf(stderr, "Given set doesn't match criteria.\nTerminating program.\n");
                return 1;
            }
        }
    }
    return 0;
}

int rel_check(rel_t *rel, set_t *univ)
{
    for (int i = 0; i < rel->size; i++) // Check if the elements are only from the universe
    {
        bool found_A = false;
        bool found_B = false;

        for (int j = 0; j < univ->size; j++)
        {
            if (strcmp(rel->elements[i].a, univ->elements[j]) == 0)
            {
                found_A = true;
            }
            if (strcmp(rel->elements[i].b, univ->elements[j]) == 0)
            {
                found_B = true;
            }
        }

        if (!found_A || !found_B) // Print an error if an element not from the universe is found
        {
            fprintf(stderr, "Given set doesn't match criteria.\nTerminating program.\n");
            return 1;
        }
    }

    for (int i = 0; i < rel->size; i++) // Check if elements are not duplicite
    {
        for (int j = 0; j < rel->size; j++)
        {
            if ((strcmp(rel->elements[i].a, rel->elements[j].a) == 0) && (strcmp(rel->elements[i].b, rel->elements[j].b) == 0) && (i != j))
            {
                fprintf(stderr, "Given relation doesn't match criteria.\nTerminating program.\n");
                return 1;
            }
        }
    }
    return 0;
}

void cmd_empty(set_t *set) // Empty command
{
    if (set->size == 0) // If the size of set is 0, the set is empty therefore the command is true
    {
        printf("true\n");
    }
    else
        printf("false\n");
}

void cmd_card(set_t *set) // Card command
{
    printf("%d\n", set->size); // Print the number of elements in the set
}

void cmd_complement(set_t *universe, set_t *set) // Complement command
{
    if (set->size == 0) // If there are no elements in the set, its complement is the same as universe
    {
        set_print(universe, 'S');
    }
    else
    {
        set_t complement;                                     // Create a new set to store the complements
        set_const(&complement, (universe->size - set->size)); // The size of complement is the size of universe minus the size of set

        int comp_index = 0;

        for (int i = 0; i < universe->size; i++) // Find all the elements that are both in the given set and in universe
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

            if (!found) // For elements from universe that are not in the set, copy them to the newly created set
            {
                strcpy(complement.elements[comp_index], universe->elements[i]);
                comp_index++;
            }
        }
        set_print(&complement, 'S'); // Print the complement
        set_dest(&complement);       // Clean up
    }
}

<<<<<<< HEAD
void cmd_union(set_t *set_A, set_t *set_B)
{
    set_t set_union;
    set_const(&set_union, set_A->size + set_B->size);
    int pos = 0;

    for (int i = 0; i < set_A->size; i++)
    {
        strcpy(set_union.elements[i], set_A->elements[i]);
        pos++;
    }

    for (int i = 0; i < set_B->size; i++)
    {
        for (int j = 0; j < set_A->size; j++)
        {
            if (strcmp(set_B->elements[i], set_union.elements[j]) == 0)
            {
                break;
            }
            else if (j == (set_A->size - 1))
            {
                strcpy(set_union.elements[pos], set_B->elements[i]);
                pos++;
                break;
            }
        }
    }
    set_print(&set_union, 'S');
    // set_dest(&set_union);
}

void cmd_intersect(set_t *set_A, set_t *set_B)
{
    set_t intersect;
    int pos = 0;

    if (set_A->size > set_B->size)
    {
        set_const(&intersect, set_A->size);
    }
    else
    {
        set_const(&intersect, set_B->size);
    }

    for (int i = 0; i < set_A->size; i++)
    {
        for (int j = 0; j < set_B->size; j++)
        {
            if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
            {
                strcpy(intersect.elements[pos], set_A->elements[i]);
                pos++;
                break;
            }
        }
    }
    set_print(&intersect, 'S');
    set_dest(&intersect);
}

void cmd_minus(set_t *set_A, set_t *set_B)
{
    set_t minus;
    int pos = 0;

    set_const(&minus, set_A->size);
    
    for (int i = 0; i < set_A->size; i++)
    {
        for (int j = 0; j < set_B->size; j++)
        {
            if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
            {
                break;
            }
            else if (j == set_B->size - 1)
            {
                strcpy(minus.elements[pos], set_A->elements[i]);
                pos++;
            }
        }
    }
    minus.size=pos;
    set_print(&minus, 'S');
    set_dest(&minus);
}

bool cmd_subseteq(set_t *set_A, set_t *set_B)
{
    for (int i = 0; i < set_A->size; i++)
    {
        for (int j = 0; j < set_B->size; j++)
        {
            if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
            {
                break;
            }
            else if (j == set_B->size - 1)
            {
                return false;
            }
        }
    }
    return true;
}

bool cmd_equals(set_t *set_A, set_t *set_B)
{
    if (set_A->size != set_B->size)
    {
        return false;
    }

    for (int i = 0; i < set_A->size; i++)
    {
        for (int j = 0; j < set_B->size; j++)
        {
            if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
            {
                break;
            }
            else if (j == set_B->size - 1)
            {
                return false;
            }
        }
    }
    return true;
}
=======
// void cmd_union(set_t *set_A, set_t *set_B)
// {
//     set_t set_union;
//     set_const(&set_union, set_A->size + set_B->size);
//     int pos = 0;

//     for (int i = 0; i < set_A->size; i++)
//     {
//         strcpy(set_union.elements[i], set_A->elements[i]);
//         pos++;
//     }

//     for (int i = 0; i < set_B->size; i++)
//     {
//         for (int j = 0; j < set_A->size; j++)
//         {
//             if (strcmp(set_B->elements[i], set_union.elements[j]) == 0)
//             {
//                 break;
//             }
//             else if (j == (set_A->size - 1))
//             {
//                 strcpy(set_union.elements[pos], set_B->elements[i]);
//                 pos++;
//                 break;
//             }
//         }
//     }
//     set_print(&set_union, 'S');
//     // set_dest(&set_union);
// }

// void cmd_intersect(set_t *set_A, set_t *set_B)
// {
//     set_t intersect;
//     int pos = 0;

//     if (set_A->size > set_B->size)
//     {
//         set_const(&intersect, set_A->size);
//     }
//     else
//     {
//         set_const(&intersect, set_B->size);
//     }

//     for (int i = 0; i < set_A->size; i++)
//     {
//         for (int j = 0; j < set_B->size; j++)
//         {
//             if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
//             {
//                 strcpy(intersect.elements[pos], set_B->elements[i]);
//                 pos++;
//                 break;
//             }
//         }
//     }
//     set_print(&intersect, 'S');
//     // set_dest(&intersect);
// }

// void cmd_minus(set_t *set_A, set_t *set_B)
// {
//     set_t minus;
//     int pos = 0;

//     for (int i = 0; i < set_A->size; i++)
//     {
//         for (int j = 0; j < set_B->size; j++)
//         {
//             if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
//             {
//                 break;
//             }
//             else if (j + 1 == set_B->size)
//             {
//                 strcpy(minus.elements[pos], set_A->elements[i]);
//                 pos++;
//             }
//         }
//     }
//     set_print(&minus, 'S');
//     // set_dest(&minus);
// }

// bool cmd_subseteq(set_t *set_A, set_t *set_B)
// {
//     for (int i = 0; i < set_A->size; i++)
//     {
//         for (int j = 0; j < set_B->size; j++)
//         {
//             if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
//             {
//                 break;
//             }
//             else if (j + 1 == set_B->size)
//             {
//                 return false;
//             }
//         }
//     }
//     return true;
// }

// bool cmd_equals(set_t *set_A, set_t *set_B)
// {
//     if (set_A->size != set_B->size)
//     {
//         return false;
//     }

//     for (int i = 0; i < set_A->size; i++)
//     {
//         for (int j = 0; j < set_B->size; j++)
//         {
//             if (strcmp(set_A->elements[i], set_B->elements[j]) == 0)
//             {
//                 break;
//             }
//             else if (j + 1 == set_B->size)
//             {
//                 return false;
//             }
//         }
//     }
//     return true;
// }
>>>>>>> 2b21caad6c475bf248fa0400e1fc756baf05f956

/* bool cmd_subset(set_t *set_A, set_t *set_B)
{
    if(cmd_equals(&set_A, &set_B))
    {
        return false;
    }
    else if(cmd_subseteq(&set_A, &set_B) )
    {
        return true;
    }
    return false;
} */

void cmd_reflexive(set_t *universe, rel_t *rel) // Reflexive command
{
    if (rel->size == 0 && universe->size != 0) // If the size of relation is 0 and universe isn't empty, the relation isn't reflexive
    {
        printf("false\n");
        return;
    }

    for (int i = 0; i < universe->size; i++)
    {
        for (int j = 0; j < rel->size; j++)
        {
            if (strcmp(universe->elements[i], rel->elements[j].a) == 0 && strcmp(universe->elements[i], rel->elements[j].b) == 0)
                break;
            else if (j == rel->size - 1)
            {
                printf("false\n");
                return;
            }
        }
    }

    printf("true\n");
}

void cmd_symmetric(rel_t *rel) // Symmetric command
{
    for (int i = 0; i < rel->size; i++)
    {
        for (int j = 0; j < rel->size; j++)
        {
            if (strcmp(rel->elements[i].b, rel->elements[j].a) == 0 && strcmp(rel->elements[i].a, rel->elements[j].b) == 0)
                break;
            else if (j == rel->size - 1)
            {
                printf("false\n");
                return;
            }
        }
    }
    printf("true\n");
}

void cmd_antisymmetric(rel_t *rel) // Antisymmetric command
{
    for (int i = 0; i < rel->size; i++)
    {
        for (int j = 0; j < rel->size; j++)
        {
            if (strcmp(rel->elements[i].b, rel->elements[j].a) == 0 && strcmp(rel->elements[i].a, rel->elements[j].b) == 0)
            {
                if (!(strcmp(rel->elements[i].a, rel->elements[j].a) == 0 && strcmp(rel->elements[i].b, rel->elements[j].b) == 0))
                {
                    printf("false\n");
                    return;
                }
                else if (j == rel->size - 1)
                {
                    printf("true\n");
                    return;
                }
            }
        }
    }
    printf("true\n");
}

void cmd_transitive(rel_t *rel) // Transitive command
{
    for (int i = 0; i < rel->size; i++)
    {
        for (int j = 0; j < rel->size; j++)
        {

            if (strcmp(rel->elements[i].b, rel->elements[j].a) == 0)
            {
                for (int k = 0; k < rel->size; k++)
                {

                    if (strcmp(rel->elements[i].a, rel->elements[k].a) == 0 && strcmp(rel->elements[j].b, rel->elements[k].b) == 0)
                        break;

                    if (k == rel->size - 1)
                    {
                        printf("false\n");
                        return;
                    }
                }
            }
        }
    }
    printf("true\n");
}

int cmd_function(rel_t *rel, bool from_cmd) // Function command, from_cmd allows to call this cmd from another cmd without it printing its result
{
    for (int i = 0; i < rel->size; i++)
    {
        for (int j = i + 1; j < rel->size; j++)
        {
            if (strcmp(rel->elements[i].a, rel->elements[j].a) == 0)
            {
                if (from_cmd == false)
                    printf("false\n");
                return 0;
            }
            else if (j == rel->size - 1)
            {
                if (from_cmd == false)
                    printf("true\n");
                return 1;
            }
        }
    }
    if (from_cmd == false)
        printf("true\n");
    return 1;
}

void cmd_domain(rel_t *rel)
{
    printf("S ");

    int num = 0;
    char **dom;
    dom = (char **)malloc(rel->size * sizeof(char *));
    *dom = (char *)malloc(31 * sizeof(char));

    for (int i = 0; i < rel->size; i++)
    { //fills domain array for later check
        dom[i] = rel->elements[i].a;
    }

    for (int i = 0; i < rel->size; i++) // Check if elements are not duplicite
    {
        for (int j = i + 1; j < rel->size; j++)
        {
            if (strcmp(dom[i], dom[j]) == 0)
            {
                num++;
                break;
            }
        }
        if (num == 0)
            printf("%s ", dom[i]);
        num = 0;
    }
    printf("\n");
    free(dom);
}

void cmd_codomain(rel_t *rel)
{
    printf("S");

    int num = 0;
    char **codom;
    codom = (char **)malloc(rel->size * sizeof(char *));
    *codom = (char *)malloc(31 * sizeof(char));

    for (int i = 0; i < rel->size; i++)
    { //fills codomain array for later check
        codom[i] = rel->elements[i].b;
    }

    for (int i = 0; i < rel->size; i++) // Check if elements are not duplicite
    {
        for (int j = i + 1; j < rel->size; j++)
        {
            if (strcmp(codom[i], codom[j]) == 0)
            {
                num++;
                break;
            }
        }
        if (num == 0)
            printf(" %s", codom[i]);
        num = 0;
    }
    printf("\n");
    free(codom);
}

int cmd_surjective(rel_t *rel, set_t *setB, bool from_cmd)
{
    if (cmd_function(rel, true) == 0)
    { //checks if rel is function
        if (from_cmd == false)
            printf("false\n");
        return 0;
    }

    if (rel->size < setB->size)
    { //from definition of surjectivness, the relation has to have equal or more elements than the set its projecting from
        if (from_cmd == false)
            printf("false\n");
        return 0;
    }

    int num = 0;
    for (int i = 0; i < setB->size; i++)
    { //for one set element, it goes threw every relations element and its b part, than decide, if every set element is represented at least one in the relation b side
        for (int j = 0; j < rel->size; j++)
        {
            if (strcmp(setB->elements[i], rel->elements[j].b) != 0)
            {
                num++;
            }
            else
            {
                num = 0;
                break;
            }
        }
        if (num != 0)
        {
            if (from_cmd == false)
                printf("false\n");
            return 0;
        }
    }
    if (from_cmd == false)
        printf("true\n");
    return 1;
}

int cmd_injective(rel_t *rel, set_t *setA, set_t *setB, bool from_cmd)
{
    if (cmd_function(rel, true) == 0)
    { //checks if rel is function
        if (from_cmd == false)
            printf("false\n");
        return 0;
    }

    int num = 0;
    for (int i = 0; i < setA->size; i++)
    { //for one setA/B element, it goes threw every relation element and its a/b part, than decide if everz set element is represented at most once
        for (int j = 0; j < rel->size; j++)
        {
            if (strcmp(setA->elements[i], rel->elements[j].a) == 0)
            {
                num++;
                break;
            }
        }
        if (num > 1)
        {
            if (from_cmd == false)
                printf("false\n");
            return 0;
        }
        num = 0;
    }

    num = 0;
    for (int i = 0; i < setB->size; i++)
    {
        for (int j = 0; j < rel->size; j++)
        {
            if (strcmp(setB->elements[i], rel->elements[j].b) == 0)
            {
                num++;
                break;
            }
        }
        if (num > 1)
        {
            if (from_cmd == false)
                printf("false\n");
            return 0;
        }
        num = 0;
    }

    if (from_cmd == false)
        printf("true\n");
    return 1;
}

//void cmd_bijective(rel_t *rel, set_t *setA, set_t *setB){ //calls both functions surjective and injective, if both are true, than bijective is also true
//    if((cmd_surjective(&rel, &setB, true) && cmd_injective(&rel, &setB, true))==1){
//        printf("true\n");
//    } else printf("false\n");
//}

int execute_command(set_t *universe, set_t *sets, rel_t *rels, char *string) // Decide what happens when reading a line defining a command
{
    char *command = (char *)malloc(31 * sizeof(char)); // Allocate memory for the name of the element
    int index_A = 0;                                   // Indexes of the sets or relations used
    int index_B = 0;
    int index_C = 0;

    //tenhle kod nic nedela, ale je tam aby to fungovalo i kdyz se zatim s polem rels nic neprovadi :D
    //char *test = rels[3].elements[0].a;
    //if (false)
    //printf("%s", test);

    sscanf(string, "C %s %d %d %d\n", command, &index_A, &index_B, &index_C); // Read the line given and save the information needed
    printf("%s %d %d %d:\n", command, index_A, index_B, index_C);

    if (strcmp(command, "empty") == 0) // Execute a command based on what was read from the line of text
    {
        if (index_A == 0 || index_B != 0 || index_C != 0)
        {
            fprintf(stderr, "Wrong input file format.\nTerminating program.\n");
            return 1;
        }
        cmd_empty(&sets[index_A]);
    }
    else if (strcmp(command, "card") == 0)
    {
        if (index_A == 0 || index_B != 0 || index_C != 0)
        {
            fprintf(stderr, "Wrong input file format.\nTerminating program.\n");
            return 1;
        }
        cmd_card(&sets[index_A]);
    }
    else if (strcmp(command, "complement") == 0)
    {
        cmd_complement(universe, &sets[index_A]);
    }
    // else if (strcmp(command, "union") == 0)
    // {
    //     cmd_union(&sets[index_A], &sets[index_B]);
    // }
    // else if (strcmp(command, "intersect") == 0)
    // {
    //     cmd_intersect(&sets[index_A], &sets[index_B]);
    // }
    // else if (strcmp(command, "minus") == 0)
    // {
    //     cmd_minus(&sets[index_A], &sets[index_B]);
    // }
    // else if (strcmp(command, "subseteq") == 0)
    // {
    //     cmd_subseteq(&sets[index_A], &sets[index_B]);
    // }
    // else if (strcmp(command, "subset") == 0)
    // {
    //     cmd_subset(&sets[index_A], &sets[index_B]);
    // }
    // else if (strcmp(command, "equals") == 0)
    // {
    //     cmd_equals(&sets[index_A], &sets[index_B]);
    // }
    else if (strcmp(command, "reflexive") == 0)
    {
        cmd_reflexive(universe, &rels[index_A]);
    }
    else if (strcmp(command, "symmetric") == 0)
    {
        cmd_symmetric(&rels[index_A]);
    }
    else if (strcmp(command, "antisymmetric") == 0)
    {
        cmd_antisymmetric(&rels[index_A]);
    }
    else if (strcmp(command, "transitive") == 0)
    {
        cmd_transitive(&rels[index_A]);
    }
    else if (strcmp(command, "function") == 0)
    {
        cmd_function(&rels[index_A], false);
    }
    else if (strcmp(command, "domain") == 0)
    {
        cmd_domain(&rels[index_A]);
    }
    else if (strcmp(command, "codomain") == 0)
    {
        cmd_codomain(&rels[index_A]);
    }
    else if (strcmp(command, "surjective") == 0)
    {
        cmd_surjective(&rels[index_A], &sets[index_C], false);
    }
    else if (strcmp(command, "injective") == 0)
    {
        cmd_injective(&rels[index_A], &sets[index_B], &sets[index_C], false);
    }
    else if (strcmp(command, "bijective") == 0)
    {
        //cmd_bijective(&rels[index_A], &sets[index_B], &sets[index_C]);

        if ((cmd_surjective(&rels[index_A], &sets[index_C], true) && cmd_injective(&rels[index_A], &sets[index_B], &sets[index_C], true)) == 1)
        {
            printf("true\n");
        }
        else
            printf("false\n");
    }
    free(command);
    return 0;
}

int main(int argc, char **argv)
{
    char *path = argv[1]; // Store the path for the text file containing data
    if (argc == 2)        // Only two arguments are expected - 1. is name of the program, 2. is the path to the file
    {
        FILE *input_file;
        input_file = fopen(path, "r"); // Open the text file given

        if (input_file == NULL) // If file can't be opened, print error
        {
            fprintf(stderr, "Input file could not be opened. Does the file exist and do you have permissions?\nTerminating program.\n");
            return 1;
        }

        set_t universe; // Set used to store the universe

        int sets_max_number = 5;                                        // Maximum number of sets to be allocated, changes throughout the program
        set_t *sets = (set_t *)malloc(sets_max_number * sizeof(set_t)); // Alocate enough memory for 5 sets

        int rels_max_number = 5; // Same for relations
        rel_t *rels = (rel_t *)malloc(rels_max_number * sizeof(rel_t));

        int line_max_len = 50;                                    // Maximum buffer length for the line being read, can be changed if the line doesn't fit
        char *line = (char *)malloc(line_max_len * sizeof(char)); // Allocate memory for the line
        int line_index = 0;                                       // Which character from the line is currently being read
        int row = 1;                                              // Which row of the text file is being read, used to correctly store sets and relations on their index
        char c;                                                   // Character being read

        while (((c = fgetc(input_file)) != EOF)) // Go through the text file until the end of file
        {
            if (c != '\n') // Go trough a line until newline character
            {
                if (line_index >= line_max_len) // If the line read is too long to fit in the allocated memory, allocate more
                {
                    line_max_len += 50;
                    line = realloc(line, (line_max_len) * sizeof(char));
                }
                line[line_index] = c; // Save the text to line
                line_index++;
            }
            else // If a newline character is found, decide what happens with the line
            {
                line[line_index] = '\n';

                switch (line[0])
                {
                case 'U':                                                                          // If the first character of the line is 'U', it defines the universe
                    if ((set_from_line(&universe, line) == 0) && (universe_check(&universe) == 0)) // Save the universe to a set, if it ran correctly print it
                    {
                        // printf("%d: ", row);
                        set_print(&universe, 'U');
                    }
                    else // If the functions to save the universe or check it for being correct returned error, clean up and stop the program
                    {
                        // TODO Clean up
                        // goto()?
                        return 1;
                    }
                    break;
                case 'S':                       // If the first character of the line is 'S', it defines a new set
                    if (row >= sets_max_number) // Check if the new set fits into the memory we allocated for sets
                    {
                        sets_max_number += 10;
                        sets = realloc(sets, (sets_max_number) * sizeof(set_t)); // If it doesn't fit, allocate more memory (10 more sets can fit now)
                    }
                    if ((set_from_line(&sets[row], line) == 0) && (set_check(&sets[row], &universe) == 0)) // Save the set to the array of sets, if it ran correctly print it
                    {
                        // printf("%d: ", row);
                        set_print(&sets[row], 'S');
                    }
                    else // If the functions to save the set or check it for being correct returned error, clean up and stop the program
                    {
                        // TODO Clean up
                        return 1;
                    }
                    break;
                case 'R':                       // If the first character of the line is 'R', it defines a new relation
                    if (row >= rels_max_number) // Check if the new relation fits into the memory we allocated for relations
                    {
                        rels_max_number += 10;
                        rels = realloc(rels, (rels_max_number) * sizeof(rel_t)); // If it doesn't fit, allocate more memory (10 more relations can fit now)
                    }
                    if ((rel_from_line(&rels[row], line) == 0) && (rel_check(&rels[row], &universe) == 0)) // Save the relation to the array of relations, if it ran correctly print it // TODO check rel
                    {
                        // printf("%d: ", row);
                        rel_print(&rels[row]);
                    }
                    else // If the functions to save the relation or check it for being correct returned error, clean up and stop the program
                    {
                        // TODO Clean up
                        return 1;
                    }
                    break;
                case 'C':                                                    // If the first character of the line is 'C', it defines a command to be executed
                    if ((execute_command(&universe, sets, rels, line) != 0)) // Execute the command
                    {
                        fprintf(stderr, "Invalid input file.\nTerminating program.\n");
                        return 1;
                    }
                    break;
                default:
                    break;
                }

                row++;
                line_index = 0;
                memset(line, '\0', line_max_len * sizeof(char)); // At the end of the line, clear the line
            }
            if (row > 1001)
            {
                fprintf(stderr, "Invalid input file.\nTerminating program.\n");
                return 1;
            }
        }

        // After reading the whole text document, the program is finished
        // Free all allocated memory and clean up

        free(line);         // Free memory allocated for the line from the text file
        fclose(input_file); // Close the text file

        // for (int i = 0; i < row + 1; i++) // Go through all sets and run the destructor
        // {
        //     set_dest(&sets[i]);
        // }
        free(sets); // Free the whole array of sets

        // for (int i = 0; i < row + 1; i++) // Go through all relations and run the destructor
        // {
        //     rel_dest(&rels[i]);
        // }
        free(rels); // Free the whole array of relations

        set_dest(&universe); // Run the destructor for the set storing the universe
    }
    else // If the wrong number of arguments is given, print and error and terminate
    {
        fprintf(stderr, "Wrong number of arguments supplied.\nTerminating program.\n");
        return 1;
    }
}
