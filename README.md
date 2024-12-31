# MAC 0499 - Trabalho de Conclusão de curso
## Aluno: Pedro Zamecki Andrade
## Orientador: Prof. Paulo Vechiatto de Miranda

# Projeto: USPKart - Jogo de corrida 3D em OpenGL
### Resumo: Este projeto consiste em um jogo de corrida 3D desenvolvido em OpenGL, utilizando a linguagem de programação C++. O jogo foi desenvolvido como trabalho de conclusão de curso para o curso de Bacharelado em Ciência da Computação da Universidade de São Paulo. O jogo foi desenvolvido utilizando a biblioteca gráfica OpenGL e a biblioteca de áudio OpenAL.

## Instruções para executar o projeto

### Pré-requisitos
- CMake 3.8 ou superior
- Compilador C++ (GCC, Clang, MSVC, etc.)

### Passos para compilar e executar

1. Clone o repositório:
   ```sh
   git clone https://github.com/PedroZamecki/USPKart.git
   cd USPKart
   ```

2. Crie um diretório de build:
   ```sh
   mkdir build
   cd build
   ```

3. Configure o projeto com CMake:
   - Para modo DEBUG:
     ```sh
     cmake -DCMAKE_BUILD_TYPE=Debug ..
     ```
   - Para modo RELEASE:
     ```sh
     cmake -DCMAKE_BUILD_TYPE=Release ..
     ```

4. Compile o projeto:
   ```sh
   cmake --build .
   ```

5. Execute o projeto:
   ```sh
   ./USPKart
   ```

### Notas
- O script CMake irá automaticamente baixar e configurar o vcpkg se ele não estiver presente.
- Certifique-se de que você tem uma conexão com a internet para que o vcpkg possa ser baixado.
