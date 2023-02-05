//inclusion of all the necessary libraries to compile this program
#include <stdio.h>
#include <sys/sysinfo.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <utmp.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


//function to print information about the memory
void printMemoryInfo(int sample, int delay, bool g){ 
   
   //for conversion from bytes to gigabytes
   double GB = 1024 * 1024 *1024; 
   
   //we create a sysinfo struct to access information about the memory usage of our machine
   struct sysinfo sys;
   sysinfo (&sys);

   //line printed for user-friendly visualization
   printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot)\n");
   
   //if the graphics flag is not being used
   if(!g){
   for(int i = 0; i<sample;i++){
      printf ("%.2f GB / %.2f GB -- %.2f GB / %.2f GB\n", (sys.totalram-sys.freeram) / GB, sys.totalram / GB, (sys.totalram+sys.totalswap-sys.freeswap - sys.freeram)/GB, (sys.totalswap + sys.totalram)/GB);
      sleep(delay);
      }
   }//if the graphics flag is being used
   else{
      float mem1 = sys.totalram-sys.freeram;//we calculate physically used memory at the beginning of our sample and store it in mem1
      for(int i = 0; i<sample;i++){
         float mem2 = sys.totalram-sys.freeram;
         //we print out the values of memory as required
         printf ("%.2f GB / %.2f GB -- %.2f GB / %.2f GB ", (sys.totalram-sys.freeram) / GB, sys.totalram / GB, (sys.totalram + sys.totalswap - sys.freeswap - sys.freeram)/GB, (sys.totalswap + sys.totalram)/GB);
         //for graphical representations

         printf("   |");//this is a line beside every line of memory usage. it will always exist
         
         float diff = ((mem2-mem1)/GB)*100;//here we calculate the difference between the memory usages of the 2 samples
         if(i>0){
            //if there difference is positive we print out a : for every 0.01 increase
            if(diff>0){
               for(int j=0;j<diff;j++){
                  printf(":");
               }
               printf("@   ");// we end it with a @
            }
            //if there difference is positive we print out a : for every 0.01 decrease
            else if(diff<0){
               for(int j=0;j>diff;j--){
                  //if there difference is negative we print out a # for every 0.01 decrease
                  printf("#");
               }
               printf("*   "); //we end it with a #
            }
            //if there is no difference, then we just print out an o to signal that
            else if(diff==0){//
               printf("o   ");
            }
            //store the value of mem2 in mem 1
            mem1 = mem2;
            //mem2 will now have a new value
            mem2 = sys.totalram-sys.freeram;
            
            sleep(delay);
         }
         //we print out the difference at the end of the line 
         printf("%.2f (%.2f)\n", diff, mem1/GB);
      }
   }
   printf("------------------------------------------\n");
   
}

//function to print the users that are connected to the system and which sessions they are connected to
void printSessionUsers(int sample, int time){
   
   //line for user-friendliness
   printf("### Session/users ###\n");
   
   //we check how many users and what sessions they are connected to sample number of times
   //each with time seconds of delay
   for(int i=1;i<sample+1;i++){
      //we create a utmp struct pointer called user to access information about who is currently using the machine/system
      //and how many sessions they are connected to
      struct utmp *user;
         setutent();
         user=getutent();
         int x = 0;
         while(user) {
            //to prevent lines such as: 
            //reboot           ~ (5.13.0-28-generic)
            //runlevel         ~ (5.13.0-28-generic)
            //from showing up in output
            if(user->ut_type==USER_PROCESS) {
            // we print out the name of the user, the session name and the host
            printf("%9s%12s (%s)\n", user->ut_user, user->ut_line, user->ut_host);
            x++;      
            }
         user=getutent();
         }

      //usage of escape sequences for user-friendly visualization
      if(i!=(sample)){
         printf("Sample: %d\n",i);//we print out what sample we are currently printing so that the user can understand which sample each sample of data corresponds to
         sleep(time);
         for(int z = 0; z<x;z++){
            printf("\033[A");//we move the cursor up the number of times we printed each user
         }
         printf("\033[1F");//we move cursor the beginning of the previous line
         fflush(stdout);
      }
   }
   printf("------------------------------------------\n");
}




