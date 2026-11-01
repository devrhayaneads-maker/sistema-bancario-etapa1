// NOVABANK - Etapa 2, versão com funções (o desafio do fim do enunciado)
// INF101 - Programação de Computadores I - UNIVIÇOSA
// Rhayane Freitas
//
// É o mesmo sistema do sistema_bancario.cpp, com as mesmas telas e
// mensagens, só que cada opção do menu virou uma função.
//
// Como esses protótipos não recebem nada (void depositar(), por exemplo),
// os vetores tiveram que sair de dentro do main e virar globais (declarados
// fora de qualquer função), senão as funções não enxergam as contas.

#include <iostream>  // cin e cout
#include <string>
#include <iomanip>   // setprecision e setw, pra formatar os números
#include <limits>    // numeric_limits, usado no cin.ignore
#include <cmath>     // round, pra acertar os centavos
#include <cstdlib>   // exit, pra encerrar o programa de dentro de uma função

#ifdef _WIN32        // só entra aqui quando compila no Windows
#include <windows.h> // sem isso os acentos saem errados no terminal do Windows
#endif

using namespace std;   // pra não ter que escrever std::cout

// contas (item 2.1). a posição i de todos esses vetores é a mesma conta
const int MAX_CONTAS = 100;
int numeroConta[MAX_CONTAS];
string nomeCliente[MAX_CONTAS];
string cpf[MAX_CONTAS];
int tipoConta[MAX_CONTAS];   // 1 = corrente, 2 = poupança
double saldo[MAX_CONTAS];
bool contaAtiva[MAX_CONTAS];
double saldoInicial[MAX_CONTAS];   // acrescentei, pro extrato mostrar de onde a conta partiu
int quantidadeContas = 0;          // quantas posições já estão ocupadas

// histórico (item 2.2). cada posição é uma movimentação
const int MAX_MOVIMENTACOES = 500;
int contaMovimentacao[MAX_MOVIMENTACOES];     // de qual conta é a movimentação
char tipoMovimentacao[MAX_MOVIMENTACOES];     // 'D' = entrada, 'S' = saída
double valorMovimentacao[MAX_MOVIMENTACOES];
int contaTransferencia[MAX_MOVIMENTACOES];    // outra conta da transferência, ou 0 se não for uma
int quantidadeMovimentacoes = 0;

const double VALOR_MAXIMO = 1000000000.00;   // limite de cada valor digitado (1 bilhão)

// protótipos: aqui eu só aviso o compilador que essas funções existem, o
// código de cada uma vem depois do main. essas primeiras são as que o
// enunciado sugeriu. só a registrarMovimentacao ganhou parâmetros, porque sem
// eles ela não tem como saber o que gravar
void cadastrarConta();
void listarContas();
void consultarConta();
void depositar();
void sacar();
void transferir();
void registrarMovimentacao(int conta, char tipo, double valor, int outraConta);
void exibirExtrato();

// essas eu criei. ativarDesativarConta e excluirConta são as opções 8 e 9, e
// as outras são pedaços que, na versão sem funções, eu tive que copiar e colar
// várias vezes
void ativarDesativarConta();
void excluirConta();
int lerInteiro();
double lerValor(string pergunta, bool aceitaZero);
int buscarConta(int numero);
int pedirConta(string pergunta);
double centavos(double valor);

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // o que o programa escreve sai com acento
    SetConsoleCP(CP_UTF8);         // e o que a pessoa digita também (senão João chegava Jo?o)
