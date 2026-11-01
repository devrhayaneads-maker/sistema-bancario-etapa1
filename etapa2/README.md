# NOVABANK - Etapa 2

Etapa 2 do trabalho de INF101 - Programação de Computadores I
Centro Universitário de Viçosa - UNIVIÇOSA
Professores: Anderson R. Lamas e Vanderlea Queiroz

Estudante: Rhayane de Freitas Eloi
Entrega: 11/11/2026

## O que mudou da Etapa 1 para a Etapa 2

Na Etapa 1 o sistema guardava uma conta só, em variáveis soltas. Agora ele guarda
até 100 contas e registra cada depósito, saque e transferência num histórico de
até 500 movimentações, tudo em vetores.

Os vetores das contas são paralelos: a posição i de numeroConta, nomeCliente,
cpf, tipoConta, saldo e contaAtiva é sempre a mesma conta. No histórico, cada
posição é uma movimentação.

Tudo fica só na memória, então quando o programa fecha as contas e o histórico
somem (salvar em arquivo é a Etapa 3). Por isso coloquei uma confirmação antes
de sair.

## Arquivos e como rodar

- src/sistema_bancario.cpp: a versão principal, com tudo dentro do main()
- src/desafio_funcoes.cpp: o desafio do fim do enunciado, o mesmo sistema separado em funções

As duas mostram as mesmas telas e mensagens, só muda a organização do código.

Para compilar pelo terminal com o g++:

```
mkdir bin
g++ -std=c++11 -o bin/sistema_bancario.exe src/sistema_bancario.cpp
g++ -std=c++11 -o bin/desafio_funcoes.exe src/desafio_funcoes.cpp
bin\sistema_bancario.exe
```

No Dev-C++ ou no Code::Blocks é só abrir o .cpp e compilar.

Nos valores em dinheiro os centavos podem ir com ponto ou vírgula (1500.50 ou
1500,50), mas ponto de milhar não: dois mil reais se digita 2000.

## Vetores

Contas (item 2.1), com MAX_CONTAS = 100 e quantidadeContas dizendo quantas
posições já estão ocupadas:

| Vetor        | Tipo   | Guarda                          |
|--------------|--------|---------------------------------|
| numeroConta  | int    | número da conta                 |
| nomeCliente  | string | nome do titular                 |
| cpf          | string | CPF, 11 dígitos                 |
| tipoConta    | int    | 1 corrente, 2 poupança          |
| saldo        | double | saldo atual                     |
| contaAtiva   | bool   | se a conta está ativa           |
| saldoInicial | double | saldo do cadastro (acrescentei) |

Histórico (item 2.2), com MAX_MOVIMENTACOES = 500 e quantidadeMovimentacoes:

| Vetor              | Tipo   | Guarda                                         |
|--------------------|--------|------------------------------------------------|
| contaMovimentacao  | int    | número da conta                                |
| tipoMovimentacao   | char   | D (entrada) ou S (saída)                       |
| valorMovimentacao  | double | valor                                          |
| contaTransferencia | int    | a outra conta, se foi transferência (acrescentei) |

Os dois que eu acrescentei são para o extrato. O saldoInicial guarda com quanto a
conta foi aberta, aí o extrato fecha (saldo inicial + movimentações = saldo
atual). O contaTransferencia foi porque a transferência é gravada como um S na
origem e um D no destino, como pede o item 2.5, e sem ele o extrato de quem
transferiu mostrava "Saque" como se fosse um saque normal. Quando não é
transferência ele fica 0.

## Menu

```
 1 - Cadastrar conta
 2 - Listar contas
 3 - Consultar conta
 4 - Depositar
 5 - Sacar
 6 - Transferir
 7 - Extrato
 8 - Ativar/Desativar conta
 9 - Excluir conta
 0 - Sair
```

De 1 a 7 são as funcionalidades do item 2.3. A 8 e a 9 eu acrescentei. Sem a 8
nenhuma conta ficaria inativa e não dava para testar a regra de que a
transferência só acontece entre contas ativas. A 9 apaga a conta de vez e libera
a vaga (na Etapa 1 era a opção de remover conta).

## Como cada opção funciona

Da opção 3 em diante o programa pede o número da conta e faz uma busca
sequencial nos vetores até achar a posição dela.

O depósito soma o valor no saldo e grava um D no histórico, igual ao exemplo do
item 2.4. O saque confere se tem saldo, tira o valor e grava um S. Depósito,
saque e transferência só funcionam com a conta ativa.

A transferência segue o item 2.5. Antes de mexer em qualquer saldo o programa
confere:

1. se a conta de origem existe e está ativa
2. se cabem dois registros no histórico
3. se a conta de destino existe, é outra conta e está ativa
4. se o valor é maior que zero e a origem tem saldo

No exemplo do enunciado (Ana, conta 1001, com R$ 1000,00 transferindo R$ 300,00
para o Bruno, conta 1002, com R$ 500,00) os saldos ficam 700,00 e 800,00 e o
histórico ganha duas linhas:

| Conta | Tipo | Valor  |
|-------|------|--------|
| 1001  | S    | 300.00 |
| 1002  | D    | 300.00 |

O extrato mostra o saldo inicial, passa pelo histórico inteiro mostrando só o que
é da conta pedida (item 2.6) e termina com o saldo atual.

