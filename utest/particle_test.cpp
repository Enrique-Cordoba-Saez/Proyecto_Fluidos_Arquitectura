#include "sim/grid.hpp"
#include "sim/particle.hpp"
#include "sim/progargs.hpp"

#include "gtest/gtest.h"

// Para una partícula dentro de los límites del recinto, comprobar que se reposiciona en el bloque
// correcto
static double const menosNueveComaOcho        = -9.8;
double const ceroComaCeroSesentayCuatro       = 0.064903137;
double const menosCeroComaCeroSesentaYCuatro  = -ceroComaCeroSesentayCuatro;
double const ceroComaCeroQuince               = 0.015217851;
double const ceroComaCeroVeintinueve          = 0.029195517;
double const ceroComaDoscientosNoventaySeis   = 0.296863;
double const menosCeroComaVeintinueve         = -ceroComaDoscientosNoventaySeis;
double const ceroComaCientoDieciocho          = 0.11840522;
double const ceroComaCientoCuarenta           = 0.14053094;
double const ceroComaDoscientosTrece          = 0.21360235;
double const ceroComaCientoVeintitres         = 0.12304694;
double const ceroComaCientoTreintaiocho       = 0.13833959;
double const menosCeroComaCeroCincuentaiCinco = -0.055703137;
double const ceroComaDoscientosOnce           = 0.21121116;
double const menosCeroComaCeroSesentaySiete   = -0.067149506;
double const ceroComaCientoCinco              = 0.105217851;

TEST(ReposicionarParticulasTest, CorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula =
      Particle(std::vector<double>{0.0, menosNueveComaOcho, 0.0},
               std::vector<double>{menosCeroComaCeroCincuentaiCinco, ceroComaCeroQuince,
                                   ceroComaCeroVeintinueve},
               0.0, std::vector<double>{}, std::vector<double>{});
  Particulas.push_back(nuevaParticula);
  std::vector<int> const Numero_Bloques    = {15, 21, 15};
  std::vector<double> const Tamano_Bloques = {0.00866667, 0.00857143, 0.00866667};
  auto Bloques                             = crearBloques(Numero_Bloques);
  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
  ASSERT_EQ(Particulas[0].getBlockIndexes(), (std::vector<int>{1, 11, 10}));
  // Comprobar que el identificador de la partícula se ha añadido al bloque correspondiente
  bool found = false;
  for (auto p_id : Bloques[1][11][10]) {
    if (p_id == 0) { found = true; }
  }
  ASSERT_TRUE(found);
}

// Para una partícula por debajo de los límites del recinto, comprobar que se le asigna el primer
// bloque
TEST(ReposicionarParticulasTest, BajoRecintoCorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula =
      Particle(std::vector<double>{0.0, menosNueveComaOcho, 0.0},
               std::vector<double>{-0.075703137, -0.095217851, -0.069195517}, 0.0,
               std::vector<double>{}, std::vector<double>{});
  Particulas.push_back(nuevaParticula);
  std::vector<int> const Numero_Bloques    = {15, 21, 15};
  std::vector<double> const Tamano_Bloques = {0.00866667, 0.00857143, 0.00866667};
  auto Bloques                             = crearBloques(Numero_Bloques);
  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
  ASSERT_EQ(Particulas[0].getBlockIndexes(), (std::vector<int>{0, 0, 0}));
  bool found = false;
  for (auto p_id : Bloques[0][0][0]) {
    if (p_id == 0) { found = true; }
  }
  ASSERT_TRUE(found);
}

// Para una partícula por encima de los límites del recinto, comprobar que se le asigna el último
// bloque

TEST(ReposicionarParticulasTest, SobreRecintoCorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula =
      Particle(std::vector<double>{0.0, menosNueveComaOcho, 0.0},
               std::vector<double>{0.075703137, 0.15217851, 0.069195517}, 0.0,
               std::vector<double>{}, std::vector<double>{});
  Particulas.push_back(nuevaParticula);
  std::vector<int> const Numero_Bloques    = {15, 21, 15};
  std::vector<double> const Tamano_Bloques = {0.00866667, 0.00857143, 0.00866667};
  auto Bloques                             = crearBloques(Numero_Bloques);
  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);
  ASSERT_EQ(
      Particulas[0].getBlockIndexes(),
      (std::vector<int>{Numero_Bloques[0] - 1, Numero_Bloques[1] - 1, Numero_Bloques[2] - 1}));
  bool found = false;
  for (auto p_id : Bloques[Numero_Bloques[0] - 1][Numero_Bloques[1] - 1][Numero_Bloques[2] - 1]) {
    if (p_id == 0) { found = true; }
  }
  ASSERT_TRUE(found);
}

