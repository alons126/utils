# Foil_vol_calc2

A small C++ utility that estimates the remaining volume of a rectangular foil blank after subtracting **two pairs of right-triangular cutouts** (modeled as triangular prisms).

This was written as a quick geometry/volume check. All dimensions are treated as **centimeters**.

## Geometry model

The program computes:

- **V0**: volume of the full rectangular blank
  - `V0 = t · h · w`
- **V1**: volume removed by cutout type #1 (two identical right-triangular prisms)
  - each prism has triangle legs `(a1, b1)` and thickness `t`
  - `V1 = 2 · ( (1/2 · a1 · b1) · t )`
- **V2**: volume removed by cutout type #2 (two identical right-triangular prisms)
  - each prism has triangle legs `(a2, b2)` and thickness `t`
  - `V2 = 2 · ( (1/2 · a2 · b2) · t )`

Remaining volume:

- **V**: `V = V0 − V1 − V2`

Effective width:

- **w_eff**: the width of an equivalent perfect rectangle with the same `t` and `h` such that
  - `V = t · h · w_eff`  →  `w_eff = V / (t · h)`

> Note: there is a `separation` variable in the code that is currently unused (kept for possible future extensions of the model).

## Inputs

Inputs are hard-coded in `main.cpp` and selected via:

```cpp
std::string Size = "small";  // or "large"
```

Two sets of parameters are included:

- `small`
- `large`

## Output

The program prints:

- `V0`, `V1`, `V2` (cm³)
- `V` (cm³)
- `w_eff` (cm)
- `Half w_eff` (cm)

## Build and run

From this directory:

```bash
# Build
c++ -std=c++17 -O2 -Wall -Wextra -pedantic -o foil_vol main.cpp

# Run
./foil_vol
```

## Files

- `main.cpp` — implementation

## Assumptions and limitations

- Cutouts are modeled as **right-triangular** prisms (legs `a` and `b`).
- Only two cutout types are included, each duplicated twice.
- No unit conversion is performed — **all values must be in cm**.

## License

Internal utility / personal use.
