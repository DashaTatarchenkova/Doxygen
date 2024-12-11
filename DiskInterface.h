#ifndef DISKINTERFACE_H_INCLUDED
#define DISKINTERFACE_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

using namespace std;

/**
 * @class Reader
 * @brief Класс для чтения данных с диска или файла.
 *
 * Класс позволяет открыть диск  и прочитать из него данные
 * в указанный буфер.
 */
class Reader
{
private:
    #ifdef __WIN32
    HANDLE diskHandle; ///< Хэндл для доступа к диску.
    DWORD NumberOfBytesRead; ///< Количество байт, прочитанных при последней операции.
    #elif defined(__linux__)
    bool isDone = false; ///< Флаг завершения чтения.
    ifstream inputDisk;
    #endif

public:

    Reader(){};

    ~Reader();

    /**
     * @brief Открывает диск или файл.
     * @param disk Указатель на строку с именем диска или файла.
     * @return true, если операция прошла успешно.
     * @return false, если возникла ошибка.
     */
    bool OpenDisk(const wchar_t* disk);

    /**
     * @brief Читает данные из открытого диска или файла.
     * @param lpBuffer Буфер, куда будут записаны данные.
     * @param numberOfBytesToRead Количество байт для чтения.
     * @return true, если операция завершена успешно.
     * @return false, если возникла ошибка.
     */
    bool Read(unsigned char lpBuffer[], unsigned long numberOfBytesToRead);

    /**
     * @brief Проверяет, завершено ли чтение файла.
     * @return true, если файл прочитан полностью.
     * @return false, если чтение не завершено.
     */
    bool IsDone();

    /**
     * @brief Перемещает указатель чтения в файле.
     * @param Distance Смещение в байтах относительно текущей позиции.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool SetFilePointer(int64_t Distance);
};

/**
 * @class Writer
 * @brief Класс для записи данных в файл.
 *
 * Класс позволяет открыть файл и записать в него данные из переданного буфера.
 */
class Writer
{
private:
    #ifdef __WIN32
    HANDLE outFileHandle; ///< Хэндл для доступа к файлу.
    DWORD NumberOfBytesWritten; ///< Количество байт, записанных при последней операции.
    #elif defined(__linux__)
    ofstream outputImage;
    #endif

public:

    Writer(){};

    ~Writer();

    /**
     * @brief Открывает файл для записи.
     * @param outFile Указатель на строку с именем файла.
     * @return true, если файл успешно открыт.
     * @return false, если возникла ошибка.
     */
    bool OpenFile(const wchar_t* outFile);

    /**
     * @brief Записывает данные в файл.
     * @param lpBuffer Буфер с данными для записи.
     * @param numberOfBytesToWrite Количество байт для записи.
     * @return true, если операция завершена успешно.
     * @return false, если возникла ошибка.
     */
    bool Write(unsigned char lpBuffer[], unsigned long numberOfBytesToWrite);

    /**
     * @brief Перемещает указатель записи в файле.
     * @param Distance Смещение в байтах относительно текущей позиции.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool SetFilePointer(int64_t Distance);
};

#endif // DISKINTERFACE_H_INCLUDED
