/* CS360 Lab 4: C  - Derek Russell*/

#include <stdio.h>
#include <stdlib.h>
#define max_file_size  10485760

FILE *fp;

int initialize( int argc, char ** argv );

unsigned char * readFile();

int main( int argc, char ** argv )
{
	initialize(argc, argv);
    
    unsigned char * data = readFile();
    
    unsigned char header_bytes[4];

    int i;
    unsigned char mask = 1;
    for(i = 0; i < max_file_size; i++)
    {
        if(((data[i]) == 0xFF) && (data[i+1] & 0xe0) == 0xe0) 
        {
            header_bytes[0] = data[i];
            header_bytes[1] = data[i+1];
            header_bytes[2] = data[i+2];
            header_bytes[3] = data[i+3];

            break; 
        }
    }

    unsigned char header_in_bits_one[8];
    int j;

    for(j = 0; j < 8; j++)
    {
       header_in_bits_one[j] = ((header_bytes[0]) & (mask << j)) != 0;
    }

    unsigned char header_in_bits_two[8];
    for(j = 0; j < 8; j++)
    {
       header_in_bits_two[j] = ((header_bytes[1]) & (mask << j)) != 0;
    }
            
    unsigned char header_in_bits_three[8];
    for(j = 0; j < 8; j++)
    {

       header_in_bits_three[j] = ((header_bytes[2]) & (mask << j)) != 0;
    }
            
    unsigned char header_in_bits_four[8];
    for(j = 0; j < 8; j++)
    {

       header_in_bits_four[j] = ((header_bytes[3]) & (mask << j)) != 0;
    }


    if(header_in_bits_two[3] == 1)
    {
        if((header_in_bits_two[2]) == 0 && (header_in_bits_two[1] == 1))
        {
            if((header_in_bits_three[7] == 1) && (header_in_bits_three[6] == 0))
            {
                if((header_in_bits_three[5] == 0) && (header_in_bits_three[4] == 1))
                {
                    printf("Bit Rate: 128kbps at");
                }
                else if((header_in_bits_three[5] == 1) && (header_in_bits_three[4] == 0))
                {
                    printf("Bit Rate: 160kbps at");
                }
            else 
            {                
                printf("other bit rate");
            }
            }   
            else 
            {
                printf("bitrate print isnt working");
            }
   
        
            if((header_in_bits_three[3] == 0) && (header_in_bits_three[2] == 0))
            {
                printf(" 44.1kHz\n");
            }
            else if((header_in_bits_three[3] == 1) && (header_in_bits_three[2] == 0))
            {
                printf(" 32kHz \n");
            }
            else if((header_in_bits_three[3] == 0) && (header_in_bits_three[2] == 1))
            {
                printf(" 48kHz\n");
            }
            else
            {
                printf(" reserved kHz\n");
            }
    
        
            if((header_in_bits_four[3] == 0))
            {
                printf("Not Copyrighted\n");
            }
            else
            {
                printf("This is Copyrighted Material\n");
            }
        
        
            if((header_in_bits_four[2] == 0))
            {
                printf("Copy of Original Material\n");
            }
            else
            {
                printf("Original Material\n");
            }
    
            if((header_in_bits_four[2]) == 1 && (header_in_bits_four[2] == 0))
            {
                printf("----ALERT!! This is a copy of Copyrighted Material! ALERT!!----");
            } 
            
        }
        else
        {
            printf("----NOT a MPEG Layer3 So we Done!----");
        }
    }    
    
    fclose(fp);
    free(data);
    exit(EXIT_SUCCESS);	
}


int initialize( int argc, char ** argv )
{
    if( argc != 2 )
	{
		printf( "Usage: %s filename.mp3\n", argv[0] );
        return(EXIT_FAILURE);
	}
	
	fp = fopen(argv[1], "rb");
	
    if( fp == NULL )
	{
		printf( "Can't open file %s\n", argv[1] );
		return(EXIT_FAILURE);
	}
    return 1;
}

unsigned char * readFile()
{
	double size = 0;
	fseek( fp, 0, SEEK_END );
	size = ftell(fp);	
	rewind(fp);		
	
	if( size < 1 || size > max_file_size )
	{
		printf("File size is not within the allowed range\n"); 
        fclose(fp); 
        exit(EXIT_SUCCESS);	
	}
	
	printf( "File size: %.2f MB\n", (size / max_file_size) * 10);

	unsigned char * data = (unsigned char *)malloc(size);

	double bytesRead = fread( data, sizeof(unsigned char), size, fp );
	if( bytesRead != size )
	{
		printf( "Error reading file. Unexpected number of bytes read: %.2f\n",bytesRead );
        free(data);
        fclose(fp);
        exit(EXIT_SUCCESS);	
	}
	return data;
}
