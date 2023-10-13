#ifndef PARTICLE_HPP
#define PARTICLE_HPP

struct Vector {
    double x;
    double y;
    double z;
};

class Particle {
public:
    Particle(
        double density = 0.0, double acceleration = 0.0,
        const Vector& initialPosition = Vector(),
        const Vector& initialHeadVector = Vector(),
        const Vector& initialVelocityVector = Vector()
    );

    // Getter methods
    double getDensity() const;
    double getAcceleration() const;
    Vector getPosition() const;
    Vector getHeadVector() const;
    Vector getVelocityVector() const;

    void reposition();
    void incremento_densidad(); // estoy mezclando inglés y español
    void transformacion_densidad();
    void transferencia_aceleracion();

private:
    double density;
    double acceleration;
    Vector position;
    Vector headVector;
    Vector velocityVector;
};

#endif // PARTICLE_HPP
