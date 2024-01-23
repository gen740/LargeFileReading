import numpy as np


N = 200_000_000
def create_large_file():
    # Large file creation
    np.random.seed(0)
    large_array = np.random.randint(0, N, N, dtype=np.uint32)
    large_array.tofile("large_array.raw")


def load_large_file():
    large_array = np.fromfile("large_array.raw", dtype=np.uint32)

    for i in range(10):
        print(large_array[i])

    for i in range(N-10, N):
        print(large_array[i])


if __name__ == "__main__":
    # create_large_file()
    load_large_file()
