#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<io.h>
#include "helpman.h"
#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 1024

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

typedef struct superblock
{
	int TotalInode;
	int FreeInode;
}SUPERBLOCK, PSUPERBLOCK;

typedef struct inode
{
	char Filename[MAXINODE];
	int InodeNumber;
	int Filesize;
	int FileActualsize;
	int FileType;
	char *Buffer;      //point to 1KB area
	int LinkCount;
	int ReferenceCount;
	int Permission;
	struct node *next;
}INODE, *PINODE, **PPINODE;

typedef struct filetable
{
	int Readoffset;
	int Writeoffset;
	int Count;
	int Mode;
	PINODE ptrinode;
}FILETABLE, *PFILETABLE;

typedef struct ufdt
{
	PFILETABLE ptrfiletable;
}UFDT;

UFDT UFDTArr[MAXINODE];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;
/***************************************************************
Function NAme:InitialiseSuperBlock
Input Prameters:Nothing
Return Value of function:void
Use in project:use to initialize ufdtarray to NULL.
               also Initialize the members of superblock
******************************************************************/
void InitialiseSuperBlock()
{
	int i = 0;

	while (i < MAXINODE)
	{
		UFDTArr[i].ptrfiletable = NULL; //UFDT array initialize with NULL
		i++;
	}
	SUPERBLOCKobj.TotalInode = MAXINODE;
	SUPERBLOCKobj.FreeInode = MAXINODE;
}
/***************************************************************
Function NAme:CreateDILB()
Input Prameters:nothing
Return Value of function:void
Use in project:use to crete linklist of structure inode
               and create DILB block
******************************************************************/
void CreateDILB()
{
	int i = 1;
	PINODE newn = NULL;
	PINODE temp = NULL;

	while (i <= MAXINODE)
	{
		newn = (PINODE)malloc(sizeof(INODE));

		newn->LinkCount = newn->ReferenceCount = 0;
		newn->FileType = newn->Filesize = 0;
		newn->Buffer = NULL;
		newn->next = NULL;
		newn->InodeNumber = i;

		if (temp == NULL)
		{
			head = newn;
			temp = head;
		}
		else
		{
			temp->next = newn;
			temp = temp->next;
		}
		i++;
	}
	printf("\nDILB creted successfully\n");
}
/***************************************************************
Function NAme:ls_file
Input Prameters:nothig
Return Value of function:void
Use in project:To show you all the files and folders of the directory that you're working in
******************************************************************/
void ls_file()
{
	int i = 0;
	PINODE temp = NULL;

	if (SUPERBLOCKobj.FreeInode == MAXINODE)
	{
		printf("ERROR:there are no files");
		return;
	}
	temp = head;
	printf("\nFile Name\tInode number\tFile size\tLink Count\n");
	printf("-----------------------------------------------------\n");
	while (temp != NULL)
	{
		if (temp->FileType != 0)
		{
			printf("%s\t\t%d\t\t%d\t\t%d\n", temp->Filename, temp->InodeNumber, temp->FileActualsize, temp->LinkCount);
		}
		temp = temp->next;
	}
	printf("--------------------------------------------------------\n");
}
/***************************************************************
Function NAme:CloseAllFile
Input Prameters:nothing
Return Value of function:void
Use in project:To close all opened files
******************************************************************/
void CloseAllFile()
{
	int i = 0;
	while (i < 50)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
		{
			UFDTArr[i].ptrfiletable->Readoffset = 0;
			UFDTArr[i].ptrfiletable->Writeoffset = 0;
			(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
			break;
		}
		i++;
	}
}
/***************************************************************
Function NAme:Stat_File
Input Prameters:name of file
Return Value of function:integer(indicate errors or success of function)
Use in project:To dispaly the information of File using name
******************************************************************/
int Stat_File(char *name) 
{
	PINODE temp = head;
	int i = 0;

	if (name == NULL)                           //if no filename given in command
	{
		return -1;
	}

	while (temp != NULL)
	{
		if (_stricmp(name, temp->Filename) == 0)  //file found
		{
			break;
		}
		temp = temp->next;
	}
	if (temp == NULL)                            //file not found
	{
		return -2;
	}

	printf("\n----Statistical information about file---------\n");
	printf("File Name:%s\n", temp->Filename);
	printf("Inode Number:%d\n", temp->InodeNumber);
	printf("File Size:%d\n", temp->Filesize);
	printf("Actual File Size:%d\n",temp->FileActualsize);
	printf("Link Count:%d\n",temp->LinkCount);
	printf("Reference count:%d\n",temp->ReferenceCount);

	if (temp->Permission == 1)
	{
		printf("File permission: Read only\n");
	}
	else if (temp->Permission == 2)
	{
		printf("File permission: Write only\n");
	}
	else if (temp->Permission == 3)
	{
		printf("File permission: Read and Write\n");
	}
	printf("------------------------------------------\n");
	return 0;
}
/***************************************************************
Function NAme:Fstat_File
Input Prameters:file descriptor
Return Value of function:integer(success or error)
Use in project:To diplay the information of file using discriptor
******************************************************************/
int Fstat_File(int fd)                                
{
	int i = 0;
	PINODE temp = head;

	if (fd < 0)       //wrong parameter
	{
		return -1;
	}
	if (UFDTArr[fd].ptrfiletable == NULL)
	{
		return -2;
	}
	temp = UFDTArr[fd].ptrfiletable->ptrinode;

	printf("\n----Statistical information about file---------\n");
	printf("File Name:%s\n", temp->Filename);
	printf("Inode Number:%d\n", temp->InodeNumber);
	printf("File Size:%d\n", temp->Filesize);
	printf("Actual File Size:%d\n", temp->FileActualsize);
	printf("Link Count:%d\n", temp->LinkCount);
	printf("Reference count:%d\n", temp->ReferenceCount);

	if (temp->Permission == 1)
	{
		printf("File permission: Read only\n");
	}
	else if (temp->Permission == 2)
	{
		printf("File permission: Write only\n");
	}
	else if (temp->Permission == 3)
	{
		printf("File permission: Read and Write\n");
	}
	printf("------------------------------------------\n");
	return 0;
}
/***************************************************************
Function NAme:GetFDFromName
Input Prameters:file name
Return Value of function:file descriptor(integer)
Use in project:to get file descriptor using file name
******************************************************************/
int GetFDFromName(char *name)      
{
	int i = 0;

	while (i < 50)
	{
		if (UFDTArr[i].ptrfiletable != NULL)
		{
			if (_stricmp((UFDTArr[i].ptrfiletable->ptrinode->Filename), name) == 0)
			{
				break;
			}
		}
		i++;
	}
	if (i == 50)                         //file not found
	{
		return -1;
	}
	else                                    //return file Descriptor
	{
		return i;
	}
}
/***************************************************************
Function NAme:CloseFileByName
Input Prameters:file name
Return Value of function:integer(error:file not found(-1) or success(0))
Use in project:
******************************************************************/
int CloseFileByName(char *name)
{
	int i = 0;
	i = GetFDFromName(name);

	if (i == -1)     //file not found
	{
		return -1;
	}

	UFDTArr[i].ptrfiletable->Readoffset = 0;
	UFDTArr[i].ptrfiletable->Writeoffset = 0;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;

	return 0;
}
/***************************************************************
Function NAme:Remove_File
Input Prameters:File name
Return Value of function:integer(error or success)
Use in project:to delete the file
******************************************************************/
int Remove_File(char *name)
{
	int fd = 0;

	fd = GetFDFromName(name);
	if (fd == -1)                     //no file found
	{
		return -1;
	}

	(UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

	if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
	{
		UFDTArr[fd].ptrfiletable->ptrinode->FileType == 0;
		free(UFDTArr[fd].ptrfiletable);
	}

	UFDTArr[fd].ptrfiletable = NULL;
	(SUPERBLOCKobj.FreeInode)++;                             //after deleting file we have to incerese freeinode

}
/***************************************************************
Function NAme:Get_Inode
Input Prameters:File name
Return Value of function:address of inode of that file
Use in project:to know the inode address for create file
******************************************************************/
PINODE Get_Inode(char *name)
{
	PINODE temp = head;
	int i = 0;

	if (name == NULL)
	{
		return NULL;
	}

	while (temp != NULL)
	{
		if (strcmp(name, temp->Filename) == 0)
		{
			break;
		}
		temp = temp->next;
	}
	return temp;
}
/***************************************************************
Function NAme:CreateFile
Input Prameters:File name,permission(read,write,both)
Return Value of function:file descriptor
Use in project:to create file
******************************************************************/
int CreateFile(char *name, int Permission)
{
	int i = 0;
	PINODE temp = head;

	if ((name == NULL) || (Permission == 0) || (Permission > 3))   
	{
		return -1;
	}

	if (SUPERBLOCKobj.FreeInode == 0)
	{
		return -2;
	}
	if (Get_Inode(name) != NULL)                       //duplicate file name
	{
		return -3;
	}

	while (temp != NULL)                   //finding empty inode
	{  
		if (temp->FileType == 0)
		{
			break;
		}
		temp = temp->next;
	}
	while (i < 50)
	{
		if (UFDTArr[i].ptrfiletable == NULL)        
		{
			break;
		}
		i++;
	}
	(SUPERBLOCKobj.FreeInode)--;

	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
	if (UFDTArr[i].ptrfiletable == NULL)                            //failur in memory allocation
	{
		return -4;
	}

	UFDTArr[i].ptrfiletable->Count = 1;
	UFDTArr[i].ptrfiletable->Mode = Permission;
	UFDTArr[i].ptrfiletable->Readoffset = 0;
	UFDTArr[i].ptrfiletable->Writeoffset = 0;

	UFDTArr[i].ptrfiletable->ptrinode = temp;
	strcpy(UFDTArr[i].ptrfiletable->ptrinode->Filename, name);

    UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
	UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;
	UFDTArr[i].ptrfiletable->ptrinode->Filesize = MAXFILESIZE;
	UFDTArr[i].ptrfiletable->ptrinode->FileActualsize = 0;
	UFDTArr[i].ptrfiletable->ptrinode->Permission = Permission;
	UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char*)malloc(MAXFILESIZE);
	memset(UFDTArr[i].ptrfiletable->ptrinode->Buffer,0,1024);   //used to fill the block of memory

	return i;
}
/***************************************************************
Function NAme:OpenFile
Input Prameters:file name,mode for opening of file
Return Value of function:integer
Use in project:to open the required file
******************************************************************/
int OpenFile(char *name, int mode)
{
	int i = 0;
	PINODE temp = head;

	if ((name == NULL) || (mode <= 0))
	{
		return -1;
	}
	temp = Get_Inode(name);
	if (temp == NULL)                     //file not present
	{
		return -2;
	}
	if (temp->Permission < mode)     //permission denied
	{
		return -3;
	}
	while (i < 50)
	{
		if (UFDTArr[i].ptrfiletable == NULL)
		{
			break;
		}
		i++;
	}
	UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

	if (UFDTArr[i].ptrfiletable == NULL)
	{
		return -1;
	}
	UFDTArr[i].ptrfiletable->Count = 1;
	UFDTArr[i].ptrfiletable->Mode = mode;
	if (mode == READ + WRITE)
	{
		UFDTArr[i].ptrfiletable->Readoffset = 0;
		UFDTArr[i].ptrfiletable->Writeoffset = 0;
	}
	else if (mode == READ)
	{
		UFDTArr[i].ptrfiletable->Readoffset = 0;
	}
	else if (mode == WRITE)
	{
		UFDTArr[i].ptrfiletable->Writeoffset = 0;
	}
	UFDTArr[i].ptrfiletable->ptrinode = temp;
	(UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

	return i;
}
/***************************************************************
Function NAme:ReadFile
Input Prameters:File descriptor,array to read data,no of bytes to read
Return Value of function:integer
Use in project:to read the content from the file
******************************************************************/
int ReadFile(int fd, char *arr, int isize)
{
	int read_size = 0;

	if (UFDTArr[fd].ptrfiletable == NULL)       //file not exist
	{
		return -1;
	}
	if ((UFDTArr[fd].ptrfiletable->Mode != READ) && (UFDTArr[fd].ptrfiletable->Mode != READ + WRITE))
	{
		return -2;                    //if we give write mode all other
	}
	if (UFDTArr[fd].ptrfiletable->ptrinode->Permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->Permission != READ + WRITE)
	{
		return -2;         //checking permissionof file
	}
	if (UFDTArr[fd].ptrfiletable->Readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize)
	{
		return -3;                 //at the end
	}
	if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
	{
		return -4;
	}
	 
	read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) - (UFDTArr[fd].ptrfiletable->Readoffset);
	if (read_size < isize)
	{
		strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->Readoffset), read_size);
		UFDTArr[fd].ptrfiletable->Readoffset = (UFDTArr[fd].ptrfiletable->Readoffset )+ read_size;
	}
	else
	{
		strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->Readoffset), isize);
		(UFDTArr[fd].ptrfiletable->Readoffset) =( UFDTArr[fd].ptrfiletable->Readoffset) +isize;
	}
	return isize;
}
/***************************************************************
Function NAme:Truncate_File
Input Prameters:name of file
Return Value of function:integer
Use in project:to clear the content in the file
******************************************************************/
int Truncate_File(char *name)
{
	int fd = GetFDFromName(name);
	if(fd==-1)
	{
		return -1;
	}
	memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer, 0, 1024);
	UFDTArr[fd].ptrfiletable->Readoffset = 0;
	UFDTArr[fd].ptrfiletable->Writeoffset = 0;
	UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize = 0;
}
/***************************************************************
Function NAme:WriteFile
Input Prameters:file descriptor,array in which the data which is to be write is store
                 length of data to write
Return Value of function:return length of data
Use in project:to write the content in the file
******************************************************************/
int WriteFile(int fd, char *arr, int isize)
{
	if ((UFDTArr[fd].ptrfiletable->Mode != WRITE) && (UFDTArr[fd].ptrfiletable->Mode != READ + WRITE))
	{
		return -1;                    //if we give read mode 
	}
	if (UFDTArr[fd].ptrfiletable->ptrinode->Permission != WRITE && UFDTArr[fd].ptrfiletable->ptrinode->Permission != READ + WRITE)
	{
		return -1;         //checking permission of file
	}
	if (UFDTArr[fd].ptrfiletable->Writeoffset ==MAXFILESIZE )
	{
		return -2;                 //at the end
	}
	if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
	{
		return -3;
	}
	strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->Writeoffset), arr,isize);
	(UFDTArr[fd].ptrfiletable->Writeoffset) = (UFDTArr[fd].ptrfiletable->Writeoffset) + isize;
	(UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) + isize;
	return isize;
}
/***************************************************************
Function NAme:LseekFile
Input Prameters:file desciptor,change in offset,startpoint
Return Value of function:integer (error and success)
Use in project:The lseek() function shall set the file offset for the open file
******************************************************************/
int LseekFile(int fd, int size, int from)
{
	if (fd < 0 || from>2)
	{
		return -1;
	}
	if (UFDTArr[fd].ptrfiletable == NULL)
	{
		return -1;
	}

	if ((UFDTArr[fd].ptrfiletable->Mode == READ) || (UFDTArr[fd].ptrfiletable->Mode == READ + WRITE))
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->Readoffset) + size) >UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->Readoffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->Readoffset) = (UFDTArr[fd].ptrfiletable->Readoffset) + size;
		}
		else if (from == START)
		{
			if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize))
			{
				return -1;
			}
			if (size < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->Readoffset) = size;
		}
		else if (from == END)
		{
			if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) > MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->Readoffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->Readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) + size;
		}
	}
	else if (UFDTArr[fd].ptrfiletable->Mode == WRITE)
	{
		if (from == CURRENT)
		{
			if (((UFDTArr[fd].ptrfiletable->Writeoffset) + size) > MAXFILESIZE)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->Writeoffset) + size) < 0)
			{
				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->Writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize))
			{
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) = (UFDTArr[fd].ptrfiletable->Writeoffset) + size;
			}

			(UFDTArr[fd].ptrfiletable->Writeoffset) = (UFDTArr[fd].ptrfiletable->Writeoffset) + size;

		}
		else if (from == START)
		{
			if (size > MAXFILESIZE)
			{
				return -1;
			}
			if (size < 0)
			{
				return -1;
			}
			if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize))
			{
				(UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) = size;
			}
			(UFDTArr[fd].ptrfiletable->Writeoffset) = size;
		}
		else if (from == END)
		{

			if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) + size > MAXFILESIZE)
			{

				return -1;
			}
			if (((UFDTArr[fd].ptrfiletable->Writeoffset) + size) < 0)
			{
				return -1;
			}
			(UFDTArr[fd].ptrfiletable->Writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualsize) + size;
		}
	}
}
int main()
{
	char User[] = { 'N','E','H','A','\0' };
	char Password[8];
	printf("enter login password");
	scanf("%s", Password);
	if (strcmp(User, Password) != 0)
	{
		printf("Password does not matched");
		return 0;
	}
	char *ptr = NULL;
	int ret = 0, fd = 0, count = 0;
	char command[4][80], str[80], arr[1024]; //80 beacause max size of command is 80;
	InitialiseSuperBlock();
	CreateDILB();

	while (1)
	{
		fflush(stdin);        //it cleared the input stream
		strncpy(str, "", 80);
		printf("\nMARVELLOUS VFS:>");
		fgets(str, 80, stdin);
		//scanf("%s", str);
		count = sscanf(str, "%s %s %s %s", command[0], command[1], command[2], command[3]); //tokenisation of string
		if (count == 1)
		{
			if (_stricmp(command[0], "ls") == 0)
			{
				ls_file();
				continue;
			}
			else if (_stricmp(command[0], "closeall") == 0)
			{
				CloseAllFile();
				printf("All files closed successfully\n");
				continue;
			}
		else if (_stricmp(command[0], "clear") == 0)
			{
				system("cls");
				continue;
			}
			else if (_stricmp(command[0], "help") == 0)
			{
				DisplayHelp();
				continue;
			}
			else if (_stricmp(command[0], "exit") == 0)
			{
				printf("Terminating the MARVELLOUS Virtual File System\n");
				break;
			}
			else
			{
				printf("1ERROR :Command Not found!!!\n");
				continue;
			}

		}
		else if (count == 2)
		{
			if (_stricmp(command[0], "stat") == 0)
			{
				ret = Stat_File(command[1]);
				if (ret == -1)
				{
					printf("ERROR:Incorrect Parameter\n");
				}
				if (ret == -2)
				{
					printf("ERROR: There is no such File\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "fstat") == 0)
			{
				ret = Fstat_File(atoi(command[1]));
				if (ret == -1)
				{
					printf("ERROR:Incorrect Parameter\n");
				}
				if (ret == -2)
				{
					printf("ERROR: There is no such File\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "close") == 0)
			{
				ret = CloseFileByName(command[1]);
				if (ret == -1)
				{
					printf("ERROR: There is no such file\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "rm") == 0)
			{
				ret = Remove_File(command[1]);
				if (ret == -1)
				{
					printf("ERROR: There is no such file\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "write") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR:Incorrect parameters\n ");
				}
				printf("Enter the data\n");
				scanf("%[^\n]", arr);

				ret = strlen(arr);
				if (ret == 0)
				{
					printf("ERROR:Incorrect parameters\n");
					//continue;
				}
				ret = WriteFile(fd, arr, ret);
				if (ret == -1)
				{
					printf("ERROR:Permission denied\n");
				}
				if (ret == -2)
				{
					printf("ERROR:There is no sufficient memory to write\n");
				}
				if (ret == -3)
				{
					printf("ERROR:It is not Regular file\n ");
				}
				continue;
			
			}
			else if (_stricmp(command[0], "truncate") == 0)
			{
				ret = Truncate_File(command[1]);
				if (ret == -1)
				{
					printf("ERROR:Incorrect parameters\n");
				}
		
			}
			else if (_stricmp(command[0], "man") == 0)
			{
				Man(command[1]);
			}
			else
			{
				printf("2ERROR:Command not found!!!\n");
				continue;
			}
		}
		else if (count == 3)
		{
			if (_stricmp(command[0], "create") == 0)
			{
				ret = CreateFile(command[1], atoi(command[2])); //atoi():converts a string into an integer numerical representation
				if (ret >= 0)
				{
					printf("File is successfully creted with file descriptor: %d\n", ret);
				}
				if (ret == -1)
				{
					printf("ERROR:Incorrect parameters\n");
				}
				if (ret == -2)
				{
					printf("ERROR: There is no inodes\n ");
				}
				if (ret == -3)
				{
					printf("ERROR: File already exist\n");
				}
				if (ret == -4)
				{
					printf("ERROR:Memory allocation failure\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "open") == 0)
			{
				ret = OpenFile(command[1], atoi(command[2]));
				if (ret >= 0)
				{
					printf("File is successfully opened with file descriptor: %d\n", ret);
				}
				if (ret == -1)
				{
					printf("ERROR:Incorrect parameters\n");
				}
				if (ret == -2)
				{
					printf("ERROR: File not present\n ");
				}
				if (ret == -3)
				{
					printf("ERROR: Permission Denied\n");
				}
				continue;
			}
			else if (_stricmp(command[0], "read") == 0)
			{
				fd = GetFDFromName(command[1]);
				if (fd == -1)
				{
					printf("ERROR: Incorrect parameters\n");
				}
				ptr = (char*)malloc(sizeof(atoi(command[2])) + 1);
				if (ptr == NULL)
				{
					printf("ERROR:Memory allocation failure\n");
				}
				ret = ReadFile(fd, ptr, atoi(command[2]));
				if (ret == -1)
				{
					printf("ERROR:File not existing\n");
				}
				if (ret == -2)
				{
					printf("ERROR:Permission denied\n");
				}
				if (ret == -3)
				{
					printf("ERROR:Reached at the end of file\n");
				}
				if (ret == -4)
				{
					printf("ERROR:It is not regular file\n");
				}
				if (ret == 0)
				{
					printf("ERROR:File empty\n");
				}
				if (ret > 0)
				{
					_write(2, ptr, ret);
				}
				continue;
			}
			else
			{
				printf("3ERROR:Command not found!!!\n");
				continue;
			}

		}
		else if (count == 4)
		{
		  if (_stricmp(command[0], "lseek") == 0)
		  {
			  fd = GetFDFromName(command[1]);
			  if (fd == -1)
			  {
				  printf("ERROR:Incorrect parameters\n");
			  }
			  ret = LseekFile(fd,atoi(command[2]),atoi(command[3]));
			  if (ret == -1)
			  {
				  printf("ERROR:unable to perform lseek\n");
			  }
		  }
		  else
		  {
			  printf("4ERROR:Cammand not found!!!\n");
			  continue;
		  }
		}
		else
		{
		     printf("5ERROR:Command not found\n");
		     continue;
		}

	}

	return 0;
}




