import matplotlib.pyplot as plt


def plot_results(data_path, output_path):
    # 讀取檔案

    with open(data_path, "r") as file:
        # 讀取第一行
        first_line = file.readline().split()
        data_location = first_line[0]
        iteration = int(first_line[1])

        # 讀取iteration行的makespan
        makespans = [int(next(file).strip()) for _ in range(iteration)]

    # 產生iteration的列表
    iterations = list(range(1, iteration + 1))

    # 繪製折線圖
    plt.rcParams.update({"font.size": 30})

    plt.figure(figsize=(16, 10))
    plt.plot(iterations, makespans, marker="o", linestyle="-")
    plt.xlabel("Iteration")
    plt.ylabel("Makespan")
    plt.title(f"Results for {data_location}")

    plt.savefig(output_path)
    plt.show()


if __name__ == "__main__":
    data_path = "ii_log/log_tai100_20_1.txt"
    fig_path = "ii_log/log_tai100_20_1.png"
    plot_results(data_path, fig_path)
