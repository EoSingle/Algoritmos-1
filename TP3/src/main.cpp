// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <bitset>

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

    public:
        // Construtores
        Solver(const std::vector<Section> &_sections, const std::vector<Maneuver> &_maneuvers) : sections(_sections), maneuvers(_maneuvers) {
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

        // Calcula a pontuação de uma combinação dado a seção anterior
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

        /* 
        Calcula a pontuação máxima
        F(i, m) = max[m' in M]{F(i+1, m') + pontos(m'|m) * bonus(i)}
        M:         conjunto de manobras
        m' in M:   uma combinação de manobras
        i =        índice da seção
        pontos(m'|m): pontos da manobra de m' dado m na seção i - 1
        bonus(i):  bônus da seção i
        */
        int calculateMaxScore(int section = -1, int combination = 0, int lastcombination = 0){
            // Se chegou na última seção
            if(section == this->nsections){
                return 0;
            }

            // Combinação ultrapassa o tempo da seção
            if(this->combinations[combination].getTime() > this->sections[section].getTime()){
                return 0;
            }

            // Recursão /!\ Logical Error
            int maxScore = 0;
            for(unsigned int i = 0; i < this->combinations.size(); i++){
                int score = calculateMaxScore(section + 1, i, combination);
                score += points(this->combinations[i], this->combinations[lastcombination]) * this->sections[section].getBonus();
                if(score > maxScore){
                    maxScore = score;
                }
            }

            return maxScore;
        }

        // Resolve o problema
        int solve(){
            // Pre-calcula as combinações de manobras
            generateCombinatios();

            // Imprime a memoização
            printCombinations();

            // Calcula a pontuação máxima
            int score = calculateMaxScore();

            return score;
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
    print(sections, maneuvers);

    // Instancia o solver
    Solver solver(sections, maneuvers);

    // Pontuação Total Máxima
    int maxScore = solver.solve();

    std::cout << "Pontuação Total Máxima: " << maxScore << std::endl;

    return 0;
}
