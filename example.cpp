#include "dumps_host.hpp"

int main()
{
    dumps::dump_game( "PUT_YOUR_APIKEY_HERE" , dumps::game::splitgate , true );

    printf( "\n" );

    while ( 1 ) { }
}
