/*
FILE: serve_client.c

UNIT: CNCO3002

PURPOSE: Fullfilling commands from client.

REFERENCE: 
[1] Curtin University.(AU). Worksheet 05(str_echo.c): Threads. In Advanced Computer Communications (Semester 2 2018 Sri Lanka Inst Info Tech - INT[1]). Retrieved from https://lms.curtin.edu.au

[2] Stevens, W. Richard, Bill Fenner, and Andrew M Rudoff. 2012. UNIX Network Programming(Chapter 30) : serv07. 1st ed. Boston [etc.]: Addison-Wesley.

[3] "Get The Current Time In C". 2018. Stack Overflow. https://stackoverflow.com/questions/5141960/get-the-current-time-in-c.

[4] "Socket Function Reference : Getpeername". 2018. Support.Sas.Com. https://support.sas.com/documentation/onlinedoc/sasc/doc750/html/lr2/zeername.htm.

[5] "How To Copy A Char Array In C?". 2018. Stack Overflow. https://stackoverflow.com/questions/16645583/how-to-copy-a-char-array-in-c.

[6] "String Array With Garbage Character At End". 2018. Stack Overflow. https://stackoverflow.com/questions/270708/string-array-with-garbage-character-at-end.

[7] "Concatenate Char Array In C". 2018. Stack Overflow. https://stackoverflow.com/questions/2218290/concatenate-char-array-in-c.

[8] "How Can I Check If A Directory Exists?". 2018. Stack Overflow. https://stackoverflow.com/questions/12510874/how-can-i-check-if-a-directory-exists.

[9] "How To Remove The Character At A Given Index From A String In C?". 2018. Stack Overflow. https://stackoverflow.com/questions/5457608/how-to-remove-the-character-at-a-given-index-from-a-string-in-c.

[10] "The Free Knowledge-Sharing Platform For Technology". 2018. Tech.Io. https://tech.io/playgrounds/14213/how-to-play-with-strings-in-c/string-split.

[11] "Create File Inside A Directory (C)". 2018. Stack Overflow. https://stackoverflow.com/questions/22949500/create-file-inside-a-directory-c.

[12] "What's The Best Way To Check If A File Exists In C? (Cross Platform)". 2018. Stack Overflow. https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c-cross-platform?fbclid=IwAR1fhLAlTSsrylzKsUEIaumVX8knIwL5Pk4keMKAFbwXkqF0YMuLgSZ_IWo.

[13] "Return To The Beginning Of An If Statement". 2018. Stack Overflow. https://stackoverflow.com/questions/26368697/return-to-the-beginning-of-an-if-statement?fbclid=IwAR3GRZeoARFQ7v_utXs981MfYz1KUc4SzLB-cCFEfNmCFGD8jGB9HD23pRA.
*/

#include	"acc.h"

pthread_mutex_t nlock = PTHREAD_MUTEX_INITIALIZER;