#endif

    int opcao;

    // o menu fica repetindo até a pessoa escolher 0 e confirmar
    do {
        cout << "\n";
        cout << "***********************************************\n";
        cout << "**                 NOVABANK                  **\n";
        cout << "**       Sistema de Contas Bancárias         **\n";
        cout << "***********************************************\n";
        cout << " Contas: " << quantidadeContas << " de " << MAX_CONTAS
             << "   Movimentações: " << quantidadeMovimentacoes << " de " << MAX_MOVIMENTACOES << "\n";
        cout << "***********************************************\n";
        cout << " 1 - Cadastrar conta\n";
        cout << " 2 - Listar contas\n";
        cout << " 3 - Consultar conta\n";
        cout << " 4 - Depositar\n";
        cout << " 5 - Sacar\n";
        cout << " 6 - Transferir\n";
        cout << " 7 - Extrato\n";
        cout << " 8 - Ativar/Desativar conta\n";
        cout << " 9 - Excluir conta\n";
        cout << " 0 - Sair\n";
        cout << "***********************************************\n";
        cout << "Escolha uma opção: ";

        opcao = lerInteiro();   // volta -1 se a pessoa digitar letra

        // cada opção chama a sua função, e o código delas fica lá embaixo
        switch (opcao) {
            case 1: cadastrarConta();       break;
            case 2: listarContas();         break;
            case 3: consultarConta();       break;
            case 4: depositar();            break;
            case 5: sacar();                break;
            case 6: transferir();           break;
            case 7: exibirExtrato();        break;
            case 8: ativarDesativarConta(); break;
            case 9: excluirConta();         break;

            case 0: {
                // nada fica salvo nesta etapa, então confirmo antes de sair
                char resposta;
                cout << "\nAs contas e movimentações ficam só na memória e serão perdidas.\n";
                cout << "Deseja realmente sair? (S/N): ";
                cin >> resposta;
                if (cin.fail()) return 0;   // a entrada acabou, sai de qualquer jeito
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (resposta == 'S' || resposta == 's') {
                    cout << "\n>> Encerrando o NOVABANK. Obrigado por utilizar nossos serviços!\n";
                } else {
                    opcao = -1;   // só pro do-while não terminar
                    cout << "\n>> Saída cancelada.\n";
                }
                break;
            }

            // qualquer outro número
            default:
                cout << "\n>> Opção inválida! Escolha um número de 0 a 9.\n";
                break;
        }

        // sem essa pausa o menu volta antes da pessoa ler o resultado
        if (opcao != 0) {
            cout << "\nPressione ENTER para continuar...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    } while (opcao != 0);

    return 0;
}

// lê um número inteiro do teclado. letra, ou letra colada no número (12abc),
// volta como -1, que não serve para nenhum campo do sistema
int lerInteiro() {
    int numero;
    cin >> numero;
    if (cin.fail()) {   // deu erro na leitura (digitou letra, por exemplo)
        // a entrada acabou e não tem como perguntar de novo. como aqui eu estou
        // dentro de uma função, o return só sairia dela, então uso o exit
        if (cin.eof()) exit(0);
        cin.clear();   // tira o erro, senão o cin fica travado
        numero = -1;
    } else {
        // pula espaço e TAB depois do número (o peek espia o próximo caractere
        // sem tirar, o get tira). se sobrar outra coisa antes do ENTER ('\n'),
        // foi letra colada no número, tipo 12abc
        while (cin.peek() == ' ' || cin.peek() == '\t') {
            cin.get();
        }
        if (!cin.eof() && cin.peek() != '\n') {
            numero = -1;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');   // joga fora o resto da linha
    return numero;
}

// a mesma leitura de dinheiro da Etapa 1, como texto, só que agora escrita
// uma vez só. fica perguntando até vir um valor válido. "pergunta" é o texto
// que aparece antes de ler, e "aceitaZero" diz se o 0 vale (só no saldo inicial)
double lerValor(string pergunta, bool aceitaZero) {
    double valor = 0.0;
    bool valido;

    cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
    do {
        cout << pergunta;
        string valorDigitado;
        getline(cin, valorDigitado);   // a linha inteira, como texto
        if (cin.fail()) exit(0);

        // tira o espaço que a pessoa deixou nas pontas. empty() diz se o texto
        // está vazio, e erase(posição, 1) apaga um caractere
        while (!valorDigitado.empty() &&
               (valorDigitado[0] == ' ' || valorDigitado[0] == '\t')) {
            valorDigitado.erase(0, 1);
        }
        while (!valorDigitado.empty() &&
               (valorDigitado[valorDigitado.length() - 1] == ' ' ||
                valorDigitado[valorDigitado.length() - 1] == '\t')) {
            valorDigitado.erase(valorDigitado.length() - 1, 1);
        }

        // primeiro só confiro o que foi digitado, caractere por caractere
        bool soNumero = true;     // apareceu letra ou símbolo estranho?
        int digitos = 0;          // quantos algarismos o valor tem
        bool negativo = false;    // começou com sinal de menos?
        int separadores = 0;      // quantos . ou , foram digitados
        int casasDecimais = 0;    // quantos algarismos vieram depois deles

        for (int c = 0; c < (int) valorDigitado.length(); c++) {
            char letra = valorDigitado[c];
            if (letra == '-' && c == 0) {
                negativo = true;
            } else if (letra == '.' || letra == ',') {
                separadores++;
                casasDecimais = 0;
            } else if (letra >= '0' && letra <= '9') {
                digitos++;
                if (separadores > 0) {
                    casasDecimais++;
                }
            } else {
                soNumero = false;
            }
        }

        // cada problema tem a sua mensagem, pra pessoa saber o que corrigir
        valido = false;
        if (!soNumero || digitos == 0) {
            cout << "   >> Inválido! Digite apenas números, ex: 1500.50\n";
        } else if (negativo) {
            cout << "   >> Inválido! O valor não pode ser negativo.\n";
        } else if (separadores > 1) {
            cout << "   >> Inválido! Use um separador só, ex: 1500.50\n";
        } else if (separadores == 1 && casasDecimais > 2) {
            cout << "   >> Inválido! Os centavos têm no máximo duas casas.\n";
            cout << "      (dois mil reais se digita 2000, sem ponto de milhar)\n";
        } else {
            valido = true;
        }

        if (valido) {
            // monto o número na mão: junto os algarismos e divido no final.
            // ex: "1500,50" vira 150050 / 100 = 1500.50
            valor = 0.0;
            double divisor = 1.0;
            bool depoisDoSeparador = false;

            for (int c = 0; c < (int) valorDigitado.length(); c++) {
                char letra = valorDigitado[c];
                if (letra == '.' || letra == ',') {
                    depoisDoSeparador = true;
                } else if (letra >= '0' && letra <= '9') {
                    valor = valor * 10 + (letra - '0');
                    if (depoisDoSeparador) {
                        divisor = divisor * 10;
                    }
                }
            }
            valor = valor / divisor;

            if (valor > VALOR_MAXIMO) {
                valido = false;
                cout << "   >> Inválido! O valor máximo por operação é R$ "
                     << fixed << setprecision(2) << VALOR_MAXIMO << "\n";
            } else if (valor == 0 && !aceitaZero) {   // só o saldo inicial pode ser 0
                valido = false;
                cout << "   >> Inválido! O valor deve ser maior que zero.\n";
            }
        }
    } while (!valido);

    return valor;
}

// busca sequencial pelo número: olho uma posição de cada vez. devolve a
// posição da conta nos vetores, ou -1 se não achar
int buscarConta(int numero) {
    for (int i = 0; i < quantidadeContas; i++) {
        if (numeroConta[i] == numero) {
            return i;   // achou: o return já sai da função, nem termina o for
        }
    }
    return -1;
}

// pergunta o número da conta e já procura. se der errado, avisa e devolve -1,
// então quem chama só precisa testar o -1
int pedirConta(string pergunta) {
    if (quantidadeContas == 0) {
        cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
        return -1;
    }

    cout << pergunta;
    int numero = lerInteiro();
    if (numero <= 0) {   // letra (que vira -1), zero ou negativo
        cout << "\n>> Número de conta inválido.\n";
        return -1;
    }

    int posicao = buscarConta(numero);
    if (posicao == -1) {
        cout << "\n>> Conta " << numero << " não encontrada.\n";
    }
    return posicao;
}

// passa um valor em reais pra centavos inteiros (0.70 vira 70). toda conta
// com dinheiro eu faço em centavos: testando, vi que 0.70 + 0.10 dava
// 0.7999999 no double, aparecia R$ 0.80 na tela mas sacar os 0.80 dava saldo
// insuficiente. o round arredonda pro inteiro mais perto
double centavos(double valor) {
    return round(valor * 100);
}

// grava uma linha no histórico, na primeira posição livre, e aumenta a
// quantidade. quem chama já conferiu se tem espaço
void registrarMovimentacao(int conta, char tipo, double valor, int outraConta) {
    contaMovimentacao[quantidadeMovimentacoes] = conta;
    tipoMovimentacao[quantidadeMovimentacoes] = tipo;
    valorMovimentacao[quantidadeMovimentacoes] = valor;
    contaTransferencia[quantidadeMovimentacoes] = outraConta;
    quantidadeMovimentacoes++;
}

// opção 1 do menu. a conta nova vai na primeira posição livre dos vetores
void cadastrarConta() {
    // o vetor tem tamanho fixo, então preciso testar se ainda cabe
    if (quantidadeContas >= MAX_CONTAS) {
        cout << "\n>> Limite de " << MAX_CONTAS << " contas atingido.\n";
        return;
    }

    int i = quantidadeContas;   // próxima posição livre
    cout << "\n--------- CADASTRO DE CONTA ---------\n";

    // número da conta: maior que zero e sem repetir (a buscarConta diz se já existe)
    bool numeroValido;
    do {
        cout << "Número da conta....: ";
        numeroConta[i] = lerInteiro();

        numeroValido = (numeroConta[i] > 0);
        if (!numeroValido) {
            cout << "   >> Inválido! O número da conta deve ser um inteiro maior que zero.\n";
        } else if (buscarConta(numeroConta[i]) != -1) {
            numeroValido = false;
            cout << "   >> Esse número de conta já está cadastrado.\n";
        }
    } while (!numeroValido);

    // nome: precisa ter pelo menos uma letra e caber em 80 caracteres
    bool nomeValido;
    do {
        cout << "Nome do titular....: ";
        getline(cin, nomeCliente[i]);   // a linha inteira, com os espaços do nome
        if (cin.fail()) exit(0);

        nomeValido = false;
        for (int k = 0; k < (int) nomeCliente[i].length(); k++) {
            char letra = nomeCliente[i][k];
            if ((letra >= 'A' && letra <= 'Z') ||
                (letra >= 'a' && letra <= 'z') ||
                letra < 0) {   // letra com acento ocupa 2 bytes, e eles vêm negativos
                nomeValido = true;
            }
        }
        if (!nomeValido) {
            cout << "   >> Inválido! O nome precisa ter letras, ex: Maria Silva\n";
        } else if (nomeCliente[i].length() > 80) {
            nomeValido = false;
            cout << "   >> Inválido! O nome pode ter no máximo 80 caracteres.\n";
        }
    } while (!nomeValido);

    // CPF: exatamente 11 caracteres, e todos têm que ser dígitos
    bool cpfValido;
    do {
        cout << "CPF (11 dígitos)...: ";
        getline(cin, cpf[i]);
        if (cin.fail()) exit(0);

        cpfValido = (cpf[i].length() == 11);
        for (int k = 0; k < (int) cpf[i].length() && cpfValido; k++) {
            if (cpf[i][k] < '0' || cpf[i][k] > '9') {
                cpfValido = false;
            }
        }
        if (!cpfValido) {
            cout << "   >> Inválido! Digite exatamente 11 dígitos, sem pontos, traços ou espaços.\n";
        }
    } while (!cpfValido);

    // tipo: só aceita 1 (corrente) ou 2 (poupança)
    do {
        cout << "Tipo (1=Corrente, 2=Poupança): ";
        tipoConta[i] = lerInteiro();
        if (tipoConta[i] != 1 && tipoConta[i] != 2) {
            cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
        }
    } while (tipoConta[i] != 1 && tipoConta[i] != 2);

    saldo[i] = lerValor("Saldo inicial R$...: ", true);   // true: aqui o 0 vale
    saldoInicial[i] = saldo[i];   // guardo pra mostrar no extrato

    contaAtiva[i] = true;   // conta nova já nasce ativa
    quantidadeContas++;     // só agora a posição i passa a valer

    cout << "\n>> Conta " << numeroConta[i] << " cadastrada com sucesso e ATIVADA!\n";
}

void listarContas() {
    if (quantidadeContas == 0) {
        cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
        return;
    }

    cout << "\n--------- CONTAS CADASTRADAS ---------\n";
    // o nome ficou por último porque o setw conta letra com acento como duas
    cout << "     Conta  Tipo      Situação     Saldo (R$)  Titular\n";
    for (int i = 0; i < quantidadeContas; i++) {
        // setw deixa as colunas alinhadas, e fixed com setprecision(2) mostra
        // sempre duas casas: 1000.00
        cout << setw(10) << numeroConta[i] << "  ";
        if (tipoConta[i] == 1) {
            cout << "Corrente  ";
        } else {
            cout << "Poupança  ";
        }
        if (contaAtiva[i]) {
            cout << "ATIVA   ";
        } else {
            cout << "INATIVA ";
        }
        cout << setw(15) << fixed << setprecision(2) << saldo[i]
             << "  " << nomeCliente[i] << "\n";
    }
    cout << "\nTotal: " << quantidadeContas << " conta(s)\n";
}

void consultarConta() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;   // não achou (a pedirConta já avisou)

    // conto quantas movimentações dessa conta existem no histórico
    int movimentacoesDaConta = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            movimentacoesDaConta++;
        }
    }

    cout << "\n--------- DADOS DA CONTA ---------\n";
    cout << "Número da conta : " << numeroConta[posicao] << "\n";
    cout << "Titular         : " << nomeCliente[posicao] << "\n";
    cout << "CPF             : " << cpf[posicao] << "\n";

    cout << "Tipo            : ";
    if (tipoConta[posicao] == 1) {
        cout << "Corrente\n";
    } else {
        cout << "Poupança\n";
    }

    cout << "Saldo           : R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";

    cout << "Situação        : ";
    if (contaAtiva[posicao]) {
        cout << "ATIVA\n";
    } else {
        cout << "INATIVA\n";
    }
    cout << "Movimentações   : " << movimentacoesDaConta << "\n";
}

