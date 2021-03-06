/**
* @file support_cosim.h
*
* @brief This file contains helper functions to unzip and load the FMU to memory. Current version works for FMI - Version 1.0.
* This package is one of the different packages of hysim - hybrid simulation
*
* @author Thiyagarajan Purusothaman
*
* @date June, 25th 2014 - hysim v1.0
*
**/

// Used 7z options, version 4.57:
// -x   Extracts files from an archive with their full paths in the current dir, or in an output dir if specified
// -aoa Overwrite All existing files without prompt
// -o   Specifies a destination directory where files are to be extracted
#define UNZIP_CMD "7z x -aoa -o"

#define XML_FILE  "modelDescription.xml"

#if WINDOWS
#define DLL_DIR   "binaries\\win32\\"
#define DLL_SUFFIX ".dll"

#define DLL_DIR2   "binaries\\win32\\"
#define DLL_SUFFIX2 ".dll"
#else
#if __APPLE__

// Use these for platforms other than OpenModelica
#define DLL_DIR   "binaries/darwin64/"
#define DLL_SUFFIX ".dylib"

// Use these for OpenModelica 1.8.1
#define DLL_DIR2   "binaries/darwin-x86_64/"
#define DLL_SUFFIX2 ".so"

#else /*__APPLE__*/
// Linux
#ifdef __x86_64
#define DLL_DIR   "binaries/linux64/"
#define DLL_DIR2   "binaries/linux32/"
#else
// It may be necessary to compile with -m32, see ../Makefile
#define DLL_DIR   "binaries/linux32/"
#define DLL_DIR2   "binaries/linux64/"
#endif /*__x86_64*/
#define DLL_SUFFIX ".so"
#define DLL_SUFFIX2 ".so"
#endif /*__APPLE__*/
#endif /*WINDOWS*/

#define RESULT_FILE "result.csv"
#define BUFSIZE 4096

// return codes of the 7z command line tool
#define SEVEN_ZIP_NO_ERROR 0 // success
#define SEVEN_ZIP_WARNING 1  // e.g., one or more files were locked during zip
#define SEVEN_ZIP_ERROR 2
#define SEVEN_ZIP_COMMAND_LINE_ERROR 7
#define SEVEN_ZIP_OUT_OF_MEMORY 8
#define SEVEN_ZIP_STOPPED_BY_USER 255
#ifdef __cplusplus
extern "C" {
#endif

int unzip(const char *zipPath, const char *outPath);
void fmuLogger(fmiComponent c, fmiString instanceName, fmiStatus status,
		fmiString category, fmiString message, ...);
ScalarVariable* getSV(FMU* fmu, char type, fmiValueReference vr);
ScalarVariable* getSV_CS(FMU* fmu, char type, fmiValueReference vr);
const char* fmiStatusToString(fmiStatus status);
const char* fmiStatusToString_CS(fmiStatus status);
char* loadFMU(char* fmuFileName, FMU *fmu);

#ifndef _MSC_VER
typedef int boolean;
#endif

int error(const char* message);
void printHelp(const char* fmusim);

#ifdef __cplusplus
}
#endif
