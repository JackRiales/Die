/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

// Memory table length
#define CELL_TABLE_LENGTH 9001 // TODO(Jack) Find out the highest number this can be.
#define NUM_RESERVES      7    // Convenience value for linear looping of reserves. Just for now.
#define NUM_COMMANDS      11   // See above. :)

// Exception + Error message strings
#define MSG_NO_INPUT_FILE       "No input file? Kill yourself.\n"
#define MSG_BAD_ALLOCATION      "Could not allocate buffer.\n"
#define MSG_BAD_INPUT_FILE_PATH "Could not open file at path %s.\n"
#define MSG_BAD_INPUT_FILE_READ "Could not read input file. Bytes read: %d\n"

// My personal preference of basic type names
typedef int          s32; // Integer types
typedef unsigned int u32;
typedef u32          b32; // Booleans
#define TRUE            1
#define FALSE           0

/*
 * Main cell table structure.
 * Since I'm originally going for a brainfuck style language,
 * this object will be responsible for the table of numbers (I call them cells)
 * and the interactive pointer, that can be moved around.
 * I may want to have a more robust language in the future and this may be replaced
 * with some sort of heap or stack implementation to hold variables or something.
 */
typedef struct {
    s32  Cells[CELL_TABLE_LENGTH];
    s32 *Pointer;
} cell_table;

/*
 * Command function callback template.
 * For the moment, we're only worried about passing in the cell table
 * so they can increment, move the pointer, etc.
 * Later, this will probably have a lot more capability.
 */
typedef void (*function)(cell_table *table);

/*
 * Interpreters are used to create associative arrays
 * of command strings (i.e. "die") and their associated function.
 * Later when we get around to variable commands, this might be deprecated
 * in favor of a more complex system that can handle things like that.
 * All we can do now is do string-lookups to functions.
 */
typedef struct
{
    char     *ComString;  // Passed command string
    function  Function;   // Associated function
} interpreter;

/*
 * A structure-of-arrays style struct that holds all the tokens
 * parsed out of a die source file. Effectively an array of strings (tokens)
 * and a convenient count property.
 */
typedef struct
{
    u32    Count;
    char **TokenList;
} source_tokens;

/*
 * Variable struct for user-defined variables, included with a name and an associated cell.
 * In the future I may allow assignment operations, which may involve either static or dynamic
 * typing, but for right now I merely want them used to name and reference cells.
 * For example, `you idiot` => [0] is named idiot, and `hey idiot` returns the pointer to [0].
 */
typedef struct
{
    char *Name;
    s32  *CellAddress;
} var;
