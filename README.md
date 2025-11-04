[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/7RAFczHS)
# Project 4 - HashTable

Project description can be found [in the repo](Project4_HashTable.pdf)

Place your complexity analysis below.

---

insert : = O(O(n)+O(n)+O(n^2)+O(infinite)) = O(n^2) The setUpProbeOffsets function is O(infinite) because at worst case it keeps trying to find the last random number to put in the list and could potentially never find it

remove: O(n) because the the longest O time is getIndex function is O(n) at worst case it iterates through the whole vector table looking for the right key

contains:  O(n) because the the longest O time is getIndex function which is O(n) as described for remove

get: O(n) because the the longest O time is getIndex function which is O(n) as described for remove

operator[]:  O(n) because the the longest O time is getIndex function which is O(n) as described for remove

