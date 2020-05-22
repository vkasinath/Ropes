HOMEWORK 8: ROPES


NAME:  Vishaal Kasinath


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

https://www.geeksforgeeks.org/wp-content/uploads/Preorder-from-Inorder-and-Postorder-traversals.jpg
https://kukuruku.co/post/ropes-fast-strings/
https://www.geeksforgeeks.org/write-a-c-program-to-get-count-of-leaf-nodes-in-a-binary-tree/
https://stackoverflow.com/questions/2826431/stl-rope-when-and-where-to-use


Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  20

BALANCED/WORST CASE RUNNIG TIME ANALYSIS:
n = characters in the string of the current Rope (*this)
m = characters in the string of the Rope passed in (for functions that take
    a second rope)
v = longest "value" of a leaf
p = longest path from root to a leaf
Using these variables, state and explain the big-O running time of each 
of the following operations two ways: 1) with a balanced Rope, 2) with an 
extremely unbalanced Rope. Explain what you considered an extremely 
unbalanced rope for the string "Hello my name is Simon".

an unbalanced rope is where one subtree is empty or has very few substrings 
and the other subtree is heavily loaded with multiple depths of small substrings
something similar to shown below
				H
               /  \
              e    l
                    \
                     l
                    / \
                   o    m
                       / \
                      y    n
                          / \
                          a  m
                            / \
                            e  i
                              / \
                              s  S
                                / \
                                i  m
                                  / \
                                  o  n


Copy Constructor: O(p)
Recursive copy tree will constantly shift between left and right, traversing from top to bottom.
Has to do a left and right copy (which is of O(1)) at every depth.
A balanced rope will have comparatively smaller depth than an unbalanced rope for the same string

Construct from Node*: O(1)
Close to O(1) for large value of v (rope with single node where v=entire string length)
Closer to O(p) for small value of v (rope with a single character in each node thereby having a deep long row)

Index: O(p*v)
Worst case scenario of finding very last character, have to go through all depths before getting to the very last node 

Report: O(n)
i can be 0 and j can be the end of the string which means we have to go through the enitre string 

iterator operator++: O(n)
in order traversal requires starting from root to go to the last farthest left node.
deeper the rope, the more iterations traversing the nodes

Split: O(v)
If we have a very short rope with only two nodes and both nodes have 10,000 characters,
split will be dependent on getting to the right index in the value of the node
conversely, if we have a long rope, the number of characters in each node is going to be significantly smaller,
however, our traversal from top to bottom could be rate determining

Concat: O(p)
Biggest work done on the copy tree side which depends on the depth of the rope to copy.
Actual concatenation is of O(1), all we are doing is repointing left, right, and root.


TESTING & DEBUGGING STRATEGY: 
Briefly describe the tests in your StudentTests() function.
How did you test the "corner cases" of your implementation?

Test Case 1: Creating ropes out of large strings
Test Case 2: Splitting large string ropes to check for unbalanced ropes
Test Case 3: Splitting with rhs 0 length rope
Test Case 4: Splitting with an invalid index
Test Case 5: Splitting the same rope into varying left and right length ropes to check for one character, two character lopsided ropes
Test Case 6: Concatinating ropes of large strings

Debugging Strategy: 
Use gdb heavily to watch left, right, parent node pointers, specifically for deconstructor.
Edge cases were tested using a combination of gdb the rope print functions an standard std:cout.
Memory problems with concat function; realised had to copy instead of pointer assignment.

					

MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

