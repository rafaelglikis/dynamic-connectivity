# Introduction
There is given an undirected graph G = (V, E) from which edges are deleted one at a time.
Questions like "Are the vertices u and v in the same connected component?" have to be answered "on-line".

This is an implementation of an algorithm ([ES81] S. Even and Y. Shiloach, "An On-Line Edge-Deletion Problem",
Journal of the ACM, 28(1):1-4, 1981) which maintains a data structure in which each question is answered in constant
time and for which the total time involved in answering q questions and maintaining the data structure is O(q+|V|*|E|), 
and O(q+|E|*log(|E|)) for acyclic graphs.

# Usage
Usage:
  dynamic_connectivity ACTION* [OPTION]*

Example:
  dynamic_connectivity benchmark --random -v 500 -e 1000 -d 50 -q 50

Test:
  test                        run all tests

Benchmark:
  benchmark                   run benchmark specified benchmarks
  --path                      run benchmark with path graph
  --tree                      run benchmark with tree graph
  --random                    run benchmark with tree graph
  --path-complete             run benchmark with path of partly complete graphs

Benchmark options:
  -v, --vertices <number>     specify number of vertices (default 10000)
  -e, --edges <number>        specify number of edges (default 10000*log(10000))
  -d, --deletions <number>    specify number of deletions (default 5000)
  -q, --queries <number>      specify number of queries (default 5000)

Examples:
  example                     run an example