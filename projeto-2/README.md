# Segundo Trabalho: Cálculo de velocidades com erros
O código anexo é um programa em C++ que lê informações de  tempo e posição (altura) de um experimento de corpo em queda livre presentes em um arquivo e avalia a aceleração gravitacional e as velocidades em cada instante. As informações de tempo e altura são apresentadas com o erro experimental, e o programa calcula a propagação de erros considerando que os erros são independentes e distribuidos gaussianamente.

Infelizmente, o código está implementado sem uso de encapsulação e sem uso de sobrecarga de operadores.

O seu trabalho é entregar uma nova versão desse código onde a struct Measurement seja transformada numa class Measurement com encapsulação de seus membros, e as operações aritméticas sobre objetos do tipo Measurement sejam feitas por meio de sobrecarga de operadores.
