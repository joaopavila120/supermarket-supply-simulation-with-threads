# supermarket-supply-simulation-with-threads
A concurrent simulation of truck deliveries and shelf stocking in a supermarket, implemented in C with POSIX threads and semaphores.

# Supermarket Supply Simulation

This project simulates the process of supplying a supermarket with products delivered by trucks and stocked by employees, using a concurrent program written in C with POSIX threads and semaphores.

## 📌 Problem Description

In a supermarket, goods arrive by trucks. The unloading area can hold a maximum of `B` boxes at once. Trucks deliver orders of up to `C` boxes each. If the delivery exceeds the available space, the truck must wait until there's room to unload the remaining boxes. 

Employees (threads) move the boxes from the unloading area to the storage area. Each employee transports one box at a time, with a delay simulating their walking time.

The program ends when a total of `N` boxes have been delivered and stocked.

---

## 🚀 How to Run

Compile the program:

```bash
gcc -pthread -o supermarket mercado.c

./mercado N B C F T L

Where: 
N (1 < N < 100): total number of boxes to deliver.
B (1 < B < 10): maximum boxes in the loading area.
C (1 < C < 5): maximum boxes per truck.
F (1 < F < 3): number of employees.
T (1 < T < 20): maximum interval between truck arrivals (in seconds).
L (1 < L < 15): maximum time an employee takes to deliver a box (in tenths of seconds).

example: ./mercado 20 5 4 2 12 10
