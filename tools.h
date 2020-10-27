#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#define mode 0744
#define flags O_RDWR | O_CREAT

typedef struct normalUser{
	int userID;
	char name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}normalUser;

typedef struct jointUser{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}jointUser;


typedef struct admin{
	int userID;
	char username[30];
	char password[10];
}admin;

void attemptNormalUserLogin(int sd);
void attemptJointUserLogin(int sd);
void attemptAdminLogin(int sd);
void showMenu(int sd);
void deposit(int sd);
void withdraw(int sd);
void balanceEnquiry(int sd);
void changePassword(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
int option,currUserID;


int getNewUserID1(){
	int fd=open("Normal_user",O_RDONLY,mode);
	normalUser record;
	lseek(fd,-sizeof(normalUser),SEEK_END);
	read(fd,&record,sizeof(normalUser));
	close(fd);
	return record.userID+1;
}

int getNewUserID2(){
	int fd=open("Joint_user",O_RDONLY,mode);
	jointUser record;
	lseek(fd,-sizeof(jointUser),SEEK_END);
	read(fd,&record,sizeof(jointUser));
	close(fd);
	return record.userID+1;
}

int getNewUserID3(){
	int fd=open("Admin",O_RDONLY,mode);
	admin record;
	lseek(fd,-sizeof(admin),SEEK_END);
	read(fd,&record,sizeof(admin));
	close(fd);
	return record.userID+1;
}

void create_admin(){
	int fd=open("Admin",flags,mode);
	admin newAdmin;
	int size=lseek(fd,0,SEEK_END);
  //printf("%d",size);
	printf("Please enter the name of the admin: ");
	scanf(" %[^\n]",newAdmin.username);
	//printf("Please enter the password(max 10 characters): ");
	//scanf(" %[^\n]",newAdmin.password);
	char *pass=getpass("Please enter the password(max 10 characters): ");
	memcpy(newAdmin.password,pass,sizeof(pass));
	if(size==0) newAdmin.userID=1000;
	else newAdmin.userID=getNewUserID3();
	printf("Your userID is : %d\n",newAdmin.userID);
	write(fd,&newAdmin,sizeof(admin));
	close(fd);
}

void create_normuser(){
	int fd=open("Normal_user",flags,mode);
	int size=lseek(fd,0,SEEK_END);
	normalUser newNUser;
	printf("Please enter the name of the normal user: ");
	scanf(" %[^\n]",newNUser.name);
	//printf("Please enter the password(max 10 characters): ");
	//scanf(" %[^\n]",newNUser.password);
	char *pass=getpass("Please enter the password(max 10 characters): ");
	memcpy(newNUser.password,pass,sizeof(pass));
	if(!size) newNUser.userID=1000;
	else newNUser.userID=getNewUserID1();
	printf("Please enter the balance amount: Rs. ");
	scanf("%f",&newNUser.balance);
	//newNUser.balance=1000;
	newNUser.account_no=(newNUser.userID-1000)+100000;
	printf("Your userID is : %d\n",newNUser.userID);
	strcpy(newNUser.status,"ACTIVE");
	write(fd,&newNUser,sizeof(normalUser));
	close(fd);
}

void create_jntuser(){
	int fd=open("Joint_user",flags,mode);
	int size=lseek(fd,0,SEEK_END);
	jointUser newJUser;
	printf("Please enter the main name of the joint user: ");
	scanf(" %[^\n]",newJUser.name1);
	printf("Please enter the second name of the joint user: ");
	scanf(" %[^\n]",newJUser.name2);
	//printf("Please enter the password(max 10 characters): ");
	//scanf(" %[^\n]",newJUser.password);
	char *pass=getpass("Please enter the password(max 10 characters): ");
	memcpy(newJUser.password,pass,sizeof(pass));
	if(!size) newJUser.userID=1000;
	else newJUser.userID=getNewUserID2();
	printf("Please enter the balance amount: Rs. ");
	scanf(" %f",&newJUser.balance);
	//newJUser.balance=1000;
	newJUser.account_no=(newJUser.userID-1000)+100000;
	printf("Your userID is : %d\n",newJUser.userID);
	strcpy(newJUser.status,"ACTIVE");
	write(fd,&newJUser,sizeof(jointUser));
	close(fd);
}

void create_mode(){
	printf("\e[1;1H\e[2J");
	printf("\n\n\n\t\t**********************************************************\n");
	printf("\t\t***   WELCOME TO BANKING SYSTEM  Account creation mode ***\n");
	printf("\t\t**********************************************************\n\n\n");
	int choice,select;
	do {
		printf("Enter your choice:\n1. Admin\n2. Normal user\n3. Joint user\nchoice:");
		scanf("%d",&choice);
		switch (choice) {
			case 1:
					create_admin();
					printf("Do you want to add more account(0/1)? ");
					scanf("%d",&select);
					break;
			case 2:
					create_normuser();
					printf("Do you want to add more account(0/1)? ");
					scanf("%d",&select);
					break;
			case 3:
					create_jntuser();
					printf("Do you want to add more account(0/1)? ");
					scanf("%d",&select);
					break;
		}
	} while(select);
}

void chooseOption(int sd){
	write(1,"1 : Normal-User Login\n",sizeof("1 : Normal-User Login\n"));
	write(1,"2 : Joint Account-User Login\n",sizeof("2 : Joint Account-User Login\n"));
	write(1,"3 : Admin Login\n",sizeof("3 : Admin Login\n"));
	write(1,"4 : Exit\n",sizeof("4 : Exit\n"));
	write(1,"Choose an option : ",sizeof("Choose an option : "));
	scanf("%d",&option);
	switch(option){
		case 1 :
			attemptNormalUserLogin(sd);
			break;
		case 2 :
			attemptJointUserLogin(sd);
			break;
		case 3 :
			attemptAdminLogin(sd);
			break;
		case 4 :
		write(1,"Visit again with valid credential...\n\n",sizeof("Visit again with valid credential...\n\n"));
			exit(0);
			//create_mode();
			break;
		default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			chooseOption(sd);
			break;
	}
	return;
}

void showMenu(int sd){
	int select;
	if(option==1 || option==2){
		write(1,"1 : Deposit\n",sizeof("1 : Deposit\n"));
		write(1,"2 : Withdraw\n",sizeof("2 : Withdraw\n"));
		write(1,"3 : Balance Enquiry\n",sizeof("3 : Balance Enquiry\n"));
		write(1,"4 : Password Change\n",sizeof("4 : Password Change\n"));
		write(1,"5 : View Details\n",sizeof("5 : View Details\n"));
		write(1,"6 : Exit\n",sizeof("6 : Exit\n"));
		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		switch(select){
			case 1 :
				deposit(sd);
				break;
			case 2 :
				withdraw(sd);
				break;
			case 3 :
				balanceEnquiry(sd);
				break;
			case 4 :
				changePassword(sd);
				break;
			case 5 :
				viewDetails(sd);
				break;
			case 6 :
				write(sd,&select,sizeof(int));
				write(1,"\n\n\t\tThanks for visiting us\n\t*********Have a nice day*********\n\n\n",sizeof("\n\n\t\tThanks for visiting us\n\t*********Have a nice day*********\n\n\n"));
				exit(0);
			default :
				write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
				showMenu(sd);
				break;
		}
	}
	else if(option==3){
		write(1,"1 : Add Account\n",sizeof("1 : Add Account\n"));
		write(1,"2 : Delete Account\n",sizeof("2 : Delete Account\n"));
		write(1,"3 : Modify Account\n",sizeof("3 : Modify Account\n"));
		write(1,"4 : Search Account\n",sizeof("4 : Search Account\n"));
		write(1,"5 : Exit\n",sizeof("6 : Exit\n"));
		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		switch(select){
			case 1 :
				addAccount(sd);
				break;
			case 2 :
				deleteAccount(sd);
				break;
			case 3 :
				modifyAccount(sd);
				break;
			case 4 :
				searchAccount(sd);
				break;
			case 5 :
				write(sd,&select,sizeof(int));
				write(1,"\n\n\t\tThanks for visiting us\n\t*********Have a nice day*********\n\n\n",sizeof("\n\n\t\tThanks for visiting us\n\t*********Have a nice day*********\n\n\n"));
				exit(0);
			default :
				write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
				showMenu(sd);
				break;
		}
	}
}

void attemptNormalUserLogin(int sd){
	bool result;
	normalUser currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
	//write(1,"Password : ",sizeof("Password : "));
	//scanf("%s",currUser.password);
  char *pass=getpass("Password: ");
  memcpy(currUser.password,pass,sizeof(pass));
	//to the server
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(normalUser));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\t***Invalid login!!***\n\n",sizeof("\n\t***Invalid login!!***\n\n"));
		chooseOption(sd);
	}
	else{
		write(1,"\n\t***Succesfully logged in!!***\n\n",sizeof("\n\t***Succesfully logged in!!***\n\n"));
	}
	return;
}

