#include "particle.hpp"

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
