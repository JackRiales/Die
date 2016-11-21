/*
 *  DIE Language | Jack Riales
 *  A Language for the Aggressive Programmer, based on the ideas of Brain^&*#.
 */

// Function declaration macro
#define COMMAND(ComName) void ComName(cell_table *table)

/*
 * Com: DoNothing
 * Function: Does nothing!
 */
COMMAND(DoNothing)
{
  return;
}

/*
 * Com: Increment
 * Function: Increments the currently pointed-to value.
 */
COMMAND(Increment)
{

}

/*
 * Com: Decrement
 * Function: Decrements the currently pointed-to value.
 */
COMMAND(Decrement)
{

}
