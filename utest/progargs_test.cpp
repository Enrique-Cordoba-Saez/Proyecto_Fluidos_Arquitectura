#include "sim/grid.hpp"
#include "sim/progargs.hpp"

#include "gtest/gtest.h"
#include <fstream>

// Todos los argumentos de entrada siguen el formato correcto
TEST(ConstructorTest, CorrectoTest) {
  std::vector<char const *> const args = {"10", "../../files/small.fld", "output.fld"};
  auto procesador                      = ProgArgs(3, args);
  ASSERT_EQ(procesador.getArchivoEntrada(), "../../files/small.fld");
  ASSERT_EQ(procesador.getArchivoSalida(), "output.fld");
}

// Se pasan 4 argumentos en vez de 3
TEST(ConstructorTest, NumArgsIncorrectoTest) {
  std::vector<char const *> const args = {"10", "input.txt", "output.txt", "sobrante"};
  EXPECT_EXIT(ProgArgs const progArgs(4, args);, ::testing::ExitedWithCode(-1 + 256), ".*");
}

// No se pasa un número como pasos de tiempo
TEST(ConstructorTest, TimeStepsIncorrecto1Test) {
  std::vector<char const *> const args = {"a", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs const progArgs(3, args);, ::testing::ExitedWithCode(-1 + 256), ".*");
}

// Se pasa un número negativo como pasos de tiempo
TEST(ConstructorTest, TimeStepsIncorrecto2Test) {
  std::vector<char const *> const args = {"-5", "input.txt", "output.txt"};
  EXPECT_EXIT(ProgArgs const progArgs(3, args);, ::testing::ExitedWithCode(-2 + 256), ".*");
}

// ------------------------------------------------------

// El archivo se lee correctamente y tiene el contenido adecuado
TEST(LeerArchivoTest, CorrectoTest) {
  std::vector<char const *> const args   = {"10", "../../files/small.fld", "output.fld"};
  auto procesador                        = ProgArgs(3, args);
  std::vector<double> const valoresDoble = procesador.leerArchivo();
  ASSERT_EQ(typeid(valoresDoble), typeid(std::vector<double>));
}

// El archivo de entrada no existe y no se puede abrir
TEST(LeerArchivoTest, ArchivoEntradaIncorrectoTest) {
  std::vector<char const *> const args = {"10", "../../files/small.txt", "output.fld"};
  auto procesador                      = ProgArgs(3, args);
  EXPECT_EXIT(procesador.leerArchivo(), ::testing::ExitedWithCode(-3 + 256), ".*");
}

// El header del archivo de entrada indica un número de partículas negativo (-5)
TEST(LeerArchivoTest, NumParticlesIncorrecto1Test) {
  std::vector<char const *> const args = {"10", "../../files/small_-5.fld", "output.fld"};
  auto procesador                      = ProgArgs(3, args);
  EXPECT_EXIT(procesador.leerArchivo(), ::testing::ExitedWithCode(-5 + 256), ".*");
}

// El header del archivo de entrada indica que hay 4800 partículas, pero hay 4799 realmente
TEST(LeerArchivoTest, NumParticlesIncorrecto2Test) {
  std::vector<char const *> const args = {"10", "../../files/small_incomplete.fld", "output.fld"};
  auto procesador                      = ProgArgs(3, args);
  EXPECT_EXIT(procesador.leerArchivo(), ::testing::ExitedWithCode(-5 + 256), ".*");
}

// ------------------------------------------------------

// El archivo de salida se crea correctamente
TEST(EscribirArchivoTest, CorrectoTest) {
  std::vector<char const *> const args    = {"10", "../../files/small.fld",
                                             "../../files/small_out.fld"};
  auto procesador                         = ProgArgs(3, args);
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  // Creación de las partículas
  std::vector<Particle> Particulas;
  for (size_t i = 0; i < valoresDobles.size(); i += nueve) {
    Particle const nuevaParticula = Particle(
        std::vector<double>{0.0, 0.0, 0.0},
        std::vector<double>{valoresDobles[i], valoresDobles[i + 1], valoresDobles[i + 2]}, 0.0,
        std::vector<double>{valoresDobles[i + 3], valoresDobles[i + 4], valoresDobles[i + cinco]},
        std::vector<double>{valoresDobles[i + seis], valoresDobles[i + siete],
                            valoresDobles[i + ocho]});
    Particulas.push_back(nuevaParticula);
  }
  // Nombre del archivo de salida que se espera crear
  std::string const archivoSalida = procesador.getArchivoSalida();

  // Borra el archivo si ya existe
  int const borrado = std::remove(archivoSalida.c_str());
  if (borrado == 1) { std::cout << "Se ha borrado el archivo y existente.\n"; }

  // Llamar a la función que debería crear el archivo
  procesador.escribirArchivo(Particulas);

  // Verificar si el archivo se ha creado correctamente
  std::ifstream archivo(archivoSalida, std::ios::in | std::ios::binary);
  EXPECT_TRUE(archivo.is_open());
  archivo.close();
}