Na exclusão eu segui o que um banco faz: com o saldo zerado o programa pede
confirmação (S/N) e exclui; se a conta ainda tem dinheiro, ele mostra o saldo e
pede para sacar ou transferir antes; e se o saldo estivesse negativo ele não
deixaria excluir. Hoje o saldo nunca fica negativo, porque saque e transferência
conferem antes, mas deixei a regra pronta caso um dia tenha cheque especial.
Quando a conta é excluída, as movimentações dela saem do histórico e as contas
que vinham depois andam uma posição para trás no vetor, para não ficar buraco. A
transferência continua aparecendo no extrato da outra conta.

## Validações

- número da conta maior que zero e sem repetir
- nome com pelo menos uma letra e no máximo 80 caracteres
- CPF com 11 dígitos, sem pontos, traços ou espaços
- tipo 1 ou 2
- saldo inicial pode ser zero, mas depósito, saque e transferência têm que ser
  maiores que zero, e nenhum valor passa de R$ 1000000000.00 por operação
- o valor é lido como texto e conferido caractere por caractere, igual na Etapa 1,
  porque o cin >> aceitava 1500,50 como 1500 e 2.000 como 2 reais sem avisar
- letra onde era para ser número não trava o programa: no cadastro ele pergunta
  de novo, e no menu ou no número da conta ele avisa e volta para o menu
- quando chega em 100 contas ou 500 movimentações ele avisa em vez de gravar
  fora do vetor

## Problemas que achei testando

Testando o saque, depositei R$ 0,10 numa conta que tinha R$ 0,70. A tela
mostrava R$ 0.80, mas sacar os 0,80 dava saldo insuficiente. É que o double não
guarda 0,1 certinho, e 0,70 + 0,10 vira 0,7999999... Resolvi fazendo toda conta
com dinheiro em centavos inteiros: round(valor * 100) transforma 0,70 em 70, e
inteiro não tem esse erro. O saque, por exemplo, ficou assim:

```cpp
saldo[posicao] = (round(saldo[posicao] * 100) - round(valor * 100)) / 100;
```

A conferência de saldo suficiente também compara em centavos. Testei compilando
do jeito dos compiladores antigos de 32 bits e, comparando direto, sacar o saldo
inteiro às vezes dava insuficiente.

O outro foi com acento. Cadastrei "João Ávila" e na listagem apareceu
"Jo�o �vila". Só a saída do terminal estava em UTF-8, o que eu digitava chegava
em outro formato. Agora o programa acerta as duas:

```cpp
SetConsoleOutputCP(CP_UTF8);
SetConsoleCP(CP_UTF8);
```

## Desafio: versão com funções

No desafio_funcoes.cpp usei os protótipos que estão no enunciado:

```cpp
void cadastrarConta();
void listarContas();
void consultarConta();
void depositar();
void sacar();
void transferir();
void registrarMovimentacao(int conta, char tipo, double valor, int outraConta);
void exibirExtrato();
```

Como essas funções não recebem parâmetros, os vetores tiveram que sair de dentro
do main() e virar globais, senão as funções não enxergam as contas. Só a
registrarMovimentacao ganhou parâmetros, porque sem eles ela não teria como saber
o que gravar.

Também criei funções para os pedaços que na versão sem funções eu tive que copiar
e colar várias vezes: lerInteiro() (lê um número sem travar com letra),
lerValor() (o leitor de dinheiro da Etapa 1), buscarConta() (a busca
sequencial), pedirConta() (pergunta o número, busca e avisa se não achou) e
centavos() (passa reais para centavos inteiros). A ativarDesativarConta() e a
excluirConta() são as opções 8 e 9.
Na versão principal, por exemplo, o leitor de valor aparece duas vezes, e aqui
ele existe uma vez só.

## Roteiro de teste

1. Opção 2 sem nada cadastrado: avisa que não tem conta.
2. Opção 1 testando as recusas (número 0, CPF 123, tipo 3, saldo 2.000 e -50) e
   depois cadastrando a 1001, Ana, corrente, saldo 1000.
3. Opção 1: 1002, Bruno, poupança, saldo 500.
4. Opção 2: as duas contas na tabela.
5. Opção 6: transferir 300 da 1001 para a 1002, igual ao exemplo do enunciado.
   Os saldos ficam 700.00 e 800.00.
6. Opção 7 na 1001 e na 1002: saldo inicial, a transferência saindo de uma e
   entrando na outra, e o saldo atual.
7. Opção 4: depositar 50,25 na 1001.
8. Opção 5: sacar 10000 da 1002 (saldo insuficiente) e depois 800 (o saldo vai
   a 0.00).
9. Opção 8: desativar a 1002 e tentar a opção 6 de novo. Bloqueia porque o
   destino está inativo.
10. Opção 3 na 1002: aparece INATIVA.
11. Opção 9 na 1001: não deixa excluir porque ainda tem saldo.
12. Opção 9 na 1002 (saldo zerado): responder N cancela; repetindo e respondendo
    S, exclui. Na opção 2 sobra só a 1001, e o extrato dela continua mostrando a
    transferência para a 1002.
13. Opção 0: responder N (volta para o menu) e depois 0 e S para sair.
