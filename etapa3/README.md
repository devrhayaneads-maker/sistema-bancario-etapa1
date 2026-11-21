# NOVABANK - Etapa 3

Etapa 3 do trabalho de INF101 - Programação de Computadores I
Centro Universitário de Viçosa - UNIVIÇOSA
Professores: Anderson R. Lamas e Vanderlea Queiroz

Estudante: Rhayane de Freitas Eloi
Entrega: 25/11/2026

## O que mudou da Etapa 2 para a Etapa 3

Na Etapa 2 as contas e o histórico ficavam só nos vetores, então quando o
programa fechava tudo sumia. Agora o sistema grava tudo em dois arquivos de
texto e, quando abre de novo, lê esses arquivos e coloca os dados de volta nos
mesmos vetores da Etapa 2. Não precisa cadastrar as contas outra vez.

Comecei pela versão com funções da Etapa 2 (o desafio_funcoes.cpp), porque essa
etapa pede funções próprias para ler e gravar os arquivos.

## Arquivos e como rodar

- src/sistema_bancario.cpp: o programa

Para compilar pelo terminal com o g++:

```
mkdir bin
g++ -std=c++11 -o bin/sistema_bancario.exe src/sistema_bancario.cpp
bin\sistema_bancario.exe
```

No Dev-C++ ou no Code::Blocks é só abrir o .cpp e compilar.

Os arquivos contas.txt e movimentacoes.txt são criados na pasta de onde o
programa está rodando.

## Os arquivos de dados (item 2.1)

contas.txt, uma conta por linha, com os campos separados por ponto e vírgula:

```
numero;nome;cpf;tipo;saldo;ativa
1001;Ana Silva;12345678900;1;700.00;1
1002;Bruno Souza;98765432100;2;800.00;1
```

O tipo é 1 para corrente e 2 para poupança, e o último campo é 1 se a conta está
ativa e 0 se está inativa.

movimentacoes.txt, uma movimentação por linha:

```
conta;tipo;valor
1001;D;500.00
1001;S;300.00
```

D é depósito (entrada) e S é saque (saída).

Na transferência eu acrescentei um quarto campo, com a outra conta:

```
1001;S;300.00;1002
1002;D;300.00;1001
```

Foi pelo mesmo motivo da Etapa 2, quando criei o vetor contaTransferencia: sem
ele, depois de fechar e abrir o programa, o extrato de quem transferiu mostrava
"Saque" em vez de "Transferência enviada para a conta 1002". Depósito e saque
continuam com os 3 campos do enunciado, e o programa lê linhas com 3 ou com 4
campos, então um arquivo no formato do enunciado também funciona.

## Carregar ao abrir o programa (item 2.2)

Antes de mostrar o menu, o main() chama:

```cpp
carregarContas();
carregarMovimentacoes();
```

As duas fazem a mesma coisa:

1. abrem o arquivo com ifstream e conferem com is_open()
2. se não abriu, é a primeira vez e o arquivo ainda não existe: avisam e o
   sistema começa vazio
3. leem uma linha por vez com getline()
4. separam a linha nos pedaços entre os ";" (função separarCampos)
5. transformam cada pedaço de texto em número com as mesmas funções que leem o
   que a pessoa digita
6. colocam nos vetores e aumentam quantidadeContas ou quantidadeMovimentacoes
7. fecham o arquivo com close()

As contas são carregadas primeiro porque uma movimentação só entra se a conta
dela existir.

Antes de guardar uma linha, eu confiro se ela está certa: número de campos,
número da conta maior que zero e sem repetir, CPF com 11 dígitos, tipo 1 ou 2,
saldo que seja um valor válido e situação 1 ou 0. No histórico, a conta tem que
existir, o tipo tem que ser D ou S e o valor maior que zero. Se a linha estiver
estragada (alguém mexeu no arquivo, por exemplo), o programa avisa qual linha
foi e pula ela, em vez de travar ou criar uma conta com lixo. Linha em branco
ele só pula.

