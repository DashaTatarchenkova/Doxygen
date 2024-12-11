/**
 * @file VMDK.h
 * @brief Заголовочный файл для создания файла формата .vmdk monolithicFlat.
 */

#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <codecvt>
#include "RawCopy.h"


#define SECTOR_SIZE 512               /// Размер сектора в байтах
#define HEADS 255                     /// Количество головок
#define SECTORS 63                    /// Количество секторов

/**
 * @class FlatVMDK
 * @brief Класс для работы c созданием vmdk.
 *
 * Этот класс предоставляет методы для создания vmdk, смена широких строк и ГСЧ для CID.
 */

class FlatVMDK {
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
     FlatVMDK(std::wstring outDir, std::wstring outName, std::wstring d) {
        srand(static_cast<unsigned int>(time(nullptr)));
        outFileDir = outDir;
        outFileName = outName;
        disk = d;
    }
    #endif // _WIN32

    #ifdef __linux__
    FlatVMDK(std::string outDir, std::string outName, std::string d) {
        srand(static_cast<unsigned int>(time(nullptr)));
        outFileDir = outDir;
        outFileName = outName;
        disk = d;
    }
    #endif // __linux__
    /**
     * @brief Создание VMDK файла и дескриптора.
     *
     * @param[in] bufSize Буфер обмена данных.
     * @param[in] capacitySectors общее кол-во секторов в выбранном пользователем диске.
     *
     * @return Возврашает true при успешном создании образа, false при неудачном создании
     */
    //Метод для создания flat файла и дескриптора
    bool CreateVMDK(unsigned long bufSize, uint64_t capacitySectors);

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

//Основной метод, который объединяет копирование данных и создание дескриптора
bool FlatVMDK::CreateVMDK(unsigned long bufSize, uint64_t capacitySectors) {


    if (capacitySectors == 0) {
        std::wcout << L"Не удалось определить количество секторов." << std::endl;
        return false;
    }

    // Расчет геометрии диска
    uint64_t cylinders = (capacitySectors / (HEADS * SECTORS));
    // Случайная генерация cid
    uint32_t cid = generateRandomCID();

    //Создание дескриптора VMDK

    #ifdef _WIN32
    std::wstring outFile = outFileDir + L"\\"  + outFileName + L".vmdk";
    std::wofstream descriptorFile(outFile.data(), std::ios::binary);
    std::wstring filenameFlat = outFileName + L"-flat.vmdk";



    descriptorFile << L"# Disk DescriptorFile\n"
                   << L"version=1\n"
                   << L"encoding=\"UTF-8\"\n"
                   << L"CID=" << cid << "\n"
                   << L"parentCID=ffffffff\n"
                   << L"isNativeSnapshot=\"no\"\n"
                   << L"createType=\"monolithicFlat\"\n"
                   << L"\n"
                   << L"# Extent description\n"
                   << L"RW " << capacitySectors << L" FLAT \"" << filenameFlat <<  L"\" 0\n"
                   << L"\n"
                   << L"# The Disk Data Base\n"
                   << L"#DDB\n"
                   << L"ddb.adapterType = \"lsilogic\"\n"
                   << L"ddb.geometry.cylinders = \"" << cylinders << L"\"\n"
                   << L"ddb.geometry.heads = \"255\"\n"
                   << L"ddb.geometry.sectors = \"63\"\n"
                   << L"ddb.virtualHWVersion = \"10\"\n";

    //Путь до побайтовой копии
    outFile = outFileDir + L"\\"  + outFileName + L"-flat.vmdk";
    #endif // _WIN32

    #ifdef __linux__
    std::string outFile = outFileDir + "/"  + outFileName + ".vmdk";
    std::ofstream descriptorFile(outFile.data(), std::ios::binary);
    std::string filenameFlat = outFileName + "-flat.vmdk";



    descriptorFile << "# Disk DescriptorFile\n"
                   << "version=1\n"
                   << "encoding=\"UTF-8\"\n"
                   << "CID=" << cid << "\n"
                   << "parentCID=ffffffff\n"
                   << "isNativeSnapshot=\"no\"\n"
                   << "createType=\"monolithicFlat\"\n"
                   << "\n"
                   << "# Extent description\n"
                   << "RW " << capacitySectors << " FLAT \"" << filenameFlat <<  "\" 0\n"
                   << "\n"
                   << "# The Disk Data Base\n"
                   << "#DDB\n"
                   << "ddb.adapterType = \"lsilogic\"\n"
                   << "ddb.geometry.cylinders = \"" << cylinders << "\"\n"
                   << "ddb.geometry.heads = \"255\"\n"
                   << "ddb.geometry.sectors = \"63\"\n"
                   << "ddb.virtualHWVersion = \"10\"\n";

    //Путь до побайтовой копии
    outFile = outFileDir + "/"  + outFileName + "-flat.vmdk";
    #endif // __linux__

    descriptorFile.close();


    // Копирование данных диска с помощью CreateRawCopy
    RawCopy RC(disk, outFile, bufSize);
    RC.CreateRawCopy();
    return true;
}


#endif // HEAD_H_INCLUDED
