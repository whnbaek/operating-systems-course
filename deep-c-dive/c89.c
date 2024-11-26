#include <stdio.h>


//Function will not return 
_Noreturn fun( )
{
    printf("\nI TOO WILL BE PRINTED :) \n\n");
    while(1);
}
 
int main( )
{
    printf("\nI WILL BE PRINTED :) \n");
    fun( );
    printf("\nI WILL NOT BE PRINTED :( WHY? \n");
}
