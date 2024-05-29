# Snake Game Com Curses
Um pequeno clone do famoso jogo da cobrinha usando curses.

O projeto foi feito com o foco em sistemas Linux/UNIX-like e a biblioteca NCurses. Porém, também pode ser compilado para sistemas Windows usando PDCurses.

Este projeto foi feito apenas como forma de estudo.

### ATENÇÃO ⚠️
PDcurses é bem limitado se comparado a NCurses. Se compilado com a segunda, o jogo funcionará mas pode apresentar algumas limitações.

## Como Compilar
É necessário o compilador GCC e um leitor de Makefiles. Em distros linux esses utilitários são facilmente encontrados em pacotes como `build-essential` no caso de sistemas com base em Debian.
Para Windows, os scripts de montagem foram feitos com base no projeto MinGW, uma implementação do GCC e Make para sistemas Windows.
### Linux/Unix-like
Tendo a biblioteca NCurses já instalada, basta usar o comando:
```
make
```
### Windows
Primeiro, é preciso baixar e montar a biblioteca PDCurses:
```
mingw32-make.exe build-curses
```
Depois, compilar o jogo usando a biblioteca já montada:
```
mingw32-make.exe compile-with-local-curses
```
## Aprendendo a Jogar
O jogo usa um sistema de perfis para salvar os dados do jogador. Assim que o jogo iniciar ele pedirá pelo nome do usuário e criará um perfil com ele.

Para jogar selecione `Inicar`. Os botões de locomoção são: `W`, `A`, `S`, `D` além das setas do teclado.

No menu de opções é possível ver a melhor pontuação e a pontuação mais recente do perfil. Além de poder alterar a largura da grade e as cores dos objetos do jogo.

Com a opção de salvar perfil, o jogo cria um arquivo com a extensão `.data` com o nome do usuário, sendo possível carrega-lo com a opção correspondente digitando o nome
do perfil.

O jogo armazena os dados do perfil em uso em um arquivo chamado de `data.data`. Todo o perfil para ser usado precisa antes ser carregado para esse arquivo. Portanto, antes de mudar de perfil, sempre salve-o para não perder o progresso feito.

Criar um novo perfil significa sobrescrever todos os dados atuais do arquivo `data.data`. Portanto, salve o perfil atual caso queira usa-lo novamente.
