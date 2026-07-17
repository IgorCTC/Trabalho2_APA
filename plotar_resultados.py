#!/usr/bin/env python3

from __future__ import annotations

import csv
import re
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.lines import Line2D


ROOT_DIR = Path(__file__).resolve().parent
CSV_PATH = ROOT_DIR / "resultados_benchmarks.csv"
PLOTS_DIR = ROOT_DIR / "plots"
ANNOTATE_EDGE_PROPORTION = False


CIRCUIT_ALGORITHMS = [
    ("runApagador", "Apagador"),
    ("runBuscaCircuito", "Busca"),
    ("verifica_circuito_IA", "IA Circuito"),
]

CONNEXITY_ALGORITHMS = [
    ("runVerificaConexo", "Verifica Conexo"),
    ("verifica_conexo_IA", "IA Conexo"),
]

GROUP_COLORS = ["#2E86AB", "#F18F01", "#C73E1D", "#6A4C93", "#2E8B57"]
ANNOTATION_OFFSETS = [
    (6, 8),
    (6, -10),
    (-18, 8),
    (-18, -10),
]

plt.rcParams.update(
    {
        "font.size": 13,
        "axes.titlesize": 19,
        "axes.labelsize": 15,
        "xtick.labelsize": 13,
        "ytick.labelsize": 13,
        "legend.fontsize": 12,
    }
)


def load_rows(csv_path: Path) -> list[dict[str, str]]:
    if not csv_path.exists():
        raise FileNotFoundError(f"CSV nao encontrado: {csv_path}")

    with csv_path.open(newline="", encoding="utf-8") as csv_file:
        return list(csv.DictReader(csv_file))


def parse_vertex_count(instance_name: str) -> int:
    match = re.search(r"v(\d+)a(\d+)\.txt$", instance_name)
    if not match:
        raise ValueError(f"Nome de instancia invalido: {instance_name}")
    return int(match.group(1))


def parse_edge_ratio(instance_name: str) -> int:
    match = re.search(r"v(\d+)a(\d+)\.txt$", instance_name)
    if not match:
        raise ValueError(f"Nome de instancia invalido: {instance_name}")
    return int(match.group(2))


def build_series(
    rows: list[dict[str, str]],
    algorithms: list[tuple[str, str]],
) -> tuple[list[int], dict[int, dict[str, list[tuple[int, float]]]]]:
    grouped: dict[int, dict[str, list[tuple[int, float]]]] = defaultdict(lambda: defaultdict(list))

    for row in rows:
        instance_file = row["instance_file"]
        vertex_count = parse_vertex_count(instance_file)
        edge_ratio = parse_edge_ratio(instance_file)

        for algorithm_key, _label in algorithms:
            elapsed_key = f"{algorithm_key}_elapsed_ns"
            if elapsed_key not in row or not row[elapsed_key]:
                continue
            grouped[vertex_count][algorithm_key].append((edge_ratio, int(row[elapsed_key])))

    vertex_counts = sorted(grouped.keys())
    return vertex_counts, grouped


def plot_points(
    rows: list[dict[str, str]],
    algorithms: list[tuple[str, str]],
    title: str,
    output_name: str,
    *,
    log_scale: bool = False,
) -> None:
    vertex_counts, grouped = build_series(rows, algorithms)
    if not vertex_counts:
        raise ValueError("Nao ha dados suficientes para gerar o grafico.")

    PLOTS_DIR.mkdir(parents=True, exist_ok=True)

    num_groups = len(vertex_counts)
    num_algorithms = len(algorithms)
    spacing = num_algorithms + 1

    fig, ax = plt.subplots(figsize=(max(12, num_groups * 3.2), 7))

    box_data = []
    positions = []
    tick_positions = []
    tick_labels = []
    legend_handles = []

    for group_index, vertex_count in enumerate(vertex_counts):
        base_position = group_index * spacing + 1
        tick_positions.append(base_position + (num_algorithms - 1) / 2)
        tick_labels.append(f"{vertex_count}")

        for algo_index, (algorithm_key, algorithm_label) in enumerate(algorithms):
            points = grouped[vertex_count].get(algorithm_key, [])
            if not points:
                continue

            position = base_position + algo_index
            color = GROUP_COLORS[algo_index % len(GROUP_COLORS)]
            legend_handles.append(Line2D([0], [0], marker="o", linestyle="", color="none", markerfacecolor=color, markeredgecolor="#333333", markersize=8, label=algorithm_label))

            for point_index, (edge_ratio, elapsed_ns) in enumerate(points):
                jitter = (point_index - (len(points) - 1) / 2) * 0.06
                x_value = position + jitter
                ax.scatter(
                    x_value,
                    elapsed_ns,
                    s=36,
                    color=color,
                    edgecolors="#333333",
                    linewidths=0.5,
                    alpha=0.85,
                    zorder=3,
                )
                if ANNOTATE_EDGE_PROPORTION:
                    offset_x, offset_y = ANNOTATION_OFFSETS[point_index % len(ANNOTATION_OFFSETS)]
                    ax.annotate(
                        f"{edge_ratio}%",
                        (x_value, elapsed_ns),
                        textcoords="offset points",
                        xytext=(offset_x, offset_y),
                        fontsize=10,
                        alpha=0.8,
                    )

    ax.set_title(title, pad=18)
    ax.set_xlabel("Quantidade de vertices")
    ax.set_ylabel("Tempo de execucao (ns)")
    ax.set_xticks(tick_positions)
    ax.set_xticklabels(tick_labels)
    if log_scale:
        ax.set_yscale("log")
    ax.grid(axis="y", linestyle="--", alpha=0.35)

    handles_by_label = {}
    for handle in legend_handles:
        handles_by_label[handle.get_label()] = handle
    ax.legend(handles_by_label.values(), handles_by_label.keys(), loc="upper left", frameon=True)

    fig.tight_layout()
    output_path = PLOTS_DIR / output_name
    fig.savefig(output_path, dpi=200, bbox_inches="tight")
    plt.close(fig)


def main() -> None:
    rows = load_rows(CSV_PATH)
    plot_points(
        rows,
        CIRCUIT_ALGORITHMS,
        "Tempo de execucao - algoritmos de verificacao de circuito",
        "circuito_pontos.png",
        log_scale=True,
    )
    plot_points(
        rows,
        CONNEXITY_ALGORITHMS,
        "Tempo de execucao - algoritmos de verificacao de conexidade",
        "conexo_pontos.png",
    )
    print(f"Graficos salvos em {PLOTS_DIR}")


if __name__ == "__main__":
    main()