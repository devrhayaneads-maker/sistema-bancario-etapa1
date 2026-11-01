// NOVABANK - Etapa 2
// INF101 - Programação de Computadores I - UNIVIÇOSA
// Rhayane Freitas
//
// Agora são várias contas (até 100) e cada depósito, saque e transferência
// fica guardado num histórico. Tudo em vetor: no desafio das 5 contas da
// Etapa 1 eu já tinha visto que criar numeroConta1, numeroConta2... não tem
// como. Fiz tudo dentro do main(), como o enunciado permite. A versão
// separada em funções (o desafio do fim do enunciado) está no
// desafio_funcoes.cpp.
//
// Os dados ficam só na memória: fechou o programa, perdeu tudo. Salvar em
// arquivo fica para a Etapa 3.

#include <iostream>  // cin e cout
#include <string>
#include <iomanip>   // setprecision e setw, pra formatar os números
#include <limits>    // numeric_limits, usado no cin.ignore
#include <cmath>     // round, pra acertar os centavos

#ifdef _WIN32        // só entra aqui quando compila no Windows
#include <windows.h> // sem isso os acentos saem errados no terminal do Windows
#endif

using namespace std;   // pra não ter que escrever std::cout

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // o que o programa escreve sai com acento
    // a entrada também. testando com o nome João, a tela mostrava Jo?o, porque
    // o que a pessoa digitava chegava num formato e saía em outro
    SetConsoleCP(CP_UTF8);
