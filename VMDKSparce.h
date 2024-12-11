/**
 * @file VMDKSparse.h
 * @brief Заголовочный файл для создания sparse-файла формата VMDK.
 */

#ifndef VMDKSPARCE_H_INCLUDED
#define VMDKSPARCE_H_INCLUDED

#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sstream>
#include <cmath>
#include <cstring>
#include "VMDK.h"

#define SECTOR_SIZE 512               ///< Размер сектора в байтах
#define HEADS 16                      ///< Количество головок
#define SECTORS 63                    ///< Количество секторов

#define VMDK_MAGICNUMBER 0x564D444B   ///< 'VMDK' в hex (магическое число)
#define VMDK_VERSION 1                ///< Версия VMDK
#define GRAIN_SIZE 128                ///< Размер зерна в секторах (64K)
#define DESCRIPTOR_SIZE 1             ///< Размер дескриптора (в секторах)
#define VMDK_HEADER_SIZE 512          ///< Размер заголовка (512 байт)

// Константы для конфигурации виртуального диска
constexpr uint32_t grainSize = 128;                         ///< Размер блока данных (зерна) 128 секторов
constexpr uint32_t GTE_COUNT = 512;                         ///< Количество записей в таблице зерен
constexpr uint32_t gtCoverage = GTE_COUNT * grainSize;      ///< Покрытие одной таблицы зерен (32 MB для G = 7)
constexpr uint32_t NUM_SECTORS = 128;                       ///< Количество секторов в буфере
constexpr uint32_t BUFFER_SIZE = NUM_SECTORS * SECTOR_SIZE; ///< Размер буфера


/**
 * @class SparseVMDK
 * @brief Класс для создания sparse-файлов формата VMDK.
 *
 * Класс `SparseVMDK` предоставляет функции для создания sparse VMDK-файла и его структуры,
 * включая генерацию CID и обработку строковых форматов.
 */
class SparseVMDK {
public:

    #ifdef _WIN32
    /**
     * @brief Конструктор для создания экземпляра класса FlatVMDK на платформе Windows.
     *
     * Инициализирует генератор случайных чисел и устанавливает базовые параметры для VMDK-файла.
     *
     * @param[in] outDir Директория для сохранения VMDK-файла.
     * @param[in] outName Имя создаваемого VMDK-файла.
     * @param[in] d Имя исходного диска или файла, из которого будет произведено копирование данных.
     */
    SparseVMDK(std::wstring outDir, std::wstring outName, std::wstring d) {
        srand(static_cast<unsigned int>(time(nullptr)));
        outFileDir = outDir;
        outFileName = outName;
        disk = d;
    }
    #endif // _WIN32

    #ifdef __linux__
    /**
     * @brief Конструктор для создания экземпляра класса FlatVMDK на платформе Windows.
     *
     * Инициализирует генератор случайных чисел и устанавливает базовые параметры для VMDK-файла.
     *
     * @param[in] outDir Директория для сохранения VMDK-файла.
     * @param[in] outName Имя создаваемого VMDK-файла.
     * @param[in] d Имя исходного диска или файла, из которого будет произведено копирование данных.
     */
    SparseVMDK(std::string outDir, std::string outName, std::string d) {
        srand(static_cast<unsigned int>(time(nullptr)));
        outFileDir = outDir;
        outFileName = outName;
        disk = d;
    }
    #endif // __linux__
    /**
     * @brief Создает sparse-файл VMDK формата.
     *
     * Основной метод для создания sparse-файла VMDK и его дескриптора,
     * включая расчет геометрии и инициализацию данных.
     *
     * @param[in] bufSize Размер буфера для копирования данных.
     * @param[in] capacitySectors Общее количество секторов на диске.
     * @return Возвращает `true` при успешном создании файла, иначе `false`.
     */
    bool CreateSparse(unsigned long bufSize, uint64_t capacitySectors);
private:
    #ifdef _WIN32
    std::wstring outFileDir;  ///< Директория прописанная пользователем (Windows).
    std::wstring outFileName; ///< Имя файла прописанная пользователем (Windows).
    std::wstring disk;        ///< Имя исходного диска или файла (Windows).
    #endif // _WIN32

    #ifdef __linux__
    std::string outFileDir;   ///< Директория прописанная пользователем (Linux).
    std::string outFileName;  ///< Имя файла прописанная пользователем (Linux).
    std::string disk;         ///< Имя исходного диска или файла (Linux).
    #endif // __linux__

