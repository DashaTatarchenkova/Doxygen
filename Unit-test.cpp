/**
 * @brief Тест-кейсы для различных классов, с использованием библиотеки doctest.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "ConsoleIO.h"
#include "DiskInfo.h"
#include "DiskInterface.h"
#include "RawCopy.h"
#include "VMDK.h"
#include "VMDKSparce.h"
#include "LogsReadWrite.h"

using namespace std;
/**
 * @brief Тест получения количества физических дисков.
 *
 * Проверяет, что метод **GetNumOfPhysicalDisk** возвращает неотрицательное значение.
 */
TEST_CASE("DiskInfo: GetNumOfPhysicalDisk") {
    DiskInfo diskInfo;
    int num = diskInfo.GetNumOfPhysicalDisk();
    CHECK(num >= 0);
}

/**
 * @brief Тест получения информации о диске.
 *
 * Проверяет, что метод **GetDiskInfo** возвращает true для валидного ID и заполняет
 * **diskName** и **totalSize** положительными значениями.
 */
TEST_CASE("DiskInfo: GetDiskInfo") {
    DiskInfo diskInfo;
    DiskInfoStruct info;
    CHECK(diskInfo.GetDiskInfo(0, 0, &info) == true);
    CHECK(info.diskName.length() > 0);
    CHECK(info.totalSize >= 0);
}

/**
 * @brief Тест для класса DiskInfo.
 *
 * Проверка работы с некорректным индексом диска
 */
TEST_CASE("Тест DiskInfo") {
    DiskInfo diskInfo;
    DiskInfoStruct diskInfoStruct;
    bool success = diskInfo.GetDiskInfo(-1, -1, &diskInfoStruct);
    CHECK_FALSE(success);
}

/**
 * @brief Тест открытия диска для чтения.
 *
 * Проверяет, что метод **OpenDisk** возвращает true при открытии физического диска.
 */

TEST_CASE("Reader: OpenDisk ph") {
    #ifdef _WIN32
    Reader reader;
    CHECK(reader.OpenDisk(L"\\\\.\\PhysicalDrive2") == true);
    #endif // _WIN32

    #ifdef __linux__
    Reader reader;
    CHECK(reader.OpenDisk(L"/dev/sda") == true);
    #endif // __linux__
}

/**
 * @brief Тест открытия диска для чтения.
 *
 * Проверяет, что метод **OpenDisk** возвращает true при открытии логического диска.
 */

TEST_CASE("Reader: OpenDisk log") {
    #ifdef _WIN32
    Reader reader;
    CHECK(reader.OpenDisk(L"\\\\.\\J:") == true);
    #endif // _WIN32

    #ifdef __linux__
    Reader reader;
    CHECK(reader.OpenDisk(L"/dev/sda2") == true);
    #endif // __linux__
}

/**
 * @brief Тест операции чтения данных с диска.
 *
 * Проверяет, что метод **Read** возвращает true при чтении данных размером 512 байт.
 */
TEST_CASE("Reader: Read") {
    #ifdef _WIN32
    Reader reader;
    unsigned char buffer[512];
    reader.OpenDisk(L"\\\\.\\J:");
    bool readSuccess = reader.Read(buffer, 512);
    CHECK(readSuccess == true);
    #endif // _WIN32

    #ifdef __linux__
    Reader reader;
    unsigned char buffer[512];
    reader.OpenDisk(L"/dev/sda2");
    bool readSuccess = reader.Read(buffer, 512);
    CHECK(readSuccess == true);
    #endif // __linux__
}

 /**
 * @brief Тесты для класса Reader.
 */
TEST_CASE("Тест Reader") {
    Reader reader;

    /** @brief Проверка открытия недопустимого пути к диску. */
    SUBCASE("Открытие недопустимого пути к диску") {
        bool result = reader.OpenDisk(L"/invalid/path/to/disk");
        CHECK_FALSE(result);
    }

    /** @brief Проверка операции чтения без открытия диска. */
      SUBCASE("Чтение без открытия диска") {
        unsigned char buffer[10];
        bool result = reader.Read(buffer, sizeof(buffer));
        CHECK_FALSE(result);
    }

    /** @brief Проверка чтения пустого диска или файла. */
    SUBCASE("Чтение пустого диска/файла") {
        #ifdef __linux__
        std::ofstream outFile("/tmp/empty_disk.img");
        outFile.close();
        reader.OpenDisk(L"/tmp/empty_disk.img");
        unsigned char buffer[10];
        bool result = reader.Read(buffer, sizeof(buffer));
        CHECK_FALSE(result);
        #endif // __linux__
    }
}

