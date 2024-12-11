/**
 * @file ConsoleIO.h
 * @brief Заголовочный файл для работы с выводом информации в консоль, а также вводом информации пользователем.
 */

#ifndef CONSOLEIO_H_INCLUDED
#define CONSOLEIO_H_INCLUDED

#include <string>
#include <cstdint>

/**
 * @brief Перечисление типов образов.
 */
enum class ImageType : int
{
    DD = 1,   /**< Тип образа DD. */
    VMDK = 2, /**< Тип образа VMDK. */
};

/**
 * @brief Преобразует тип образа в строку (wstring).
 * 
 * @param t Тип образа (ImageType).
 * @return std::wstring Строковое представление типа образа.
 */
std::wstring ImageTypeToWstring(ImageType t);

/**
 * @brief Преобразует тип образа в строку (string).
 * 
 * @param t Тип образа (ImageType).
 * @return std::string Строковое представление типа образа.
 */
std::string ImageTypeToString(ImageType t);

/**
 * @brief Печатает в консоль все физические и логические диски.
 * 
 * Каждому диску также присваивается уникальный ID.
 */
void DisplayDisks();

/**
 * @brief Запрашивает ID физического диска.
 * 
 * @param max_size Максимальное количество физических дисков.
 * @return int ID выбранного физического диска.
 */
int AskPhysDiskNum(int max_size);

/**
 * @brief Запрашивает ID логического диска.
 * 
 * @param max_size Максимальное количество логических дисков.
 * @return int ID выбранного логического диска.
 */
int AskLogDiskNum(int max_size);

#ifdef _WIN32
/**
 * @brief Запрашивает путь до каталога для хранения файла.
 * 
 * @return std::wstring Путь до каталога.
 */
std::wstring AskDirPath();

/**
 * @brief Запрашивает название файла для сохранения.
 * 
 * @return std::wstring Название выходного файла.
 */
std::wstring AskOutFileName();
#endif // _WIN32

#ifdef __linux__
/**
 * @brief Запрашивает путь до каталога для хранения файла (Linux).
 * 
 * @return std::string Путь до каталога.
 */
std::string AskDirPath();

/**
 * @brief Запрашивает название файла для сохранения (Linux).
 * 
 * @return std::string Название выходного файла.
 */
std::string AskOutFileName();
#endif // __linux__

/**
 * @brief Запрашивает тип создаваемого образа (DD или VMDK).
 * 
 * @return ImageType Выбранный тип образа.
 */
ImageType AskImageType();

/**
 * @brief Запрашивает размер буфера для чтения и записи.
 * 
 * Размер буфера должен быть кратен 512. Размер по умолчанию равен 4096 байт.
 * 
 * @return uint64_t Размер буфера.
 */
uint64_t AskBuffSize();

#endif // CONSOLEIO_H_INCLUDED
