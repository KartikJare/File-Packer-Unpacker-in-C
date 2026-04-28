File Packager & Unpackager
A lightweight C-based utility designed to archive multiple files from a directory into a single custom packed file and extract them back into a specified directory. This project demonstrates low-level file I/O operations and directory handling in a Linux environment.

🛠 Features
Directory Packing: Scans a source directory and bundles all regular files into one archive.

Metadata Preservation: Uses a custom FileHeader structure to store filenames and sizes within the archive.

Automated Extraction: Recreates the destination directory and restores files with their original names and content.

System Level I/O: Utilizes high-performance system calls including open(), read(), write(), and lseek().

🏗 Data Structure
The program prepends a header to every file's data within the archive to track metadata:

C
struct FileHeader {
    char FileName[100]; // Original name of the file
    int Filesize;       // Size of the file in bytes
};
🚀 Usage
1. Compilation
Use gcc to compile the source code:

Bash
gcc Package&unpackage.c -o mypacker
2. Packing Files
To bundle files from a directory into a single archive:

Bash
./mypacker pack <Source_Directory> <Output_Archive_Name>
Example: ./mypacker pack ./my_docs archive.dat

3. Unpacking Files
To extract files from an archive into a new directory:

Bash
./mypacker unpack <Archive_Name> <Destination_Directory>
Example: ./mypacker unpack archive.dat ./extracted_files

📝 Logic Overview
Packing Process (PackFiles)
Opens the target directory using opendir() and iterates through entries via readdir().

Filters out . and .. and ensures only regular files are processed via S_ISREG.

Calculates file size using lseek to the end of the file.

Writes the FileHeader followed by the raw file content into the destination archive.

Unpacking Process (UnpackedFiles)
Reads the FileHeader from the archive to determine the filename and exact byte count to read.

Creates the output directory and opens a new file for writing.

Reads the content in chunks defined by BUFFER_SIZE (1024 bytes) until the specific file size is reached.

⚠️ Limitations
Filename Length: Names are capped at 99 characters.

Non-Recursive: Does not currently pack subdirectories inside the source folder.

Permissions: Files are recreated with 0777 permissions rather than original file modes.
