#ifndef __LINK_H__
#define __LINK_H__

/**
 * @brief Class with the link information.
 *
 * This class represents to each link in the physical network.
 *
 */

class Link {
 public:
  /**
   * @brief Construct a new Link object
   *
   */
  Link(void);
  /**
   * @brief Construct a new Link object
   *
   * @param id The id of this new Link object
   */
  Link(int id);
  /**
   * @brief Destroy the Link object
   *
   */
  ~Link();
  /**
   * @brief Set the Id object
   *
   * @param id The new Id of this Link object
   */
  void setId(int id);
  /**
   * @brief Get the Id object
   *
   * @return int The Id of this Link object
   */
  int getId(void);

 private:
  int id;
};

#endif