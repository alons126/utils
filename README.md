# utils

A personal collection of small, focused utilities used for **geometry checks, numerical sanity tests, and quick calculations** in analysis and simulation workflows.

This repository is intentionally lightweight: each subdirectory is typically a **self-contained tool** with minimal dependencies, written to answer a very specific technical question.

## Repository structure

Each utility lives in its own folder. Typical examples include:

- Geometry or volume calculators
- Small C++ or Python test programs
- One-off numerical checks used during detector or analysis development

Example:

```text
utils/
├── Foil_vol_calc2/
│   ├── main.cpp
│   └── README.md
├── <other utility>/
│   ├── ...
│   └── README.md
└── README.md
```

Each subdirectory **should contain its own README** describing:
- What the tool does
- Assumptions and units
- How to build/run it

## Philosophy

- Minimal code
- Explicit assumptions
- No hidden magic
- Easy to recompile or modify on any machine

Most tools are written quickly for validation purposes and are **not meant to be general-purpose libraries**.

## Languages

Depending on the utility, you may find:

- **C++** (simple executables, often compiled with `c++ -std=c++17`)
- **Python** (small scripts or numerical checks)
- Occasional shell snippets

## How to use

1. `cd` into the desired utility directory
2. Read the local `README.md`
3. Build or run using the instructions provided there

There is no global build system by design.

## Intended use

This repository is mainly for:

- Quick cross-checks during detector geometry work
- Sanity checks during analysis development
- Reproducible numerical calculations referenced in notes or documentation

## License

Personal research utilities.
Not intended for redistribution as a standalone package.
