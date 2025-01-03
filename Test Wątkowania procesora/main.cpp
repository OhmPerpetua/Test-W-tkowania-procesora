#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

// Funkcja do obliczenia wartoœci funkcji pod ca³k¹
double f(double x) {
    return 4.0 / (1.0 + x * x);
}

// Funkcja do obliczenia ca³ki numerycznie
void integrate(double a, double b, int num_steps, int thread_id, int num_threads, double& result) {
    double step = (b - a) / num_steps;
    double sum = 0.0;

    for (int i = thread_id; i < num_steps; i += num_threads) {
        double x = a + i * step;
        sum += f(x);
    }

    result = sum * step;
}

int main() {
    double a = 0.0;
    double b = 1.0;
    int num_steps;
    int num_threads;

    std::cout << "Podaj ilosc liczb podzialu (np. 100000000, 1000000000, 3000000000): ";
    std::cin >> num_steps;

    std::cout << "Podaj ilosc watkow (od 1 do 50): ";
    std::cin >> num_threads;

    std::vector<std::thread> threads;
    std::vector<double> results(num_threads, 0.0);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(integrate, a, b, num_steps, i, num_threads, std::ref(results[i]));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double pi = 0.0;
    for (const auto& result : results) {
        pi += result;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "Przyblizona wartosc Pi: " << pi << std::endl;
    std::cout << "Czas obliczen: " << duration.count() << " sekund" << std::endl;

    return 0;
}