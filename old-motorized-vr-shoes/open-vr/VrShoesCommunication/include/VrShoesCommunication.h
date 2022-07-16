#pragma once

#ifdef VRSHOESCOMMUNICATION_EXPORTS
#define VRSHOESCOMMUNICATION_API _declspec(dllexport)
#else
#define VRSHOESCOMMUNICATION_API _declspec(dllimport)
#endif