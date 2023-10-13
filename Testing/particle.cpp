#include "particle.hpp"
#include <iostream>

Particle::Particle(
    double initialDensity, double initialAcceleration,
    const Vector& initialPosition,
    const Vector& initialHeadVector,
    const Vector& initialVelocityVector
) : density(initialDensity),
    acceleration(initialAcceleration),
    position(initialPosition),
    headVector(initialHeadVector),
    velocityVector(initialVelocityVector) {
    // Constructor implementation
}

double Particle::getDensity() const {
    return density;
}

double Particle::getAcceleration() const {
    return acceleration;
}

Vector Particle::getPosition() const {
    return position;
}

Vector Particle::getHeadVector() const {
    return headVector;
}

Vector Particle::getVelocityVector() const {
    return velocityVector;
}

void Particle::reposition() {
    std::cout << "Repositioning particle..." << std::endl;

    incremento_densidad();
    transformacion_densidad();
    transferencia_aceleracion();
}

void Particle::incremento_densidad() {
    std::cout << "Incremento de densidad..." << std::endl;
}

void Particle::transformacion_densidad() {
    std::cout << "Transformacion de densidad..." << std::endl;
}

void Particle::transferencia_aceleracion() {
    std::cout << "Transferencia de aceleracion..." << std::endl;
}