// depósito: soma no saldo e grava um 'D' no histórico, igual ao item 2.4
void depositar() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    // conta inativa não movimenta dinheiro
    if (!contaAtiva[posicao]) {
        cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
        return;
    }
    // precisa caber mais um registro no histórico
    if (quantidadeMovimentacoes + 1 > MAX_MOVIMENTACOES) {
        cout << "\n>> Histórico de movimentações cheio (limite de "
             << MAX_MOVIMENTACOES << ").\n";
        return;
    }

    cout << "\n--------- DEPÓSITO ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
    cout << "Saldo atual: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";

    double valor = lerValor("Valor R$: ", false);   // false: aqui o 0 não vale

    saldo[posicao] = (centavos(saldo[posicao]) + centavos(valor)) / 100;   // soma em centavos e volta pra reais
    registrarMovimentacao(numeroConta[posicao], 'D', valor, 0);   // 0: não é transferência

    cout << "\n>> Depósito realizado! Novo saldo: R$ "
         << fixed << setprecision(2) << saldo[posicao] << "\n";
}

// no saque tem que conferir se tem saldo antes de tirar
void sacar() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    if (!contaAtiva[posicao]) {
        cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
        return;
    }
    if (quantidadeMovimentacoes + 1 > MAX_MOVIMENTACOES) {
        cout << "\n>> Histórico de movimentações cheio (limite de "
             << MAX_MOVIMENTACOES << ").\n";
        return;
    }

    cout << "\n--------- SAQUE ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
    cout << "Saldo atual: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";

    double valor = lerValor("Valor R$: ", false);

    // saldo insuficiente: não mexe em nada e só avisa. comparo em centavos
    // porque num compilador de 32 bits a comparação direta errava no saque do
    // saldo inteiro
    if (centavos(valor) > centavos(saldo[posicao])) {
        cout << "\n>> Saldo insuficiente! Saldo disponível: R$ "
             << fixed << setprecision(2) << saldo[posicao] << "\n";
        return;
    }

    saldo[posicao] = (centavos(saldo[posicao]) - centavos(valor)) / 100;
    registrarMovimentacao(numeroConta[posicao], 'S', valor, 0);

    cout << "\n>> Saque realizado! Novo saldo: R$ "
         << fixed << setprecision(2) << saldo[posicao] << "\n";
}

