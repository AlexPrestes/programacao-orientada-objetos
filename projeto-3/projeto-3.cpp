#include <algorithm>
#include <iostream>
#include <vector>
#include <typeinfo>


struct LimiteExcedido {
    int val;
};

// Classe que mantem um conjunto de valores sem duplicacao e em ordem crescente.
// Permite verificar a existencia ou nao de um valor e pegar uma faixa de
// elementos entre dois valores especificados.
class OrderedUniqueValues {
  // Invariante:
  // Se size() > 1 && 0 <= i < size()-1 então _data[i] < data[i+1]
  std::vector<int> _data;

public:
  // Sinonimmo de um tipo para iterador para os elementos.
  using const_iterator = std::vector<int>::const_iterator;

  // Verifica se um elementos com o dado valor foi inserido.
  bool find(int value) {
    // Como os dados estao ordenados em _data, entao basta fazer uma busca
    // binaria.
    return std::binary_search(begin(_data), end(_data), value);
  }

  // Retorna um par de iteradores para o primeiro e um depois do ultimo
  // valores que sao maiores ou iguais a min_value e menores ou iguais a
  // max_value.
  std::pair<const_iterator, const_iterator> find_range(int min_value,
                                                       int max_value) const {
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
  virtual void insert(int value) {
    auto [first, last] = std::equal_range(begin(_data), end(_data), value);
    if (first == last) {
      _data.insert(last, value);
    }
  }
};


// Classe derivada de OrderedUniqueValues
// Permite instanciar uma classe OrderedUniqueValues com tamanho limitado
class LimitedOrderedUniqueValues : public OrderedUniqueValues {
    size_t _limit;

public:
    LimitedOrderedUniqueValues(int limit) {
        _limit = (limit > 0) ? limit : 0;
    }

    void insert(int value) override {
        if (size() < _limit) {
            OrderedUniqueValues::insert(value);
        } else {
            throw LimiteExcedido{value};
        }
    }
};

int main(int, char *[]) {
  // Alguns testes simples.
  std::vector<int>   some_values{7, -10, 4, 8, -2, 9, -10, 8, -5, 6, -9,  5};
  std::vector<size_t> some_sizes{1,   2, 3, 4,  5, 6,   6, 6,  7, 8,  9, 10};
  

  std::cout << "Inicio testes classe" << std::endl;
  OrderedUniqueValues ouv;
  for (size_t i = 0; i < some_values.size(); ++i) {
    ouv.insert(some_values[i]);
    if (ouv.size() != some_sizes[i]) {
      std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_values[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << ouv.size() << std::endl;
    }
  }

  for (auto x : some_values) {
    if (!ouv.find(x)) {
      std::cerr << "Nao achou valor inserido " << x << std::endl;
    }
  }

  auto [first1, last1] = ouv.find_range(0, 9);
  for (auto current = first1; current != last1; ++current) {
    if (*current < 0) {
      std::cerr << "Erro na selecao dos valores nao-negativos: " << *current
                << std::endl;
    }
  }
  auto [first2, last2] = ouv.find_range(-10, 0);
  for (auto current = first2; current != last2; ++current) {
    if (*current >= 0) {
      std::cerr << "Erro na selecao dos valores negativos: " << *current
                << std::endl;
    }
  }

  std::cout << "Fim testes classe" << std::endl;

  //################################
  //# Teste para a classe derivada #
  //################################
  
  std::cout << "Inicio testes classe derivada" << std::endl;
  LimitedOrderedUniqueValues louv{10};

  for (size_t i = 0; i < some_values.size(); ++i) {
    louv.insert(some_values[i]);
    if (louv.size() != some_sizes[i]) {
      std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_values[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << louv.size() << std::endl;
    }
  }

  for (auto x : some_values) {
    if (!louv.find(x)) {
      std::cerr << "Nao achou valor inserido " << x << std::endl;
    }
  }

  auto [lfirst1, llast1] = louv.find_range(0, 9);
  for (auto lcurrent = lfirst1; lcurrent != llast1; ++lcurrent) {
    if (*lcurrent < 0) {
      std::cerr << "Erro na selecao dos valores nao-negativos: " << *lcurrent
                << std::endl;
    }
  }
  auto [lfirst2, llast2] = louv.find_range(-10, 0);
  for (auto lcurrent = lfirst2; lcurrent != llast2; ++lcurrent) {
    if (*lcurrent >= 0) {
      std::cerr << "Erro na selecao dos valores negativos: " << *lcurrent
                << std::endl;
    }
  }
  std::cout << "Fim testes classe derivada" << std::endl;
  
  //######################
  //# teste polimorfismo #
  //######################

  std::cout << "Inicio teste polimorfismo" << std::endl;

  OrderedUniqueValues *ouv1 = new OrderedUniqueValues;
  OrderedUniqueValues *ouv2 = new LimitedOrderedUniqueValues(10);

  for (size_t i = 0; i < some_values.size(); ++i) {
    ouv1->insert(some_values[i]);
    if (ouv1->size() != some_sizes[i]) {
      std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_values[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << ouv1->size() << std::endl;
    }
  }
  for (size_t i = 0; i < some_values.size(); ++i) {
    ouv2->insert(some_values[i]);
    if (ouv2->size() != some_sizes[i]) {
      std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_values[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << ouv2->size() << std::endl;
    }
  }
  std::cout << "Fim teste polimorfismo" << std::endl;
  
  //####################
  //# teste da exceção #
  //####################

  std::cout << "Inicio teste exceção" << std::endl;

  try {
    LimitedOrderedUniqueValues louv{9};
    for (size_t i = 0; i < some_values.size(); ++i) {
      louv.insert(some_values[i]);
      if (louv.size() != some_sizes[i]) {
        std::cerr << "Erro de insercao: indice " << i
                << ", valor: " << some_values[i]
                << ", tamanho esperado: " << some_sizes[i]
                << ", tamanho obtido: " << louv.size() << std::endl;
      }
    }
  }
  catch (const LimiteExcedido& e) {
    std::cerr << "Valor " << e.val << " fora do limite." << std::endl;
  }

  std::cout << "Fim teste exceção" << std::endl;

  return 0;
}
