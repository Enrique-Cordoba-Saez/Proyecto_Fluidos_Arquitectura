int calculoAceleraciones(std::vector<Particle>& particulas, double const Longitud_Suavizado_h,
                         double const Masa_Particula_m) {
  for (auto& particle : particulas) { //probar con indice, en vez de directamente con los objetos
    //Iniciación de aceleraciones
    std::cout << "Iniciación de aceleraciones" << std::endl;
    particle.setAcceleration(0.0, Gravedad, 0.0);
    particle.setDensity(0.0);

    //Incremento de densidades
    incrementoDensidad(particle, particulas, Longitud_Suavizado_h, Masa_Particula_m);
  }
  return 1;
}

int incrementoDensidad(Particle& particula, std::vector<Particle>& particulas, double const Longitud_Suavizado_h,
                       double const Masa_Particula_m) {
  // Your implementation of incrementoDensidad
  std::cout << "Incremento de densidad" << particula.getPosition()[0] << std::endl;
  auto it = std::find(particulas.begin(), particulas.end(), particula); //Esta linea da problemas
  size_t index = std::distance(particulas.begin(), it);
  size_t length = particulas.size() - 1;

  for (size_t i = index + 1; i <= length; ++i) {
    Particle & particula2 = particulas[i];
    // Squared value of h
    double h2 = Longitud_Suavizado_h * Longitud_Suavizado_h;

    // Distance between particula and particula2
    double r = std::pow(particula.getAcceleration()[0] - particula2.getAcceleration()[0], 2) +
               std::pow(particula.getAcceleration()[1] - particula2.getAcceleration()[1], 2) +
               std::pow(particula.getAcceleration()[2] - particula2.getAcceleration()[2], 2);

    // Density increment
    double var_den = std::pow(h2 - r, 3);

    if (r < h2) {
      particula.setDensity(particula.getDensity() + var_den);
      particula.setDensity(particula.getDensity() + var_den);

      transformacionDensidad(particula, Longitud_Suavizado_h, Masa_Particula_m);
      transformacionDensidad(particula2, Longitud_Suavizado_h, Masa_Particula_m);

      transferenciaAcerlacion(particula, particula2, Longitud_Suavizado_h, Masa_Particula_m);
    }
  }
  return particulas.size();
}

int transformacionDensidad(Particle& particula, double const Longitud_Suavizado_h,
                           double const Masa_Particula_m){
  double factor = (particula.getDensity() * std::pow(Longitud_Suavizado_h, seis))
                  * (315 / (64 * Pi * std::pow(Longitud_Suavizado_h, nueve)))
                  * Masa_Particula_m;
  particula.setDensity(factor);
  return 1;
}

int transferenciaAcerlacion(Particle& particula, Particle& particula2, double const Longitud_Suavizado_h,
                            double const Masa_Particula_m){
  // Calculate squared distance
  double distanciaSquared = std::max(
      std::pow(particula.getPosition()[0] - particula2.getPosition()[0], 2) +
          std::pow(particula.getPosition()[1] - particula2.getPosition()[1], 2) +
          std::pow(particula.getPosition()[2] - particula2.getPosition()[2], 2),
      1e-12
  );

  // Calculate acceleration components
  double factor = 15 / (Pi * std::pow(Longitud_Suavizado_h, 6)) * Masa_Particula_m * ((Longitud_Suavizado_h - std::sqrt(distanciaSquared)) / std::sqrt(distanciaSquared));

  double var_ax = (particula.getPosition()[0] - particula2.getPosition()[0]) * factor *
                      (particula.getDensity() + particula2.getDensity() - Densidad_De_Fluido) +
                  (particula.getVelocityVector()[0] - particula2.getVelocityVector()[0]) *
                      (45 / (Pi * std::pow(Longitud_Suavizado_h, 6))) * Viscosidad * Masa_Particula_m / (particula.getDensity() - particula2.getDensity() + 1);

  double var_ay = (particula.getPosition()[1] - particula2.getPosition()[1]) * factor *
                      (particula.getDensity() + particula2.getDensity() - Densidad_De_Fluido) +
                  (particula.getVelocityVector()[1] - particula2.getVelocityVector()[1]) *
                      (45 / (Pi * std::pow(Longitud_Suavizado_h, 6))) * Viscosidad * Masa_Particula_m / (particula.getDensity() - particula2.getDensity() + 1);

  double var_az = (particula.getPosition()[2] - particula2.getPosition()[2]) * factor *
                      (particula.getDensity() + particula2.getDensity() - Densidad_De_Fluido) +
                  (particula.getVelocityVector()[2] - particula2.getVelocityVector()[2]) *
                      (45 / (Pi * std::pow(Longitud_Suavizado_h, 6))) * Viscosidad * Masa_Particula_m / (particula.getDensity() - particula2.getDensity() + 1);

  // Update accelerations
  particula.setAcceleration(particula.getAcceleration()[0] + var_ax,
                            particula.getAcceleration()[1] + var_ay,
                            particula.getAcceleration()[2] + var_az);

  particula2.setAcceleration(particula2.getAcceleration()[0] - var_ax,
                             particula2.getAcceleration()[1] - var_ay,
                             particula2.getAcceleration()[2] - var_az);
  return 1;
}
