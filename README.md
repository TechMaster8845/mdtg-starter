# MDTG Starter Project  
MultiDimensional Tree Graph Engine (C++20)

O MDTG (MultiDimensional Tree Graph) é um engine moderno em C++20 projetado para servir como 
estrutura de dados central em pipelines complexos, incluindo:

- Compiladores IFC/BIM  
- Motores de análise geométrica  
- Motores de simulação  
- Processamento massivo de grafos hierárquicos  
- Sistemas paralelos baseados em árvores multidimensionais  

Este repositório fornece a base mínima, limpa e reutilizável, para iniciar qualquer projeto
com MDTG integrado.

---

## Arquitetura Geral

O engine é composto por:

- MDTGNode  
- Matrix4 (transformações 4×4)  
- Sistema básico de propriedades  
- Motor determinístico de propagação top-down  
- Executor paralelo via ThreadPool  
- Demonstração completa em main.cpp

### Pipeline

AST / Entrada → MDTG Builder → MDTG Tree → Parallel Scheduler → Analyzer → Output

---

## Estrutura do Projeto

mdtg-starter/
├── include/
│    └── mdtg/
│         ├── matrix4.hpp
│         ├── mdtg_node.hpp
│         └── thread_pool.hpp
├── src/
│    ├── main.cpp
│    ├── mdtg_node.cpp
│    └── thread_pool.cpp
├── CMakeLists.txt
└── README.md

---

## Build (Windows / Linux)

### Criar diretório de build

mkdir build
cd build

### Gerar via CMake

cmake ..

### Compilar

Windows (MSVC):

cmake --build . --config Release

Linux:

make -j$(nproc)

### Executar

./Release/mdtg.exe

---

## O que é o MDTGNode?

Cada MDTGNode representa um nó de uma estrutura multidimensional contendo:

- Transformação local
- Transformação global
- Propriedades locais e herdadas
- Geometria opcional
- Filhos hierárquicos
- Execução paralela determinística

Fluxo interno:

- Propagação de transformações
- Resolução de propriedades
- Análise local
- Execução paralela por níveis

---

## Demonstração

O main.cpp monta uma árvore MDTG completa e executa:

- computeGlobalTransform()
- resolveProperties()
- evaluateNode()

Com multithreading real e barreiras por nível.

Saída:

MDTG Finalizado

---

## Tecnologias Utilizadas

- C++20  
- CMake  
- ThreadPool customizado  
- Smart pointers seguros  
- Execução paralela determinística  

---

## Dual Licensing Model

O MDTG é distribuído sob modelo dual:

### MIT License (Non-Commercial)
Permite uso pessoal, de pesquisa e educacional.

### Commercial License
Obrigatória para:

- Empresas  
- Softwares BIM/IFC  
- Produtos proprietários  
- Servidores ou SaaS  
- Qualquer uso comercial ou lucrativo  

Benefícios da licença comercial:

- Uso em produtos fechados  
- Direitos completos de produção  
- Suporte técnico  
- Atualizações e compatibilidade  

Para adquirir licença comercial:

E-mail: fabiopetronilho1977@outlook.com  
WhatsApp: +55 11 99799-8427  

---

## Autor

Fábio Petronilho  
Desenvolvedor BIM / Engenharia da Computação  
GitHub: https://github.com/TechMaster8845/mdtg-starter
