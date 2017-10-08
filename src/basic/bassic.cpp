#include "bassic.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <basic/basic.h>
#include <QDebug>
#include "global/global_definitions.h"


#ifdef __linux
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#else
#include <windows.h>
#endif



using namespace std;
void sendLogMessage(const QString& logMessage)
{
    cerr << logMessage.toStdString() << endl;
}


void doErrorExit(const char*message, const char *argument)
{
    fprintf(stderr, message, argument);
    exit(EXIT_FAILURE);
}


template <>
double findMinInQVector(const QVector<double>& vector)
{
    auto stdVector = vector.toStdVector();
    return *(std::min_element(stdVector.begin(), stdVector.end(),
                              [] (double x1, double x2)->bool
                                 {
                                     if (isfinite(x1)) {
                                         if (isfinite(x2))
                                             return x1 < x2;
                                         else
                                             return true;
                                     } else {
                                         return false;
                                     }
                                 }));
}

template <>
double findMaxInQVector(const QVector<double>& vector)
{
    auto stdVector = vector.toStdVector();
    return *(std::max_element(stdVector.begin(), stdVector.end(),
                              [] (double x1, double x2)->bool
                                 {
                                     if (isfinite(x1)) {
                                         if (isfinite(x2))
                                             return x1 < x2;
                                         else
                                             return false;
                                     } else {
                                         return true;
                                     }
                                 }
                              ));
}


//special case for double values to support not finite values
template <>
double findMinInQVectorInRange(const QVector<double>& vector, const QVector<double>& rangeVector, double rangeBegin, double rangeEnd)
{
    auto stdVector = vector.toStdVector();
    auto stdRangeVector = rangeVector.toStdVector();
    auto lowerBound = lower_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeBegin);
    size_t low_index = lowerBound - stdRangeVector.begin();
    auto upperBound = upper_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeEnd);
    size_t high_index = upperBound - stdRangeVector.begin();
    if (low_index >= high_index) {
        return NAN;
    }
    return *(std::min_element(stdVector.begin()+low_index, stdVector.begin()+high_index,
             [] (double x1, double x2)->bool
                {
                    if (isfinite(x1)) {
                        if (isfinite(x2))
                            return x1 < x2;
                        else
                            return true;
                    } else {
                        return false;
                    }
                })
            );




}

//special case for double values to support not finite values
template <>
double findMaxInQVectorInRange(const QVector<double>& vector, const QVector<double>& rangeVector, double rangeBegin, double rangeEnd)
{
    //rangeVector must be sorted
        auto stdVector = vector.toStdVector();
        auto stdRangeVector = rangeVector.toStdVector();
        auto lowerBound = lower_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeBegin);
        size_t low_index = lowerBound - stdRangeVector.begin();
        auto upperBound = upper_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeEnd);
        size_t high_index = upperBound - stdRangeVector.begin();
        if (low_index >= high_index) {
            return NAN;
        }
        return *(std::max_element(stdVector.begin()+low_index, stdVector.begin()+high_index,
                  [] (double x1, double x2)->bool
                     {
                         if (isfinite(x1)) {
                             if (isfinite(x2))
                                 return x1 < x2;
                             else
                                 return false;
                         } else {
                             return true;
                         }
                     })
                 );
}

//template <typename T>
//T findMaxInQVectorInRange(const QVector<T>& vector, const QVector<T>& rangeVector, T rangeBegin, T rangeEnd)
//{
//    //rangeVector must be sorted
//    auto stdVector = vector.toStdVector();
//    auto stdRangeVector = rangeVector.toStdVector();
//    auto lowerBound = lower_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeBegin);
//    size_t low_index = lowerBound - stdRangeVector.begin();
//    auto upperBound = upper_bound(stdRangeVector.begin(), stdRangeVector.end(), rangeEnd);
//    size_t high_index = upperBound - stdRangeVector.begin();
//    return *(std::max_element(stdVector.begin()+low_index, stdVector.begin()+high_index));
//}


