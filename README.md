<h1 align="center">Cutting Learned Index into Pieces: An In-depth Inquiry into Updatable Learned Indexes</h1>

### Abstract
Recently, numerous high-performance updatable learned indexes have been designed to support the writing requirements in practical systems. Researchers have proposed various strategies to improve the availability of updatable learned indexes. However, it is unclear which strategy is more profitable. Therefore, we deconstruct the design of learned indexes into multiple dimensions and in-depth evaluate their impacts on the overall performance, respectively.  Through the in-depth exploration of learned indexes, we reckon that the approximation algorithm is the most crucial design dimension for improving the performance of the learned indexes rather than the popular works that focus on the learned index structure. Moreover, this paper makes a comprehensive end-to-end evaluation based on a high-performance key-value store to answer people’s concerns about which learned index is better and whether learned indexes can outperform traditional ones. Finally, according to end-to-end and in-depth evaluation results, we give some constructive suggestions on designing a better learned index in these dimensions, especially how to design an excellent approximate algorithm to improve the lookup and insertion performance of learned indexes.

### Authors
Jiake Ge, Boyu Shi, Yanfeng Chai, Yuanhui Luo, Yunda Guo, Yinxuan He, and Yunpeng
Chai*. 

### How to run the benchmark for Learned Index
1. Generate YCSB data using the benchmark/generate_ycsb.sh
2. Set different learned indexes to test by changing index_num at benchmark/fixtures/viper_fixture.hpp:127
3. Change path in redo.sh and execute
