//
// Created by Kry0m on 03/10/2023.
//
#include <iostream>
#include <vector>
#include <span>
#include "sim/progargs.hpp"

int main(int argc, char* argv[]) {
    std::span const args_view{argv, static_cast<size_t>((argc))};
    procesarArgumentos(argc, args_view);
}
