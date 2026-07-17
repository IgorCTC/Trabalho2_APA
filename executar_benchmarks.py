#!/usr/bin/env python3

from __future__ import annotations

import csv
import subprocess
from pathlib import Path


ROOT_DIR = Path(__file__).resolve().parent
INSTANCES_DIR = ROOT_DIR / "instances"
CSV_OUTPUT = ROOT_DIR / "resultados_benchmarks.csv"
BUILD_DIR = ROOT_DIR / ".benchmark_bin"
OVERWRITE_BUILD = True

BUILD_SPECS = [
    {
        "name": "runApagador",
        "source": ROOT_DIR / "Apagador" / "src" / "runApagador.cpp",
        "output": BUILD_DIR / "runApagador",
        "includes": [
            ROOT_DIR / "verifica_conexo" / "Grafo" / "include",
            ROOT_DIR / "verifica_conexo" / "Lista" / "include",
        ],
    },
    {
        "name": "runBuscaCircuito",
        "source": ROOT_DIR / "busca_circuito" / "runBuscaCircuito.cpp",
        "output": BUILD_DIR / "runBuscaCircuito",
        "includes": [ROOT_DIR / "busca_circuito"],
    },
    {
        "name": "verifica_circuito_IA",
        "source": ROOT_DIR / "verifica_circuito_IA" / "verifica_circuito_IA.cpp",
        "output": BUILD_DIR / "verifica_circuito_IA",
        "includes": [],
    },
    {
        "name": "runVerificaConexo",
        "source": ROOT_DIR / "verifica_conexo" / "Algoritmos" / "conexo" / "src" / "runVerificaConexo.cpp",
        "output": BUILD_DIR / "runVerificaConexo",
        "includes": [
            ROOT_DIR / "verifica_conexo" / "Grafo" / "include",
            ROOT_DIR / "verifica_conexo" / "Lista" / "include",
        ],
    },
    {
        "name": "verifica_conexo_IA",
        "source": ROOT_DIR / "verifica_conexo_IA" / "verifica_conexo_IA.cpp",
        "output": BUILD_DIR / "verifica_conexo_IA",
        "includes": [],
    },
]


def run_benchmark(executable: Path, instance_path: Path) -> tuple[str, str, int, int]:
    if not executable.exists():
        raise FileNotFoundError(f"Executavel nao encontrado: {executable}")

    completed = subprocess.run(
        [str(executable), str(instance_path)],
        check=True,
        capture_output=True,
        text=True,
    )

    stdout = completed.stdout.strip().splitlines()
    if not stdout:
        raise RuntimeError(f"{executable.name} nao produziu saida para {instance_path.name}")

    last_line = stdout[-1].strip()
    parts = last_line.split(",", 3)
    if len(parts) != 4:
        raise RuntimeError(f"Saida inesperada de {executable.name}: {last_line}")

    algorithm, instance_label, elapsed_ns_text, result_text = parts
    return algorithm, instance_label, int(elapsed_ns_text), int(result_text)


def build_executable(spec: dict[str, object]) -> Path:
    source = spec["source"]
    output = spec["output"]
    includes = spec["includes"]

    assert isinstance(source, Path)
    assert isinstance(output, Path)
    assert isinstance(includes, list)

    BUILD_DIR.mkdir(parents=True, exist_ok=True)

    needs_build = not output.exists() or source.stat().st_mtime > output.stat().st_mtime
    if needs_build or OVERWRITE_BUILD:
        command = ["g++", "-std=c++20", str(source), "-o", str(output)]
        for include_dir in includes:
            command.extend(["-I", str(include_dir)])

        subprocess.run(command, check=True)

    return output


def main() -> None:
    if not INSTANCES_DIR.exists():
        raise FileNotFoundError(f"Diretorio de instancias nao encontrado: {INSTANCES_DIR}")

    instances = sorted(INSTANCES_DIR.glob("*.txt"))
    if not instances:
        raise FileNotFoundError(f"Nenhum arquivo .txt encontrado em {INSTANCES_DIR}")

    executables: dict[str, Path] = {}
    for spec in BUILD_SPECS:
        executables[spec["name"]] = build_executable(spec)

    algorithm_order = [spec["name"] for spec in BUILD_SPECS]
    rows: list[dict[str, object]] = []

    for instance_path in instances:
        row: dict[str, object] = {"instance_file": instance_path.name}
        for executable in executables.values():
            algorithm, instance_label, elapsed_ns, result = run_benchmark(executable, instance_path)
            row[f"{algorithm}_elapsed_ns"] = elapsed_ns
            row[f"{algorithm}_result"] = result
            print(f"[ok] {algorithm} -> {instance_path.name} ({elapsed_ns} ns)")
        rows.append(row)

    with CSV_OUTPUT.open("w", newline="", encoding="utf-8") as csv_file:
        fieldnames = ["instance_file"]
        for algorithm_name in algorithm_order:
            fieldnames.append(f"{algorithm_name}_elapsed_ns")
            fieldnames.append(f"{algorithm_name}_result")

        writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
        writer.writeheader()
        writer.writerows(rows)

    print(f"CSV salvo em {CSV_OUTPUT}")


if __name__ == "__main__":
    main()