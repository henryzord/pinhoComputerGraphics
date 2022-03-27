//
//  Matematica.cpp
//  Testes
//
//  Created by Márcio Sarroglia Pinho on 19/03/20.
//  Copyright © 2020 Márcio Sarroglia Pinho. All rights reserved.
//
#include <stdio.h>
#include <math.h>

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
     * Imprime o vetor atual.
     */
    void print() {
        printf("(%0.2f, %0.2f, %0.2f)", x, y, z);
    }
};

class Reta {
public:
    Vetor p1, p2;

    Reta(Vetor a, Vetor b) {
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
    int intersects(Reta other) {
        int o1 = this->angle(other.p1);
        int o2 = this->angle(other.p2);
        int o3 = other.angle(this->p1);
        int o4 = other.angle(this->p2);

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


/**
 * Verifica se um polígono é côncavo ou convexo.
 * @param num_vert Número de vértices do polígono
 * @param vetores Coordenadas dos vértices
 * @return 1 se o polígono for convexo, 0 se for côncavo
 */
int classificaPoligono(int num_vert, Vetor *vetores) {
    int signal = 1;

    for(int i = 0; i < num_vert; i++) {
        int index[3] = {
            ((i - 1) < 0) * (num_vert - 1) + ((i - 1) > 0) * (i - 1),
            i,
            ((i + 1) < num_vert) * (i + 1) + ((i + 1) > num_vert) * 0
        };

        Vetor a = vetores[index[1]] - vetores[index[0]];
        Vetor b = vetores[index[1]] - vetores[index[2]];

        Vetor c1 = a.cross(b);
        int new_signal = c1.z >= 0? 1 : -1;
        if(i == 0) {
            signal = new_signal;
        } else if(new_signal != signal) {
            return 0;
        }
    }
    return 1;
}

/**
 * Verifica se um Vetor está dentro de um polígono côncavo.
 * @param num_vert Número de vértices do polígono
 * @param Vetors Coordenadas dos vértices
 * @param Vetor Coordenada do Vetor
 * @return 1 se o Vetor estiver dentro do polígono, 0 se não
 */
int estaDentroConcavo(int num_vert, Vetor *pontos, Vetor ponto) {
    double max_x = -INFINITY;
    int sub = 0;
    for(int i = 0; i < num_vert; i++) {
        max_x = fmax(max_x, pontos[i].x);
    }
    max_x += 1; // just to be on the safe side

    Vetor pimag = Vetor(max_x, ponto.y);  // ponto imaginário fora do polígono
    Reta imag = Reta(ponto, pimag);  // reta do ponto até o ponto imaginário

    int count_intersects = 0;
    for(int i = 0; i < num_vert; i++) {
        int index[2] = {
            ((i - 1) < 0) * (num_vert - 1) + ((i - 1) > 0) * (i - 1),
            i
        };
        Reta aresta = Reta(pontos[index[0]], pontos[index[1]]);

        if(aresta.onSegment(ponto)) {
            return 1;
        }

        int intersects = imag.intersects(aresta);
        // se o ponto está dentro, mas está na altura de um vértice
        // subtrai uma interseção
        if((ponto.y == aresta.p1.y) || (ponto.y == aresta.p2.y)) {
            intersects = 0;
        }
        count_intersects += intersects;
    }
    return (count_intersects - sub) % 2;
}

/**
 * Verifica se um Vetor está dentro de um polígono convexo.
 * @param num_vert Número de vértices do polígono
 * @param vetores Coordenadas dos vértices
 * @param Vetor Coordenada do Vetor
 * @return 1 se o Vetor estiver dentro do polígono, 0 se não
 */
int estaDentroConvexo(int num_vert, Vetor *vetores, Vetor ponto) {
    int signal = 1;

    for(int i = 0; i < num_vert; i++) {
        int index[2] = {
            ((i - 1) < 0) * (num_vert - 1) + ((i - 1) > 0) * (i - 1),
            i
        };

        Vetor a = vetores[index[1]] - vetores[index[0]];
        Vetor b = vetores[index[1]] - ponto;

        Vetor c1 = a.cross(b);
        int new_signal = c1.z >= 0? 1 : -1;
        if(i == 0) {
            signal = new_signal;
        } else if(new_signal != signal) {
            return 0;
        }
    }
    return 1;

}

int main() {
    int num_vert = 6;
    int num_pontos = 5;

    // polígono côncavo
    Vetor pontos_concavo [6] = {
        *new Vetor(0.5, 0), // V1
        *new Vetor(0, 3),  // V2
        *new Vetor(3.5, 3.5),  // V3
        *new Vetor(3, 0.5),  // V4
        *new Vetor(2.5, 2),  // V5
        *new Vetor(2, 0),  // V6
    };
    Vetor pontos_convexo [6] = {
        *new Vetor(0.5, 0), // V1
        *new Vetor(0, 3),  // V2
        *new Vetor(3.5, 3.5),  // V3
        *new Vetor(3, 0.5),  // V4
        *new Vetor(2.5, 0),  // V5
        *new Vetor(2, 0),  // V6
    };

    Vetor pontos [5] = {
        *new Vetor(1.5, 2), // está dentro dos dois polígonos
        *new Vetor(0, 3),  // está em cima de um vértice dos dois polígonos
        *new Vetor(1.5, 0),  // está em cima de uma aresta dos dois polígonos
        *new Vetor(0, 0),  // é colinear com uma aresta, porém está fora
        *new Vetor(2.25, 0.0),  // está dentro do convexo mas fora do côncavo
    };

    char *txt[5] = {
        "circunscrito",
        "sobre um vértice",
        "sobre uma aresta",
        "colinear com uma aresta, porém fora",
        "dentro do convexo e fora do côncavo"
    };

    printf("o polígono côncavo é côncavo (pela função)? ");
    printf("%s\n", classificaPoligono(num_vert, pontos_concavo) == 0? "sim" : "não");

    printf("o polígono convexo é convexo (pela função)? ");
    printf("%s\n", classificaPoligono(num_vert, pontos_convexo) == 1? "sim" : "não");

    // agora verifica se os pontos estão dentro ou fora dos dois polígonos
    for(int i = 0; i < num_pontos; i++) {
        printf("o ponto %s está dentro do polígono côncavo? ", txt[i]);
        printf("%s\n", estaDentroConcavo(num_vert, pontos_concavo, pontos[i]) == 1? "sim" : "não");

        printf("o ponto %s está dentro do polígono convexo? ", txt[i]);
        printf("%s\n", estaDentroConvexo(num_vert, pontos_convexo, pontos[i]) == 1? "sim" : "não");
    }
    return 0;
}