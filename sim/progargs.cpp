//
// Created by Kry0m on 03/10/2023.
//
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <array>
#include <span>
#include "progargs.hpp"

std::vector<char *> procesarArgumentos(int argc, std::span<char*> args_view) {
    std::vector const args(args_view.begin() + 1, args_view.end());
    std::cout << argc << '\n';
    for (const auto& arg : args) {
        std::cout << arg << '\n';
    }
    std::vector<double> const valoresDobles = leerArchivo(args[1]);
    escribirArchivo(args[2], valoresDobles);
    return args;
}

std::vector<double> leerArchivo(const std::string& nombre_archivo) {
    std::ifstream archivo(nombre_archivo, std::ios::binary);
    std::vector <double> valoresDoble;
    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo de entrada.\n";
    }

    std::vector<char> buffer(sizeof(float));
    while (archivo.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
        float data_simple = 0;
        std::memcpy(&data_simple, buffer.data(), buffer.size());
        double const data_double = data_simple;
        valoresDoble.push_back(data_double);
    }
    archivo.close();
    return valoresDoble;
}

int escribirArchivo(const std::string& nombre_archivo, std::vector<double> valoresDoble) {
    std::vector<float> const valoresSimple(valoresDoble.begin(), valoresDoble.end());
    std::ofstream archivo_out(nombre_archivo, std::ios::binary);
    if (!archivo_out) {
        std::cerr << "No se pudo abrir el archivo de salida.\n";
        return 1;
    }

    for (const auto& valor : valoresDoble) {
        auto valor_simple = static_cast<float>(valor);
        std::array<char, sizeof(float)> buffer{};
        std::memcpy(buffer.data(), &valor_simple, sizeof(float));
        archivo_out.write(buffer.data(), buffer.size());
    }
    archivo_out.close();

    std::cout << "Proceso completado. El archivo de salida se ha creado correctamente.\n";
    return 0;
}