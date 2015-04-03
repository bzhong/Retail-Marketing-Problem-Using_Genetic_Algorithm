# Retail Marketing Problem Using Genetic Algorithm
## Problem Definition
Retail clustering is to classify customer information and mine some useful conclusions in order to help marketing decisions. The data source is a Q&A survey including several aspects of one supermarket, which represents feedbacks from customers. Also the survey contains customers’ brief personal information such as nationality, age, gender, email and phone number. So the program’s inputs are these original data, each customer’s all information in each row of xlsx format data sheet.

Next, to solve clustering problem, traditionally using neural networks is appropriate. Evolutionary Computation especially Genetic Algorithm is used to solve optimization problems. But if I use a little trick to turn clustering problem to be optimization problem then I can use genetic algorithm to solve it. Carefully observing clustering problem I can see that each cluster can be described as a irregular shape. It has a ‘center’ that has different meaning in different circumstances. For example, in fuzzy logic, the center occurred at defuzzification process is named center of gravity. In my case, the center is a point to make sure the sum of distances between all data points belonging to this cluster and the center is shortest one. Thus what I actually expect to find is a set of these virtual centers that can bring best clustering. Since each customer answers ten questions, the virtual center has ten dimensions to represent it. Now I can adjust values of each dimensions for each virtual center to make clustering better. That is an optimization problem and genetic algorithm will be a better choice. Therefore, the processing is genetic algorithm.

As for outputs, I expect to see several clusters which contain corresponding data points. Meanwhile, I expect to analyze the data points in each cluster to obtain deeper useful information for marketing. Therefore, the outputs are multiple clusters with mined data distribution.

Notice the outputs are not graphs with outlines surrounding those data points. Since the dimension of data points is too high (ten dimensions for ten survey problems), I can’t draw the distribution figure. Actually if I know positions of all centers of these clusters, and I define that one data point belongs to certain cluster if the distance between the point and the center is shorter than that between the data point and any other centers, I have completed the clustering problem. Then I can classify each real data point to corresponding cluster, obtain statistics features for one cluster such as nationality, gender, age and so on. Based on these figures, I can provide analysis results for marketing.

## Simulations
#### Program outline
Programming language: C++  
Inputs:  
number of chromosomes  
number of clusters  
number of iterations  
probability for crossover and mutation respectively  
selection strategy: Roulette Wheel or Tournament  
Outputs:  
results of eight data sets (morning: FN, HR, MK, OP and evening: FN, HR, MK, OP)  
in each data set, display every cluster with its value of center  
in each cluster, display distribution for age and gender respectively

#### How to build and run
You can use any compilers or IDEs (g++ in Linux, Xcode in Mac OS X, Visual Studio in Windows) to compile and run. Easy enough:).
