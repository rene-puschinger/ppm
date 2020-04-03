# ppm
Prediction by Partial Matching implementation

**Created by René Puschinger in 2007 (revised in 2015).**

A minimal implementation (console only) of the PPM data compression algorithm as presented in the paper
"Data Compression Using Adaptive Coding and Partial String Matching" by John G. Cleary and Ian H. Witten
(IEEE TRANSACTIONS ON COMMUNICATIONS)

**The code is intended for educational purposes only.**

Compilation
-----------
To build the application you can use the GNU C++ Compiler:
```
g++ *.cpp -o ppm
```

Usage
-----
To encode a file book.txt, use
```
ppm c book.txt book.ppm
```
This will create the compressed version book.ppm.

To decompress the file, use
```
ppm d book.ppm book2.txt
```

This will decompress the file equal to the original file.

Benchmark
---------
A benchmark of various popular compression tools follows to compare them with PPM. The less size of the compressed file the better. In all cases default options have been used.

Original file: book.txt (included in the test-data folder), size 1 344 739 bytes.

Compressed by ZIP (LZSS compression mod): size 490 883 bytes.

Compressed by 7-Zip (LZSS compression mod): size 386 768 bytes.

Compressed by BZ2 (Burrows-Wheeler's Transform): size 349 584 bytes.

Compressed by **PPM**: size **323 934** bytes.

Bibliography
------------
(in BibTeX notation)

@ARTICLE{Cleary84datacompression,
    author = {John G. Cleary and Ian H. Witten},
    title = {Data Compression Using Adaptive Coding and Partial String Matching},
    journal = {IEEE TRANSACTIONS ON COMMUNICATIONS},
    year = {1984},
    volume = {32},
    number = {4},
    pages = {396--402}
}

@article{citeulike:582182,
    author = {Bell, Timothy and Witten, I. H. and Cleary, J. G.},
    citeulike-article-id = {582182},
    journal = {ACM Computing Surveys},
    keywords = {bibtex-import},
    number = {4},
    pages = {557--592},
    posted-at = {2006-04-11 18:20:50},
    priority = {0},
    title = {Modeling for text compression},
    volume = {21},
    year = {1989}
}

@article{journals/cacm/WittenNC87,
  added-at = {2011-06-07T00:00:00.000+0200},
  author = {Witten, Ian H. and Neal, Radford M. and Cleary, John G.},
  biburl = {http://www.bibsonomy.org/bibtex/2c45cf90746fc77c11307375db1210050/dblp},
  ee = {http://doi.acm.org/10.1145/214762.214771},
  interhash = {cac6eef352e981b984056fc969914aed},
  intrahash = {c45cf90746fc77c11307375db1210050},
  journal = {Commun. ACM},
  keywords = {dblp},
  number = 6,
  pages = {520-540},
  timestamp = {2011-06-08T11:41:10.000+0200},
  title = {Arithmetic Coding for Data Compression.},
  url = {http://dblp.uni-trier.de/db/journals/cacm/cacm30.html#WittenNC87},
  volume = 30,
  year = 1987
}

@techreport{citeulike:513403,
    abstract = {We provide a tutorial on arithmetic coding, showing how it provides nearly optimal data compression and how it can be matched with almost any probabilistic model. We indicate the main disadvantage of arithmetic coding, its slowness, and give the basis of a fast, space-efficient, approximate arithmetic coder with only minimal loss of compression efficiency. Our coder is based on the replacement of arithmetic by table lookups coupled with a new deterministic probability estimation scheme. Index...},
    author = {Howard, Paul G. and Vitter, Jeffrey S.},
    citeulike-article-id = {513403},
    citeulike-linkout-0 = {http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.48.8561},
    keywords = {arithmetic, coding, compression},
    number = {CS-91-45},
    posted-at = {2006-02-20 15:25:45},
    priority = {0},
    title = {Practical Implementations of Arithmetic Coding},
    url = {http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.48.8561},
    year = {1991}
}
