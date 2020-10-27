##Online banking management system using system calls in c programming

COMPILE :-
1. $gcc server.c -o server
2. $gcc client.c -o client

RUN :-
1. $./server  ---  first in one terminal
2. $./client  ---  in the other terminal

DETAILS :-
------------------------Online banking management system-------------------------------

The project aims to develop a banking system that is user-friendly and multi-functional.
Project build using the socket between the client and the server.The server is concurrent.

Admin functions :-
1. add account
2. delete account
3. modify account
4. search for account
5. Exit

Customer functions :-
1. deposite
2. withdraw
3. balance enquiry
4. password change
5. view details
6. Exit

CLIENT :-
1. The system will give you 3 options: Sign up, sign in and exit.
Sign up --> Create mode will launch giving 3 options: admin, normal customer(single account holder), joint (account holder) customer.
-->Initially no user or admin will exist in the system. So, one has to setup admin credential. System owner can create default customer accounts, too in the create mode.
Sign in: --> Login mode will launch giving 4 options : Normal User login, Joint User login, Admin login,exit.
2.Login with your credentials.
3.If successful you get different options then you choose one of them to perform its action.

SERVER :-
1. The server checks the login credentials entered by the client and sends the notification.
2. Performs the actions chose by the client.
3. Implements suitable write and read locks when required.
4. Can handle 10 customers at a time

Features :-
1. Password hidden, and protected admin access, authentication done on server side.
2. Successful record based locking mechanism
3. admin, single account holder customer and joint account holder customer; all three are treated as different (data structure and files are diff)
4. after deleting account the recordes will not be deleted, just the account will be closed.

P.S. 3 files will be created after you sign up for each user. admin, normal_user, joint_user. Right now no limitations on no of possible admin accounts.
