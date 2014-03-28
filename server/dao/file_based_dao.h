/* ========================================================================= 
 * BusTrack Bus Tracking System
 * CS3249 User Interface Development
 * =========================================================================
 * Copyright 2014 Ko Wan Ling, Terence Then, Kek Yan Rong and Wong Yong Jie
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================================= */

#ifndef BUSTRACK_FILE_BASED_DAO_H_
#define BUSTRACK_FILE_BASED_DAO_H_

#include <fstream>
#include <memory>
#include <unordered_map>

#include <QDebug>
#include <QFile>
#include <QDir>

#include "data_access_object.h"

namespace bustrack {

  template <typename T>
  class FileBasedDAO : public DataAccessObject {
  public:
    /**
     * Constructs a new instance of a file-based data access object.
     * The data directory indicates the directory to look for the data file.
     *
     * @param data_dir A QDir object that points to a data directory.
     */
    FileBasedDAO(const QDir& data_dir, const QString& data_file_name) :
        data_dir_(data_dir) {
      data_file_path_ = data_dir_.filePath(data_file_name);
    };

    /**
     * Remove the default constructor.
     */
    FileBasedDAO() = delete;

    /**
     * Create a virtual destructor in case someone wants to inherit.
     */
    virtual ~FileBasedDAO() = default;

    /**
     * Saves the internal list of items to disk.
     */
    virtual void commit() = 0;

    /**
     * Restore the internal list of items from disk.
     */
    virtual void rollback() = 0;

    /**
     * Returns a vector of the items.
     */
    std::vector<T> getItems() const {
      std::vector<T> items;
      for (std::pair<std::string, T> item_pair : items_) {
        T item = item_pair.second;
        items.push_back(item);
      }

      return items;
    };

    /**
     * Returns an instance of the item given the ID.
     * If an item of the given ID does not exist, an exception is thrown.
     */
    T getItem(const std::string& id) const {
      return items_.at(id);
    };

    /**
     * Returns true if an item of the given ID exists, false otherwise.
     */
    bool itemExists(const std::string& id) const {
      return items_.find(id) != items_.end();
    };

    /**
     * Creates an item using the specified instance of the class.
     * Returns false if an item of the given ID already exists, true otherwise.
     *
     * @param item The item to be created.
     */
    bool createItem(T item) {
      if (itemExists(item.getId())) {
        return false;
      }

      items_.insert(std::make_pair(item.getId(), item));
      return true;
    };

    /**
     * Replaces an existing item using the specified instance of the class.
     * Returns false if an existing item does not exist, true otherwise.
     *
     * @param item The item to be replaced.
     */
    bool replaceItem(T item) {
      if (!itemExists(item.getId())) {
        return false;
      }

      items_.insert(std::make_pair(item.getId(), item));
      return true;
    };

    /**
     * Removes an existing item given by the ID.
     * Returns true if the item could be found, false otherwise.
     *
     * @param id The ID of the item to be removed.
     */
    bool removeItem(const std::string& id) {
      if (!itemExists(id)) {
        return false;
      }

      items_.erase(items_.find(id));
      return true;
    };

  protected:
    QDir data_dir_;
    QString data_file_path_;
    std::unordered_map<std::string, T> items_;

    /**
     * Prepares a commit transaction.
     * @return An open file stream upon success, nullptr on failure.
     */
    std::shared_ptr<std::fstream> prepareCommit() {
      if (commit_stream_ != nullptr) {
        qCritical() << "The last commit stream was not closed properly!";
        return nullptr;
      }

      // Create a new transaction stream.
      commit_stream_ = std::make_shared<std::fstream> (
          data_file_path_.toStdString(), std::ios::out);
      if (commit_stream_->fail()) {
        qWarning() << "Failed to prepare commit transaction: " << 
          data_file_path_;
        commit_stream_.reset();
        return nullptr;
      }

      return commit_stream_;
    };

    /**
     * Closes a commit transaction.
     */
    void closeCommit() {
      if (commit_stream_ == nullptr) {
        qCritical() << "Failed to close the commit stream!";
        return;
      }

      // Close all resources.
      commit_stream_->close();
      commit_stream_.reset();
    }

    /**
     * Prepares a rollback transaction.
     */
    std::shared_ptr<std::fstream> prepareRollback() {
      if (rollback_stream_ != nullptr) {
        qCritical() << "The last rollback stream was not closed properly!";
        return nullptr;
      }

      // Check if the data file exists.
      if (!QFile::exists(data_file_path_)) {
        return nullptr;
      }

      rollback_stream_ = std::make_shared<std::fstream> (
          data_file_path_.toStdString(), std::ios::in);
      if (rollback_stream_->fail()) {
        qCritical() << "Failed to open the rollback stream!";
        rollback_stream_.reset();
        return nullptr;
      }

      items_.clear();
      return rollback_stream_;
    }

    /**
     * Closes a rollback transaction.
     */
    void closeRollback() {
      if (rollback_stream_ == nullptr) {
        qCritical() << "Failed to close the commit stream!";
        return;
      }

      // Close all resources.
      rollback_stream_->close();
      rollback_stream_.reset();
    }

  private:
    std::shared_ptr<std::fstream> commit_stream_;
    std::shared_ptr<std::fstream> rollback_stream_;
  };

}

#endif /* BUSTRACK_FILE_BASED_DAO_H_ */

/* vim: set ts=2 sw=2 et: */