void attemptJointUserLogin(int sd){
	bool result;
	jointUser currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
	//write(1,"Password : ",sizeof("Password : "));
	//scanf("%s",currUser.password);
  char *pass=getpass("Password: ");
  memcpy(currUser.password,pass,sizeof(pass));
	//to the server
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(jointUser));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\t***Invalid login!!***\n\n",sizeof("\n\t***Invalid login!!***\n\n"));
		chooseOption(sd);
	}
	else{
		write(1,"\n\t***Succesfully logged in!!***\n\n",sizeof("\n\t***Succesfully logged in!!***\n\n"));
	}
	return;
}

void attemptAdminLogin(int sd){
	bool result;
	admin currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
//	write(1,"Password : ",sizeof("Password : "));
	//scanf("%s",currUser.password);
  char *pass=getpass("Password: ");
  memcpy(currUser.password,pass,sizeof(pass));
	//to the server
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(admin));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\t***Invalid login!!***\n\n",sizeof("\n\t***Invalid login!!***\n\n"));
		chooseOption(sd);
	}
	else{
		write(1,"\n\t***Succesfully logged in!!***\n\n",sizeof("\n\t***Succesfully logged in!!***\n\n"));
	}
	return;
}

void deposit(int sd){
	float amt;
	int select=1;
	bool result;

	write(1,"\nAmount to Deposit : Rs.",sizeof("\nAmount to Deposit : Rs."));
	scanf("%f",&amt);

	while(amt<=0){
		printf("Enter a valid amount!!\n");
		write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
		scanf("%f",&amt);
	}

	//to the server
	write(sd,&select,sizeof(int));
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\t***Error depositing your money!!***\n\n",sizeof("\n\t***Error depositing your money!!***\n\n"));
	}
	else{
		write(1,"\n\t***Succesfully deposited!!***\n\n",sizeof("\n\t***Succesfully deposited!!***\n\n"));
	}
	showMenu(sd);
	return;
}

