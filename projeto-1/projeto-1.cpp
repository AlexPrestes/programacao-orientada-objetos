#include <algorithm>  // Para std::sort
#include <fstream>    // Para std::ifstream e std::getline
#include <functional> // Para std::function
#include <iomanip>    // Para std::setw
#include <iostream>   // Para std::cout, std::endl
#include <string>     // Para std::string, std::stod, std::stoi
#include <vector>     // Para std::vector
#include <cmath>      // Para std::pow

//=====================================================================
//
// Tipo de dados especiais
//
//=====================================================================

// Caixa do Histograma
struct Caixa {
    double inicio;   // x do inicio da caixa
    double fim;      // x do final da caixa
    int qtd;         // quantidade de elementos na caixa
};

//=====================================================================
//
// Prototipo das funções usadas no main
//
//=====================================================================

// Função que lê os dados double do arquivo.
std::vector<double> le_dados(std::string nome_arquivo);

// Função que recebe vetor double e mostra média e desvio padrão.
std::vector<double> mostra_media_desvio(std::vector<double> valores);

// Função que mostra o histograma.
std::vector<Caixa> mostra_histograma(std::vector<double> valores, int B);

//=====================================================================
//
// Programa principal
//
//=====================================================================

int main(int , char const *argv[]) {
    // recebe valores da linha de comando.
    auto nome_arquivo = argv[1];
    auto B = std::stoi(argv[2]);
    
    // chama a função que le os dados do arquivo.
    auto valores = le_dados(nome_arquivo);

    // calcula média e desvio padrão do arquivo.
    auto media_desvio = mostra_media_desvio(valores);

    for (auto& i : media_desvio) {
        std::cout << i << std::endl;
    }

    // Cria um vetor com as caixas do histograma.
    auto histograma = mostra_histograma(valores, B);

    for (auto& v : histograma) {
        std::cout << v.inicio << " ";
        std::cout << v.fim << " ";
        std::cout << v.qtd << std::endl;
    }
}

//=====================================================================
//
// Funções auxiliares
//
//=====================================================================

std::vector<double> le_dados(std::string nome_arquivo) {
    std::ifstream arquivo(nome_arquivo);
    std::vector<double> valores;

    for ( std::string linha; std::getline(arquivo, linha, ' '); ) {
        try {
            valores.push_back(std::stod(linha));
        } catch (...) {}
    }
    return valores;
}


std::vector<double> mostra_media_desvio(std::vector<double> valores) {
    std::vector<double> media_desvio;
    double media = 0, desvio = 0;

    media_desvio.push_back(valores.size());

    for (auto valor : valores) {
        media += valor;
    }
    media /= valores.size();

    media_desvio.push_back(media);

    for (auto valor : valores) {
        desvio += std::pow(valor - media, 2);
    }
    desvio = desvio / (valores.size() - 1);
    desvio = std::sqrt(desvio);

    media_desvio.push_back(desvio);

    return media_desvio;
}


std::vector<Caixa> mostra_histograma(std::vector<double> valores, int B) {
    
    std::vector<Caixa> histograma(B, {0,0,0});
    double xmin, xmax, delta;
    int n;
    std::sort(valores.begin(), valores.end());
    
    
    xmin = valores[0];
    xmax = valores[valores.size()-1];
    delta = (xmax - xmin)/B;

    for (int i = 0; i < B; i++) {
        histograma[i].inicio = xmin + i*delta;
        histograma[i].fim = xmin + (i+1)*delta;
    }
    
    for (auto valor : valores) {
        n = (int)( (valor-xmin) / delta);
        if ( valor == xmax) { n = B-1; }
        histograma[n].qtd += 1;
    }

    return histograma;
}
