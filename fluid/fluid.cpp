//
// Created by Kry0m on 03/10/2023.
//
#include <iostream>
#include <vector>
#include "sim/progargs.hpp"

int main(){
    std::vector<double> const valoresDobles = leerArchivo("../../files/small.fld");
    escribirArchivo("../../files/small_out.fld", valoresDobles);
}
