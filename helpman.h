#pragma once
/***************************************************************
Function NAme:DisplayHelp
Input Prameters:nothing
Return Value of function:void
Use in project:display information about command
******************************************************************/
void DisplayHelp()
{
	printf("ls: To show you all the files and folders of the directory that you're working in\n");
	printf("clear: To clear teminal scren\n");
	printf("open: To open the file\n");
	printf("close: To close the file\n");
	printf("closeall: To close all opened files\n");
	printf("read: To read the content from File\n");
	printf("write: To write the content in the file\n");
	printf("exit: To terminate the File system\n");
	printf("stat: To dispaly the information of File using name\n");
	printf("fstat: To diplay the information of file using discriptor\n");
	printf("truncate: To remove all the data from the file\n");
	printf("rm: To delete the File\n");
}
/***************************************************************
Function NAme:Man
Input Prameters:command 
Return Value of function:void
Use in project:dispaly whole info about required command
******************************************************************/
void Man(char *name)
{
	if (name == NULL)
	{
		return;
	}
	if (_stricmp(name, "ls") == 0)
	{
		printf("NAME: \n   ls - list directory contents\n");
		printf("USAGE: \n  ls \n");
		printf("DESCRIPTION:\n   List information about the FILEs (the current directory by default). \n ");
		printf("AUTHOR: \n    Written by Richard M.Stallman and David MacKenzie.\n");
	
	}
	else if (_stricmp(name,"rm") == 0)
	{
		printf("NAME: \n  rm - remove files or directories\n");
		printf("USAGE:\n rm File_name\n");
		printf("DESCRIPTION:\n This manual page documents the GNU version of rm.  rm removes each specified file.By default, it does not remove directories.\n");
		printf("AUTHOR: \n Written by Paul Rubin, David MacKenzie, Richard M. Stallman, and Jim Meyering.\n");

	}
	else if (_stricmp(name, "stat") == 0)
	{
		printf("NAME:\n stat - display file or file system status\n");
		printf("USAGE:\n stat File_name\n");
		printf("DESCRIPTION:\n  Display file or file system status.\n");
		printf("AUTHOR:\n Written by Michael Meskes.\n");

	}
	else if (_stricmp(name, "fstat") == 0)
	{
		printf("NAME:\n fstat -display file status\n");
		printf("USAGE:\n fstat File_descriptor\n");
		printf("DESCRIPTION:\n fstat() is identical to stat(), except that the file about which information is to be retrieved is specified by the file descriptor fd.\n");
		printf("RETURN VALUE: \nOn success, zero is returned.  On error, -1 is returned, and errno is set appropriately.\n");

	}
	else if (_stricmp(name, "create") == 0)
	{
		printf("NAME:\n create -possibly create file\n");
		printf("USAGE:\n create File_name File_permission(read,write or both)\n");
		printf("DESCRIPTION:\n crete new regular file\n");
		printf("RETURN VALUE:\n creat() return the new file descriptor, or -1 if an error occurred.\n");
	}
	else if (_stricmp(name, "open") == 0)
	{
		printf("NAME:\n open -open the file\n");
		printf("USAGE:\n open File_name mode\n");
		printf("DESCRIPTION:\n  The open() system call opens the file specified by pathname.\n");
		printf("RETURN TYPE:\n File Descriptor\n");
	}
	else if (_stricmp(name, "read") == 0)
	{
		printf("NAME:\n read - read from a file descriptor\n");
		printf("USAGE:\n read File_Name No_of_bytes_to_read\n");
		printf("DESCRIPTION:\n  read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.\n");
		printf("RETURN VALUE:\nno of bytes read is return on error -1 returned\n");
	}
	else if (_stricmp(name, "truncate") == 0)
	{
		printf("NAME:\n truncate- truncate file to specified length\n");
		printf("USAGE:\n truncate File_name\n");
		printf("DESCRIPTION:\n  The truncate() functions cause the regular file named	by path or referenced by fd to be truncated to a size of precisely	length bytes.\n");
		printf("RETURN VALUE:\non success 0,on error -1 returned");
	}
	else if (_stricmp(name, "write") == 0)
	{
		printf("NAME:\n write - write to a file name\n");
		printf("USAGE:\n write File_name\nafter that enter data you want to enter\n");
		printf("DESCRIPTION:\n write() writes up to count bytes from the buffer starting at buf to the file referred to by the file name.\n");

	}
	else if (_stricmp(name, "close") == 0)
	{
		printf("NAME: \n  close — close a file descriptor\n");
		printf("USAGE:\n close File_name\n");
		printf("DESCRIPTION:\n The close() function shall deallocate the file descriptor indicated by fildes.\n");
	
	}
	else if (_stricmp(name, "lseek") == 0)
	{
		printf("NAME:\n lseek — move the read/write file offset\n");
		printf("USAGE: \nlseek File_name ChangeInoffset Startpoint\n");
		printf("DESCRIPTION:\n The lseek() function shall set the file offset for the open file\n");
	
	}
	else if (_stricmp(name, "closeall") == 0)
	{
		printf("NAME:\n closeall - used to close all opened files\n");
		printf("USAGE:\n closeall\n");
		printf("DESCRIPTION:\n close all opend files\n");
	
	}
	else 
	{
		printf("ERROR:No manual entry available\n");
	}





}