// transferência (item 2.5): sai da origem, entra no destino e grava dois registros
void transferir() {
    // com zero contas, a pedirConta já avisa. com uma só, não tem pra quem mandar
    if (quantidadeContas == 1) {
        cout << "\n>> Para transferir é preciso ter pelo menos duas contas cadastradas.\n";
        return;
    }

    // origem: tem que existir e estar ativa
    int origem = pedirConta("\nConta de origem: ");
    if (origem == -1) return;

    if (!contaAtiva[origem]) {
        cout << "\n>> Operação não permitida: a conta de origem está INATIVA.\n";
        return;
    }
    // são dois registros (item 2.5), então precisam caber os dois antes de começar
    if (quantidadeMovimentacoes + 2 > MAX_MOVIMENTACOES) {
        if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
            cout << "\n>> Sem espaço no histórico para a transferência: ela grava 2 movimentações e só resta 1.\n";
        } else {
            cout << "\n>> Histórico de movimentações cheio (limite de "
                 << MAX_MOVIMENTACOES << ").\n";
        }
        return;
    }

    // destino: tem que existir, ser outra conta e estar ativo
    int destino = pedirConta("Conta de destino: ");
    if (destino == -1) return;

    if (destino == origem) {
        cout << "\n>> A conta de destino precisa ser diferente da conta de origem.\n";
        return;
    }
    if (!contaAtiva[destino]) {
        cout << "\n>> Operação não permitida: a conta de destino está INATIVA.\n";
        return;
    }

    cout << "\n--------- TRANSFERÊNCIA ---------\n";
    cout << "De:   conta nº " << numeroConta[origem] << " - " << nomeCliente[origem] << "\n";
    cout << "Para: conta nº " << numeroConta[destino] << " - " << nomeCliente[destino] << "\n";
    cout << "Saldo da conta de origem: R$ " << fixed << setprecision(2) << saldo[origem] << "\n";

    double valor = lerValor("Valor R$: ", false);

    // a origem precisa ter saldo suficiente (comparando em centavos)
    if (centavos(valor) > centavos(saldo[origem])) {
        cout << "\n>> Saldo insuficiente! Saldo disponível: R$ "
             << fixed << setprecision(2) << saldo[origem] << "\n";
        return;
    }

    saldo[origem] = (centavos(saldo[origem]) - centavos(valor)) / 100;
    saldo[destino] = (centavos(saldo[destino]) + centavos(valor)) / 100;

    // saída na origem e entrada no destino: a quantidade sobe 2 no total
    registrarMovimentacao(numeroConta[origem], 'S', valor, numeroConta[destino]);
    registrarMovimentacao(numeroConta[destino], 'D', valor, numeroConta[origem]);

    cout << "\n>> Transferência realizada!\n";
    cout << "   Conta nº " << numeroConta[origem] << " - " << nomeCliente[origem]
         << ": R$ " << fixed << setprecision(2) << saldo[origem] << "\n";
    cout << "   Conta nº " << numeroConta[destino] << " - " << nomeCliente[destino]
         << ": R$ " << fixed << setprecision(2) << saldo[destino] << "\n";
}

