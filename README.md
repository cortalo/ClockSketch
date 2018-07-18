## Sliding Sketch

### Introduction

Data stream processing becomes a hot issue in recent yearsdue to the arrival of big data era. There are three fundamen-tal stream processing tasks: membership query, frequencyquery and heavy hitter query. While most existing solutionsaddress these queries in fixed windows, this paper focuses onmore challenging queries: answering these queries in slidingwindows. While most existing solutions address each queriesindividually, by using different algorithms, this paper focuseson a generic framework. In this paper, we propose a genericframework, namely Sliding sketches, which can be appliedto many existing solutions for the above three queries, andenable them to support queries in sliding windows. Our keyidea is tointroduce time zones to sketches for the first time, andwe achieve that a query at any time from multiple time zonescan get at least one accurate result, which has a small jet lag.We apply our framework to five state-of-the-art sketchesfor the above three kinds of queries. Theoretical analysisand extensive experimental results show that after usingour framework, the accuracy of existing sketches that donot support sliding windows becomes much higher than thecorresponding best algorithms in sliding windows.

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
