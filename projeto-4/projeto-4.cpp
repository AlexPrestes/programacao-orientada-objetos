#include <algorithm>
#include <iostream>
#include <vector>
#include <typeinfo>


template<typename T>
struct LimiteExcedido {
    T val;
};

// Classe que mantem um conjunto de valores sem duplicacao e em ordem crescente.
// Permite verificar a existencia ou nao de um valor e pegar uma faixa de
// elementos entre dois valores especificados.
template<typename T>
class OrderedUniqueValues : public std::vector<T> {
  // Invariante:
  // Se size() > 1 && 0 <= i < size()-1 então _data[i] < data[i+1]
  std::vector<T> _data;

public:
  // Sinonimmo de um tipo para iterador para os elementos.
  using const_iterator = typename std::vector<T>::const_iterator;

  // Verifica se um elementos com o dado valor foi inserido.
  bool find(T value) {
    // Como os dados estao ordenados em _data, entao basta fazer uma busca
    // binaria.
    return std::binary_search(begin(_data), end(_data), value);
  }

  // Retorna um par de iteradores para o primeiro e um depois do ultimo
  // valores que sao maiores ou iguais a min_value e menores ou iguais a
  // max_value.
  std::pair<const_iterator, const_iterator> find_range(T min_value,
                                                       T max_value) const {
    // Dados ordenados em _data, entao podemos usar lower_bound e upper_bound.
    // Encontra o primeiro elemento que tem valor maior ou igual a min_value.
    auto first = std::lower_bound(begin(_data), end(_data), min_value);
    // Encontra o primeiro elemento que tem valor maior do que max_value.
    auto last = std::upper_bound(begin(_data), end(_data), max_value);
    return {first, last};
  }

  // Numero de elementos correntemente armazenados.
  size_t size() const { return _data.size(); }

  // Insere um novo elemento, se nao existir ainda.
  virtual void insert(T value) {
    auto [first, last] = std::equal_range(begin(_data), end(_data), value);
    if (first == last) {
      _data.insert(last, value);
    }
  }
};


// Classe derivada de OrderedUniqueValues
// Permite instanciar uma classe OrderedUniqueValues com tamanho limitado
template<class T>
class LimitedOrderedUniqueValues : public OrderedUniqueValues<T> {
    size_t _limit;

public:
    LimitedOrderedUniqueValues(int limit) {
        _limit = (limit >= 0) ? limit : 0;
    }
    
    void insert(T value) override {
        if (this->size() < _limit) {
            OrderedUniqueValues<T>::insert(value);
        } else {
            throw LimiteExcedido<T>{value};
        }
    }
};

int main(int, char *[]) {
  // Alguns testes simples.
  std::vector<int>   some_values{ 7 , -10,  4 ,  8 , -2 ,  9 , -10,  8 , -5 ,  6 , -9 ,  5 , 200};
  std::vector<char>   some_chars{'j', 'a', 'b', 'g', 'v', 'k', 'g', 'b', 'h', 'i', 'o', 'p', 'l'};
  std::vector<size_t> some_sizes{ 1 ,  2 ,  3 ,  4 ,  5 ,  6 ,  6 ,  6 ,  7 ,  8 ,  9 , 10 , 11 };
  

  // ########################
  // # Teste para vetor int #
  // ########################

  std::cout << "Teste: int" << std::endl;

  LimitedOrderedUniqueValues<int> louvi(10);


  try {
      
  for (size_t i = 0; i < some_values.size(); ++i) {
    louvi.insert(some_values[i]);
    if (louvi.size() != some_sizes[i]) {
      std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_values[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << louvi.size() << std::endl;
    }
  }
  } catch (LimiteExcedido<int> e) {
    std::cout << "Limite excedido" << std::endl;
    std::cout << "Valor "<< e.val << " não foi inserido." << std::endl;
  }

  for (auto x : some_values) {
    if (!louvi.find(x)) {
      std::cerr << "Nao achou valor inserido " << x << std::endl;
    }
  }

  auto [first1i, last1i] = louvi.find_range(0, 10);
  for (auto current = first1i; current != last1i; ++current) {
    if (*current < 0) {
      std::cerr << "Erro na selecao dos valores nao-negativos: " << *current
                << std::endl;
    }
  }
  auto [first2i, last2i] = louvi.find_range(-10, 0);
  for (auto current = first2i; current != last2i; ++current) {
    if (*current >= 0) {
      std::cerr << "Erro na selecao dos valores negativos: " << *current
                << std::endl;
    }
  }
 
  // ########################
  // # Teste para vetor char #
  // ########################

  std::cout << "Teste: char" << std::endl;

  LimitedOrderedUniqueValues<char> louvc(10);

  try {
      
  for (size_t i = 0; i < some_chars.size(); ++i) {
    louvc.insert(some_chars[i]);
    if (louvc.size() != some_sizes[i]) {
      std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_chars[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << louvc.size() << std::endl;
    }
  }
  } catch (LimiteExcedido<char> e) {
    std::cout << "Limite excedido" << std::endl;
    std::cout << "Valor "<< e.val << " não foi inserido." << std::endl;
  }

  for (auto x : some_chars) {
    if (!louvc.find(x)) {
      std::cerr << "Nao achou valor inserido " << x << std::endl;
    }
  }

  auto [first1c, last1c] = louvc.find_range(0, 9);
  for (auto current = first1c; current != last1c; ++current) {
    if (*current < 0) {
      std::cerr << "Erro na selecao dos valores nao-negativos: " << *current
                << std::endl;
    }
  }
  auto [first2c, last2c] = louvc.find_range(-10, 0);
  for (auto current = first2c; current != last2c; ++current) {
    if (*current >= 0) {
      std::cerr << "Erro na selecao dos valores negativos: " << *current
                << std::endl;
    }
  }
  return 0;
}
