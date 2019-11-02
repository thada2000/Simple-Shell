#include<sys/resource.h>
#include<sys/wait.h>
#include<stdio.h>
#include<string.h>
#include<getopt.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<ctype.h>
#include<math.h>
#include<sys/time.h>

void signalHandler(int num){
  fprintf(stderr, "signal %d caught\n", num);
  exit(num);
}

struct processInfo {
  char* args[1000];
  int size;
};

int max(int a, int b){
  if(a>b)
    return a;
  else
    return b;
}

void ioRedir(int fd1,int fd2);

int main(int argc, char** argv){
  static struct option long_opts[]={
    {"rdonly",required_argument,0,1},
    {"wronly",required_argument,0,2},
    {"command",required_argument,0,3},
    {"verbose",no_argument,0,4},
    {"append",no_argument,0,5},
    {"cloexec",no_argument,0,6},
    {"creat",no_argument,0,7},
    {"directory",no_argument,0,8},
    {"dsync",no_argument,0,9},
    {"excl",no_argument,0,10},
    {"nofollow",no_argument,0,11},
    {"nonblock",no_argument,0,12},
    {"rsync",no_argument,0,13},
    {"sync",no_argument,0,14},
    {"trunc",no_argument,0,15},
    {"rdwr",required_argument,0,16},
    {"abort",no_argument,0,17},
    {"ignore",required_argument,0,18},
    {"default",required_argument,0,19},
    {"catch", required_argument,0,20},
    {"close", required_argument,0,21},    
    {"pause", no_argument, 0, 22},
    {"pipe", no_argument, 0, 23},
    {"wait",no_argument, 0, 24},
    {"profile",no_argument,0,25},
    {0,0,0,0}
  };
  opterr = 0;
  int size;
  int vflag=0;
  int exitCode = 0;
  int exitSignal=0;
  int fdsCommand[3];
  int fds = 0;
  int numF=0;
  int fdTable[1000];
  char* fdsOut[1000];
  char* argsCom[1000];
  int currHighestFD =2;
  struct processInfo processArgs[1000];
  pid_t runningProcesses[1000];
  int maxsizeFD = 100;
  int fflag = 0;
  int numP=0;
  int sflag=0;
  int wait=0;
  int runningID=0;
  int pflag =0;
  while(1){
    struct rusage structUse;
    int ssize=0;
    int terminatedP=0;
    int opt =0;
    int usage=0;
    int filed=0;
    int pfds[2];
    double startTS=0,startTU=0;
    opt = getopt_long(argc,argv,"",long_opts,NULL);
    if(opt == -1)
      break;
    int j =0;int k=0;
    int i = optind - 1;
    int errorFlag = 0;
    size =0;int z= 0;
    switch(opt){
    case 5:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--append\n");
      fflag|= O_APPEND;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 6:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--cloexec\n");
      fflag|= O_CLOEXEC;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);      }
      break;
    case 7:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--creat\n");
      fflag|= O_CREAT;
      if(pflag){
       double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 8:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--directory\n");
      fflag|= O_DIRECTORY;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 9:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--dsync\n");
      fflag|= O_DSYNC;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 10:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--excl\n");
      fflag|= O_EXCL;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 11:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--nofollow\n");
      fflag=fflag | O_NOFOLLOW;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 12:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--nonblock\n");
      fflag|= O_NONBLOCK;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 13:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--rsync\n");
      fflag|= O_RSYNC;
      if(pflag){
        double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 14:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--sync\n");
      fflag|= O_SYNC;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 15:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
      fprintf(stdout, "--trunc\n");
      fflag|= O_TRUNC;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 1:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag){
	fprintf(stdout,"--rdonly %s\n",optarg);
	fflush(stdout);
      }
      filed=open(optarg,O_RDONLY|fflag,0666);
      if(filed<0){
	fprintf(stderr,"Error: --rdonly %s: %s\n",optarg,strerror(errno));
	exitCode = 1;
	fds++;
	fdTable[fds-1]=-1;
      }
      else{
      currHighestFD++;
      fdTable[fds]=filed;
      fds++;
      }
      numF++;
      fflag = 0 ;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
  break;
    case 2:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag){
	fprintf(stdout,"--wronly %s\n",optarg);
	fflush(stdout);
      }
      filed=open(optarg,O_WRONLY|fflag,0666);
      if(filed<0){
	fprintf(stderr,"Error: --wronly %s: %s\n",optarg,strerror(errno));
	exitCode = 1;
	fdTable[fds]=-1;
	fds++;
      }
      else{
      currHighestFD++;
         fdTable[fds]=filed;
	 fds++;
      }
      numF++;
      fflag=0;
      if(pflag){
        double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;	
    case 16:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag){
	fprintf(stdout,"--rdwr %s\n",optarg);
	fflush(stdout);
      }
      filed=open(optarg,O_RDWR|fflag,0666);
      if(filed<0){
	fprintf(stderr,"Error: --rdwr %s: %s\n",optarg,strerror(errno));
	exitCode = 1;
	fdTable[fds]=-1;
	fds++;
      }
      else{
      currHighestFD++;
         fdTable[fds]=filed;
	 fds++;
      }
      numF++;
      fflag=0;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 23:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--pipe\n");
      pipe(pfds);
	fdTable[fds]=pfds[0];
	fds++;
	fdTable[fds]=pfds[1];
	fds++;
	currHighestFD+=2;
	numF+=2;
      if(pflag){
       double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
	break;
    case 3:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
       while(i<argc && j<3){
	 if(!isdigit(argv[i][0])){
	   errorFlag = 1;
	 }
	 if(atoi(argv[i]) > fds-1){
	   errorFlag = 1;
	 }
	 else{
	 fdsCommand[j]=fdTable[atoi(argv[i])];
	 }
	 fdsOut[j]=argv[i];
	if(fdsCommand[j] == -1){
	  errorFlag=1;
	}
	j++;
	i++;
       }
       j=0;
       while(i<argc){
	 if((argv[i][0] =='-' && argv[i][1]=='-')|| argv[i][0]=='>'){
	   break;
	 }
	 argsCom[j]=argv[i];
	 processArgs[numP].args[j]=argv[i];
      	 j++;
	 i++;
	 size++;
       }
       processArgs[numP].size=size;
       if(vflag){
	 fprintf(stdout,"--command");
	 for( z = 0; z<3;z++){
	   fprintf(stdout," %s",fdsOut[z]);
	 }
	 for(z=0;z<size;z++){
	   fprintf(stdout," %s",argsCom[z]);
	 }
	 fprintf(stdout,"\n");
	 fflush(stdout);
       }
       if( j ==0 ){
	 fprintf(stderr,"Error: --command Missing argument\n");
	 exitCode = 1;
	 continue;
       }
       if(errorFlag){
	 fprintf(stderr,"Error: --command Invalid File Descriptor\n");
	 exitCode = 1;
	 continue;
       }
       argsCom[size]= NULL;
       pid_t f = fork();
       runningProcesses[numP]=f;
       if(f< 0){
	 fprintf(stderr, "fork failed in --command\n");
	 exitCode=1;
       }
       else if ( f == 0 ){
       ioRedir(fdsCommand[0],0);
       ioRedir(fdsCommand[1],1);
       ioRedir(fdsCommand[2],2);
       for (j = 3; j<=currHighestFD;j++){
	 close(j);
       }
       execvp(argsCom[0],argsCom);
       fprintf(stderr, "Error: in execvp(argsCom[0], argsCom): %s",strerror(errno));  
       exitCode = 1;
       }
       else{
	 numP++;
       }
       optind = i;
      if(pflag){
       double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
       break;
    case 4:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      vflag = 1;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 19:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--default\n");
      signal(atoi(optarg),SIG_DFL);
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 18:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--ignore\n");
      signal(atoi(optarg),SIG_IGN);
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 17:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--abort\n");
      fflush(stdout);
      raise(SIGSEGV);
      if(pflag){ double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);}
      break;
    case 20:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--catch");
      signal(atoi(optarg), signalHandler);
      if(pflag){
	usage=getrusage(RUSAGE_SELF,&structUse);
        double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 21:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--close\n");
      close(fdTable[atoi(optarg)]);
      fdTable[atoi(optarg)]=-1;
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 22:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--pause\n");
      pause();
      if(pflag){
	double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"system time: %f, user time: %f",systime-startTS,utime-startTU);
      }
      break;
    case 24:
      if(pflag){
	getrusage(RUSAGE_SELF,&structUse);
        startTU=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	startTS=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
      }
      if(vflag)
	fprintf(stdout,"--wait\n");
      terminatedP=0;
      while(terminatedP!=numP){
	int waits;
	runningID = waitpid(-1, &waits, 0);
	 if(WIFSIGNALED(waits)){
	   wait = WTERMSIG(waits);
	     exitSignal=max(exitSignal, wait);
	     sflag=1;
	     fprintf(stdout,"signal %d",wait);
	 }
	     else{if(WIFEXITED(waits)){
		 wait = WEXITSTATUS(waits);
	   exitCode = max(exitCode, wait);
	   fprintf(stdout,"exit %d", WEXITSTATUS(waits));
	}
	    }
	 for(j=0; j<numP;j++){
	   if(runningProcesses[j]==runningID){
	     break;
	   }
	 }
	 k=0;
	 while(processArgs[j].size!=k){
	 fprintf(stdout," %s", processArgs[j].args[k]);
	 k++;
	 }
	 fprintf(stdout,"\n");
	 terminatedP++;
	if(terminatedP==numP)
	  break; 
      }
      fflush(stdout);
      numP=0;
      if(pflag){
        double systime =0;
	double utime=0;
	usage=getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"parent: system time: %f, user time: %f\n",systime-startTS,utime-startTU);
	usage=getrusage(RUSAGE_CHILDREN,&structUse);
	utime=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
	fprintf(stdout,"children: system time: %f, user time: %f\n",systime-startTS,utime-startTU);
      }
      break;
    case 25:
      if(vflag){
	fprintf(stdout,"--profile\n");
      }
      pflag=1;
      break;
    case '?':
      fprintf(stderr, "Error: Inavlid Option or Missing Argument\n");
      exitCode = 1;
      break;
    }
  }
  struct rusage structUse;
  double systime =0;
	double utime=0;
	getrusage(RUSAGE_SELF,&structUse);
	utime+=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime+=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
        fprintf(stdout,"parent: system time: %f, user time: %f\n",systime,utime);
	getrusage(RUSAGE_CHILDREN,&structUse);
	utime=(double)structUse.ru_utime.tv_sec+(double)structUse.ru_utime.tv_usec*pow(10,-6);
	systime=(double)structUse.ru_stime.tv_sec+(double)structUse.ru_stime.tv_usec*pow(10,-6);
	fprintf(stdout,"children: system time: %f, user time: %f\n",systime,utime);
  if(sflag){
    signal(exitSignal,SIG_DFL);
    fflush(stdout);
    raise(exitSignal);
  }
  else{
  exit(exitCode);
  }
}
void ioRedir(int fd1, int fd2){
  dup2(fd1,fd2);
}
