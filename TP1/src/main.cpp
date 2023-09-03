// Lucas Albano Olive Cruz - 2022036209

// TODO: Pesquisar algoritmo de Tarjan

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <queue>
#include <algorithm>

// Estrutura que representa um vertice
struct Vertice{
    float x, y; 
    int grau;
    std::vector<unsigned int> vizinhos;
};

// Le o grafo do arquivo e armazena em um vetor de vertices
void leGrafo(const std::string& arquivo, std::vector<Vertice>& vertices){
    std::ifstream file(arquivo);
    if(!file.is_open()){
        std::cout << "Erro ao abrir o arquivo" << std::endl;
        exit(1);
    }

    std::string line;
    if(std::getline(file, line)){
        std::istringstream iss(line);
        int nvertices, narestas;
        if(iss >> nvertices >> narestas){
            vertices.resize(nvertices);

            for(int i=0; i < nvertices; i++){
                file >> vertices[i].x >> vertices[i].y >> vertices[i].grau;

                vertices[i].vizinhos.resize(vertices[i].grau);
                for(int j=0; j < vertices[i].grau; j++){
                    file >> vertices[i].vizinhos[j];
                }
            }
        } else {
            std::cout << "Erro ao ler a primeira linha do arquivo" << std::endl;
            exit(1);
        }
    } else {
        std::cout << "Arquivo está vazio" << std::endl;
        exit(1);
    }

    file.close();
}

// Extrai as arestas do grafo
void extraiArestas(const std::vector<Vertice>& vertices, std::vector<std::pair<int, int>>& arestas){
    for(unsigned int i=0; i < vertices.size(); i++){
        for(int j=0; j < vertices[i].grau; j++){
            if(i < vertices[i].vizinhos[j]){
                arestas.push_back(std::make_pair(i + 1, vertices[i].vizinhos[j]));
            }
        }
    }
}

// Busca em largura - Constrói a árvore BFS
void bfs(int inicio, const std::vector<std::pair<int, int>>& arestas, std::vector<int>& parente, std::vector<int>& nivel){
    std::queue<int> fila;
    fila.push(inicio);
    nivel[inicio] = 0;
    parente[inicio] = -1;

    while(!fila.empty()){
        int u = fila.front();
        fila.pop();

        for(unsigned int i=0; i < arestas.size(); i++){
            if(arestas[i].first == u){
                int v = arestas[i].second;
                if(nivel[v] == -1){
                    nivel[v] = nivel[u] + 1;
                    parente[v] = u;
                    fila.push(v);
                }
            }
        }
    }
}

// Encontra as faces do grafo - Ainda não funciona
void encontraFaces(const std::vector<Vertice>& vertices, const std::vector<std::pair<int, int>>& arestas){
    int nvertices = vertices.size();
    std::vector<int> nivel(nvertices, -1);
    std::vector<int> parente(nvertices, -1);

    for(int i=0; i < nvertices; i++){
        if(nivel[i] == -1){
            bfs(i, arestas, parente, nivel);
        }
    }

    std::vector<std::vector<int>> faces;
    for(int u = 0; u < nvertices; u++){
        for(unsigned int v=0; v < arestas.size(); v++){
            if(parente[arestas[v].first] != arestas[v].second && parente[arestas[v].second] != arestas[v].first){
                std::vector<int> face;
                int w = arestas[v].first;
                while(w != -1){
                    face.push_back(w);
                    w = parente[w];
                }
                face.push_back(arestas[v].second);
                faces.push_back(face);
            }
        }
    }

    std::cout << "Faces:" << std::endl;
    for(unsigned int i=0; i < faces.size(); i++){
        std::cout << "Face " << i + 1 << ": ";
        for(unsigned int j=0; j < faces[i].size(); j++){
            std::cout << faces[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[]){
    // Verifica se o numero de argumentos é valido
    if(argc != 2){
        std::cout << "Uso: " << argv[0] << " <arquivo>" << std::endl;
        return 1;
    }

    // Vetor de vertices
    std::vector<Vertice> vertices;
    // Vetor de arestas
    std::vector<std::pair<int, int>> arestas;

    // Le o arquivo
    leGrafo(argv[1], vertices);

    // Extrai as arestas
    extraiArestas(vertices, arestas);

    // Imprime os vertices
    for(unsigned int i=0; i < vertices.size(); i++){
        std::cout << "Vertice " << i + 1 << ": (" << vertices[i].x << ", " << vertices[i].y << ")" << std::endl;
        std::cout << "Grau: " << vertices[i].grau << std::endl;
        std::cout << "Vizinhos: ";
        for(int j=0; j < vertices[i].grau; j++){
            std::cout << vertices[i].vizinhos[j] << " ";
        }
        std::cout << std::endl << std::endl;
    }

    // Imprime as arestas
    for(unsigned int i=0; i < arestas.size(); i++){
        std::cout << "Aresta " << i << ": (" << arestas[i].first << ", " << arestas[i].second << ")" << std::endl;
    }

    // Encontra as faces - Ainda não funciona
    encontraFaces(vertices, arestas);

    return 0;
}