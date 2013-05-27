asa-jobs
========

Second project assignment for the Analysis and Synthesis of Algorithms course @ IST 2013

Each student is interpreted as a vertex in a subset S of V.
Each job is also interpreted as a vertex, in V-S.
Maximum matching computed using Hopcroft-Karp's algorithm.

- Problem
-
O mercado de trabalho é complexo. Os alunos tiram cursos para poderem concorrer a empregos. Por outro lado, as empresas analisam os candidatos para determinar se estão aptos para realizar determinadas tarefas. Quando os dois interesses estão alinhados, os alunos conseguem emprego.
Caso contrário, o desemprego cresce e a economia cresce menos.

Neste projecto iremos fazer uma simulação simples.
Cada aluno irá produzir uma lista de empregos que ele, ou ela, deseja.
As empresas, por outro lado, irão produzir uma lista com os alunos que aceitam contratar. Os nosso objectivo é melhorar a vida de todos, maximizando o número de empregos atribuídos. Cada aluno pode ocupar apenas um emprego, o que torna a tarefa algo complicada.

- Input
- 
A primeira linha do input contém dois inteiros. O primeiro indica o número de alunos e o segundo inteiro o número de empregos. Seguem-se as listas de empregos, que estão alinhadas com os interesses das empresas. Cada aluno indica uma lista com os empregos que deseja e para os quais é simultaneamente aceite pela respectiva empresa. Cada lista é dada por uma linha de números inteiros. A linha começa com o número de empregos que vão constar da lista. Os restantes números da linha correspondem a números de emprego. Os empregos começam a ser numerados em 0. Numa linha os inteiros são separados por um espaço em branco, e as linhas não contém mais caracteres, excepto o fim de linha.

- Output
- 
Para cada caso de testes, o output deve consistir numa só linha, que contém o número máximo de empregos que é possível atribuir.

- Examples
- 

- Input 1<br/>
2 2<br/>
2 0 1<br/>
2 0 1<br/>

- Output 1<br/>
2<br/>

- Input 2<br/>
2 2<br/>
1 0<br/>
1 0<br/>

- Output 2<br/>
1<br/>
