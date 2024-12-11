/**
 * @file DiskInfo.h
 * @brief Заголовочный файл для работы с информацией о дисках.
 */

#ifndef DISKINFO_H
#define DISKINFO_H

#include <windows.h>
#include <vector>
#include <map>
#include <iostream>
#include <cstdint>
#include <string>

using namespace std;

/**
 * @brief Перечисление типов дисков.
 */
enum class DiskType : int
{
    Physical = 0, ///< Физический диск
    Logical,      ///< Логический диск
};

/**
 * @struct DiskInfoStruct
 * @brief Структура для хранения информации о диске.
 */
typedef struct
{
    DiskType type;              ///< Тип диска (логический или физический)
    wstring diskName;           ///< Имя диска. Пример для Windows: "\\.\PhysicalDrive0" или "\\.\C:". Пример для Linux: "/dev/sdb" или "dev/sdb1"
    wstring fileSystemType;     ///< Тип файловой системы
    int16_t numOfLogicalDisk;   ///< Число логических дисков. Эта часть задается только если тип диска физический,если логический, то -1.                
    float totalSize;            ///< Общий объем (в Гб)
    int64_t total_sectors;      ///< Общее количество секторов
    int64_t total_bytes;        ///< Общий объем в байтах
    float freeSpace;            ///< Свободный объем (в Гб)
} DiskInfoStruct;

/// Указатель на структуру DiskInfoStruct.
typedef DiskInfoStruct* PDiskInfoStruct;

/**
 * @class DiskInfo
 * @brief Класс для работы с информацией о дисках.
 *
 * Этот класс предоставляет методы для получения информации о физических и логических дисках.
 */
class DiskInfo
{
private:
    int physical_disk_amount;
    map<wstring, vector<wstring>> connected_drives;

public:
    /**
     * @brief Конструктор класса DiskInfo.
     */
    DiskInfo();

    /**
     * @brief Вычисление количества физических дисков.
     *
     * @return Количество физических дисков.
     */
    int GetNumOfPhysicalDisk() const { return connected_drives.size(); };

    /**
     * @brief Получение информации о диске.
     *
     * Заполняет структуру DiskInfoStruct данными о диске. Возвращает false, если произошли ошибки.
     *
     * @param[in] IdPhDisk Индекс физического диска.
     * @param[in] IdLogicDisk Индекс логического диска.
     * @param[out] diskInfo Структура, содержащая информацию о диске.
     * @return True, если информация была получена успешно, иначе False.
     */
    bool GetDiskInfo(int IdPhDisk, int IdLogicDisk, PDiskInfoStruct diskInfo);

    /**
     * @brief Отображение информации обо всех дисках.
     *
     * Выводит информацию о всех обнаруженных дисках.
     */
    void DisplayAllDrives();
};

#endif // DISKINFO_H