// extrato (item 2.6): passo pelo histórico inteiro e mostro só o que é da conta
void exibirExtrato() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    cout << "\n--------- EXTRATO ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao];
    if (!contaAtiva[posicao]) {
        cout << " (INATIVA)";
    }
    cout << "\n\n";
    cout << "Saldo inicial: R$ " << fixed << setprecision(2) << saldoInicial[posicao] << "\n";

    // contaTransferencia diferente de 0 quer dizer que a movimentação veio de
    // uma transferência, aí mostro qual foi a outra conta
    int encontradas = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            encontradas++;

            if (tipoMovimentacao[m] == 'D') {
                if (contaTransferencia[m] != 0) {
                    cout << "(+) Transferência recebida da conta " << contaTransferencia[m];
                } else {
                    cout << "(+) Depósito";
                }
            } else if (tipoMovimentacao[m] == 'S') {
                if (contaTransferencia[m] != 0) {
                    cout << "(-) Transferência enviada para a conta " << contaTransferencia[m];
                } else {
                    cout << "(-) Saque";
                }
            }
            cout << ": R$ " << fixed << setprecision(2) << valorMovimentacao[m] << "\n";
        }
    }

    if (encontradas == 0) {
        cout << "Nenhuma movimentação registrada nesta conta.\n";
    }
    cout << "\nSaldo atual: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";
}