void serv_client(int sock_fd, int listen_fd, int cli_id, FILE *ss)
{
	ssize_t		n;
	char		line[MAXLINE];
		
	printf("MES-S > New client with client id of : %d connected \n",cli_id);	
	printf("MES-S > Sockfd for the new client is : %d \n",sock_fd);


	for ( ; ; ) 
	{
		//Readinf data at socket
		if ( (n = Readline(sock_fd, line, MAXLINE)) == 0)
		{
			return;		/* connection closed by other end */	
		}
		else
		{	
			int j, r, count, size;

			//Getting upto where the data is filled in the line[] array
			size = strlen(line)-1;
	
			r = 0;
			count = 0;

			//Accessing line[] array charachter by charachter
			for(j=0; j<size; j++)
			{
				//checking if there is any whitespace as charachter
				if(isspace(line[j]))
				{
					count++;
					r = 1;
				}
			}

			//no whitespace
			if (r == 0 && count == 0)
			{
				if((strcasecmp("get_client_list\n",line)) == 0)
				{
					char *gclPath;
					bzero(&gclPath, sizeof(gclPath));
					gclPath = "./mailboxes";

					//Checking if the directory exists
					DIR* location;
					bzero(&location,sizeof(DIR*));
					location = opendir(gclPath);

				/*m*/	if(location)//Directory exists -> Existing user
					{
						char *tPath = "./mailboxes/clientlist.txt";
						FILE *cl = NULL;
						cl = fopen(tPath, "r");
								
						if (cl == NULL)
						{
							char *eline;
							eline = "Server cannot retrieve the client list \n";
							//Writing the error back to client
							Writen(sock_fd, eline, strlen(eline));
							return;	
						}

						//To store the line read from textfile
						char line[1024];
	
						//Reading textfile line by line
						while(fgets(line, sizeof(line), cl) != NULL)
						{
							Writen(sock_fd, line, strlen(line));
						}

						fclose(cl);
						
						continue;

					}
					else if(ENOENT == errno)//Directory doesn't exists -> Main thread failed to make the file
					{
						char *eline;
						eline = "The server is malfunctioning! Please try again \n";
						//Writing the error back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else//OPENDIR() function failed
					{
						char *eline;
						eline = "The server is malfunctioning! Please try again \n";
						//Writing the error back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}

				}
				else if((strcasecmp("quit\n",line)) == 0)
				{
					char *eline;
					eline = "Bye! Thank you for using MES.\n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
				
					Shutdown(sock_fd, SHUT_WR);
				}
				else
				{
					char *eline;
					eline = "Incorrect command. Please enter a valid command \n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
					continue;
				}
				
			}
			//one Whitespace found (Two word comand)
			else if (r == 1 && count == 1)
			{
				//Reference no [9] & [10] starts from here
				//Storing the two word command in seperate two chars and removing the new line charachter from second word

				//int init_size = strlen(line);
				char delim[] = " ";
				char *first;
				char *second;

				bzero(&first, sizeof(first));
				bzero(&second, sizeof(second));
			
				//Checking if the first charachter of the word is a whitespace
				if(line[0]==' ')
				{
					char *eline;
					eline = "Incorrect command. Please enter a valid command \n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
					continue;
				}

				char *ptr = strtok(line, delim);
				first = ptr;

				ptr = strtok(NULL, delim);
				second = ptr;
 
				int idxToDel = strlen(second)-1;
	 			
				//Removing the newline charachter from the second word
				memmove(&second[idxToDel], &second[idxToDel + 1], strlen(second) - idxToDel);
			
				//Reference no [9] & [10] ends here

				if((strcasecmp("make",first)) == 0)
				{
					int x;
					x = store_peer_info(sock_fd,second,cli_id, ss);

					if(x == -1)
					{
						char *eline;
						eline = "The user already exists! Please try again with different username. \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else if(x == -2)
					{
						char *eline;
						eline = "Server was unable to aqquire info! Please try again. \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else if(x == -3)
					{
						char *eline;
						eline = "Server is malfunctioning! Please try again. \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else
					{
						char *eline;
						eline = "Mailbox for the user created successfully \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
				}
				else if((strcasecmp("send",first)) == 0)
				{
					//Even the name should not exceed 20 charactors, 21 spcaes are allocated for each array
					//The last 21st space is to store end of the string charachtor('/0')
					//Otherwise there would be garbage values appended to trailer of string when printing
					char destination[21];
					int j;

					for(j = 0; second[j]; j++)
					{
				  		destination[j] = tolower(second[j]);
					}
	
					//Specifying that last space of the array is the end of string charachtor
					destination[strlen(second)] = '\0';
					
					//Building directory path to check if sender exist
					char pathTo[260];
					bzero(&pathTo,260);
					sprintf(pathTo, "./mailboxes/%s", destination );

					//Checking if the directory exists
					DIR* dir;
					bzero(&dir,sizeof(DIR*));
					dir = opendir(pathTo);

					if(dir)//Directory exists -> Existing user
					{						
						char *eline;
						eline = "The recieving user verified \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));

						eline = "Please enter the senders name : \n";
						Writen(sock_fd, eline, strlen(eline));

						ssize_t		a;
						char		from[21];
						char 		mail[51];
						char		id[10];
						char 		loc[21];
						char		fsen[21];
						
						//Copieng reciever's name to a new variable
						//And defining eof string charachtor in new variable
						strncpy(loc, destination, strlen(destination));
						loc[strlen(destination)] = '\0';
						
						if ( (a = Readline(sock_fd, from, 21)) == 0) //To get the sender's name
						{
							return;	
						}
						else
						{
							int fdel = strlen(from)-1;

							//Removing the newline charachter from the char array
							memmove(&from[fdel], &from[fdel + 1], strlen(from) - fdel);

							//Coverting senders name from uppercase to lowercase
							int v;
							for(v = 0; from[v]; v++)
							{
				  				fsen[v] = tolower(from[v]);
							}

							//Specifying that last space of the array is the end of string charachtor
							fsen[strlen(from)] = '\0';							

							bzero(&eline, strlen(eline));
							eline = "Please enter a unique id for the email. : \n";
							Writen(sock_fd, eline, strlen(eline));

							if ( (a = Readline(sock_fd, id, 10)) == 0) //To get the sender's id
							{
								return;	
							}
							else
							{
								char user[260];

								int del = strlen(id)-1;
								//Removing the newline charachter from the char array
								memmove(&id[del], &id[del + 1], strlen(id) - del);

								//bzero(&user,260);		
								sprintf(user, "./mailboxes/%s/%s.txt", loc,id );

								if((access(user, F_OK)) == 0)
								{
									char nuser[260];

									do
									{

										bzero(&eline, strlen(eline));
										eline = "Their is email with the id you enter. Please re-enter : \n";
										Writen(sock_fd, eline, strlen(eline));

										if ( (a = Readline(sock_fd, id, 10)) == 0)
										{
											return;
										}
										else
										{
											int ndel = strlen(id)-1;
											//Removing the newline charachter from the char array
											memmove(&id[ndel], &id[ndel + 1], strlen(id) - ndel);

											bzero(&nuser,260);		
											sprintf(nuser, "./mailboxes/%s/%s.txt", loc,id );
										}	
	
									}while( (access(nuser, F_OK)) == 0 );
								}
								
								bzero(&eline, strlen(eline));
								eline = "Please enter the message and press enter. : \n";
								Writen(sock_fd, eline, strlen(eline));

								bzero(&eline, strlen(eline));
								eline = "\n";
								Writen(sock_fd, eline, strlen(eline));

							if ( (a = Readline(sock_fd, mail, 51)) == 0)//To get the message
							{
								return;	
							}
							else
							{
								char finalDest[260];
								bzero(&finalDest,260);
								sprintf(finalDest, "./mailboxes/%s", loc);

								int mdel = strlen(mail)-1;
								//Removing the newline charachter from the char array
								memmove(&mail[mdel], &mail[mdel + 1], strlen(mail) - mdel);
								
								int q = get_mail(fsen, loc, id, mail, finalDest);

								if(q==0)
								{
									bzero(&eline, strlen(eline));
									eline = "Done! Mail sent.\n";
									Writen(sock_fd, eline, strlen(eline));
								}
								else
								{
									bzero(&eline, strlen(eline));
									eline = "Unable to send the maill.\n";
									Writen(sock_fd, eline, strlen(eline));
								}
							}
				
							}							


							
						}
						continue;
					}
					else if(ENOENT == errno)//Directory doesn't exists -> Non existing user
					{
						char *eline;
						eline = "The recieving user does not exist \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else//OPENDIR() function failed
					{
						char *eline;
						eline = "The server is malfunctioning! Please try again \n";
						//Writing the error back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					
				}
				else if((strcasecmp("get_mailbox",first)) == 0)
				{
					char mailbox[21];
					int w;

					for(w = 0; second[w]; w++)
					{
				  		mailbox[w] = tolower(second[w]);
					}
	
					//Specifying that last space of the array is the end of string charachtor
					mailbox[strlen(second)] = '\0';
					
					//Building directory path to check if sender exist
					char userPath[260];
					bzero(&userPath,260);
					sprintf(userPath, "./mailboxes/%s", mailbox );

					//Checking if the directory exists
					DIR* locate;
					bzero(&locate,sizeof(DIR*));
					locate = opendir(userPath);

					if(locate)//Directory exists -> Existing user
					{

						//Building directory path
						char txtPath[260];

						//Creating path for new mail
						sprintf(txtPath, "./mailboxes/%s/mailbox_info.txt", mailbox);

						FILE *fp = NULL;
						fp = fopen(txtPath, "r");
								
						if (fp == NULL)
						{
							return -1;
						}

						//To store the line read from textfile
						char line[1024];
	
						//Reading textfile line by line
						while(fgets(line, sizeof(line), fp) != NULL)
						{
							Writen(sock_fd, line, strlen(line));
						}

						fclose(fp);
						
						continue;

					}
					else if(ENOENT == errno)//Directory doesn't exists -> Non existing user
					{
						char *eline;
						eline = "The user doesn't exist!\n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else//OPENDIR() function failed
					{
						char *eline;
						eline = "The server is malfunctioning! Please try again \n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
				}
				else
				{
					char *eline;
					eline = "Incorrect command. Please enter a valid command \n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
					continue;
				}


			
			}
			//Two whitespaces found
			else if (r == 1 && count == 2)
			{
				
				//Reference no [9] & [10] starts from here
				//Storing the two word command in seperate two chars and removing the new line charachter from second word

				char delim[] = " ";
				char *first;
				char *second;
				char *third;

				bzero(&first, sizeof(first));
				bzero(&second, sizeof(second));
				bzero(&third, sizeof(third));
			
				//Checking if the first charachter of the word is a whitespace
				if(line[0]==' ')
				{
					char *eline;
					eline = "Their seems to be a whitespace char infront of the command. Please try again! \n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
					continue;
				}

				char *ptr = strtok(line, delim);
				first = ptr;

				ptr = strtok(NULL, delim);
				second = ptr;
	
				ptr = strtok(NULL, delim);
				third = ptr;
 
				int idxToDel = strlen(third)-1;

				//Removing the newline charachter from the third word
				memmove(&third[idxToDel], &third[idxToDel + 1], strlen(third) - idxToDel);

				if((strcasecmp("delete",first)) == 0)
				{
					char username[21];
					int v,k;

					for(v = 0; second[v]; v++)
					{
				  		username[v] = tolower(second[v]);
					}
	
					//Specifying that last space of the array is the end of string charachtor
					username[strlen(second)] = '\0';
					
					//Path to mailbox of particular user
					char usern[260];
					bzero(&usern,260);
					sprintf(usern, "./mailboxes/%s", username );

					//Path of mail that client is asking to delete					
					char mailid[260];
					bzero(&mailid,260);
					sprintf(mailid, "./mailboxes/%s/%s.txt", username, third);

					//Path to mailbox_info.txt of the particular client					
					char mailbox_info[260];
					bzero(&mailbox_info,260);
					sprintf(mailbox_info, "./mailboxes/%s/mailbox_info.txt", username);

					//Checking if the directory exists
					DIR* ud;
					bzero(&ud,sizeof(DIR*));
					ud = opendir(usern);

					if(ud)//Directory exists -> existing user
					{
						char *eline;
						eline = "User verified!\n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));

						int o;	

						if((access(mailid, F_OK)) == 0)
						{
							o = delete_mail(usern, mailbox_info, third, username );
							if(o == 0)
							{
								bzero(&eline, strlen(eline));
								eline = "Done! Mail deleted.\n";
								Writen(sock_fd, eline, strlen(eline));
							}
							else
							{
								bzero(&eline, strlen(eline));
								eline = "Unable to delete the maill.\n";
								Writen(sock_fd, eline, strlen(eline));
							}
						}
						else
						{
							char *eline;
							eline = "Their is no mail under the specified mail id!\n";
							//Writing a message back to client
							Writen(sock_fd, eline, strlen(eline));
							continue;	
						}
						continue;
					}
					else if(ENOENT == errno)//Directory doesn't exists -> Non existing user
					{
						char *eline;
						eline = "The user doesn't exist!\n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else
					{
						char *eline;
						eline = "Server is malfunctioning. Please try again!\n";
						//Writing the error back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;		
					}

					

					continue;
				}
				else if((strcasecmp("read",first)) == 0)
				{
					char cname[21];
					int t;

					for(t = 0; second[t]; t++)
					{
				  		cname[t] = tolower(second[t]);
					}
	
					//Specifying that last space of the array is the end of string charachtor
					cname[strlen(second)] = '\0';
					
					//Path to mailbox of particular user
					char usern[260];
					bzero(&usern,260);
					sprintf(usern, "./mailboxes/%s", cname );

					//Path of mail that client is asking to view					
					char mailid[260];
					bzero(&mailid,260);
					sprintf(mailid, "./mailboxes/%s/%s.txt", cname, third);

					//Path to mailbox_info.txt of the particular client					
					char mailbox_info[260];
					bzero(&mailbox_info,260);
					sprintf(mailbox_info, "./mailboxes/%s/mailbox_info.txt", cname);

					//Checking if the directory exists
					DIR* ud;
					bzero(&ud,sizeof(DIR*));
					ud = opendir(usern);

					if(ud)//Directory exists -> existing user
					{
						char *eline;
						eline = "User verified!\n";
						//Writing message back to client
						Writen(sock_fd, eline, strlen(eline));

						if((access(mailid, F_OK)) == 0)
						{
							FILE *fp = NULL;
							fp = fopen(mailid, "r");
								
							if (fp == NULL)
							{
								Pthread_mutex_unlock(&nlock);
								return -1;
							}

							//To store the line read from textfile
							char line[1024];
		
							//Reading textfile line by line
							while(fgets(line, sizeof(line), fp) != NULL)
							{
								Writen(sock_fd, line, strlen(line));
							}

							fclose(fp);							
			
							int v = update_mailbox(mailbox_info, third);
							if(v == 0)
							{
								bzero(&eline, strlen(eline));
								eline = "Done! MailBox info updated.\n";
								Writen(sock_fd, eline, strlen(eline));
							}
							else
							{
								bzero(&eline, strlen(eline));
								eline = "Failed to update the mailbox info.\n";
								Writen(sock_fd, eline, strlen(eline));
							}
						}
						else
						{
							char *eline;
							eline = "Their is no mail under the specified mail id!\n";
							//Writing a message back to client
							Writen(sock_fd, eline, strlen(eline));
							continue;	
						}
						continue;
					}
					else if(ENOENT == errno)//Directory doesn't exists -> Non existing user
					{
						char *eline;
						eline = "The user doesn't exist!\n";
						//Writing a message back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;
					}
					else
					{
						char *eline;
						eline = "Server is malfunctioning. Please try again!\n";
						//Writing the error back to client
						Writen(sock_fd, eline, strlen(eline));
						continue;		
					}

					
					continue;	
				}
				else
				{
					char *eline;
					eline = "Incorrect command. Please enter a valid command \n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
					continue;
				}
			}
			//More than two whitespaces(Incorrect command)
			else if( count > 2 )
			{
				char *eline;
				eline = "Incorrect command. Please enter a valid command \n";
				//Writing the error back to client
				Writen(sock_fd, eline, strlen(eline));
				continue;
			}
			//Error in the isspace function
			else
			{
					char *eline;
					eline = "Server could't execute the command \n";
					//Writing a message back to client
					Writen(sock_fd, eline, strlen(eline));
					continue;
			}

		}
	}
}

//returns -2 ,-1, -3, 0
int store_peer_info(int sock,char name[20], int cid, FILE *fp)
{
		
	//Mutex lock is used to avoid threads acessing connfd & count variables simultaneously
	Pthread_mutex_lock(&nlock);

	//Even the name should not exceed 20 charactors, 21 spcaes are allocated for each array
	//The last 21st space is to store end of the string charachtor('/0')
	//Otherwise there would be garbage values appended to trailer of string when printing
	char nname[21];
	char lname[21];
	int j,i;

	bzero(&nname,21);
	bzero(&lname,21);

	//Copieng one contents of one char array to a another
	strncpy(nname, name, 20);

	//Specifying that last space of the array is the end of string charachtor
	nname[21 - 1] = '\0';

	for(j = 0; nname[j]; j++)
	{
  		lname[j] = tolower(nname[j]);
	}

	//Specifying that last space of the array is the end of string charachtor
	lname[21 - 1] = '\0';

	//Creating a mailbox for the user
	//Reference no [7] starts here

	const char* xname =  "./mailboxes/";
	const char* dname =  "/mailbox_info.txt";//new
	char* fullPath;
	char* secfullpath; //new

        //make space for the new string 
	fullPath = malloc(sizeof(xname)+sizeof(lname));

	secfullpath = malloc(strlen(xname)+strlen(lname)+strlen(dname));

	//Copying first half of the path into full name var
	strcpy(fullPath, xname);
		
	//Appending half of the path into full name var
	strcat(fullPath,lname);

	//Copying first half of the path into full name var 
	strcpy(secfullpath, fullPath);
		
	//Appending half of the path into full name var 
	strcat(secfullpath,dname);

	//Reference no [7] ends here

	DIR* dir = opendir(fullPath);

	//Checking if a directory exists (Checking if there is user by the same name)
	if (dir)
	{
    		
    		closedir(dir);

		//Unlocking the lock for other threads access store_perr_info() function
		Pthread_mutex_unlock(&nlock);

		return -1;
	}
	//If there is no user by the same name -> Add data to the client list & make mailbox for the user
	else if (ENOENT == errno)
	{
	    	//Socket address strctur and length is to store client's infromation
		struct sockaddr_in addr;
		//To store the length of the servers socket address structure
		int len;
		len = sizeof(addr);

		//To store the return value of the getpeername function
		int errno;
		errno = 0;
		
		//This function gets the connected client's socket info and store it in a socket address structure
		errno = getpeername(sock, (struct sockaddr*) &addr, &len);

		if(errno == -1)
		{
			printf("Getpeername error : Unable to aqquire peer's network address and port");
			return -2;
		}
		else
		{
			
			//Reference No [3]
			time_t rawtime;
	    		struct tm * timeinfo;
	    		time ( &rawtime );
	    		timeinfo = localtime ( &rawtime );
			char buff[500];
			asctime_r(timeinfo, &buff);
			
			if (fp == NULL)
			{
 
		    		printf("Error opening file!\n");	
	    			exit(1);
			}
			
			//Putting data about client to the file
			fprintf(fp, "%d %s %s %d %d %d:%d:%d \n",cid, lname, (inet_ntoa( addr.sin_addr )), ((int)(ntohs( addr.sin_port))), 0, 
			timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			fflush(fp);
			
			//Creating mailbox for the client
			mkdir(fullPath, 0777); 

			//Creating a command to create mailbox_info.txt file
			char pathFile[260];
			sprintf(pathFile, "touch %s/mailbox_info.txt", fullPath );
			
			//Executing the created command as terminal command
			system(pathFile);

		}

		//Emptying the structure so it doesn't hold any garbgae value
		bzero(&addr, len);
		Pthread_mutex_unlock(&nlock);

		return 0;	

	}
	//Problem with opendir() function
	else
	{
  		printf("Error: opendir function failed \n");
		fflush(stdout);

		//Unlocking the lock for other threads access store_perr_info() function
		Pthread_mutex_unlock(&nlock);
		return -3;
	}

}

//returns = -1,0
int get_mail(char sender[21],char reciever[21], char mail_id[10], int mail_content[51], char DestFinal[260])
{
	//Mutex lock is used to avoid threads acessing connfd & count variables simultaneously
	Pthread_mutex_lock(&nlock);

	//Building directory path
	char txtDirectory[260];
	char mailBoxInfo[260];
	char clientList[260];

	//Creating path for new mail
	sprintf(txtDirectory, "./mailboxes/%s/%s.txt", reciever, mail_id);

	//Creating path to access mailbox_info.txt of the recieving user of the mail
	sprintf(mailBoxInfo, "./mailboxes/%s/mailbox_info.txt",reciever);

	//Creating path to access clientlist.txt
	sprintf(clientList, "./mailboxes/clientlist.txt");

	//Reference No [3]
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	int dow = timeinfo->tm_wday;
	int mon = timeinfo->tm_mon;

	char* asc_day;
	char* asc_month;

	//To get the day of week in string format
	if(dow == 0)
	{
		asc_day = "Sun";
	}
	else if(dow == 1)
	{
		asc_day = "Mon";
	}
	else if(dow == 2)
	{
		asc_day = "Tue";
	}
	else if(dow == 3)
	{
		asc_day = "Wed";
	}
	else if(dow == 4)
	{
		asc_day = "Thu";
	}
	else if(dow == 5)
	{
		asc_day = "Fri";
	}
	else
	{
		asc_day = "Sat";
	}

	
	//To get the month in string format
	if(mon == 0)
	{
		asc_month = "Jan";
	}
	else if(mon == 1)
	{
		asc_month = "Feb";
	}
	else if(mon == 2)
	{
		asc_month = "Mar";
	}
	else if(mon == 3)
	{
		asc_month = "Apr";
	}
	else if(mon == 4)
	{
		asc_month = "May";
	}
	else if(mon == 5)
	{
		asc_month = "Jun";
	}
	else if(mon == 6)
	{
		asc_month = "Jul";
	}
	else if(mon == 7)
	{
		asc_month = "Aug";
	}
	else if(mon == 8)
	{
		asc_month = "Sep";
	}
	else if(mon == 9)
	{
		asc_month = "Oct";
	}
	else if(mon == 10)
	{
		asc_month = "Nov";
	}
	else
	{
		asc_month = "Dec";
	}


	//Creating txt file to store the mail
	FILE *fp = NULL;
	fp = fopen(txtDirectory, "w");
			
	if (fp == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//Creating header & body & trailer of the email
	fprintf(fp, "From: %s \n", sender);	
	fprintf(fp, "To: %s \n", reciever);
	fprintf(fp, "Date: %s, %d %s %d %d:%d:%d\n", asc_day, timeinfo->tm_mday, asc_month, timeinfo->tm_year,
	timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	fprintf(fp, "\n");
	fprintf(fp, "Hi %s,\n", reciever);
	fprintf(fp, "%s\n", mail_content);
	fprintf(fp, "\n");
	fprintf(fp, "Regards\n");
	fprintf(fp, "\n");
	fprintf(fp, "%s\n", sender);

	fclose(fp);

	//Opening mailbox_info.txt of the receiving user to store mail info
	FILE *fd = NULL;
	fd = fopen(mailBoxInfo, "a");

	if (fd == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	char *status = "unread";	

	fprintf(fd, "%s %s %d:%d:%d %s\n", mail_id, sender, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, status);
		
	fclose(fd);

	//opening clientlist.txt update the line related to client
	FILE *fo = NULL;
	fo = fopen(clientList, "r+");
			
	if (fo == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//To store the line read from textfile
	char line[1024];
	
	//Reading textfile line by line
	while(fgets(line, sizeof(line), fo) != NULL)
	{
		if((strstr(line, reciever)) != NULL)
		{
			char delim[] = " ";
			char *first, *second, *third, *fourth, *fifth, *sixth;

			bzero(&first, sizeof(first));
			bzero(&second, sizeof(second));
			bzero(&third, sizeof(third));
			bzero(&fourth, sizeof(fourth));
			bzero(&fifth, sizeof(fifth));
			bzero(&sixth, sizeof(sixth));
			
			//Breaking down of the read line to serveral parts, starts from here.
			char *ptr = strtok(line, delim);
			first = ptr;

			ptr = strtok(NULL, delim);
			second = ptr;

			ptr = strtok(NULL, delim);
			third = ptr;

			ptr = strtok(NULL, delim);
			fourth = ptr;

			ptr = strtok(NULL, delim);
			fifth = ptr;

			ptr = strtok(NULL, delim);
			sixth = ptr;
			//Breaking down of the read line to serveral parts, ends here.

			//Converting the no of email argument to int for the incrementation
			int count = atoi(fifth);
			count = count + 1;

			//Getting the size of line read
			int seek = strlen(first) + strlen(second) + strlen(third) + strlen(fourth) + strlen(fifth) + strlen(sixth) + 7;
			
			//Setting the cursor to the 0th position of the line that is going to be replaced
			fseek(fo, -(seek), SEEK_CUR);

			//Writing the new line to the file
			fprintf(fo, "%s %s %s %s %d %s",first, second, third, fourth, count, sixth);
			
		}
	}
			
	Pthread_mutex_unlock(&nlock);

	return 0;
	
}

//return -1,0
int delete_mail(char PathToFile[260], char PathToMail[260], char MailId[10], char uname[21] )
{
	Pthread_mutex_lock(&nlock);

	char temp[120];
	sprintf(temp, "%s/temp.txt", PathToFile);

	char delete[120];
	sprintf(delete, "%s/delete.txt", PathToFile);

	char clientList[120];
	//Creating path to access clientlist.txt
	sprintf(clientList, "./mailboxes/clientlist.txt");

	//openingtemp.txt 
	FILE *ft = NULL;
	ft = fopen(temp, "w");		
	if (ft == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//opening delete.txt 
	FILE *del = NULL;
	del = fopen(delete, "w");		
	if (del == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}
 
	//opening mailbox_info.txt to update
	FILE *fl = NULL;
	fl = fopen(PathToMail, "r+");		
	if (fl == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//To store the line read from textfile
	char line[1024];
	
	//Reading textfile line by line
	while(fgets(line, sizeof(line), fl) != NULL)
	{
		//If the 0th characher of the line is equal to mail id - put it into the delete.txt
		if(line[0] == MailId[0])
		{
			fprintf(del, "%s", line);
			bzero(&line, strlen(line));
		}
		//If the 0th characher of the line is not equal to mail id - put it into the temp.txt
		else
		{
			fprintf(ft, "%s", line);
			bzero(&line, strlen(line));
		}	
	}

	fclose(del);
	fclose(fl);
	fclose(ft);

	//Deleting current mailbox_info.txt file
	if(remove(PathToMail) != 0)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//Deleting delete.txt file
	if(remove(delete) != 0)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//opening new mailbox_info.txt 
	FILE *new = NULL;
	new = fopen(PathToMail, "w");		
	if (new == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//Reopenig the temp.txt file to read data
	FILE *tf = NULL;
	tf = fopen(temp, "r");		
	if (tf == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}


	//To store the line read from textfile
	char nline[1024];
	
	//Reading temp.txt line by line and storing each line in 
	while(fgets(nline, sizeof(nline), tf) != NULL)
	{
		fprintf(new, "%s", nline);		
	}

	fclose(tf);
	fclose(new);
	
	//Deleting temp.txt file
	if(remove(temp) != 0)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	char mail_del[120];
	sprintf(mail_del, "%s/%s.txt", PathToFile, MailId);

	//Deleting relevent mail file
	if(remove(mail_del) != 0)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	FILE *cl = NULL;
	cl = fopen(clientList, "r+");
			
	if (cl == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}	
	
	//To store the line read from client list
	char cline[1024];
	
	//Reading textfile line by line
	while(fgets(cline, sizeof(cline), cl) != NULL)
	{
		if((strstr(cline, uname)) != NULL)
		{
			char delim[] = " ";
			char *first, *second, *third, *fourth, *fifth, *sixth;

			bzero(&first, sizeof(first));
			bzero(&second, sizeof(second));
			bzero(&third, sizeof(third));
			bzero(&fourth, sizeof(fourth));
			bzero(&fifth, sizeof(fifth));
			bzero(&sixth, sizeof(sixth));
			
			//Breaking down of the read line to serveral parts, starts from here.
			char *ptr = strtok(cline, delim);
			first = ptr;

			ptr = strtok(NULL, delim);
			second = ptr;

			ptr = strtok(NULL, delim);
			third = ptr;

			ptr = strtok(NULL, delim);
			fourth = ptr;

			ptr = strtok(NULL, delim);
			fifth = ptr;

			ptr = strtok(NULL, delim);
			sixth = ptr;
			//Breaking down of the read line to serveral parts, ends here.

			//Converting the no of email argument to int for the decrementation
			int count = atoi(fifth);
			count = count - 1;

			//Getting the size of line read
			int seek = strlen(first) + strlen(second) + strlen(third) + strlen(fourth) + strlen(fifth) + strlen(sixth) + 7;
			
			//Setting the cursor to the 0th position of the line that is going to be replaced
			fseek(cl, -(seek), SEEK_CUR);

			//Writing the new line to the file
			fprintf(cl, "%s %s %s %s %d %s",first, second, third, fourth, count, sixth);
			
		}
	}

	Pthread_mutex_unlock(&nlock);

	return 0;
}

//Return 0,-1
int update_mailbox(char mbpath[260], char mid[10])
{
	Pthread_mutex_lock(&nlock);

	//opening clientlist.txt update the line related to client
	FILE *fo = NULL;
	fo = fopen(mbpath, "r+");
			
	if (fo == NULL)
	{
		Pthread_mutex_unlock(&nlock);
		return -1;
	}

	//To store the line read from textfile
	char line[1024];
	
	//Reading textfile line by line
	while(fgets(line, sizeof(line), fo) != NULL)
	{
		//checkinf if first character of line is equal to mail id
		if(line[0] == mid[0])
		{
			char delim[] = " ";
			char *first, *second, *third, *fourth, *nfourth;

			bzero(&first, sizeof(first));
			bzero(&second, sizeof(second));
			bzero(&third, sizeof(third));
			bzero(&fourth, sizeof(fourth));
			
			//Breaking down of the read line to serveral parts, starts from here.
			char *ptr = strtok(line, delim);
			first = ptr;

			ptr = strtok(NULL, delim);
			second = ptr;

			ptr = strtok(NULL, delim);
			third = ptr;

			ptr = strtok(NULL, delim);
			fourth = ptr;
			//Breaking down of the read line to serveral parts, ends here.

			nfourth = "read  ";

			//Getting the size of line read
			int seek = strlen(first) + strlen(second) + strlen(third)+ 6+4;
			
			//Setting the cursor to the 0th position of the line that is going to be replaced
			fseek(fo, -(seek), SEEK_CUR);

			//Writing the new line to the file
			fprintf(fo, "%s %s %s %s",first, second, third, nfourth);
			
		}
	}

	fclose(fo);

	Pthread_mutex_unlock(&nlock);

	return 0;
}

