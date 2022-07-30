#pragma once

#ifdef BLECOMMUNICATION_EXPORTS
#define BLECOMMUNICATION_API _declspec(dllexport)
#else
#define BLECOMMUNICATION_API _declspec(dllimport)
#endif