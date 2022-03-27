#ifndef RETA_H
#define RETA_H

#include <math.h>

class Line {
public:
    Vetor p1, p2;

    Line(Vetor a, Vetor b) {
        this->p1 = a;
        this->p2 = b;
    }

    /**
     * Dada uma reta e um ponto, verifica se o ponto está sobre o segmento de reta
     * @param ponto
     * @return 1 se o ponto estiver no segmento da reta, 0 de outra forma
     */
    int onSegment(Vetor ponto) {
        double dist1 = sqrt(pow(this->p1.x - ponto.x, 2) + pow(this->p1.y - ponto.y, 2));
        double dist2 = sqrt(pow(this->p2.x - ponto.x, 2) + pow(this->p2.y - ponto.y, 2));

        double dist3 = sqrt(pow(this->p1.x - this->p2.x, 2) + pow(this->p1.y - this->p2.y, 2));

        return (dist1 + dist2) == dist3;
    }

    /**
     * Computa se duas retas se intersectam.
     * @param other Outra reta
     * @return 1 se as retas se intersectam, 0 de outra forma
     */
    int intersects(Line other) {
        int o1 = (int)this->angle(other.p1);
        int o2 = (int)this->angle(other.p2);
        int o3 = (int)other.angle(this->p1);
        int o4 = (int)other.angle(this->p2);

        // caso geral
        if(o1 != o2 && o3 != o4) {
            return 1;
        }
        if(o1 == 0 && this->onSegment(other.p1)) {
            return 1;
        }
        if(o2 == 0 && this->onSegment(other.p2)) {
            return 1;
        }
        if(o3 == 0 && other.onSegment(this->p1)) {
            return 1;
        }
        if(o4 == 0 && other.onSegment(this->p2)) {
            return 1;
        }
        return 0;
    }

    /**
     * Orientação entre ponto e reta: 0 se forem colineares, 1 se estiverem em sentido horário, 2 anti-horário
     * @param ponto
     * @return
     */
    double angle(Vetor ponto) {
        int val = (int)((this->p2.y - this->p1.y) * (ponto.x - this->p2.x) - (this->p2.x - this->p1.x) * (ponto.y - this->p2.y));

        // ponto e reta são colineares
        if (val == 0) {
            return 0;
        }
        return (val > 0)? 1: 2; // horário ou anti-horário
    }
};

# endif // RETA_H