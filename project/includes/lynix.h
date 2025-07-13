#ifndef LYNIX_H
#define LYNIX_H

#ifdef _WIN32
#ifdef LYNIX_EXPORTS
#define LYNIX_API __declspec(dllexport)
#else
#define LYNIX_API __declspec(dllimport)
#endif
#else
#define LYNIX_API __attribute__((visibility("default")))
#endif



#endif