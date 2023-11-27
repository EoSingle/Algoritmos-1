// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <bitset>
#include <unordered_set>
#include <functional>


#define MAX_SECTIONS 100
#define MAX_MANEUVERS 10

// Classe que representa uma seção
class Section{
    private:
        // Bonificação e tempo para percorrer a seção
        int bonus, time;

    public:
        // Construtores
        Section() : bonus(0), time(0) {}
        Section(int _bonus, int _time) : bonus(_bonus), time(_time) {}

        // Getters
        int getBonus() const { return this->bonus; }
        int getTime()  const { return this->time; }
};

// Classe que representa uma manobra
class Maneuver{
    private:
        // Pontos pela manobra e tempo para realizar a manobra
        int points, time;

    public:
        // Construtores
        Maneuver() : points(0), time(0) {}
        Maneuver(int _points, int _time) : points(_points), time(_time) {}

        // Getters
        int getPoints() const { return this->points; }
        int getTime()   const { return this->time; }

        // Operador de comparação
        bool operator==(const Maneuver& other) const {
            return (this->points == other.points) && (this->time == other.time);
        }
};

namespace std {
    template <>
    struct hash<Maneuver> {
        std::size_t operator()(const Maneuver& maneuver) const {
            // Use combinações dos hashes dos atributos points e time para calcular o hash.
            std::size_t hashPoints = std::hash<int>{}(maneuver.getPoints());
            std::size_t hashTime = std::hash<int>{}(maneuver.getTime());

            // Combinação dos hashes usando operações bit a bit.
            return hashPoints ^ (hashTime + 0x9e3779b9 + (hashPoints << 6) + (hashPoints >> 2));
        }
    };
}

// Classe que representa uma combinação de manobras
class Combination{
    private:
        // Vetor de manobras
        std::vector<Maneuver> maneuvers;
        int score;
        int time;

    public:
        // Construtores
        Combination() : maneuvers(std::vector<Maneuver>()), score(0), time(0) {}
        Combination(std::vector<Maneuver> _maneuvers) : maneuvers(_maneuvers), score(0), time(0) {
            calculateScore();
            calculateTime();
        }

        // Getters
        std::vector<Maneuver> getManeuvers() const { return this->maneuvers; }
        int getScore() const { return this->score; }
        int getTime() const { return this->time; }

        // Calcula a pontuação da combinação
        void calculateScore(){
            this->score = 0;
            for(long unsigned int i = 0; i < maneuvers.size(); i++){
                this->score += maneuvers[i].getPoints();
            }
        }

        // Calcula o tempo da combinação
        void calculateTime(){
            this->time = 0;
            for(long unsigned int i = 0; i < maneuvers.size(); i++){
                this->time += maneuvers[i].getTime();
            }
        }
};

class Solver{
    private:
        // Vetor de seções e manobras
        std::vector<Section> sections;
        std::vector<Maneuver> maneuvers;
        std::vector<Combination> combinations;

        // Número de seções e manobras
        int nsections, nmaneuvers;

        // Memoização
        std::vector<std::vector<std::vector<int>>> memoization;


    public:
        // Construtores
        Solver(const std::vector<Section> &_sections, const std::vector<Maneuver> &_maneuvers) 
        : sections(_sections), maneuvers(_maneuvers) {
            this->nsections = sections.size();
            this->nmaneuvers = maneuvers.size();

        }

        // Gera todas as combinações de manobras
        void generateCombinatios(){
            for(int i=0; i < (1 << this->nmaneuvers); i++){
                std::vector<Maneuver> combination;
                for(int j=0; j < this->nmaneuvers; j++){
                    if(i & (1 << j)){
                        combination.push_back(maneuvers[j]);
                    }
                }
                combinations.push_back(Combination(combination));
            }
        }

