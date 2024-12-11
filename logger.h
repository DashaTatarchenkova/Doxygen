#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>

using namespace std::chrono;

/**
 * @brief Записывает сообщение в лог-файл.
 *
 * Функция записывает сообщение в файл.
 * Сообщение содержит уровень лога, временную метку и текст сообщения.
 * Если файл не удаётся открыть, выводится сообщение об ошибке через `perror`.
 *
 * @param level Уровень логирования (например, "INFO", "ERROR", "DEBUG").
 * @param message Текст сообщения для записи в лог.
 */
void log_message(const char* level, const char* message) {
    // Открыть файл для добавления
    FILE* log_file = fopen("grebaniy_log_dlya_grebanogo_bugfix.txt", "a");
    if (log_file == NULL) {
        perror("Ошибка открытия файла лога");
        return;
    }

    // Получить текущее время
    time_point<system_clock> time_now = system_clock::now();

    time_t rawtime = system_clock::to_time_t(time_now);
    tm timeinfo = *localtime(&rawtime);

    // Форматировать временную метку
    char buf[80];
    strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", &timeinfo);

    // Записать сообщение в лог-файл
    fprintf(log_file, "%s|-|%s|-|_ %s _|\n", level, buf, message);
    fclose(log_file);
}

#endif // LOGGER_H_INCLUDED
