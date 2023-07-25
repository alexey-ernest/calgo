# Matrix mulitplication algorithms
  - Brute force algorithm has O(n^3) time complexity and O(1) space complexity and is faster than divide-and-conquere and Strassen's algorithms for smaller n number (n < 2048) due to it's simplicity.
  - Divide-and-concure algorithm has O(n^3) time complexity and requires additional space up to O(n^3) due to extra calculations for 8 sub-problems of size n/2. It starts to outperform brute force algorithm at n >= 4096.
  - Strassen's algorithm has a lower assymptotic time complexity of O(n^lg(7))=O(n^2.81), but it requires an extra space like a div-and-c algorithm. It outperforms the divide-and-c algorihtm and starts to outperform brute force algorithm at n>=4096 (160s Strassen's vs 800s brute vs 330s divide-and-c).
    
## Expiremental results
 - ./matrix_brute.o 2048  22.84s user 0.11s system 99% cpu 23.056 total
 - ./matrix_dc.o 2048  9.47s user 0.05s system 99% cpu 9.568 total
 - ./matrix_strassen.o 2048  3.56s user 0.04s system 99% cpu 3.629 total
 - ./matrix_dc.o 4096  75.10s user 0.35s system 99% cpu 1:15.85 total
 - ./matrix_strassen.o 4096  24.35s user 0.18s system 99% cpu 24.624 total
 - ./matrix_strassen.o 8192  171.93s user 0.94s system 99% cpu 2:53.80 total
 - ./matrix_strassen.o 16384  1223.34s user 11.15s system 98% cpu 20:49.63 total
 - ./matrix_strassen.o 32768  8460.71s user 89.33s system 98% cpu 2:24:30.79 total