void withdraw(int sd){
	float amt;
	int select=2;
	bool result;

	write(1,"\nAmount to Withdraw : Rs.",sizeof("\nAmount to Withdraw : Rs."));
	scanf("%f",&amt);

	while(amt<=0){
		printf("\n\t***Enter a valid amount!!***\n");
		write(1,"\nAmount to Withdraw : Rs.",sizeof("\nAmount to Withdraw : Rs."));
		scanf("%f",&amt);
	}

	//to the server
	write(sd,&select,sizeof(int));
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\t***Error withdrawing your money, please check your balance!!***\n\n",sizeof("\n\t***Error depositing your money, please check your balance!!***\n\n"));
	}
	else{
		write(1,"\n\t***Succesfully withdrew!!***\n\n",sizeof("\n\t***Succesfully withdrew!!***\n\n"));
	}
	showMenu(sd);
	return;
}

void balanceEnquiry(int sd){
	float amt;
	int select=3;
	int len;

	//to the server
	write(sd,&select,sizeof(int));

	len=read(sd,&amt,sizeof(float));	//from server

	write(1,"\nAvailable Balance :: Rs.",sizeof("\nAvailable Balance :: Rs."));
	printf("%0.2f\n\n",amt);

	showMenu(sd);
	return;
}

void changePassword(int sd){
	int select=4;
	char nPassword[10];
	bool result;
	//write(1,"Enter the new password(max 10 characters) : ",sizeof("Enter the new password(max 10 characters) : "));
	//scanf("%s",nPassword);
  char *pass=getpass("\nEnter the new password(max 10 characters): ");
  memcpy(nPassword,pass,sizeof(pass));
	//to the server
	write(sd,&select,sizeof(int));
	write(sd,nPassword,sizeof(nPassword));
	read(sd,&result,sizeof(result)); //from the server
	if(!result){
		write(1,"\n\t***Error changing your password!!***\n\n",sizeof("\n\t***Error changing your password!!***\n\n"));
	}
	else{
		write(1,"\n\t***Succesfully changed your password!!***\n\n",sizeof("\n\t***Succesfully changed your password!!***\n\n"));
	}
	showMenu(sd);
	return;
}

