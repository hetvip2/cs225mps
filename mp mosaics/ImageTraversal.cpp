#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "Point.h"

#include "ImageTraversal.h"

namespace Traversals {
  /**
  * Calculates a metric for the difference between two pixels, used to
  * calculate if a pixel is within a tolerance.
  *
  * @param p1 First pixel
  * @param p2 Second pixel
  * @return the difference between two HSLAPixels
  */
  double calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
    double h = fabs(p1.h - p2.h);
    double s = p1.s - p2.s;
    double l = p1.l - p2.l;

    // Handle the case where we found the bigger angle between two hues:
    if (h > 180) { h = 360 - h; }
    h /= 360;

    return sqrt((h*h) + (s*s) + (l*l));
  }
  
  /**
  * Adds a Point for the bfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void bfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Adds a Point for the dfs traversal to visit at some point in the future.
  * @param work_list the deque storing a list of points to be processed
  * @param point the point to be added
  */
  void dfs_add(std::deque<Point> & work_list, const Point & point) {
    /** @todo [Part 1] */
    work_list.push_back(point);
  }

  /**
  * Removes the current Point in the bfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void bfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
      if (!work_list.empty()) {
        work_list.pop_front();
    }
    
  }

  /**
  * Removes the current Point in the dfs traversal
  * @param work_list the deque storing a list of points to be processed
  */
  void dfs_pop(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
      if (!work_list.empty()) {
        work_list.pop_back();
    }
  }

  /**
   * Returns the current Point in the bfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point bfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
      return work_list.front();
    }
    return Point(0, 0);
  }

  /**
   * Returns the current Point in the dfs traversal
   * @param work_list the deque storing a list of points to be processed
   * @return the current Point 
   */
  Point dfs_peek(std::deque<Point> & work_list) {
    /** @todo [Part 1] */
    if (!work_list.empty()) {
      return work_list.back();
    }
    return Point(0, 0);
  }

ImageTraversal::Iterator::Iterator(PNG png, const Point & start, double tolerance, TraversalFunctions fns) : png_(png) {
      //png_ = png;
      current_ = start;
      tolerance_ = tolerance;
      fns_ = fns;


      startPixel_ = png.getPixel(start.x, start.y);
      work_list_.push_back(start);
      visited_.insert(encodePoint(start));
    }
  /**
  * Initializes a ImageTraversal on a given `png` image,
  * starting at `start`, and with a given `tolerance`.
  * @param png The image this traversal is going to traverse
  * @param start The start point of this traversal
  * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
  * it will not be included in this traversal
  * @param fns the set of functions describing a traversal's operation
  */
  ImageTraversal::ImageTraversal(const PNG & png, const Point & start, double tolerance, TraversalFunctions fns) {  
    /** @todo [Part 1] */
    png_ = png;
    start_ = start;
    tolerance_ = tolerance;
    fns_ = fns;


  }

  /**
  * Returns an iterator for the traversal starting at the first point.
  */
  ImageTraversal::Iterator ImageTraversal::begin() {
    /** @todo [Part 1] */

      return ImageTraversal::Iterator(png_, start_, tolerance_, fns_);

  }

  /**
  * Returns an iterator for the traversal one past the end of the traversal.
  */
  ImageTraversal::Iterator ImageTraversal::end() {
    /** @todo [Part 1] */
    return ImageTraversal::Iterator();
  }

  /**
  * Default iterator constructor.
  */
  ImageTraversal::Iterator::Iterator() {
    /** @todo [Part 1] */
    //traversal_ = nullptr;
    //png_ = nullptr;
    current_ = Point(0, 0);
  }

ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
    /** @todo [Part 1] */
      if (work_list_.empty()) {
        //png_ = nullptr;
        return *this;
    }
    fns_.pop(work_list_);
    visited_.insert(encodePoint(current_));

    std::vector<Point> neighbors = {
        Point(current_.x + 1, current_.y), // right
        Point(current_.x, current_.y + 1), // down
        Point(current_.x - 1, current_.y), // left
        Point(current_.x, current_.y - 1)  // up
    };

    for (const Point &neighbor : neighbors) {
        if (neighbor.x < png_.width() && neighbor.y < png_.height()) {
            unsigned long encodedPoint = encodePoint(neighbor);

            if (visited_.find(encodedPoint) == visited_.end()) {
                double delta = calculateDelta(startPixel_, png_.getPixel(neighbor.x, neighbor.y));

                if (delta <= tolerance_) {
                    fns_.add(work_list_, neighbor);
                }
            }
        }
    }

    // find unvisited
    while (!work_list_.empty()) {
        Point next = fns_.peek(work_list_);
        unsigned long encodedNext = encodePoint(next);

        // if visited then remove
        if (visited_.find(encodedNext) != visited_.end()) {
            fns_.pop(work_list_);
        } else {
            //curr to next visited
            current_ = next;
            break;
        }
    }

    // if worklist empty end traversal
    if (work_list_.empty()) {
        traversal_ = nullptr;
    }

    return *this;
}

  /**
  * Iterator accessor operator.
  *
  * Accesses the current Point in the ImageTraversal.
  */
  Point ImageTraversal::Iterator::operator*() {
    /** @todo [Part 1] */
 f   //return Point(0, 0);
    return current_;
  }

  /**
  * Iterator inequality operator.
  *
  * Determines if two iterators are not equal.
  */
  bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
    /** @todo [Part 1] */
    return !work_list_.empty() != !other.work_list_.empty();
  }

  /**
   * Iterator size function.
   *
   * @return size_t the size of the iterator work queue.
   */
  size_t ImageTraversal::Iterator::size() const {
    // if (traversal_) {
    //   return work_list_.size();
    // } else {
    //     return 0;
    // }
    return work_list_.size();
  }

  /**
   * Iterator empty function.
   *
   * @return bool whether the iterator work queue is empty.
   */
  bool ImageTraversal::Iterator::empty() const {
    // if (traversal_) {
    //   return work_list_.empty();
    // } else {
    //     return true;
    // }
    return work_list_.empty();
  }
//converts 2D point into a long value
unsigned long ImageTraversal::Iterator::encodePoint(const Point& p) const {
    return static_cast<unsigned long>(p.x) * 100000 + p.y;
  }
}
