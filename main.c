#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE   10

int main() {
    char buf[BUFF_SIZE];
    char *line, *insert;
    line = (char *)malloc( BUFF_SIZE );

    while ( 1 ) {
    //    int multiplier = 2;

        while ( !feof( stdin ) ) {
//            fgets( buf, BUFF_SIZE-1, stdin );

  //          line = (char *) realloc(line, BUFF_SIZE*multiplier - 1);
    //        insert = line + BUFF_SIZE*multiplier-2;
      //      memcpy(insert, buf, BUFF_SIZE - 1);

        //    if ( line[strlen(line)-1] == '\n' ) break;

            //buffer = (char *)realloc( buffer, BUFF_SIZE * multiplier );
            //tmp = (char *)malloc( BUFF_SIZE * multiplier );
            //memcpy( tmp, buffer, BUFF_SIZE * (multiplier - 1) );
            //free( buffer );
            //buffer = tmp;

          //  multiplier++;
        }

        puts( line );
        free( line );
        line = insert = 0;
    }


    return 0;
}
