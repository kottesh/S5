import math
import csv

# Function to calculate entropy
def entropy(labels):
    total = len(labels)
    label_counts = {}
    
    for label in labels:
        if label in label_counts:
            label_counts[label] += 1
        else:
            label_counts[label] = 1
    
    ent = 0.0
    for label in label_counts:
        probability = label_counts[label] / total
        ent -= probability * math.log2(probability)
    
    return ent

# Function to calculate the information gain
def information_gain(data, feature_index, target_index):
    # Calculate the total entropy for the target
    total_entropy = entropy([row[target_index] for row in data])

    # Calculate the values and their frequencies for the feature
    values = {}
    for row in data:
        feature_value = row[feature_index]
        if feature_value in values:
            values[feature_value].append(row)
        else:
            values[feature_value] = [row]

    # Calculate the weighted entropy after splitting
    weighted_entropy = 0.0
    total_count = len(data)
    for subset in values.values():
        subset_entropy = entropy([row[target_index] for row in subset])
        weighted_entropy += (len(subset) / total_count) * subset_entropy

    # Information gain is the reduction in entropy
    return total_entropy - weighted_entropy

# Function to split the data based on a feature
def split_data(data, feature_index, value):
    subset = []
    for row in data:
        if row[feature_index] == value:
            reduced_row = row[:feature_index] + row[feature_index+1:]
            subset.append(reduced_row)
    return subset

# Function to determine the best feature to split on
def best_feature_to_split(data, target_index):
    num_features = len(data[0]) - 1  # Excluding the target
    best_gain = 0
    best_feature = -1
    
    for feature_index in range(num_features):
        gain = information_gain(data, feature_index, target_index)
        if gain > best_gain:
            best_gain = gain
            best_feature = feature_index
            
    return best_feature

# Function to build the decision tree with print statements for each step
def build_tree_with_print(data, target_index, header, depth=0):
    # Get the target values
    target_values = [row[target_index] for row in data]
    
    # If all target values are the same, return that value
    if target_values.count(target_values[0]) == len(target_values):
        print(f"{'|   ' * depth}Leaf node: Predict = {target_values[0]}")
        return target_values[0]
    
    # If no more features, return the majority target value
    if len(data[0]) == 1:  # Only target left
        majority_value = max(set(target_values), key=target_values.count)
        print(f"{'|   ' * depth}Leaf node: Predict = {majority_value}")
        return majority_value
    
    # Best feature to split on
    best_feature = best_feature_to_split(data, target_index)
    best_feature_name = header[best_feature]
    
    print(f"{'|   ' * depth}Node: Splitting on {best_feature_name}")
    
    # Create the tree structure
    tree = {best_feature_name: {}}
    
    # Get all unique values of the best feature
    feature_values = set(row[best_feature] for row in data)
    
    for value in feature_values:
        print(f"{'|   ' * (depth+1)}Branch: {best_feature_name} = {value}")
        
        # Split the data and build the sub-tree recursively
        subset = split_data(data, best_feature, value)
        tree[best_feature_name][value] = build_tree_with_print(subset, target_index-1, header[:best_feature] + header[best_feature+1:], depth+1)
    
    return tree

# Load and prepare the dataset
def load_data(file_path):
    with open(file_path, mode='r') as file:
        reader = csv.reader(file)
        data = [row for row in reader]

    # Remove the identifier column (assuming it's the first column)
    header = data[0][1:]
    data = [row[1:] for row in data[1:]]

    return header, data

# Main function to run the decision tree
def main():
    file_path = 'data.csv'
    header, data = load_data(file_path)
    
    # Build and print the decision tree
    tree = build_tree_with_print(data, target_index=len(data[0])-1, header=header)
    print(tree)

if __name__ == "__main__":
    main()
