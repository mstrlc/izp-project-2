#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct // Definiton of the data type used to store sets
{
    char **elements; // Array of elements stored as strings
    int size;        // Number of elements in set
} set_t;

void set_const(set_t *set, int size) //Constructor for sets
{
    set->size = size;
    set->elements = malloc(size * sizeof(char *));

    for (int i = 0; i < set->size; i++)
    {
        set->elements[i] = malloc(31 * sizeof(char));
        if (set->elements[i] == NULL) // Error
        {
            fprintf(stderr, "Memory not accessible. \nTerminating program.\n");
            exit(1);
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

void set_from_line(set_t *set, char *path, int line)
{
    FILE *input_file;
    input_file = fopen(path, "r");

    if (input_file == NULL)
    {
        fprintf(stderr, "Input file could not be opened. Does the file exist and do you have permissions?\nTerminating program.\n");
        exit(1);
    }

    char *buffer = (char *)malloc(sizeof(char) * 1000);
    for (int i = 0; i <= line + 1; i++)
    {
        fgets(buffer, 1000, input_file);
    }

    int length = 0;
    while (buffer[length] != '\n')
    {
        length++;
    }

    char returned_line[length];
    strcpy(returned_line, buffer);

    free(buffer);
    fclose(input_file);

    char *element = (char *)malloc(31 * sizeof(char));

    int i = 0;
    int number_of_elements = 0;
    while (returned_line[i] != '\0')
    {
        if (returned_line[i] == ' ')
        {
            number_of_elements++;
        }
        i++;
    }

    set_const(set, number_of_elements);

    if (number_of_elements == 0)
    {
        set->elements[0] = NULL;
    }
    else
    {

        int element_index = 0;
        for (int i = 1; returned_line[i] != '\0'; i++)
        {
            memset(element, '\0', 31);
            if (returned_line[i] != ' ')
            {
                int j = 0;
                while (returned_line[i] != ' ' && returned_line[i] != '\n')
                {
                    element[j] = returned_line[i];
                    i++;
                    j++;
                }
                strcpy((set->elements[element_index]), element);
                element_index++;
            }
        }
    }
    free(element);
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

void execute_command(set_t *universe, char *string, char *path)
{
    char *command = (char *)malloc(31 * sizeof(char));
    int set_A_index = 0;
    int set_B_index = 0;

    sscanf(string, "C %s %d\n", command, &set_A_index);
    // printf("%s %d %d:\n", command, set_A_index, set_B_index);

    set_t set_A;
    set_t set_B;

    set_from_line(&set_A, path, set_A_index);
    if (set_B_index != 0)
    {
        set_from_line(&set_B, path, set_B_index);
    }

    if (strcmp(command, "empty") == 0)
    {
        cmd_empty(&set_A);
    }
    else if (strcmp(command, "card") == 0)
    {
        cmd_card(&set_A);
    }
    else if (strcmp(command, "complement") == 0)
    {
        cmd_complement(universe, &set_A);
    }

    set_dest(&set_A);
    if (set_B_index != 0)
    {
        set_dest(&set_B);
    }

    free(command);
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

    printf("Program path: %s, number of arguments: %d\n", argv[0], argc); // ! jen aby se to zkompilovalo, kdyz to tu neni tak to krici

    set_t universe; // Set defining the universe
    set_from_line(&universe, path, 0);

    char line_buffer[1000];

    FILE *input_file;
    input_file = fopen(path, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "Input file could not be opened. Does the file exist and do you have permissions?\nTerminating program.\n");
        exit(1);
    }
    while ((fgets(line_buffer, 1000, input_file)) != NULL)
    {
        if (line_buffer[0] == 'U' || line_buffer[0] == 'S') // Line defining a set or relation
        {
            printf("%s", line_buffer);
        }
        if (line_buffer[0] == 'C') // Line defining a command
        {
            execute_command(&universe, line_buffer, path);
        }
    }

    fclose(input_file);

    set_dest(&universe);

    return 0;
}
