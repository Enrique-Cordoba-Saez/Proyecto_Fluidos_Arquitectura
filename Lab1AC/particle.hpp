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
        double density = 0.0, double acceleration = 0.0, // vector de aceleracion?
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

private:
    double density;
    double acceleration;
    Vector position;
    Vector headVector;
    Vector velocityVector;
};

#endif // PARTICLE_HPP
