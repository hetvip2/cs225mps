/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>
#include <stack>

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] == second[curDim]) {
        return first < second;
    }
    return first[curDim] < second[curDim];
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentp,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    double dist = 0;
    double pdist = 0;
    
    for (int i = 0; i < Dim; i++) {
        int currval = (target[i] - currentp[i]) * (target[i] - currentp[i]);
        int pval = (target[i] - potential[i]) * (target[i] - potential[i]);
        
        dist += currval;
        pdist += pval;
    }

    if (pdist < dist) {
        return true;
    }

    if (pdist == dist) {
        bool ret = potential < currentp;
        return ret;
    }

    return false;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    root = nullptr;
    points = newPoints;
    int s = points.size() - 1;
    root = treeHelper(0, s, 0);
}

// kdtree helper
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::treeHelper(int left, int right, int depth)
{
    if (left > right) {
        return nullptr;
    } 
    int split = depth % Dim;
    int median = (left + right) / 2;

    auto leftadd = points.begin() + left;
    auto rightadd = points.begin() + right + 1;
    auto midadd = points.begin() + median;

    select(leftadd, rightadd, midadd, 
           [split](const Point<Dim>& one, const Point<Dim>& two) {
               return smallerDimVal(one, two, split);
           });

    KDTreeNode* node = new KDTreeNode(points[median]);
    
    node->left = treeHelper(left, median - 1, depth + 1);
    node->right = treeHelper(median + 1, right, depth + 1);

    return node;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
    if (other.root == nullptr) {
        root = nullptr;
        return;
    }
    root = copier(other.root);
}
// helper for copying
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copier(KDTreeNode* node) {
    if (node == nullptr) {
        return nullptr;
    }
    KDTreeNode* newNode = new KDTreeNode(node->point);

    newNode->left = copier(node->left);
    newNode->right = copier(node->right);

    return newNode;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
    if (this != &rhs) {
        destroy(root);
        root = copier(rhs.root);
    }
    return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
    destroy(root);
}

//destructor helper
template <int Dim>
void KDTree<Dim>::destroy(KDTreeNode* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    return findNearestNeighbor(root, query, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(KDTreeNode* node, const Point<Dim>& query, int depth) const {
    
    if (node == nullptr) {
        return Point<Dim>();
    }
    
    int currdim = depth % Dim;
    KDTreeNode* movenext = nullptr;
    KDTreeNode* opp = nullptr;

    if (query[currdim] < node->point[currdim]) {
        movenext = node->left;
        opp = node->right;
    } else {
        movenext = node->right;
        opp = node->left;
    }

    Point<Dim> pt = findNearestNeighbor(movenext, query, depth + 1);

    if (closest(query, node->point, pt)) {
        pt = node->point;
    }

    double distsquare = squaredD(query, pt);
    double dist = (query[currdim] - node->point[currdim]) * (query[currdim] - node->point[currdim]);
    if (opp != nullptr) {
        if (dist <= distsquare){
        Point<Dim> temp = findNearestNeighbor(opp, query, depth + 1);
        if (closest(query, temp, pt)) {
            pt = temp;
        }
        }
    }
    return pt;
}
// helper for handling nearest distance
template <int Dim>
bool KDTree<Dim>::closest(const Point<Dim>& target, const Point<Dim>& p1, const Point<Dim>& p2) const {
    double dist1 = squaredD(target, p1);
    double dist2 = squaredD(target, p2);
    
    if (dist1 < dist2) {
        return true;
    } else if (dist1 > dist2) {
        return false;
    }
    for (int i = 0; i < Dim; i++) {
        if (p1[i] < p2[i]){
            return true;
        }
        if (p1[i] > p2[i]){
            return false;
        }
    }
    return false;
}
//helper for calculating squared distance
template <int Dim>
double KDTree<Dim>::squaredD(const Point<Dim>& p1, const Point<Dim>& p2) const {
    double dist = 0.0;
    for (int i = 0; i < Dim; i++) {
        double diff = p1[i] - p2[i];
        dist += diff * diff;
    }
    return dist;
}



// template <typename RandIter, typename Comparator>
// void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
// {
//     /**
//      * @todo Implement this function!
//      */ 
//     while (start < end - 1) {
//         RandIter pivot = end - 1;
//         RandIter i = start;

//         for (RandIter j = start; j < pivot; j++) {
//             if (cmp(*j, *pivot)) {
//                 std::iter_swap(i, j);
//                 i++;
//             }
//         }
//         std::iter_swap(i, pivot);

//         if (i == k) {
//             return;
//         } else if (k < i) {
//             end = i;
//         } else {
//             start = i + 1;
//         }
//     } 
// }
template <typename RandIter, typename Comparator>
RandIter partition(RandIter begin, RandIter end, RandIter pivotIndex, Comparator cmp) {
    auto pivotValue = *pivotIndex;
    std::iter_swap(pivotIndex, end - 1);  // Move pivot to the end
    RandIter storeIndex = begin;

    // Iterate from `begin` to `end - 1`
    for (RandIter i = begin; i < end - 1; ++i) {
        if (cmp(*i, pivotValue)) {  // Compare using the provided comparator
            std::iter_swap(storeIndex, i);  // Swap elements
            ++storeIndex;
        }
    }

    std::iter_swap(storeIndex, end - 1);  // Move pivot to its final position
    return storeIndex;
}

template <typename RandIter, typename Comparator>
void select(RandIter begin, RandIter end, RandIter k, Comparator cmp) {
    while (begin != end) {
        // Choose pivotIndex as the middle of the range
        RandIter pivotIndex = begin + (end - begin) / 2;

        // Partition the array and get the final position of the pivot
        RandIter pivotFinal = partition(begin, end, pivotIndex, cmp);

        if (pivotFinal == k) {
            return;  // We have found the k-th element
        } else if (k < pivotFinal) {
            end = pivotFinal;  // Search in the left partition
        } else {
            begin = pivotFinal + 1;  // Search in the right partition
        }
    }
}

