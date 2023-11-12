// Lucas Albano Olive Cruz - 2022036209

#include <iostream>
#include <string>
#include <vector>

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
};

// Resolve o problema
std::vector<int> resolve(std::vector<Section> sections, std::vector<Maneuver> maneuvers){
    std::vector<int> result;

    return result;
}

// Imprime as seções e manobras
void print(std::vector<Section> sections, std::vector<Maneuver> maneuvers){
    for(long unsigned int i = 0; i < sections.size(); i++){
        std::cout << "== Seção " << i+1 << " ==" << std::endl;
        std::cout << "Bônus: " << sections[i].getBonus() << std::endl;
        std::cout << "Tempo: " << sections[i].getTime() << std::endl;
    }
    
    for(long unsigned int i = 0; i < maneuvers.size(); i++){
        std::cout << "== Manobra " << i+1 << " ==" << std::endl;
        std::cout << "Pontos: " << maneuvers[i].getPoints() << std::endl;
        std::cout << "Tempo: " << maneuvers[i].getTime() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    // Número de seções e manobras
    int nsections, nmaneuvers;
    scanf("%d %d", &nsections, &nmaneuvers);

    // Vetor de seções
    std::vector<Section> sections(nsections);

    for(int i = 0; i < nsections; i++){
        int bonus, time;
        scanf("%d %d", &bonus, &time);
        sections[i] = Section(bonus, time);
    }

    // Vetor de manobras
    std::vector<Maneuver> maneuvers(nmaneuvers);

    for(int i = 0; i < nmaneuvers; i++){
        int points, time;
        scanf("%d %d", &points, &time);
        maneuvers[i] = Maneuver(points, time);
    }

    print(sections, maneuvers);





    // Considerar o índice começando em 1 para a saída

    // Pontuação Total Máxima

    // Numero de manobras na seção + índice da manobra

    return 0;
}