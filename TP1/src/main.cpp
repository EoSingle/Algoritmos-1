// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

// Estrutura Ponto
struct Ponto{
    float x,y;

    Ponto(float _x, float _y) : x(_x), y(_y) {}

    Ponto operator - (const Ponto& p) const{
        return Ponto(this->x - p.x, this->y - p.y);
    }

    int ProdutoVetorial(const Ponto& p) const{
        return this->x * p.y - this->y * p.x;
    }

    int ProdutoVetorial(const Ponto& a, const Ponto& b) const{
        return (a - *this).ProdutoVetorial(b - *this);
    }

    int PosicaoPlano () const{
        return (this->y < 0 || (this->y == 0 && this->x < 0));
    }
};

// Estrutura que representa um Vértice
struct Vertice{
    Ponto cord; 
    int grau;
    std::vector<unsigned int> vizinhos;

    Vertice(Ponto _cord, int _grau) : cord(_cord), grau(_grau) {}
    Vertice() : cord(Ponto(0,0)), grau(0) {}
};

// Função de comparação para ordenar os vertices
bool Compara(int base, int a, int b, std::vector<Vertice>& vertices){
    Ponto p = vertices[a].cord - vertices[base].cord;
    Ponto q = vertices[b].cord - vertices[base].cord;
    if(p.PosicaoPlano() != q.PosicaoPlano()) return p.PosicaoPlano() < q.PosicaoPlano();
    return p.ProdutoVetorial(q) > 0;
}

double Distancia(Ponto a, Ponto b){
    double x = (a.x - b.x), y = (a.y - b.y);
    return sqrt(x*x + y*y);
}

int TipoCurva(Ponto base, Ponto a, Ponto b){
    double v = base.x*(a.y-b.y)+a.x*(b.y-base.y)+b.x*(base.y-a.y);
    if (v < 0) return -1;// esquerda
    if (v > 0) return +1; // direita
    return 0; // em frente
}

bool ComparaCurva(int base, int a, int b, std::vector<Vertice>& vertices){
    Ponto ponto_base = vertices[base].cord,
    ponto_a = vertices[a].cord,
    ponto_b = vertices[b].cord;

    int tipo = TipoCurva(ponto_base, ponto_a, ponto_b);

    if(tipo == 0) 
        return (Distancia(ponto_base, ponto_a) <= Distancia(ponto_base, ponto_b))? 0:1;

    return (tipo == 1)? 1:0; 
}

double InclinacaoRelativa(Ponto central, Ponto vizinho) {
    return atan2(vizinho.y - central.y, vizinho.x - central.x);
}

bool ComparaAngulo(int i_base, int i_a, int i_b, std::vector<Vertice>& vertices){
    Ponto base = vertices[i_base].cord,
    a = vertices[i_a].cord, 
    b = vertices[i_b].cord;

    double angulo_a = InclinacaoRelativa(base, a);
    double angulo_b = InclinacaoRelativa(base, b);
    if(angulo_a == angulo_b){
        return Distancia(base, a) > Distancia(base, b);
    }
    return angulo_a > angulo_b;
}

