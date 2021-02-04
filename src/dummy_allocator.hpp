#ifndef __DUMMY_ALLOCATOR_H__
#define __DUMMY_ALLOCATOR_H__

#include "allocator.hpp"

/**
 * @brief Class "DummyAllocator" which extends class father "Allocator".
 */

class DummyAllocator : public Allocator {
 public:
  /**
   * @brief Constructs a new DummyAllocator object.
   */
  DummyAllocator();
  /**
   * @brief Constructs a new DummyAllocator object and initialize the network.
   *
   * @param network pointer type network representing the network of nodes.
   */
  DummyAllocator(Network *network);
  /**
   * @brief Delete the object DummyAllocator.
   */
  ~DummyAllocator();
  /**
   * @brief
   *
   * @param src
   * @param dst
   * @param bitRate
   * @param con
   *
   * @return
   */
  allocationStatus exec(int src, int dst, BitRate bitRate, Connection &con);
};

#endif