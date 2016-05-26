
# Summary

## Introduction

I was part of the Early English Book group (EEBO).
Our research questions are:

1. What kinds of texts were recycled?
2. Can we identify groups of texts that are “related” through their content?


## What I did

I am mainly responsible for the data analysis part, including:

- constructing author and text networks
- network analysis
  - ranking the nodes using PageRank
  - community detection (network clustering)
- visualization of the network analysis result
- topic modeling and result visualization


## What I brought to the group

I brought the following:

- network analysis result and visualization
- topic modeling result and  visualization

so that humanity people in our group can interact with the visualization to gain deeper understanding of the data or check with their interpretation. 

Humanity people might also get to know that computer is powerful and complementary to human intelligence. 


## What I learned

Two aspects: group work and technical:

- how to work with humanity people
  - be patient, use easy-to-understand terms at the beginning
  - listen to them carefully to understand what they need
  - be proactive because usually there is a gap between knowing what to do and what can be done
- how to work with others
  - trust them
  - support them
- technical aspects
  - better usage with Gephi (network analysis tool), for example, I get to know: 
    - the difference among different network layouts
    - how to do community detection
  - topic modeling and visualization in R using library `lda`and `LDAvis`

## What to learn in the future

- group work
  - how to express my ideas
  - how to find some common interest with others
- technical aspects:
  - faster way to visualize network using some ready-made software, instead of coding it
  - more network clustering algorithms and understand how they work
  - more topic modeling algorithms

## What to praise for this hackathon

### Theme

I like the idea, bringing together people from different backgrounds.
It's good to talk and share with them.

### Organization

The organization is highly efficient and sensible.
Also, the datasets are very interesting to study.
Last, free coffee and tea is nice.

### Awesome teammates

I can feel their trust and support. 

## What can be improved

### More international

I am the only non-Finnish speaker in the group, so in some cases, Finnish becomes the primary language for the sake of more effective communication. 
However, I still hope English can be used more often because this course is English and intended for people from different backgrounds (including language).

### Better communication

A more effective channel for us to know what everyone is doing would be good. 



## Other interesting ideas

### Automatic spelling variation detection

I am not sure how this is done in reality.
But [this paper](http://ucrel.lancs.ac.uk/publications/CL2007/paper/238_Paper.pdf) does this job automatically.
Basically, it clusters words with similar spelling into groups and with this information, spelling normalization is easier. 

### Multi-typed network

In our current approach, we consider uni-typed network (either author or text as the network node).
It would be interesting to construct multi-typed network for our dataset. 
In this network, author, text and possibly other types of objects are linked together in the same network. 
There are works in computer science field that study graph clustering for this kind of network.
For example, [this paper](http://www.ccs.neu.edu/home/yzsun/papers/kdd09_netclus.pdf) utilizes ranking information for the purpose of clustering in multi-typed network.
  
# Related documents

- [Poster](http://dhh16.hiit.fi/dhh16-eebo-poster.pdf) (PDF)
- [Presentation](http://dhh16.hiit.fi/dhh16-eebo-slides.pdf) (PDF)
- [Code](https://github.com/dhh16/eebo) (Github)
- Overlap network, 16th century: [full authors](https://www.cs.helsinki.fi/u/hxiao/eebo/sixteen-author-long/) · [short authors](https://www.cs.helsinki.fi/u/hxiao/eebo/sixteen-author-short/) · [texts](https://www.cs.helsinki.fi/u/hxiao/eebo/sixteen-text/)
- Overlap network, Civil War: [authors](https://www.cs.helsinki.fi/u/hxiao/eebo/civil-war/) · [texts](https://www.cs.helsinki.fi/u/hxiao/eebo/civil-war-text/)
- Topic models: [16th century](https://www.cs.helsinki.fi/u/hxiao/eebo/topic_sixteen/) · [Civil War](https://www.cs.helsinki.fi/u/hxiao/eebo/topic_civil_war/)