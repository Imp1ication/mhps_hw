import os


def calculate_statistics(directory, prefix):
    # 檢查目錄是否存在
    if not os.path.exists(directory):
        print("指定的目錄不存在。")
        return

    # 取得指定目錄下以指定前綴開頭的檔案
    files = [file for file in os.listdir(directory) if file.startswith(prefix)]

    # 遍歷每個檔案，讀取資料並計算統計值
    output_file = os.path.join(directory, "statistics.txt")

    with open(output_file, "w") as out_f:
        for file in files:
            file_path = os.path.join(directory, file)
            with open(file_path, "r") as in_f:
                data = [int(line.strip()) for line in in_f]

            # 計算統計值
            min_value = min(data)
            average_value = sum(data) / len(data)
            max_value = max(data)

            # 取得檔案名稱
            filename = file[len(prefix) :]
            filename = os.path.splitext(filename)[0]

            # 印出與寫入統計值
            print(f"{filename} \t{min_value}/{average_value:.2f}/{max_value}")
            out_f.write(f"{filename} \t{min_value}/{average_value:.2f}/{max_value}\n")


# 呼叫函式列出檔案
if __name__ == "__main__":
    directories = {"ii_log", "sa_log", "ts_log", "ts2_log"}

    for directory in directories:
        print(f"--- {directory} ---")
        calculate_statistics(directory, "best_")