// ------------------------------------------------------

// La simulación produce los resultados esperados (en un archivo de salida) para un determinado
// archivo de entrada
TEST(SimulacionCorrecta, CorrectoTest) {
  std::vector<char const *> const args    = {"5", "../../files/small.fld",
                                             "../../files/small_out.fld"};
  auto procesador                         = ProgArgs(3, args);
  std::vector<double> const valoresDobles = procesador.leerArchivo();
  // Creación de las partículas
  std::vector<Particle> Particulas;
  for (size_t i = 0; i < valoresDobles.size(); i += nueve) {
    Particle const nuevaParticula = Particle(
        std::vector<double>{0.0, 0.0, 0.0},
        std::vector<double>{valoresDobles[i], valoresDobles[i + 1], valoresDobles[i + 2]}, 0.0,
        std::vector<double>{valoresDobles[i + 3], valoresDobles[i + 4], valoresDobles[i + cinco]},
        std::vector<double>{valoresDobles[i + seis], valoresDobles[i + siete],
                            valoresDobles[i + ocho]});
    Particulas.push_back(nuevaParticula);
  }
  // Declaración de parámetros de la simulación
  double const Masa_Particula_m            = Densidad_De_Fluido / pow(procesador.getPpm(), 3);
  double const Longitud_Suavizado_h        = Multiplicador_De_Radio / procesador.getPpm();
  std::vector<int> const Numero_Bloques    = calcularNumBloques(Longitud_Suavizado_h);
  std::vector<double> const Tamano_Bloques = {
    (Limite_Superior[0] - Limite_Inferior[0]) / double(Numero_Bloques[0]),
    (Limite_Superior[1] - Limite_Inferior[1]) / double(Numero_Bloques[1]),
    (Limite_Superior[2] - Limite_Inferior[2]) / double(Numero_Bloques[2])};

  auto Bloques = crearBloques(Numero_Bloques);

  int const time_steps = procesador.getTimesteps();
  for (int i = 1; i <= time_steps; i++) {
    // 1. Reposicionamiento de cada partícula en la malla.
    reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
    // printBloques(Bloques);
    //  2. Cálculo de fuerzas y aceleraciones para cada partícula.
    calculoAceleraciones(Particulas, Longitud_Suavizado_h, Masa_Particula_m, Bloques);
    // 3. Procesamiento de colisiones.
    chocarParticulasRecinto(Particulas, Numero_Bloques);
    // 4. Movimiento de partículas.
    movimientoParticulas(Particulas);
    // 5. Procesamiento de límites.
    chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  }
  procesador.escribirArchivo(Particulas);

  std::string const archivoSalida = procesador.getArchivoSalida();
  std::ifstream archivo_creado(archivoSalida, std::ios::in | std::ios::binary);

  std::string lectura_archivo_creado;
  if (archivo_creado.is_open()) {
    archivo_creado >>
        lectura_archivo_creado;  // Mover los contenidos del archivo a una cadena de texto
  }

  std::string const archivoSalidaProvisto = "../../files/small-5_CORRECTO.fld";
  std::ifstream archivo_provisto(archivoSalidaProvisto, std::ios::in | std::ios::binary);

  std::string lectura_archivo_provisto;
  if (archivo_provisto.is_open()) {
    archivo_provisto >>
        lectura_archivo_provisto;  // Mover los contenidos del archivo a una cadena de texto
  }
  ASSERT_EQ(lectura_archivo_creado, lectura_archivo_provisto);
}