import argparse
import time
from typing import Optional

import matplotlib.pyplot as plt
import numpy as np


def load_imu_csv(path: str, gimbal_id: Optional[int]):
    t_s = []
    pitch = []
    yaw = []

    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = [p.strip() for p in line.split(",")]
            if len(parts) != 4:
                continue
            try:
                t = float(parts[0])
                gid = int(parts[1])
                p = float(parts[2])
                y = float(parts[3])
            except ValueError:
                continue

            if gimbal_id is not None and gid != gimbal_id:
                continue

            t_s.append(t)
            pitch.append(p)
            yaw.append(y)

    return np.array(t_s), np.array(pitch), np.array(yaw)


def main():
    parser = argparse.ArgumentParser(description="Plot IMU pitch/yaw from log/imu.txt")
    parser.add_argument("--file", default="../log/imu.txt", help="path to imu log file")
    parser.add_argument("--id", type=int, default=None, help="filter by gimbal id (optional)")
    parser.add_argument("--deg", action="store_true", help="plot degrees (default: radians)")
    args = parser.parse_args()

    try:
        t_s, pitch, yaw = load_imu_csv(args.file, args.id)
    except FileNotFoundError:
        print(f"错误：未找到日志文件 {args.file}")
        return 1

    if t_s.size == 0:
        if args.id is None:
            print(f"未在日志文件 {args.file} 中检测到任何有效数据。")
        else:
            print(f"未在日志文件 {args.file} 中检测到任何有效数据 (id={args.id})。")
        return 1

    if args.deg:
        pitch = pitch * 180.0 / np.pi
        yaw = yaw * 180.0 / np.pi
        unit = "deg"
    else:
        unit = "rad"

    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(1, 1, 1)

    ax.plot(t_s, pitch, label=f"pitch ({unit})", color="tab:blue", alpha=0.9)
    ax.plot(t_s, yaw, label=f"yaw ({unit})", color="tab:orange", alpha=0.9)

    title = f"IMU Pitch/Yaw ({args.file})"
    if args.id is not None:
        title += f" [id={args.id}]"
    ax.set_title(title)
    ax.set_xlabel("t (s)")
    ax.set_ylabel(f"angle ({unit})")
    ax.legend(loc="upper right")
    ax.grid(True)

    timestamp = time.strftime("%Y%m%d_%H%M%S")
    filename = f"imu_plot_{timestamp}.png"
    fig.savefig(filename, bbox_inches="tight", dpi=300)
    print(f"\n✅ 图表已成功保存为本地文件: {filename}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
