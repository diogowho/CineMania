# Introdução  

O objetivo do projeto é desenvolver uma aplicação em consola, utilizando a linguagem C ANSI e a abordagem procedural e modular, por forma a implementar um sistema que facilite a gestão de filmes que permita criar, armazenar, organizar e pesquisar informação relacionada com filmes. O sistema deve utilizar vetores, estruturas, manipulação de ficheiros, sub‑programas e módulos. O programa deve permitir inserir filmes, listar os registos e efetuar múltiplos tipos de pesquisa sobre filmes: títulos, atores, géneros e realizadores.  

---  

## Detalhes de Implementação  

### 2.1 Informação dos filmes  

Para cada **filme** deve ser registada informação sobre:  

- **code** – código do filme (deve ser único, não pode ser alterado e nunca pode ser repetido)  
- **title** – título do filme  
- **genres** – géneros em que o filme se encontra listado (um filme pode ter um ou mais destes géneros):  
  - ACTION
  - ADVENTURE 
  - ANIMATION
  - BIOGRAPHY
  - COMEDY 
  - CRIME
  - DRAMA
  - FAMILY
  - FANTASY
  - HISTORY
  - HORROR
  - MUSIC
  - MUSICAL
  - MYSTERY
  - ROMANCE
  - SCI‑FI
  - SPORT
  - THRILLER
  - WAR
  - WESTERN  
- **description** – descrição do filme  
- **director** – realizador do filme  
- **actors** – lista de atores do filme  
- **year** – ano do filme  
- **duration** – duração do filme (em minutos)  
- **rating** – nota do filme (valor entre [0, 10])  
- **favorite** – número de utilizadores que marcaram o filme como favorito  
- **revenue** – receita do filme (em milhões)  

### 2.1.1 Ficheiros Disponibilizados  

Juntamente com este enunciado são disponibilizados 2 ficheiros de entrada, de modo a poderem ser utilizados para testar a importação de filmes:  

- `movies_small.csv`  
- `movies_all.csv`  

Cada ficheiro possui um número arbitrário de filmes, contendo filmes duplicados; pode assumir que nunca existirão mais de 2000 filmes num ficheiro.

Cada filme tem um código respetivo (campo **code**). Pode detetar filmes duplicados quando um código de filme ocorrer mais que uma vez. Filmes duplicados devem ser ignorados durante o processo de importação.

O programa deve importar corretamente qualquer ficheiro de entrada com o mesmo formato e estrutura dos ficheiros *.csv* disponibilizados. Também deve importar corretamente o ficheiro de exportação gerado pelo próprio programa.  

Adicionalmente, é fornecido o ficheiro `movies_all.xlsx` que contém a informação do ficheiro `movies_all.csv` em formato Excel. Pode utilizar este ficheiro para inspecionar a informação de antemão e para validar o resultado de algumas funcionalidades posteriormente.  

### 2.2 Menu

O Menu deve disponibilizar opções para:

1. Listar todos os filmes * (em tabela, com todos os detalhes exceto a descrição) com as seguintes opções:
    - Por ordem crescente ou decrescente do code
    - Visualizar os dados com ou sem paginação (caso o utilizador pretenda paginação, deve apresentar 25 linhas de cada vez)

2. Pesquisar filmes * (com todos os detalhes exceto a descrição em tabela) os resultados devem ser apresentados por ordem alfabética do title e todas as pesquisas devem ser case insensitive:
    - Mostrar todos os filmes cujos títulos contenham a string introduzida
    - Mostrar todos os filmes com o género selecionado
    - Mostrar todos os filmes do realizador introduzido
    - Mostrar todos os filmes com o ator introduzido

3. Consultar informação de um filme (fornecer code)

4. Adicionar filme (o code deve ser único e atribuído automaticamente, os restantes detalhes são fornecidos pelo utilizador)

5. Alterar informação de um filme (fornecer code e permitir editar title, genres, year, duration, rating, favorite e revenue - só podem ser aceites dados válidos!)

6. Eliminar filme (fornecer code)

7. Limpar todos os filmes em memória (solicita confirmação, limpa e apresenta mensagem de sucesso)

8. Importar filmes de ficheiro .csv (solicita nome do ficheiro, no final da importação informa quantos filmes foram importados)

9. Exportar filmes para ficheiro .csv (solicita nome do ficheiro e exporta a informação dos filmes atualmente em memória para ficheiro, em formato CSV. Não deve permitir a exportação se já existir um ficheiro com esse nome.

0. Sair da aplicação (solicita confirmação)

---  

## NOTAS:  

A numeração do **Menu** deve ser mantida na aplicação. Podem adicionar outras funcionalidades que considerem que se justifiquem, mas sempre com numeração superior a 9.  

Devem ser efetuadas todas as validações necessárias (por exemplo, verificar se os valores introduzidos são válidos, se têm o formato esperado, etc.).  

Os dados relativos às capacidades máximas, por exemplo, número máximo de **2000 filmes**, deve ser definido como constante simbólica.  

---  

## 3 Regras de Desenvolvimento e Entrega do Projeto

### 3.2 Implementação e Codificação  

O programa deve ser desenvolvido utilizando a linguagem C ANSI, colocando em prática os conceitos fundamentais do paradigma procedural e modular e é permitida a utilização das bibliotecas disponíveis.  

Em relação às regras de codificação, **siga as convenções adotadas normalmente no decorrer da UC de AFP**:  

- **camelCase** para os nomes de sub‑programas e dos seus parâmetros, de variáveis locais e de variáveis  
- **PascalCase** para os nomes dos novos tipos/estruturas  
- **Não utilize** o símbolo **'underscore'** nos identificadores, nem abreviaturas no início das mesmas  

É necessário que o projeto cumpra o que é pedido no enunciado, ficando ao critério do programador qualquer pormenor de implementação que não seja referido (o qual deve ser devidamente documentado).  

Nas funcionalidades desenvolvidas devem ser incluídas todas as validações necessárias para impedir um comportamento incorreto do sistema.  

Sempre que fizer sentido, os sub‑programas devem ser responsáveis por **apresentar no ecrã mensagens de informação e/ou erro indicando o processamento que foi realizado**.  

As mensagens apresentadas pela aplicação podem ser escritas em português ou inglês.
