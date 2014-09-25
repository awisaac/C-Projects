// COMP 282 Lab 3 - catcher.c
// Andrew Isaac

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>

// converts signal int to char * for output
char *number2sig_nhame(int i) {
  
  static char *signame;
  
  if (i == SIGHUP) { signame = "HUP" ;}
  if (i == SIGINT) { signame = "INT"; }
  if (i == SIGQUIT) { signame = "QUIT"; }
  if (i == SIGILL) { signame = "ILL"; }
  if (i == SIGTRAP) { signame = "TRAP"; }
  if (i == SIGABRT) { signame = "ABRT"; }
  if (i == SIGIOT) { signame = "IOT"; }
  if (i == SIGBUS) { signame = "BUS"; }
  if (i == SIGFPE) { signame = "FPE"; }
  if (i == SIGKILL) { signame = "KILL"; }
  if (i == SIGUSR1) { signame = "USR1"; }
  if (i == SIGSEGV) { signame = "SEGV"; }
  if (i == SIGUSR2) { signame = "USR2"; }
  if (i == SIGPIPE) { signame = "PIPE"; }
  if (i == SIGALRM) { signame = "ALRM"; }
  if (i == SIGTERM) { signame = "TERM"; }
  if (i == SIGSTKFLT) { signame = "STKFLT"; }
  if (i == SIGCHLD) { signame ="CHLD"; }
  if (i == SIGCONT) { signame = "CONT"; }
  if (i == SIGSTOP) { signame = "STOP"; }
  if (i == SIGTSTP) { signame = "TSTP"; }
  if (i == SIGTTIN) { signame = "TTIN"; }
  if (i == SIGTTOU) { signame = "TTOU"; }
  if (i == SIGURG) { signame = "URG"; }
  if (i == SIGXCPU) { signame ="XCPU"; }
  if (i == SIGXFSZ) { signame ="XFSZ"; }
  if (i == SIGVTALRM) { signame = "VTALRM"; }
  if (i == SIGPROF) { signame = "PROF"; }
  if (i == SIGWINCH) { signame = "WINCH"; }

  return signame;
}

// converts string command line arguments into ints for input
int sig_name2number(char *s) {
  int signumber = -1;

  if (!strcmp(s, "HUP"))     { signumber = SIGHUP; }
  if (!strcmp(s, "INT"))     { signumber = SIGINT; }
  if (!strcmp(s, "QUIT"))    { signumber = SIGQUIT; }
  if (!strcmp(s, "ILL"))     { signumber = SIGILL; }
  if (!strcmp(s, "TRAP"))    { signumber = SIGTRAP; }
  if (!strcmp(s, "ABRT"))    { signumber = SIGABRT; }
  if (!strcmp(s, "IOT"))     { signumber = SIGIOT; }
  if (!strcmp(s, "BUS"))     { signumber = SIGBUS; }
  if (!strcmp(s, "FPE"))     { signumber = SIGFPE; }
  if (!strcmp(s, "KILL"))    { signumber = SIGKILL; }
  if (!strcmp(s, "USR1"))   { signumber = SIGUSR1 ; }
  if (!strcmp(s, "SEGV"))   { signumber = SIGSEGV ; }
  if (!strcmp(s, "USR2"))   { signumber = SIGUSR2 ; }
  if (!strcmp(s, "PIPE"))   { signumber = SIGPIPE ; }
  if (!strcmp(s, "ALRM"))   { signumber = SIGALRM ; }
  if (!strcmp(s, "TERM"))   { signumber = SIGTERM ; }
  if (!strcmp(s, "STKFLT")) { signumber = SIGSTKFLT ; }
  if (!strcmp(s, "CHLD"))   { signumber = SIGCHLD ; }
  if (!strcmp(s, "CONT"))   { signumber = SIGCONT ; }
  if (!strcmp(s, "STOP"))   { signumber = SIGSTOP ; }
  if (!strcmp(s, "TSTP"))   { signumber = SIGTSTP ; }
  if (!strcmp(s, "TTIN"))   { signumber = SIGTTIN ; }
  if (!strcmp(s, "TTOU"))   { signumber = SIGTTOU ; }
  if (!strcmp(s, "URG"))    { signumber = SIGURG ; }
  if (!strcmp(s, "XCPU"))   { signumber = SIGXCPU ; }
  if (!strcmp(s, "XFSZ"))   { signumber = SIGXFSZ ; }
  if (!strcmp(s, "VTALRM")) { signumber = SIGVTALRM ; }
  if (!strcmp(s, "PROF"))   { signumber = SIGPROF ; }
  if (!strcmp(s, "WINCH"))  { signumber = SIGWINCH ; }

  return signumber;
}

// handles signals received
void handler(int signum) {
   
   signal(signum, handler); // unreliable signals need to be re-registered
   static int tCount = 0, sCount = 0;
   
   sCount++; // count total signals received
         
   printf("Signal %s caught at %d\n",number2sig_nhame(signum),time(NULL));
   
   // counts number of TERM signals consecutively received   
   if (signum == SIGTERM) {
      tCount++;
   } 
   else {
      tCount = 0;
   }
   
   // 3 consecutive term signals ends program   
   if (tCount >= 3) {
      fprintf(stderr,"catcher: Total signals count = %d\n",sCount);
      exit(0);
   }   
}

int main(int argc, char *argv[]) {
   
   int i;
   void (*pHandler)(int);
   int signame;
   
   // output pid
   fprintf(stderr,"catcher: $$ = %d\n", getpid()); 
   
   // register handler for each argument 
   for (i = 1; i < argc; i++) {
      signame = sig_name2number(argv[i]); 
      pHandler = signal(signame,handler);
      
      // exit for unknown signal
      if (pHandler == SIG_ERR) {
         exit(2);
      }
   }
   
   // wait until signal received recieved and then wait again
   while (1) {
   
      pause();
   }
   
   return 0;
}