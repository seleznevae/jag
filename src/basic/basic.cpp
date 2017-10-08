#include "basic/basic.h"
#include <stdio.h>
#include <stdlib.h>
#include <QMessageBox>
#include "global/global_definitions.h"
#include <stdexcept>
#include "core/jagmainwindow.h"


void errorExit(const char *str, const char *arg) {

//    if (gJagMainWindowPointer) {
//        char buffer[1000] = {'\0'};
//        sprintf(buffer, str, arg);
//        QMessageBox::critical (gJagMainWindowPointer, "Fatal error", buffer);
//    }
    fprintf(stderr, str, arg);
    exit(EXIT_FAILURE);
}


void throwRuntimeError(const char *str, const char *arg)
{
    char *rawMessage = (char*)malloc (strlen(str) + 100 + (arg ? strlen(arg) : 0));
    if (rawMessage == NULL) {
        errorExit("Can't allocate enough memory in %s\n", __FUNCTION__);
    }
    sprintf(rawMessage, str, arg);
    std::string messageString(rawMessage);
    free(rawMessage);
    throw std::runtime_error(messageString);

}


void showCriticalMessage(const char *str, const char *arg)
{
    if (global::gJagMainWindowPointer) {
        char buffer[1000] = {'\0'};
        sprintf(buffer, str, arg);
//        QMessageBox::critical(gJagMainWindowPointer, "Fatal error", buffer);
        showCriticalMessage(buffer);
    }
}


void showCriticalMessage(QString message)
{
    if (global::gJagMainWindowPointer) {
        QMessageBox::critical(global::gJagMainWindowPointer, "Fatal error", message);
    }
}


void throwIOError(const char *str, const char *arg)
{
    char *rawMessage = (char*)malloc (strlen(str) + 100 + (arg ? strlen(arg) : 0));
    if (rawMessage == NULL) {
        errorExit("Can't allocate enough memory in %s\n", __FUNCTION__);
    }
    sprintf(rawMessage, str, arg);
    std::string messageString(rawMessage);
    free(rawMessage);
    throw IOError(messageString);
}
