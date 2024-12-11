#ifndef LOGSREADWRITE_H_INCLUDED
#define LOGSREADWRITE_H_INCLUDED

#include "ConsoleIO.h"
#include <cstdint>
#include <string>

#pragma pack(push, 1)

/**
 * @struct LogFile
 * @brief Структура для хранения информации о лог-файле.
 *
 * Содержит данные о типе, диске, серийном номере, пути
 * к файлу, времени завершения и статистике записи для различных форматов.
 */
typedef struct
{
    ImageType type;               ///< Тип  (DD, Sparse, и т.д.).
    std::wstring disk;            ///< Имя диска.
    std::wstring serialNum;       ///< Серийный номер диска.
    std::wstring outFileDir;      ///< Директория для выходного файла.
    std::wstring outFileName;     ///< Имя выходного файла.
    time_t endTime;               ///< Время завершения операции.

    // Поля для RawCopy
    uint64_t numOfSectorsWriten;  ///< Количество записанных секторов.
    uint64_t totalSectors;        ///< Общее количество секторов.

    // Поля для VMDK Sparse
    uint64_t numOfGrainWriten;    ///< Количество записанных зерен (grains).
    uint64_t numOfGrainRead;      ///< Количество прочитанных зерен (grains).
    uint64_t totalGrains;         ///< Общее количество зерен (grains).
    uint64_t dataOffset;          ///< Смещение данных.
    uint64_t gtOffset;            ///< Смещение таблицы зерен (grain table).
} LogFile;

#pragma pack(pop)

/**
 * @class LogsReadWrite
 * @brief Класс для работы с лог-файлами.
 *
 * Класс предоставляет методы для чтения, записи и удаления лог-файлов, а также
 * для создания логов в различных форматах (DD и Sparse).
 *
 * @tparam T Тип данных, используемый для строк.
 */
template<typename T>
class LogsReadWrite
{
private:
    uint64_t maxLogFileId = 0; ///< Максимальный идентификатор лог-файла, найденного в директории.

public:

    LogsReadWrite() {};

    /**
     * @brief Читает лог-файлы из текущей директории.
     * @param logFiles Указатель на массив структур LogFile для хранения данных.
     * @param filesRead Указатель на количество прочитанных лог-файлов.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool ReadLogFiles(LogFile* logFiles, int* filesRead);

    /**
     * @brief Считывает таблицу зерен (GTE) в массив.
     * @param GTEs Указатель на массив для записи GTE.
     * @param numOfGrainRead Количество зерен, которые нужно прочитать.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool ReadGTEs(uint32_t* GTEs, uint64_t numOfGrainRead);

    /**
     * @brief Удаляет лог-файл.
     * @param lF Имя лог-файла для удаления.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool DeleteLogFile(std::string lF);

    /**
     * @brief Создаёт лог-файл для посекторного копирования.
     * @param disk Имя диска.
     * @param serialNum Серийный номер диска.
     * @param outFileDir Директория для выходного файла.
     * @param outFileName Имя выходного файла.
     * @param endTime Время завершения операции.
     * @param numOfSectorsWriten Количество записанных секторов.
     * @param totalSectors Общее количество секторов.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool CreateRawCopyLog(T disk,
                          T serialNum,
                          T outFileDir,
                          T outFileName,
                          time_t endTime,
                          uint64_t numOfSectorsWriten,
                          uint64_t totalSectors);

    /**
     * @brief Создаёт лог-файл для Sparse-образа.
     * @param disk Имя диска.
     * @param serialNum Серийный номер диска.
     * @param outFileDir Директория для выходного файла.
     * @param outFileName Имя выходного файла.
     * @param endTime Время завершения операции.
     * @param numOfGrainWriten Количество записанных зерен.
     * @param numOfGrainRead Количество прочитанных зерен.
     * @param totalGrains Общее количество зерен.
     * @param dataOffset Смещение данных.
     * @param gtOffset Смещение таблицы зерен.
     * @param GTEs Массив GTE для записи.
     * @return true, если операция успешна.
     * @return false, если возникла ошибка.
     */
    bool CreateSparseLog(T disk,
                         T serialNum,
                         T outFileDir,
                         T outFileName,
                         time_t endTime,
                         uint64_t numOfGrainWriten,
                         uint64_t numOfGrainRead,
                         uint64_t totalGrains,
                         uint64_t dataOffset,
                         uint64_t gtOffset,
                         uint32_t GTEs[]);
};

#endif // LOGSREADWRITE_H_INCLUDED