#endif

    // contas (item 2.1). os vetores são "paralelos": a posição i de todos eles
    // é a mesma conta. ex: numeroConta[0], nomeCliente[0] e saldo[0] são da
    // primeira conta cadastrada
    const int MAX_CONTAS = 100;
    int numeroConta[MAX_CONTAS];
    string nomeCliente[MAX_CONTAS];
    string cpf[MAX_CONTAS];
    int tipoConta[MAX_CONTAS];   // 1 = corrente, 2 = poupança
    double saldo[MAX_CONTAS];
    bool contaAtiva[MAX_CONTAS];
    double saldoInicial[MAX_CONTAS];   // esse eu acrescentei, pro extrato mostrar de onde a conta partiu
    int quantidadeContas = 0;          // quantas posições já estão ocupadas

    // histórico (item 2.2). cada posição é uma movimentação
    const int MAX_MOVIMENTACOES = 500;
    int contaMovimentacao[MAX_MOVIMENTACOES];     // de qual conta é a movimentação
    char tipoMovimentacao[MAX_MOVIMENTACOES];     // 'D' = entrada, 'S' = saída
    double valorMovimentacao[MAX_MOVIMENTACOES];
    int quantidadeMovimentacoes = 0;

    // esse vetor eu acrescentei. a transferência vira um S e um D (item 2.5) e,
    // sem ele, o extrato mostrava "Saque" para quem transferiu, igualzinho a um
    // saque normal. aqui fica o número da outra conta da transferência, ou 0
    int contaTransferencia[MAX_MOVIMENTACOES];

    // limite pra cada valor digitado. com número muito grande o double começa
    // a perder os centavos, e ninguém deposita mais de 1 bilhão de uma vez
    const double VALOR_MAXIMO = 1000000000.00;

    int opcao = 0;

    // o menu fica repetindo (do-while) até a pessoa escolher 0 e confirmar
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

        // lê a opção. esse jeito de ler número (veio da Etapa 1) aparece várias
        // vezes no programa e serve para letra digitada não travar o cin
        cin >> opcao;
        if (cin.fail()) {   // deu erro na leitura (digitou letra, por exemplo)
            if (cin.eof()) return 0;   // a entrada acabou, não dá para perguntar de novo
            cin.clear();    // tira o erro, senão o cin fica travado
            opcao = -1;     // -1 e não 0, porque agora o 0 é o sair
        } else {
            // pula o espaço e o TAB ('\t') que sobraram depois do número. o peek
            // espia o próximo caractere sem tirar ele da entrada, e o get tira
            while (cin.peek() == ' ' || cin.peek() == '\t') {
                cin.get();
            }
            // se antes do ENTER ('\n') ainda sobrou alguma coisa, foi letra
            // colada no número (ex: 12abc)
            if (!cin.eof() && cin.peek() != '\n') {
                opcao = -1;
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');   // joga fora o resto da linha, até o ENTER

        // da opção 3 em diante, todas trabalham com uma conta que já existe.
        // então pergunto o número aqui, antes do switch, e procuro a posição
        // dela nos vetores. assim não preciso repetir isso em cada opção
        int posicao = -1;   // -1 quer dizer que nenhuma conta foi encontrada

        if (opcao >= 3 && opcao <= 9) {
            if (quantidadeContas == 0) {
                cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
            } else if (opcao == 6 && quantidadeContas < 2) {
                cout << "\n>> Para transferir é preciso ter pelo menos duas contas cadastradas.\n";
            } else {
                int numeroDigitado = 0;
                if (opcao == 6) {
                    cout << "\nConta de origem: ";
                } else {
                    cout << "\nInforme o número da conta: ";
                }
                // mesma leitura de número do menu
                cin >> numeroDigitado;
                if (cin.fail()) {
                    if (cin.eof()) return 0;
                    cin.clear();
                    numeroDigitado = 0;
                } else {
                    while (cin.peek() == ' ' || cin.peek() == '\t') {
                        cin.get();
                    }
                    if (!cin.eof() && cin.peek() != '\n') {
                        numeroDigitado = 0;
                    }
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (numeroDigitado <= 0) {
                    cout << "\n>> Número de conta inválido.\n";
                } else {
                    // busca sequencial: olho uma posição de cada vez até achar a
                    // conta com esse número e guardo a posição dela
                    for (int i = 0; i < quantidadeContas; i++) {
                        if (numeroConta[i] == numeroDigitado) {
                            posicao = i;
                        }
                    }
                    if (posicao == -1) {
                        cout << "\n>> Conta " << numeroDigitado << " não encontrada.\n";
                    }
                }
            }
        }

        switch (opcao) {

            // 1 - cadastrar
            case 1: {
                // o vetor tem tamanho fixo, então preciso testar se ainda cabe
                if (quantidadeContas >= MAX_CONTAS) {
                    cout << "\n>> Limite de " << MAX_CONTAS << " contas atingido.\n";
                    break;
                }

                int i = quantidadeContas;   // próxima posição livre: a conta nova vai aqui
                cout << "\n--------- CADASTRO DE CONTA ---------\n";

                // número da conta: tem que ser maior que zero e não pode repetir
                bool numeroValido;
                do {
                    cout << "Número da conta....: ";
                    cin >> numeroConta[i];
                    if (cin.fail()) {
                        if (cin.eof()) return 0;
                        cin.clear();
                        numeroConta[i] = 0;
                    } else {
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        if (!cin.eof() && cin.peek() != '\n') {
                            numeroConta[i] = 0;
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    numeroValido = (numeroConta[i] > 0);
                    if (!numeroValido) {
                        cout << "   >> Inválido! O número da conta deve ser um inteiro maior que zero.\n";
                    } else {
                        // percorro as contas que já existem pra não repetir número
                        for (int j = 0; j < quantidadeContas; j++) {
                            if (numeroConta[j] == numeroConta[i]) {
                                numeroValido = false;
                            }
                        }
                        if (!numeroValido) {
                            cout << "   >> Esse número de conta já está cadastrado.\n";
                        }
                    }
                } while (!numeroValido);

                bool nomeValido;
                do {
                    cout << "Nome do titular....: ";
                    // getline lê a linha inteira. o cin >> para no primeiro espaço,
                    // e de "Maria Silva" só pegaria o "Maria"
                    getline(cin, nomeCliente[i]);
                    if (cin.fail()) return 0;   // a entrada acabou no meio do cadastro

                    // mesma regra da Etapa 1: precisa ter pelo menos uma letra e
                    // caber em 80 caracteres. length() é o tamanho do texto, e o
                    // (int) é só porque o compilador avisa quando compara int com
                    // o tipo que o length devolve
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
                    if (cin.fail()) return 0;

                    cpfValido = (cpf[i].length() == 11);
                    // confiro caractere por caractere se é um dígito de '0' a '9'
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
                    cin >> tipoConta[i];
                    if (cin.fail()) {
                        if (cin.eof()) return 0;
                        cin.clear();
                        tipoConta[i] = 0;
                    } else {
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        if (!cin.eof() && cin.peek() != '\n') {
                            tipoConta[i] = 0;
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (tipoConta[i] != 1 && tipoConta[i] != 2) {
                        cout << "   >> Inválido! Digite 1 para Corrente ou 2 para Poupança.\n";
                    }
                } while (tipoConta[i] != 1 && tipoConta[i] != 2);

                // valor em dinheiro eu leio como texto, igual na Etapa 1: o cin >>
                // aceitava 2.000 como 2 reais e 1500,50 como 1500, sem reclamar
                cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
                bool saldoValido;
                do {
                    cout << "Saldo inicial R$...: ";
                    string valorDigitado;
                    getline(cin, valorDigitado);
                    if (cin.fail()) return 0;

                    // tira o espaço que a pessoa deixou nas pontas. empty() diz se
                    // o texto está vazio, e erase(posição, 1) apaga um caractere
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
                    saldoValido = false;
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
                        saldoValido = true;
                    }

                    if (saldoValido) {
                        // monto o número na mão: vou juntando os algarismos e conto
                        // quantos vieram depois da vírgula, para dividir no final.
                        // ex: "1500,50" vira 150050 / 100 = 1500.50
                        saldo[i] = 0.0;
                        double divisor = 1.0;
                        bool depoisDoSeparador = false;

                        for (int c = 0; c < (int) valorDigitado.length(); c++) {
                            char letra = valorDigitado[c];
                            if (letra == '.' || letra == ',') {
                                depoisDoSeparador = true;
                            } else if (letra >= '0' && letra <= '9') {
                                saldo[i] = saldo[i] * 10 + (letra - '0');
                                if (depoisDoSeparador) {
                                    divisor = divisor * 10;
                                }
                            }
                        }
                        saldo[i] = saldo[i] / divisor;

                        if (saldo[i] > VALOR_MAXIMO) {
                            saldoValido = false;
                            cout << "   >> Inválido! O valor máximo por operação é R$ "
                                 << fixed << setprecision(2) << VALOR_MAXIMO << "\n";
                        }
                    }
                } while (!saldoValido);

                saldoInicial[i] = saldo[i];   // guardo pra mostrar no extrato
                contaAtiva[i] = true;   // conta nova já nasce ativa
                quantidadeContas++;     // só agora a posição i passa a valer

                cout << "\n>> Conta " << numeroConta[i] << " cadastrada com sucesso e ATIVADA!\n";
                break;
            }

            // 2 - listar todas as contas numa tabela
            case 2:
                if (quantidadeContas == 0) {
                    cout << "\n>> Nenhuma conta cadastrada. Use a opção 1.\n";
                    break;
                }

                cout << "\n--------- CONTAS CADASTRADAS ---------\n";
                // o nome ficou por último porque o setw conta letra com acento
                // como duas e desalinhava o resto da linha
                cout << "     Conta  Tipo      Situação     Saldo (R$)  Titular\n";
                for (int i = 0; i < quantidadeContas; i++) {
                    // setw(10) faz o próximo valor ocupar 10 espaços, encostado à
                    // direita. é o que deixa as colunas alinhadas
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
                    // fixed com setprecision(2) mostra sempre duas casas: 1000.00
                    cout << setw(15) << fixed << setprecision(2) << saldo[i]
                         << "  " << nomeCliente[i] << "\n";
                }
                cout << "\nTotal: " << quantidadeContas << " conta(s)\n";
                break;

            // 3 - consultar os dados de uma conta
            case 3: {
                if (posicao == -1) break;   // a busca lá em cima não achou e já avisou

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
                break;
            }

            // 4, 5 e 6 - depósito, saque e transferência
            // os três fazem quase tudo igual (conferir se a conta está ativa, se
            // cabe no histórico e ler o valor), então deixei no mesmo case e só
            // no final separo o que cada um faz
            case 4:
            case 5:
            case 6: {
                if (posicao == -1) break;

                // conta inativa não movimenta dinheiro
                if (!contaAtiva[posicao]) {
                    if (opcao == 6) {
                        cout << "\n>> Operação não permitida: a conta de origem está INATIVA.\n";
                    } else {
                        cout << "\n>> Operação não permitida: a conta está INATIVA.\n";
                    }
                    break;
                }

                // a transferência grava dois registros e os outros só um. confiro
                // o espaço antes de tudo, pra nunca gravar a saída e faltar lugar
                // pra entrada
                int registros = 1;
                if (opcao == 6) {
                    registros = 2;
                }
                if (quantidadeMovimentacoes + registros > MAX_MOVIMENTACOES) {
                    if (quantidadeMovimentacoes < MAX_MOVIMENTACOES) {
                        // só acontece na transferência, com 499 de 500
                        cout << "\n>> Sem espaço no histórico para a transferência: ela grava 2 movimentações e só resta 1.\n";
                    } else {
                        cout << "\n>> Histórico de movimentações cheio (limite de "
                             << MAX_MOVIMENTACOES << ").\n";
                    }
                    break;
                }

                // só a transferência precisa de uma segunda conta, o destino. confiro
                // se ele existe, se é outra conta e se está ativo (item 2.5)
                int destino = -1;   // posição da conta de destino nos vetores
                if (opcao == 6) {
                    int numeroDestino = 0;
                    cout << "Conta de destino: ";
                    cin >> numeroDestino;
                    if (cin.fail()) {
                        if (cin.eof()) return 0;
                        cin.clear();
                        numeroDestino = 0;
                    } else {
                        while (cin.peek() == ' ' || cin.peek() == '\t') {
                            cin.get();
                        }
                        if (!cin.eof() && cin.peek() != '\n') {
                            numeroDestino = 0;
                        }
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    if (numeroDestino <= 0) {
                        cout << "\n>> Número de conta inválido.\n";
                        break;
                    }
                    for (int i = 0; i < quantidadeContas; i++) {
                        if (numeroConta[i] == numeroDestino) {
                            destino = i;
                        }
                    }
                    if (destino == -1) {
                        cout << "\n>> Conta " << numeroDestino << " não encontrada.\n";
                        break;
                    }
                    if (destino == posicao) {
                        cout << "\n>> A conta de destino precisa ser diferente da conta de origem.\n";
                        break;
                    }
                    if (!contaAtiva[destino]) {
                        cout << "\n>> Operação não permitida: a conta de destino está INATIVA.\n";
                        break;
                    }
                }

                // mostra o título e os dados da operação antes de pedir o valor
                if (opcao == 4) {
                    cout << "\n--------- DEPÓSITO ---------\n";
                } else if (opcao == 5) {
                    cout << "\n--------- SAQUE ---------\n";
                } else {
                    cout << "\n--------- TRANSFERÊNCIA ---------\n";
                }

                if (opcao == 6) {
                    cout << "De:   conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
                    cout << "Para: conta nº " << numeroConta[destino] << " - " << nomeCliente[destino] << "\n";
                    cout << "Saldo da conta de origem: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";
                } else {
                    cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
                    cout << "Saldo atual: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";
                }

                // mesma leitura do saldo inicial, só que aqui o zero não vale
                double valor = 0.0;
                cout << "(centavos com ponto ou vírgula, ex: 1500.50 - sem ponto de milhar)\n";
                bool valorValido;
                do {
                    cout << "Valor R$: ";
                    string valorDigitado;
                    getline(cin, valorDigitado);
                    if (cin.fail()) return 0;

                    // tira os espaços das pontas
                    while (!valorDigitado.empty() &&
                           (valorDigitado[0] == ' ' || valorDigitado[0] == '\t')) {
                        valorDigitado.erase(0, 1);
                    }
                    while (!valorDigitado.empty() &&
                           (valorDigitado[valorDigitado.length() - 1] == ' ' ||
                            valorDigitado[valorDigitado.length() - 1] == '\t')) {
                        valorDigitado.erase(valorDigitado.length() - 1, 1);
                    }

                    // confiro caractere por caractere, igual no saldo inicial
                    bool soNumero = true;
                    int digitos = 0;
                    bool negativo = false;
                    int separadores = 0;
                    int casasDecimais = 0;

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

                    valorValido = false;
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
                        valorValido = true;
                    }

                    // monto o número na mão, igual no saldo inicial
                    if (valorValido) {
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
                            valorValido = false;
                            cout << "   >> Inválido! O valor máximo por operação é R$ "
                                 << fixed << setprecision(2) << VALOR_MAXIMO << "\n";
                        } else if (valor == 0) {
                            valorValido = false;
                            cout << "   >> Inválido! O valor deve ser maior que zero.\n";
                        }
                    }
                } while (!valorValido);

                if (opcao == 4) {
                    // depósito: soma no saldo. toda conta com dinheiro eu faço em
                    // centavos inteiros: round(x * 100) transforma 0.70 em 70, aí
                    // somo ou tiro e divido por 100 pra voltar pra reais.
                    // testando, vi que 0.70 + 0.10 dava 0.7999999 no double: na tela
                    // aparecia R$ 0.80, mas sacar os 0.80 dava saldo insuficiente.
                    // em centavos inteiros esse erro não acontece
                    saldo[posicao] = (round(saldo[posicao] * 100) + round(valor * 100)) / 100;

                    // grava a movimentação 'D' no histórico, igual ao exemplo do item 2.4
                    contaMovimentacao[quantidadeMovimentacoes] = numeroConta[posicao];
                    tipoMovimentacao[quantidadeMovimentacoes] = 'D';
                    valorMovimentacao[quantidadeMovimentacoes] = valor;
                    contaTransferencia[quantidadeMovimentacoes] = 0;
                    quantidadeMovimentacoes++;

                    cout << "\n>> Depósito realizado! Novo saldo: R$ "
                         << fixed << setprecision(2) << saldo[posicao] << "\n";

                } else if (round(valor * 100) > round(saldo[posicao] * 100)) {
                    // daqui pra baixo é saque ou transferência, que tiram dinheiro.
                    // se o valor passa do saldo, não mexe em nada e só avisa. a
                    // comparação também é em centavos: num compilador de 32 bits,
                    // comparando direto, sacar o saldo inteiro dava insuficiente
                    cout << "\n>> Saldo insuficiente! Saldo disponível: R$ "
                         << fixed << setprecision(2) << saldo[posicao] << "\n";

                } else if (opcao == 5) {
                    // saque: tira do saldo (em centavos) e grava um 'S'
                    saldo[posicao] = (round(saldo[posicao] * 100) - round(valor * 100)) / 100;

                    contaMovimentacao[quantidadeMovimentacoes] = numeroConta[posicao];
                    tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                    valorMovimentacao[quantidadeMovimentacoes] = valor;
                    contaTransferencia[quantidadeMovimentacoes] = 0;
                    quantidadeMovimentacoes++;

                    cout << "\n>> Saque realizado! Novo saldo: R$ "
                         << fixed << setprecision(2) << saldo[posicao] << "\n";

                } else {
                    // transferência: tira da origem e põe no destino, em centavos
                    saldo[posicao] = (round(saldo[posicao] * 100) - round(valor * 100)) / 100;
                    saldo[destino] = (round(saldo[destino] * 100) + round(valor * 100)) / 100;

                    // saída na conta de origem
                    contaMovimentacao[quantidadeMovimentacoes] = numeroConta[posicao];
                    tipoMovimentacao[quantidadeMovimentacoes] = 'S';
                    valorMovimentacao[quantidadeMovimentacoes] = valor;
                    contaTransferencia[quantidadeMovimentacoes] = numeroConta[destino];

                    // entrada na conta de destino, na posição seguinte do histórico
                    contaMovimentacao[quantidadeMovimentacoes + 1] = numeroConta[destino];
                    tipoMovimentacao[quantidadeMovimentacoes + 1] = 'D';
                    valorMovimentacao[quantidadeMovimentacoes + 1] = valor;
                    contaTransferencia[quantidadeMovimentacoes + 1] = numeroConta[posicao];

                    quantidadeMovimentacoes = quantidadeMovimentacoes + 2;   // uma transferência, dois registros (item 2.5)

                    cout << "\n>> Transferência realizada!\n";
                    cout << "   Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao]
                         << ": R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";
                    cout << "   Conta nº " << numeroConta[destino] << " - " << nomeCliente[destino]
                         << ": R$ " << fixed << setprecision(2) << saldo[destino] << "\n";
                }
                break;
            }

            // 7 - extrato
            case 7: {
                if (posicao == -1) break;

                cout << "\n--------- EXTRATO ---------\n";
                cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao];
                if (!contaAtiva[posicao]) {
                    cout << " (INATIVA)";
                }
                cout << "\n\n";
                cout << "Saldo inicial: R$ " << fixed << setprecision(2) << saldoInicial[posicao] << "\n";

                // percorro o histórico inteiro e mostro só o que é dessa conta
                // (item 2.6). contaTransferencia diferente de 0 quer dizer que a
                // movimentação veio de uma transferência, aí mostro a outra conta
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
                break;
            }

            // 8 - ativa se estiver inativa, e o contrário
            case 8:
                if (posicao == -1) break;

                if (contaAtiva[posicao]) {
                    contaAtiva[posicao] = false;
                    cout << "\n>> Conta nº " << numeroConta[posicao] << " DESATIVADA.\n";
                } else {
                    contaAtiva[posicao] = true;
                    cout << "\n>> Conta nº " << numeroConta[posicao] << " ATIVADA.\n";
                }
                break;

            // 9 - excluir de vez (na Etapa 1 era a opção 7, remover conta)
            case 9: {
                if (posicao == -1) break;

                cout << "\n--------- EXCLUIR CONTA ---------\n";
                cout << "Conta nº " << numeroConta[posicao] << " - " << nomeCliente[posicao] << "\n";
                cout << "Saldo: R$ " << fixed << setprecision(2) << saldo[posicao] << "\n";

                // banco de verdade não encerra conta com dinheiro dentro, senão o
                // dinheiro sumiria junto com ela. tem que sacar ou transferir antes
                if (saldo[posicao] > 0) {
                    cout << "\n>> Não é possível excluir: a conta ainda tem saldo.\n";
                    cout << "   Saque ou transfira o dinheiro antes de encerrar a conta.\n";
                    if (!contaAtiva[posicao]) {
                        cout << "   (a conta está INATIVA: ative na opção 8 para poder sacar)\n";
                    }
                    break;
                }
                // hoje o saldo nunca fica negativo, porque o saque e a transferência
                // conferem antes. mas, se um dia tiver cheque especial, a regra já está aqui
                if (saldo[posicao] < 0) {
                    cout << "\n>> Não é possível excluir: a conta está com saldo negativo.\n";
                    cout << "   Regularize as pendências antes de encerrar a conta.\n";
                    break;
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
                // pede confirmação, igual na Etapa 1: só exclui se responder S
                char resposta;
                cout << "Deseja realmente excluir? (S/N): ";
                cin >> resposta;
                if (cin.fail()) return 0;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (resposta != 'S' && resposta != 's') {
                    cout << "\n>> Exclusão cancelada.\n";
                    break;
                }

                int numeroExcluido = numeroConta[posicao];   // guardo antes, porque o vetor vai andar

                // tiro as movimentações dela do histórico: ando pelo vetor copiando
                // pra frente só as que vão ficar. "ficaram" diz onde vai a próxima
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

                // e puxo as contas que vêm depois uma posição pra trás, pra não
                // ficar buraco no meio do vetor
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
                break;
            }

            // 0 - sair
            case 0: {
                // nada fica salvo nesta etapa, então confirmo antes de sair, pra
                // não perder tudo por causa de um 0 digitado sem querer
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