// ------------------------------------------------------------

// Para una partícula cualquiera, comprobar que se actualizan correctamente sus parámetros
TEST(MovimientoParticulasTest, CorrectoTest) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle const nuevaParticula = Particle(
      std::vector<double>{0.0, menosNueveComaOcho, 0.0},
      std::vector<double>{menosCeroComaCeroCincuentaiCinco, ceroComaCeroQuince,
                          ceroComaCeroVeintinueve},
      0.0,
      std::vector<double>{ceroComaDoscientosOnce, ceroComaCientoDieciocho, ceroComaCientoCuarenta},
      std::vector<double>{ceroComaDoscientosTrece, ceroComaCientoVeintitres,
                          ceroComaCientoTreintaiocho});
  Particulas.push_back(nuevaParticula);
  movimientoParticulas(Particulas);
  std::vector<float> position;
  for (int i = 0; i <= 2; ++i) {
    position.push_back(static_cast<float>(Particulas[0].getPosition()[i]));
  }

  ASSERT_EQ(position, (std::vector<float>{-0.055491924, 0.015326456, 0.029336048}));
  ASSERT_EQ(Particulas[0].getVelocityVector(),
            (std::vector<double>{ceroComaDoscientosOnce, 0.11350522, ceroComaCientoCuarenta}));
  ASSERT_EQ(Particulas[0].getHeadVector(),
            (std::vector<double>{ceroComaDoscientosOnce, 0.10860522, ceroComaCientoCuarenta}));
}

// ------------------------------------------------------------

TEST(ColisionesParticulasTest_Parte3_Test, TestEjeX_Minimo) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula =
      Particle(std::vector<double>{0.0, menosNueveComaOcho, 0.0},
               std::vector<double>{menosCeroComaCeroSesentaYCuatro, ceroComaCeroQuince,
                                   ceroComaCeroVeintinueve},
               0.0,
               std::vector<double>{menosCeroComaVeintinueve, ceroComaCientoDieciocho,
                                   ceroComaCientoCuarenta},
               std::vector<double>{ceroComaDoscientosTrece, ceroComaCientoVeintitres,
                                   ceroComaCientoTreintaiocho});

  int const Ppm                         = 19523;
  double const Longitud_Suavizado_h     = Multiplicador_De_Radio / Ppm;
  std::vector<int> const Numero_Bloques = {
    int(std::floor((Limite_Superior[0] - Limite_Inferior[0]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[1] - Limite_Inferior[1]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[2] - Limite_Inferior[2]) / Longitud_Suavizado_h))};
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada x ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(0, 0, 0);
  Particulas.push_back(nuevaParticula);

  double const new_position_x =
      menosCeroComaCeroSesentaYCuatro + menosCeroComaVeintinueve * Paso_de_tiempo;
  double const diferencia_con_limite  = Tamano_de_particula - new_position_x + Limite_Inferior[0];
  double const newExpectedAceleration = 0.0 + Colisiones_De_Rigidez * diferencia_con_limite -
                                        Amortiguamiento * ceroComaDoscientosTrece;
  chocarParticulasRecinto(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getAcceleration(),
            (std::vector<double>{newExpectedAceleration, menosNueveComaOcho, 0}));
}

