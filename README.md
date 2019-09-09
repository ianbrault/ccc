# ccc

A **C**ommand-line **C** **C**alculator inspired by `bc`

## Installation

To clone and build the repository:

```bash
$ git clone https://github.com/ianbrault/ccc
$ cd ccc && make
$ make test  # optional
```

## Usage

`ccc` currently supports basic integer arithmetic (addition, subtraction, and
multiplication). Support for floating point numbers (and division) is coming soon.

`ccc` can be run as a simple command-line script or can be used as an interactive REPL

```bash
$ ccc "16 * (36 + 64)"
1600

$ ccc

>>> 4 * (32 - 16)
64
```

## Changelog

**0.1.0:** initial release