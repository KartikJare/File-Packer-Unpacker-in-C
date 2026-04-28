#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

struct FileHeader
{
    char FileName[100];
    int Filesize;
};

void PackFiles(char * DirName,char *PackedFile)
{
    DIR *dp;
    struct dirent *strat;
    int source,Dest;
    struct FileHeader fobj;
    char Buffer[BUFFER_SIZE];
    int bytesRead;

    dp = opendir(DirName);
    if(dp == NULL)
    {
        printf("Unable to open directory\n");
        return;
    }

    Dest = open(PackedFile,O_WRONLY | O_CREAT | O_TRUNC,0777);
    if(Dest < 0)
    {
        printf("Unable to create packed file\n");
        closedir(dp);
        return;
    }

    while((strat = readdir(dp)) != NULL)
    {
        if(strcmp(strat->d_name,".") == 0 || strcmp(strat->d_name,"..") == 0)
        {
            continue;
        }

        char path[200];
        struct stat filestat;

        snprintf(path,sizeof(path),"%s/%s",DirName,strat->d_name);
        
        if(stat(path,&filestat) != 0)
        {
            printf("stat failed\n");
            continue;
        }

        if(!S_ISREG(filestat.st_mode))
        {
            continue;
        }
        
        source = open(path,O_RDONLY);
        if(source < 0)
        {
            printf("Unable to open source file\n");
            continue;
        }

        strcpy(fobj.FileName,strat->d_name);

        fobj.Filesize = lseek(source,0,SEEK_END);
        lseek(source,0,SEEK_SET);

        write(Dest,&fobj,sizeof(fobj));

        while((bytesRead = read(source,Buffer,sizeof(Buffer))) > 0)
        {
            write(Dest,Buffer,bytesRead);
        }

        close(source);
    }
    
    close(Dest);
    closedir(dp);

    printf("Packing completed Successfully...\n");
}

void UnpackedFiles(char *PackFiles,char *OutDir)
{
    int Source,Dest;
    struct FileHeader fobj;
    char Buffer[BUFFER_SIZE];
    int bytesRead;

    Source = open(PackFiles,O_RDONLY);
    if(Source < 0)
    {
        printf("Unable to open packed file\n");
        return;
    }

    mkdir(OutDir,0777);

    while(read(Source,&fobj,sizeof(fobj)) > 0)
    {
        char newPath[200];
        snprintf(newPath, sizeof(newPath), "%s/%s", OutDir, fobj.FileName);
        
        Dest = open(newPath,O_WRONLY | O_CREAT | O_TRUNC,0777);
        if(Dest < 0)
        {
            printf("Unable to create file\n");
            continue;
        }

        int remaining = fobj.Filesize;

        while (remaining > 0)
        {
            int chunk = (remaining > sizeof(Buffer)) ? sizeof(Buffer) : remaining;

            bytesRead = read(Source,Buffer,chunk);
            write(Dest,Buffer,bytesRead);

            remaining -= bytesRead;
        }
        close(Dest);
    }

    close(Source);

    printf("Unpacking completed successfully...\n");
}

int main(int argc,char * argv[])
{
    if(argc < 4)
    {
        printf("Usage: To show packing and unpacking\n");
        printf("Pack   : %s pack <Directory> <OutputFile>\n", argv[0]);
        printf("Unpack : %s unpack <PackedFile> <OutputDir>\n", argv[0]);
        return -1;
    }

    if(strcmp(argv[1], "pack") == 0)
    {
        PackFiles(argv[2], argv[3]);
    }
    else if(strcmp(argv[1], "unpack") == 0)
    {
        UnpackedFiles(argv[2],argv[3]);
    }
    else
    {
        printf("Invalid option\n");
    }

    return 0;
}