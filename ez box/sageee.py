#!/usr/bin/python3

#		THIS FILE SERVER ONLY AS STORAGE FOR CODE THAT WAS RUN IN COCALC

def appendSBoxes(newSBox, SBoxes):
    for s in SBoxes:
        if np.array_equal(newSBox, s):
            return SBoxes
    SBoxes = np.vstack([SBoxes, newSBox])
    return SBoxes


from sage.crypto.sbox import SBox
import numpy as np
# ---------------------------------------------	ANALYSIS ON RANDOM ----------------------------
    
# S = SBox(a); S
# ddt = S.difference_distribution_table()
# nddt = np.array(ddt)
# print(np.partition(nddt.flatten(), -2)[-2])

SBoxes = np.empty((0,16), int)
a = np.random.permutation([15,2,11,4,9,0,7,8,6,10,1,12,14,3,13,5])
SBoxes = np.vstack([SBoxes, a])

while len(SBoxes) < 100:
    newSbox = np.random.permutation([15,2,11,4,9,0,7,8,6,10,1,12,14,3,13,5])
    SBoxes = appendSBoxes(newSbox, SBoxes)

du_maxes = []
non_lin = []
combined = []
for s in SBoxes:
    SB = SBox(s)
    d = SB.differential_uniformity()
    n = SB.nonlinearity()
    du_maxes = np.append(du_maxes, d)
    non_lin = np.append(non_lin, n)
    if n == 0:
        combined = np.append(combined, d)
    else:
        combined = np.append(combined, (d / n))
    
print(du_maxes)
print(non_lin)
print(combined)



------------------------         GENETIC ALGORITHM ----------------------------------------------



import random


def fitness(permutation):
    SB = SBox(permutation)
    d = SB.differential_uniformity()
    n = SB.nonlinearity()
    mlrb = SB.maximal_linear_bias_relative()
    if n == 0:
        fit = d * mlrb
    else:
        fit = (d / n) * mlrb
    if SB.is_apn():
        fit = 0
    return fit

def rankPop(population):
    fitnessResults = {}
    for i in range(0,len(population)):
        fitnessResults[i] = fitness(population[i])
    return sorted(fitnessResults.items(), key = operator.itemgetter(1), reverse = False)

def selection(popRanked, eliteSize):
    results = []
    for i in range(0, eliteSize):
        results.append(popRanked[i][0])
    for i in range(0, 40):
        results.append(random.choice(popRanked)[0])
    return results


def matingPool(population, selectionResults):
    matingpool = []
    for i in range(0, len(selectionResults)):
        index = selectionResults[i]
        matingpool.append(population[index])
    while len(matingpool) < 100 :
        matingpool.append(np.random.permutation([15,2,11,4,9,0,7,8,6,10,1,12,14,3,13,5]))
    return matingpool
        
def breed(parent1, parent2):
    child = []
    childP1 = []
    childP2 = []
    
    geneA = int(random.random() * len(parent1))
    geneB = int(random.random() * len(parent1))
    
    startGene = min(geneA, geneB)
    endGene = max(geneA, geneB)

    for i in range(startGene, endGene):
        childP1.append(parent1[i])
        
    childP2 = [item for item in parent2 if item not in childP1]

    child = childP1 + childP2
    return child
    

def breedPopulation(matingpool, eliteSize):
    children = []
    length = len(matingpool) - eliteSize
    pool = random.sample(matingpool, len(matingpool))

    for i in range(0,eliteSize):
        children.append(matingpool[i])
    
    for i in range(0, length):
        child = breed(pool[i], pool[len(matingpool)-i-1])
        children.append(child)
    return children


def mutate(individual, mutationRate):
    for swapped in range(len(individual)):
        if(random.random() < mutationRate):
            swapWith = int(random.random() * len(individual))
            
            city1 = individual[swapped]
            city2 = individual[swapWith]
            
            individual[swapped] = city2
            individual[swapWith] = city1
    return individual

def mutatePopulation(population, mutationRate):
    mutatedPop = []
    
    for ind in range(0, len(population)):
        mutatedInd = mutate(population[ind], mutationRate)
        mutatedPop.append(mutatedInd)
    return mutatedPop

def nextGeneration(currentGen, eliteSize, mutationRate):
    popRanked = rankRoutes(currentGen)
    selectionResults = selection(popRanked, eliteSize)
    matingpool = matingPool(currentGen, selectionResults)
    children = breedPopulation(matingpool, eliteSize)
    nextGeneration = mutatePopulation(children, mutationRate)
    return nextGeneration


-----------------------------		TRAINING 	--------------------------------------

pop = SBoxes
progress = []
progress.append(1 / rankRoutes(pop)[0][1])
generations = 2
    
for i in range(0, generations):
    pop = nextGeneration(pop, eliteSize, mutationRate)
    progress.append(1 / rankRoutes(pop)[0][1])
    
plt.plot(progress)
plt.ylabel('Distance')
plt.xlabel('Generation')
plt.show()