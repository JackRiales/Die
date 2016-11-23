/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */


// Memory table length
#define CELL_TABLE_LENGTH 9001
#define NUM_COMMANDS      4 // for right now

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

typedef void (*function)(cell_table *table);
typedef struct
{
    char     *ComString;  // Passed command string
    function  Function;   // Associated function
} interpreter;

typedef struct
{
    u32    Count;
    char **TokenList;
} source_tokens;
