#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    char **elements;
    int size;
} set_t;

typedef struct
{

} rel_t;

void set_const(set_t *set, int size) //Constructor for sets
{
    set->size = size;

    if (size == 0)
    {
        set->elements = malloc(sizeof(char *));
    }
    else
    {
        set->elements = malloc(size * sizeof(char *));

        for (int i = 0; i < set->size; i++)
        {
            set->elements[i] = malloc(31 * sizeof(char));
            if (set->elements[i] == NULL) // Error
            {
                fprintf(stderr, "Memory not accessible.\nTerminating program.\n");
                exit(1);
            }
        }
    }
}

void set_dest(set_t *set)
{
    for (int i = 0; i < set->size; i++)
    {
        if (set->elements[i] != NULL)
        {
            free(set->elements[i]);
        }
    }
    free(set->elements);
    set->size = 0;
}

void set_init(set_t *set, char *string)
{
    for (int i = 0; i < set->size; i++)
    {
        strcpy(set->elements[i], string);
    }
}

void set_print(set_t *set, char type) // Print a given set
{
    printf("%c ", type);
    if (set->elements[0] != NULL) // If the first element is null, there are no elements in the set
    {
        printf("%s", set->elements[0]);     // Print the first element
        for (int i = 1; i < set->size; i++) // Print second through last elements
        {
            printf(" %s", set->elements[i]);
        }
    }
    printf("\n");
}

int set_from_line(set_t *set, char *line)
{
    char *element = (char *)malloc(31 * sizeof(char));

    int i = 0;
    int number_of_elements = 0;
    while (line[i] != '\n')
    {
        if (line[i] == ' ')
        {
            number_of_elements++;
        }
        i++;
    }

    set_const(set, number_of_elements);

    if (number_of_elements != 0)
    {
        int element_index = 0;

        for (int i = 1; line[i] != '\0'; i++)
        {
            memset(element, '\0', 31);
            if (line[i] != ' ')
            {
                int j = 0;
                while (line[i] != ' ' && line[i] != '\n')
                {
                    element[j] = line[i];
                    i++;
                    j++;
                    if (j > 30)
                    {
                        free(element);
                        fprintf(stderr, "Maximum length of element in set is 30 characters.\nTerminating program.\n");
                        return 1;
                    }
                }
                strcpy((set->elements[element_index]), element);
                element_index++;
            }
        }
    }
    free(element);
    return 0;
}

int set_check(set_t *set)
{
    if (true) // TODO doplnit kontrolu
    {
        return 0;
    }
    else
    {
        fprintf(stderr, "Given set doesn't match criteria.\nTerminating program.\n");
        return 1;
    }
}

void cmd_empty(set_t *set)
{
    if (set->size == 0)
    {
        printf("true\n");
    }
    else
        printf("false\n");
}

void cmd_card(set_t *set)
{
    printf("%d\n", set->size);
}

void cmd_complement(set_t *universe, set_t *set)
{
    if (set->size == 0)
    {
        set_print(universe, 'S');
    }
    else
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
                strcpy(complement.elements[comp_index], universe->elements[i]);
                comp_index++;
            }
        }
        set_print(&complement, 'S');
        set_dest(&complement);
    }
}

void execute_command(set_t *universe, set_t *sets, char *string)
{
    char *command = (char *)malloc(31 * sizeof(char));
    int index_A = 0;
    int index_B = 0;

    sscanf(string, "C %s %d %d\n", command, &index_A, &index_B);
    // printf("A: %d B: %d\n", index_A, index_B);

    if (strcmp(command, "empty") == 0)
    {
        cmd_empty(&sets[index_A]);
    }
    else if (strcmp(command, "card") == 0)
    {
        cmd_card(&sets[index_A]);
    }
    else if (strcmp(command, "complement") == 0)
    {
        cmd_complement(universe, &sets[index_A]);
    }

    free(command);
}

int main(int argc, char **argv)
{
    char *path = argv[1];
    // printf("number of arguments: %d, path: %s\n", argc, path);
    if (argc == 2)
    {
        FILE *input_file;
        input_file = fopen(path, "r");

        if (input_file == NULL)
        {
            fprintf(stderr, "Input file could not be opened. Does the file exist and do you have permissions?\nTerminating program.\n");
            return 1;
        }

        set_t universe;

        int sets_max_number = 5;
        set_t *sets = (set_t *)malloc(sets_max_number * sizeof(set_t));
        int set_count = 0;

        int line_max_len = 50;
        char *line = (char *)malloc(line_max_len * sizeof(char));
        int line_index = 0;
        int row = 1;
        char c;

        while ((c = fgetc(input_file)) != EOF)
        {
            if (c != '\n')
            {
                if (line_index >= line_max_len)
                {
                    line_max_len += 50;
                    line = realloc(line, (line_max_len) * sizeof(char));
                }
                line[line_index] = c;
                line_index++;
            }
            else
            {
                line[line_index] = '\n';

                switch (line[0])
                {
                case 'U':
                    if (set_from_line(&universe, line) == 0)
                    {
                        set_print(&universe, 'U');
                    }
                    else
                    {
                        // TODO Clean up
                        return 1;
                    }
                    break;
                case 'S':
                    if (set_count >= sets_max_number)
                    {
                        sets_max_number += 10;
                        sets = realloc(sets, (sets_max_number) * sizeof(set_t));
                    }
                    if ((set_from_line(&sets[row], line) == 0) && (set_check(&sets[row]) == 0))
                    {
                        set_print(&sets[row], 'S');
                        set_count++;
                    }
                    else
                    {
                        // TODO Clean up
                        return 1;
                    }
                    break;
                case 'C':
                    execute_command(&universe, sets, line);
                    break;
                default:
                    break;
                }

                row++;
                line_index = 0;
                memset(line, '\0', 50 * sizeof(char));
            }
        }

        // Cleaning up

        free(line);
        fclose(input_file);

        for (int i = 0; i < set_count + 2; i++)
        {
            set_dest(&sets[i]);
        }
        free(sets);

        set_dest(&universe);
    }
    else
    {
        fprintf(stderr, "Wrong number of arguments supplied.\nTerminating program.\n");
        return 1;
    }
}