# My Email System

This is a solution developed for one of my university assignments which was based on &#34;C socket programming&#34;. This is called My Email System(MES) which was developed to reflect a simple email system.

**Note** - MES does not follow standard SMTP and POP protocols.

My email system is a combination of two programs ( MES-C & MES-S) that is developed to implement a own personal email system where, client program is waiting for commands from the user and server program waiting to receive commands from client program, to provide service to the user.

Here's how to run the programs,

1) How to compile :
	* Change path in terminal to the path of "MES" program  

	* This program is developed using make files, so have to use "make" command to compile the files. 

	* Compile server
		make -f makeserver

	* Compile client
		make -f makeclient

2) How to run : 
	* Run the programs in the following order
	
	* Server program
		./msss.out No_of_threads_for_thread_pool Port_no_server_should_listen_at
		
			eg: ./msss.out 10 50002

	* Client program
		./mssc.out IP_Address_of_server Port_no_that_server_is_listening_at
		
			eg: ./mssc.out 127.0.0.1 50002

3) The server recognize following commands
	i) make client_name
		eg: make sam

	ii) get_client_list

	iii) send reciever_name
		eg: send sam

	iv) get_mailbox client_name
		eg: get_mailbox sam

	v) read client_name mail_id
		eg: read sam 1

	vi) delete client_name mail_id
		eg: delete sam 1

	vii) quit   