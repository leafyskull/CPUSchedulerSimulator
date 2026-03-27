

- What to report: <br>
1. "What values of base and scaling factors provided a good tradeoffs of the above performance metrics." <br>

My first thoughts on the scaling of our scaling factors: <br>
- The average service time we're using is 0.06 seconds. My understanding is that an ideal quantum should be such that 80-85% of processes should complete in one CPU burst, so it should perhaps be larger than the average service time. Just for testing sake, I'll try a base quantum that is lower than the average service time, equal, and higher. <br>

- If A is too high, we won't be adding much quantum time to high priority processes.
- If A is too low, we will be allocating too much time potentially.
- If B is too high, we'll be allocating the minimum possible quantum, and get a high number of average context switches per process.
- If B is too low, we won't be reducing the quantum for processes with a high execution time, potentially letting them monopolize the CPU.

My hypothesis is that we probably want A and B to not differ too much, nor be too large in scale. I'll try some single-digit, similar values for the two of them and see. <br>

First, I made A and B the same value, and tried values 1 - 10. Here are some results: <br>

> A, B = 1: <br>
Average turnaround time: 23603.361710 seconds <br>
Average number of context switches per process: 1.420200 <br>
Average number of processes in the readyQueue: 5213.383257 <br>

> A, B = 2: <br>
Average turnaround time: 11781.713460 seconds <br>
Average number of context switches per process: 1.405200 <br>
Average number of processes in the readyQueue: 5137.678888 <br>

> A, B = 3: <br>
Average turnaround time: 7688.032752 seconds <br>
Average number of context switches per process: 1.380200 <br>
Average number of processes in the readyQueue: 5025.323338 <br>

> A, B = 4: <br>
Average turnaround time: 5738.973662 seconds <br>
Average number of context switches per process: 1.401500 <br>
Average number of processes in the readyQueue: 4952.941486 <br>

> A, B = 5: <br>
Average turnaround time: 4497.511385 seconds <br>
Average number of context switches per process: 1.403300 <br>
Average number of processes in the readyQueue: 4825.051738 <br>

> A, B = 6: <br>
Average turnaround time: 3794.816305 seconds <br>
Average number of context switches per process: 1.381200 <br>
Average number of processes in the readyQueue: 4788.238021 <br>

> A, B = 7: <br>
Average turnaround time: 3171.714303 seconds <br>
Average number of context switches per process: 1.434100 <br>
Average number of processes in the readyQueue: 4672.340185 <br>

> A, B = 8: <br>
Average turnaround time: 2742.485075 seconds <br>
Average number of context switches per process: 1.485500 <br>
Average number of processes in the readyQueue: 4607.254182 <br>

> A, B = 9: <br>
Average turnaround time: 2404.120421 seconds <br>
Average number of context switches per process: 1.517600 <br>
Average number of processes in the readyQueue: 4525.638143 <br>

> A, B = 10: <br>
Average turnaround time: 2193.098936 seconds <br>
Average number of context switches per process: 1.510700 <br>
Average number of processes in the readyQueue: 4480.717564 <br>

Ok so far, we seem to be benefitting by increasing these parameters. <br>


2. "Are there any starvation concerns? How would you tackle them." <br>


3. "Show a graph between priority and turnaround time." <br>


4. "Based on your recommended values, how this scheduler compares to a vanilla RR with the base
   quantum." <br>





Idea:
- First, find a good combination of scaling factors.
- Create a graph comparing priority and turnaround time with these optimal factors
- Assess starvation concerns, if any
- Compare to vanilla RR (very high A, B = 0)