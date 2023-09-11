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

// Estrutura que representa um vertice
struct Vertice{
    Ponto cord; 
    int grau;
    std::vector<unsigned int> vizinhos;
};

struct Aresta{
    unsigned int origem, destino;
    std::string cor;

    Aresta(){
        this->cor = "branco";
    }
};

// Extrai as arestas do grafo
void extraiArestas(const std::vector<Vertice>& vertices, std::vector<Aresta>& arestas, int nvertices){
    int w=0;
    for(int i=0; i < nvertices; i++){
        for(int j=0; j < vertices[i].grau; j++){
            arestas[w].origem = i + 1;
            arestas[w].destino = vertices[i].vizinhos[j];
            w++;
        }
    }
}

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
double InclinacaoRelativa(Ponto p, Ponto q) {
    return atan2(q.y - p.y, q.x - p.x);
}

// Determina se ao caminhar de a para b e depois de b para c estamos fazendo uma curva `a esquerda, `a direita, ou seguindo em frente.
int TipoCurva(Ponto a, Ponto b, Ponto c){
    float v = a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y);
    if (v == 0) return 0; // colinear
    return (v > 0)? 1 : -1; // horario ou antihorario
}

bool Compara(Ponto base, Ponto a, Ponto b){
    int orientation = TipoCurva(base, a, b);

    // Desempate pelo mais distante
    if(orientation == 0) 
        return (Distancia(base, b) >= Distancia(base, a))? 0 : 1;

    // True para sentido anti-horário
    return (orientation == 1)? 1 : 0;
}

// Encontra as faces do grafo - Ainda não funciona
void encontraFaces(){}

int main(int argc, char* argv[]){
    // Vetor de vertices
    std::vector<Vertice> vertices;
    // Vetor de arestas
    std::vector<Aresta> arestas;
    // Numero de vértices e arestas
    int nvertices, narestas;

    std::cin >> nvertices >> narestas;

    // Considerar as duas direções de cada aresta
    narestas = narestas*2;

    vertices.resize(nvertices);
    arestas.resize(narestas);

    for(int i = 0; i < nvertices; i++){
        std::cin >> vertices[i].cord.x >> vertices[i].cord.y >> vertices[i].grau;
        vertices[i].vizinhos.resize(vertices[i].grau);
        for(int j = 0; j < vertices[i].grau; j++){
            std::cin >> vertices[i].vizinhos[j];
        }
    }

    // Ordena os vizinhos de cada vértice
    for(Vertice& vertice: vertices){
        std::sort(vertice.vizinhos.begin(), vertice.vizinhos.end(), [&](int a, int b){
            return Compara(vertice.cord, vertices[a-1].cord, vertices[b-1].cord);
        });
    }

    // Extrai as arestas
    extraiArestas(vertices, arestas, nvertices);

    // Imprime os vertices
    for(unsigned int i=0; i < vertices.size(); i++){
        std::cout << "Vertice " << i + 1 << ": (" << vertices[i].cord.x << ", " << vertices[i].cord.y << ")" << std::endl;
        std::cout << "Grau: " << vertices[i].grau << std::endl;
        std::cout << "Vizinhos: ";
        for(int j=0; j < vertices[i].grau; j++){
            std::cout << vertices[i].vizinhos[j] << " ";
        }
        std::cout << std::endl << std::endl;
    }

    // Imprime as arestas
    for(int i=0; i < narestas; i++){
        std::cout << "Aresta " << i << ": (" << arestas[i].origem << ", " << arestas[i].destino << ")" << std::endl;
    }

    
    // Encontra as faces - Ainda não funciona
    //encontraFaces();

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
