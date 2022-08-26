# Thread

<h2>Compilation Instructions</h2>
<b>gcc JKim_thd.c -o JKim_thd.exe -lpthread<br>					                                                  
./JKim_thd.exe 1000000 10 > JKimResultC.txt<br>                                                                                            
./JKim_thd.exe 1000000 50 50 > JKimResultG.txt</b>


<h2>Program Design</h2>
This program demonstrates operating system’s ability to break down a big task into smaller tasks and delegate them among multiple threads. It is the “divide-and-conquer” approach. This program sums each number starting from 1 to 1000000 or whatever number a user designates. The user specifies how many child and grandchild threads to delegate for this summing task. The program equally divides the workload among the threads and puts them to work. An array keeps track of each child thread’s sum and the main/parent process totals them up to produce the final sum. Essentially, this program executes the following sequence summation formula:<br>

![image](https://user-images.githubusercontent.com/44533760/186806292-cde8ea0e-7a2f-4cd9-893f-f80fcc2f3af5.png)

Thread is a lightweight method for an operating system to complete multiple tasks simultaneously. The following code creates and runs threads using a for-loop. This was the heart of the program:<br>

![image](https://user-images.githubusercontent.com/44533760/186806325-022448d2-59c8-4174-9f65-ac84e28315a4.png)<br>
![image](https://user-images.githubusercontent.com/44533760/186806333-b482e879-f6ea-4382-9222-39a9e18b7de7.png)

Once created, a thread executes a runner function that takes a single pointer parameter:<br>
![image](https://user-images.githubusercontent.com/44533760/186806346-4af0493c-7bcf-4c4f-8f55-d05d2e81836e.png)

