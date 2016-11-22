/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Memory table length
#define CELL_TABLE_LENGTH 9001
#define NUM_COMMANDS      3 // for right now

// Exception + Error message strings
#define MSG_NO_INPUT_FILE       "No input file? Kill yourself.\n"
#define MSG_BAD_ALLOCATION      "Could not allocate buffer.\n"
#define MSG_BAD_INPUT_FILE_PATH "Could not open file at path %s.\n"
#define MSG_BAD_INPUT_FILE_READ "Could not read input file.\n"

// My personal preference of basic type names
typedef int          s32;
typedef unsigned int u32;
typedef u32          b32;
#define TRUE            1
#define FALSE           0

typedef struct
{
    s32  Cells[CELL_TABLE_LENGTH];
    s32 *Pointer;
} cell_table;

#include "commands.c"

typedef void (*function)(cell_table *table);
typedef struct
{
    char     *ComString;  // Passed command string
    function  Function;   // Associated function
} interpreter;

static const interpreter gInterpreters[NUM_COMMANDS] = {
    {"...", DoNothing},
    {"die", Increment},
    {"please", Decrement}
};

/*
 * Takes in a command string and returns the associated function
 */
static function
InterpretCommand(char *command)
{
    if (!command) return DoNothing;
    for (int i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(gInterpreters[i].ComString, command))
            return gInterpreters[i].Function;
    }
    return DoNothing;
}

/*
 * Reads the source file at the given path
 * and outputs it as a character string.
 */
static char*
ReadSource(char *path)
{
    FILE *source_file;
    char *source_buffer;
    u32 size;

    // Try to open the file
    source_file = fopen(path, "r");
    if (!source_file) {
        fprintf(stderr, MSG_BAD_INPUT_FILE_PATH, path);
        return NULL;
    }

    // Getting the size of the file
    fseek(source_file, 0L, SEEK_END);
    size = ftell(source_file);
    rewind(source_file);

    // Create and read to source buffer
    source_buffer = (char*) calloc(1, size + 1);
    if (!source_buffer) {
        fclose(source_file);
        fprintf(stderr, MSG_BAD_ALLOCATION);
        return NULL;
    }

    if (fread(source_buffer, size, 1, source_file) != 1) {
        fclose(source_file);
        free(source_buffer);
        fprintf(stderr, MSG_BAD_INPUT_FILE_READ);
        return NULL;
    }

    fclose(source_file);
    return source_buffer;
}

/*
 * Preprocesses the input file and outputs a list of
 * commands to process.
 */
static void
Preproc(char *command_string)
{

}

/*
 * Processes a list of commands on the given table and pointer
 * Will break if command buffer is
 */
static void
Process(cell_table *table, u32 count, function commands[])
{
    for (size_t i = 0; i < count; i++)
    {
        commands[i](table);
    }
}

int
main(int argc, char **argv)
{
    // No input file sent
    if (argc < 2) {
        printf(MSG_NO_INPUT_FILE);
        return 0;
    }

    char *source_file_path = argv[argc-1];
    printf("Read in filename %s.\n", source_file_path);

    char *source_buffer = ReadSource(source_file_path);
    printf("Source: \n=>\n%s\n", source_buffer);

    cell_table MainCellTable;
    MainCellTable.Pointer = MainCellTable.Cells;



    free(source_buffer);
    return 0;
}
