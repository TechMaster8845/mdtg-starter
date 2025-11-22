# MDTG Starter Project  
MultiDimensional Tree Graph Engine (C++20)

O **MDTG (MultiDimensional Tree Graph)** é um engine moderno em C++20 projetado para servir como  
estrutura de dados central de pipelines complexos, como:

- Compiladores IFC/BIM  
- Motores de análise geométrica  
- Motores de simulação  
- Processamento massivo de grafos hierárquicos  
- Sistemas paralelos baseados em árvores multidimensionais  

Este repositório contém a **base mínima**, limpa e reutilizável, para iniciar qualquer projeto  
com MDTG integrado.

---

## 🌐 Arquitetura Geral

O engine é composto por:

- `MDTGNode`  
- `Matrix4` (transformações 4×4)  
- Sistema básico de propriedades  
- Motor de propagação determinística top-down  
- Executor paralelo via `ThreadPool`  
- Demonstração completa no `main.cpp`

### Pipeline

```

AST / Entrada → MDTG Builder → MDTG Tree → Parallel Scheduler → Analyzer → Output

```

---

## 📦 Estrutura do Projeto

```

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

````

---

## ⚙️ Build (Windows / Linux)

### 1) Criar diretório de build

```bash
mkdir build
cd build
````

### 2) Gerar o projeto via CMake

```bash
cmake ..
```

### 3) Compilar

#### Windows (MSVC)

```bash
cmake --build . --config Release
```

#### Linux / MinGW

```bash
make -j$(nproc)
```

### 4) Executar

```bash
./Release/mdtg.exe
```

---

## 🧩 O que é o MDTGNode?

Cada `MDTGNode` representa um nó de uma estrutura hierárquica multidimensional:

* Transformação local
* Transformação global
* Propriedades locais
* Propriedades resolvidas (herdadas + sobrescritas)
* Geometria opcional
* Filhos
* Execução paralela determinística

### Exemplo de fluxo interno

* Propagação de transformações
* Resolução de propriedades
* Análise local (CPU-bound)
* Execução paralela por níveis

---

## 🚀 Demonstração inclusa

O `main.cpp` monta uma árvore MDTG exemplar, percorre níveis da árvore e executa:

* `computeGlobalTransform()`
* `resolveProperties()`
* `evaluateNode()`

Tudo isso usando **multithreading real**, controlado por barreiras por nível.

Saída esperada:

```
MDTG Finalizado
```

---

## 🔧 Tecnologias Utilizadas

* **C++20**
* **CMake**
* **ThreadPool customizado**
* Containers seguros (`std::unique_ptr`, mutexes, atomic)
* Execução paralela determinística por níveis

---

## 📘 Próximos Passos (sugestões)

* Integração com IFC Parser → gerar MDTG automaticamente
* Scheduler baseado em DAG (dependências)
* Otimizações com Eigen para matrizes
* Pool de memória customizado
* Verificador BIM / Verificador geométrico
* Analisadores plugináveis

---

## 📄 Licença

MIT License (opcional — adicionar se desejar).
Este projeto é livre para uso acadêmico, comercial e profissional.

---

## 👤 Autor

**Fábio Petronilho**
Desenvolvedor BIM / Engenharia da Computação
GitHub: [https://github.com/TechMaster8845/mdtg-starter](https://github.com/TechMaster8845/mdtg-starter)



## Licensing

The MDTG Engine is distributed under a dual license model:

### MIT License (Non-Commercial)
Permits personal, educational, academic, and non-commercial usage.

### Commercial License
Required for any commercial, corporate, industrial, or revenue-generating usage.

The commercial license permits:
- Integration into proprietary or closed-source products
- Use inside engineering workflows
- Use as part of commercial BIM tools and analysis engines
- Use inside SaaS platforms
- Internal use within companies

To obtain a commercial license, contact:
fabiopetronilho1977@outlook.com