void viewDetails(int sd){
	int select=5;

	//to the server
	write(sd,&select,sizeof(int));

	if(option==1){
		normalUser currUser1;
		read(sd,&currUser1,sizeof(normalUser));

		printf("\nUser ID : %d\n",currUser1.userID);
		printf("Name : %s\n",currUser1.name);
		printf("Account Number : %d\n",currUser1.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser1.balance);
		printf("Status : %s\n\n",currUser1.status);
	}
	else if(option==2){
		jointUser currUser2;
		read(sd,&currUser2,sizeof(jointUser));

		printf("\nUser ID : %d\n",currUser2.userID);
		printf("Main Account Holder's Name : %s\n",currUser2.name1);
		printf("Other Account Holder's Name : %s\n",currUser2.name2);
		printf("Account Number : %d\n",currUser2.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser2.balance);
		printf("Status : %s\n\n",currUser2.status);
	}
	showMenu(sd);
	return;
}

void addAccount(int sd){
	int select=1;
	int type;
	bool result;
	write(sd,&select,sizeof(int));
	write(1,"\nEnter the type account(1: Normal Account 2: Joint Account) : ",sizeof("\nEnter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);
	write(sd,&type,sizeof(int));
	if(type==1){
		normalUser addUser1;
		write(1,"Name of the account holder : ",sizeof("Name of the account holder : "));
		scanf(" %[^\n]",addUser1.name);
		//write(1,"Password(max 10 characters) : ",sizeof("Password(max 10 characters) : "));
		//scanf("%s",addUser1.password);
    char *pass=getpass("Password(max 10 characters): ");
    memcpy(addUser1.password,pass,sizeof(pass));
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&addUser1.balance);
		write(sd,&addUser1,sizeof(normalUser));
	}

	if(type==2){
		jointUser addUser2;
		write(1,"Name of the primary account holder : ",sizeof("Name of the primary account holder : "));
		scanf(" %[^\n]",addUser2.name1);
		write(1,"Name of the other account holder : ",sizeof("Name of the other account holder : "));
		scanf(" %[^\n]",addUser2.name2);
		//write(1,"Password(max 10 characters) : ",sizeof("Password(max 10 characters) : "));
		//scanf("%s",addUser2.password);
    char *pass=getpass("Password(max 10 characters): ");
    memcpy(addUser2.password,pass,sizeof(pass));
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&addUser2.balance);
		write(sd,&addUser2,sizeof(jointUser));
	}

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\t***Error adding the account!!***\n\n",sizeof("\n\t***Error adding the account!!***\n\n"));
	}
	else{
		write(1,"\n\t***Succesfully added the account!!***\n\n",sizeof("\n\t***Succesfully added the account!!***\n\n"));
	}
	showMenu(sd);
	return;
}

