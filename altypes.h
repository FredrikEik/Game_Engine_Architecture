#ifndef ALTYPES_H
#define ALTYPES_H

//Platform differences makes it cleaner to have this in separate file
#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif
#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#endif // ALTYPES_H
