import networkx as nx
import matplotlib.pyplot as plt 
import random
import numpy as np


def create_spanning_tree(G):
    # First phase
    F = nx.Graph()
    while True:
        # Apply expansion rule with the highest priority
        rule_applied = False
        for v in F.nodes():
            neighbors = set(G.neighbors(v)) - set(F.nodes())
            if len(neighbors) >= 2:
                # Rule 1
                for w in neighbors:
                    F.add_edge(v, w)
                rule_applied = True
                break
            elif len(neighbors) == 1:
                w = list(neighbors)[0]
                w_neighbors = set(G.neighbors(w)) - set(F.nodes())
                if len(w_neighbors) >= 3:
                    # Rule 2
                    #expand v
                    F.add_edge(v, w)
                    #expand w
                    for u in w_neighbors:
                        F.add_edge(w, u)
                    rule_applied = True
                    break
                elif(len(w_neighbors) == 2):
                    # Rule 3
                    #expand v
                    F.add_edge(v, w)
                    #expand w
                    for u in w_neighbors:
                        F.add_edge(w, u)
                    rule_applied = True
                    break
        
        # Rule 4
        if not rule_applied:
            for v in G.nodes():
                if v not in F.nodes():
                    neighbors = set(G.neighbors(v)) - set(F.nodes())
                    if len(neighbors) >= 3:
                        #expand v
                        for w in neighbors:
                            F.add_edge(v, w)
                        rule_applied = True
                        break
        
        if not rule_applied:
            break

    F_star = F.copy()


    # Second phase
    while not (nx.number_of_nodes(F) == nx.number_of_nodes(G)):
        for v in G.nodes():
            if v in F.nodes():
                continue
            neighbors = set(G.neighbors(v)) & set(F.nodes())
            if len(neighbors) > 0:
                w = list(neighbors)[0]
                F.add_edge(v, w)
                # break
    while not nx.is_connected(F):
        for c1, c2 in nx.connected_components(F):
            for v in c1:
                for w in c2:
                    if (v, w) in G.edges():
                        F.add_edge(v, w)
                        break
            break
    T_star = F.copy()
    return T_star




#input: A connected simple undirected graph G with maximum degree at least 3

# # Create an empty graph
# G = nx.Graph()

# # Add edges to create a graph with a maximum degree of at least 3
# G.add_edges_from([(1, 2), (1, 3), (1, 4), (2, 3), (2, 4), (2, 5), (3, 4), (3, 5), (3, 6)])

# Create empty graph
G = nx.Graph()

# Add 1000 nodes to the graph
for i in range(50):
    G.add_node(i)

# Connect each node to at least 3 other randomly chosen nodes
for node in G.nodes():
    while len(list(G.neighbors(node))) < 3:
        neighbor = np.random.choice(list(G.nodes()))
        if neighbor != node and not G.has_edge(node, neighbor):
            G.add_edge(node, neighbor)



plt.figure(1)
nx.draw_networkx( G,
        pos=nx.spring_layout(G, iterations=1000), 
        arrows=False, with_labels=True )
plt.show()

# Print the maximum degree of the graph
print("Maximum degree: ", max([d for n, d in G.degree()]))

# Get the maximum leaves spanning tree
T = create_spanning_tree(G)

plt.figure(2)
nx.draw_networkx( T,
        pos=nx.spring_layout(T, iterations=1000),
        arrows=False, with_labels=True )
plt.show()

# Print the edges of the tree
print(T.edges())