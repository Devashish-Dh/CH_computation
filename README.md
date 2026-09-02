# CH computation — parallel CPU and CUDA

Small learning project for **divide-and-conquer / convex-hull style computation**, CPU multithreading, and CUDA. Visualization helpers live under `Viz/` (Python + matplotlib).

## Layout

| Path | Role |
| --- | --- |
| [`DnC_Approach/`](DnC_Approach/) | Divide-and-conquer approach (see its README) |
| [`Viz/`](Viz/) | Plotting / inspection |

## Python (visualizations)

Use any venv; do not hard-code another machine’s path.

```bash
python -m venv .venv
# Windows: .venv\Scripts\activate
# Unix:    source .venv/bin/activate
pip install numpy matplotlib PyQt5
```

Requires Python 3.8+ if you use the Qt5Agg matplotlib backend.
