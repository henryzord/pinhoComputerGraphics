#ifndef TWOD_VECTORS_H
#define TWOD_VECTORS_H

#define _USE_MATH_DEFINES
#include <math.h>

int toDegrees(double radians) {
    return (int)((radians * 180.0) / M_PI);
}

double toRadians(int degrees) {
    return (M_PI * (double)degrees)/180.0;
}

class Vetor {
public:
    double x, y, z;

    Vetor() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vetor(double x, double y) {
        this->x = x;
        this->y = y;
        this->z = 0;
    }

    Vetor(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    static double point_distance(Vetor a, Vetor b) {
        return sqrt(
            pow(a.x - b.x, 2) +
            pow(a.y - b.y, 2) +
            pow(a.z - b.z, 2)
        );
    }

    /**
     * Produto interno (com sobrecarga de operador)
     * @param other Outro vetor
     * @return
     */
    double operator *(Vetor const other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    /**
     * Produto interno (método).
     * @param other Outro vetor
     * @return
     */
    double dot(Vetor const other) {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    /**
     * Produto vetorial.
     * @param other Outro vetor
     * @return
     */
    Vetor cross(Vetor const other) {
        double nx = this->y * other.z - this->z * other.y;
        double ny = this->z * other.x - this->x * other.z;
        double nz = this->x * other.y - this->y * other.x;

        return {nx, ny, nz};
    }

    /**
     * Multiplicação de um vetor por um escalar, com sobrecarga de operador.
     * @param n Escalar
     * @return
     */
    Vetor operator *(double n) {
        return {this->x * n, this->y * n, this->z * n};
    }

    /**
     * Adição de um vetor por um escalar, com sobrecarga de operador.
     * @param n Escalar
     * @return
     */
    Vetor operator +(Vetor const other) {
        return {this->x + other.x, this->y + other.y, this->z + other.z};
    }

    /**
     * Subtração de um vetor por um escalar, com sobrecarga de operador.
     * @param n Escalar
     * @return
     */
    Vetor operator -(Vetor const other) {
        return {this->x - other.x, this->y - other.y, this->z - other.z};
    }

    /**
     * Módulo do vetor.
     * @return O módulo do vetor atual.
     */
    double module() {
        return sqrt(x * x + y * y + z * z);
    }

    /**
     * Versor do vetor.
     * @return Um novo vetor, que é um versor do vetor atual.
     */
    Vetor versor() {
        double mod = module();
        return {x / mod, y / mod, z / mod};
    }

    /**
     * Ângulo que este vetor faz com outro vetor.
     * @param other Outro vetor
     * @return Ângulo (em graus) entre este e outro vetor
     */
    int angle(Vetor other) {
        double dotProduct = this->dot(other);
        if(dotProduct == 0) {
            return 90;
        }

        double thisModule = this->module();
        double otherModule = other.module();
        double thisAngle = acos(dotProduct/(thisModule * otherModule));
        int thisDegreeAngle = toDegrees(thisAngle);
        return thisDegreeAngle;
    }

    /**
     * Imprime o vetor atual.
     */
    void print() {
        printf("(%0.2f, %0.2f, %0.2f)", x, y, z);
    }
};

#endif // TWO_VECTORS_H