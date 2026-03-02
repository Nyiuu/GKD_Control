import argparse
import time
from typing import Optional

import matplotlib.pyplot as plt
import numpy as np


def load_imu_csv(path: str, gimbal_id: Optional[int]):
    t_s = []
    pitch = []
    yaw = []
    pitch_rate = []
    yaw_rate = []
    has_rate = False

    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = [p.strip() for p in line.split(",")]
            if len(parts) not in (4, 6):
                continue
            try:
                t = float(parts[0])
                gid = int(parts[1])
                p = float(parts[2])
                y = float(parts[3])
                if len(parts) == 6:
                    pr = float(parts[4])
                    yr = float(parts[5])
            except ValueError:
                continue

            if gimbal_id is not None and gid != gimbal_id:
                continue

            t_s.append(t)
            pitch.append(p)
            yaw.append(y)
            if len(parts) == 6:
                if not has_rate and len(t_s) > 1:
                    # Backfill previous samples when rate data appears later.
                    pitch_rate.extend([np.nan] * (len(t_s) - 1))
                    yaw_rate.extend([np.nan] * (len(t_s) - 1))
                pitch_rate.append(pr)
                yaw_rate.append(yr)
                has_rate = True
            elif has_rate:
                # Keep arrays aligned if some lines have no rate data.
                pitch_rate.append(np.nan)
                yaw_rate.append(np.nan)

    if has_rate:
        return (
            np.array(t_s),
            np.array(pitch),
            np.array(yaw),
            np.array(pitch_rate),
            np.array(yaw_rate),
            True,
        )
    return np.array(t_s), np.array(pitch), np.array(yaw), None, None, False


def main():
    parser = argparse.ArgumentParser(description="Plot IMU pitch/yaw (+ rate) from log/imu.txt")
    parser.add_argument("--file", default="../log/imu.txt", help="path to imu log file")
    parser.add_argument("--id", type=int, default=None, help="filter by gimbal id (optional)")
    parser.add_argument("--deg", action="store_true", help="plot degrees (default: radians)")
    args = parser.parse_args()

    try:
        t_s, pitch, yaw, pitch_rate, yaw_rate, has_rate = load_imu_csv(args.file, args.id)
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
        if has_rate:
            pitch_rate = pitch_rate * 180.0 / np.pi
            yaw_rate = yaw_rate * 180.0 / np.pi
        unit = "deg"
        rate_unit = "deg/s"
    else:
        unit = "rad"
        rate_unit = "rad/s"

    if has_rate:
        fig = plt.figure(figsize=(10, 7))
        ax_angle = plt.subplot2grid((2, 1), (0, 0))
        ax_rate = plt.subplot2grid((2, 1), (1, 0))
        plt.subplots_adjust(hspace=0.3)
    else:
        fig = plt.figure(figsize=(10, 5))
        ax_angle = fig.add_subplot(1, 1, 1)
        ax_rate = None

    ax_angle.plot(t_s, pitch, label=f"pitch ({unit})", color="tab:blue", alpha=0.9)
    ax_angle.plot(t_s, yaw, label=f"yaw ({unit})", color="tab:orange", alpha=0.9)

    title = f"IMU Pitch/Yaw ({args.file})"
    if args.id is not None:
        title += f" [id={args.id}]"
    ax_angle.set_title(title)
    ax_angle.set_xlabel("t (s)")
    ax_angle.set_ylabel(f"angle ({unit})")
    ax_angle.legend(loc="upper right")
    ax_angle.grid(True)
    #ax_angle.set_ylim (top=3, bottom=1)

    if has_rate and ax_rate is not None:
        ax_rate.plot(t_s, pitch_rate, label=f"pitch_rate ({rate_unit})", color="tab:green", alpha=0.9)
        ax_rate.plot(t_s, yaw_rate, label=f"yaw_rate ({rate_unit})", color="tab:red", alpha=0.9)
        ax_rate.set_title("IMU Pitch/Yaw Rate")
        ax_rate.set_xlabel("t (s)")
        ax_rate.set_ylabel(f"rate ({rate_unit})")
        ax_rate.legend(loc="upper right")
        ax_rate.grid(True)

    timestamp = time.strftime("%Y%m%d_%H%M%S")
    filename = f"imu_plot_{timestamp}.png"
    fig.savefig(filename, bbox_inches="tight", dpi=300)
    print(f"\n✅ 图表已成功保存为本地文件: {filename}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