void printCpuInfo(int sample, int delay, bool g){

   printf("### CPU Information and Statistics ###\n");
   
   //counter variable to count how many cores there are
   int num_processors = 0;

   //-------------------------------------------------------------------------------------------
   //COUNTING NUMBER OF PROCESSORS:
   
   //we open file /proc/cpuinfo as it contains the information we want
   FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
   //must check if we were successful in opening the file or else we will face a segmentation fault
   if(cpuinfo==NULL){
      printf("/proc/cpuinfo file could not be opened\nHence, the number of cores could not be printed\n");
   }
   else{
      //word[256] acts as a buffer
      //as we iterate through every word in the file we check how many times the word processor occurs in the file
      //the number of times "processor" occurs in the file is the number of cores in the machine/computer
      char word[256];
      while(fscanf(cpuinfo, "%s", word)!=EOF){
         if(strcmp(word, "processor")==0){
         //incrementing our counter variable that we created at the beginning
         num_processors+=1;
         }
      }
   }
   //we must close the file we have opened
   fclose(cpuinfo);
   printf("Number of cores: %d\n", num_processors);
   //-------------------------------------------------------------------------------------------


   //-------------------------------------------------------------------------------------------
   //Calculating and displaying information about CPU usage
   for(int i=0; i<sample;i++){

      //every word in the file we will open is in a char array format and this includes numbers
      //hence, we create this char arrays to store the numbers that we wish to work with
      char wword[256];
      char struser[15];
      char strnice[15];
      char strsystem[15];
      char stridle[15];
      char striowait[15];
      char strirq[15];
      char strsoftirq[15];
      char strsteal[15];

      //these are long variables 
      //we convert the number stored in our char array variables into integers and store them in these variables
      //the point of this is so that we can perform mathematical calculations such as addition, subtraction etc on them
      long user;
      long nice;
      long system;
      long idle;
      long iowait;
      long irq;
      long softirq;
      long steal;

      //we open file /proc/stat as it contains the information we need
      FILE *stat = fopen("/proc/stat", "r");
      if(stat==NULL){
      printf("/proc/stat file could not be opened\nHence, the information about cpu usage could not be printed\n");
      }else{
      //we scan every word the FIRST line of the file as it contains the information we need
      //the first word is just "cpu" so we store it and don't need it
      fscanf(stat, "%s", wword);

      //from now onwards, we scan every string of numbers, store them in the array variables, convert them to integers and store them in their long variables
      fscanf(stat, "%s", struser);
      user = atoi(struser);
      fscanf(stat, "%s", strnice);
      nice = atoi(strnice);
      fscanf(stat, "%s", strsystem);
      system = atoi(strsystem);
      fscanf(stat, "%s", stridle);
      idle = atoi(stridle);
      fscanf(stat, "%s", striowait);
      iowait = atoi(striowait);
      fscanf(stat, "%s", strirq);
      irq = atoi(strirq);
      fscanf(stat, "%s", strsoftirq);
      softirq = atoi(strsoftirq);
      fscanf(stat, "%s", strsteal);
      steal = atoi(strsteal);
      }
      fclose(stat);

      //for calculations
      //information about this can be found in the documentation
      long prevnonidle = user+nice+system+irq+softirq+steal;
      long previdle = idle+iowait;

      //we wait for the delay amount of seconds
      sleep(delay);


      //we open file /proc/stat again as it contains the information we need
      stat = fopen("/proc/stat", "r");
      //,ust check if we have opened the file properly or not
      if(stat==NULL){
         printf("/proc/stat file could not be opened\nHence, the information about cpu usage could not be printed\n");
      }
      else{
      //we scan every word the FIRST line of the file as it contains the information we need
      //the first word is just "cpu" so we store it and don't need it
      fscanf(stat, "%s", wword);
      //from now onwards, we scan every string of numbers, store them in the array variables, convert them to integers and store them in their long variables
      fscanf(stat, "%s", struser);
      user = atoi(struser);
      fscanf(stat, "%s", strnice);
      nice = atoi(strnice);
      fscanf(stat, "%s", strsystem);
      system = atoi(strsystem);
      fscanf(stat, "%s", stridle);
      idle = atoi(stridle);
      fscanf(stat, "%s", striowait);
      iowait = atoi(striowait);
      fscanf(stat, "%s", strirq);
      irq = atoi(strirq);
      fscanf(stat, "%s", strsoftirq);
      softirq = atoi(strsoftirq);
      fscanf(stat, "%s", strsteal);
      steal = atoi(strsteal);
      }
      //we must close the file
      fclose(stat);

      //for calculations
      //information about this can be found in the documentation
      long nonidle = user+nice+system+steal+irq+softirq;
      long iidle = idle+iowait;
      long prevtotal = previdle+prevnonidle;
      long total = iidle+nonidle;
      long totald = total - prevtotal;
      long idled = iidle - previdle;
   
      double val1 = totald-idled;
      double val2 = totald;

      float percent = (val1/val2)*100;

      //if -g or --graphics is used, then we must produce graphical output
      if(g){
         printf("                ");
         for(int i = 0; i<=percent;i++){
            printf("|");
         }  
         printf(" %.2f \n", percent);
      }
      
      if(i<(sample-1)){
         printf("Total cpu use: %.2f %% \n", percent);
         //escape commands to make our program look more user-friendly
         
         printf("\033[A");//move up one line
         fflush(stdout);
         printf("\033[4D");//move to the left 4 columns
      }
      if(i==(sample-1)){
         printf("Total cpu use: %.2f %% \n", percent);
         }
   }
      printf("------------------------------------------\n");
}

