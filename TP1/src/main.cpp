// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stack>
#include <queue>
#include <algorithm>

#include <map>
#include <set>

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

bool ComparaHora(Ponto base, Ponto a, Ponto b){
    int orientation = TipoCurva(base, a, b);

    // Desempate pelo mais distante
    if(orientation == 0) 
        return (Distancia(base, b) >= Distancia(base, a))? 0 : 1;

    // True para sentido anti-horário
    return (orientation == 1)? 1 : 0;
}

// Visita todos as arestas e as pinta de preto (1) - DFS
void encontraFaces(){
}

int main(int argc, char* argv[]){
    // Vetor de vertices
    std::vector<Vertice> vertices;
    // Numero de vértices e arestas
    int nvertices, narestas;
    // Vizinhos de cada vertice ordenado
    std::vector<std::queue<unsigned int>> adjacentes;

    std::cin >> nvertices >> narestas;
    vertices.resize(nvertices);

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

    // Insere os vértices de forma ordenada na fila
    for(int i=0; i < nvertices; i++){
        for(int j=0; j<vertices[i].grau; j++){
            adjacentes[i].push(vertices[i].vizinhos[j]);
        }
    }

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

    // Encontra as faces
    encontraFaces();

    return 0;
}

/*
vector<Vertices>
map<ID,pos>
set<pos,compara>
*/
