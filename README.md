# Spreadsheet(CZ)

## Obsah
- [Popis](#popis)
- [Funkcionalita](#funkcionalita)
- [Použití](#použití)
- [Instalace](#instalace)
- [Třídy](#třídy)
- [Operace](#operace)
- [Podporované výrazy](#podporované-výrazy)
- [Testování](#testování)

---

## Popis
Tento projekt implementuje tabulkový procesor umožňující operace s buňkami, výpočty hodnot na základě vzorců, detekci cyklických závislostí a ukládání/načítání tabulek.

## Funkcionalita
- Správa obsahu buněk (nastavení hodnot, kopírování, načítání, ukládání)
- Výpočet hodnot buněk podle vzorců
- Detekce cyklických závislostí mezi buňkami
- Možnost ukládání a načítání tabulek

## Použití
Program podporuje operace s buňkami zadané uživatelem, včetně nastavení hodnot, kopírování buněk a načítání dat ze souborů.

## Instalace
1. Překlad projektu pomocí CMake:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
2. Spuštění programu:
   ```sh
   ./BIG
   ```

## Třídy
### CSpreadsheet
- Hlavní třída tabulkového procesoru.
- Implementuje operace s buňkami.

### CPos
- Identifikátor buňky v tabulce (např. A7, B15).
- Umožňuje konverzi mezi různými formáty identifikátorů.

### CExpressionBuilder
- Používá se pro vyhodnocování výrazů ve vzorcích buněk.
- Rozšiřuje rozhraní pro práci se syntaktickým analyzátorem.

### CValue
- Uchovává hodnotu buňky (číslo, řetězec, nedefinovaná hodnota).

## Operace
- `setCell(pos, value)`: Nastaví hodnotu buňky na konkrétní hodnotu nebo vzorec.
- `getValue(pos)`: Vrátí vypočítanou hodnotu buňky.
- `copyRect(dstCell, srcCell, w, h)`: Zkopíruje blok buněk.
- `save(os)`: Uloží tabulku do souboru.
- `load(is)`: Načte tabulku ze souboru.

## Podporované výrazy
Tabulkový procesor podporuje výpočty a operace podobné standardním tabulkovým aplikacím:
- Aritmetické operace: `+`, `-`, `*`, `/`, `^`
- Logické operace: `=`, `<>`, `<`, `<=`, `>`, `>=`
- Vestavěné funkce:
  - `sum(range)`: součet hodnot v oblasti
  - `count(range)`: počet neprázdných buněk
  - `min(range)`, `max(range)`: minimální a maximální hodnota v oblasti
  - `if(cond, true, false)`: podmíněná hodnota

## Testování
Program obsahuje několik testů pro ověření správnosti:
- Testy bez cyklických závislostí
- Testy kopírování a ukládání dat
- Testy výkonu a rychlosti výpočtů
- Testy detekce cyklických závislostí


# Spreadsheet (EN)

## Contents

- [Description](#description)
- [Functionality](#functionality)
- [Usage](#usage)
- [Installation](#installation)
- [Classes](#classes)
- [Operations](#operations)
- [Supported Expressions](#supported-expressions)
- [Testing](#testing)

---

## Description

This project implements a spreadsheet processor capable of handling cell operations, computing values based on formulas, detecting cyclic dependencies, and saving/loading tables.

## Functionality

- Managing cell contents (setting values, copying, saving, loading)
- Computing cell values based on formulas
- Detecting cyclic dependencies between cells
- Saving and loading tables

## Usage

The program supports user-defined operations on cells, including setting values, copying cells, and loading data from files.

## Installation

1. Compile the project using CMake:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
2. Run the program:
   ```sh
   ./BIG
   ```

## Classes

### CSpreadsheet

- The main class implementing the spreadsheet processor.
- Manages operations on cells.

### CPos

- Identifies a cell in the spreadsheet (e.g., A7, B15).
- Converts between different cell identifier formats.

### CExpressionBuilder

- Used for evaluating expressions in cell formulas.
- Extends the interface for working with the syntax analyzer.

### CValue

- Stores the value of a cell (number, string, or undefined value).

## Operations

- `setCell(pos, value)`: Sets a cell's value to a number, string, or formula.
- `getValue(pos)`: Retrieves the computed value of a cell.
- `copyRect(dstCell, srcCell, w, h)`: Copies a rectangular block of cells.
- `save(os)`: Saves the spreadsheet to a file.
- `load(is)`: Loads the spreadsheet from a file.

## Supported Expressions

The spreadsheet processor supports calculations and operations similar to standard spreadsheet applications:

- Arithmetic operations: `+`, `-`, `*`, `/`, `^`
- Logical operations: `=`, `<>`, `<`, `<=`, `>`, `>=`
- Built-in functions:
  - `sum(range)`: sum of values in a range
  - `count(range)`: count of non-empty cells
  - `min(range)`, `max(range)`: minimum and maximum values in a range
  - `if(cond, true, false)`: conditional value selection

## Testing

The program includes multiple tests to verify correctness:

- Non-cyclic dependency tests
- Copying and saving data tests
- Performance and evaluation speed tests
- Cyclic dependency detection tests