//function to print out information about the system
void printSystemInfo(){

   //creating a utsname struct object called 
   struct utsname system;
      //we must check if we have created the system struct properly 
      if(uname(&system) == -1){
         printf("FAILURE! Call to uname did not execute\n System information cannot be printed out\n");
      }else{
         printf("###System Information###\n");
         printf("System Name      = %s\n", system.sysname);//prints out the name of the system
         printf("Machine name     = %s\n", system.nodename);//prints out the node/machine name
         printf("Release          = %s\n", system.release);//prints out the release of the system
         printf("Version          = %s\n", system.version);//prints out the version of the system 
         printf("Architecture     = %s\n", system.machine);//prints out the architecture of the machine/system
      }
   printf("------------------------------------------\n");
}



//function to check if a string is a number
//we pass in a pointer to a string and iterate through each character to confirm if the string is an integer or not
//negative integers are not counted 
bool isNumber(char *string){
   for(int i = 0; string[i]!='\0';i++){
      if(!isdigit(string[i])){
         return false;
      }
   }

   //now that we know the string is an integer, we need to check if it is negative or positive
   //we convert the string to an integer and check if it is less than or equal to 0
   int a = atoi(string);
   if(a<=0){
      return false;
   }
   return true;
}



//main function
int main(int argc, char **argv){
	
   //counter variables to check which information to print and how many times they are being entered by the user
   //for example, if --user is entered twice then user == 2 and the program will not execute
   int user = 0;
   int system = 0;
   int graphics = 0;
   int tdelay = 0;
   int sample = 0;
   

   //default values for sample and delay
   int sample_val = 10;
   int delay_val = 1;

   //invalid is a boolean variable
   //if invalid ever equals true, then we will not execute the program as this means that the user has entered an incorrect command line argument
   bool invalid = false;

   //boolean variable for graphics
   //initally is set to false
   //if the user ever enters -g or --g, then g will equal true and information with graphics will be printed out 
   bool g = false;

   //no command line arguments are used
   //therefore, we run the whole program with default values of 10 samples with each having a 1 second delay and no graphical output
   if(argc == 1){
      printf("------------------------------------------\n");
		printf("Number of samples: %d -- every %d seconds\n", sample_val, delay_val);
      printf("------------------------------------------\n");
		printMemoryInfo(sample_val, delay_val, g);
		printSessionUsers(sample_val, delay_val);
		printCpuInfo(sample_val,delay_val, g);
		printSystemInfo();
      return 0;
	}
   
   //to handle commands such as:
   //.a.out -g
   //./a.out --graphics
   //meaning where there is only argument and it is for graphical output
   else if((argc==2 && strcmp(argv[1],"-g")==0) || (argc==2 && strcmp(argv[1],"--graphics")==0)){
      g=true;
      printf("------------------------------------------\n");
		printf("Number of samples: %d -- every %d seconds\n", sample_val, delay_val);
      printf("------------------------------------------\n");
      printMemoryInfo(sample_val, delay_val, g);
		printSessionUsers(sample_val, delay_val);
		printCpuInfo(sample_val,delay_val, g);
		printSystemInfo();
      return 0;
   } 

   //user has entered command line or positional arguments
   else if(argc>1){
      //for loop to check and strcmp each argument to identify what arguments they are 
      //and also to check if they are the correct arguments
      for(int i=1; i<argc;i++){
         if(strcmp(argv[i], "--user")==0){
            user +=1;
         }
         //user has entered --system
         else if(strcmp(argv[i], "--system")==0){
            system+=1;
         }
         //user has enetered --graphics
         else if(strcmp(argv[i], "--graphics")==0){
            graphics+=1;
            g=true;//g=true so whenever we print our information about our machine, we will use graphics
         }
         //user has entered -g
         else if(strcmp(argv[i], "-g")==0){
            graphics+=1;
            g=true;//similar as above. g=true so whenever we print our information about our machine, we will use graphics
         }
         //checking if user has entered --tdelay=n properly
         else if(strncmp(argv[i], "--tdelay=", 9)==0){
            int tru = 1;
            if(strlen(argv[i])<=9){
               invalid = true;
               //this means that the user has just entered --tdelay=.
               //user did not enter value for n in --tdelay=n
            }
            else{
               for(int j = 9; j<strlen(argv[i]);j++){
                  if(!isdigit(argv[i][j])){
                     tru = 0;
                     invalid = true;
                     //user has entered a character for value of n in --tdelay=n
                     //for example --tdelay=abc
                     //this is not valid
                  }
               }
               if(tru==1){
                  char num[strlen(argv[i])-9];
                  for(int z = 9, k =0; z<strlen(argv[i]);z++,k++){
                     num[k]=argv[i][z];
                  }
                  //user has entered correct format and we can now extract the value of n from --tdelay=n
                  delay_val = atoi(num);
                  tdelay+=1;
               }
               
            }
            
         }
         //checking if user has entered --samples=n properly
         else if(strncmp(argv[i], "--samples=", 10)==0){
            int tru = 1;
            if(strlen(argv[i])<=10){
               invalid = true;
               //this means that the user has just entered --samples=
               //user did not enter value for n in --samples=n
            }
            else{
               for(int j = 10; j<strlen(argv[i]);j++){
                  if(!isdigit(argv[i][j])){
                     tru = 0;
                     invalid = true;
                     //user has entered a character for value of n in --samples=n
                     //for example --samples=abc
                     //this is not valid
                  }
               }
               if(tru==1){
                  char num[strlen(argv[i])-10];
                  for(int z = 10, k =0; z<strlen(argv[i]);z++,k++){
                     num[k]=argv[i][z];
                  }
                  sample_val = atoi(num);
                  sample+=1;
                  //user has entered correct format and we can now extract the value of n from --tdelay=n
               }
            }
         }
         //checking is user has entered a command that is neither
         //--user or --system or --graphics or -g or a number
         else if(strcmp(argv[i], "--user")!=0 && strcmp(argv[i], "--system")!=0 && strcmp(argv[i], "--graphics")!=0 && strcmp(argv[i], "-g")!=0 && !isNumber(argv[i])){
            invalid = true;
            //user has entered an invalid command
         }
      }
   }
      

      //checking if user has entered sth like:
      //./a.out n where n is an integer
      //this is invalid
      if(isNumber(argv[argc-1])&&(!isNumber(argv[argc-2]))){
         printf("Enter correct commands\n");
         return 0;
      }

      //checking if user has entered sth like:
      //  ./a.out 12 --system 
      //  ./a.out 12 2 --user
      //these positional arguments must occur at the end
      if(!isNumber(argv[argc-1])){
         for(int i=1; i<(argc-1);i++){
            if(isNumber(argv[i])){
               invalid = true;
               
            }
         }
      }

      //checking if the last 2 arguments are positional arguments for tdelay and samples
      if(isNumber(argv[argc-1]) && isNumber(argv[argc-2])){
            //checking if the user has entered: ./a.out n m where n and m are integer positional arguments 
            if(argc==3){
               sample_val = atoi(argv[argc-2]);
               delay_val = atoi(argv[argc-1]);
               printf("------------------------------------------\n");
		         printf("Number of samples: %d -- every %d seconds\n", sample_val, delay_val);
               printf("------------------------------------------\n");
               printMemoryInfo(sample_val, delay_val, g);
               printCpuInfo(sample_val, delay_val, g);
               printSessionUsers(sample_val, delay_val);
               printSystemInfo();
               return 0;
               
            }
            else if(argc>3){
               for(int i = 1; i<argc-2;i++){
                  if(strcmp(argv[i], "--user")!=0 && strcmp(argv[i], "--system")!=0 && strcmp(argv[i], "-g")!=0 && strcmp(argv[i], "--graphics")!=0){
                     invalid = true;
                     //user has entered an invalid argument such as ./a.out abc --system 12 2 where abc is invalid
                  }
               }
               if(!invalid){
                  //arguments are valid so we extract the values of the positional arguments
                  sample_val = atoi(argv[argc-2]);
                  delay_val = atoi(argv[argc-1]);
                  //user has entered sth in the form of:
                  //   ./a.out -g n m
                  //           or
                  //   ./a.out --graphics n m
                  //where n and m are integer positional arguments
                  if((argc==4 &&strcmp(argv[1], "-g")==0) || (argc==4 && strcmp(argv[1], "--graphics")==0)){
                     printf("------------------------------------------\n");
                     printf("Number of samples: %d -- every %d seconds\n", sample_val, delay_val);
                     printf("------------------------------------------\n");
                     printMemoryInfo(sample_val, delay_val, g);
                     printCpuInfo(sample_val, delay_val, g);
                     printSessionUsers(sample_val, delay_val);
                     printSystemInfo();
                     return 0;
                  }
               }
            }
         }   
      
      
      //sth about the user inpur was illegal/invalid
      //hence, we exit the program
      if(invalid){
         printf("Enter correct commands\n");
         return 0;
      }


      //user has entered the same command more than once
      //this is not valid for our program
      //hence, we let the user know to enter the correct commadnds
      else if(user >1|| system>1 || graphics>1 || tdelay>1){
         invalid = true;
         printf("Enter correct commands\n");
      }

      //not invalid so we can execute
      if(!invalid){   
      printf("------------------------------------------\n");
		printf("Number of samples: %d -- every %d seconds\n", sample_val, delay_val);
      printf("------------------------------------------\n");
      //user has entered --system once
      if(system==1){
		   printMemoryInfo(sample_val, delay_val, g);
         printCpuInfo(sample_val, delay_val, g);
      }
      //user has entered --user once
      if(user == 1){
         printSessionUsers(sample_val, delay_val);
      }
      //user has entered only --tdelay=n and/or --samples=n
      if(system==0 && user == 0 && graphics == 0){
         if(tdelay==1 || sample == 1){
            printMemoryInfo(sample_val, delay_val, g);
            printCpuInfo(sample_val, delay_val, g);
            printSessionUsers(sample_val, delay_val);
         }
      }
      //user has entered the graphics flag only
      else if(graphics==1 && user==0 && system==0){
         printMemoryInfo(sample_val, delay_val, g);
               printCpuInfo(sample_val, delay_val, g);
               printSessionUsers(sample_val, delay_val);
      }
      //we print information about the system regardless of what argument/flag is used
      printSystemInfo();
      }
}