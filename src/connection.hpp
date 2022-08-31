#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <vector>

#include "bitrate.hpp"
/**
 * @brief Class with the connection information.
 *
 * This class contains the information regarding the connections that are made
 * between the nodes on a network during the allocation process.
 *
 */
class Connection {
 public:
  /**
   * @brief Constructs a new Connection object. It assigns it the Id passed as a
   * parameter, and creates a links vector and a slots vector.
   *
   * @param id the id of the new connection object.
   */
  Connection(long long id, double time, BitRate *bitRate);
  /**
   * @brief Destroys the Connection object.
   *
   */
  ~Connection();
  /**
   * @brief Adds a new link to the Connection object. The link id is added to
   * the links vector, and the slots to the slots vector.
   *
   * @param idLink the id of the new link added to the connection object.
   * @param slots the vector that contains the position of the slots.
   */
  void addLink(int idLink, std::vector<int> slots);
  /**
   * @brief Adds a new link to the Connection object. The link id is added to
   * the links vector, and the slots in the range fromSlot-toSlot are
   * added to the slots vector.
   *
   * @param idLink the id of the new link added to the connection object.
   * @param fromSlot the position of the first slot to be taken on the link.
   * @param toSlot the position of the last slot to be taken on the link.
   */
  void addLink(int idLink, int fromSlot, int toSlot);

  std::vector<int> getLinks(void);
  std::vector<std::vector<int> > getSlots(void);
  double getTimeConnection(void);
  BitRate *getBitrate(void);
  long long getId(void);

 private:
  long long id;
  double timeConnection;
  BitRate *bitRate;
  std::vector<int> links;
  std::vector<std::vector<int> > slots;

  friend class Controller;
};
#endif