Antes de conferir, eu tiro os espaços que sobram no começo e no fim de cada
pedaço. Assim uma linha copiada do PDF com um espaço no fim, ou escrita como
`1003 ; Carlos Lima ; 11122233344 ; 1 ; 1500.00 ; 1`, continua valendo. Também
pulo o BOM, 3 caracteres invisíveis que alguns editores (como o bloco de notas)
gravam no começo do arquivo. Sem isso, a primeira conta do arquivo dava erro.

No fim aparece quantas contas e quantas movimentações foram carregadas:

```
>> 3 conta(s) carregada(s) do contas.txt
>> 4 movimentação(ões) carregada(s) do movimentacoes.txt
```

## Salvar (itens 2.3 e 2.4)

salvarContas() e salvarMovimentacoes() abrem o arquivo com ofstream, passam
pelo vetor inteiro gravando uma linha por posição e fecham. Como não uso o
ios::app, o arquivo começa do zero cada vez, mas como todas as linhas são
gravadas de novo nada se perde, igual ao exemplo do enunciado.

O saldo e os valores são gravados com 2 casas (700.00), usando o mesmo
fixed e setprecision(2) da tela.

Quando cada uma é chamada:

| Operação                 | salvarContas | salvarMovimentacoes |
|--------------------------|:------------:|:-------------------:|
| Cadastrar conta          | sim          |                     |
| Depositar                | sim          | sim                 |
| Sacar                    | sim          | sim                 |
| Transferir               | sim          | sim                 |
| Ativar/Desativar conta   | sim          |                     |
| Excluir conta            | sim          | sim                 |

Como grava logo depois de cada operação, se o programa for fechado de qualquer
jeito os dados já estão no arquivo. Por isso tirei a pergunta "Deseja realmente
sair?" que tinha na Etapa 2.

Pensei em usar o ios::app nas movimentações, para só acrescentar a linha nova no
fim do arquivo. Mas testando com um arquivo feito no bloco de notas, se a última
linha não tiver enter no final, a linha nova grudava nela
(1003;D;1500.001001;D;50.00) e as duas se perdiam na próxima vez que o programa
abria. Gravando tudo de novo isso não acontece. E na exclusão de conta eu teria
que reescrever o arquivo inteiro de qualquer jeito, porque as movimentações da
conta excluída saem do meio dele.

Se o arquivo não abrir para gravar, o programa avisa com uma mensagem de erro.

## Transferência (item 2.5)

Continua igual à Etapa 2: tira da origem, põe no destino e grava um S na origem
e um D no destino. A diferença é que no fim chama salvarContas() e
salvarMovimentacoes(). No exemplo do enunciado (Ana com R$ 1000,00 transferindo
R$ 300,00 para o Bruno, que tinha R$ 500,00), depois de fechar e abrir o
programa os saldos continuam 700.00 e 800.00 e os arquivos ficam assim:

```
contas.txt
1001;Ana Silva;12345678900;1;700.00;1
1002;Bruno Souza;98765432100;2;800.00;1

movimentacoes.txt
1001;S;300.00;1002
1002;D;300.00;1001
```

## Saldo inicial do extrato

Na Etapa 2 eu tinha o vetor saldoInicial para o extrato começar no saldo de
abertura da conta. Como o contas.txt do enunciado não tem esse campo, eu tirei o
vetor e passei a calcular: pego o saldo atual e desfaço as movimentações da
conta (tiro o que entrou e devolvo o que saiu). O que sobra é o saldo com que a
conta começou. A conta é feita em centavos, como todo o resto.

Exemplo: Ana tem 751.00 hoje, recebeu 50.25 e 0.75 e mandou 300.00. Então
751.00 - 50.25 - 0.75 + 300.00 = 1000.00, que foi o saldo do cadastro.

## Funções (item 2.7)

As quatro sugeridas no enunciado:

```cpp
void carregarContas();
void salvarContas();
void carregarMovimentacoes();
void salvarMovimentacoes();
```

E as que eu criei nessa etapa:

- separarCampos(): separa a linha do arquivo nos pedaços entre os ";"
- textoParaNumero(): era o miolo do lerNumero() da Etapa 2. Separei porque agora
  o número vem do teclado e também do arquivo. O lerNumero() só lê a linha e
  chama ela
