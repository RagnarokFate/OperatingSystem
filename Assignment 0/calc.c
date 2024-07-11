#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int i, num1, num2, result, flag = 0, flag2 = 0;
    char op, input[10];
    char space = 32;
    char exit[] = "exit";
    while(1){
        flag = 0;

        fgets(input, sizeof(input), stdin);
        //check if the user entered exit at first
        for(i=0; i < 4; i++){
            if(input[i] != exit[i])
                break;
        }
        if(i == 4)
            break;// if the user clicked exit at first you should exit

       num1 = input[0]; // we check if the first index is integer
       if(num1 < 48 || num1 > 57 ){
        flag = 1;
       }
       else{
        num1 = num1 -48; // to get the number not the ascii
       }
        //we check if the second index is space
        if (input[1] != space){
            flag = 1;
        }

        //now we check if the third index is an operation
        if (input[2] != '+' && input[2] !='-' && input[2] != '*' && input[2] != '/'){
            flag =1;
        }

        else{
            op = input[2]; // op = the operation the user asked
        }

        // now we want to to check the second space
        if(input[3] != space){
            flag = 1;
        }

        num2 = input[4]; // check the second number

        if(num2 < 48 || num2 > 57 ){
        flag = 1;
        }
        else{
        num2 = num2 -48; // to get the number not the ascii
        }

        //printf("expression length is: %d \n", strlen(input));
        if(strlen(input) > 6){
            flag = 1;
        }
        if(flag != 1){

            if (op=='+'){
                result = num1 + num2;
            }
            if(op == '-'){
                result = num1 - num2;
            }
            if (op == '*'){
                result = num1 * num2;
            }
            if(op == '/'){
                if (num2 != 0) {
                    result = num1 / num2;
            }
            else{
                printf("error, division by zero\n");
                flag = -1;

            }

            }
        }





        if(flag == 0)
            printf("%d\n", result);


        if (flag ==1){
            printf("%s", input);
        }

        //printf("%d,  %c", input[5], input[5]);


    }



    return 0;
}

