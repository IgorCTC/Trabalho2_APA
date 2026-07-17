#!/usr/bin/env python3

from __future__ import annotations

import random
from pathlib import Path


# Ajuste estes valores diretamente no codigo.
OUTPUT_DIR = Path("instances")
VERTEX_COUNTS = [10, 20, 40, 80]
INSTANCES_PER_COUNT = 5
EDGE_COUNT_START_PERCENT = 80
EDGE_COUNT_STEP_PERCENT = 20
SEED = 42
OVERWRITE = True


def build_vertices(count: int) -> list[str]:
    return [f"x{i}" for i in range(1, count + 1)]


def build_edges(vertices: list[str], edge_count: int, rng: random.Random) -> list[tuple[str, str]]:
    if edge_count < 0:
        raise ValueError("edge_count must be non-negative")

    possible_edges = [(origin, target) for origin in vertices for target in vertices if origin != target]
    max_edges = len(possible_edges)
    if edge_count > max_edges:
        raise ValueError(f"Nao e possivel gerar {edge_count} arestas com {len(vertices)} vertices. Maximo: {max_edges}.")

    if edge_count == 0:
        return []

    return rng.sample(possible_edges, edge_count)


def render_instance(vertices: list[str], edges: list[tuple[str, str]]) -> str:
    vertices_line = ",".join(vertices)
    edges_line = ",".join(f"({origin},{target})" for origin, target in edges)
    return f"X = {{{vertices_line}}}\nU = {{{edges_line}}}\n"


def main() -> None:
    if not VERTEX_COUNTS:
        raise ValueError("VERTEX_COUNTS nao pode ser vazio")
    if any(count < 1 for count in VERTEX_COUNTS):
        raise ValueError("Todos os valores de VERTEX_COUNTS devem ser maiores ou iguais a 1")
    if INSTANCES_PER_COUNT < 1:
        raise ValueError("INSTANCES_PER_COUNT deve ser maior ou igual a 1")
    if EDGE_COUNT_START_PERCENT < 0:
        raise ValueError("EDGE_COUNT_START_PERCENT deve ser maior ou igual a 0")
    if EDGE_COUNT_STEP_PERCENT < 0:
        raise ValueError("EDGE_COUNT_STEP_PERCENT deve ser maior ou igual a 0")

    OUTPUT_DIR.mkdir(parents=True, exist_ok=True)

    rng = random.Random(SEED)
    created_files: list[Path] = []

    for vertex_count in VERTEX_COUNTS:
        vertices = build_vertices(vertex_count)
        max_edges = vertex_count * (vertex_count - 1)

        start_edge_count = round(vertex_count * (EDGE_COUNT_START_PERCENT / 100))
        edge_step = max(1, round(vertex_count * (EDGE_COUNT_STEP_PERCENT / 100))) if INSTANCES_PER_COUNT > 1 else 0
        edge_counts = [start_edge_count + (index * edge_step) for index in range(INSTANCES_PER_COUNT)]

        for edge_count in edge_counts:
            if edge_count > max_edges:
                print(f"[skip] v{vertex_count}a{edge_count}: excede o maximo possivel de arestas ({max_edges}).")
                continue

            try:
                edges = build_edges(vertices, edge_count, rng)
            except ValueError as exc:
                print(f"[skip] v{vertex_count}a{edge_count}: {exc}")
                continue

            file_path = OUTPUT_DIR / f"v{vertex_count}a{round((edge_count/vertex_count)*100)}.txt"
            if file_path.exists() and not OVERWRITE:
                print(f"[skip] {file_path.name}: arquivo ja existe. Use --overwrite para substituir.")
                continue

            file_path.write_text(render_instance(vertices, edges), encoding="utf-8")
            created_files.append(file_path)
            print(f"[ok] {file_path.name}")

    if not created_files:
        print("Nenhum arquivo foi criado.")


if __name__ == "__main__":
    main()