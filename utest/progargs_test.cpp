#include "gtest/gtest.h"
#include "sim/grid.hpp"
#include "sim/progargs.hpp"
#include <fstream>

// Todos los argumentos de entrada siguen el formato correcto
TEST(ConstructorTest, CorrectoTest) {
  std::vector<const char *> const args = {"10", "../../files/small.fld", "output.fld"};
  auto procesador = ProgArgs(3, args);
  ASSERT_EQ(procesador.getArchivoEntrada(), "../../files/small.fld");
  ASSERT_EQ(procesador.getArchivoSalida(), "output.fld");
}

// Se pasan 4 argumentos en vez de 3
TEST(ConstructorTest, NumArgsIncorrectoTest) {
  std::vector<const char *> const args = {"10", "input.txt", "output.txt", "sobrante"};
  EXPECT_EXIT(ProgArgs const progArgs(4, args);, ::testing::ExitedWithCode(-1 + 256), ".*");
}

// No se pasa un número como pasos de tiempo
TEST(ConstructorTest, TimeStepsIncorrecto1Test) {
  std::vector<const char *> const args = {"a", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs const progArgs(3, args);, ::testing::ExitedWithCode(-1 + 256), ".*");
}

// Se pasa un número negativo como pasos de tiempo
TEST(ConstructorTest, TimeStepsIncorrecto2Test) {
  std::vector<const char *> const args = {"-5", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs const progArgs(3, args);, ::testing::ExitedWithCode(-2 + 256), ".*");
}

// ------------------------------------------------------

// El archivo se lee correctamente y tiene el contenido adecuado
TEST(LeerArchivoTest, CorrectoTest) {
  std::vector<const char *> const args = {"10", "../../files/small.fld", "output.fld"};
  auto procesador = ProgArgs(3, args);
  std::vector<double> const valoresDoble = procesador.leerArchivo();
  ASSERT_EQ(typeid(valoresDoble), typeid(std::vector<double>));
}

// El archivo de entrada no existe y no se puede abrir
TEST(LeerArchivoTest, ArchivoEntradaIncorrectoTest) {
  std::vector<const char *> const args = {"10", "../../files/small.txt", "output.fld"};
  auto procesador = ProgArgs(3, args);
  EXPECT_EXIT(procesador.leerArchivo(), ::testing::ExitedWithCode(-3 + 256), ".*");
}

// El header del archivo de entrada indica un número de partículas negativo (-5)
TEST(LeerArchivoTest, NumParticlesIncorrecto1Test) {
  std::vector<const char *> const args = {"10", "../../files/small_-5.fld", "output.fld"};
  auto procesador = ProgArgs(3, args);
  EXPECT_EXIT(procesador.leerArchivo(), ::testing::ExitedWithCode(-5 + 256), ".*");
}

// El header del archivo de entrada indica que hay 4800 partículas, pero hay 4799 realmente
TEST(LeerArchivoTest, NumParticlesIncorrecto2Test) {
  std::vector<const char *> const args = {"10", "../../files/small_incomplete.fld", "output.fld"};
  auto procesador = ProgArgs(3, args);
  EXPECT_EXIT(procesador.leerArchivo(), ::testing::ExitedWithCode(-5 + 256), ".*");
}

// ------------------------------------------------------

// El archivo de salida se crea correctamente
TEST(EscribirArchivoTest, CorrectoTest) {
  std::vector<const char *> const args = {"10", "../../files/small.fld", "../../files/small_out.fld"};
  auto procesador = ProgArgs(3, args);
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  //Creación de las partículas
  std::vector<Particle> Particulas;
  for (size_t i = 0; i < valoresDobles.size(); i+=nueve){
    Particle const nuevaParticula = Particle(std::vector<double>{0.0, 0.0, 0.0}, std::vector<double>{valoresDobles[i], valoresDobles[i+1], valoresDobles[i+2]},
                                             0.0, std::vector<double> {valoresDobles[i+3], valoresDobles[i+4], valoresDobles[i+cinco]},
                                             std::vector<double> {valoresDobles[i+seis], valoresDobles[i+siete], valoresDobles[i+ocho]});
    Particulas.push_back(nuevaParticula);
  }
  // Nombre del archivo de salida que se espera crear
  const std::string archivoSalida = procesador.getArchivoSalida();

  // Borra el archivo si ya existe
  int const borrado = std::remove(archivoSalida.c_str());
  if (borrado == 1) {
    std::cout << "Se ha borrado el archivo y existente.\n";
  }

  // Llamar a la función que debería crear el archivo
  procesador.escribirArchivo(Particulas);

  // Verificar si el archivo se ha creado correctamente
  std::ifstream archivo(archivoSalida, std::ios::in | std::ios::binary);
  EXPECT_TRUE(archivo.is_open());
  archivo.close();
}