/**
 * @brief Тест открытия файла для записи.
 *
 * Проверяет, что метод **OpenFile** успешно открывает файл для записи.
 */
TEST_CASE("Writer: OpenFile") {
    Writer writer;
    CHECK(writer.OpenFile(L"output_test_file") == true);
}

/**
 * @brief Тест записи данных в файл.
 *
 * Проверяет, что метод **Write** записывает данные размером 512 байт в файл.
 */
TEST_CASE("Writer: Write") {
    Writer writer;
    writer.OpenFile(L"output_test_file");
    unsigned char buffer[512] = {0};
    CHECK(writer.Write(buffer, 512) == true);
}

/**
 * @brief Тест для класса Writer.
 *
 * Проверка открытия файла по некорректному пути.
 */
TEST_CASE("Тест Writer") {
    Writer writer;
    bool result = writer.OpenFile(L"/invalid/path/test_output.img");
    CHECK_FALSE(result);
}

/**
 * @brief Тест создания raw-копии.
 *
 * Проверяет, что метод **CreateRawCopy** не вызывает исключений.
 */
TEST_CASE("RawCopy: CreateRawCopy") {
    #ifdef _WIN32
    RawCopy rawCopy(L"\\\\.\\J:", L"226760159", L"output_test.dd", 4194304, 1961595);
    CHECK_NOTHROW(rawCopy.CreateRawCopy(0));
    #endif // _WIN32

    #ifdef __linux__
    RawCopy rawCopy(L"/dev/sdb1", L"07614A61715128C8", L"output_test.dd", 4194304, 1961595);
    CHECK_NOTHROW(rawCopy.CreateRawCopy(0));
    #endif // __linux__
}

/**
 * @brief Тест создания raw-копии многопоточно.
 *
 * Проверяет, что метод **CreateRawCopyThreads** не вызывает исключений.
 */
TEST_CASE("RawCopy: CreateRawCopyThreads") {
    #ifdef _WIN32
    RawCopy rawCopy(L"\\\\.\\J:", L"226760159", L"output_test_mn.dd", 4194304, 1961595);
    CHECK_NOTHROW(rawCopy.CreateRawCopyThreads(0));
    #endif // _WIN32

    #ifdef __linux__
    RawCopy rawCopy(L"/dev/sdb1", L"07614A61715128C8", L"output_test_mn.dd", 4194304, 1961595);
    CHECK_NOTHROW(rawCopy.CreateRawCopyThreads(0));
    #endif // __linux__
}

/**
 * @brief Тест создания VMDK-файла.
 *
 * Проверяет, что метод **CreateVMDK** создает файл с заданным размером буфера и количеством секторов.
 */
TEST_CASE("FlatVMDK: CreateVMDK") {
    #ifdef _WIN32
    FlatVMDK flatVMDK(L"D:\\output_dir", L"test_file", L"\\\\.\\J:", L"226760159");
    CHECK(flatVMDK.CreateVMDK(4194304, 1961595) == true);
    #endif // _WIN32

    #ifdef __linux__
    FlatVMDK flatVMDK(L"/home/yashuoki/test/", L"test_file_flat", L"/dev/sdb1", L"07614A61715128C8");
    CHECK(flatVMDK.CreateVMDK(4194304, 1961595) == true);
    #endif // __linux__
}

/**
 * @brief Тест создания Sparce VMDK-файла.
 *
 * Проверяет, что метод **CreateSparse** создает файл с заданным размером буфера и количеством секторов.
 */
