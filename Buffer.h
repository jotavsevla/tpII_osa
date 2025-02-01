#ifndef BUFFER_H
#define BUFFER_H

#include <fstream>
#include <string>
#include "Registro.h"

using namespace std;

class Buffer {
private:
    string nomeArquivo;
    fstream arquivo;
    string dadosBuffer;

public:
    // Construtor
    Buffer(const string& nome);

    // Métodos de manipulação de arquivo
    bool abrirLeitura();
    bool abrirEscrita();
    bool abrirLeituraBinaria();
    bool abrirEscritaBinaria();
    void fechar();

    // Métodos de manipulação de registros
    void escreverRegistroDelimitado(const Registro& reg);
    bool lerRegistroDelimitado(Registro& reg);
    void escreverRegistroDescritor(const Registro& reg);
    bool lerRegistroDescritor(Registro& reg);
};

#endif
