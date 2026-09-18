# NOVABANK — Sistema de Registro e Gestão de Contas Bancárias

**Etapa 1** do trabalho da disciplina **INF101 – Programação de Computadores I**
Centro Universitário de Viçosa – UNIVIÇOSA
Professores: Anderson R. Lamas e Vanderlea Queiroz

Estudante: Rhayane Freitas
Entrega: 30/09/2026

---

## Sobre o sistema

O NOVABANK permite cadastrar uma conta bancária e operar sobre ela através de um
menu que fica em execução até que o usuário escolha sair.

Conforme o escopo da Etapa 1, o programa principal usa **apenas variáveis
individuais** e estruturas de controle (`if/else`, `switch`, `do-while`, `for`).
Não são usados vetores nem funções próprias — esses assuntos pertencem à Etapa 2.

O desafio das 5 contas (item 3.5) está implementado em um **segundo programa**,
para que o programa da Etapa 1 continue respeitando a restrição de variáveis
individuais.

## Estrutura do repositório

```
sistema-bancario-etapa1/
├── src/
│   ├── sistema_bancario.cpp   → Etapa 1: cadastro de 1 conta (variáveis individuais)
│   └── desafio_5_contas.cpp   → Desafio 3.5: cadastro de até 5 contas (vetores)
├── bin/                        → executáveis gerados na compilação
└── README.md
```

## Como compilar e executar

Com o compilador **g++** (MinGW, Dev-C++, Code::Blocks ou terminal):

```bash
g++ -std=c++11 -o bin/sistema_bancario.exe src/sistema_bancario.cpp
g++ -std=c++11 -o bin/desafio_5_contas.exe src/desafio_5_contas.cpp
```

Para executar:

```bash
bin\sistema_bancario.exe
bin\desafio_5_contas.exe
```

> Os centavos podem ser digitados com **ponto** ou **vírgula** (`1500.50` ou
> `1500,50`). O que o sistema não aceita é ponto de milhar: dois mil reais se
> digita `2000`, e não `2.000`. O motivo está explicado nas validações.

## Variáveis obrigatórias (item 3.1)

| Variável      | Tipo     | Finalidade                    |
|---------------|----------|-------------------------------|
| `numeroConta` | `int`    | Número da conta               |
| `nomeCliente` | `string` | Nome do titular               |
| `cpf`         | `string` | CPF do titular                |
| `tipoConta`   | `int`    | 1 = Corrente; 2 = Poupança    |
| `saldo`       | `double` | Saldo atual                   |
| `contaAtiva`  | `bool`   | Indica se a conta está ativa  |

Variáveis acrescentadas para teste (item 3.5): `agencia` (`int`),
`dataAbertura` (`string`) e `contaCadastrada` (`bool`, controla se já existe
conta cadastrada).

## Menu (item 3.2)

```
***********************************************
**                 NOVABANK                  **
**       Sistema de Contas Bancárias         **
***********************************************
 1 - Cadastrar conta
 2 - Consultar conta
 3 - Verificar saldo
 4 - Alterar tipo da conta
 5 - Ativar/Desativar conta
 6 - Sair
 7 - Remover conta
***********************************************
```

A opção escolhida é tratada por um `switch`, e o menu é repetido por um
`do-while` que só termina quando a opção 6 é escolhida. Qualquer valor fora
da faixa 1–7 cai no `default` e exibe um aviso.

A **opção 7 (Remover conta)** é um acréscimo meu, além das cinco operações
pedidas no item 3.3: ela pede confirmação (S/N) e, se confirmada, devolve
todas as variáveis da conta aos valores iniciais, deixando o sistema como
estava antes do cadastro.

## Validações implementadas (item 3.4)

- **Número da conta** deve ser maior que zero.
- **Saldo inicial** não pode ser negativo.
- **Formato do saldo**: o valor é lido como texto e conferido algarismo por
  algarismo. Isso foi necessário porque `cin >> saldo` aceita entrada errada sem
  reclamar: `1500,50` viraria `1500.00` (os centavos somem) e `2.000` viraria
  `R$ 2,00`. Agora ponto e vírgula valem como separador de centavos, e ponto de
  milhar, mais de um separador, mais de duas casas decimais ou letras no meio do
  valor são recusados com aviso.
- **Tipo da conta** deve ser 1 ou 2.
- **Verificar saldo** e **Alterar tipo** só funcionam se `contaAtiva == true`.
- **Data de abertura** precisa estar no formato `dd/mm/aaaa`, com mês de 1 a 12,
  ano entre 1900 e 2100 e dia que exista naquele mês — `31/04/2026` é recusada, e
  `29/02` só passa em ano bissexto.
- Extras: nome precisa ter letras (recusa vazio, só espaços, só números e só
  símbolos) e no máximo 80 caracteres; letra colada em campo numérico é
  recusada (`12abc` não vira 12); CPF deve ter
  exatamente 11 dígitos numéricos; agência deve ser maior que zero; digitar
  letras onde se espera número não trava o programa (o estado do `cin` é limpo e
  o valor é recusado); consultar/alterar antes de cadastrar exibe aviso em vez
  de mostrar lixo de memória; cadastrar sobre uma conta existente e remover uma
  conta pedem confirmação (S/N) antes de apagar os dados.

## Desafio: "Se quiséssemos cadastrar até 5 contas, qual solução seria possível?"

**Com apenas variáveis individuais**, seria necessário declarar cada campo cinco
vezes (`numeroConta1`, `numeroConta2`, ..., `numeroConta5`, e o mesmo para nome,
CPF, tipo, saldo e situação) — 30 variáveis — e repetir o código de cadastro,
consulta e alteração para cada uma delas. O programa ficaria cinco vezes maior,
e mudar o limite para 10 contas exigiria reescrever tudo de novo.

**A solução adequada é usar vetores (arrays)**: uma única declaração
`int numeroConta[5];` guarda os cinco números, e o mesmo bloco de código atende
qualquer conta apenas trocando o índice. Para aumentar o limite basta alterar a
constante `MAX_CONTAS`.

A implementação está em [`src/desafio_5_contas.cpp`](src/desafio_5_contas.cpp),
usando **vetores paralelos** — a posição `i` de todos os vetores descreve a mesma
conta. Esse programa acrescenta:

- controle de lotação (`totalContas` × `MAX_CONTAS`);
- bloqueio de números de conta duplicados;
- busca sequencial pela conta informada antes das opções 2 a 5;
- listagem das contas disponíveis a cada operação.

Essa é exatamente a limitação que motiva a **Etapa 2** (vetores e funções): ao
repetir a mesma validação para cada campo, fica evidente a necessidade de
funções para eliminar a repetição de código.

## Roteiro de teste sugerido

1. Escolher a opção **2** sem ter cadastrado nada → deve avisar que não há conta.
2. Opção **1** e testar as recusas: número `-5`, agência `0`, nome vazio, CPF
   `123`, tipo `3`, saldo `-100`.
3. Cadastrar com dados válidos e consultar na opção **2**.
4. Opção **3** para ver o saldo.
5. Opção **4** para trocar Corrente ↔ Poupança.
6. Opção **5** para desativar e tentar a opção **3** novamente → deve bloquear.
7. Opção **7** para remover a conta: responder `N` → cancela; responder `S` →
   remove, e a opção **2** volta a avisar que não há conta cadastrada.
8. Digitar uma opção inexistente (ex.: `9`) → deve avisar e voltar ao menu.
9. Opção **6** para sair.
