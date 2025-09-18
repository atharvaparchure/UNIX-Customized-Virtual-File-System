/////////////////////////////////////////////////////////////////////////////////////
//
// Header files inclusion
//
/////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include<iostream>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
//
// User defined Macro
//
/////////////////////////////////////////////////////////////////////////////////////

//max file size
#define MAXFILESIZE 100

//maximum number of files that we can open
#define MAXOPENEDFILES 20

//maximum number of files that we can create
#define MAXINODE 5

#define READ 1
#define WRITE 2

#define REGULARFILE 1
#define SPECIALFILE 2

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

/////////////////////////////////////////////////////////////////////////////////////
//
// User defined Macros for error handling
//
/////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1
#define ERR_NO_INODES -2
#define ERR_FILE_ALREADY_EXISTS -3
#define ERR_FILE_NOT_EXIST -4
#define ERR_PERMISSION_DENIED -5
#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7
#define ERR_MAX_FILES_OPENED -8


/////////////////////////////////////////////////////////////////////////////////////
//
// Structure name:      bootblock
// Desctiption :        holds information to boot the operating system
//
/////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

/////////////////////////////////////////////////////////////////////////////////////
//
// Structure name:      superblock
// Desctiption :        holds information about the file system
//
/////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

/////////////////////////////////////////////////////////////////////////////////////
//
// Structure name:      Inode
// Desctiption :        holds information about the file
//
/////////////////////////////////////////////////////////////////////////////////////

typedef struct Inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    int LinkCount;
    char *Buffer;
    struct Inode *next;
}INODE, *PINODE, **PPINODE;

/////////////////////////////////////////////////////////////////////////////////////
//
// Structure name:      FileTable
// Desctiption :        holds information about the opened file
//
/////////////////////////////////////////////////////////////////////////////////////

// FIXED: Typo in struct name from 'FIleTable' to 'FileTable'
typedef struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Count;
    int Mode;
    PINODE ptrinode;
}FILETABLE,*PFILETABLE;

/////////////////////////////////////////////////////////////////////////////////////
//
// Structure name:      Uarea
// Desctiption :        holds information about the process
//
/////////////////////////////////////////////////////////////////////////////////////

struct UArea
{
    char ProcessName[50];
    PFILETABLE UFDT[MAXOPENEDFILES];
};

/////////////////////////////////////////////////////////////////////////////////////
//
// Gobal variables of project
//
/////////////////////////////////////////////////////////////////////////////////////

