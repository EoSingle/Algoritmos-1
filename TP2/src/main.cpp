// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <limits>
#include <cstdio>

class Aresta {
    private:
        long int destino, ano, tempo, custo;

    public:
        Aresta(long int _d, long int _a, long int _t, long int _c) : 
        destino(_d), ano(_a), tempo(_t), custo(_c) {}

        long int getDestino() const { return this->destino; }
        long int getAno()     const { return this->ano; }
        long int getTempo()   const { return this->tempo; }
        long int getCusto()   const { return this->custo; }

        bool operator > (const Aresta& other) const {
            return this->tempo > other.getTempo();
        }

        bool operator < (const Aresta& other) const {
            return this->ano > other.getAno();
        }

};

class ComparaCusto {
    public:
        bool operator()(const Aresta& a, const Aresta& b) {
            return a.getCusto() > b.getCusto();
        }
};

class Grafo {
    private:
        int nvertices, narestas;
        std::list<Aresta> * adj;

    public:
        Grafo(int _nvertices, int _narestas){
            this->nvertices = _nvertices;
            this->narestas = _narestas;

            adj = new std::list<Aresta>[nvertices];
        }

        void addAresta(long int u, long int v, long int a, long int t, long int c){
            adj[u-1].push_back(Aresta(v,a,t,c));
            adj[v-1].push_back(Aresta(u,a,t,c));
        }

        void print() const {
            std::cout << "Origem | Destino | Ano | Tempo | Custo" << std::endl;
            for(int i = 0; i < nvertices; i++){
                for(const Aresta& aresta : adj[i]){
                    std::cout << i+1 << " " << aresta.getDestino() << " " << aresta.getAno() << " " <<
                    aresta.getTempo() << " " << aresta.getCusto() << std::endl;  
                }
            }
        }

        void dijkstra(int origem) {
            std::vector<long int> distancia(nvertices, std::numeric_limits<long int>::max());
            std::vector<bool> visitado(nvertices, false);
            std::priority_queue<Aresta, std::vector<Aresta>, std::greater<Aresta>> filaPrioridade;
            std::vector<long int> ano(nvertices, 0);

            distancia[origem] = 0;
            filaPrioridade.push({origem, 0, 0, 0});

            while (!filaPrioridade.empty()) {
                Aresta aresta = filaPrioridade.top();
                filaPrioridade.pop();

                long int u = aresta.getDestino();

                if (visitado[u]) continue;
                visitado[u] = true;

                for (const auto& vizinho : adj[u]) {
                    long int v = vizinho.getDestino() - 1;
                    long int tempo = aresta.getTempo() + vizinho.getTempo();

                    if (tempo < distancia[v]) {
                        distancia[v] = tempo;
                        ano[v] = vizinho.getAno();
                        filaPrioridade.push({v, vizinho.getAno(), tempo, vizinho.getCusto()});
                    }
                }
            }

            long int maior = 0;
            for (int i = 0; i < nvertices; ++i) {
                std::cout << distancia[i] << std::endl;
                if(ano[i] > maior) maior = ano[i];
            }
            
            std::cout << maior << std::endl;
        }

        void anoMST(int origem) {
            std::vector<bool> visitado(nvertices, false);
            std::priority_queue<Aresta, std::vector<Aresta>> filaPrioridade;
            std::vector<long int> anos(nvertices, std::numeric_limits<long int>::max());
            
            anos[origem] = 0;
            filaPrioridade.push({origem, 0, 0, 0});

            while(!filaPrioridade.empty()){
                int u = filaPrioridade.top().getDestino();
                filaPrioridade.pop();

                if(visitado[u]) continue;
                visitado[u] = true;

                for(const Aresta& aresta : adj[u]){
                    int v = aresta.getDestino() - 1;
                    long int ano = aresta.getAno();

                    if(!visitado[v] && ano < anos[v]){
                        anos[v] = ano;
                        filaPrioridade.push({v, ano, aresta.getTempo(), aresta.getCusto()});
                    }
                }
            }

            long int maior = 0;
            for (int i = 0; i < nvertices; i++) {
                if(anos[i] > maior) maior = anos[i];
            }

            std::cout << maior << std::endl;
        }

        void custoMST(int origem) {
            long int custoTotal = 0;
            std::vector<bool> visitado(nvertices, false);
            std::priority_queue<Aresta, std::vector<Aresta>, ComparaCusto> filaPrioridade;

            filaPrioridade.push({origem, 0, 0, 0});

            while (!filaPrioridade.empty()) {
                Aresta aresta = filaPrioridade.top();
                filaPrioridade.pop();

                long int u = aresta.getDestino();

                if (visitado[u]) {
                    continue;
                }

                visitado[u] = true;

                custoTotal += aresta.getCusto();

                for (const auto& adjacente : adj[u]) {
                    long int v = adjacente.getDestino() - 1;
                    if (!visitado[v]) {
                        filaPrioridade.push({v, adjacente.getAno(), adjacente.getTempo(), adjacente.getCusto()});
                    }
                }
            }
            std::cout << custoTotal << std::endl;
        }     
};

int main(int argc, char* argv[]) {
    int nvilas, nestradas;
    scanf("%d %d", &nvilas, &nestradas);

    Grafo G(nvilas, nestradas);

    for (int i = 0; i < nestradas; i++) {
        long int u, v, a, t, c;
        scanf("%ld %ld %ld %ld %ld", &u, &v, &a, &t, &c);
        G.addAresta(u, v, a, t, c);
    }

    // G.print();

    G.dijkstra(0);
    G.anoMST(0);
    G.custoMST(0);

    return 0;
}
