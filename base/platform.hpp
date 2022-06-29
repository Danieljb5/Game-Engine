#pragma once

#ifdef __unix__
#ifndef LINUX
#define LINUX
#endif
#endif

#if defined(_WIN32) || defined(WIN32)
#ifndef WINDOWS
#define WINDOWS
#endif
#endif

#ifndef __unix__
#if defined(_WIN32) || defined(WIN32)
#else
#error Unsupported platform
#endif
#endif