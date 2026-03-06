#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <random>
#include <algorithm>
#include <cctype>
#include <vector>
#include <fstream>

struct info_pers {
  int score;
  int nbre;
};


template<typename T>
int check_perdants(T joueurs) {
  int num {0};
  for (auto [nom, info] : joueurs) {
    if (info.score == 0)
      ++num;
  }
  return num;
}


int main() {
  std::cout << "Veux-tu:\n"
  << "1. Voir les gagnants précédents\n"
  << "2. Jouer\n";
  int choix;
  while (true) {
    if (std::cin>>choix) break;
    else {
      std::cout << "(Veuillez entrer un entier)\n"
        << "nombre de joueurs = ";
      std::cin.clear();
      std::cin.ignore(1000, '\n');
    }
  }
  if (choix==1) {
    std::ifstream fichier("liste_gagnants.txt");
    if (!fichier) std::cerr << "Impossible de consulter la base de données\n";
    std::string ligne;
    std::cout << "Liste des gagnants\n"
      << "-----------------------\n";
    while (std::getline(fichier, ligne))
      std::cout << ligne << '\n';
    fichier.close();
  }
  else if (choix==2) {
    bool ok {true};
    while (ok) {
      int nbre_joueurs;
      std::map<std::string, info_pers> joueurs;
      std::cout << "Bienvenue dans ce jeu\nCombien de"
      << "joueurs êtes-vous?\nnombre de joueurs = ";
      while (true) {
        if (std::cin >> nbre_joueurs) break;
        else {
          std::cout << "(Veuillez entrer un entier)\n"
            << "nombre de joueurs = ";
          std::cin.clear();
          std::cin.ignore(1000, '\n');
        }
      }
      for (int i=0; i<nbre_joueurs; ++i) {
        std::string name;
        std::cout << "Entrer un pseudo: ";
        std::cin >> name;
        joueurs[name] = {30, 0};
        std::cout << "Pseudo créé\n";
      }
      std::cout << "Tous les joueurs ont été enregistrés!"
      << "\nOn peut commencer.\n";
      while (1) {
        int nbre_perdants {check_perdants(joueurs)};
        if (nbre_perdants == nbre_joueurs-1)
          break;
        double resultat {0};
        for (auto& [nom, info] : joueurs) {
        if (info.score != 0) {
          std::cout << nom << ", entrer votre nombre: ";
  
          while (true) {
             if (std::cin >> info.nbre)
               break;
            else {
              std::cout << "Veuillez entrer un entier\n"
                << "Votre nombre: ";
              std::cin.clear();
              std::cin.ignore(1000, '\n');
            }
          }
  
          resultat += info.nbre; }
        }
        resultat /= (nbre_joueurs-nbre_perdants);
        resultat *= 0.8;
        std::cout << "Nous avons "<< resultat << ".\n";
        double min_abs_value {101.0};
        constexpr double EPS {0.001};
        double vabs;
        std::vector<std::string> gn_bataille;
        for (const auto& [nom, info] : joueurs) {
          if (info.score != 0) {
            vabs = std::fabs(info.nbre-resultat);
            if (std::fabs(vabs-min_abs_value) < EPS)
              gn_bataille.push_back(nom);
            else if (vabs < min_abs_value) {
              min_abs_value = vabs;
              gn_bataille.clear();
              gn_bataille.push_back(nom);
            }
          }
        }
        std::cout << "Ils sont: ";
        for (const auto& nom : gn_bataille) 
          std::cout << nom << "\n";
  
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(3, 10);
        int mallus {dist(rng)};
        for (auto& [nom, info] : joueurs) {
          if (std::find(gn_bataille.begin(), 
            gn_bataille.end(), nom) == gn_bataille.end()) {
            if (info.score < mallus)
              info.score = 0;
            else
              info.score -= mallus;
          }
        }
        for (const auto& [nom, info] : joueurs) {
          std::cout << "Nom: " << nom << ", Score: "
          << info.score << "\n"; 
        }
      }
      std::string gagnant;
      for (auto [nom, info] : joueurs)
        gagnant = (info.score !=0) ? nom : "nada"; 
      std::cout << "Le nom du vainqueur est: "<< gagnant<<"\n";
      std::ofstream fichier("liste_gagnants.txt", std::ios::app);
      if (!fichier) std::cerr << "Impossible d'ouvrir le fichier\n";
      for (const auto [nom, info] : joueurs) {
        if (info.score != 0) {
          fichier << nom << "......." << info.score;
        }
      }
      fichier.close();
      std::cout << "Base de données mise à jour avec succès\n"
        << "Voulez-vous jouer?\n>> ";
      std::string s;
      std::cin>>s;
      s.erase(std::remove_if(s.begin(),
        s.end(), ::isspace), s.end());
      std::transform(
        s.begin(), s.end(), s.begin(),
        [](unsigned char c){ return std::tolower(c); }
      );
      if (s == "non" || s == "no") {
        ok = false;
        std::cout << "Merci d'avoir joué";
      }
    }
  }
}