        // Imprime as combinações de manobras
        void printCombinations(){
            for(long unsigned int i = 0; i < combinations.size(); i++){
                std::cout << "== Combinação " << i+1 << " ==" << std::endl;
                std::cout << "Pontuação: " << combinations[i].getScore() << std::endl;
                std::cout << "Tempo: " << combinations[i].getTime() << std::endl;
                std::cout << "Manobras: ";
                for(long unsigned int j = 0; j < combinations[i].getManeuvers().size(); j++){
                    std::cout << combinations[i].getManeuvers()[j].getPoints() << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        // Imprime as combinações de manobras que geram a pontuação máxima
        // Para cada seção, imprime o tamanho e a combinação que gerou a pontuação máxima
        void printCombinationsMaxScore(){
            for(int i=0; i < nsections; i++){
                int maxScore = 0;
                int maxCombination = 0;
                for(long unsigned int j = 0; j < combinations.size(); j++){
                    if(memoization[i][j][0] > maxScore){
                        maxScore = memoization[i][j][0];
                        maxCombination = j;
                    }
                }
                std::cout << combinations[maxCombination].getManeuvers().size() << " ";
                
                // Encontra os índices em maneuvers que correspondem às manobras da combinação
                std::vector<int> maneuversIndexes;
                for (const auto& maneuver : combinations[maxCombination].getManeuvers()) {
                    for (long unsigned int j = 0; j < maneuvers.size(); ++j) {
                        if (maneuver == maneuvers[j]) {
                            maneuversIndexes.push_back(j);
                            break;
                        }
                    }
                }

                // Imprime os índices das manobras - offset de 1 para imprimir os índices começando em 1
                for (long unsigned int j = 0; j < maneuversIndexes.size(); ++j) {
                    std::cout << maneuversIndexes[j] + 1 << " ";
                }

                std::cout << std::endl;
                
            }
        }

        // Calcula a pontuação de uma combinação dado a seção anterior - Versão 1
        int points(Combination Current, Combination Previous){
            int score = 0;
            for(long unsigned int i = 0; i < Current.getManeuvers().size(); i++){
                bool found = false;
                for(long unsigned int j = 0; j < Previous.getManeuvers().size(); j++){
                    if(Current.getManeuvers()[i] == Previous.getManeuvers()[j]){
                        found = true;
                        break;
                    }
                }
                score += (found ? floor(Current.getManeuvers()[i].getPoints() * 0.5) : Current.getManeuvers()[i].getPoints());
            }
            return score;
        }

        // Calcula a pontuação de uma combinação dado a seção anterior - Versão 2
        int points2(const Combination& Current, const Combination& Previous) {
            int score = 0;

            // Criar um conjunto para armazenar as manobras anteriores para verificação de existência.
            std::unordered_set<Maneuver> previousManeuversSet;
            for (const auto& maneuver : Previous.getManeuvers()) {
                previousManeuversSet.insert(maneuver);
            }

            // Iterar sobre as manobras atuais e calcular a pontuação.
            for (const auto& currentManeuver : Current.getManeuvers()) {
                bool found = previousManeuversSet.find(currentManeuver) != previousManeuversSet.end();
                score += (found ? static_cast<int>(std::floor(currentManeuver.getPoints() * 0.5)) : currentManeuver.getPoints());
            }

            return score;
        }
 
        // Calcula a pontuação máxima - Versão Recursiva
        int calculateMaxScore(int section = -1, int combination = 0, int lastcombination = 0){
            //std::cout << "F(" << section << ", " << combination << ", " << lastcombination << ")" << std::endl;
            // Verifica se a combinação já foi calculada
            if(section != -1){
                if(this->memoization[section][combination][lastcombination] != -1){
                    std::cout << "F(" << section << ", " << combination << ", " << lastcombination << ") = " << this->memoization[section][combination][lastcombination] << std::endl;
                    return this->memoization[section][combination][lastcombination];
                }
            }

            // Se chegou na última seção
            if(section == this->nsections){
                this->memoization[section][combination][lastcombination] = 0;
                return 0;
            }

            // Combinação ultrapassa o tempo da seção
            if(this->combinations[combination].getTime() > this->sections[section].getTime()){
                this->memoization[section][combination][lastcombination] = 0;
                return 0;
            }

            // Recursão
            int maxScore = 0;
            for(unsigned int i = 0; i < this->combinations.size(); i++){
                int score = calculateMaxScore(section + 1, i, combination);
                score += points2(this->combinations[combination], this->combinations[lastcombination]) * this->sections[section].getBonus() * this->combinations[combination].getManeuvers().size();
                if(score > maxScore){
                    maxScore = score;
                }
            }

            // Memoização
            if(section != -1) this->memoization[section][combination][lastcombination] = maxScore;
            
            return maxScore;
        }

        // Calcula a pontuação máxima - Versão Iterativa
        int calculateMaxScore2() {
            memoization.resize(nsections + 1,
                            std::vector<std::vector<int>>(nmaneuvers, std::vector<int>(nmaneuvers, -1)));

            // Casos bases
            for (long unsigned int section = 0; section <= sections.size(); ++section) {
                for (long unsigned int combination = 0; combination < combinations.size(); ++combination) {
                    for (long unsigned int lastcombination = 0; lastcombination < combinations.size(); ++lastcombination) {
                        if (combinations[combination].getTime() > sections[section].getTime()) {
                            memoization[section][combination][lastcombination] = 0;
                        } else if (section == sections.size() ) {
                            memoization[section][combination][lastcombination] = 0;
                        }
                    }
                }
            }

            // Preenchimento da tabela de memoização de baixo para cima
            for (int section = nsections - 1; section >= 0; --section) {
                for (long unsigned int combination = 0; combination < this->combinations.size(); ++combination) {
                    for (long unsigned int lastcombination = 0; lastcombination < this->combinations.size(); ++lastcombination){
                        if (combinations[combination].getTime() > sections[section].getTime()) {
                            memoization[section][combination][lastcombination] = 0;
                        }
                        else {
                            int maxScore = 0;
                            // Percorre as combinações possíveis na próxima seção
                            for (long unsigned int i = 0; i < this->combinations.size(); ++i){
                                int score = memoization[section + 1][i][combination] + points2(combinations[combination], combinations[lastcombination]) * sections[section].getBonus() * combinations[combination].getManeuvers().size();

                                if (score > maxScore) {
                                    maxScore = score;
                                }
                            }
                            memoization[section][combination][lastcombination] = maxScore;
                        }
                    }
                }
            }

            // O resultado final é o maior valor da primeira seção
            int maxScore = 0;
            for (long unsigned int combination = 0; combination < this->combinations.size(); ++combination) {
                for (long unsigned int lastcombination = 0; lastcombination < this->combinations.size(); ++lastcombination) {
                    maxScore = std::max(maxScore, memoization[0][combination][lastcombination]);
                }
            }

            return maxScore;
            
        }

        // Resolve o problema
        void solve(){
            // Pre-calcula as combinações de manobras
            generateCombinatios();

            // Imprime as combinações de manobras
            //printCombinations();

            // Inicializa a memoização
            this->memoization = std::vector<std::vector<std::vector<int>>>(this->nsections + 1, std::vector<std::vector<int>>(this->combinations.size(), std::vector<int>(this->combinations.size(), -1)));

            // Calcula a pontuação máxima
            int score = calculateMaxScore2();

            // Imprime a pontuação máxima
            std::cout << score << std::endl;

            // Imprime as combinações de manobras que geram a pontuação máxima
            printCombinationsMaxScore();
        }
};

// Imprime as seções e manobras
void print(std::vector<Section> sections, std::vector<Maneuver> maneuvers){
    for(long unsigned int i = 0; i < sections.size(); i++){
        std::cout << "== Seção " << i+1 << " ==" << std::endl;
        std::cout << "Bônus: " << sections[i].getBonus() << std::endl;
        std::cout << "Tempo: " << sections[i].getTime() << std::endl;
    }
    std::cout << std::endl;

    for(long unsigned int i = 0; i < maneuvers.size(); i++){
        std::cout << "== Manobra " << i+1 << " ==" << std::endl;
        std::cout << "Pontos: " << maneuvers[i].getPoints() << std::endl;
        std::cout << "Tempo: " << maneuvers[i].getTime() << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    // Número de seções e manobras
    int nsections, nmaneuvers;
    scanf("%d %d", &nsections, &nmaneuvers);

    // Vetor de seções
    std::vector<Section> sections(nsections);

    // Lê as seções
    for(int i = 0; i < nsections; i++){
        int bonus, time;
        scanf("%d %d", &bonus, &time);
        sections[i] = Section(bonus, time);
    }

    // Vetor de manobras
    std::vector<Maneuver> maneuvers(nmaneuvers);

    // Lê as manobras
    for(int i = 0; i < nmaneuvers; i++){
        int points, time;
        scanf("%d %d", &points, &time);
        maneuvers[i] = Maneuver(points, time);
    }

    // Imprime as seções e manobras
    //print(sections, maneuvers);

    // Instancia o solver
    Solver solver(sections, maneuvers);

    // Resolve o problema
    solver.solve();

    return 0;
}