TEST_CASE("Test SparseVMDK Creation") {

    DiskInfo disks;
    DiskInfoStruct disk_info;

    // Запрашиваем индекс физического диска
    int pDisk = 2;
    disks.GetDiskInfo(pDisk, 0, &disk_info);

    // Получаем индекс диска для копирования
    int lDisk = 1;
    disks.GetDiskInfo(pDisk, lDisk, &disk_info);

    const wchar_t* disk = (disk_info.diskName).data();
    const wchar_t* serialNum = (disk_info.serial_id).data();
    int64_t bufSize = 4194304;
    wstring outFileName = L"test_file_sparce";
    #ifdef _WIN32
    wstring outFileDir = L"D:\\output_dir";
    #endif // _WIN32
    #ifdef __linux__
    wstring outFileDir = L"/home/yashuoki/test/";
    #endif // __linux__

    SparseVMDK sparse(outFileDir, outFileName, disk, disk_info.serial_id, bufSize, disk_info.total_sectors);

    // Проверяем успешное создание sparse VMDK с корректными параметрами
    bool result = sparse.CreateSparse();
    CHECK(result);

    // Проверяем, что создание с нулевой емкостью не проходит
    SparseVMDK sparse2(outFileDir, outFileName, disk, disk_info.serial_id, bufSize, 0);
    CHECK_FALSE(sparse2.CreateSparse());
}

/**
 * @brief Тест создания Sparce VMDK-файла многопоточно.
 *
 * Проверяет, что метод **CreateSparseThread** создает файл с заданным размером буфера и количеством секторов.
 */
TEST_CASE("Test SparseVMDK Threads Creation") {

    DiskInfo disks;
    DiskInfoStruct disk_info;

    // Запрашиваем индекс физического диска
    int pDisk = 2;
    disks.GetDiskInfo(pDisk, 0, &disk_info);

    // Получаем индекс диска для копирования
    int lDisk = 1;
    disks.GetDiskInfo(pDisk, lDisk, &disk_info);

    const wchar_t* disk = (disk_info.diskName).data();
    const wchar_t* serialNum = (disk_info.serial_id).data();
    int64_t bufSize = 4194304;
    wstring outFileName = L"test_file_sparce_mn";
    #ifdef _WIN32
    wstring outFileDir = L"D:\\output_dir";
    #endif // _WIN32
    #ifdef __linux__
    wstring outFileDir = L"/home/yashuoki/test/";
    #endif // __linux__

    SparseVMDK sparse(outFileDir, outFileName, disk, disk_info.serial_id, bufSize, disk_info.total_sectors);

    // Проверяем успешное создание sparse VMDK с корректными параметрами
    bool result = sparse.CreateSparseThread();
    CHECK(result);
}

//Тест возобновления
// Указываем путь к лог-файлу
const string logFileName = "LogFile1";
const string logFileName2 = "LogFile2";
/**
 * @brief Тест создания log файла.
 *
 * Проверяет, что метод **CreateRawCopyLog** создает log файл, а также его содержмое.
 */
TEST_CASE("LogsReadWrite - CreateRawCopyLog") {
    LogsReadWrite<std::wstring> logManager;

    // Проверка, что лог-файл был создан успешно
    bool result = logManager.CreateRawCopyLog(L"Disk1", L"12345", L"/logs", L"log1", time(nullptr), 1024, 4096);
    CHECK(result == true);

    // Проверка, что лог-файл существует
    std::ifstream file(logFileName);
    CHECK(file.is_open());
    file.close();

    // Чтение и проверка содержимого файла
    std::wifstream logFile(logFileName); // используем wifstream для работы с широкими строками
    std::wstring line;

    if (logFile.is_open()) {
        std::getline(logFile, line);
        CHECK(line == std::to_wstring(static_cast<int>(ImageType::DD))); // преобразуем в wstring для сравнения
        std::getline(logFile, line);
        CHECK(line == L"Disk1");  // Проверяем, что диск записан корректно
        std::getline(logFile, line);
        CHECK(line == L"12345");  // Проверяем, что серийный номер записан корректно
        std::getline(logFile, line);
        CHECK(line == L"/logs");  // Проверяем, что директория записана корректно
        std::getline(logFile, line);
        CHECK(line == L"log1");  // Проверяем, что имя файла записано корректно
        std::getline(logFile, line);
        CHECK(line == std::to_wstring(time(nullptr)));  // Проверяем время окончания
        std::getline(logFile, line);
        CHECK(line == std::to_wstring(1024));  // Проверяем количество записанных секторов
        std::getline(logFile, line);
        CHECK(line == std::to_wstring(4096));  // Проверяем общее количество секторов
        logFile.close();
    }
}

