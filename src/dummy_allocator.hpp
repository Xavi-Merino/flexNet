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
   * @brief The method to allocate resources. It must be implemented on an
   * inherited class.
   *
   * @param src start node, type integer.
   * @param dst destiny node, type integer.
   * @param bitRate the bit rate of the connection. param typer BitRare.
   * @param con the Connection object. It's passed by reference.
   *
   * @return param type allocationStatus what represents if it is ALLOCATED,
   * NOT_ALLOCATED, N_A (not assigned )
   */
  allocationStatus exec(int src, int dst, BitRate bitRate, Connection &con);
};

#endif