TEST(ColisionesParticulasTest_Parte3_Test, TestEjeZ_Maximo) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula =
      Particle(std::vector<double>{0.0, menosNueveComaOcho, 0.0},
               std::vector<double>{menosCeroComaCeroCincuentaiCinco, ceroComaCeroQuince,
                                   ceroComaCeroSesentayCuatro},
               0.0,
               std::vector<double>{ceroComaDoscientosOnce, ceroComaCientoDieciocho,
                                   ceroComaDoscientosNoventaySeis},
               std::vector<double>{ceroComaCientoTreintaiocho, ceroComaCientoVeintitres,
                                   ceroComaDoscientosTrece});

  int const Ppm                         = 19523;
  double const Longitud_Suavizado_h     = Multiplicador_De_Radio / Ppm;
  std::vector<int> const Numero_Bloques = {
    int(std::floor((Limite_Superior[0] - Limite_Inferior[0]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[1] - Limite_Inferior[1]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[2] - Limite_Inferior[2]) / Longitud_Suavizado_h))};
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada z ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(Numero_Bloques[0] - 1, Numero_Bloques[1] - 1,
                                 Numero_Bloques[2] - 1);
  Particulas.push_back(nuevaParticula);

  double const new_position_z =
      ceroComaCeroSesentayCuatro + ceroComaDoscientosNoventaySeis * Paso_de_tiempo;
  double const diferencia_con_limite  = Tamano_de_particula + new_position_z - Limite_Superior[2];
  double const newExpectedAceleration = 0.0 - Colisiones_De_Rigidez * diferencia_con_limite -
                                        Amortiguamiento * ceroComaDoscientosTrece;
  chocarParticulasRecinto(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getAcceleration(),
            (std::vector<double>{0.0, menosNueveComaOcho, newExpectedAceleration}));
}

// ------------------------------------------------------------

TEST(InteraccionesLimitesRecinto_Parte5_Test, TestEjeZ_Minimo) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula = Particle(
      std::vector<double>{0.0, menosNueveComaOcho, 0.0},
      std::vector<double>{menosCeroComaCeroCincuentaiCinco, ceroComaCeroQuince,
                          menosCeroComaCeroSesentaySiete},
      0.0,
      std::vector<double>{ceroComaDoscientosOnce, ceroComaCientoDieciocho, ceroComaCientoCuarenta},
      std::vector<double>{ceroComaDoscientosTrece, ceroComaCientoVeintitres,
                          ceroComaCientoTreintaiocho});

  int const Ppm                         = 19523;
  double const Longitud_Suavizado_h     = Multiplicador_De_Radio / Ppm;
  std::vector<int> const Numero_Bloques = {
    int(std::floor((Limite_Superior[0] - Limite_Inferior[0]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[1] - Limite_Inferior[1]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[2] - Limite_Inferior[2]) / Longitud_Suavizado_h))};
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada z ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(0, 0, 0);
  Particulas.push_back(nuevaParticula);

  double const diferencia_con_limite = menosCeroComaCeroSesentaySiete - Limite_Inferior[2];
  double const new_position_z        = Limite_Inferior[2] - diferencia_con_limite;
  chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getPosition(), (std::vector<double>{menosCeroComaCeroCincuentaiCinco,
                                                              ceroComaCeroQuince, new_position_z}));

  // En los dos casos inferiores sencillamente se invierte el signo del valor en la coordenada z
  ASSERT_EQ(Particulas[0].getHeadVector(),
            (std::vector<double>{ceroComaDoscientosOnce, ceroComaCientoDieciocho,
                                 -ceroComaCientoCuarenta}));
  ASSERT_EQ(Particulas[0].getVelocityVector(),
            (std::vector<double>{ceroComaDoscientosTrece, ceroComaCientoVeintitres,
                                 -ceroComaCientoTreintaiocho}));
}

