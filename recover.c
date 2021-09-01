/*
jpeg begins with a 4 byte block header at the start of every 512 byte jpeg. 
Open the memory card
read the whole card
read 512 bytes into a buffer
check the first 4 bytes if it is a Jpeg
if first Jpeg  
write Jpeg 001
if not first Jpeg, close previous Jpeg 
number the files it outputs by naming each ###.jpg
int sprintf(char *str, const char *format, [arg1, arg2, ... ]);
int main() {
    char output[50]; 
    int num1 = 3, num2 = 5, ans; 
    ans = num1 * num2; 
    sprintf(output, "%d multiplied by %d is %d", num1, num2, ans);  
    printf("%s", output); 
  
    return 0; 
}
OUTPUT
3 multiplied by 5 is 15
*/



#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
 
 
typedef uint8_t BYTE;

const int block_size = 512;
 
int main(int argc, char *argv[]) // This creates a pointer for the address of things in the command line arugments
{
     if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    
        // Open files and determine scaling factor
        FILE *input = fopen(argv[1], "r"); // creates a pointer to a file that should contain data (card.raw) and "r" = reads it
    if (input == NULL)
    {
        printf("Could not open file.\n"); // if it points to something that is empty (NULL) print this.
        return 1;
    }
    
    BYTE buffer [block_size]; //create an ARRAY of 512 bytes to store the data from the card input to a buffer / header.
    // uint8_t arrayName arraySize or 512 = HEADER_SIZE

    FILE *image = NULL; // creates a pointer to a file image that will store all the image data. We will have to close this

    int jpeg_count = 0; //Count must start at 000
    char file_name[20]; // how long the name of the file will be
    
    while ((fread(&buffer, sizeof(BYTE) , block_size , input)) == block_size) // creates a ptr to buffer of 512 bytes and keeps reading till no more blocks
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            
            if (jpeg_count != 0) // if it does not equal 0
            {
                fclose(image); // then I wilclose
                sprintf(file_name, "%03i.jpg", jpeg_count); //%03i means print an "i" integer with "3" digits to represent it
            }
            else 
            {
                sprintf(file_name, "%03i.jpg", jpeg_count); //%03i means print an integer with 3 digits to represent it
  
            }
            jpeg_count +=1; 
        }
        image = fopen(file_name, "w"); // Open file called image with the jpeg name
        fwrite(&buffer, block_size , 1, image);
    }

    fclose(input);
    fclose(image);
    return 0;
}
