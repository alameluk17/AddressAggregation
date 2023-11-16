# IP Address Aggregation Project

## Overview

This C project focuses on IP address aggregation for efficient utilization of address space. It allows users to input a starting IP address and prefix, and then defines address aggregations for multiple groups based on user-specified parameters such as the number of customers per group and addresses per customer.

## Project Structure

The project consists of a single C file:

- **`ip_aggregation.c`**: This file contains the main implementation of the IP address aggregation algorithm. It includes functions for calculating total hosts, finding the next power of two, printing IP addresses, calculating end IP addresses, creating customer nodes, and aggregating and printing customer IP addresses.

## Getting Started

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/your-username/ip-aggregation-project.git
3. Compile the C file:
   ```bash
   gcc ip_aggregation.c -o ip_aggregation
4. Run the executable:
   ```bash
   ./ip_aggregation
5. Follow the on-screen prompts to input the starting IP address, prefix, and group details.

## Usage

The program prompts the user to enter the starting IP address and prefix. It then iterates over multiple groups, allowing the user to specify the number of customers and addresses per customer for each group. The program calculates and displays the address aggregation for each group, along with the corresponding customer IP addresses.
