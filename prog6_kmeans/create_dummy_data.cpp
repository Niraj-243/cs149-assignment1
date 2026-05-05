#include <fstream>
#include <random>
#include <iostream>

int main() {
    const int M = 2'000'000;  // number of points (start small)
    const int N = 3;          // dimension
    const int K = 5;          // clusters
    const double epsilon = 1e-4;

    std::ofstream out("data.dat", std::ios::binary);
    if (!out) {
        std::cerr << "Error opening file\n";
        return 1;
    }

    // 🔴 WRITE HEADER
    out.write(reinterpret_cast<const char*>(&M), sizeof(int));
    out.write(reinterpret_cast<const char*>(&N), sizeof(int));
    out.write(reinterpret_cast<const char*>(&K), sizeof(int));
    out.write(reinterpret_cast<const char*>(&epsilon), sizeof(double));

    std::mt19937 rng(42);
    std::normal_distribution<double> noise(0.0, 1.0);
    std::uniform_real_distribution<double> center_dist(-10.0, 10.0);

    // 🔴 Generate cluster centers
    double centers[16][16];
    for (int k = 0; k < K; k++)
        for (int d = 0; d < N; d++)
            centers[k][d] = center_dist(rng);

    // 🔴 Write data points
    for (int i = 0; i < M; i++) {
        int c = i % K;  // assign cluster
        for (int d = 0; d < N; d++) {
            double val = centers[c][d] + noise(rng);
            out.write(reinterpret_cast<char*>(&val), sizeof(double));
        }
    }

    // 🔴 Write initial centroids
    for (int k = 0; k < K; k++) {
        for (int d = 0; d < N; d++) {
            double val = centers[k][d];
            out.write(reinterpret_cast<char*>(&val), sizeof(double));
        }
    }

    out.close();

    std::cout << "Generated dataset successfully.\n";
}
