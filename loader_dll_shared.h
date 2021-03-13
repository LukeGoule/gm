#ifndef __SHAREDH__
#define __SHAREDH__
/*
loader_dll_shared.h

This file's sole purpose is to make sure the loader and cheat are PER-FUCKING-FECTLY aligned so
when the loader scans the DLL data for these strings nothing can go wrong.

Another point is I don't think this system of loader information sharing could work if I were to
pack the DLL. I could resort to some kind of temporary file that holds the username and a hash of
the password (this is if I were to make a website etc etc out of it, not likely to happen - for now :))
*/

#define USERNAME_FINDER_DEFAULT         "USERNAMEOVERRIDEFINDER"
#define PASSWORD_FINDER_DEFAULT         "PASSWORDOVERRIDEFINDER"
#define MACHINEID_FINDER_DEFAULT        "MACHINEIDOVERRIDEFINDER"

// real bytes with 0s appended since the array we are looking to write to is mostly 0s.
#define USERNAME_FINDER_DEFAULT_BYTES   0x55, 0x53, 0x45, 0x52, 0x4e, 0x41, 0x4d, 0x45, 0x4f, 0x56, 0x45, 0x52, 0x52, 0x49, 0x44, 0x45, 0x46, 0x49, 0x4e, 0x44, 0x45, 0x52, 
#define PASSWORD_FINDER_DEFAULT_BYTES   0x50, 0x41, 0x53, 0x53, 0x57, 0x4f, 0x52, 0x44, 0x4f, 0x56, 0x45, 0x52, 0x52, 0x49, 0x44, 0x45, 0x46, 0x49, 0x4e, 0x44, 0x45, 0x52,
#define MACHINEID_FINDER_DEFAULT_BYTES  0x4d, 0x41, 0x43, 0x48, 0x49, 0x4e, 0x45, 0x49, 0x44, 0x4f, 0x56, 0x45, 0x52, 0x52, 0x49, 0x44, 0x45, 0x46, 0x49, 0x4e, 0x44, 0x45, 0x52,

#endif