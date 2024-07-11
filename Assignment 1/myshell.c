#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 100

typedef enum { false, true } bool;

int main(void)
{


    typedef struct node
    {
        int execution_order;
        char command[BUFFER_SIZE];
        bool is_background;
    } history_node;


    close(2);
    dup(1);
    char command[BUFFER_SIZE];

    char* argv[BUFFER_SIZE];

    history_node history[BUFFER_SIZE];

    int history_size=0,i=0;
    bool is_background=false,insert_to_history=false,execute=false;
    pid_t pid;


    while (1)
    {
        fprintf(stdout, "my-shell> ");
        memset(command, 0, BUFFER_SIZE);
        fgets(command, BUFFER_SIZE, stdin);
        if(strncmp(command, "exit", 4) == 0)
        {
            break;
        }

        command[strlen(command)-1]='\0';  //replace \n with null


        //my code

        insert_to_history=true;
        execute=true;

        i=0;
        while(command[i]==' ' ||command[i]==9)
        {
            i++;
        }

        if(command[i]=='\0')
        {
            insert_to_history=false;
            execute=false;
        }





        if(command[0]=='!')
        {

            int number=0;
            int dig=0;
            i=1;

            if(command[i]=='!')
            {
                number=history_size;
            }
            else
            {

                while(command[i]-48>=0&&command[i]-48<=9)
                {
                    dig=(int)(command[i]-48);
                    number=number*10+dig;
                    i++;
                }
            }


            if(number <= history_size && number>0) ///insert to history and execute
            {

                history_size++;

                history[history_size-1].is_background =history[number-1].is_background;

                for(i=0; i<BUFFER_SIZE; i++)
                {
                    history[history_size-1].command[i]= history[number-1].command[i];
                }


                history[history_size-1].execution_order = history_size;


                insert_to_history=false;
                execute=true;
            }
            else
            {

                if(number>history_size) //print no history and do not insert to history do not execute
                {
                    insert_to_history=false;
                    execute=false;
                    printf("No History\n");
                }

                if(number==0  && command[1]!='0' ) //number<0 or not number insert to history
                {

                    insert_to_history=true;
                    execute=true;

                }
                if(number==0 && command[1]=='0')//do nothing !0
                {
                    insert_to_history=false;
                    execute=false;
                }

            }

        }

        if(insert_to_history)
        {

            history_size++;

            history[history_size-1].is_background =false;

            if(command[strlen(command)-1]=='&')
            {
                //printf("it is background \n");
                command[strlen(command)-1]='\0';
                history[history_size-1].is_background =true;
            }



            for(i=0; i<BUFFER_SIZE; i++)
            {
                history[history_size-1].command[i]=command[i];
            }

            history[history_size-1].execution_order = history_size;


        }

        if(strncmp(history[history_size-1].command,"history",7)==0 && execute) ///print the history
        {

            /// print history dawnward

            execute=false;

            for(int i=history_size-1; i>= 0; i--)
            {

                printf("%d %s",history[i].execution_order,history[i].command);
                if(history[i].is_background)
                {
                    printf(" &\n");
                }
                else
                {
                    printf("\n");
                }
            }
        }
        else
        {

            if(execute)
            {
                fflush(stdout);
                pid = fork();

                if(pid == -1)
                {
                    perror("error");
                    exit(1);
                }
                if(pid == 0) ///son
                {


                    for(i=0; i<BUFFER_SIZE; i++)
                    {
                        command[i]=history[history_size-1].command[i];

                    }

                    //printf("the command is: %s\n",command );

                    i = 0;
                    argv[i] = strtok(command," \n"); //the first token is the command itself
                    while (argv[i] != NULL)
                    {
                        i++;
                        argv[i]= strtok(NULL, " \n");
                    }
                    argv[i]=NULL;  //replace \n with null



                    execvp(argv[0], argv);
                    perror("error");
                    exit(0);    ///only if execvp failed

                }
                else ///father
                {
                    if(!history[history_size-1].is_background)
                    {
                        while (wait(NULL) != pid);  ///wait till the son exit
                    }
                }


            }

        }
    }

    return 0;
}
