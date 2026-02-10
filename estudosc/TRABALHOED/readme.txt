ola professor, seguem aqui minhas considerações sobre o trabalho:
1) para acessar os registros em cada um dos programas, criei uma chave para cada cpf gerado, a chave consiste nos primeiros 9 digitos
 do cpf como especificado no enunciado do trabalho, sem os 2 digitos verificadores, portanto, no arquivo registros.txt que é usado em todos os programas
a primeira sequencia que aparece em cada linha é essa chave de 9 digitos.
2) falando sobre o segmento de geração de nomes, cpf e nota, utilizei a seguinte lógica:
nome: criei um vetor contendo cada letra do alphabeto e num espaço de 50 caracteres eu fiz um for para que cada letra fosse aleatória
nota: uma nota aleatória de 1 a 100
cpf: numeros aleatorios também
3)o código de heap foi usado heap máximo, onde a nota do pai sempre é maior ou igual a dos filhos,  então o topo tem a Maior nota
pra inserir, inserimos no final e mandamos subir, e para remover, a gente remove o elemento do topo e pega o último
elemento do array pra ficar no lugar dele, depois mandamos ele descer pra reajustar a prioridade. Implementei uma opção para salvar dados
manualmente porém não adicionei nas outras porque não consegui fazer funcionar.
4) o código da hash, a gente usa a chave como índice para guardar o elemento na tabela, e na inserção, se der colisão, usamos endereçamento
aberto como no enunciado, e usamos tentativa linear, ou seja, caso colida, procuramos o endereço livre em seguida até encontrar um.
para remover temos 3 estados de status, 0 para caso esteja livre e nao tenha sido usado, 1 para ocupado e -1 para vazio porém tinha algo ali, então ele deve ser considerado um espaço na tabela
5) a implementação da arvore B+ cria índices na árvore que apontam pros registros no arquivo, atraves da chave e a posição
quando buscamos uma chave, a árvore retorna a posição e utilizando o fseek na posição acessamos o registro desejado.
a função de listar todos foi bem mais simples na arvore B+ devido ao ponteiro entre os nós, desse modo a gente acha o primeiro nó folha
e vai indo pro ponteiro da direita depois em ordem. Na arvore adicionei uma opção no menu dela de gerar um arquivo novo porém decidi não
incluir no heap e hash pois não estava conseguindo fazer funcionar.
6) eu tive um problema recorrente que era, eu consegui fazer o programa adicionar e remover, porem não alterava o arquivo, pra resolver isso
antes do menu, tem uma opçaão de usar o arquivo ja existente ou criar um novo(reset) ai agora fica mais dinâmico também.
ass: Felipe Cabral Liporage