    /**
     * @brief Преобразует строку типа `std::wstring` в строку типа `std::string`.
     *
     * Метод, необходимый для работы со строками в различных кодировках на разных платформах.
     *
     * @param[in] wstr Широкая строка (`std::wstring`) для преобразования.
     * @return Преобразованная строка типа `std::string`.
     */
    //Метод преобразования wstring в string
    std::string WCharToString(const std::wstring& wstr) {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wstr); // Преобразуем
    }

    /**
     * @brief Генерирует случайный CID (идентификатор) для VMDK-дескриптора.
     *
     * CID используется для уникальной идентификации диска и является частью дескриптора VMDK.
     *
     * @return Случайное 32-битное 8-ми значное целое число, представляющее уникальный идентификатор CID.
     */
    //Генерация случайного CID для дескриптора VMDK
    uint32_t generateRandomCID() {
        return 10000000 + (rand() % 90000000);
    }
};

#pragma pack(1)  // Обеспечиваем выравнивание структуры по 1 байту

/**
 * @struct SparseExtentHeader
 * @brief Структура для хранения заголовка sparse VMDK.
 *
 * Структура `SparseExtentHeader` содержит основные поля для описания
 * sparse VMDK-файла, включая магическое число, емкость, размер зерна и другие параметры.
 */
typedef struct SparseExtentHeader
{
    uint32_t    magicNumber;        ///< Магическое число VMDK ('VMDK' в hex)
    uint32_t    version;            ///< Версия VMDK
    uint32_t    flags;              ///< Флаги конфигурации VMDK
    uint64_t    capacity;           ///< Емкость в секторах
    uint64_t    grainSize;          ///< Размер зерна в секторах
    uint64_t    descriptorOffset;   ///< Смещение дескриптора в секторах
    uint64_t    descriptorSize;     ///< Размер дескриптора в секторах
    uint32_t    numGTEsPerGT;       ///< Количество записей в таблице зерен
    uint64_t    rgdOffset;          ///< Смещение резервной таблицы зерен
    uint64_t    gdOffset;           ///< Смещение основной таблицы зерен
    uint64_t    overHead;           ///< Размер метаданных
    bool        uncleanShutdown;    ///< Флаг завершения с ошибками
    char        singleEndLineChar;  ///< Символ конца строки
    char        nonEndLineChar;     ///< Символ, не являющийся концом строки
    char        doubleEndLineChar1; ///< Первый символ двойного конца строки
    char        doubleEndLineChar2; ///< Второй символ двойного конца строки
    uint16_t    compressAlgorithm;  ///< Алгоритм сжатия
    uint8_t     pad[433];           ///< Заполнение до 512 байт для выравнивания
} SparseExtentHeader;
#pragma pack()


