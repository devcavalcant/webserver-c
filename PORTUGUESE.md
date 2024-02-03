<h3 align="center">Servidor Web em C com sockets</h3>

O projeto consiste na implementação de um servidor web de forma pura em C, utilizando apenas bibliotecas providas de forma padrão. O servidor tem a capacidade de devolver uma página *fallback.html* em casos onde a página requerida pelo cliente não seja encontrada ou não exista. Adicionalmente, o servidor conta com a possibilidade de encontrar diversas páginas web que sejam desejadas e, em casos onde não há a requisição de uma página em específico (ou seja, buscando pela rota "/"), é retornada a página *index.html*.

Um servidor Web é um dos diversos serviços que estão na rede, desde aplicações mais simples até as mais complexas. Para comunicação desses serviços, é utilizado uma relação cliente-servidor, que trabalha de uma forma mais centralizada (diferente do p2p). O servidor trabalha de forma reativa às interações externas, sendo assim, é via de regra que a primeira comunicação seja realizada pelo cliente, onde realiza-se o ínicio da comunicação (*3way handshake*).

#### Motivação

O estudo em si de um servidor web por muitas vezes é visto e aplicado de uma forma alto nível e facilitada, o que ajuda os desenvolvedores e empresas que desejam criar um serviço, contudo, para um estudo mais significativo e baixo nível, foi escolhido a linguagem C, conhecida pela sua característica de ser mais próxima às ideias do hardware em comparação a outras do mercado.