void deleteAccount(int sd){
	int select=2;
	int type,userID;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"\nEnter the type account(1: Normal Account 2: Joint Account) : ",sizeof("\nEnter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&userID);
	write(sd,&userID,sizeof(int));

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\tError deleting the account ,please re-check the User ID!!\n\n",sizeof("\n\tError deleting the account ,please re-check the User ID!!\n\n"));
	}
	else{
		write(1,"\n\t***Succesfully deleted the account!!***\n\n",sizeof("\n\t***Succesfully deleted the account!!***\n\n"));
	}
	showMenu(sd);
	return;
}

void modifyAccount(int sd){
	int select=3;
	int type;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"\nEnter the type account(1: Normal Account 2: Joint Account) : ",sizeof("\nEnter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		normalUser modUser1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&modUser1.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&modUser1.account_no);
		write(1,"New Name of the account holder : ",sizeof("New Name of the account holder : "));
		scanf(" %[^\n]",modUser1.name);
		//write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		//scanf("%s",modUser1.password);
    char *pass=getpass("Password(max 10 characters): ");
    memcpy(modUser1.password,pass,sizeof(pass));
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&modUser1.balance);
		write(sd,&modUser1,sizeof(normalUser));
	}

	if(type==2){
		jointUser modUser2;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&modUser2.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&modUser2.account_no);
		write(1,"New Name of the primary account holder : ",sizeof("New Name of the primary account holder : "));
		scanf(" %[^\n]",modUser2.name1);
		write(1,"New Name of the other account holder : ",sizeof("New Name of the other account holder : "));
		scanf(" %[^\n]",modUser2.name2);
		//write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		//scanf("%s",modUser2.password);
    char *pass=getpass("Password(max 10 characters): ");
    memcpy(modUser2.password,pass,sizeof(pass));
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&modUser2.balance);
		write(sd,&modUser2,sizeof(jointUser));
	}

	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"\n\tError modifying the account ,please re-check the User ID and Account No!!\n\n",sizeof("\n\tError modifying the account ,please re-check the User ID and Account No!!\n\n"));
	}
	else{
		write(1,"\n\t***Succesfully modified the account!!***\n\n",sizeof("\n\t***Succesfully modified the account!!***\n\n"));
	}
	showMenu(sd);
	return;
}

void searchAccount(int sd){
	int select=4;
	int type,len;
	bool result;

	write(sd,&select,sizeof(int));

	write(1,"\nEnter the type account(1: Normal Account 2: Joint Account) : ",sizeof("\nEnter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){
		normalUser searchUser1;
		int userID;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID);
		write(sd,&userID,sizeof(int));

		len=read(sd,&searchUser1,sizeof(normalUser));
		if(len==0){
			write(1,"Please re-check the User ID!!\n\n",sizeof("Please re-check the User ID!!\n\n"));
		}
		else{
			printf("User ID : %d\n",searchUser1.userID);
			printf("Name : %s\n",searchUser1.name);
			printf("Account Number : %d\n",searchUser1.account_no);
			printf("Available Balance : Rs.%0.2f\n",searchUser1.balance);
			printf("Status : %s\n\n",searchUser1.status);
		}
	}

	if(type==2){
		jointUser searchUser2;
		int userID1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID1);
		write(sd,&userID1,sizeof(int));

		len=read(sd,&searchUser2,sizeof(jointUser));
		if(len==0){
			write(1,"Please re-check the User ID!!\n\n",sizeof("Please re-check the User ID!!\n\n"));
		}
		else{
			printf("User ID : %d\n",searchUser2.userID);
			printf("Main Account Holder's Name : %s\n",searchUser2.name1);
			printf("Other Account Holder's Name : %s\n",searchUser2.name2);
			printf("Account Number : %d\n",searchUser2.account_no);
			printf("Available Balance : Rs.%0.2f\n",searchUser2.balance);
			printf("Status : %s\n\n",searchUser2.status);
		}
	}
	showMenu(sd);
	return;
}
