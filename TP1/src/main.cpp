// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include <algorithm>

// Estrutura ponto
struct Ponto{
    float x,y;
};
struct Aresta{
    unsigned int destino;
    int cor;

    Aresta(){
        this->cor = 0;
    }

    bool visitado(){
        return this->cor != 0;
    }
};

// Estrutura que representa um vertice
struct Vertice{
    Ponto cord; 
    int grau;
    std::vector<Aresta> vizinhos;
};

// Distancia euclidiana de a para b.
float Distancia(Ponto a, Ponto b){
    float x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}

// Coeficiente da reta que passa na origem e p.
double Inclinacao(Ponto p){
    return atan2(p.y, p.x);
}

// Coeficiente da reta orientada de p para q.
double InclinacaoRelativa(Ponto central, Ponto vizinho) {
    return atan2(vizinho.y - central.y, vizinho.x - central.x);
}

// Determina se ao caminhar de a para b e depois de b para c estamos fazendo uma curva `a esquerda, `a direita, ou seguindo em frente.
float TipoCurva(Ponto a, Ponto b, Ponto c){
    float v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v == 0) return 0; // colinear
    return (v > 0)? 1 : -1; // horario ou antihorario
}

// Compara dois pontos em relação a um ponto central
bool Compara(Ponto base, Ponto a, Ponto b){
    double angulo_a = InclinacaoRelativa(base, a);
    double angulo_b = InclinacaoRelativa(base, b);
    if(angulo_a == angulo_b){
        return Distancia(base, a) > Distancia(base, b);
    }
    return angulo_a > angulo_b;
}

// Visita todos as arestas e as pinta de preto (1) - DFS
void encontraFaces(std::vector<std::vector<Aresta>>& arestas, int vertice, unsigned int anterior = 0){
    for(unsigned int i=0; i < arestas[vertice-1].size(); i++){
        if(!arestas[vertice-1][i].visitado() && arestas[vertice-1][i].destino != anterior){
            arestas[vertice-1][i].cor = 1;
            std::cout << "Aresta " << vertice << " -> " << arestas[vertice-1][i].destino << std::endl;
            encontraFaces(arestas, arestas[vertice-1][i].destino, vertice);
        }
    }
}


int main(int argc, char* argv[]){
    // Vetor de vertices
    std::vector<Vertice> vertices;
    // Numero de vértices e arestas
    int nvertices, narestas;

    std::cin >> nvertices >> narestas;
    vertices.resize(nvertices);

    for(int i = 0; i < nvertices; i++){
        std::cin >> vertices[i].cord.x >> vertices[i].cord.y >> vertices[i].grau;
        vertices[i].vizinhos.resize(vertices[i].grau);
        for(int j = 0; j < vertices[i].grau; j++){
            std::cin >> vertices[i].vizinhos[j].destino;
        }
    }

    // Ordena os vizinhos de cada vértice
    for(Vertice& vertice: vertices){
        std::sort(vertice.vizinhos.begin(), vertice.vizinhos.end(), [&](Aresta a, Aresta b){
            return Compara(vertice.cord, vertices[a.destino-1].cord, vertices[b.destino-1].cord);
        });
    }

    // Imprime os vertices
    for(unsigned int i=0; i < vertices.size(); i++){
        std::cout << "Vertice " << i + 1 << ": (" << vertices[i].cord.x << ", " << vertices[i].cord.y << ")" << std::endl;
        std::cout << "Grau: " << vertices[i].grau << std::endl;
        std::cout << "Vizinhos: ";
        for(int j=0; j < vertices[i].grau; j++){
            std::cout << vertices[i].vizinhos[j].destino << " ";
        }
        std::cout << std::endl << std::endl;
    }

    // Extrai as arestas
    std::vector<std::vector<Aresta>> arestas;
    arestas.resize(nvertices);
    for(unsigned int i=0; i < vertices.size(); i++){
        for(int j=0; j < vertices[i].grau; j++){
            arestas[i].push_back(vertices[i].vizinhos[j]);
        }
    }

    // Imprime as arestas
    for(unsigned int i=0; i < arestas.size(); i++){
        std::cout << "Arestas do vertice " << i + 1 << ": ";
        for(unsigned int j=0; j < arestas[i].size(); j++){
            std::cout << arestas[i][j].destino << " ";
        }
        std::cout << std::endl;
    }
    
    // Encontra as faces
    encontraFaces(arestas, 1);

    return 0;
}

/*
Ideia para o algoritmo

Considerar os dois caminhos de cada aresta (OK)

Ordenar os vizinhos de cada vértice pelo angulo no sentido antihorário (OK)

Andar por cada aresta em sentido antihorário e colorir ela ao cruzá-la 

Ao voltar para o vértice original a face foi encontrada

Fazer isso até colorir todas as arestas
*/
