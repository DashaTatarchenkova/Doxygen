#ifndef RAWCOPY_H_INCLUDED
#define RAWCOPY_H_INCLUDED

#include "DiskInterface.h"
#include "LogsReadWrite.h"

/**
 * @class RawCopy
 * @brief Класс для создания посекторной копии диска.
 *
 * Класс предоставляет методы для создания RAW-копии диска, получения времени создания и других связанных операций.
 */
class RawCopy
{
private:
    std::wstring disk;             ///< Имя диска, с которого создаётся копия.
    std::wstring serialNumber;     ///< Серийный номер диска.
    std::wstring outFile;          ///< Имя выходного файла для RAW-копии.
    time_t startTime;              ///< Время начала операции.
    time_t endTime;                ///< Время завершения операции.

    unsigned long bufSize;         ///< Размер буфера для чтения.
    unsigned long totalSectors;    ///< Общее количество секторов на диске.

    /**
     * @brief Получает текущее системное время.
     * @return Текущее время в формате `time_t`.
     */
    time_t timeNow();

public:
    /**
     * @brief Конструктор для инициализации параметров копирования.
     *
     * @param d Имя диска.
     * @param sN Серийный номер диска.
     * @param oF Имя выходного файла.
     * @param bS Размер буфера.
     * @param tS Общее количество секторов.
     */
    RawCopy(std::wstring d, std::wstring sN, std::wstring oF, unsigned long bS, unsigned long tS)
        : disk{d}, serialNumber{sN}, outFile{oF}, bufSize{bS}, totalSectors{tS} {};

    /**
     * @brief Создаёт RAW-копию диска.
     *
     * @param SectorsWritten Количество секторов, которые уже были записаны (в случае возобновления копирования).
     * @return true, если копирование успешно завершено.
     * @return false, если возникла ошибка.
     */
    bool CreateRawCopy(unsigned long long SectorsWritten);

    /**
     * @brief Возвращает время, затраченное на создание RAW-копии.
     *
     * @return Продолжительность операции в секундах.
     */
    time_t GetCreationTime() { return endTime - startTime; };

    /**
     * @brief Возвращает время начала или окончания операции.
     *
     * @param endOrStart Если true, возвращается время начала. Если false — время окончания.
     * @return Время в формате `time_t`.
     */
    time_t GetEndStartTime(bool endOrStart) { return endOrStart ? startTime : endTime; };

    /**
     * @brief Возвращает время создания копии в строковом формате.
     *
     * @return Время создания копии в виде строки.
     */
    std::wstring wcsCreationTime();

    /**
     * @brief Конвертирует время в строковый формат.
     *
     * @param timeToWcs Время в формате `time_t` для преобразования.
     * @return Время в виде строки.
     */
    std::wstring tToWcs(time_t timeToWcs);
};

#endif // RAWCOPY_H_INCLUDED