bool SparseVMDK::CreateSparse(unsigned long bufSize, uint64_t capacitySectors){
    if (capacitySectors == 0) {
        std::wcout << L"Не удалось определить количество секторов." << std::endl;
        return false;
    }

    // Расчет геометрии диска
    uint64_t cylinders = (capacitySectors / (HEADS * SECTORS));
    // Случайная генерация cid
    uint32_t cid = generateRandomCID();

    #ifdef _WIN32
    std::wstring outFile = outFileDir + L"\\"  + outFileName + L".vmdk";
    std::wstring Filename = outFileName + L".vmdk";
    #endif // _WIN32

    #ifdef __linux__
    std::string outFile = outFileDir + "//"  + outFileName + ".vmdk";
    std::string Filename = outFileName + ".vmdk";
    #endif // __linux__

    // Записываем заголовок VMDK
    SparseExtentHeader header;
    memset(&header, 0, sizeof(header));
    header.magicNumber = VMDK_MAGICNUMBER;  // Устанавливаем магическое число
    header.version = VMDK_VERSION;          // Версия VMDK
    header.flags = 3;                       // Устанавливаем флаги
    header.capacity = capacitySectors;      // Ёмкость в секторах
    header.grainSize = GRAIN_SIZE;          // Размер зерна
    header.descriptorOffset = 1;            // Смещение дескриптора сразу после заголовка
    header.descriptorSize = DESCRIPTOR_SIZE; // Размер дескриптора в секторах
    header.numGTEsPerGT = 512;              // Количество записей на таблицу зерен
    header.rgdOffset = 0;                   // Смещение резервной таблицы зерен
    header.gdOffset = 2;                    // Смещение основной таблицы зерен
    header.overHead = VMDK_HEADER_SIZE + SECTOR_SIZE * DESCRIPTOR_SIZE; // Метаданные
    header.uncleanShutdown = false;         // Флаг чистого завершения
    header.singleEndLineChar = '\n';        // Символ конца строки
    header.nonEndLineChar = ' ';            // Символ без конца строки
    header.doubleEndLineChar1 = '\r';       // Первый символ двойного конца строки
    header.doubleEndLineChar2 = '\n';       // Второй символ двойного конца строки
    header.compressAlgorithm = 0;           // Без сжатия

    #ifdef _WIN32
    std::string outFileS = WCharToString(Filename);
    #endif // _WIN32

    #ifdef __linux__
    std::string outFileS = Filename;
    #endif // __linux__

    //Создаем дескриптор
    std::stringstream desc1;
    desc1 << "# Disk DescriptorFile\n"
                << "version=1\n"
                //<< "encoding=\"UTF-8\"\n"
                << "CID=" << cid << "\n"
                << "parentCID=ffffffff\n"
                //<< "isNativeSnapshot=\"no\"\n"
                << "createType=\"monolithicSparse\"\n"
                << "\n"
                << "# Extent description\n"
                << "RW " << capacitySectors << " SPARSE \"" << outFileS <<  "\" 0\n"
                << "\n"
                << "# The Disk Data Base\n"
                << "#DDB\n"
                << "ddb.adapterType = \"ide\"\n"
                << "ddb.geometry.cylinders = \"" << cylinders << "\"\n"
                << "ddb.geometry.heads = \"16\"\n"
                << "ddb.geometry.sectors = \"63\"\n"
                << "ddb.virtualHWVersion = \"10\"\n";

    std::string descriptor = desc1.str();


    uint64_t sizeGD = capacitySectors;

    uint64_t totalGrains = (capacitySectors%grainSize)==0 ? (capacitySectors/grainSize) : (capacitySectors/grainSize)+1;     // Кол-во зерен(grains)
    uint64_t numGT = (totalGrains%GTE_COUNT)==0 ? (totalGrains/GTE_COUNT) : (totalGrains/GTE_COUNT)+1;               // Кол-во GT
    uint64_t totalGTEs = totalGrains;                             // Общее кол-во GTE
    uint64_t totalGDEs = numGT;                                   // код-во одинаково

    // Получение смещения из заголовка
    uint64_t gdOffset = header.gdOffset * 512;

    // Смещение для 1-й GT
    uint64_t gtOffset = gdOffset + numGT*4;
    // Выравнивание, чтобы было кратно 512
    if(gtOffset%512 != 0)
    {
        gtOffset += 512-(gtOffset%512);
    }

    // Смещение для данных = Заголовок + Дескриптор + GD + GTs + (выравнивание до числа кратного 128 секторам)
    uint32_t dataOffset = gtOffset + totalGrains*4;
    // Выравнивание
    if(dataOffset%65536 != 0)
    {
        dataOffset += 65536-(dataOffset%65536);
    }

    Writer writer;


    if(writer.OpenFile(outFile.data()))
    {
        #ifdef _WIN32
        std::wcout << L"Будет сделана копия типа 'Sparse' из \"" << disk << L"\" в  \"" << outFile << L"\"" << std::endl;

        bool wresHeader = writer.Write((unsigned char*)&header, sizeof(header));
        bool wresDesc = writer.Write((unsigned char*)(descriptor.c_str()), descriptor.length());

        //Выравниваем до 1 Кб
        unsigned char* bufPadding = new unsigned char[512 - descriptor.length()]{};
        #endif // _WIN32

        #ifdef __linux__
        std::cout << "Будет сделана копия типа 'Sparse' из \"" << disk << "\" в  \"" << outFile << "" << std::endl;

        bool wresHeader = writer.Write((char*)&header, sizeof(header));
        bool wresDesc = writer.Write((char*)(descriptor.c_str()), descriptor.length());

        //Выравниваем до 1 Кб
        char* bufPadding = new char[512 - descriptor.length()]{};
        #endif // __linux__

        bool wresPadding = writer.Write(bufPadding, 512 - descriptor.length());

        // bool wresSet = writer.SetFilePointer(1024); // утсановка позиции для записи
        // std::string test = " HERE "; проверка
        // bool wresTest = writer.Write((unsigned char*)(test.c_str()), test.length());

        if(!wresHeader || !wresDesc)
        {
            std::cout << "Header write error" << std::endl;
            delete[] bufPadding; // Освобождение памяти
            return false;
        }
        else
        {
            //1.Сначала запишем все GDE
            //  Заполним массив значаний GDE
            uint32_t curGDEvalue = gtOffset/512;   // Значение первой GDE
            uint32_t GDEs[numGT]={};               // Массив значений GDE
            for(size_t i=0; i!= numGT; i++)
            {
                GDEs[i] = curGDEvalue; //Записываем в массив значение
                curGDEvalue += 4;      //Следующая GT начинается через 4 сектора
            }

            //   Записываем GDE в файл
            bool wresSetGD = writer.SetFilePointer(gdOffset); // установка позиции для записи

            if(wresSetGD)
            {
                #ifdef _WIN32
                bool wresGD = writer.Write((unsigned char*)(&GDEs), numGT*4);
                #endif // _WIN32

                #ifdef __linux__
                bool wresGD = writer.Write((char*)(&GDEs), numGT*4);
                #endif // __linux__

                if(!wresGD)
                {
                    std::cout << "GD write error" << std::endl;
                    delete[] bufPadding; // Освобождение памяти
                    return false;
                }
                else
                {
                    // std::cout << "GD is write" << std::endl;
                }
            }

            //2.Заполнение области с данными
            //Одновременно с заполнением данных будет заполняться массив GTE
            uint32_t curGTEvalue = dataOffset/512;  //Первое значение GTE
            uint32_t GTEs[totalGrains] = {};          //Массив значений GTE

            unsigned char zeros[65536]={};          // Нулевой буфер для сравнения

            //   Установим указатель на начало области с данными
            bool wresSetData = writer.SetFilePointer(dataOffset);
            if(!wresSetData) {
                    std::cout << "Set Data err\n";
                    delete[] bufPadding; // Освобождение памяти
                    return false;
                    }

            //   Дальше идет чтение данных с диска
            //   и запись в файл, если зерно не равно нулю
            Reader reader;

            if(!(reader.OpenDisk(disk.data())))
            {
                std::cout << "Open disk error" << std::endl;
                delete[] bufPadding; // Освобождение памяти
                return false;
            }
            else
            {
                #ifdef _WIN32
                unsigned char* readBuffer = new unsigned char[BUFFER_SIZE];     // Буфер для чтения
                #endif // _WIN32

                #ifdef __linux__
                char* readBuffer = new char[BUFFER_SIZE];     // Буфер для чтения
                #endif // __linux__

                for(size_t i=0; i != totalGrains; i++)
                {
                    bool rres = reader.Read(readBuffer, BUFFER_SIZE);           // Чтение данных в буфер

                    if(i == totalGrains-1) {
                        std::cout << "gggg";
                    }

                    if(!rres)
                    {
                        std::cout << "READ error" << std::endl;
                        delete[] readBuffer; // Освобождение памяти
                        delete[] bufPadding;
                        return false;
                    }
                    else
                    {
                        int zeroMemory = memcmp(readBuffer, zeros, BUFFER_SIZE);    // Сравниваем нулевой и считанный буфер
                        if(zeroMemory) //Если не нули
                        {
                            //Записываем данные
                            if(!(writer.Write(readBuffer, BUFFER_SIZE)))
                            {
                                std::cout << "Write data error\n";
                                delete[] readBuffer; // Освобождение памяти
                                delete[] bufPadding;
                                return false;
                            }
                            GTEs[i] = curGTEvalue;   // Записываем значение GTE в массив
                            curGTEvalue += 128;      // Следующий блок данных будет через 128 секторов
                        }
                        else   //Если нули
                        {
                            GTEs[i] = 0;   // Записываем значение GTE в массив
                        }
                    }
                }
                delete[] readBuffer;
                // std::cout << "DATA is write\n";
            }

            //3.Запись массива с GTE в файл
            bool wresSetGT = writer.SetFilePointer(gtOffset); // установка позиции для записи

            if(wresSetGT)
            {
                #ifdef _WIN32
                bool wresGT = writer.Write((unsigned char*)(&GTEs), totalGrains*4);
                #endif // _WIN32

                #ifdef __linux__
                bool wresGT = writer.Write((char*)(&GTEs), totalGrains*4);
                #endif // __linux__

                if(!wresGT)
                {
                    std::cout << "GT write error" << std::endl;
                    delete[] bufPadding; // Освобождение памяти
                    return false;
                }
                else
                {
                    // std::cout << "GT is write" << std::endl;
                }
            }
            delete[] bufPadding;

        }
        #ifdef _WIN32
        std::wcout<< L"Конец создания копии" << std::endl;
        #endif // _WIN32

        #ifdef __linux__
        std::cout<< "Конец создания копии" << std::endl;
        #endif // __linux__
        return true;

    }
    else { return false; }
}


#endif // VMDKSPARCE_H_INCLUDED
