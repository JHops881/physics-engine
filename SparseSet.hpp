#pragma once
#include <vector>
#include <limits>
#include <stdexcept>
#include <string>
#include <format>

typedef unsigned int U32;
constexpr U32 INVALID_DENSE_INDEX = std::numeric_limits<U32>::max();
constexpr U32 INVALID_ID          = std::numeric_limits<U32>::max();

template<typename T>
class SparseSet {
private:
    /**
     * @brief A dynamic block of memory that stores the objects that we care 
     * about. i.e. The ones that the sparse set was created to store. 
     */
    std::vector<T> dense = {}; 

    /**
     * @brief This stores the IDs that are associated with the objects 
     * stored in dense. We can think of this as a reverse lookup table. So,
     * for any object stored in the dense array, it's associated ID is 
     * stored at the same index in this array.
     * 
     * Like so:
     * indices     ->   0           1           2           3 ...
     * this array  -> | objA's ID | objB's ID | objC's ID | ...
     * dense array -> | objA      | objB      | objC      | ...
     */
    std::vector<U32> associated_ids = {};

    /**
     * @brief Maps an ID (an index in this array) to location (an index in the
     * dense array of the object associated with that ID) For instance:
     * ID       ->   0  | 1  | 2  | 3  | 4  | 5  | 6  | ...
     * location ->   6  | 23 | 16 | 18 | 17 | 49 | 4  | ...
     * This allows us to have an ID (an index in this array) always map to the
     * same object in the dense array even if that objet gets moved around,
     * because--in that case--we just change the location.
     */
    std::vector<U32> sparse = {};

    /**
     * @brief A temporary 
     * 
     */
    std::vector<U32> free_ids = {}; // stores retired entity ids that can be reused

    U32 next_id = 0;

public:    
    /**
     * @brief Add an object in to the sparse set.
     * 
     * @param object The object that you want to add into the sparse set.
     * @return U32 A unique ID for the object in the sparse set.
     */
    U32 add(const T& object) {
        // We will give this back to the user once we store the object.
        U32 id = 0;
        // Check to see if there are free IDs hanging around to scoop up.
        if (free_ids.empty()) { 
            // Case: None free.
            // Take the next ID available on the counter.
            id = next_id;
            next_id++;
        } else {
            // Case: One of more free. Take the first.
            id = free_ids.back();
            free_ids.pop_back();
        }
    #ifdef _DEBUG
        if (sparse[id] != INVALID_DENSE_INDEX) {
            std::string msg = "SparseSet::add() failed. ID already exists.";
            throw std::runtime_error(msg);
        }
    #endif
        // Put the object in the dense and grab its location (dense index)
        U32 dense_index = dense.size();
        dense.push_back(object);

        // Store the dense location / associate it to the ID.
        sparse[id] = dense_index;
        associated_ids.push_back(id);

        return id;
    }

    /**
     * @brief Get a reference to an object stored in the sparse set.
     * 
     * @param id The ID of the object.
     * @return T& A reference to the object.
     */
    T& get(U32 id) {
    #ifdef _DEBUG
        if (id > sparse.size()) {
            std::string msg = "SparseSet::get() failed. ID is out of range.";
            throw std::runtime_error(msg);
        }
        if (sparse[id] == INVALID_DENSE_INDEX) {
            std::string msg = std::format(
                "SparseSet::get() failed. Nothing exists at this ID -> {}", id);
            throw std::runtime_error(msg);
        }
    #endif
        size_t dense_index = sparse[id];
        return dense[dense_index];
    }

    /**
     * @brief Set the value associated with an ID.
     * 
     * @param id The ID of the object/value that you want to set.
     * @param object The new object/value associated with the ID.
     */
    void set(U32 id, const T& object) {
    #ifdef _DEBUG
        // Make sure that the ID has been initialized.
        if (id > sparse.size()) {
            std::string msg = "SparseSet::set() failed. ID not valid.";
            throw std::runtime_error(msg);
        }
    #endif
        // Who cares? Just assign the ID to the object--no checks needed.
        size_t dense_index = sparse[id];
        dense[dense_index] = object;
    }

    /**
     * @brief Remove an object from the sparse set.
     * 
     * @param id The ID of the object that you want to remove.
     */
    void remove(U32 id) {
    #ifdef _DEBUG
        if (id > sparse.size()) {
            std::string msg = "SparseSet::remove() failed. ID is out of range.";
            throw std::runtime_error(msg);
        }
        if (sparse[id] == INVALID_DENSE_INDEX) {
            std::string msg =
                "SparseSet::remove() failed. Nothing exists at this ID.";
            throw std::runtime_error(msg);
        }
    #endif 
        // Perfrom a swap-and-pop.
        
        // Get the index of the object in the dense array.
        size_t index_to_delete = sparse[id];

        // Get the object and ID of the last object in the dense array
        T last_object = dense.back();
        U32 last_associated_id = associated_ids.back();

        dense[index_to_delete] = last_object;
        associated_ids[index_to_delete] = last_associated_id;
        sparse[last_associated_id] = index_to_delete;

        sparse[id] = INVALID_DENSE_INDEX;

        associated_ids.pop_back();
        dense.pop_back();
        free_ids.push_back(id);
    }

    U32 get_associated_handle(size_t dense_index) const {
    #ifdef _DEBUG
        if (dense_index > dense.size() or dense.size() != associated_ids.size()) {
            std::string message = std::string("SparseSet::GetAssociatedHandle() failed. Dense index may be out of range.");
            throw std::runtime_error(message);
        }
    #endif
        return associated_ids[dense_index];
    }

    std::vector<T>& get_dense() {
        return dense;
    }

    bool has(U32 handle) const {
        return (handle < sparse.size() and sparse[handle] != INVALID_DENSE_INDEX);
    }
};