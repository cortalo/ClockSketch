## Sliding Sketch

### Introduction

Data stream processing becomes a hot issue in recent yearsdue to the arrival of big data era. There are three fundamen-tal stream processing tasks: membership query, frequency query and heavy hitter query. While most existing solutions address these queries in fixed windows, this paper focuses onmore challenging queries: answering these queries in sliding windows. While most existing solutions address each queries individually, by using different algorithms, this paper focuses on a generic framework. In this paper, we propose a generic framework, namely Sliding sketches, which can be applied to many existing solutions for the above three queries, and enable them to support queries in sliding windows. Our key idea is to introduce time zones to sketches for the first time, and we achieve that a query at any time from multiple time zones can get at least one accurate result, which has a small jet lag. We apply our framework to five state-of-the-art sketches for the above three kinds of queries. Theoretical analysis and extensive experimental results show that after using our framework, the accuracy of existing sketches that do not support sliding windows becomes much higher than the corresponding best algorithms in sliding windows.

### About this repo

This repo contains all the algorithms in our experiments, as shown in the following table.

| Task                      | Algorithms                               |
| ------------------------- | ---------------------------------------- |
| Membership query          | Sliding Bloom Filter |
| Estimating item frequency | Sliding CM Sketch, Sliding CM-CU Sketch. Sliding Count Sketch |
| Finding heavy hitter      | Sliding HeavyKeeper             |

### How to build

The project is built upon [cmake](https://cmake.org/). You can use the following commands to build and run.

```
mkdir build
cd build
make
```