void ativarDesativarConta() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    if (contaAtiva[posicao]) {
        contaAtiva[posicao] = false;
        cout << "\n>> Conta nº " << numeroConta[posicao] << " DESATIVADA.\n";
    } else {
        contaAtiva[posicao] = true;
        cout << "\n>> Conta nº " << numeroConta[posicao] << " ATIVADA.\n";
    }
}

// na Etapa 1 essa era a opção 7 (remover conta). aqui só deixo excluir com
// o saldo zerado, senão o dinheiro sumiria junto com a conta
void excluirConta() {
    int posicao = pedirConta("\nInforme o número da conta: ");
    if (posicao == -1) return;

    cout << "\n--------- EXCLUIR CONTA ---------\n";
    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
    cout << "Saldo: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";

    // com saldo: tem que sacar ou transferir antes
    if (saldo[posicao] > 0) {
        cout << "\n>> Não é possível excluir: a conta ainda tem saldo.\n";
        cout << "   Saque ou transfira o dinheiro antes de encerrar a conta.\n";
        if (!contaAtiva[posicao]) {
            cout << "   (a conta está INATIVA: ative na opção 8 para poder sacar)\n";
        }
        return;
    }
    // hoje nunca fica negativo, mas, se um dia tiver cheque especial, já está aqui
    if (saldo[posicao] < 0) {
        cout << "\n>> Não é possível excluir: a conta está com saldo negativo.\n";
        cout << "   Regularize as pendências antes de encerrar a conta.\n";
        return;
    }

    // conto quantas movimentações vão junto, pra avisar antes de confirmar
    int movimentacoesDaConta = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] == numeroConta[posicao]) {
            movimentacoesDaConta++;
        }
    }

    if (movimentacoesDaConta == 0) {
        cout << "\nA exclusão é permanente: a conta vai ser apagada.\n";
    } else {
        cout << "\nA exclusão é permanente: a conta e as " << movimentacoesDaConta
             << " movimentação(ões) dela vão ser apagadas.\n";
    }
    // pede confirmação: só exclui se responder S
    char resposta;
    cout << "Deseja realmente excluir? (S/N): ";
    cin >> resposta;
    if (cin.fail()) exit(0);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (resposta != 'S' && resposta != 's') {
        cout << "\n>> Exclusão cancelada.\n";
        return;
    }

    int numeroExcluido = numeroConta[posicao];   // guardo antes, porque o vetor vai andar

    // copio pra frente só as movimentações que vão continuar no histórico.
    // "ficaram" diz onde vai a próxima
    int ficaram = 0;
    for (int m = 0; m < quantidadeMovimentacoes; m++) {
        if (contaMovimentacao[m] != numeroExcluido) {
            contaMovimentacao[ficaram] = contaMovimentacao[m];
            tipoMovimentacao[ficaram] = tipoMovimentacao[m];
            valorMovimentacao[ficaram] = valorMovimentacao[m];
            contaTransferencia[ficaram] = contaTransferencia[m];
            ficaram++;
        }
    }
    quantidadeMovimentacoes = ficaram;

    // e puxo as contas que vêm depois uma posição pra trás, pra não ficar buraco
    for (int i = posicao; i < quantidadeContas - 1; i++) {
        numeroConta[i] = numeroConta[i + 1];
        nomeCliente[i] = nomeCliente[i + 1];
        cpf[i] = cpf[i + 1];
        tipoConta[i] = tipoConta[i + 1];
        saldo[i] = saldo[i + 1];
        contaAtiva[i] = contaAtiva[i + 1];
        saldoInicial[i] = saldoInicial[i + 1];
    }
    quantidadeContas--;   // a última posição sobrou, então tem uma conta a menos

    cout << "\n>> Conta nº " << numeroExcluido << " excluída com sucesso.\n";
}
