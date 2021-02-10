#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main( int argc, char* argv[]){
    FILE *f = fopen(argv[1], "r");
    FILE *w = fopen(argv[2], "w");
    char line[256];
    char linec[256];

    if( f == NULL || w == NULL ){
        printf("Error file not found.\n");
        return 0;
    }

    while( fgets ( line, sizeof line, f ) != NULL ){
        fputs( line, stdout );
        strcpy( linec, line );

        fprintf( w, linec );
    }
    fclose( w );
    fclose( f );
    return 0;
}
