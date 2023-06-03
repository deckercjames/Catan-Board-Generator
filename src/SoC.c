
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * All resource types in the game (used for both ports and hexes)
 */
typedef enum resources
{
    WOOD       = 'w',
    BRICK      = 'b',
    WHEAT      = 'g',
    SHEEP      = 's',
    ORE        = 'o',
    DESERT     = 'd',
    ANTHING    = 'a',
    TERMINATOR = '\0',
} resources_t;

typedef struct resource_frequencies{
    resources_t resource;
    int frequency;
} resource_frequencies_t;

/**
 * A list of resources that can appear on the board hexes, and their frequencies
 * 
 * Must end with terminator element
 */
const resource_frequencies_t hex_resources[] = {
    {WOOD,       4},
    {BRICK,      3},
    {WHEAT,      4},
    {SHEEP,      4},
    {ORE,        3},
    {DESERT,     1},
    {TERMINATOR, 0} // Null Terminator
};

/**
 * A list of resources that can appear on the ports, and their frequencies
 * 
 * Must end with terminator element
 */
const resource_frequencies_t port_resources[] = {
    {WOOD,       1},
    {BRICK,      1},
    {WHEAT,      1},
    {SHEEP,      1},
    {ORE,        1},
    {ANTHING,    4},
    {TERMINATOR, 0} // Null Terminator
};

/**
 * Populates resources_out with the appropriet number of each resource
 * 
 * @param resoucres_out the array to write resources to
 * @param resources_size the size of the output array
 * @param resource_frequencies information about the frequency of each resource. Must be terminated
 */
void get_resources(resources_t *resources_out, size_t out_size, const resource_frequencies_t *resource_frequencies)
{
    // Current index of resources_out
    size_t idx = 0;
    // For each valid resource
    for (const resource_frequencies_t *resource_frequency = resource_frequencies; resource_frequency->resource; resource_frequency++)
    {
        // Add the correct number for this valid resource
        for (int i = 0; i < resource_frequency->frequency; i++)
        {
            if (idx >= out_size)
            {
                fprintf(stderr, "Error: More resources than there is space for");
                return;
            }
            resources_out[idx] = resource_frequency->resource;
            idx++;
        }
    }
}

/**
 * Randomizes an array of resources
 * 
 * @param resources the array to randomize
 * @param size the number of elements in the array
 */
void randomize(resources_t *recources, size_t size)
{
    for(size_t i = size; i > 0; i--)
    {
        size_t randHex = rand() % i;
        resources_t temp = recources[i-1];
        recources[i-1] = recources[randHex];
        recources[randHex] = temp;
    }
}

/**
 * Coppies the base_nums to the out_nums, inserting a zero at the same index of any desert hexes. All numbers
 * to the right of the desert are shifted to the right
 * 
 * Requires:
 * The out_nums is at least as long the hexes array
 * 
 * behavior is undefined if out_nums overlaps with base_nums
 * 
 * @param hexes the order of the hexes in the game
 * @param hexes_size the size of the hexes array
 * @param base_nums the numbers to copy to out_nums
 * @param base_nums_size the size of the base_nums array
 * @param out_nums the array to write the numbers to. Must be at least as long as the hexes array
 */
void insert_zeros_for_desert(const resources_t *hexes, size_t hexes_size, const int *base_nums, size_t base_nums_size, int *out_nums)
{
    size_t base_num_idx = 0;
    for (size_t i = 0; i < hexes_size; i++)
    {
        if (hexes[i] == DESERT)
        {
            out_nums[i] = 0;
            continue;
        }
        if (base_num_idx >= base_nums_size)
        {
            fprintf(stderr, "Error: Not enough base numbers to copy");
            return;
        }
        out_nums[i] = base_nums[base_num_idx];
        base_num_idx++;
    }
}

/**
 * Writes the hexes to the standard out, with their numbers. Each line contains three attributes:
 * 
 * Location,Resource,Number
 * 
 * They are listed in alphebetic order by location. Each line is terminated by a carrage return and a new line
 * 
 * @example
 * A,s,5
 * B,b,2
 * C,w,6
 * ...
 * 
 * @param hexes the hexes to write out
 * @param nmbers the numbers for each hex
 * @param placement_order the order the hexes should be placed in
 * @param the size for all input arrays
 */
void ouput_board(resources_t *hexes, int *numbers, const char *placement_order, size_t size)
{
    char location_to_idx[size];
    for (size_t i = 0; i < size; i++)
    {
        location_to_idx[placement_order[i]-'A'] = i;
    }
    
    for(size_t i = 0; i < size; i++)
    {
        //the letter that is being written
        const char letter = (char)('A'+i);
        
        //get the index in the letter in the placement order
        int index = location_to_idx[i];
        
        // Write to stdout
        printf("%c,%c,%d\r\n", letter, hexes[index], numbers[index]);
    }
}

/**
 * argv[1] is an optional integer to seed the random generator
*/
int main(int argc, char **argv)
{
    const size_t num_tiles = 19;
    const size_t num_ports = 9;
    const char placement_order[num_tiles] = {'A', 'B', 'C', 'G', 'L', 'P', 'S', 'R', 'Q', 'M', 'H', 'D', 'E', 'F', 'K', 'O', 'N', 'I', 'J'};
    
    // Seed the random generator
    if (argc > 1)
    {
        srand(atoi(argv[1]));
    }
    else
    {
        srand(time(NULL));
    }
    
    // Get the hexes
    resources_t hexes[num_tiles];
    get_resources(hexes, num_tiles, hex_resources);
    randomize(hexes, num_tiles);
    
    // Get the numbers
    int base_numbers[] = {5, 2, 6, 3, 8, 10, 9, 12, 11, 4, 8, 10, 9, 4, 5, 6, 3, 11};
    int true_numbers[num_tiles];
    insert_zeros_for_desert(hexes, num_tiles, base_numbers, sizeof(base_numbers)/sizeof(int), true_numbers);
    
    // Write hexes to stdout
    ouput_board(hexes, true_numbers, placement_order, num_tiles);
    
    // Ports
    resources_t ports[9];
    get_resources(ports, num_ports, port_resources);
    randomize(ports, num_ports);
    for (size_t i = 0; i < num_ports; i++)
    {
        printf("P%c,%c,x\r\n", (char)('a'+i), ports[i]);
    }
}