SuperBlock superobj;
BootBlock bootobj;
PINODE head = NULL;
UArea uareaobj;

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       InitializeUAREA
// Desctiption :        It is used to initialize the contents of UAREA
// Author :             Atharva Prasad Parchue
// Date:                10/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void InitializeUAREA()
{
    strcpy(uareaobj.ProcessName,"myexe");

    int i = 0;

    while(i < MAXOPENEDFILES)
    {
        uareaobj.UFDT[i] = NULL;
        i++;
    }

    cout<<"Marvellous CVFS : UAREA initialized successfully\n";
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       InitializeSuperBlock
// Desctiption :        It is used to initialize the contents of super block
// Author :             Atharva Prasad Parchue
// Date:                10/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void InitializeSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    cout<<"Marvellous CVFS : Super block initialized successfully\n";
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       CreateDILB
// Desctiption :        It is used to create linked list of Inodes
// Author :             Atharva Prasad Parchue
// Date:                10/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while(i <= MAXINODE)
    {
        newn = new INODE;

        newn->InodeNumber = i;
        newn->FileSize = MAXFILESIZE; // Set max file size
        newn->ActualFileSize = 0;
        newn->LinkCount = 0;
        newn->Permission = 0;
        newn->FileType = 0; // 0 means file is free
        newn->ReferenceCount = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)
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
    cout<<"Marvellous CVFS : DILB created successfully\n";
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       StartAuxiliaryDataInitialization
// Desctiption :        It is used to initialize the auxiliary data
// Author :             Atharva Prasad Parchue
// Date:                10/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void StartAuxiliaryDataInitialization()
{
    strcpy(bootobj.Information , "Boot process of operating system done");
    cout<<bootobj.Information<<"\n";

    InitializeSuperBlock();
    CreateDILB();
    InitializeUAREA();

    cout<<"Marvellous CVFS : Auxiliary data initialized successfully\n";
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       DisplayHelp
// Desctiption :        It is used to display the info about commands
// Author :             Atharva Prasad Parchue
// Date:                11/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("-----------------------------------------Command Manual for Marvellous CVFS----------------------------------------\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
    printf("creat   : Create a new regular file (Usage: creat file_name permission)\n");
    printf("read    : Read data from a file (Usage: read file_descriptor bytes_to_read)\n");
    printf("write   : Write data to a file (Usage: write file_descriptor)\n");
    printf("ls      : List all created files\n");
    printf("stat    : Display statistical information of a file (Usage: stat file_name)\n");
    printf("unlink  : Delete a file (Usage: unlink file_name)\n");
    printf("man     : Display the manual page for a command (Usage: man command_name)\n");
    printf("clear   : Clear the console\n");
    printf("exit    : Terminate the Marvellous CVFS shell\n");
    printf("-------------------------------------------------------------------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       ManPage
// Desctiption :        It is used to display the manual page if the command
// input:               it accepts the command name
// output:              displays the manual details of the command
// Author :             Atharva Prasad Parchue
// Date:                11/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void ManPage(char *name)
{
    if(name == NULL) return;

    if(strcmp(name,"creat") == 0)
    {
        printf("DESCRIPTION : This command is used to create a new regular file.\n");
        printf("USAGE       : creat file_name permission\n");
        printf("permission  : 1 for Read, 2 for Write, 3 for Read & Write\n");
    }
    else if(strcmp(name, "exit") == 0)
    {
        printf("DESCRIPTION : This command is used to terminate the Marvellous CVFS.\n");
        printf("USAGE       : exit\n");
    }
    else if(strcmp(name,"unlink") == 0)
    {
        printf("DESCRIPTION : This command is used to delete a regular file.\n");
        printf("USAGE       : unlink file_name\n");
    }
    else if(strcmp(name,"stat") == 0)
    {
        printf("DESCRIPTION : This command is used to display statistical information about a file.\n");
        printf("USAGE       : stat file_name\n");
    }
    else if(strcmp(name,"ls") == 0)
    {
        printf("DESCRIPTION : This command is used to list all file names.\n");
        printf("USAGE       : ls\n");
    }
    else if(strcmp(name,"write") == 0)
    {
        printf("DESCRIPTION : This command is used to write data into a file.\n");
        printf("USAGE       : write file_descriptor\n");
        printf("NOTE        : You will be prompted to enter the data after running the command.\n");
    }
    else if(strcmp(name,"read") == 0)
    {
        printf("DESCRIPTION : This command is used to read data from a file.\n");
        printf("USAGE       : read file_descriptor size\n");
        printf("size        : Number of bytes that you want to read from the file.\n");
    }
    else
    {
        printf("No manual entry for %s\n",name);
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       IsFileExists
// Desctiption :        It is used to check whether the given file exists or not
// input:               it accepts the file name
// output:              it returns boolean value(true : present , false : not present)
// Author :             Atharva Prasad Parchue
// Date:                11/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

bool IsFileExists(char *name)
{
    PINODE temp = head;

    if(name == NULL) return false;

    while(temp != NULL)
    {
        if(temp->FileType != 0 && strcmp(name, temp->FileName) == 0)
        {
            return true;
        }
        temp = temp->next;
    }
    return false;
}


/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       CreateFile
// Desctiption :        It is used to create a new file
// input:               it accepts the file name and permission
// output:              it returns the file descriptor(fd)
// Author :             Atharva Prasad Parchue
// Date:                11/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

int CreateFile(char *name, int permission)
{
    PINODE temp = head;
    int i = 0;

    if(name == NULL || permission == 0) return ERR_INVALID_PARAMETER;
    if(permission < 1 || permission > 3) return ERR_INVALID_PARAMETER;
    if(superobj.FreeInodes == 0) return ERR_NO_INODES;
    if(IsFileExists(name) == true) return ERR_FILE_ALREADY_EXISTS;

    // Find a free entry in the User File Descriptor Table (UFDT)
    // FIXED: Loop limit changed from MAXINODE to MAXOPENEDFILES
    for(i = 0; i < MAXOPENEDFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }
    // FIXED: Condition changed from MAXINODE to MAXOPENEDFILES
    if(i == MAXOPENEDFILES)
    {
        return ERR_MAX_FILES_OPENED;
    }

    // Find a free inode
    while(temp != NULL)
    {
        if(temp->FileType == 0) // 0 means inode is free
        {
            break;
        }
        temp = temp->next;
    }

    if(temp == NULL) return ERR_NO_INODES; // Should not happen if FreeInodes > 0

    // Initialize File Table entry
    uareaobj.UFDT[i] = new FILETABLE;
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Count = 1;
    uareaobj.UFDT[i]->Mode = permission;
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialize Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, name);
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->LinkCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;
    uareaobj.UFDT[i]->ptrinode->Buffer = new char[MAXFILESIZE];

    superobj.FreeInodes--;
    return i; // Return the file descriptor (index in UFDT)
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       UnlinkFile
// Desctiption :        It is used to delete a regular file
// input:               it accepts the file name
// output:              returns success or error code
// Author :             Atharva Prasad Parchue
// Date:                15/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

int UnlinkFile(char* name)
{
    if(name == NULL) return ERR_INVALID_PARAMETER;
    if(IsFileExists(name) == false) return ERR_FILE_NOT_EXIST;
    
    // FIXED: Major logic rewrite. Now searches main inode list, not just open files.
    PINODE temp = head;
    
    // 1. Find the file in the main inode list.
    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0 && temp->FileType == REGULARFILE)
        {
            // Deallocate buffer and reset inode to make it free
            delete [] temp->Buffer;
            temp->Buffer = NULL;
            temp->FileType = 0;
            temp->ActualFileSize = 0;
            temp->LinkCount = 0;
            temp->ReferenceCount = 0;
            temp->Permission = 0;
            memset(temp->FileName, 0, 50);

            // 2. Check if the file was open and remove it from UFDT.
            for(int i = 0; i < MAXOPENEDFILES; i++)
            {
                if(uareaobj.UFDT[i] != NULL && uareaobj.UFDT[i]->ptrinode->InodeNumber == temp->InodeNumber)
                {
                    delete uareaobj.UFDT[i];
                    uareaobj.UFDT[i] = NULL;
                    break;
                }
            }

            superobj.FreeInodes++;
            printf("File '%s' unlinked successfully.\n", name);
            return EXECUTE_SUCCESS;
        }
        temp = temp->next;
    }
    
    return ERR_FILE_NOT_EXIST; // Should not be reached if IsFileExists is correct
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       ls_File
// Desctiption :        It is used to display the information about all files
// Author :             Atharva Prasad Parchue
// Date:                15/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

void ls_File()
{
    PINODE temp = head;
    printf("--------------------------------------------------\n");
    printf("File Name\n");
    printf("--------------------------------------------------\n");
    while(temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%s\n",temp->FileName);
        }
        temp = temp->next;
    }
    printf("--------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       stat_file
// Desctiption :        It is used to display the information about the given file
// Author :             Atharva Prasad Parchue
// Date:                15/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

int stat_file(char *name)
{
    if(name == NULL) return ERR_INVALID_PARAMETER;
    if(IsFileExists(name) == false) return ERR_FILE_NOT_EXIST;

    PINODE temp = head;
    while(temp != NULL)
    {
        if(strcmp(name, temp->FileName) == 0)
        {
            printf("----------------- File Statistics -----------------\n");
            printf("Filename           : %s\n", temp->FileName);
            printf("Inode Number       : %d\n", temp->InodeNumber);
            printf("File size on disk  : %d bytes\n", temp->FileSize);
            printf("Actual file size   : %d bytes\n", temp->ActualFileSize);
            printf("Link count         : %d\n", temp->LinkCount);
            printf("Reference count    : %d\n", temp->ReferenceCount);
            printf("File permission    : ");
            if(temp->Permission == 1) printf("Read\n");
            else if(temp->Permission == 2) printf("Write\n");
            else if(temp->Permission == 3) printf("Read & Write\n");
            printf("File type          : Regular File\n");
            printf("-------------------------------------------------\n");
            break;
        }
        temp = temp->next;
    }
    return EXECUTE_SUCCESS;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       write_file
// Desctiption :        It is used to write the contents into the given file
// Author :             Atharva Prasad Parchue
// Date:                15/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

int write_file(int fd, char *data, int size)
{
    if(fd < 0 || fd >= MAXOPENEDFILES) return ERR_INVALID_PARAMETER;
    if(uareaobj.UFDT[fd] == NULL) return ERR_FILE_NOT_EXIST;
    
    if((uareaobj.UFDT[fd]->ptrinode->Permission & WRITE) == 0) return ERR_PERMISSION_DENIED;

    if((uareaobj.UFDT[fd]->ptrinode->FileSize - uareaobj.UFDT[fd]->WriteOffset) < size) return ERR_INSUFFICIENT_SPACE;

    // Write data from user buffer to inode buffer
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    uareaobj.UFDT[fd]->WriteOffset += size;
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize += size;
    
    return size;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Function name:       read_file
// Desctiption :        It is used to read the contents from the given file
// Author :             Atharva Prasad Parchue
// Date:                15/8/2025
//
/////////////////////////////////////////////////////////////////////////////////////

int read_file(int fd, char *data, int size)
{
    if(fd < 0 || fd >= MAXOPENEDFILES) return ERR_INVALID_PARAMETER;
    if(uareaobj.UFDT[fd] == NULL) return ERR_FILE_NOT_EXIST;
    if(data == NULL || size <= 0) return ERR_INVALID_PARAMETER;
    
    
    if((uareaobj.UFDT[fd]->ptrinode->Permission & READ) == 0) return ERR_PERMISSION_DENIED;

    if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize - uareaobj.UFDT[fd]->ReadOffset) < size) return ERR_INSUFFICIENT_DATA;

    // Read data from inode buffer into user buffer
    strncpy(data, uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    uareaobj.UFDT[fd]->ReadOffset += size;
    
    return size;
}

/////////////////////////////////////////////////////////////////////////////////////
//
// Entry point function of project (main)
//
/////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80];
    int iCount = 0;
    int iRet = 0;
    char Command[4][80]; // Reduced to 4 as we parse max 4 tokens
    char InputBuffer[MAXFILESIZE];

    StartAuxiliaryDataInitialization();

    cout<<"-------------------------------------------------------------------------------------------------------------------\n";
    cout<<"------------------------------------------UNIX CVFS started successfully--------------------------------------------\n";
    cout<<"-------------------------------------------------------------------------------------------------------------------\n";
    DisplayHelp();

    while(1)
    {
        printf("\nUNIX CVFS > ");
        
        // Read a full line of input
        if (fgets(str, sizeof(str), stdin) == NULL) {
            // End of file (Ctrl+D) or error
            break;
        }

        // Remove trailing newline character from fgets
        str[strcspn(str, "\n")] = 0;

        // Parse the input string into command tokens
        // FIXED: sscanf is now width-limited to prevent buffer overflows
        iCount = sscanf(str, "%79s %79s %79s %79s", Command[0], Command[1], Command[2], Command[3]);

        if(iCount == 0) {
            continue; // Empty input
        }

        if(iCount == 1)
        {
            if(strcmp(Command[0], "exit") == 0)
            {
                printf("Thank you for using UNIX CVFS.\n");
                // TODO: Add deallocation logic for all allocated resources before exiting
                break;
            }
            else if(strcmp(Command[0], "help") == 0)
            {
                DisplayHelp();
            }
            else if(strcmp(Command[0], "clear") == 0)
            {
                system("clear || cls"); // Works on Linux/macOS and Windows
            }
            else if(strcmp(Command[0], "ls") == 0)
            {
                ls_File();
            }
            else
            {
                printf("Command not found. Please refer to 'help'.\n");
            }
        }
        else if(iCount == 2)
        {
            if(strcmp(Command[0], "man") == 0)
            {
                ManPage(Command[1]);
            }
            else if(strcmp(Command[0], "unlink") == 0)
            {
                iRet = UnlinkFile(Command[1]);
                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error: Cannot unlink, file '%s' not found.\n", Command[1]);
                }
            }
            else if(strcmp(Command[0], "stat") == 0)
            {
                iRet = stat_file(Command[1]);
                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error: Cannot get stats, file '%s' not found.\n", Command[1]);
                }
            }
            else if(strcmp(Command[0], "write") == 0)
            {
                printf("Enter data to write: ");
                // FIXED: Robust input reading and size calculation
                fgets(InputBuffer, MAXFILESIZE, stdin);
                InputBuffer[strcspn(InputBuffer, "\n")] = 0; // Remove trailing newline
                int writeSize = strlen(InputBuffer);

                iRet = write_file(atoi(Command[1]), InputBuffer, writeSize);

                if(iRet >= 0)
                {
                    printf("%d bytes successfully written.\n", iRet);
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error: Insufficient space in the file.\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error: Write permission denied.\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error: File with descriptor %s is not open.\n", Command[1]);
                }
            }
            else
            {
                printf("Command not found or wrong number of arguments. Please refer to 'help'.\n");
            }
        }
        else if(iCount == 3)
        {
            if(strcmp(Command[0], "creat") == 0)
            {
                iRet = CreateFile(Command[1], atoi(Command[2]));
                if(iRet >= 0)
                {
                    printf("File '%s' created successfully with file descriptor: %d\n", Command[1], iRet);
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error: Invalid parameters. Check 'man creat'.\n");
                }
                else if(iRet == ERR_NO_INODES)
                {
                    printf("Error: No free inodes available to create file.\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXISTS)
                {
                    printf("Error: File '%s' already exists.\n", Command[1]);
                }
                 else if(iRet == ERR_MAX_FILES_OPENED)
                {
                    printf("Error: Maximum number of files are already open.\n");
                }
            }
            else if(strcmp(Command[0], "read") == 0)
            {
                int readSize = atoi(Command[2]);
                char *EmptyBuffer = NULL;

                
                if (readSize > 0) {
                    EmptyBuffer = new char[readSize + 1];
                    memset(EmptyBuffer, 0, readSize + 1); // Clear buffer

                    iRet = read_file(atoi(Command[1]), EmptyBuffer, readSize);
                    
                    if(iRet > 0)
                    {
                        printf("Read successful. Data: %s\n", EmptyBuffer);
                    }
                    else if(iRet == ERR_INSUFFICIENT_DATA)
                    {
                        printf("Error: Not enough data in file to read.\n");
                    }
                    else if(iRet == ERR_PERMISSION_DENIED)
                    {
                        printf("Error: Read permission denied.\n");
                    }
                    else if(iRet == ERR_FILE_NOT_EXIST)
                    {
                        printf("Error: File with descriptor %s is not open.\n", Command[1]);
                    }

                    
                    delete [] EmptyBuffer;
                } else {
                    printf("Error: Read size must be greater than zero.\n");
                }
            }
            else
            {
                printf("Command not found or wrong number of arguments. Please refer to 'help'.\n");
            }
        }
        else
        {
            printf("Command not found or too many arguments. Please refer to 'help'.\n");
        }
    }

    return 0;
}