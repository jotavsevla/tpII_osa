#include "Buffer.h"

Buffer::Buffer(const string& nome) : nomeArquivo(nome) {}

bool Buffer::abrirLeitura() {
    arquivo.open(nomeArquivo, ios::in);
    return arquivo.is_open();
}

bool Buffer::abrirEscrita() {
    arquivo.open(nomeArquivo, ios::out);
    return arquivo.is_open();
}

bool Buffer::abrirLeituraBinaria() {
    arquivo.open(nomeArquivo, ios::in | ios::binary);
    return arquivo.is_open();
}

bool Buffer::abrirEscritaBinaria() {
    arquivo.open(nomeArquivo, ios::out | ios::binary);
    return arquivo.is_open();
}

void Buffer::fechar() {
    if (arquivo.is_open()) {
        arquivo.close();
    }
}

void Buffer::escreverRegistroDelimitado(const Registro& reg) {
    arquivo << reg.packDelimitado();
}

bool Buffer::lerRegistroDelimitado(Registro& reg) {
    string linha;
    if (getline(arquivo, linha)) {
        reg.unpackDelimitado(linha);
        return true;
    }
    return false;
}

void Buffer::escreverRegistroDescritor(const Registro& reg) {
    string buffer = reg.packDescritor();
    arquivo.write(buffer.c_str(), buffer.size());
}

bool Buffer::lerRegistroDescritor(Registro& reg) {
    int tamanho;
    if (arquivo.read(reinterpret_cast<char*>(&tamanho), sizeof(int))) {
        string buffer(tamanho - sizeof(int), '\0');
        if (arquivo.read(&buffer[0], tamanho - sizeof(int))) {
            string completo;
            completo.resize(sizeof(int));
            memcpy(&completo[0], &tamanho, sizeof(int));
            completo += buffer;
            return reg.unpackDescritor(completo);
        }
    }
    return false;
}