QString stripDirectoryFromFileName(const QString& fileName) {
    QString outFileName = fileName;
    int index;
    if ((index = outFileName.lastIndexOf("/")) != -1) {
        outFileName.remove(0, index + 1);
    }
    if ((index = outFileName.lastIndexOf("\\")) != -1) {
        outFileName.remove(0, index + 1);
    }
    return outFileName;
}

QString createIndexedFileName(const QString& fileName, int index) {
    QString ret(fileName);
    int indexOfLastPoint = ret.lastIndexOf(".");
    ret.insert(indexOfLastPoint, "_" + QString::number(index));
    return ret;
}







/////////////////////////////////////////////////////
#ifdef __linux
static int lockFileId = 0;
bool lockUniqueResource()
{

   if ((lockFileId = open("/tmp/JaGLockFile", O_WRONLY | O_CREAT, 0777)) == -1)
       return false;

   struct flock flock_st;
   flock_st.l_type = F_WRLCK;
   flock_st.l_whence = SEEK_SET;
   flock_st.l_start = 0;
   flock_st.l_len  = 0;
   flock_st.l_pid  = getpid();

   if (fcntl(lockFileId, F_SETLK, &flock_st) != 0) {
       return false;
   }

   return true;

}

bool unlockUniqueResource()
{
    return !close(lockFileId);
}

#else

static HANDLE hMutex;
bool lockUniqueResource()
{
    try {
        wchar_t lpMutexName[]=L"JaGUniqueMutex";

//        hMutex = OpenMutex(
//              MUTEX_ALL_ACCESS, 0, "JaGUniqueMutex");
        hMutex = OpenMutex(
              MUTEX_ALL_ACCESS, 0, lpMutexName);

        if (!hMutex) {
            // Mutex doesn’t exist. This is
            // the first instance so create
            // the mutex.
//            hMutex = CreateMutex(
//                NULL,                        // default security descriptor
//                FALSE,                       // mutex not owned
//                TEXT("JaGUniqueMutex"));     // object name

            hMutex = CreateMutex(
                NULL,                        // default security descriptor
                FALSE,                       // mutex not owned
                lpMutexName);     // object name

            return true;
        } else {
            // The mutex exists so this is the
            // the second instance so return.
            return false;
        }
    } catch(...) {
        return false;
    }

}

bool unlockUniqueResource()
{
    ReleaseMutex(hMutex);
    return true;
}

#endif


QString valueToString(double value, GraphModel::StringRepresentation format)
{
    switch (format) {
        case GraphModel::StringRepresentation::Decimal:
            return QString::number(value, 'g', global::gDoubleToStringConversionPrecision);
            break;
        case GraphModel::StringRepresentation::Hexadecimal:
            {
            QString hexRepresentation = QString::number(static_cast<unsigned>(value), 16);
            //adding some stuff to make it pretty
            //suppose that each value at least represent some 2 byte unsigned number
            hexRepresentation = QString((64 - hexRepresentation.size()) % 4, '0') + hexRepresentation;
            QString prettyRepresentation;
            for (int i = 0; i < hexRepresentation.size()/4; ++i) {
                prettyRepresentation += " " + hexRepresentation.mid(i*4, 4);
            }
            return "0x" + prettyRepresentation;
            }
            break;
        case GraphModel::StringRepresentation::Binary:
            {
            QString binRepresentation = QString::number(static_cast<unsigned>(value), 2);
            //adding some stuff to make it pretty
            //suppose that each value at least represent some 2 byte unsigned number
            binRepresentation = QString((128 - binRepresentation.size()) % 4, '0') + binRepresentation;
            QString prettyRepresentation;
            for (int i = 0; i < binRepresentation.size()/4; ++i) {
                prettyRepresentation += " " + binRepresentation.mid(i*4, 4);
            }
            return "0b" + prettyRepresentation;
            }
            break;
        default:
            return "";
            break;
    }


}
