/**************************************************/
/* Project1: Creating a Shell                     */
/* Programmer: Nahesha Paulection                 */
/* Course: CMPE320                                */
/* Section: 2(12pm-1:50pm)                        */
/* Instructor: S.Lee                              */
/**************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define BUFFER_LEN 1024

 void command_line(void);
 static char *args[512];
 static char history[50][50];
 int count_cmd_arg = 0;
 int history_count = 0;
 int run;
 int parent_wait = 1;
 char *read_line(void);
 char **parse_command(char *cmd);
 void execute_command(char** parsed_cmd);
 void print_history(void);
 void print_help(void);
// void execute_most_recent_his(void);

int main(void)
{
  char *line;
  char **cmd;

  int status;
  run = 1;
   printf("*************************************\n");
   printf("*                                   *\n");
   printf("* Welcome to Nahesha's Shell        *\n");
   printf("* type 'help' for previous commands *\n");
   printf("*                                   *\n");
   printf("*                                   *\n");
   printf("*************************************\n");

  while(run){
   printf("Nahesha's shell>> ");
   //reads the user input in
   line = read_line();
   //parses through the user input
   if(strstr(line,"&")!=NULL)
   {
       parent_wait = 0;
   }
   cmd = parse_command(line);

   //if the user writes the work exit, the program terminates
   if((strcmp(cmd[0],"exit"))== 0)
   {
     return 0;
   }
   // Executes the command given based on the user input
   execute_command(cmd);
   }

  return 0;
}

// allocates space for the input of the user and stores it into a variable
char *read_line(void)
{
  char* usr_input = malloc(BUFFER_LEN*sizeof(char));
  fgets(usr_input, BUFFER_LEN, stdin);
  return usr_input;

}
// splits up the command from the user by space and enter
// If enter is not included; there will be symbols to represent
// enter
char **parse_command(char *cmd)
{
//   if ((strcmp(args[0],"history")) != 0){
//    if ((strcmp(args[0],"!!")) != 0){
//       history_count++;
//       history[history_count] = cmd;
//     }
//   }
    char temp_cmd[50];
    strcpy(temp_cmd,cmd);
   if(strstr(temp_cmd,"history")==NULL)
    {
     if(strstr(temp_cmd,"!!")==NULL)
      {
       history_count++;
       strcpy(history[history_count],temp_cmd);
//       history[history_count] = temp_cmd;
       
       printf("\n%s", temp_cmd);       
      }
    }
 
   
 int i = 0;
    char *token;
    token = strtok(cmd," &\n");
    // goes through the command split them based on spce and if the user presses enter
    while(token != NULL){
      args[i] = token;
      i++;
      count_cmd_arg++;
      token = strtok(NULL," &\n");
   }

  args[i] = NULL;
  return args;

}

// Function where the command is executed
void execute_command(char** parsed_cmd)
{
    // Forking a child
    pid_t child_pid = fork();

    if (child_pid == -1) {
        printf("\nFailed forking child..");
        return;
    } else if (child_pid == 0) {
    // this is the child process
     setsid();
       //checks to see if the first word is history if so it will do the appropriate action

     if((strcmp(parsed_cmd[0],"history"))== 0)
     {
        print_history();
     }
     //if you enter !! it will execute the recently ran
    else if((strcmp(parsed_cmd[0],"!!"))== 0)
    {
        //as long as there is a history for the word bank, it execute the last command
       if(history_count != 0)
      { 

        execute_command(parse_command(history[history_count]));
        
       }
       else
       {
           printf("--No command in History--\n");
       }

    }
//     // checks whether it is equal to a single exclamation point
//    else if(strcmp((strstr(parsed_cmd[0],"!")),"!") == 0)
//    {
//                 
//    }
    else if((strcmp(parsed_cmd[0],"help"))== 0)
    {
        print_help();
    }
    // if the input given by the user is incorrect,
    //it will give the correct response otherwise run the command
    else if (execvp(parsed_cmd[0], parsed_cmd) < 0) 
    {
        printf("\nCould not execute command..\n");
        abort();
    }

        exit(0);
    } else {
        if(parent_wait == 1){
//         waiting for child to terminate
            int childStatus;
            waitpid(child_pid, &childStatus, 0);
            }
            parent_wait = 1;

    }

}
//prints the 10 most commands,
//it will continuously change the amount based
void print_history(void)
{
  int recent_ten = 0;
  if(history_count == 0)
  {
      printf("-- No commands were made --\n");
  }

 for(int i = history_count; i > 0; i--)
 {
    if(recent_ten != 10){
       printf("%d ",i);
       printf("%s\n",history[i]);
       recent_ten++;
    }

 }
    recent_ten = 0;
}

//prints out commands only for this specific program
void print_help(void)
{
   printf("To see your 10 most recent system calls, type in 'history'\n");
   printf("To exit this shell write 'exit'\n");

}