// Encontra as faces
std::vector<std::vector<unsigned int>> EncontraFaces(std::vector<Vertice> vertices, std::vector<std::vector<unsigned int>> adjacentes){
    std::vector<std::vector<unsigned int>> faces;
    std::vector<std::vector<bool>> visitados(vertices.size());

    // Inicializa o vetor de visitados
    for(unsigned int i=0; i < vertices.size(); i++){
        visitados[i].resize(vertices[i].grau);
        visitados[i].assign(vertices[i].grau, false);

        // Ordena os vizinhos de cada vértice
        
        std::sort(adjacentes[i].begin(), adjacentes[i].end(), [&](int a, int b){
            return ComparaAngulo(i, a, b, vertices);
        });

        /*auto compara = [&](int a, int b){
            Ponto p = vertices[a].cord - vertices[i].cord;
            Ponto q = vertices[b].cord - vertices[i].cord;
            if(p.PosicaoPlano() != q.PosicaoPlano()) return p.PosicaoPlano() < q.PosicaoPlano();
            return p.ProdutoVetorial(q) > 0;
        };
        std::sort(adjacentes[i].begin(), adjacentes[i].end(), compara);*/

    }

    // Percorre os vértices
    for(unsigned int i=0; i < vertices.size(); i++){
        // Percorre as arestas
        for(int j=0; j < vertices[i].grau; j++){
            // Se a aresta já foi visitada, pula
            if(visitados[i][j]) continue;

            std::vector<unsigned int> face;
            int vertice_atual = i, aresta_atual = j;

            // Enquanto não voltar para o vértice inicial
            while(!visitados[vertice_atual][aresta_atual]){
                // Marca a aresta como visitada e adiciona o vértice na face
                visitados[vertice_atual][aresta_atual] = true;
                face.push_back(vertice_atual);

                int aux = adjacentes[vertice_atual][aresta_atual];
                // Encontra a próxima aresta

                unsigned int proximo = std::lower_bound(adjacentes[aux].begin(), adjacentes[aux].end(), vertice_atual, [&](int a, int b){
                    return ComparaAngulo(aux, a, b, vertices);
                }) - adjacentes[aux].begin() + 1;         

                /*unsigned int proximo = std::lower_bound(adjacentes[aux].begin(), adjacentes[aux].end(), vertice_atual, [&](int a, int b){
                    Ponto p = vertices[a].cord - vertices[aux].cord;
                    Ponto q = vertices[b].cord - vertices[aux].cord;
                    if(p.PosicaoPlano() != q.PosicaoPlano()) return p.PosicaoPlano() < q.PosicaoPlano();
                    return p.ProdutoVetorial(q) > 0;
                }) - adjacentes[aux].begin() + 1;*/

                // Se o próximo for igual ao tamanho do vetor encontramos a face
                if(proximo == adjacentes[aux].size()) proximo = 0;

                // Atualiza o vertice e a aresta atual
                vertice_atual = aux;
                aresta_atual = proximo;
            }
            // Adiciona o ultimo vertice da face
            face.push_back(vertice_atual);

            // Adiciona a face no vetor de faces
            faces.push_back(face);
        }
    }
    return faces;
}

// Função principal
int main(int argc, char* argv[]){
    // Vetor de vertices
    std::vector<Vertice> vertices;
    // Numero de vértices e arestas
    int nvertices, narestas;
    // Vizinhos de cada vertice ordenado
    std::vector<std::vector<unsigned int>> adjacentes;

    std::cin >> nvertices >> narestas;
    vertices.resize(nvertices);
    adjacentes.resize(nvertices);

    // Leitura dos vértices
    for(int i = 0; i < nvertices; i++){
        std::cin >> vertices[i].cord.x >> vertices[i].cord.y >> vertices[i].grau;
        vertices[i].vizinhos.resize(vertices[i].grau);
        for(int j = 0; j < vertices[i].grau; j++){
            std::cin >> vertices[i].vizinhos[j];
        }
    }

    // Insere os vértices na matriz de adjacentes
    for(int i=0; i < nvertices; i++){
        for(int j=0; j < vertices[i].grau; j++){
            adjacentes[i].push_back(vertices[i].vizinhos[j]-1); // -1 para compensar o offset
        }
    }
    
    // Vetor de faces
    std::vector<std::vector<unsigned int>> faces = EncontraFaces(vertices, adjacentes);

    // Imprime o número de faces
    std::cout << faces.size() << std::endl;
    // Imprime as faces
    for(unsigned int i=0; i < faces.size(); i++){
        std::cout << faces[i].size() << " ";
        for(unsigned int j=0; j < faces[i].size(); j++){
            std::cout << faces[i][j] + 1 << " "; // +1 para compensar o offset
        }
        std::cout << std::endl;
    }

    return 0;
}
