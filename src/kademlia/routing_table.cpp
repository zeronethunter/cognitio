//  Copyright (c) 2022 NodeOps
//
//  Distributed under the GNU GPLv3 software license, see the accompanying
//  file LICENSE or visit <https://www.gnu.org/licenses/gpl-3.0.en.html>

#include "kademlia/routing_table.hpp"

#include <mutex>

namespace cognitio {
namespace kademlia {

RoutingTable::RoutingTable(const Identifier& ownerID) : ownerId_(ownerID) {
  for (Identifier::numeric_type i = 0; i <= GUID_SPACE; i++) {
    buckets_.push_back(Bucket(i));
  }
}

void RoutingTable::update(const ConnectionInfo& connection_info) {
  std::lock_guard<std::mutex> lck(locker_);
  Identifier::numeric_type connectionBucketIndex =
      ownerId_.bucketOf(connection_info.id());

  Bucket& targetBucket = buckets_[(int)connectionBucketIndex];

  if (targetBucket.contains(connection_info)) {
    targetBucket.pushToFront(connection_info.id());
  } else {
    targetBucket.add(connection_info);
  }
}

void RoutingTable::remove(const ConnectionInfo& connection_info) {
  std::lock_guard<std::mutex> lck(locker_);
  Identifier::numeric_type connectionBucketIndex =
      ownerId_.bucketOf(connection_info.id());

  Bucket& targetBucket = buckets_[(int)connectionBucketIndex];
  targetBucket.remove(connection_info);
}

FindNodeAnswer RoutingTable::findClosest(const Identifier& destinationId) {
  FindNodeAnswer answer(destinationId);

  Identifier::numeric_type connectionBucketIndex =
      ownerId_.bucketOf(destinationId);
  Bucket& targetBucket = buckets_[(int)connectionBucketIndex];

  for (const auto& id : targetBucket.getNodeIds()) {
    if (id != ownerId_) {
      answer.add(targetBucket.get(id));
    }
  }

  int bucketId = (int)targetBucket.getId().value();
  for (int i = 1; answer.size() < K_SIZE &&
                  ((bucketId - i) >= 0 || (bucketId + i) < buckets_.size());
       i++) {
    // Check the previous buckets
    if (bucketId - i >= 0) {
      Bucket& prev = buckets_[bucketId - i];

      for (const auto& id : prev.getNodeIds()) {
        answer.add(prev.get(id));
      }
    }
    // Check the next buckets
    if (bucketId + i < buckets_.size()) {
      Bucket& next = buckets_[(int)(bucketId + i)];

      for (const auto& id : next.getNodeIds()) {
        answer.add(next.get(id));
      }
    }
  }

  answer.prepare();
  return answer;
}

std::string RoutingTable::toString() const {
  std::string result;

  result = "[OwnerID=" + (std::string)ownerId_ + " ";
  for (int i = 0; i < buckets_.size(); i++) {
    result += buckets_[i].toString() + " ";
  }

  result += "]";
  return result;
}

}  // namespace kademlia
}  // namespace cognitio