- textoParaValor(): mesma coisa com o leitor de dinheiro. Devolve -1 se não for
  número e -2 se tiver mais de duas casas nos centavos, aí o lerValor() mostra a
  mesma mensagem de antes para cada caso
- cpfValido(): a conferência dos 11 dígitos, que agora é usada no cadastro e na
  leitura do arquivo
- tirarEspacos(): tira os espaços do começo e do fim de um texto ("  Ana Silva "
  vira "Ana Silva"). É usada no que a pessoa digita e em cada pedaço da linha do
  arquivo

O resto (cadastrar, listar, consultar, depositar, sacar, transferir, extrato,
ativar/desativar e excluir) é o mesmo da Etapa 2.

## Validação nova

O nome do titular não pode ter ponto e vírgula. Se alguém cadastrasse
"Ana;Silva", a linha no contas.txt ficaria com 7 campos em vez de 6 e a conta
não seria carregada da próxima vez.

Os espaços sobrando no começo e no fim do que a pessoa digita são ignorados:
" 1001 " vale como 1001 e o nome é gravado sem espaço sobrando. Antes, um
espaço sem querer depois do número fazia o programa dizer que era inválido.

## Testes (item 2.8)

Fiz os quatro testes do enunciado, fechando o programa com a opção 0 e abrindo
de novo entre as etapas. Antes de começar, apaguei o contas.txt e o
movimentacoes.txt para ser a primeira execução.

Teste 1 - cadastro e recuperação

1. Abrir o programa. Aparece que o contas.txt ainda não existe.
2. Cadastrar 1001 Ana Silva (corrente, 1000), 1002 Bruno Souza (poupança, 500)
   e 1003 Carlos Lima (corrente, 1500).
3. Sair e abrir de novo. Aparece "3 conta(s) carregada(s)".
4. Opção 2: as três contas com todos os dados.

Teste 2 - saldos

1. Opção 4: depositar 50,25 na 1001 (vai a 1050.25).
2. Opção 5: sacar 200 da 1003 (vai a 1300.00).
3. Sair e abrir de novo.
4. Opção 3 na 1001 e na 1003: 1050.25 e 1300.00.

Teste 3 - transferência

1. Opção 6: transferir 300 da 1001 para a 1002.
2. Sair e abrir de novo.
3. Opção 3: a 1001 com 750.25 e a 1002 com 800.00.
4. Opção 7 na 1001: "Transferência enviada para a conta 1002: R$ 300.00".
   Opção 7 na 1002: "Transferência recebida da conta 1001: R$ 300.00".

Teste 4 - histórico

1. Fazer mais um depósito, um saque e uma transferência.
2. Sair e abrir de novo.
3. Opção 7 na 1001: aparecem as movimentações das execuções anteriores e as
   novas, na ordem em que aconteceram.

Testes que acrescentei

- desativar a 1002, sair e abrir: ela continua INATIVA e a transferência para
  ela é bloqueada
- excluir uma conta com saldo zero, sair e abrir: ela some do contas.txt e as
  movimentações dela somem do movimentacoes.txt
- copiar os exemplos do enunciado para o contas.txt e o movimentacoes.txt na mão,
  sem enter na última linha: o programa carrega as 3 contas e as 4 movimentações
- colocar linhas estragadas no arquivo (conta repetida, saldo negativo, tipo X,
  conta que não existe): o programa avisa a linha e carrega o resto
- tentar cadastrar o nome "João;Ávila": recusa por causa do ponto e vírgula
- digitar ";" no número da conta, no CPF, no tipo e no valor: todos recusam, e
  toda linha do contas.txt continua com 6 campos
- centavos com ponto e com vírgula (1500.50, 1500,50, ,50 e 0,5) dão o mesmo
  valor, e no arquivo sempre fica com ponto e 2 casas. 2.000, 1.500,50 e
  1,500.50 são recusados
- arquivo salvo com BOM e com espaços sobrando em volta dos ";": carrega tudo
- digitar " 1001 " com espaços: aceita como 1001