/**
 * @brief Тест чтения log файла.
 *
 * Проверяет, что метод **ReadLogFiles** считывает log файл, а также данные из него.
 */
TEST_CASE("LogsReadWrite - ReadLogFiles") {
    LogsReadWrite<std::wstring> logManager;
    LogFile logFiles[10];
    int filesRead = 0;

    // Проверка, что лог-файл был прочитан
    bool result = logManager.ReadLogFiles(logFiles, &filesRead);
    CHECK(result == true);
    CHECK(filesRead == 1);

    // Проверка данных, считанных из файла
    CHECK(logFiles[0].type == (ImageType::DD));
    CHECK(logFiles[0].disk == L"Disk1");
    CHECK(logFiles[0].outFileDir == L"/logs");
    CHECK(logFiles[0].serialNum == L"12345");
    CHECK(logFiles[0].outFileName == L"log1");
    CHECK(logFiles[0].numOfSectorsWriten == 1024);
    CHECK(logFiles[0].totalSectors == 4096);
}

/**
 * @brief Тест создания log файла для VMDK_Sparse.
 *
 * Проверяет, что метод **CreateSparseLog** создает log файл, а также его содержимое.
 * Проверяет, что метод **ReadGTEs** корректно считывеат данные с массива GTEs.
 */
TEST_CASE("LogsReadWrite - CreateSparseLog") {
    LogsReadWrite<wstring> logManager;

    // Параметры для теста
    uint32_t GTEs[] = {12, 14, 24};

    // Проверка, что лог-файл был создан успешно
    bool result = logManager.CreateSparseLog(L"Disk1", L"12345", L"/logs", L"log2", time(nullptr), 32, 3, 128, 12, 14, GTEs);
    CHECK(result == true);

    // Проверка, что лог-файл существует
    ifstream file(logFileName2);
    CHECK(file.is_open());
    file.close();

    // Чтение и проверка содержимого файла
    wifstream logFile(logFileName2); // используем wifstream для работы с широкими строками
    wstring line;

    if (logFile.is_open()) {
        // Проверяем тип изображения
        getline(logFile, line);
        CHECK(line == to_wstring(static_cast<int>(ImageType::VMDK_Sparse)));

        // Проверяем остальные параметры
        getline(logFile, line);
        CHECK(line == L"Disk1");  // Имя диска
        getline(logFile, line);
        CHECK(line == L"12345");  // Серийный номер
        getline(logFile, line);
        CHECK(line == L"/logs");  // Директория
        getline(logFile, line);
        CHECK(line == L"log2");   // Имя файла
        getline(logFile, line);
        CHECK(line == to_wstring(time(nullptr)));  // Время окончания
        getline(logFile, line);
        CHECK(line == L"0");
        getline(logFile, line);
        CHECK(line == L"0");
        getline(logFile, line);
        CHECK(line == to_wstring(32));  // Количество записанных зерен
        getline(logFile, line);
        CHECK(line == to_wstring(3));   // Количество прочитанных зерен
        getline(logFile, line);
        CHECK(line == to_wstring(128)); // Общее количество зерен
        getline(logFile, line);
        CHECK(line == to_wstring(12));  // Смещение данных
        getline(logFile, line);
        CHECK(line == to_wstring(14));  // Смещение GTE

        // Проверяем содержимое массива GTEs
        for (size_t i = 0; i < 3; ++i) {
            getline(logFile, line);
            CHECK(line == to_wstring(GTEs[i]));
        }

        logFile.close();
    }
}

/**
 * @brief Тест удаления log файла.
 *
 * Проверяет, что метод **DeleteLogFile** удаляет log файл.
 */
TEST_CASE("LogsReadWrite - DeleteLogFile") {
    LogsReadWrite<std::wstring> logManager;

    // Создаем тестовый лог-файл для удаления
    std::ofstream logFile("LogFileToDelete");
    CHECK(logFile.is_open());
    logFile.close();

    bool deleteResult = logManager.DeleteLogFile("LogFileToDelete");
    CHECK(deleteResult == true);
    CHECK(!logFile.is_open());
}
