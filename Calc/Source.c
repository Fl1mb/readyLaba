#include "Parser.h"
#include "Summer.h"

int main()
{       
    char input[2500];
    printf("Input expression: \n");
    gets(input);
    char* new_input = delete_spacing(input);
    
    if (checkForSum(new_input)) {
        BigNumber result = getSum(new_input);
        printNumber(&result);
        freeBigNumber(&result);
    }
    else {
        BigNumber result = evaluate(new_input);
        printNumber(&result);
        freeBigNumber(&result);
    }
    free(new_input);

    return 0;
}
