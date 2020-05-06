#include <algorithm>  // Para std::sort
#include <fstream>    // Para std::ifstream
#include <functional> // Para std::function
#include <iostream>   // Para std::cout, std::endl
#include <string>     // Para std::string, std::stod
#include <vector>     // Para std::vector
#include <cmath>      // Para std::pow, std::sqrt

//=====================================================================
//
// Prototipo das funções usadas no main
//
//=====================================================================

// Função que lê os dados double do arquivo.
std::vector<double> le_arquivo(std::string nome_arquivo);

// Função que recebe vetor de dados e retorna média e desvio padrão
std::tuple<double,double> calcula_media_desvio(std::vector<double> valores);

// Função que monta o histograma e retorna o vetor informação e o vetor de contagem
std::tuple<std::vector<double>,std::vector<int>> monta_histograma(std::vector<double> valores, int B);

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
    auto valores = le_arquivo(nome_arquivo);

    // calcula média e desvio padrão do vetor.
    auto [media, desvio] = calcula_media_desvio(valores);
    
    // Cria um vetor com as caixas do histograma.
    auto [vetinfo, vetcont] = monta_histograma(valores, B);
    
    // Mostra a contagem de elementes
    std::cout << valores.size() << std::endl;

    // Mostra média e desvio padrão
    std::cout << media << std::endl;
    std::cout << desvio << std::endl;

    // Mostra a caixa e contagem do histograma
    for (int i = 0 ; i < B; i++) {
        std::cout << vetinfo[i] << ' ';
        std::cout << vetinfo[i+1] << ' ';
        std::cout << vetcont[i] << std::endl;
    }
}

//=====================================================================
//
// Funções auxiliares
//
//=====================================================================

// Função le_arquivo recebe o nome do arquivo e
// retorn um vetor double com todos os valores.
std::vector<double> le_arquivo(std::string nome_arquivo) {
    // importa o arquivo
    std::ifstream arquivo(nome_arquivo);
    // variavel de saida da funcao
    std::vector<double> valores;
    // variavel que recebe texto do arquivo
    std::string valor;

    // loop enquanto estiver lendo o arquivo
    while ( arquivo.good() && !arquivo.eof() ) {
        // passa para valor o texto seguinte.
        arquivo >> valor;
        // verifica se ainda tem texto
        if ( arquivo.good() ) {
            // converte a string para double e coloca no vetor.
            valores.push_back(std::stod(valor));
        }
    }
    // vetor de saida.
    return valores;
}

// Função que cálcula média e desvio padrão do vetor de entrada.
std::tuple<double,double> calcula_media_desvio(std::vector<double> valores) {
    double media{0}, desvio{0};
    // loop soma o vetor valores
    for (auto valor : valores) {
        media += valor;
    }
    // divide pela quantidade de elementos
    media /= valores.size();

    // calcula desvio padrão
    for (auto valor : valores) {
        desvio += std::pow(valor - media, 2);
    }
    desvio = desvio / (valores.size() - 1);
    desvio = std::sqrt(desvio);

    // tupla de saida
    return {media, desvio};
}

// Função que monta o histograma do vetor de entrada em um numero B de caixas
// e retorna um tupla com o vetor informação e o vetor contagem.
// vetor informação: valores de intervalo das caixas, tamanho: B+1.
// vetor contagem: quantidade de elemento em cada caixa.
std::tuple<std::vector<double>,std::vector<int>> monta_histograma(std::vector<double> valores, int B) {
    // vetor de informação
    std::vector<double> vetinformacao;
    // vetor de contagem
    std::vector<int> vetcontagem(B, {0});
    // variaveis auxiliares
    double xmin, xmax, delta;
    int n;
    // ordem o vetor valores em ordem crescente
    std::sort(valores.begin(), valores.end());
    // menor, maior valor do vetor valores
    xmin = valores[0];
    xmax = valores[valores.size()-1];
    // calculo delta da caixa
    delta = (xmax - xmin)/B;
    // loop preenche o vetor de informações
    for (int i = 0; i <= B; i++) {
        vetinformacao.push_back(xmin + i*delta);
    }
    // loop preenche o velor de contagem
    for (auto valor : valores) {
        n = (int)( (valor-xmin) / delta);
        if ( valor == xmax) { n = B-1; }
        vetcontagem[n] += 1;
    }
    // tupla de saida
    return {vetinformacao, vetcontagem};
}