TEST(InteraccionesLimitesRecinto_Parte5_Test, TestEjeY_Maximo) {
  std::vector<Particle> Particulas;
  // Vector con una partícula con una posición válida
  Particle nuevaParticula = Particle(
      std::vector<double>{0.0, menosNueveComaOcho, 0.0},
      std::vector<double>{menosCeroComaCeroCincuentaiCinco, ceroComaCientoCinco,
                          ceroComaCeroVeintinueve},
      0.0,
      std::vector<double>{ceroComaDoscientosOnce, ceroComaCientoDieciocho, ceroComaCientoCuarenta},
      std::vector<double>{ceroComaDoscientosTrece, ceroComaCientoVeintitres,
                          ceroComaCientoTreintaiocho});

  int const Ppm                         = 19523;
  double const Longitud_Suavizado_h     = Multiplicador_De_Radio / Ppm;
  std::vector<int> const Numero_Bloques = {
    int(std::floor((Limite_Superior[0] - Limite_Inferior[0]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[1] - Limite_Inferior[1]) / Longitud_Suavizado_h)),
    int(std::floor((Limite_Superior[2] - Limite_Inferior[2]) / Longitud_Suavizado_h))};
  /*Para poner la función a prueba forzamos a que compruebe las 3 coordenadas
   * pese a que solo debería trabajar la coordenada z ya que los otros 2 indices
   * de bloque no se corresponden con las coordenadas reales*/
  nuevaParticula.setBlockIndexes(Numero_Bloques[0] - 1, Numero_Bloques[1] - 1,
                                 Numero_Bloques[2] - 1);
  Particulas.push_back(nuevaParticula);

  double const diferencia_con_limite = Limite_Superior[1] - ceroComaCientoCinco;
  double const new_position_y        = Limite_Superior[1] + diferencia_con_limite;
  chocarParticulasRecintoParte5(Particulas, Numero_Bloques);
  ASSERT_EQ(Particulas[0].getPosition(),
            (std::vector<double>{menosCeroComaCeroCincuentaiCinco, new_position_y,
                                 ceroComaCeroVeintinueve}));

  // En los dos casos inferiores sencillamente se invierte el signo del valor en la coordenada y
  ASSERT_EQ(Particulas[0].getHeadVector(),
            (std::vector<double>{ceroComaDoscientosOnce, -ceroComaCientoDieciocho,
                                 ceroComaCientoCuarenta}));
  ASSERT_EQ(Particulas[0].getVelocityVector(),
            (std::vector<double>{ceroComaDoscientosTrece, -ceroComaCientoVeintitres,
                                 ceroComaCientoTreintaiocho}));
}

//--------------------------------------------------------------

TEST(CalculoAceleraciones, CorrectoTest) {
  std::vector<char const *> const args = {"1", "../../files/small.fld",
                                          "../../files/small_out.fld"};
  auto procesador                      = ProgArgs(3, args);
  // Leer archivo de entrada: devuelve vector con todos los parámetros (sin el header) en double
  std::vector<double> const valoresDobles = procesador.leerArchivo();

  // Declaración de parámetros de la simulación
  double const Masa_Particula_m         = Densidad_De_Fluido / pow(procesador.getPpm(), 3);
  double const Longitud_Suavizado_h     = Multiplicador_De_Radio / procesador.getPpm();
  std::vector<int> const Numero_Bloques = calcularNumBloques(Longitud_Suavizado_h);

  std::vector<double> const Tamano_Bloques = ParametroTamanoBloque(Numero_Bloques);

  auto Bloques = crearBloques(Numero_Bloques);

  // Creación de las partículas
  std::vector<Particle> Particulas;

  creacionParticulas(valoresDobles, Particulas);

  reposicionarParticulas(Particulas, Numero_Bloques, Tamano_Bloques, Bloques);

  calculoAceleraciones(Particulas, Longitud_Suavizado_h, Masa_Particula_m, Bloques);

  // Valores esperados extraidos de acctransf-base-1.trz
  ASSERT_EQ(Particulas[0].getPosition()[0], -0.057331390678882599);
  ASSERT_EQ(Particulas[0].getPosition()[1], -0.080263644456863403);
  ASSERT_EQ(Particulas[0].getPosition()[2], -0.057781580835580826);
  ASSERT_EQ(Particulas[0].getHeadVector()[0], 0.21121115982532501);
  ASSERT_EQ(Particulas[0].getHeadVector()[1], 0.11840522289276123);
  ASSERT_EQ(Particulas[0].getHeadVector()[2], 0.14053094387054443);
  ASSERT_EQ(Particulas[0].getVelocityVector()[0], 0.21360234916210175);
  ASSERT_EQ(Particulas[0].getVelocityVector()[1], 0.12304694205522537);
  ASSERT_EQ(Particulas[0].getVelocityVector()[2], 0.13833959400653839);

  ASSERT_NEAR(Particulas[0].getDensity(), 950.4793412074838, 1e-12);
  ASSERT_NEAR(Particulas[0].getAcceleration()[0], -2086.6237724292255, 1e-12);
  ASSERT_NEAR(Particulas[0].getAcceleration()[1], -3312.2004205970125, 1e-12);
  ASSERT_NEAR(Particulas[0].getAcceleration()[2], 569.68497720306243, 1e-12);
}