# Matrix mulitplication algorithms
  - Brute force algorithm has O(n^3) time complexity and O(1) space complexity and is faster than divide-and-conquere and Strassen's algorithms for smaller n number (n < 2048) due to it's simplicity.
  - Divide-and-concure algorithm has O(n^3) time complexity and requires additional space up to O(n^3) due to extra calculations for 8 sub-problems of size n/2. It starts to outperform brute force algorithm at n >= 4096.
  - Strassen's algorithm has a lower assymptotic time complexity of O(n^lg(7))=O(n^2.81), but it requires an extra space like a div-and-c algorithm. It outperforms the divide-and-c algorihtm and starts to outperform brute force algorithm at n>=4096.
    
