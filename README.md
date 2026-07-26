# TP II — Organização de Sistemas de Arquivos

Implementação acadêmica em C++ de conversão e leitura de **registros de tamanho
variável**. O programa lê uma base textual, representa cada linha como um
`Registro` e persiste a mesma informação em dois formatos:

1. campos delimitados por `|`;
2. registros precedidos por um descritor de tamanho binário.

O objetivo específico deste TP é comparar formas de empacotar e percorrer
registros em arquivo. O programa atual não oferece consulta por chave, arquivo
de índice, ordenação, inserção, alteração ou remoção.

## Contexto na coleção OSA

Este repositório cobre a etapa de **serialização e acesso sequencial** da
coleção de Organização de Sistemas de Arquivos:

- [`OSA-projects`](https://github.com/jotavsevla/OSA-projects) reúne os projetos
  e descreve a progressão entre empacotamento, índices e busca;
- [`LAB05_osa`](https://github.com/jotavsevla/LAB05_osa) aplica arquivo binário,
  índice primário e índice invertido a um motor de busca;
- [`LAB06_osa`](https://github.com/jotavsevla/LAB06_osa) avança para indexação
  por chave e endereço com Árvore B;
- [`LAB07_osa`](https://github.com/jotavsevla/LAB07_osa) contém uma segunda
  iteração da mesma linha de Árvore B, e não o motor de busca atribuído por
  descrições históricas.

Assim, o `tpII_osa` deve ser entendido como a base de organização física dos
registros. Busca indexada pertence aos outros módulos, não a este executável.

## Fluxo do programa

```text
Dados.txt
   │ leitura linha a linha; o cabeçalho é ignorado
   ▼
main.cpp ── cria ──> Registro
   │                    │
   │                    ├── packDelimitado()
   │                    └── packDescritor()
   ▼
Buffer ───────────> registros_delimitados.txt
   └──────────────> registros_descritores.bin
                         │
                         ▼
             releitura sequencial e impressão
```

Responsabilidades:

| Componente | Papel |
| --- | --- |
| `main.cpp` | Coordena entrada, transformação, escrita, releitura e apresentação. |
| `Registro.h/.cpp` | Mantém os quatro campos internos e faz o empacotamento/desempacotamento. |
| `Buffer.h/.cpp` | Encapsula o `fstream` e as operações sequenciais nos dois formatos. |
| `Dados.txt` | Base de entrada versionada. |
| `Makefile` | Compila os três arquivos-fonte no executável `programa`. |

Os nomes de entrada e saída são fixos no código. Por isso, o executável deve ser
iniciado a partir da raiz do repositório.

## Formatos persistidos

### Entrada consumida pelo parser

Após descartar a primeira linha, a implementação interpreta cada registro como:

```text
<nome completo>|<telefone>|<data de nascimento>
```

O nome completo é separado no **último espaço**: tudo que vem antes se torna
`nome`, e o último termo se torna `sobrenome`.

### Arquivo delimitado

`Registro::packDelimitado()` produz uma linha por registro:

```text
<nome>|<sobrenome>|<telefone>|<data de nascimento>\n
```

O caractere `|` não possui mecanismo de escape. Se um campo já contiver esse
caractere, a separação lógica dos campos fica ambígua.

Como o `Dados.txt` versionado usa finais de linha CRLF e `trim()` não remove
`\r`, esse byte permanece no último campo. No arquivo delimitado ele é seguido
pelo `\n` acrescentado por `packDelimitado()`; no binário ele integra o payload.

### Arquivo com descritor

Cada registro de `registros_descritores.bin` tem a forma:

```text
┌────────────────────────────┬────────────────────────────────────────────┐
│ int nativo: tamanho total  │ nome|sobrenome|telefone|data de nascimento │
└────────────────────────────┴────────────────────────────────────────────┘
```

O descritor armazena `sizeof(int) + tamanho_do_payload`. Não há terminador ou
quebra de linha acrescentado pelo formato binário. O leitor usa o descritor para
alocar o payload do próximo registro e depois separa seus campos por `|`.

Esse arquivo não é um formato binário portátil: tamanho do `int`, ordem dos
bytes e representação do inteiro seguem a plataforma que o gerou.

## Compilação e execução

Requisitos:

- compilador C++ com suporte a C++11;
- `make`.

```bash
make
./programa
```

Ou, em um único comando:

```bash
make run
```

Para remover objetos e o executável gerado:

```bash
make clean
```

> **Atenção:** cada execução abre os dois arquivos de saída com truncamento.
> `registros_delimitados.txt` e `registros_descritores.bin` são sobrescritos.
> O binário `programa` versionado é Mach-O x86-64; prefira recompilá-lo para a
> arquitetura da máquina local.

### Saída observável

Com o `Dados.txt` versionado, a execução conclui a conversão e percorre 100
registros em cada representação. O início da saída é:

```text
Conversão concluída!

Lendo registros do arquivo delimitado:
Nome: Dominic, Sobrenome: Norris, Telefone: Franklin, Data Nascimento: (40) 42272-1137|03/03/1971
```

Essa linha também evidencia a divergência de esquema descrita em
[Limitações conhecidas](#limitações-conhecidas).

## Estruturas, algoritmos e custo

- A entrada e os dois arquivos gerados são percorridos sequencialmente.
- Para `N` registros e `S` bytes de conteúdo, conversão e releitura custam
  `O(S)` tempo.
- A memória de trabalho é `O(L)`, onde `L` é o tamanho do maior registro, além
  das cópias temporárias feitas durante o empacotamento.
- O formato binário acrescenta um descritor de `sizeof(int)` por registro.
- Não existe índice em memória ou em disco. Uma busca sequencial acrescentada
  sobre o formato atual custaria `O(N)` registros no pior caso.

`Registro`, `std::string` e `std::fstream` controlam seus próprios recursos; não
há `new`, `delete` ou ponteiros proprietários no projeto. Cada `Buffer` possui
um único fluxo de arquivo. O programa o fecha explicitamente entre escrita e
leitura, e o destrutor de `fstream` funciona como proteção adicional no fim da
vida do objeto.

## Validação

Não há suíte de testes automatizados ou integração contínua no repositório. Uma
verificação manual mínima é:

```bash
make clean
make
./programa
```

Para a base versionada, devem aparecer duas seções de leitura com 100 registros
cada. A compilação atual foi validada com as opções do `Makefile`
(`-std=c++11 -Wall`) sem avisos.

Casos importantes para testes futuros:

- registro com nome sem espaço ou com vários sobrenomes;
- campos vazios e linhas malformadas;
- campo contendo `|`;
- arquivo binário vazio, truncado ou com descritor inválido;
- round-trip de cada formato, comparando todos os campos;
- leitura de binário produzido em outra arquitetura.

## Limitações conhecidas

1. **A base e o parser discordam sobre o esquema.** O cabeçalho de `Dados.txt`
   declara `Nome|Sobrenome|Telefone|Nascimento`, mas o código consome o primeiro
   campo como nome completo, o segundo como telefone e todo o restante como
   nascimento. Na base atual, isso desloca os valores: por exemplo, `Franklin`
   passa a telefone e `"(40) 42272-1137|03/03/1971"` passa a data.
2. **Delimitadores não são escapados.** Um `|` dentro de qualquer campo impede
   uma separação inequívoca no desempacotamento.
3. **A validação de entrada é mínima.** Quantidade de campos, datas, telefones e
   presença de sobrenome não são verificados. O `trim` remove apenas espaços
   comuns, não tabulações ou `\r`.
4. **Descritores binários não são validados antes da alocação.** Um arquivo
   corrompido ou não confiável pode declarar tamanho negativo, excessivo ou
   incompatível com os bytes disponíveis.
5. **Erros de escrita não são propagados.** Os métodos de escrita retornam
   `void`, e o programa não confirma falhas ocorridas depois da abertura.
6. **Não há interface de linha de comando.** Caminhos, formato e operação são
   definidos diretamente em `main.cpp`.
7. **Não há busca nem indexação.** Toda leitura é sequencial; o projeto não mede
   desempenho nem compara estratégias de recuperação.

## Possíveis evoluções

- alinhar formalmente o esquema de `Dados.txt` com o parser;
- escapar delimitadores ou adotar serialização com comprimentos por campo;
- usar inteiros de largura fixa e ordem de bytes definida no formato binário;
- validar limites antes de reservar memória e sinalizar registros truncados;
- aceitar caminhos e operação por argumentos;
- adicionar testes unitários de `pack`/`unpack` e testes de integração;
- construir índice primário ou Árvore B somente como uma camada separada sobre
  o armazenamento, preservando a responsabilidade deste TP.
