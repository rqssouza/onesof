OneSoft C++ Coding Exercise: Simple Server
============

Introdução
------------

Para o exercício eu utilizei o bando de dados mongodb para armazenar as linhas lidas do arquivo e utilizei 
a lib zeromq para trafegar as mensagens entre os binários. 

Ao compilar o projeto serão gerados 5 executáveis:

messageBroker: Utilizado como proxy de mensagens entre os outros binários do projeto.

server: É o executável que fica ouvindo as requisições para operações no banco de dados. O server suporta
inserção no banco queries de alguns campos e também notifica quando houve uma nova entrada no banco.

push_client: É o executável que lê as linhas do arquivo DOLF18.txt e as envia para o server para serem
armazenadas. O programa lê as linhas e as envia respeitando timestamp de cada linha.

subscribed_client: Este executável apenas se cadastra no servidor para ser notificado quando uma nova
linha é inserida no bando de dados. Quando uma nova notificação chega ela é impressa na saída padrão.

query_client: Este executável envia três queries para o servidor. Uma pedindo todas as linhas cujo o
campo askBroker = 27, outra pedindo as linhas com o bidBroker = 86 e a última pedindo as linhas com a
data hora da sessão entre 08/12/2017 9:00:00 e 08/12/2017 9:10:00.


Dependências
------------------

As dependências do projeto são as seguintes:

mongodb: Banco utilizado pelo servidor para armazenar as linhas enviadas por tcp.(https://www.mongodb.com/)

mongo-cxx-driver: Wraper c++ utilizado para acessar a instância do mongodb.(https://github.com/mongodb/mongo-cxx-driver)

zmq: Fila de mensagens utilizada para trocar mensagens via tcp entre os binários do projeto.(https://github.com/zeromq/libzmq)

zmqpp: Wraper alto c++ para a libzmq.(https://github.com/zeromq/zmqpp)

boost: Foi utilizada a libboost_serialization da boost para serializar e desserializar os objetos que eram
enviados por tcp. Também foi utilizada libboost_program_options para fazer o parse dos argumentos passados
por linha de comando para os binários.


Compilação
-------------------

Estando instaladas as dependências citadas acima basta executar o cmake para configurar gerar o Makefile
e em seguida executar o make para compilar os binários

    mkdir build
    cd build
    cmake ..
    make


Tempo gasto
---------

Por volta de 30 horas.
